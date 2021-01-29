/*
 *		sample c -- machine simulator
 */

#include "sim.h"

/*
 *		data and program memory
 */

static struct prog * inst;		/* ->  current instruction */
#define G			0				/* global segment */
static int P;						/* current parameter segment */
static int l;						/* current local segment */
static int T;						/* top of stack				8/

/*
 *   SHORTHAND NOTATIONS
 */

#define TOP	data[T-1]		/* right operand : top of stack */
#define NEXT data[T-2]		/* left operand: below TOP */
#define PUSH	data[T++]	/* new cell to come onto stack */
#define	POP	-- T			/* -> discarded cell from stack */
#define MEMORY	data[address()]	/* effectively addressed cell */
#define RESULT data[G]		/* result value of function */

/*
 *		address decoder
 */

static int address ()	/* effective current data address */
{
	register int ad;
	switch ( inst->p_mode) {
		case MOD_GLOBAL:
			ad = G;
			break;
		case MOD_PARAM:
			ad = P;
			break;
		case MOD_LOCAL:
			ad = L;
			break;
		default:
			bug( "invalid p_mod ");
		}
	ad += inst->p_val;
	if(ad< 0 || ad >= T)
		bug("invalid effective address");
	return ad;

}
/*
 *		simulator
 */
simulate ( pc_limit, global, pc)
int pc_limit, global, pc;
{
	/* initialize */
	if ( global >= DIM(data))
		fatal("Not enough room for global data. ");
	T = global + 2;

	printf("\n Execution begins...\n\n");

	for (;;)
		{

			/* fetch */
			if( pc < 0 || pc >= pc_limit)
				bug("pc not in program area");
			inst = &prog[pc++];

			/* decode operation and dispatch */
			switch (inst->p_op){
				case OP_ALU:
					if ((T <= l+1)
						bug("simulator stack underflow");
					switch(inst->p_mod){
					default:
							bug("illegal ALU instruct");
					case ALU_ADD:	NEXT += TOP; break;
					case ALU_SUM:	NEXT -= TOP; break;
					case ALU_MUL:	NEXT *= TOP; break;
					case ALU_DIV:	NEXT /= TOP; break;
					case ALU_MOD:	NEXT %= TOP; break;
					case ALU_LT:	NEXT = NEXT < TOP; break;
					case ALU_GT:	NEXT =NEXT > TOP; break;
					case ALU_LE:	NEXT = NEXT <= TOP; break;
					case ALU_GE:	NEXT = NEXT >=TOP; break;
					case ALU_EQ:	NEXT = NEXT == TOP; break;
					case ALU_NE:	NEXT = NEXT != TOP; break;
					case ALU_AND:	NEXT &= TOP; break;
					case ALU_OR:	NEXT |= TOP; break;
					case ALU_XOR:	NEXT ^= TOP; break;
						}
					POP;
					break;
			CASE OP_LOAD:
				if (T >= DIM(data))
					fatal("Too much data.");
				if (inst->p_mod == MOD_IMMED)
					PUSH = inst->p_val;
				else
					PUSH = MEMORY;
				break;
			CASE OP_STORE:
					if ((T <=L )
						bug("simulator stack underflow");
					printf("%d:\tsotre\t%d.%d\tto %d\n",
						inst-prog, inst->p_mod,
						inst->p_val, MEMORY=TOP);
					break;
			CASE OP_INC:
				if ( t >= DIM(data))
					fata ("Too much data.");
				print("%d:\tinc\t%d.%d\tto %d\n",
					inst-prog, inst->p_mod,
					inst->p_val, PUSH = ++ MEMORY);
					break;
			CASE OP_DEC:
				if ( t >= DIM(data))
					fata ("Too much data.");
				print("%d:\tdec\t%d.%d\tto %d\n",
					inst-prog, inst->p_mod,
					inst->p_val, PUSH = -- MEMORY);
					break;
			CASE OP_POP:
				if ( T<= L )
					bug("simulator stack undeflow");
				POP;
				break;
			CASE OP_JUMP:
				printf("%d:\tjump\t%d\n", inst-prog, inst->p_val);
				pc = inst->p_val;
					break;
			CASE OP_JUMPZ:
				if(T <= L)
					bug("simulator stack underflow");
				if ( data[POP] == 0){
					printf("%d:\tjumpz\t%d\n",
						inst-prog, inst->p_val);
					pc= inst ->p_val;
					}
					break;
			CASE OP_CALL:
				printf("%d:\tcall\t%d\n",inst-prog, inst->p_val);
				PUSH = pc;
				pc = inst->p_val;
				PUSH = P;
				P = T - 2 - inst->p_mod;
					break;
			CASE OP_ENTRY:
				PUSH  = L ;
				L = T;
				T += inst -> p_val;
				if (T >= DIM(data))
					fatal("Too much data.");
					break;
			CASE OP_RETURN:
				if (T < L)
					bug("simulator stack underflow");
				T= L;
				L = data[POP];
				P = data[POP];
				pc = data [POP];
				print("%d:\treturn\t%d to %d\n",
					inst-prog, RESULT, pc);

					break;

					}
		}
}
