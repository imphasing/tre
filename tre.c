#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "nfa.h"
#include "list.h"
#include "tre.h"

#define MAX_STATES 1000000

int current_index = 0;
char *regex;

int main(int argc, char **argv)
{
	regex = argv[1];
	char *match = argv[2];
	fragment *parsed = parse_regex();

	// add .* to make all expressions match inside strings, since we've basically got an implicit ^ without it
	state *star_split = create_split_state();
	state *star = create_single_state(any_char);
	star_split->output = star;
	star_split->output1 = parsed->start;
	star->output = star_split;

	bool matches = nfa_matches(match, star_split);
	printf("Result (%s on %s): %d\n", regex, match, matches);

	return 0;
}

bool is_match(state_list *list)
{
	int i = 0;
	for (; i < list->next_index; i++) {
		state *check = list->list[i];

		if (check == NULL)
			return true;

		if (check->type == state_split)
			if (check->output == NULL || check->output1 == NULL)
				return true;
	}

	return false;
}


bool nfa_matches(char *string, state *nfa)
{
	state_list *possible = list_create();
	state_list *next_possible = list_create();

	list_append(possible, nfa);

	while (*string != '\0') {
		int i = 0;
		for (; i < possible->next_index; i++) {
			state *current = possible->list[i];

			if (current->type == state_single) {
				if (current->matching_value == *string || current->matching_value == any_char) {
					if (current->output == NULL)
						return true;

					list_append(next_possible, current->output);
				}
			} else if (current->type == state_split) {
				if (current->output == NULL || current->output1 == NULL)
					return true;

				list_append(possible, current->output);
				list_append(possible, current->output1);
			}
		}

		// swap new list of next states with old clear the new old
		state_list *tmp = possible;
		possible = next_possible;
		next_possible = tmp;
		list_clear(next_possible);

		string++;
	}

	return is_match(possible);
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

void connect_dangling(fragment *frag, fragment *output)
{
	int i = 0;
	for (; i < frag->num_dangling; i++)
		*(frag->dangling[i]) = output->start;
}

fragment *parse_base()
{
	char matching_value;

	switch (peek()) {
		case '(':
			eat('(');
			fragment *regex = parse_regex();
			eat(')');
			return regex;
		case '.':
			matching_value = any_char;
			next();
			break;
		case '\\':
			eat('\\');
			matching_value = next();
			break;
		default:
			matching_value = next();
			break;
	}

	state *single = create_single_state(matching_value);
	state ***dangling = malloc(1 * sizeof(state **));
	dangling[0] = &single->output;
	single->output = NULL;

	return create_fragment(single, 1, dangling);
}

fragment *parse_factor()
{
	fragment *base = parse_base();

	if (more() && peek() == '*') {
		eat('*');

		state *next = create_split_state();
		state ***dangling = malloc(1 * sizeof(state **));
		dangling[0] = &next->output1;
		next->output = base->start;
		next->output1 = NULL;

		fragment *connected = create_fragment(next, 1, dangling);
		connect_dangling(base, connected);

		return connected;

	} else if (more() && peek() == '+') {
		eat('+');

		state *next = create_split_state();
		state ***dangling = malloc(1 * sizeof(state **));
		dangling[0] = &next->output1;
		next->output = base->start;
		next->output1 = NULL;

		fragment *connected = create_fragment(next, 1, dangling);
		connect_dangling(base, connected);

		base->dangling = connected->dangling;
		base->num_dangling = connected->num_dangling;

		return base;

	} else if (more() && peek() == '?') {
		eat('?');

		state *next = create_split_state();
		state ***dangling = malloc((1 + base->num_dangling) * sizeof(state **));
		dangling[0] = &next->output1;
		next->output1 = NULL;

		next->output = base->start;

		int i = 0;
		for (; i < base->num_dangling; i++)
			dangling[1 + i] = base->dangling[i];

		fragment *connected = create_fragment(next, 1 + base->num_dangling, dangling);
		return connected;
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
		connect_dangling(next, after);

		first->dangling = after->dangling;
		first->num_dangling = after->num_dangling;

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
		split->output = term->start;
		split->output1 = regex->start;

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
