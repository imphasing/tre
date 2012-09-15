tre:
	cc -g -Wall tre.c nfa.c list.c -o tre

all: tre

clean:
	rm -f tre
