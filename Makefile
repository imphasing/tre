solver:
	cc -g -Wall nfa.c list.c engine.c -o engine

all: engine

clean:
	rm -f engine
