#include <stdlib.h>
#include "nfa.h"

state *create_single_state(char matching_value)
{
	state *new_state = malloc(sizeof(state));
	new_state->type = state_single;
	new_state->matching_value = matching_value;

	return new_state;
}

state *create_split_state()
{
	state *new_state = malloc(sizeof(state));
	new_state->type = state_split;

	return new_state;
}

fragment *create_fragment(state *start, int num_dangling, state ***dangling)
{
	fragment *frag = malloc(sizeof(fragment));
	frag->start = start;
	frag->num_dangling = num_dangling;
	frag->dangling = dangling;

	return frag;
}
