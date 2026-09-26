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
	Variable* existing = find_variable(*variables, *count, name);
	if (existing != NULL) {
		existing->value = value;
		return;
	}

	*variables = realloc(*variables, (*count + 1) * sizeof(Variable));
	(*variables)[*count].name = strdup(name);
	(*variables)[*count].value = value;
	(*count)++;
}

Variable* require_variable(Variable* variables, int count, char* name, size_t line_number) {
	Variable* variable = find_variable(variables, count, name);
	if (variable == NULL) {
		printf("Error: Variable '%s' is undefined (line %zu)\n", name, line_number);
		exit(EXIT_FAILURE);
	}
	return variable;
}

void print_help(char* program_name) {
	printf("Usage: %s <filename|[options]>\n", program_name);
	printf("Options:\n");
	printf("--doc: Prints the commands for the language\n");
	printf("--help: Prints this help message and exits\n");
}

void print_doc() {
	printf("Commands:\n");
	printf("  add: Adds a value to a variable (a = a + b)\n");
	printf("  sub: Subtracts a value from a variable (a = a - b)\n");
	printf("  mul: Multiplies a variable by a value (a = a * b)\n");
	printf("  div: Divides a variable by a value (a = a / b)\n");
	printf("  print: Prints a string\n");
	printf("  var: Creates or resets a variable with a value\n");
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

	if (argc != 2) {
		print_help(argv[0]);
		return EXIT_SUCCESS;
	}

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
		size_t line_number = 0;

		while (getline(&line, &line_capacity, file_pointer) != -1) {
			line_number++;

			size_t length = strlen(line);
			while (length > 0 && (line[length - 1] == '\n' || line[length - 1] == '\r')) {
				line[--length] = '\0';
			}

			if (line[0] == '#') {
				continue;
			}

			char* command = strtok(line, " ");
			if (command == NULL) {
				continue;
			}

			if (strcmp(command, "add") == 0 || strcmp(command, "sub") == 0 ||
				strcmp(command, "mul") == 0 || strcmp(command, "div") == 0) {
				char* first = strtok(NULL, " ");
				char* second = strtok(NULL, " ");

				if (first == NULL || second == NULL) {
					printf("Error: Invalid arguments for `%s` on line %zu\n", command, line_number);
					exit(EXIT_FAILURE);
				}

				Variable* target = require_variable(variables, variable_count, first, line_number);
				int second_value = get_value(variables, variable_count, second);

				if (strcmp(command, "add") == 0) target->value += second_value;
				else if (strcmp(command, "sub") == 0) target->value -= second_value;
				else if (strcmp(command, "mul") == 0) target->value *= second_value;
				else {
					if (second_value == 0) {
						printf("Error: Division by zero on line %zu\n", line_number);
						exit(EXIT_FAILURE);
					}
					target->value /= second_value;
				}
			}
			else if (strcmp(command, "print") == 0) {
				char* string = strtok(NULL, "");

				if (string == NULL) {
					printf("Error: Invalid arguments for `print` on line %zu\n", line_number);
					exit(EXIT_FAILURE);
				}

				printf("%s\n", string);
			}
			else if (strcmp(command, "var") == 0) {
				char* name = strtok(NULL, " ");
				char* value = strtok(NULL, " ");

				if (name == NULL || value == NULL) {
					printf("Error: Invalid arguments for `var` on line %zu\n", line_number);
					exit(EXIT_FAILURE);
				}

				add_variable(&variables, &variable_count, name, atoi(value));
			}
			else if (strcmp(command, "let") == 0) {
				char* name = strtok(NULL, " ");
				char* value = strtok(NULL, " ");

				if (name == NULL || value == NULL) {
					printf("Error: Invalid arguments for `let` on line %zu\n", line_number);
					exit(EXIT_FAILURE);
				}

				Variable* variable = require_variable(variables, variable_count, name, line_number);
				variable->value = atoi(value);
			}
			else if (strcmp(command, "get") == 0) {
				char* name = strtok(NULL, " ");

				if (name == NULL) {
					printf("Error: Invalid arguments for `get` on line %zu\n", line_number);
					exit(EXIT_FAILURE);
				}

				Variable* variable = require_variable(variables, variable_count, name, line_number);
				printf("%d\n", variable->value);
			}
			else if (strcmp(command, "end") == 0) {
				break;
			}
			else {
				printf("Error: Invalid command '%s' on line %zu\n", command, line_number);
				exit(EXIT_FAILURE);
			}
		}

		free(line);
		fclose(file_pointer);

		for (int i = 0; i < variable_count; i++) free(variables[i].name);
		free(variables);
	}

	return EXIT_SUCCESS;
}
