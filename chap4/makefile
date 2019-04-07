a.out: parse.o y.tab.o lex.yy.o errormsg.o util.o symbol.o absyn.o table.o
	cc -g parse.o y.tab.o lex.yy.o errormsg.o util.o symbol.o absyn.o table.o

parse.o: parse.c errormsg.h util.h symbol.h absyn.h parse.h
	cc -g -c parse.c

y.tab.o: y.tab.c
	cc -g -c y.tab.c

y.tab.c: tiger.grm
	yacc -dv tiger.grm

y.tab.h: y.tab.c
	echo "y.tab.h was created at the same time as y.tab.c"

errormsg.o: errormsg.c errormsg.h util.h
	cc -g -c errormsg.c


util.o: util.c util.h
	cc -g -c util.c

symbol.o: symbol.c symbol.h util.h table.h
	cc -g -c symbol.c

absyn.h.o: absyn.c absyn.h util.h symbol.h
	cc -g -c absyn.c

table.h.o: table.c table.h util.h
	cc -g -c table.c

clean: 
	rm -f a.out util.o parse.o lex.yy.o errormsg.o y.tab.c y.tab.h y.tab.o symbol.o
