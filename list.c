#include <stdlib.h>
#include "nfa.h"
#include "list.h"

list *create_list()
{
	list *new_list = malloc(sizeof(list));
	new_list->first = NULL;

	return new_list;
}

void append_node(list *to_append, state *data)
{
	node *new_node = malloc(sizeof(node));
	new_node->data = data;
	new_node->next = NULL;

	if (to_append->first == NULL) {
		to_append->first = new_node;
	} else {
		node *next = to_append->first;
		while (next->next != NULL)
			next = next->next;
	
		next->next = new_node;
	}
}

void prepend_node(list *to_prepend, state *data)
{
	node *new_node = malloc(sizeof(node));
	new_node->data = data;
	new_node->next = to_prepend->first;

	to_prepend->first = new_node;
}
	

void remove_node(list *to_remove, int index)
{
	if (to_remove->first == NULL)
		return;
	
	if (index == 0) {
		to_remove->first = to_remove->first->next;
		return;
	}

	node *previous = to_remove->first;
	node *next = to_remove->first->next;
	int current_index = 1;

	while (next != NULL) {
		if (index == current_index) {
			previous->next = next->next;
			return;
		}

		previous = next;
		next = next->next;
		current_index++;
	}
}
