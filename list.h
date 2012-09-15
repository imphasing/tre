typedef struct node node;
struct node {
	state *data;
	node *next;
};

typedef struct list list;
struct list {
	 node *first;
};


list *create_list();
void append_node(list *list, state *data);
void prepend_node(list *list, state *data);
void remove_node(list *list, int index);
