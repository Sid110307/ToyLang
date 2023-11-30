#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	char* name;
	int value;
} Variable;

Variable* find_variable(Variable* variables, int count, char* name) {
	for (int i = 0; i < count; i++) {
		if (strcmp(variables[i].name, name) == 0) {
			return &variables[i];
		}
	}
	return NULL;
}

int get_value(Variable* variables, int count, char* name) {
	if (isdigit(name[0])) {
		return atoi(name);
	}
	else {
		Variable* variable = find_variable(variables, count, name);
		if (variable != NULL) {
			return variable->value;
		}
		else {
			printf("Error: Variable '%s' is undefined\n", name);
			exit(EXIT_FAILURE);
		}
	}
}

void add_variable(Variable** variables, int* count, char* name, int value) {
	*variables = realloc(*variables, (*count + 1) * sizeof(Variable));
	(*variables)[*count].name = strdup(name);
	(*variables)[*count].value = value;
	(*count)++;
}

void print_help(char* program_name) {
	printf("Usage: %s <filename|[options]>\n", program_name);
	printf("Options:\n");
	printf("--doc: Prints the commands for the language\n");
	printf("--help: Prints this help message and exits\n");
}

void print_doc() {
	printf("Commands:\n");
	printf("  add: Adds two numbers\n");
	printf("  sub: Subtracts two numbers\n");
	printf("  mul: Multiplies two numbers\n");
	printf("  div: Divides two numbers\n");
	printf("  print: Prints a string\n");
	printf("  var: Creates a variable with a value\n");
	printf("  let: Reassigns the value of a variable\n");
	printf("  get: Prints the value of a variable\n");
	printf("  end: Ends the program\n");
	printf("  #: Comment\n");
	printf("\nExample file (.toy extension):\n");
	printf("  var a 2\n");
	printf("  add a 0\n");
	printf("  let a 5\n");
	printf("  add a 1\n");
	printf("  get a\n");
	printf("  end\n");
	printf("\nOutput:\n");
	printf("  6\n");
}

int main(int argc, char** argv) {
	Variable* variables = NULL;
	int variable_count = 0;

	if (argc == 2) {
		if (strcmp(argv[1], "--help") == 0) {
			print_help(argv[0]);
		}
		else if (strcmp(argv[1], "--doc") == 0) {
			print_doc();
		}
		else {
			FILE* file_pointer = fopen(argv[1], "r");
			if (file_pointer == NULL) {
				printf("Error: Could not open file '%s'\n", argv[1]);
				exit(EXIT_FAILURE);
			}

			char* line = NULL;
			size_t line_capacity = 0;
			size_t line_number = 1;

			while (getline(&line, &line_capacity, file_pointer) != -1) {
				if (line[0] == '#') {
					continue;
				}

				char* command = strtok(line, " \n");
				if (command == NULL) {
					continue;
				}

				if (strcmp(command, "add") == 0) {
					char* first = strtok(NULL, " \n");
					char* second = strtok(NULL, " \n");

					if (first == NULL || second == NULL) {
						printf("Error: Invalid arguments for `add` on line %zu\n", line_number);
						exit(EXIT_FAILURE);
					}

					int first_value = get_value(variables, variable_count, first);
					int second_value = get_value(variables, variable_count, second);

					printf("%d\n", first_value + second_value);
				}
				else if (strcmp(command, "sub") == 0) {
					char* first = strtok(NULL, " \n");
					char* second = strtok(NULL, " \n");

					if (first == NULL || second == NULL) {
						printf("Error: Invalid arguments for `sub` on line %zu\n", line_number);
						exit(EXIT_FAILURE);
					}

					int first_value = get_value(variables, variable_count, first);
					int second_value = get_value(variables, variable_count, second);

					printf("%d\n", first_value - second_value);
				}
				else if (strcmp(command, "mul") == 0) {
					char* first = strtok(NULL, " \n");
					char* second = strtok(NULL, " \n");

					if (first == NULL || second == NULL) {
						printf("Error: Invalid arguments for `mul` on line %zu\n", line_number);
						exit(EXIT_FAILURE);
					}

					int first_value = get_value(variables, variable_count, first);
					int second_value = get_value(variables, variable_count, second);

					printf("%d\n", first_value * second_value);
				}
				else if (strcmp(command, "div") == 0) {
					char* first = strtok(NULL, " \n");
					char* second = strtok(NULL, " \n");

					if (first == NULL || second == NULL) {
						printf("Error: Invalid arguments for `div` on line %zu\n", line_number);
						exit(EXIT_FAILURE);
					}

					int first_value = get_value(variables, variable_count, first);
					int second_value = get_value(variables, variable_count, second);

					printf("%d\n", first_value / second_value);
				}
				else if (strcmp(command, "print") == 0) {
					char* string = strtok(NULL, "\n");

					if (string == NULL) {
						printf("Error: Invalid arguments for `print` on line %zu\n", line_number);
						exit(EXIT_FAILURE);
					}

					printf("%s\n", string);
				}
				else if (strcmp(command, "var") == 0) {
					char* name = strtok(NULL, " \n");
					char* value = strtok(NULL, " \n");

					if (name == NULL || value == NULL) {
						printf("Error: Invalid arguments for `var` on line %zu\n", line_number);
						exit(EXIT_FAILURE);
					}

					add_variable(&variables, &variable_count, name, atoi(value));
				}
				else if (strcmp(command, "let") == 0) {
					char* name = strtok(NULL, " \n");
					char* value = strtok(NULL, " \n");

					if (name == NULL || value == NULL) {
						printf("Error: Invalid arguments for `let` on line %zu\n", line_number);
						exit(EXIT_FAILURE);
					}

					Variable* variable = find_variable(variables, variable_count, name);
					if (variable != NULL) {
						variable->value = atoi(value);
					}
					else {
						printf("Error: Variable '%s' is undefined\n", name);
						exit(EXIT_FAILURE);
					}
				}
				else if (strcmp(command, "get") == 0) {
					char* name = strtok(NULL, " \n");

					if (name == NULL) {
						printf("Error: Invalid arguments for `get` on line %zu\n", line_number);
						exit(EXIT_FAILURE);
					}

					Variable* variable = find_variable(variables, variable_count, name);
					if (variable != NULL) {
						printf("%d\n", variable->value);
					}
					else {
						printf("Error: Variable '%s' is undefined\n", name);
						exit(EXIT_FAILURE);
					}
				}
				else if (strcmp(command, "end") == 0) {
					break;
				}
				else {
					printf("Error: Invalid command '%s' on line %zu\n", command, line_number);
					exit(EXIT_FAILURE);
				}

				line_number++;
			}

			free(line);
			fclose(file_pointer);
		}
	}

	return EXIT_SUCCESS;
}
