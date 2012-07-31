solver:
	cc -g -Wall nfa.c engine.c -o engine

all: engine

clean:
	rm -f engine
