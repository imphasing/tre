#include "nfa.h"

int main(int argc, char **argv);
bool nfa_matches(char *string, struct state *nfa);

char peek();
bool eat(char c);
char next();
bool more();

struct state *parse_regex();
struct state *parse_term();
struct state *parse_factor();
struct state *parse_base();


