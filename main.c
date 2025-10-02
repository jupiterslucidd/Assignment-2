#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

void oct_to_bin(const char *oct, char *out);
void oct_to_hex(const char *oct, char *out);
void hex_to_bin(const char *hex, char *out);
void to_sign_magnitude(int32_t n, char *out);
void to_ones_complement(int32_t n, char *out);
void to_twos_complement(int32_t n, char *out);

#define MAX_LINE 256
#define MAX_OUT 256

typedef struct {
    char function[50];
    char input1[100];
    char expected[200];
} TestCase;

int parse_line(char *line, TestCase *t) {
    if (line[0] == '#' || strlen(line) < 2) return 0; // ignore comments/blank lines

    char *tok = strtok(line, " \t\n");
    if (!tok) return 0;
    strcpy(t->function, tok);

    tok = strtok(NULL, " \t\n");
    if (!tok) return 0;
    strcpy(t->input1, tok);

    tok = strtok(NULL, " \t\n");
    if (!tok) return 0;
    strcpy(t->expected, tok);

    return 1;
}



int main(void) {
    FILE *file = fopen("a2_test.txt", "r");
    if (!file) {
        printf("Error: Could not open a2_test.txt\n");
        return 1;
    }

    char line[MAX_LINE];
    int testCount = 0;
    int passCount = 0;

    while (fgets(line, sizeof(line), file)) {
        TestCase t;
        if (!parse_line(line, &t)) continue;

        char actual[MAX_OUT] = "";
        int32_t n;

        if (strcmp(t.function, "oct_to_bin") == 0) {
            oct_to_bin(t.input1, actual);
        } else if (strcmp(t.function, "oct_to_hex") == 0) {
            oct_to_hex(t.input1, actual);
        } else if (strcmp(t.function, "hex_to_bin") == 0) {
            hex_to_bin(t.input1, actual);
        } else if (strcmp(t.function, "to_sign_magnitude") == 0) {
            n = atoi(t.input1);
            to_sign_magnitude(n, actual);
        } else if (strcmp(t.function, "to_ones_complement") == 0) {
            n = atoi(t.input1);
            to_ones_complement(n, actual);
        } else if (strcmp(t.function, "to_twos_complement") == 0) {
            n = atoi(t.input1);
            to_twos_complement(n, actual);
        } else {
            printf("Unknown function: %s\n", t.function);
            continue;
        }

        int ok = strcmp(actual, t.expected) == 0;
        printf("Test %d: %s(\"%s\") -> Expected: \"%s\", Got: \"%s\" [%s]\n",
               testCount + 1, t.function, t.input1,
               t.expected, actual, ok ? "PASS" : "FAIL");

        if (ok) passCount++;
        testCount++;
    }

    fclose(file);

    printf("\nSummary: %d/%d tests passed\n", passCount, testCount);
    return 0;
}

