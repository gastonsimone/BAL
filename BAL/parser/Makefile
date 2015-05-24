CC := gcc
LEX := flex
YACC := bison
CTAGS = ctags -R
PARSER = ./parser

# Use la siguiente definicion durante el desarrollo
CFLAGS = `pkg-config --cflags glib-2.0` -Wall -ansi -pedantic -pedantic-errors -g -ggdb
# Use la siguiente definicion para producir una version para uso real
#CFLAGS = `pkg-config --cflags glib-2.0` -Wall -ansi -pedantic -pedantic-errors -O3

matriz_scanner.o: matriz_scanner.c matriz_parser.tab.c
	$(CC) $(CFLAGS) -c $< -o $@

matriz_parser.o: matriz_parser.tab.c
	$(CC) $(CFLAGS) -c $^ -o $@

matriz_scanner.c: matriz_scanner.lex matriz_parser.tab.c
	$(LEX) -o $@ $<

matriz_parser.tab.c: matriz_parser.y
	$(YACC) -d $<

