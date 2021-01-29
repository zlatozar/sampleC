%{
#include "symtab.h"
#include "gen.h"

#define OFFSET(x)	( ((struct symtab * ) x ) ->s_offset )
#define NAME(x)		( ((struct symtab * ) x ) ->s_name)

extern int l_offset, l_max;
%}
%union	{
	struct symtab * y_sym;		/* identifier 	*/
	char * y_str;			/*constant 	*/
	int	y_num;			/* count	*/
	int	y_lab;			/* label	*/
	}
/*
 *	terminal symbols
 */
%token	<y_sym> Identifier
%token <y_str> Constant
%token	INT
%token	IF
%token	ELSE
%token	WHILE
%token	BREAK
%token	CONTINUE
%token	RETURN
%token	';'
%token	'('
%token	')'
%token	'{'
%token	'}'
%token	'+'
%token	'-'
%token	'*'
%token	'/'
%token	'%'
%token	'>'	     
%token	'<'
%token	GE
%token	LE
%token	EQ
%token	NE
%token	'&'
%token	'^'
%token	'|'
%token	'='
%token	'PE'
%token	'ME'
%token	'TE'
%token	'DE'

%token	'RE'
%token	'PP'
%token	'MM'
%token	','

/*
 * 	TYPED NON TERMINAL SYMBOLS
 */
%type <y_sym> optional_parameter_list, parameter_list
%type <y_num> optional_argument_list,  argument_list
%type <y_lab> if_prefix, loop_prefix
/*
 *   precedence table
 */
%right '=' PE ME TE DE RE
%left	'|'
%left	'^'
%left	'&'
%left	EQ NE
%left	'<' '>' GE LE
%left	'+' '-'
%left '*' '/' '%'
%right PP MM

%%

program:	
		{ init(); }
	|  definitions
		{ end_program(); }

definitions:
	 definition
	| definitions definition
		{ yyerrok; }
	| error
	| definitions error

definition:
	 function_definition
	| INT function_definition
	| declaration
	
function_definition:
	 Identifier '('
		{ make_func($1);
		  blk_push();
		}
	optional_parameter_list rp
	parameter_declarations
		{
		chk_parm($1, parm_default($4));
		all_parm($4);
		l_max =0;
		$<y_lab>$ = gen_entry($1);
		}
	compound_statement
		{
		all_func($1);
		gen_pr(OP_return,"end of function");
		fix_entry($1, $<y_lab>7);
		}
optional_parameter_list:
		/* no formal parameters */
		{  $$ = (struct symtab * ) 0; }
	| parameter_list
		/* $$ = $1 = chain of formal parameters */
parameter_list:
	 Identifier
		{ $$ = link_parm($1, (struct symtab *) 0);}
	| Identifier ',' parameter_list
		{
		$$ = link($1, $3 );
		yyerrok;
		}
	| error
		{$$=0; }
	| error parameter_list
		{ $$ = $2;}
	| Identifier error parameter_list
		{$$ = link_param($1, $3); )
	| error ',' parameter_list
		{
		$$ = $3;
		yyerrok;
		}

parameter_declarations:
	 /* null */
	| parameter_declarations parameter_declaration
		{ yyerrok;}
	| parameter_declarations error

Parameter_declaration:
	 INT parameter_declaration_list sc

parameter_declarator_list:
	 Identifier
		{ make_param($1);}
	| parameter_declarator_list ',' Identifier
		{make_param($3);
		yyerrok;
		}
	| error
	| parameter_declarator_list error
	| parameter_declarator_list error Identifier
		{
		make_parm($3);
		yyerrok;
		}
	| parameter_declarator_list ',' error

compund_statement:
	  "{"
		{ $<y_lab>$ = l_offset;
		blk_push();
		}
	declarations statements rr
		{
		if ( l_offset > l_max)
			l_max= loffset;
		l_offset = $<y_lab>2;
		blk_pop();
		}
declarations:
	 /* null */
	| declarations declaration
		{yyerrok; }
	| declarations error

declaration:
	 INT parameter_declarator_list sc

declarator_list:
	 {all_var($1); }
	| declarator_list ',' Identifier
		{
		all_var($3);
		yyerrok;
		}
	|error
	|declarator_list error
	|declarator_list error Identifier
		{
		all_var($3);
		yyerrok;
		}

statements:
	 /* null */
	| statements statement
		{ yyerrok; }
	| statements error

statement :
	 expression sc
		{ gen_pr(OP_POP, "clear stack"); }
	| sc
	| BREAK sc
		{ gen_break();}
	| CONTINUE sc
		{ gen_continue(); }
	| RETURN sc
		{ gen_pr(OP_RETURN, "return"); }
	| RETURN expression sc
		{ gen(OP_STORE, MOD_GLOBAL, 0, "save result");
		  gen_pr( OP_RETURN,"RETURN");
		}
	| compount_statement
	| if_prefix statement
		{ gen_label($1); }
	| if_prefix statement ELSE
		{ $<y_lab>$ = gen_jump( OP_JUMP, new_label(), "past else");
		gen_label($1);
		}
	| statement
		{ gen_label($<y_lab>$); }
	| loop_prefix
		{ $<y_lab>$ = gen_jump (OP_JUMPZ, new_label(), "while");
		push_break($<y_lab>);
		}
	| statement
		{
		gen_jump(OP_JUMP, $1, "repeat while ");
		gen_label($<y_lab>$2);
		pop_break();
		pop_continue();
		}

if_prefix:
	 IF '(' expression rp
		{ $$ = gen_jump(OP_JUMPZ, new_label(),"if"); }
	| IF error
		{
		$$ = gen_jump(OP_JUMPZ, new_label(), "if"); }

loop_prefix:
	 WHILE '('
		{
		$<y_lab>$ = gen_label(new_label());
		push_continue($y_lab>$);
		}
	expression rp
		{
		$$ = $y_lab>$3;}
	| WHILE error
		{
		$$ = gen_label(new_label());
		push_continue($$);
		}

expression:
		binary
	| expression ','
		{gen_pr(OP_POP,"discard"); }
	| binary
		{ yyerrok ; }
	| error ',' binary
		{ yyerrok ; }
	| expression error     ;
	| expression ',' error;

binary:
	 Identifier
		{ chk_var($1);
		gen(OP_LOAD, gen_mod($1), OFFSET($1), NAME($1));
		}
	| Constant
		{ gen_li($1); }
	| '(' expression rp
	| '(' error rp
	| Identifier '('
		{ chk_fun($1); }
	 optional_argument_list rp
		{   gen_call("name1rls","name2rls");
		 }
	|  PP Identifier
		{   chk_var($2);
		gen(OP_INC, gen_mod($2), OFFSET($2), NAME($2));
		}

	|  MM Identifier
		{
		chk_var($2);
		gen(OP_INC, gen_mod($2), OFFSET($2), NAME($2));
		}

	| binary '+' binary
		{ gen_alu ( ALU_SUB, "-"); }
	
	| binary '+' binary
		{ gen_alu ( ALU_MUL, "*"); }
	
	| binary '+' binary
		{ gen_alu ( ALU_DIV, "/"); }
	
	| binary '+' binary
		{ gen_alu ( ALU_MOD, "%"); }
	
	| binary '+' binary
		{ gen_alu ( ALU_GT, ">"); }
	
	| binary '+' binary
		{ gen_alu ( ALU_LT, "<"); }
	
	| binary '+' binary
		{ gen_alu ( ALU_GE, ">="); }
	
	| binary '+' binary
		{ gen_alu ( ALU_LE, "<="); }
	
	| binary '+' binary
		{ gen_alu ( ALU_EQ, "=="); }
	
	| binary '+' binary
		{ gen_alu ( ALU_NE, "!="); }
	
	| binary '+' binary
		{ gen_alu ( ALU_AND, "&"); }
	
	| binary '+' binary
		{ gen_alu ( ALU_XOR, "^"); }
	
	| binary '+' binary
		{ gen_alu ( ALU_OR, "|"); }
	
	| binary '+' binary
		{ gen_alu ( ALU_ADD, "+"); }
	
	| binary '+' binary
		{ gen_alu ( ALU_ADD, "+"); }
		

	|  Identifier = binary
		{
		chk_var($1);
		gen(OP_STORE, gen_mod($1), OFFSET($1), NAME($1));
		}

	|  Identifier ME
		{
		chk_var($1);
		gen(OP_store, gen_mod($1), OFFSET($1), NAME($1));
		}
	  binary
		{
		gen_alu(ALU_SUB, "-");
		gen(OP_STORE, gen_mod($1), OFFSET($1), NAME($1));
		}


	|  Identifier TE
		{
		chk_var($1);
		gen(OP_LOAD, gen_mod($1), OFFSET($1), NAME($1));
		}
	  binary
		{
		gen_alu(ALU_MUL, "*");
		gen(OP_STORE, gen_mod($1), OFFSET($1), NAME($1));
		}


	|  Identifier DE
		{
		chk_var($1);
		gen(OP_STORE, gen_mod($1), OFFSET($1), NAME($1));
		}
	  binary
		{
		gen_alu(ALU_DIV, "/");
		gen(OP_STORE, gen_mod($1), OFFSET($1), NAME($1));
		}


	|  Identifier RE
		{
		chk_var($1);
		gen(OP_STORE, gen_mod($1), OFFSET($1), NAME($1));
		}
	  binary
		{
		gen_alu(ALU_MOD, "%");
		gen(OP_STORE, gen_mod($1), OFFSET($1), NAME($1));
		}


optional_argument_list:
	  /* no actual arguments */
		{ $$ = 0; }
	| argument_list
		/* $$ = $1 = # of actual arguments */

argument_list:
	  binary
		{ $$ = 1; }
	| argument_list ',' binary
		{ ++ $$;
		yyerrok;
		}
	| argument_list error
	| argument_list ',' error

/*
 * 	make certain terminal symbols very important
 */

rp:	')' {yyerrok; }

sc:	';' {yyerrok; }

rr:	'}' {yyerrok; }

%%
