#include <stdlib.h>
#include "nfa.h"
#include "list.h"

state_list *list_create()
{
	state_list *new_list = malloc(sizeof(state_list));
	new_list->capacity = INITIAL_SIZE;
	new_list->next_index = 0;
	new_list->list = malloc(sizeof(state *) * INITIAL_SIZE);

	return new_list;
}

void list_append(state_list *to_append, state *data)
{
	if (to_append->next_index >= to_append->capacity)
		list_grow(to_append);
	
	to_append->list[to_append->next_index] = data;
	to_append->next_index++;
}

void list_clear(state_list *to_clear)
{
	to_clear->next_index = 0;
}

void list_grow(state_list *to_grow)
{
	to_grow->list = realloc(to_grow->list, (sizeof(state **) * to_grow->capacity) * 2);
	to_grow->capacity *= 2;
}
