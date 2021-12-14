# Luzia Millena Santos Silva, GRR20185174, 	lmss18@inf.ufpr.br
# Matheus Pacheco Santos,     GRR20192786,	mps19@inf.ufpr.br

CC = gcc -g
PREFIX = ${HOME}
LIBDIR = $(PREFIX)/lib
INCDIR = $(PREFIX)/include

CFLAGS = -Wall -std=c11 -I$(INCDIR)
LFLAGS = -Wl,-rpath -Wl,$(LIBDIR) -L$(LIBDIR) -lmatheval

PROG = newtonSNL
OBJS = utils.o \
	   alloc.o \
	   sistemas.o \
	   newtonSNL.o

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