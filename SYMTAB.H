/*
 * header file for symbol table
 */
struct symtab {
	char * s_name;			/* name pointer */
	int	s_type;			/* symbol type */
   int	s_blknum;		/* static block depth */
	union {
			int s__num;
			struct symtab * s__link;
			}s__;
	int	s_offset;		/* symbol definition */
	struct symtab * s_next;	/* next entry */
	};
#define s_pnum	s__.s__num	/* count of parameters */
#define NOT_SET (-1)
#define s_plist s__.s__link	/* chain of parameters */

/*
 * s_type values
 */

#define UDEC 0			/* not declared*/
#define FUNC 1			/* function */
#define UFUNC 2		/* undefined function */
#define VAR	3			/* declared vbariable */
#define PARM 4			/* undeclared parameter *.

/*
 *	s_type values for s_trace
 */

#define	SYMmap	"undeclared", "function", "undefined function", \
						"variable", "parameter"

/*
 *		typed functions, symbol table module
 */

struct symtab * link_parm();		/* chain parameters */
struct symtab * s_find();			/* locate symbol by name */
struct symtab * make_parm();		/* define parameter */
struct symtab * make_var();		/* define variable */
struct symtab * make_func();		/* define function */
/*
 *   typed library functions
 */
char * strsav();						/* dynamically save a string */
char * calloc();						/* dynamicall obtain memory 	*/
