#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "nfa.h"

// lol. terrible
bool nfa_matches(char *string, struct state *nfa)
{
	struct state *next = nfa;

	while (*string != '\0') {
		if (next->matching_value == *string) {
			if (next->type == state_single) {
				next = next->output;
				string++;
			} else {
				printf("WHOOPS THIS MATCHER SUX");
			}
		} else {
			break;
		}
	}

	if (next->type == state_match)
		return true;
	
	return false;
}

		
int main(int argc, char **argv)
{
	struct state *initial = create_single_state('a');
	struct state *second = create_single_state('l');
	struct state *third = create_single_state('e');
	struct state *fourth = create_single_state('x');
	struct state *match = create_match_state();

	connect_single_state(initial, second);
	connect_single_state(second, third);
	connect_single_state(third, fourth);
	connect_single_state(fourth, match);

	char test1[] = "what sucka";
	char test2[] = "alex is okay";

	bool matches = nfa_matches(test1, initial);
	bool matches2 = nfa_matches(test2, initial);

	printf("Result of test 1 (%s): %d\n", test1, matches);
	printf("Result of test 2 (%s): %d\n", test2, matches2);

	return 0;
}
