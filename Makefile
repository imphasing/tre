tre:
	cc -g -Wall nfa.c list.c tre.c -o tre

all: tre

clean:
	rm -f tre
