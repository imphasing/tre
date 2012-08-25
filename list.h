
struct list {
	struct node *first;
};

struct node {
	struct state *data;
	struct node *next;
};


struct list *create_list();
void append_node(struct list *list, struct state *data);
void prepend_node(struct list *list, struct state *data);
void remove_node(struct list *list, int index);
