CC := gcc
CTAGS = ctags -R

# Use la siguiente definicion durante el desarrollo
CFLAGS = `pkg-config --cflags glib-2.0` -Wall -ansi -pedantic -pedantic-errors -g -ggdb
# Use la siguiente definicion para producir una version para uso real
#CFLAGS = `pkg-config --cflags glib-2.0` -Wall -ansi -pedantic -pedantic-errors -O3

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

