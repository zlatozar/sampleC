#include <stdio.h>
#define VARARG fmt, v1, v2, v3, v4, v5
#define VARPARM (VARARG) char * fmt;
extern FILE * yyerfp;

message VARPARM
{
	yywher();
	fprintf(yyerfp,VARARGP);
	putc('\n',yyerfp);

}

error VARPARM
{
	extern int yynerrs;
	fprintf(yyerfp,"[error %d] ",++yynerrs);
	message(VARARG);
}

warning VARPARM
{
	fputs("[warning] ",yyerfp);
	message (VARARG);
}

fatal VARPARM
{
	fputs("[fatal error] ",yyerfp);
	message(VARARG);
	exit(1);
}

bug VARPARM
{
	fputs("BUG: " , yyerfp);
	message(VARARG);
	exit(1);

}

 char * strsave(s)
	register char * s;
{
	register char * cp = calloc(strlen(s)+1,1);
	if ((cp))
		{
		strcpy(cp,s);
		return cp;
		}
	fatal("no more room to save strings");
}
