CC = gcc
RM = rm -f
AR = ar
CTAGS = ctags -R
ARFLAGS = -r -c -s

# Use la siguiente definicion durante el desarrollo
CFLAGS = `pkg-config --cflags glib-2.0` -Wall -ansi -pedantic -pedantic-errors -g -ggdb
# Use la siguiente definicion para producir una version para uso real
#CFLAGS = `pkg-config --cflags glib-2.0` -Wall -ansi -pedantic -pedantic-errors -O3

# Directorios
PARSER = ./parser
SPARSE = ./sparse
CHOLESKY = ./cholesky

MODULOS = bal.o \
		  oper.o \
		  utils.o \
		  $(PARSER)/matriz_parser.o \
		  $(PARSER)/matriz_scanner.o \
		  $(SPARSE)/sp_coord.o \
		  $(SPARSE)/sp_packcol.o \
		  $(SPARSE)/sp_cds.o \
		  $(CHOLESKY)/cholesky.o \
		  $(CHOLESKY)/reordenamiento.o

# Donde instalar BAL
PREFIX=/usr/local
LIBDIR=$(PREFIX)/lib
INCLUDEDIR=$(PREFIX)/include
DOCDIR=$(PREFIX)/doc

libbal.a: $(MODULOS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

$(PARSER)/matriz_parser.o: $(PARSER)/matriz_parser.y
	cd $(@D) && make $(@F)

$(PARSER)/matriz_scanner.o: $(PARSER)/matriz_parser.y $(PARSER)/matriz_scanner.lex
	cd $(@D) && make $(@F)

$(SPARSE)/sp_coord.o: $(SPARSE)/sp_coord.c $(SPARSE)/sp_coord.h
	cd $(@D) && make $(@F)

$(SPARSE)/sp_packcol.o: $(SPARSE)/sp_packcol.c $(SPARSE)/sp_packcol.h
	cd $(@D) && make $(@F)

$(SPARSE)/sp_cds.o: $(SPARSE)/sp_cds.c $(SPARSE)/sp_cds.h
	cd $(@D) && make $(@F)

$(CHOLESKY)/cholesky.o: $(CHOLESKY)/cholesky.c $(CHOLESKY)/cholesky.h
	cd $(@D) && make $(@F)

$(CHOLESKY)/reordenamiento.o: $(CHOLESKY)/reordenamiento.c $(CHOLESKY)/reordenamiento.h
	cd $(@D) && make $(@F)

.PHONY: tags doc clean purge cleandoc install uninstall

tags:
	$(CTAGS)

doc:
	doxygen bal.doxygen

clean:
	cd $(PARSER) && $(RM) *~ *.o *.bak tags matriz_scanner.c matriz_parser.tab.c matriz_parser.tab.h
	cd $(SPARSE) && $(RM) *~ *.o *.bak
	cd $(CHOLESKY) && $(RM) *~ *.o *.bak
	$(RM) *~ *.o *.bak tags

purge: clean
	$(RM) *.a

cleandoc:
	$(RM) -r doc

install:
	mkdir -p -m 755 $(LIBDIR)
	cp -p libbal.a $(LIBDIR)/libbal.a
	- chmod a+r $(LIBDIR)/libbal.a
	mkdir -p -m 755 $(INCLUDEDIR)
	- rm -r $(INCLUDEDIR)/BAL
	mkdir -m 755 $(INCLUDEDIR)/BAL
	cp -p *.h $(INCLUDEDIR)/BAL
	mkdir -m 755 $(INCLUDEDIR)/BAL/sparse
	cp -p ./sparse/*.h $(INCLUDEDIR)/BAL/sparse
	mkdir -m 755 $(INCLUDEDIR)/BAL/parser
	cp -p ./parser/*.h $(INCLUDEDIR)/BAL/parser
	mkdir -m 755 $(INCLUDEDIR)/BAL/cholesky
	cp -p ./cholesky/*.h $(INCLUDEDIR)/BAL/cholesky
	- chmod -R a+r $(INCLUDEDIR)/BAL
	mkdir -p -m 755 $(DOCDIR)
	- rm -r $(DOCDIR)/BAL
	mkdir -m 755 $(DOCDIR)/BAL
	cp -p ./doc/html/* $(DOCDIR)/BAL
	- chmod -R a+r $(DOCDIR)/BAL

uninstall:
	- rm $(LIBDIR)/libbal.a
	- rm -r $(INCLUDEDIR)/BAL
	- rm -r $(DOCDIR)/BAL

