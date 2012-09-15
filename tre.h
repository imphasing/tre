#include <stdbool.h>

int main(int argc, char **argv);
bool nfa_matches(char *string, state *nfa);

char peek();
bool eat(char c);
char next();
bool more();

fragment *parse_regex();
fragment *parse_term();
fragment *parse_factor();
fragment *parse_base();
