#include <ctype.h>

#include <regex.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>


void parseVLang(const char *line) {
  regex_t regex;
  int reti;

  // Define regex patterns
  // variable assignment regex
  char *pattern_print =
      "print (.+)"; // validated, works, group1 is item to be printed

  char *pattern_variable_assigned =
      "(int|string|bool) ([a-zA-Z0-9]+)(\\s= )(.+)"; // validated, works, group1
                                                     // is type, group 2 is
                                                     // name, group 4 is value

  char *pattern_variable_unassigned = "(int|string|bool) ([a-zA-Z0-9]+)";
  // function regex
  char *pattern_function = "\\bfunction ([a-zA-Z]+) (int|string|bool) "
                           "([a-zA-Z]+):"; // Updated with \b word boundary ,
                                           // group1 is function name, group2 is
                                           // input data type, group3 is name of
                                           // input

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
  char *pattern_while = "while (.+):"; // validated, group1 is while statement

  // return statement regex
  char *pattern_return =
      "return (.+)"; // validated, works, group1 is return value
  // read statement regex
  char *pattern_read =
      "read (.+)"; // validated, works, group1 is the information to be read
}

int main(int argc, char *argv[]) {
  FILE *oldFile, *newFile;

  // Check if correct number of command line arguments are provided
  if (argc != 3) {
    fprintf(stderr, "Usage: %s oldFile newFile\n", argv[0]);
    return 1;
  }

  // Open oldFile in read mode and newFile in write mode
  oldFile = fopen(argv[1], "r");
  if (oldFile == NULL) {
    fprintf(stderr, "Error opening %s for reading\n", argv[1]);
    return 1;
  }
  // Concatenate ".c" extension to the new file name
  char newFileName[100];
  sprintf(newFileName, "%s.c", argv[2]);

  // Open newFile with ".c" extension for writing
  newFile = fopen(newFileName, "w");
  if (newFile == NULL) {
    fprintf(stderr, "Error opening %s for writing\n", newFileName);
    fclose(oldFile);
    return 1;
  }

  char line[1000];
  // Read each line from the input file and call parseVLang function
  while (fgets(line, sizeof(line), oldFile)) {
    parseVLang(line);
  }

  // Close files
  fclose(newFile);

  // Close the input file
  fclose(oldFile);
  if (errorCounter == 1) {
    printf("\nYour code has an error! Fix it!\n");
    printf("Error is on line %d\n", errorLines[0]);
  } else if (errorCounter > 1) {
    printf("\nYou have %d errors! Fix them!\n", errorCounter);
    for (int i = 0; i < 512; i++) {
      if (errorLines[i] == 0) {
        continue;
      }
      printf("Error is on line %d\n", errorLines[i]);
    }
  } else {
    printf("\nParsing successful\n");
  }

  return 0;
}
