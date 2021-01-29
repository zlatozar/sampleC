// yyac -d samplec.y

//
lex samplec.l
cc -DDEBUG lex.yy.c -ll -o lexi

// for code generator of pseudo machine
cc gen.c mem.c symtab.c message.c y.tab.c lex.yy.c \
	colib -ll -o gen


// load and go compiller and simulator
cc sim.c simgen.c mem.c symbat.c message.c y.tab.c lex.yy.c \
	colib -ll -o sim
