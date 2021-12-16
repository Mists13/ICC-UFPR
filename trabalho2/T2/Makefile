# Luzia Millena Santos Silva, GRR20185174, 	lmss18@inf.ufpr.br
# Matheus Pacheco Santos,     GRR20192786,	mps19@inf.ufpr.br

CC = gcc # -std=c11
PREFIX = ${HOME}
LIBDIR = $(PREFIX)/lib
INCDIR = $(PREFIX)/include

LIKWID = /home/soft/likwid
LIKWID_FLAGS = -I$(LIKWID)/include -DLIKWID_PERFMON 
LIKWID_LIBS = -L$(LIKWID)/lib -llikwid

AVX_FLAGS = -O3 -mavx2 -march=native -fopt-info-vec # -fstrict-aliasing

CFLAGS = $(LIKWID_FLAGS) 
LFLAGS = -Wl,-rpath -Wl,$(LIBDIR) -L$(LIBDIR) -lmatheval $(LIKWID_LIBS) -lm

PROG = newtonSNL
OBJS = utils.o alloc.o sistemas.o newtonSNL.o

.PHONY: all debug clean limpa purge faxina

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

all: likwid

debug:            CFLAGS += -g -DDEBUG
avx:              CFLAGS += $(AVX_FLAGS)

avx likwid debug: $(PROG)

$(PROG) : % : $(OBJS) %.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

clean limpa:
	@echo "Limpando ...."
	@rm -f *~ *.bak *.tmp

purge faxina:   clean
	@echo "Faxina ...."
	@rm -f  $(PROG) $(PROG_AUX) *.o $(OBJS) core a.out
	@rm -f *.png marker.out *.log
