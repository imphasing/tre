#include <stdlib.h>
#include "nfa.h"

state *create_single_state(char matching_value)
{
	state *state = malloc(sizeof(state));
	state->type = state_single;
	state->matching_value = matching_value;

	return state;
}

state *create_split_state()
{
	state *state = malloc(sizeof(state));
	state->type = state_split;

	return state;
}

fragment *create_fragment(state *start, int num_dangling, state ***dangling)
{
	fragment *frag = malloc(sizeof(fragment));
	frag->start = start;
	frag->num_dangling = num_dangling;
	frag->dangling = dangling;

	return frag;
}
