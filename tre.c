#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "nfa.h"
#include "list.h"

// wow this actually works
// doesn't match if there's characters in front of the string to test though, so it's like an implicit ^..
bool nfa_matches(char *string, struct state *nfa)
{
	struct list *possible = create_list();
	prepend_node(possible, nfa);

	while (*string != '\0') {
		struct node *next = possible->first;
		struct list *next_possible = create_list();

		while (next != NULL) {
			struct state *current = next->data;
			if (current->type == state_single) {
				if (current->matching_value == *string || current->matching_value == any_char) {
					if (current->output->type == state_match)
						return true;

					prepend_node(next_possible, current->output);
				}
			} else if (current->type == state_split) {
				if (current->output->type == state_match || current->output1->type == state_match)
					return true;

				append_node(possible, current->output);
				append_node(possible, current->output1);
			}
				
			next = next->next;
		}

		possible = next_possible;
		string++;
	}

	return false;
}

		
int main(int argc, char **argv)
{
	// (b|c)e?e?man
	struct state *match = create_match_state();

	struct state *b = create_single_state('b');
	struct state *c = create_single_state('c');
	
	struct state *e1 = create_single_state('e');
	struct state *e2 = create_single_state('e');

	struct state *m = create_single_state('m');
	struct state *a = create_single_state('a');
	struct state *n = create_single_state('n');

	struct state *first_alternation = create_split_state();
	struct state *first_optional = create_split_state();
	struct state *second_optional = create_split_state();

	connect_split_state(first_alternation, b, c);
	connect_single_state(b, first_optional);
	connect_single_state(c, first_optional);

	connect_split_state(first_optional, e1, second_optional);
	connect_single_state(e1, second_optional);

	connect_split_state(second_optional, e2, m);
	connect_single_state(e2, m);

	connect_single_state(m, a);
	connect_single_state(a, n);
	connect_single_state(n, match);

	// .* fragment, to match inside strings, should be implicitly added to a regex unless ^blah is specified
	struct state *star_split = create_split_state();
	struct state *star = create_single_state(any_char);
	connect_split_state(star_split, star, first_alternation);
	connect_single_state(star, star_split);

	bool matches = nfa_matches(argv[1], star_split);

	printf("Result (%s): %d\n", argv[1], matches);

	return 0;
}
