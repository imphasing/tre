#include <stdlib.h>
#include "nfa.h"


struct state *create_single_state(char matching_value)
{
	struct state *state = malloc(sizeof(struct state));
	state->type = state_single;
	state->matching_value = matching_value;

	return state;
}

struct state *create_split_state()
{
	struct state *state = malloc(sizeof(struct state));
	state->type = state_split;

	return state;
}

struct state *create_match_state()
{
	struct state *state = malloc(sizeof(struct state));
	state->type = state_match;

	return state;
}

void connect_single_state(struct state *state, struct state *output)
{
	state->output = output;
	return;
}

void connect_split_state(struct state *state, struct state *output, struct state *output1)
{
	state->output = output;
	state->output1 = output1;
	return;
}


	


