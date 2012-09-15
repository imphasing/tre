enum state_behavior {
	state_single,
	state_split
};

enum special_chars {
	any_char
};


// for single character matches, matching_value will have a character
// for a split state, or match state, matching_value will be uninitialized

typedef struct state state;
struct state {
	enum state_behavior type;
	char matching_value;

	state *output;
	state *output1;
};

typedef struct fragment fragment;
struct fragment {
	state *start;

	int num_dangling;
	state ***dangling;
};


state *create_single_state(char matching_value);
state *create_split_state();
fragment *create_fragment(state *start, int num_dangling, state ***dangling); 
