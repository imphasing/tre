#include <stdlib.h>
#include "nfa.h"
#include "list.h"

list *create_list()
{
	list *list = malloc(sizeof(list));
	list->first = NULL;

	return list;
}

void append_node(list *list, state *data)
{
	node *to_append = malloc(sizeof(node));
	to_append->data = data;
	to_append->next = NULL;

	if (list->first == NULL) {
		list->first = to_append;
	} else {
		node *next = list->first;
		while (next->next != NULL)
			next = next->next;
	
		next->next = to_append;
	}
}

void prepend_node(list *list, state *data)
{
	node *node = malloc(sizeof(node));
	node->data = data;
	node->next = list->first;

	list->first = node;
}
	

void remove_node(list *list, int index)
{
	if (list->first == NULL)
		return;
	
	if (index == 0) {
		list->first = list->first->next;
		return;
	}

	node *previous = list->first;
	node *next = list->first->next;
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
