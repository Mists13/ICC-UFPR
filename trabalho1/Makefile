CC = gcc -g
PREFIX = ${HOME}
LIBDIR = $(PREFIX)/lib
INCDIR = $(PREFIX)/include

CFLAGS = -Wall -std=c11 -I$(INCDIR)
LFLAGS = -Wl,-rpath -Wl,$(LIBDIR) -L$(LIBDIR) -lmatheval

PROG = trabalho1
OBJS = utils.o \
	   functions.o \
	   trabalho1.o

# regra default
all: $(PROG)

# regras de compilação
%.o: %.c 
	$(CC) -c $(CFLAGS) $<

# regras de ligação
$(PROG): % : $(OBJS) %.o
	$(CC) -o $@ $^ $(LFLAGS) 

# remove arquivos temporários
clean:
	-rm -f $(OBJS) *~
 
# remove tudo o que não for o código-fonte
purge: clean
	-rm -f $(PROG)