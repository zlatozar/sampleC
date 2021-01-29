/*
 * Created by CSD YACC (IBM PC) from "samplec.Y" */

# line 1

#include "symtab.h"
#include "gen.h"

#define OFFSET(x)	( ((struct symtab * ) x ) ->s_offset )
#define NAME(x)		( ((struct symtab * ) x ) ->s_name)

extern int l_offset, l_max;

# line 10

#define UNION 1
typedef union 	{
	struct symtab * y_sym;		/* identifier 	*/
	char * y_str;			/*constant 	*/
	int	y_num;			/* count	*/
	int	y_lab;			/* label	*/
	} YYSTYPE;
# define Identifier 257
# define Constant 258
# define INT 259
# define IF 260
# define ELSE 261
# define WHILE 262
# define BREAK 263
# define CONTINUE 264
# define RETURN 265
# define GE 266
# define LE 267
# define EQ 268
# define NE 269
# define  PE 270
# define  ME 271
# define  TE 272
# define  DE 273
# define  RE 274
# define  PP 275
# define  MM 276
# define PE 277
# define ME 278
# define TE 279
# define DE 280
# define RE 281
# define PP 282
# define MM 283
#define yyclearin yychar = -1
#define yyerrok yyerrflag = 0
#ifndef YYMAXDEPTH
#define YYMAXDEPTH 150
#endif
extern YYSTYPE yylval;  /*CSD & DECUS LEX */
YYSTYPE yyval;          /*CSD & DECUS LEX */