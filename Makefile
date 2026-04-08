cal: cal.l cal.y
	bison -d cal.y
	flex cal.l
	g++ -o $@ cal.tab.c lex.yy.c -lfl -lm
clean:
	rm -f cal cal.output *.o cal.tab.c lex.yy.c 
