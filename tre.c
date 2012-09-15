#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "nfa.h"
#include "list.h"
#include "tre.h"

int current_index = 0;
char *regex;

int main(int argc, char **argv)
{
	regex = argv[1];
	char *match = argv[2];	
	fragment *parsed = parse_regex();

	// add .* to make all expressions match inside strings
	/*
	state *star_split = create_split_state();
	state *star = create_single_state(any_char);
	connect_split_state(star_split, star, parsed);
	connect_single_state(star, star_split);
	*/

	bool matches = nfa_matches(match, parsed->start);
	printf("Result (%s on %s): %d\n", regex, match, matches);

	return 0;
}

// wow this actually works
// doesn't match if there's characters in front of the string to test though, so it's like an implicit ^..
bool nfa_matches(char *string, state *nfa)
{
	list *possible = create_list();
	prepend_node(possible, nfa);

	while (*string != '\0') {
		node *next = possible->first;
		list *next_possible = create_list();

		while (next != NULL) {
			state *current = next->data;
			if (current->type == state_single) {
				if (current->matching_value == *string || current->matching_value == any_char) {
					if (current->output == NULL)
						return true;

					if (current->output->type == state_split)
						// if we get a split state, add it to the current list and process it again
						append_node(possible, current->output);
					else
						// otherwise just add the next state to the next list and continue
						prepend_node(next_possible, current->output);
				}
			} else if (current->type == state_split) {
				if (current->output == NULL || current->output1 == NULL)
					return true;

				append_node(possible, current->output);
				append_node(possible, current->output1);
			}
				
			possible = next_possible;
			next = next->next;
		}

		string++;
	}

	return false;
}


char peek()
{
	return regex[current_index];
}

bool eat(char c)
{
	if (regex[current_index] == c) {
		current_index++;
		return true;
	}

	return false;
}

char next()
{
	return regex[current_index++];
}

bool more()
{
	return current_index < strlen(regex);
}


fragment *parse_base()
{
	switch (peek()) {
		case '(':
			eat('(');
			fragment *regex = parse_regex();
			eat(')');
			return regex;
		default:
		{
			state *single = create_single_state(next());
			state ***dangling = malloc(1 * sizeof(state **));
			dangling[0] = &single->output;

			return create_fragment(single, 1, dangling);
		}
	}
}

fragment *parse_factor()
{
	fragment *base = parse_base();

	if (more() && peek() == '*') {
		eat('*');
		state *next = create_split_state();
		next->output = base;

		// connect dangling states from base to the new state
		int i = 0;
		for (; i < base->num_dangling; i++)
			*(base->dangling[i]) = next;	

		state ***dangling = malloc(1 * sizeof(state **));
		dangling[0] = &next->output1;

		return create_fragment(next, 1, dangling);
	} else if (more() && peek() == '+') {
		eat('+');
		state *next = create_split_state();
		next->output = base;

		int i = 0;
		for (; i < base->num_dangling; i++)
			*(base->dangling[i]) = next;	

		state ***dangling = malloc(1 * sizeof(state **));
		dangling[0] = &next->output1;

		return create_fragment(base, 1, dangling);
	}
	else if (more() && peek() == '?') {
		eat('?');
		state *next = create_split_state();
		next->output = base;

		state ***dangling = malloc((1 + base->num_dangling) * sizeof(state **)); 
		dangling[0] = &next->output1;

		int i = 0;
		for (; i < base->num_dangling; i++)
			dangling[1 + i] = base->dangling[i];

		return create_fragment(next, 2 + base->num_dangling, dangling);
	}

	return base;
}

fragment *parse_term()
{
	fragment *first;
	fragment *next = parse_factor();
	first = next;

	while (more() && peek() != ')' && peek() != '|') {
		fragment *after = parse_factor();

		int i = 0;
		for (; i < next->num_dangling; i++)
			*(next->dangling[i]) = after;

		next = after;
	}

	return first;
}

fragment *parse_regex()
{
	fragment *term = parse_term();

	if (more() && peek() == '|') {
		eat('|');
		fragment *regex = parse_regex();

		state *split = create_split_state();
		split->output = term;
		split->output1 = regex;

		state ***dangling = malloc((term->num_dangling + regex->num_dangling) * sizeof(state **));

		int i = 0;
		for (; i < term->num_dangling; i++)
			dangling[i] = term->dangling[i];

		for (i = 0; i < regex->num_dangling; i++)
			dangling[term->num_dangling + i] = regex->dangling[i];

		return create_fragment(split, term->num_dangling + regex->num_dangling, dangling);
	}
	else {
		return term;
	}
}
