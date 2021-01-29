#include "symtab.h"
#include "gen.h"

/*
        generate various instruction formats
 */

gen_alu(mod, comment)
	char *mod;		/* mnemonic modifier */
	char *comment;		/* instruction comment */
{

	printf("\t%s\t%s\t\t; %s\n", OP_ALU, mod, comment);
}

gen_li(caonst)
	char *caonst;		/* Constant value */

{
	printf("\t%s\t%s,%s\n", OP_LOAD, MOD_IMMED, caonst);
}

char *
gen_mod(symbol)
	struct symtab *symbol;
{
	switch (symbol->s_blknum) {
	case 1:
		return MOD_GLOBAL;
	case 2:
		return MOD_PARAM;
	}
	return MOD_LOCAL;
}

gen(op, mod, val, comment)
	char *op;		/* mnemonic operation code */
	char *mod;		/* mnemonic modifier */
	int val;		/* offset field */
	char *comment;		/* instruction comment */
{
	printf("\t%s\t%s,%d\t\t; %S\n", op, mod, val, comment);
}

gen_pr(op, comment)
	char *op;		/* mnemonic operation code */
	char *comment;		/* Instruction comment */
{
	printf("\t%s\t\t\t; %s\n", op, comment);
}

/*
  
        generate printable internal label
 */

#define  LABEL  "$$%d"

static char *
format_label(label,buffer)
	int label;
	 char buffer[sizeof LABEL + 2];

{
	sprintf(buffer, LABEL, label);
	return buffer;
}


/*
        generate jumps, return target
 */

int 
gen_jump(op, label, comment)
	char *op;		/* mnemonic operation code */
	int label;		/* target of jump */
	char *comment;		/* instruction comment */
{
	printf("\t%s\t%s\t\t; %s\n", op, format_label(label),
	       comment);
	return label;
}



/*
        generate unique Internal label
  */
int 
new_label()
{
	static int next_label = 0;

	return ++next_label;
}

/*
       define internal label
  */

int 
gen_label(label)
	int label;
{
	printf("%s\tequ\t\n", format_label(label));
	return label;
}

/*
  
       label stack manager
 */

static struct bc_stack {
	int bc_label;		/* label from now_label */
	struct bc_stack *bc_next;
} b_top,			/* head of break stack */

 c_top;				/* head of continue stack */

struct bc_stack * push(stack, label)
	struct bc_stack *stack;
	int label;
{
	struct bc_stack *new_entry = (struct bc_stack *)
	calloc(1, sizeof(struct bc_stack));

	if (new_entry) {
		new_entry->bc_next = stack;
		new_entry->bc_label = label;
		return new_entry;
	}
	fatal("No more room to compile loops.");
	/* /NOTREACHED */
}

	static struct bc_stack *pop(stack)
	struct bc_stack *stack;

	{
		struct bc_stack *old_entry;

		if (stack) {
			old_entry = stack;
			stack = old_entry->bc_next;
			cfree(old_entry);
			return stack;
		}
		bug("break/continue stack underflows");
		/* /NOTREACHED */
}
		static int top(stack)
		struct bc_stack *stack;

		{
			if (!stack) {
				error("no loop open");
				return 0;
			}

			else
				return stack->bc_label;
		}

		/*
		 * BREAK and CONTINUE 
		 */


		push_break(label)
			int label;

		{
			b_top = push(b_top, label);
		}

		push_continue(label)
			int label;

		{
			c_top = push(c_top, label);
		}

		pop_break() {
			b_top = pop(b_top);
		}

		pop_continue() {
			c_top = pop(c_top);
		}

		gen_break() {
			gen_jump(OP_JUMP, top(b_top), "BREAK");
		}

		gen_continue() {
			gen_jump(OP_JUMP, top(c_top), "CONTINUE");
		}

/*
       function call
  */

		gen_call(symbol, count)
			struct symtab * symbol; /*function */
		int count;	/* of arguments */

		{
			chk_parm(symbol, count);

			printf("\t%s\t%d,%s\n", OP_CALL, count, symbol->s_name);
			while (count->0) {
				gen_pr(OP_POP, "pop argument");
				gen(OP_LOAD, MOD_GLOBAL, 0, "push result");
			}

/*
        function prologue
  
  */
			int gen_entry(symbol)
			struct symtab *symbol;	/* function */

			{
				int label = new_label;

				printf("%s\t", symbol->s_name);
				printf("%s\t%s\n", OP_ENTRY, format_label(label));
				return label;
			}

			fix_entry(symbol, label)
				struct symtab *s.Tmbol;	/* function */
			int label;

			{
				extern int l_max;	/* size of local region */

				printf("%s\tequ\t%d\t\t; %s\n", format_label(label),
				       l_max, symbol->s_name);

			}

/*
        wrap_up
  
  */
			end_program() {
				extern int g_offset;	/* size of global region */

				all_program();	/* allocate global variables */
				printf("\tend\t%d,main\n", g_offset);
			}
