#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "tre.h"
#include "list.h"

int current_index = 0;
char *regex;

int main(int argc, char **argv)
{
	regex = argv[1];
	char *match = argv[2];	
	struct state *parsed = parse_regex();

	struct state *star_split = create_split_state();
	struct state *star = create_single_state(any_char);
	connect_split_state(star_split, star, parsed);
	connect_single_state(star, star_split);

	bool matches = nfa_matches(match, star_split);
	printf("Result (%s on %s): %d\n", regex, match, matches);

	return 0;
}

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
					if (current->output == NULL)
						return true;

					prepend_node(next_possible, current->output);
				}
			} else if (current->type == state_split) {
				if (current->output == NULL || current->output1 == NULL)
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
	char next = regex[current_index];
	current_index++;
	return next;
}

bool more()
{
	return current_index < strlen(regex);
}


struct state *parse_base()
{
	switch (peek()) {
		case '(':
			eat('(');
			struct state *regex = parse_regex();
			eat(')');
			return regex;
		default:
			return create_single_state(next());
	}
}

struct state *parse_factor()
{
	struct state *base = parse_base();

	while (more() && peek() == '*') {
		eat('*');
		struct state *next = create_split_state();
		next->output = base;
		base->output = next;
		base = next;
	}

	return base;
}

struct state *parse_term()
{
	struct state *first;
	if (more() && peek() != ')' && peek() != '|') {
		struct state *next = parse_factor();
		first = next;

		while (more() && peek() != ')' && peek() != '|') {
			struct state *after = parse_factor();
			switch (next->type) {
				case state_single:
					next->output = after;
					next = after;
					break;
				case state_split:
					next->output1 = after;
					next = after;
					break;
			}
		}
	}
	else {
		printf("Something terrible happened.\n");
	}

	return first;
}

struct state *parse_regex()
{
	struct state *term = parse_term();

	if (more() && peek() == '|') {
		eat('|');
		struct state *regex = parse_regex();
		struct state *split = create_split_state();
		connect_split_state(split, term, regex);
		return split;
	}
	else {
		return term;
	}
}
