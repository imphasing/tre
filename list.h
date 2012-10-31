#define INITIAL_SIZE 100000

typedef struct state_list state_list;
struct state_list {
	state **list;
	int capacity;
	int next_index;
};

state_list *list_create();
void list_append(state_list *to_append, state *data);
state *list_get_index(state_list *to_index, int index);
void list_clear(state_list *to_clear);
void list_grow(state_list *to_grow);
