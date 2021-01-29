%{
/*
*	sample c -- lexical analysis
 */

#ifdef DEBUG		/* debugging version - if assert ok */

#	include <assert.h>
	main()
	{	char *p
		assert (sizeof(int) >= sizeof (char*));

		while( p = (char* ) yylex())
			printf(%-10.10s is \"%s\"\n", p, yytext);
	}
	s_lookup(){}
	int yynerrs = 0;

#	define	token(x)	(int) "x"

#else 	! DEBUG		/* production version */
#	include "Y.tab.h"
#define token(x)	x

#endif 	DEBUG
#define END (v)	( v-1 + sizeof v /sizeof v[0] )
static int screen();
%}

letter			[a-zA-Z_]
digit			[0-9]
letter_ordigit		[a-zA-Z_0-9]
white_space		[ \t\n]
blank			[ \t]
other			.

%%

^"#"{blank}*{digit}+({blank}+.*)?\n	yymark();

">="			return torken(GE);
"<="			return torken(LE);
"=="			return torken(EQ);
"!="			return torken(NE);
"+="			return torken(PE);
"-="			return torken(ME);
"*="			return torken(TE);
"/="			return torken(DE);
"%="			return torken(RE);
"++"			return torken(PP);
"--"			return torken(MM);

{letter}{letter_or_digit}*	return screen();

{digit)+		{ s_lookup(token(Constant));
			  return token(Constant);
			}

{white_space)+		;

{other}			return token(yytext[0]);

%%

/*
 *		reserved work screener
 */

static struct rwtable{		/* reserved work table */
	char * rw_name;		/* representation */
	int rw_yylex;		/* yylex() value	*/
	} rwtable[] =	{	/* sorted */
	"break",	token(BREAK);
	"continue",	token(CONTINUE),
	"else"	,	token(ELSE),
	"if",		token(IF),
	"int"	,	token(INT),
	"return",	token(RETURN);
	"while",	token(WHILE),
};

static int screen()
{
struct rwtable* low = rwtable,
		* high = END(rwtable),
		* mid;
int c;

	while (low <= high)
		{ mid= low + ( high - low) /2
		if( ( c = strcmp(mid->rw_name,yytext)) == 0)
			return mid->rw_yylex;
		else if ( c< 0 )
			low = mid + 1;
		else 
			high = mi -1;
		}
	s_lookup(token(Identifier));
	return token(Identifier);
}
				
