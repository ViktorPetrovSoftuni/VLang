#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <ctype.h>

void parseVLang(const char *line) {
    regex_t regex;
    int reti;

    // Define regex patterns
    // variable assignment regex
    char *pattern_print = "print (.+)"; // validated, works, group1 is item to be printed
    char *pattern_assignment = "^(int|string|bool) ([a-zA-Z]+)(\\s= )(.+)"; // validated, works, group1 is type, group 2 is name, group 4 is value

    // function regex
    char *pattern_function = "^function ([a-zA-Z]+) (int|string|bool) ([a-zA-Z]+):"; // validated, works, group1 is function name, group2 is input data type, group3 is name of input

    //function end regex
    char *pattern_end_function = "end ([a-zA-Z]+)"; //validated, works, group1 is function name to end

    //if statement regex
    char *pattern_if = "\\s*if\\s+([^:]+):\\s*"; // validated, works, group1 is if check statement

    //elseif statement regex
    char *pattern_else_if = "\\s*elseif\\s+([^:]+):\\s*"; //validated, group 1 is elseif check statement

    //else statement regex
    char *pattern_else = "(else:)"; // validated, to be reviewed

    //while statement regex
    char *pattern_while = "while (.+):";

    //return statement regex
    char *pattern_return = "return (.+)"; // validated, works, group1 is return value

        // Compile regex pattern
    regmatch_t matches[5]; // Array to store matches
    reti = regcomp(&regex, pattern_print, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile print statement regex\n");
        regfree(&regex);
        return;
    }

    reti = regexec(&regex, line, 2, matches, 0);
    if (!reti) {
        printf("Match found for print statement:\n%s", line);
        // Extract and print the matched value
        size_t start = matches[1].rm_so;
        size_t end = matches[1].rm_eo; 
        if (start != -1 && end != -1) {
            char match[256]; // Assuming a maximum length of 255 characters
            strncpy(match, line + start, end - start);
            match[end - start] = '\0';
            printf("Value to be printed: %s\n", match);
        }
        return;
    }
    
    // Match variable assignments
    reti = regcomp(&regex, pattern_assignment, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile variable assignment regex\n");
        return;
    }

    reti = regexec(&regex, line, 5, matches, 0);
    if (!reti) {
        printf("Match found for variable assignment:\n%s", line);
        // Extract and print the matched value
        size_t start = matches[4].rm_so;
        size_t end = matches[4].rm_eo;
        if (start != -1 && end != -1) {
            char match[256]; // Assuming a maximum length of 255 characters
            strncpy(match, line + start, end - start);
            match[end - start] = '\0';
            printf("Variable value: %s\n", match);
        }

        return;
    }
    
    // Match functions
    reti = regcomp(&regex, pattern_function, REG_EXTENDED);
    if (reti) {
        fprintf(stderr, "Could not compile function regex\n");
        return;
    }
    reti = regexec(&regex, line, 3, matches, 0);
    if (!reti) {
        printf("Match found for function declaration:\n%s\n", line);
        return;
    }
}

int main(int argc, char *argv[]) {
    FILE *oldFile;

    // Check if correct number of command line arguments are provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s inputFile\n", argv[0]);
        return 1;
    }

    // Open oldFile in read mode
    oldFile = fopen(argv[1], "r");
    if (oldFile == NULL) {
        fprintf(stderr, "Error opening %s for reading\n", argv[1]);
        return 1;
    }

    char line[1000];
    // Read each line from the input file and call parseVLang function
    while (fgets(line, sizeof(line), oldFile)) {
        parseVLang(line);
    }

    // Close the input file
    fclose(oldFile);

    printf("\nParsing successful\n");

    return 0;
}