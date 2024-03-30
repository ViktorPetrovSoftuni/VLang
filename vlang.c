#include <stdio.h>

void parseVLang(const char *line) {
    regex_t regex;
    int reti;

    // Define regex patterns
    // variable assignment regex
    char *pattern_print = "print (.+)"; // validated, works, group1 is item to be printed
    char *pattern_assignment = "^(int|string|bool) ([a-zA-Z]+)(\\s= )(.+)"; // validated, works, group1 is type, group 2 is name, group 4 is value
}

int main(int argc, char *argv[]) {

    return 0;
}
