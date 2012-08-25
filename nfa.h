
enum state_behavior {
	state_single,
	state_split,
	state_match
};

enum special_chars {
	kleene_star
};


// for single character matches, matching_value will have a character
// for a split state, or match state, matching_value will be uninitialized
struct state {
	enum state_behavior type;
	char matching_value;
	struct state *output;
	struct state *output1;
};

struct state *create_single_state(char matching_value);
struct state *create_split_state();
struct state *create_match_state();

void connect_single_state(struct state *state, struct state *output);
void connect_split_state(struct state *state, struct state *output, struct state *output1);
