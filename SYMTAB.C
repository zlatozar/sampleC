#include "symtab.h"
#include "samplec.h"
/*
 *#include "y_tab.h"
*/

/*
 * symbol table
 */
static struct symtab
		symtab,					/* blind element */
		* s_gbl;					/* global end of chain */
#define s_lcl	(& symtab)   	/* local end of chain */

	/*
	*		block table
	*/
static int blknum =0;	/*current static block depth */
/*
 *		add net name to local retion
 */

static struct symtab *s_create(name)
	register char * name;
{
		register struct symtab * new_entry = (struct symtab* )
						calloc(1,sizeof(struct symtab));
		if ( new_entry)
			{
				new_entry->s_next = s_lcl->s_next;
				s_lcl->s_next = new_entry;
				new_entry->s_name = strsave(name);
				new_entry->s_type =UDEC;
				new_entry->s_blknum=0;
				new_entry->s_pnum = NOT_SET;
				return new_entry;
			
			}
		fatal("No more room for symbols.");
}
/*
 *		move an entry from local to global region
 */
static s_move(symbol)
	register struct symtab * symbol ;
		{
		register struct symtab * ptr;
		/* find desired entry in symtab chain ( buf if missing */
		for (ptr = s_lcl;ptr->s_next != symbol; ptr = ptr->s_next)
			if ((ptr->s_next != (struct symtab*) 0 ))
				bug("s_move");
		/* unlink it form its present position */
		ptr->s_next = symbol->s_next;
			/* relink at the end of symtab */
		s_gbl->s_next = symbol;
		s_gbl = symbol;
		s_gbl->s_next = (struct symtab *) 0;
}
/*
 * initialize symbol and block table
 */
init()
{
	blk_push();
	s_gbl = s_create("main");
	s_gbl->s_type = UFUNC;
	 
}
/*
 * block push table
 */
blk_push()
{
	++blknum;
}

/*
 *		locate entry by name
 */
struct symtab * s_find(name)
char * name;
{

	register struct symtab * ptr;
	/* search symtab until match or end of symtab chain */
	for (ptr = s_lcl->s_next ; ptr; ptr = ptr->s_next)
		  if ( ! ptr->s_name)
			 bug("s_find");
		else
			/* return ptr if names match */
			if(strcmp(ptr->s_name, name) == 0 )
				return ptr;
	/* search fails, return NULL */
			return ( struct symtab * ) 0;

}

/*
 *		Interface for lexical analyzer
 *     locate or enter identifier, save text of constant
 */
s_lookup(yylex)
int yylex;						/* constant of identifer */
{

	extern char yytext[] ;		/* text of symbol */
	switch (yylex)		{
		case Constant:
			yylval.y_str = strsave(yytext);
			break;
		case Identifier:
			if(yylval.y_sym = sfind(yytext))
				break;
			yylval.y_sym = s_create(yytext);
			break	;
		default:
			bug("s_lookup");



		}
}


/*
 *	mark entry as part of parameter_list
 */
struct symtab * link_parm ( symbol, next )
	register struct symtab * symbol, * next;
{

	switch ( symbol->s_type) {

		case PARM:
			error("duplicat parameter %s", symbol->s_name);
			return next;
		case FUNC:
		case UFUNC:
		case VAR:
			symbol = s_create(symbol->s_name);
		case UDEC:
			break;
		default:
			bug("link_parm");
			}
		symbol->s_type = PARM;
		symbol->s_blknum = blknum;
		symbol->s_plist = next;
		return symbol;
}
/*
 * declare a parameter
 */
struct symtab * make_parm(symbol)
	register struct symtab * symbol;
{
	switch ( symbol->s_type) {

		case VAR:
			if( symbol->s_blknum == 2){
				error("parameter %s declared twice", symbol->s_name);
				return symbol;
				}
		case FUNC:
		case UFUNC:
		case UDEC:
			error("%s is not a parameter", symbol->s_name);

  	 	case PARM:
  			break;

			default:
			bug("make_parm");
			}
		symbol->s_type = VAR;
		symbol->s_blknum = blknum;
		return symbol;
}

/*
 * declare a variable
 */
struct symtab * make_var(symbol)
	register struct symtab * symbol;

{
	switch ( symbol->s_type) {

		case VAR:
		case FUNC:
		case UFUNC:
				if( symbol->s_blknum ==blknum || symbol->s_blknum ==2 &&
						blknum == 3)
						error("duplicate name %s", symbol->s_name);
					symbol = screate(symbol->s_name);
	 	case UDEC:
				break;
	 	case PARM:
			error ("unexpected parameter %s",symbol->s_name);
			break;
		default:
			bug("make_var");
			}
		symbol->s_type = VAR;
		symbol->s_blknum = blknum;
		return symbol;
}
/*
 * define a function
 */
struct symtab * make_func(symbol)
	register struct symtab * symbol;

{
	switch ( symbol->s_type) {


		case UDEC:
		case UFUNC:
				break;
		case VAR:
			error("function name %s same as global Variable");
			break;
	 	case FUNC:
			error("duplicate function definitions %s", symbol->s_name);
			break;
	  	default:
			bug("make_func");
			}
		symbol->s_type = FUNC;
		symbol->s_blknum = 1;
		return symbol;
}

/*
 * set or verify number of parameters
 */
chk_parm (symbol, count)
register struct symtab * symbol;
register int count;
{
	if( symbol ->s_pnum == NOT_SET)
		symbol->s_pnum = count;
	else if (((int) symbol->s_pnum != count))
		warning(" function %s should have %d arguments(s)",
			symbol->s_name,symbol->s_pnum);
}

/*
 * default undeclared parameters, count
 */
int parm_default (symbol)
	register struct symtab * symbol;
{	register int count =0;
 	while ((symbol))
		{
		++count;
		if(symbol->s_type == PARM)
			symbol->s_type = VAR;
		symbol = symbol->s_plist;
		}
		return count;

	}

/*
 * pop block table
 */
blk_pop()
{
	register struct symtab * ptr;
	for (ptr = s_lcl->s_next;
			(ptr && 	(ptr->s_blknum >= blknum || ptr->s_blknum == 0 ));
			ptr = s_lcl->s_next)
				{
				if (ptr->s_name)
					but("blk_pop null name");
#ifdef TRACE
				{
				static char * type[] = {SYMmap };
				message "Popping %s: %s, depth %d, offset %d",
					ptr-s_name, type[ptr->stype],
					ptr->s_blknum, ptr->s_offset);
				}
#endif
				if ((ptr->s_type == UFUNC))
					error (" undefined function %s",ptr->s_name);
					cfree(ptr->s_name);
					s_lcl->s_next = ptr->s_next;
					cfree(ptr);
					
			}
		--blknum;
}

/*
 *	 CHECK REFERENCE OR ASSIGNMENT TO VARIABLE
 */

chk_var ( symbol )
	register struct symtab * symbol;
{
	switch (( symbol->s_type)){

		case UDEC:
			error(" undeclared variable %s", symbol->s_name);
			break;
		case PARM:
			error("unexpected parameter %s", symbol->s_name);
			break;
		case FUNC:
		case UFUNC:
			error("function %s used as variable", symbol->s_name);
		case VAR:
			return 0;
		default:
			bug("check_var");

		}
	symbol->s_type = VAR;
	symbol->s_blknum = blknum;

}

/*
 * check reference to function, implicitly declare it
 */
chk_func(symbol)
register struct symtab * symbol;
{
	switch (symbol->s_type){
		case UDEC:
			break;
		case PARM:
			error("unexpected parameter %s", symbol->s_name);
			symbol->s_pnum = NOT_SET;
			return 0;
		case VAR:
			error("variable %s used as function", symbol->s_name);
			symbol->s_pnum = NOT_SET;
		case UFUNC:
		case FUNC:
				return 0;
			default:
				bug("check_func");

		}
		s_move(symbol);
		symbol->s_type =UFUNC;
		symbol->s_blknum = 1;

}
