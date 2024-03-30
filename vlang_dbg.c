#include <ctype.h>

#include <regex.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

int errorCounter = 0;

void parseVLang(const char *line) {
  regex_t regex;
  int reti;

  // Define regex patterns
  // variable assignment regex
  char *pattern_print =
      "print (.+)"; // validated, works, group1 is item to be printed

  char *pattern_assignment =
      "(int|string|bool) ([a-zA-Z]+)(\\s= )(.+)"; // validated, works, group1 is
                                                  // type, group 2 is name,
                                                  // group 4 is value

  // function regex
  char *pattern_function = "\\bfunction ([a-zA-Z]+) (int|string|bool) "
                           "([a-zA-Z]+):"; // Updated with \b word boundary
                                           // anchor, group1 is function name,
                                           // group2 is input data type, group3
                                           // is name of input

  // function end regex
  char *pattern_end_function =
      "\\bend (\\w+)"; // Updated with \b word boundary anchor, group1 is
                       // function name to end

  // if statement regex
  char *pattern_if = "\\s*if\\s+([^:]+):\\s*"; // validated, works, group1 is if
                                               // check statement

  // elseif statement regex
  char *pattern_else_if =
      "\\s*elseif\\s+([^:]+):\\s*"; // validated, group 1 is elseif check
                                    // statement

  // else statement regex
  char *pattern_else = "(else):"; // validated

  // while statement regex
  char *pattern_while = "while (.+):";

  // return statement regex
  char *pattern_return =
      "return (.+)"; // validated, works, group1 is return value

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
      char match[512]; // Assuming a maximum lenght of  511 characters
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
      char match[512]; // Assuming a maximum lenght of  511 characters
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
  reti =
      regexec(&regex, line, 4, matches,
              0); // Increased to 4 to capture the entire match and three groups
  if (!reti) {
    printf("Match found for function declaration:\n%s\n", line);
    // Extract and print the function name
    size_t start = matches[1].rm_so;
    size_t end = matches[1].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum lenght of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      printf("Function name: %s\n", match);
    }
    // Extract and print the function input data type
    start = matches[2].rm_so; // Removed duplicate declaration of start
    end = matches[2].rm_eo;   // Removed duplicate declaration of end
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum lenght of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      printf("Function input data type: %s\n", match);
    }
    // Extract and print the function input variable name
    start = matches[3].rm_so; // Use group 3 for the variable name
    end = matches[3].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum lenght of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      printf("Function input variable name: %s\n", match);
    }
    return;
  }

  // Match function block ends
  reti = regcomp(&regex, pattern_end_function, REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Could not compile end function regex\n");
    return;
  }
  reti = regexec(&regex, line, 2, matches, 0);
  if (!reti) {
    printf("Match found for function end:\n%s\n", line);
    // Extract and print the matched value
    size_t start = matches[1].rm_so;
    size_t end = matches[1].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum lenght of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      printf("Name of function to be ended: %s\n", match);
    }
    return;
  }

  // Match elseif statement
  reti = regcomp(&regex, pattern_else_if, REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Could not compile else-if statement regex\n");
    return;
  }
  reti = regexec(&regex, line, 0, NULL, 0);
  if (!reti) {
    printf("Match found for else-if statement:\n%s\n", line);
    return;
  }

  // Match if statement
  reti = regcomp(&regex, pattern_if, REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Could not compile if statement regex\n");
    return;
  }
  reti = regexec(&regex, line, 2, matches, REG_EXTENDED);
  if (!reti) {
    printf("Match found for if statement:\n%s\n", line);
    // Extract and print the matched value
    size_t start = matches[1].rm_so;
    size_t end = matches[1].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum lenght of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      printf("If statement check: %s\n", match);
    }
    return;
  }

  // Match else statement
  reti = regcomp(&regex, pattern_else, REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Could not compile else statement regex\n");
    return;
  }
  reti = regexec(&regex, line, 0, NULL, 0);
  if (!reti) {
    printf("Match found for else:\n%s\n", line);
    return;
  }

  // Check for code that was not detected by the regex parser
  if (strcmp(line, "") != 0) {
    int hasAlphanumeric = 0; // Assume initially no alphanumeric characters
    for (int i = 0; line[i] != '\0'; i++) {
      if (isalnum(line[i])) {
        hasAlphanumeric = 1; // Found an alphanumeric character
        break;               // No need to continue checking
      }
    }
    if (hasAlphanumeric >= 1) {
      printf("No code found on line\n%s\n", line);
      errorCounter++; // Add an error to the error counter
    }
  }
  regfree(&regex);
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
  if (errorCounter == 1) {
    printf("\nYour code has an error! Fix it!\n");
  } else if (errorCounter > 1) {
    printf("\nYou have %d errors! Fix them!\n", errorCounter);
  } else {
    printf("\nParsing successful\n");
  }

  return 0;
}