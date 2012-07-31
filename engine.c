#include "nfa.h"

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

	return 0;
}
