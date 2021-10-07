    CC     = gcc -g
    CFLAGS = -I/usr/include
    LFLAGS = -L/usr/lib  -lm -lmatheval
 
	OBJS = ep01.o iterativeMethods.o functions.o
 
# regra default (primeira regra)
all: ep01
 
# regras de ligacao
ep01: $(OBJS) $(LFLAGS)
 
# regras de compilação
functions.o: $(CFLAGS) functions.c functions.h
iterativeMethods.o: $(CFLAGS) functions.o iterativeMethods.c iterativeMethods.h 
ep01.o: $(CFLAGS) functions.o iterativeMethods.o ep01.c 

# remove arquivos temporários
clean:
	-rm -f $(OBJS) *~
 
# remove tudo o que não for o código-fonte
purge: clean
	-rm -f ep01