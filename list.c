#include <stdlib.h>
#include "list.h"
#include "nfa.h"

struct list *create_list()
{
	struct list *list = malloc(sizeof(struct list));
	list->first = NULL;

	return list;
}

void append_node(struct list *list, struct state *data)
{
	struct node *node = malloc(sizeof(struct node));
	node->data = data;
	node->next = NULL;


	if (list->first == NULL) {
		list->first = node;
	} else {
		struct node *next = list->first;
		
		while (next->next != NULL)
			next = next->next;
	
		next->next = node;
	}
}

void prepend_node(struct list *list, struct state *data)
{
	struct node *node = malloc(sizeof(struct node));
	node->data = data;
	node->next = list->first;

	list->first = node;
}
	

void remove_node(struct list *list, int index)
{
	if (list->first == NULL)
		return;
	
	if (index == 0) {
		list->first = list->first->next;
		return;
	}

	struct node *previous = list->first;
	struct node *next = list->first->next;
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
