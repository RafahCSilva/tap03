#include <stdio.h>
#include <stdlib.h>

enum TokenType {PLUS, MINUS, TIMES, SLASH, NUMBER, LPAR, RPAR, EOL, UNKNOWN};
enum NodeType {EXPRESSION, TERM, FACTOR};

typedef double TYPE;
typedef struct s_node * p_node;

struct s_node {
  enum NodeType ntype;
  TYPE nvalue;
  p_node child[2];
};

struct s_token {
  enum TokenType ttype;
  TYPE tvalue;
  int position;
  int length;
} token;

void print_tree(p_node v, int level) {
  static char N[3] = {'E', 'T', 'F'};
  
  if (v == NULL)
    return;

  char indent[1024];
  for (int i = 0; i < 4 * level; i ++)
    indent[i] = ' ';
  indent[4 * level] = '\0';

  printf("%s(%c) %.3lf\n", indent, N[v->ntype], v->nvalue);

  print_tree(v->child[0], level + 1);
  print_tree(v->child[1], level + 1);
}

void print_token_types() {
  fprintf(stderr, "========================\n");
  fprintf(stderr, "   PLUS: %d\n", (int) PLUS);
  fprintf(stderr, "  MINUS: %d\n", (int) MINUS);  
  fprintf(stderr, "  TIMES: %d\n", (int) TIMES);
  fprintf(stderr, "  SLASH: %d\n", (int) SLASH);
  fprintf(stderr, " NUMBER: %d\n", (int) NUMBER);
  fprintf(stderr, "   LPAR: %d\n", (int) LPAR);
  fprintf(stderr, "   RPAR: %d\n", (int) RPAR);
  fprintf(stderr, "    EOL: %d\n", (int) EOL);
  fprintf(stderr, "UNKNOWN: %d\n", (int) UNKNOWN);
  fprintf(stderr, "========================\n");
}

void print_token() {
  fprintf(stderr, "========================\n");
  fprintf(stderr, "    Type: %d\n", (int) token.ttype);
  fprintf(stderr, "   Value: %.3lf\n", token.tvalue);
  fprintf(stderr, "Position: %d\n", token.position);
  fprintf(stderr, "  Length: %d\n", token.length);
  fprintf(stderr, "========================\n");
}

p_node new_node(enum NodeType ntype, TYPE nvalue, p_node first, p_node second) {
  p_node node = (p_node) malloc(sizeof(struct s_node));
  if (node == NULL) {
    fprintf(stderr, "malloc failed (probably because the system is out of memory).\n");
    exit(1);
  }

  node->ntype = ntype;
  node->nvalue = nvalue;
  node->child[0] = first;
  node->child[1] = second;

  return node;
}

void tree_free(p_node v) {
  if (v == NULL)
    return;

  tree_free(v->child[0]);
  tree_free(v->child[1]);

  free(v);
}

// Buffer
char *lineptr = NULL;
size_t n = 0;

void parsing_error(char *msg) {
  fprintf(stderr, "Parsing error at position %d: %s.\n", token.position, msg);
}

enum TokenType next_token() {
  int pos = token.position + token.length;

  char c;

  while (pos < n && ((c = lineptr[pos]) == ' ' || c == '\t'))
    pos ++;

  token.position = pos;
  token.length = 1;

  switch (c) {
  case '(':
    return token.ttype = LPAR;

  case ')':
    return token.ttype = RPAR;

  case '\0':
    // this happens when pos == n, and
    // may happen when reading last line of a file w/o newline at the end
  case '\n':
    return token.ttype = EOL;

  case '+':
    return token.ttype = PLUS;

  case '-':
    return token.ttype = MINUS;

  case '*':
    return token.ttype = TIMES;

  case '/':
    return token.ttype = SLASH;

  default:
    if (c < '0' || c > '9')
      return token.ttype = UNKNOWN;
  }
  
  token.tvalue = c - '0';
  pos ++;
  while (pos < n && (c = lineptr[pos]) >= '0' && c <= '9') {
    token.tvalue *= 10;
    token.tvalue += (c - '0');
    token.length ++;
    pos ++;
  }

  return token.ttype = NUMBER;
}

p_node L();
p_node E();
p_node I();
p_node T();
p_node J();
p_node F();


p_node L() {
  p_node first = E();

  if (token.ttype != EOL) {
    parsing_error("in L()");
    tree_free(first);
    return NULL;
  }

  return first;
}

p_node E() {
  p_node first = T();
  
  if (first == NULL) {
    parsing_error("in E()");
    return NULL;
  }

  p_node second = I();

  if (second != NULL)
    return new_node(EXPRESSION, first->nvalue + second->nvalue, first, second);

  return first;
}

p_node I() {
  struct s_token t = token;

  if (token.ttype != PLUS && token.ttype != MINUS)
    return NULL;
  
  next_token();

  p_node first = T();

  if (first == NULL) {
    parsing_error("in I()");
    return NULL;
  }

  if (t.ttype == MINUS)
    first->nvalue *= -1;
  
  p_node second = I();
  
  if (second != NULL)
    return new_node(EXPRESSION, first->nvalue + second->nvalue, first, second);

  return first;
}

p_node T() {
  p_node first = F();
  
  if (first == NULL) {
    parsing_error("in T()");
    return NULL;
  }

  p_node second = J();

  if (second != NULL)
    return new_node(TERM, first->nvalue * second->nvalue, first, second);

  return first;
}

p_node J() {
  struct s_token t = token;

  if (token.ttype != TIMES && token.ttype != SLASH)
    return NULL;
  
  next_token();

  p_node first = F();

  if (first == NULL) {
    parsing_error("in J()");
    return NULL;
  }

  if (t.ttype == SLASH) {
    if (first->nvalue == 0)
      fprintf(stderr, "division by 0, please ignore result\n");
    else
      first->nvalue = 1 / first->nvalue;
  }
  
  p_node second = J();
  
  if (second != NULL)
    return new_node(TERM, first->nvalue * second->nvalue, first, second);

  return first;
}
    
p_node F() {
  struct s_token t = token;

  if (t.ttype == MINUS) {
    next_token();
    p_node first = F();
    first->nvalue *= -1;
    return first;
  }

  if (t.ttype == NUMBER) {
    next_token();
    return new_node(FACTOR, t.tvalue, NULL, NULL);
  }

  if (t.ttype != LPAR) {
    parsing_error("in F()");
    return NULL;
  }

  next_token();
  p_node first = E();

  if (token.ttype != RPAR) {
    parsing_error("in F()");
    tree_free(first);
    return NULL;
  }

  next_token();
  return first;
}

void prompt() {
  printf("\n>>> ");
}

int main() {
  print_token_types();

  prompt();
  while (getline(&lineptr, &n, stdin) > 0) {
    token.position = -token.length;
    next_token();
    // print_token();

    p_node v = L();

    if (v != NULL) {
      //printf("%.3lf\n", v->nvalue);
       print_tree(v, 0);
      tree_free(v);      
    }

    prompt();
  }
  printf("\n");

  free(lineptr);

  return 0;
}
