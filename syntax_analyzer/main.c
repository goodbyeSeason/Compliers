#include <stdio.h>
#include <string.h>

// change E' to e
// change T' to t

char AT_col[6] = {'E', 'e', 'T', 't', 'F'};
char AT_row[7] = {'i', '+', '*', '(', ')', '#'};
char AT_content [5][6][10] = {
    {"Te", "", "", "Te", "synch", "synch"},
	{"", "+Te", "", "", "#", "#"},
	{"Ft", "synch", "", "Ft", "synch", "synch"},
	{"", "#", "*Ft", "", "#", "#"},
	{"i", "synch", "", "(E)", "synch", "synch"},
};
int is_terminal_char(char x);
int check_table(char x, char a, char * info);
void my_push(char * info);
void print_format(); // just for print format

char stack[100];
int top = 0;
char content[50];
int ip = 0;
int len = 0;

int main(void)
{
    char x = 'E', a;
    int flag = 1;   // decide input-string is ok or not
    char info[10];
    printf("input string:\n");
    scanf("%s", content);
    len = strlen(content);
    if (content[len - 1] != '#') {
        printf("please ended with '#' symbol\n");
        return 0;
    }
    // init stack
    stack[0] = '#';
    stack[1] = 'E';
    top = 2;

    // main struct
    while (x != '#') {
        a = content[ip];
        print_format();
        if (x == a) {
            printf("match: %c\t", x);
            ip++;
            top--;
        } else if (is_terminal_char(x)) {
            printf("terminal char\n");
            top--;
            flag = 0;
        } else if (check_table(x, a, info) == 0) {
            // null item or synch item
            if (strcmp(info, "") == 0) {
                printf("ignore %c\t", a);
                ip++;
            } else {
                printf("equal synch\t");
                top--;
            }
            flag = 0;
        } else if (check_table(x, a, info)) {
            // successful situation
            printf("%c -> %s\t", x, info);
            top--;
            if (strcmp(info, "#") != 0) {
                my_push(info);
            }
        }
        x = stack[top-1];
        if (x == 'e') printf("x: E'\n");
        else if (x == 't') printf("x: T'\n");
        else printf("x: %c\n", x);

    }

    if (flag == 0) {
        printf("invalid input\n");
    } else {
        printf("valid input\n");
    }

}

int is_terminal_char(char x) {
    // if it is a terminal char, return 1, else 0
    int i;
    char term_symbol[5] = {'i', '*', '+', '(', ')'};
    for (i = 0; i < 5; i++) {
        if (x == term_symbol[i]) {
            return 1;
        }
    }
    return 0;
}

int check_table(char x, char a, char * info) {
    int i;
    int idx_col, idx_row;
    // printf("%s\n", AT_col);
    for (i = 0; i < 5; i++) {
        if (x == AT_col[i]) {
            idx_col = i;
            break;
        }
    }
    for (i = 0; i < 6; i++) {
        if (a == AT_row[i]) {
            idx_row = i;
            break;
        }
    }
    memset(info, '\0', sizeof(char) * 10);
    strcpy(info, AT_content[idx_col][idx_row]);
    if (strcmp(info, "") == 0 || strcmp(info, "synch") == 0) {
        return 0;
    }
    return 1;
}

void my_push(char * info) {
    int info_len = strlen(info);
    int i;
    for (i = info_len - 1; i >= 0; i--) {
        stack[top++] = info[i];
    }
}

void print_format() {
    int i, j = 0;
    char str_stack[20];
    char str_rst_cont[20];
    for (i = 0, j = 0; i < top; i++, j++) {
        if (stack[i] == 'e') {
            str_stack[j++] = 'E';
            str_stack[j] = '\'';
        } else if (stack[i] == 't') {
            str_stack[j++] = 'T';
            str_stack[j] = '\'';
        } else {
            str_stack[j] = stack[i];
        }
    }
    str_stack[i] = '\0';
    printf("%-10s", str_stack);

    for (i = 0, j = ip; j < len; i++, j++) {
        str_rst_cont[i] = content[j];
    }
    str_rst_cont[i] = '\0';
    printf("%-10s", str_rst_cont);
}
