#include <ctype.h>

#include <regex.h>

#include <stdio.h>

#include <stdlib.h>

#include <string.h>

int errorCounter = 0;
int lineCounter = 0;
int errorLines[512] = {
    0}; // Initialise error lines array and fill it with zeroes
int variablesStructCounter = 0;
// Create struct to keep used variable names and types
struct nameTypePairs {
  char variableType[50];
  char variableName[50];
};

struct nameTypePairs variables[100];

void validateVlang(const char *line) {
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
  // Compile regex pattern
  regmatch_t matches[5]; // Array to store matches
  reti = regcomp(&regex, pattern_print, REG_EXTENDED);
  if (reti) {
    regfree(&regex);
    return;
  }

  reti = regexec(&regex, line, 2, matches, 0);
  if (!reti) {
    // Extract and print the matched value
    size_t start = matches[1].rm_so;
    size_t end = matches[1].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
    }
    return;
  }

  // Match variable assignments
  reti = regcomp(&regex, pattern_variable_assigned, REG_EXTENDED);
  if (reti) {
    return;
  }

  reti = regexec(&regex, line, 5, matches, 0);
  if (!reti) {
    // Extract and print the matched value
    size_t start = matches[4].rm_so;
    size_t end = matches[4].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      // printf("Variable value: %s\n", match);
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
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      printf("Function name: %s\n", match);
    }
    // Extract and print the function input data type
    start = matches[2].rm_so; // Removed duplicate declaration of start
    end = matches[2].rm_eo;   // Removed duplicate declaration of end
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      printf("Function input data type: %s\n", match);
    }
    // Extract and print the function input variable name
    start = matches[3].rm_so; // Use group 3 for the variable name
    end = matches[3].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
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
      char match[512]; // Assuming a maximum length of  511 characters
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
      char match[512]; // Assuming a maximum length of  511 characters
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
  reti = regexec(&regex, line, 0, matches, 0);
  if (!reti) {
    printf("Match found for else:\n%s\n", line);
    return;
  }

  // Match while statement
  reti = regcomp(&regex, pattern_while, REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Could not compile while loop regex\n");
    return;
  }
  reti = regexec(&regex, line, 2, matches, 0);
  if (!reti) {
    printf("Match found for while loop:\n%s\n", line);
    // Extract and print the matched value
    size_t start = matches[1].rm_so;
    size_t end = matches[1].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      printf("While statement check: %s\n", match);
    }
    return;
  }

  // Match return statement
  reti = regcomp(&regex, pattern_return, REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Could not compile return statement regex\n");
    return;
  }
  reti = regexec(&regex, line, 1, matches, 0);
  if (!reti) {
    printf("Match found for return statement:\n%s\n", line);
    // Extract and print the matched value
    size_t start = matches[1].rm_so;
    size_t end = matches[1].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      printf("Return statement value: %s\n", match);
    }
    return;
  }

  // Match read statement
  reti = regcomp(&regex, pattern_read, REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Could not compile read statement regex\n");
    return;
  }
  reti = regexec(&regex, line, 2, matches, 0);
  if (!reti) {
    printf("Match found for read statement:\n%s\n", line);
    // Extract and print the matched value
    size_t start = matches[1].rm_so;
    size_t end = matches[1].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      printf("Read statement value to be inputted: %s\n", match);
    }
    return;
  }

  // Match variables without assigned values
  reti = regcomp(&regex, pattern_variable_unassigned, REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Could not compile unassigned variable regex\n");
    return;
  }
  reti = regexec(&regex, line, 2, matches, 0);
  if (!reti) {
    printf("Match found for unassigned variable:\n%s\n", line);
    // Extract and print the matched value
    size_t start = matches[2].rm_so;
    size_t end = matches[2].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      printf("Variable with unassigned value: %s\n", match);
    }
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
      errorLines[lineCounter - 1] = lineCounter;
    }
  }
  regfree(&regex);
}

void parseVLang(const char *line, FILE *newFile) {
  regex_t regex;
  int reti;

  // Create an array of used variable names and pairs
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
    // Extract and print the matched value
    size_t start = matches[1].rm_so;
    size_t end = matches[1].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      for (int i = 0; i < variablesStructCounter; i++) {
        if (strcmp(match, variables[i].variableName) == 0) {
          if (strcmp(variables[i].variableType, "string") == 0) {
            fprintf(newFile, "printf(\"%%s\", %s);\n", match);
          } else if (strcmp(variables[i].variableType, "int") == 0) {
            fprintf(newFile, "printf(\"%%d\", %s);\n", match);
          } else if (strcmp(variables[i].variableType, "bool") == 0) {
            fprintf(newFile, "printf(\"%%d\", %s);\n", match);            
          }
        }
      }
    }
    return;
  }

  // Match variable assignments
  reti = regcomp(&regex, pattern_variable_assigned, REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Could not compile variable assignment regex\n");
    return;
  }

  reti = regexec(&regex, line, 5, matches, 0);
  if (!reti) {
    //  Extract variable type, int, bool or char[]
    size_t start = matches[1].rm_so;
    size_t end = matches[1].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      // Compare the match variable with predefined types
      if (strcmp(match, "int") == 0 || strcmp(match, "bool") == 0 ||
          strcmp(match, "string") == 0) {
        strcpy(variables[variablesStructCounter].variableType, match);
      }
    }

    // Extract variable name
    start = matches[2].rm_so;
    end = matches[2].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      fprintf(newFile, "char %s[]", match);
      fprintf(newFile, " = ");
      strcpy(variables[variablesStructCounter].variableName, match);
    }

    // Increment struct iterator
    variablesStructCounter++;

    // Extract variable value
    start = matches[4].rm_so;
    end = matches[4].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      fprintf(newFile, "%s;", match);
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
    // printf("Match found for function declaration:\n%s\n", line);
    //  Extract and print the function name
    size_t start = matches[1].rm_so;
    size_t end = matches[1].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      printf("Function name: %s\n", match);
    }
    // Extract and print the function input data type
    start = matches[2].rm_so; // Removed duplicate declaration of start
    end = matches[2].rm_eo;   // Removed duplicate declaration of end
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      printf("Function input data type: %s\n", match);
    }
    // Extract and print the function input variable name
    start = matches[3].rm_so; // Use group 3 for the variable name
    end = matches[3].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
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
      char match[512]; // Assuming a maximum length of  511 characters
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
      char match[512]; // Assuming a maximum length of  511 characters
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
  reti = regexec(&regex, line, 0, matches, 0);
  if (!reti) {
    printf("Match found for else:\n%s\n", line);
    return;
  }

  // Match while statement
  reti = regcomp(&regex, pattern_while, REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Could not compile while loop regex\n");
    return;
  }
  reti = regexec(&regex, line, 2, matches, 0);
  if (!reti) {
    printf("Match found for while loop:\n%s\n", line);
    // Extract and print the matched value
    size_t start = matches[1].rm_so;
    size_t end = matches[1].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      printf("While statement check: %s\n", match);
    }
    return;
  }

  // Match return statement
  reti = regcomp(&regex, pattern_return, REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Could not compile return statement regex\n");
    return;
  }
  reti = regexec(&regex, line, 1, matches, 0);
  if (!reti) {
    printf("Match found for return statement:\n%s\n", line);
    // Extract and print the matched value
    size_t start = matches[1].rm_so;
    size_t end = matches[1].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      printf("Return statement value: %s\n", match);
    }
    return;
  }

  // Match read statement
  reti = regcomp(&regex, pattern_read, REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Could not compile read statement regex\n");
    return;
  }
  reti = regexec(&regex, line, 2, matches, 0);
  if (!reti) {
    printf("Match found for read statement:\n%s\n", line);
    // Extract and print the matched value
    size_t start = matches[1].rm_so;
    size_t end = matches[1].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      printf("Read statement value to be inputted: %s\n", match);
    }
    return;
  }

  // Match variables without assigned values
  reti = regcomp(&regex, pattern_variable_unassigned, REG_EXTENDED);
  if (reti) {
    fprintf(stderr, "Could not compile unassigned variable regex\n");
    return;
  }
  reti = regexec(&regex, line, 2, matches, 0);
  if (!reti) {
    printf("Match found for unassigned variable:\n%s\n", line);
    // Extract and print the matched value
    size_t start = matches[2].rm_so;
    size_t end = matches[2].rm_eo;
    if (start != -1 && end != -1) {
      char match[512]; // Assuming a maximum length of  511 characters
      strncpy(match, line + start, end - start);
      match[end - start] = '\0';
      printf("Variable with unassigned value: %s\n", match);
    }
    return;
  }
  regfree(&regex);
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
  // Read each line from the input file and call validateVlang function
  while (fgets(line, sizeof(line), oldFile)) {
    validateVlang(line);
  }

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

  // Reset file pointer to the beginning of the file
  fseek(oldFile, 0, SEEK_SET);

  if (errorCounter == 0) {
    printf("\nProceeding to compiling into IL\n");
    fprintf(newFile, "#include <stdio.h>\n#include <stdlib.h>\n#include "
                     "<string.h>\n\nint main() {\n\n"); // Initial IL file setup
    // Read each line from the input file and call validateVlang function
    while (fgets(line, sizeof(line), oldFile)) {
      parseVLang(line, newFile); // Parse VLang to IL
    }
    fprintf(newFile, "\n\n}\n");
  }
  // Close files
  fclose(oldFile);
  fclose(newFile);

  return 0;
}
