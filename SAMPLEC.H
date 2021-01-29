
typedef union 	{
	struct symtab * y_sym;		/* identifier 	*/
	char * y_str;			/*constant 	*/
	int	y_num;			/* count	*/
	int	y_lab;			/* label	*/
	} YYSTYPE;
extern YYSTYPE yylval;
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
