#include <stdio.h>
#include <string.h>

char stack[100], input[100];
int top = -1, i = 0;

void print_stack_input(const char* action) {
    printf("%-15s Stack: %-20s Input: %s\n", action, stack, input + i);
}

void shift() {
    stack[++top] = input[i++];
    stack[top+1] = '\0';
    if (input[i-1] == 'i' && input[i] == 'd') {  // Handle "id" as a token
        stack[++top] = input[i++];
        stack[top+1] = '\0';
    }
    print_stack_input("Shift");
}

int reduce() {
    if (top >= 1 && stack[top-1] == 'i' && stack[top] == 'd') {
        stack[top-1] = 'E';
        top--;
        stack[top+1] = '\0';
        print_stack_input("Reduce id->E");
        return 1;
    } else if (top >= 2 && stack[top-2] == 'E' &&
               (stack[top-1] == '+' || stack[top-1] == '*') &&
                stack[top] == 'E') {
        stack[top-2] = 'E';
        top -= 2;
        stack[top+1] = '\0';
        print_stack_input("Reduce E+E/E*E->E");
        return 1;
    }
    return 0;
}

int main() {
    printf("Enter the input (e.g., id+id*id): ");
    scanf("%s", input);

    printf("\n%-15s %-25s %s\n", "Action", "Stack", "Remaining Input");
    while (input[i] != '\0') {
        shift();
        while (reduce());
    }

    while (reduce());

    if (strcmp(stack, "E") == 0)
        printf("\nString Accepted!\n");
    else
        printf("\nString Rejected!\n");

    return 0;
}


shift 



#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX 50

typedef struct { char nt, lead[MAX], trail[MAX]; int lc, tc; } Set;

Set sets[MAX] = {{'E'}, {'T'}, {'F'}}; // Example non-terminals
int n = 3; // Number of non-terminals

bool isT(char c) { return c < 'A' || c > 'Z'; }

int find(char c) { for(int i=0; i<n; i++) if(sets[i].nt == c) return i; return -1; }

void addLead(char nt, char t) {
    int i = find(nt);
    if(!strchr(sets[i].lead, t)) sets[i].lead[sets[i].lc++] = t;
}

void addTrail(char nt, char t) {
    int i = find(nt);
    if(!strchr(sets[i].trail, t)) sets[i].trail[sets[i].tc++] = t;
}

void compute() {
    char prods[][20] = {"E->E+T", "E->T", "T->T*F", "T->F", "F->(E)", "F->id"}; // Example grammar
    bool changed;
    do {
        changed = false;
        for(int p=0; p<6; p++) {
            char lhs = prods[p][0], *rhs = prods[p]+3;
            // Leading
            for(int i=0; rhs[i]; i++) {
                if(isT(rhs[i])) { if(!strchr(sets[find(lhs)].lead, rhs[i])) { addLead(lhs, rhs[i]); changed=true; } break; }
                else for(int j=0; j<sets[find(rhs[i])].lc; j++) if(!strchr(sets[find(lhs)].lead, sets[find(rhs[i])].lead[j])) { addLead(lhs, sets[find(rhs[i])].lead[j]); changed=true; }
            }
            // Trailing
            for(int i=strlen(rhs)-1; i>=0; i--) {
                if(isT(rhs[i])) { if(!strchr(sets[find(lhs)].trail, rhs[i])) { addTrail(lhs, rhs[i]); changed=true; } break; }
                else for(int j=0; j<sets[find(rhs[i])].tc; j++) if(!strchr(sets[find(lhs)].trail, sets[find(rhs[i])].trail[j])) { addTrail(lhs, sets[find(rhs[i])].trail[j]); changed=true; }
            }
        }
    } while(changed);
}

int main() {
    compute();
    for(int i=0; i<n; i++) {
        printf("%c: Leading {", sets[i].nt);
        for(int j=0; j<sets[i].lc; j++) printf("%c ", sets[i].lead[j]);
        printf("} Trailing {");
        for(int j=0; j<sets[i].tc; j++) printf("%c ", sets[i].trail[j]);
        printf("}\n");
    }
    return 0;
}


leading 



#include <stdio.h>
#include <string.h>

typedef struct { char lhs, rhs[10]; int dot; } Item;
typedef struct { Item items[10]; int count; } State;

State states[20]; int n_states = 0;
char* grammar[] = {"E=E+T","E=T","T=T*F","T=F","F=(E)","F=id"};
int n_prod = 6;

void closure(State* s) {
    int added;
    do {
        added = 0;
        for(int i=0; i<s->count; i++) {
            char sym = s->items[i].rhs[s->items[i].dot];
            if(sym >= 'A' && sym <= 'Z') {
                for(int j=0; j<n_prod; j++) {
                    if(grammar[j][0] == sym) {
                        Item new = {sym, "", 0};
                        strcpy(new.rhs, grammar[j]+2);
                        int found = 0;
                        for(int k=0; k<s->count; k++)
                            if(!memcmp(&s->items[k], &new, sizeof(Item))) found=1;
                        if(!found) { s->items[s->count++] = new; added=1; }
                    }
                }
            }
        }
    } while(added);
}

State goto_state(State* s, char sym) {
    State new = {0};
    for(int i=0; i<s->count; i++)
        if(s->items[i].rhs[s->items[i].dot] == sym)
            new.items[new.count] = s->items[i], new.items[new.count++].dot++;
    closure(&new);
    return new;
}

int find_state(State* s) {
    for(int i=0; i<n_states; i++) {
        if(states[i].count != s->count) continue;
        int match = 1;
        for(int j=0; j<s->count; j++) {
            int found = 0;
            for(int k=0; k<states[i].count; k++)
                if(!memcmp(&states[i].items[k], &s->items[j], sizeof(Item))) { found=1; break; }
            if(!found) { match=0; break; }
        }
        if(match) return i;
    }
    return -1;
}

void build_parser() {
    State s0 = {0};
    s0.items[0] = (Item){'E', "E+T", 0}; s0.count = 1;
    closure(&s0); states[n_states++] = s0;

    for(int i=0; i<n_states; i++) {
        for(char sym='('; sym<='z'; sym++) {
            if(sym=='('||sym==')'||sym=='i'||sym=='+'||sym=='*') {
                State new = goto_state(&states[i], sym);
                if(new.count > 0 && find_state(&new) == -1)
                    states[n_states++] = new;
            }
        }
    }
}

void print_items() {
    for(int i=0; i<n_states; i++) {
        printf("State %d:\n", i);
        for(int j=0; j<states[i].count; j++) {
            Item it = states[i].items[j];
            printf("%c -> %.*s.%s\n", it.lhs, it.dot, it.rhs, it.rhs+it.dot);
        }
    }
}

int main() { build_parser(); print_items(); return 0; }


lr0






#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct Quad { char op[2], a1[5], a2[5], res[5]; } q[20];
struct Tri { char op[2], a1[5], a2[5]; } t[20];
int ptr[20], qi = 0, ti = 0;

char* tempVar() { static int n = 1; static char t[5]; sprintf(t, "t%d", n++); return t; }
int prec(char c) { return (c == '+' || c == '-') ? 1 : (c == '*' || c == '/') ? 2 : 0; }

void genIC(char* e) {
    char op[10], val[10][5]; int ot = -1, vt = -1;
    for (int i = 2; e[i]; i++) {
        if (isalnum(e[i])) strcpy(val[++vt], (char[]){e[i], 0});
        else if (strchr("+-*/", e[i])) {
            while (ot >= 0 && prec(op[ot]) >= prec(e[i])) {
                char *r = tempVar(); strcpy(q[qi].op, (char[]){op[ot--], 0});
                strcpy(q[qi].a2, val[vt--]); strcpy(q[qi].a1, val[vt--]);
                strcpy(q[qi].res, r); strcpy(val[++vt], r); qi++;
            }
            op[++ot] = e[i];
        }
    }
    while (ot >= 0) {
        char *r = tempVar(); strcpy(q[qi].op, (char[]){op[ot--], 0});
        strcpy(q[qi].a2, val[vt--]); strcpy(q[qi].a1, val[vt--]);
        strcpy(q[qi].res, r); strcpy(val[++vt], r); qi++;
    }
    strcpy(q[qi].op, "="); strcpy(q[qi].a1, val[vt]);
    strcpy(q[qi].res, (char[]){e[0], 0}); qi++;
}

void show() {
    printf("\nQUADRUPLES:\n"); for (int i = 0; i < qi; i++)
        printf("%d: (%s, %s, %s, %s)\n", i, q[i].op, q[i].a1, q[i].a2, q[i].res);
    printf("\nTRIPLES:\n"); for (int i = 0; i < qi; i++) {
        for (int j = 0; j < i; j++) {
            if (!strcmp(q[j].res, q[i].a1)) sprintf(t[i].a1, "[%d]", j);
            if (!strcmp(q[j].res, q[i].a2)) sprintf(t[i].a2, "[%d]", j);
        }
        strcpy(t[i].op, q[i].op); ti++;
        printf("%d: (%s, %s, %s)\n", i, t[i].op, t[i].a1, t[i].a2);
    }
    printf("\nINDIRECT TRIPLES:\n");
    for (int i = 0; i < ti; i++) {
        ptr[i] = i;
        printf("%d ? (%s, %s, %s)\n", i, t[ptr[i]].op, t[ptr[i]].a1, t[ptr[i]].a2);
    }
}

int main() {
    char exp[30]; printf("Enter expr: "); scanf("%s", exp);
    genIC(exp); show(); return 0;
}


quadruples



#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char stack[100][10]; int top = -1, t = 1;

int prec(char c) {
    return (c == '+' || c == '-') ? 1 : (c == '*' || c == '/') ? 2 : 0;
}

void reverse(char *s) {
    for (int i = 0, j = strlen(s)-1; i < j; i++, j--) {
        char t = s[i]; s[i] = s[j]; s[j] = t;
    }
}

void toPostfix(char *in, char *post) {
    char op[100]; int k = 0, p = -1;
    for (int i = 0; in[i]; i++) {
        char c = in[i];
        if (isalnum(c)) post[k++] = c;
        else if (c == '(') op[++p] = c;
        else if (c == ')') {
            while (p >= 0 && op[p] != '(') post[k++] = op[p--];
            if (p >= 0) p--; // pop '('
        }
        else {
            while (p >= 0 && prec(op[p]) >= prec(c)) post[k++] = op[p--];
            op[++p] = c;
        }
    }
    while (p >= 0) post[k++] = op[p--];
    post[k] = '\0';
}

void toPrefix(char *in, char *pre) {
    reverse(in);
    for (int i = 0; in[i]; i++) {
        if (in[i] == '(') in[i] = ')';
        else if (in[i] == ')') in[i] = '(';
    }
    toPostfix(in, pre);
    reverse(pre);
}

void genTAC(char *post) {
    printf("\nThree Address Code:\n");
    for (int i = 0; post[i]; i++) {
        if (isalnum(post[i])) {
            char tmp[2] = {post[i], '\0'};
            strcpy(stack[++top], tmp);
        } else {
            char a[10], b[10], temp[10];
            strcpy(b, stack[top--]); strcpy(a, stack[top--]);
            sprintf(temp, "t%d", t++);
            printf("%s = %s %c %s\n", temp, a, post[i], b);
            strcpy(stack[++top], temp);
        }
    }
}

int main() {
    char in[100], post[100], pre[100];
    printf("Infix: "); scanf("%s", in);
    toPostfix(in, post); toPrefix(in, pre);
    printf("Postfix: %s\nPrefix: %s\n", post, pre);
    genTAC(post);
    return 0;
}


prefix