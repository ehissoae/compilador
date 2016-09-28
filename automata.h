#ifndef AUTOMATA_INCLUDED
#define AUTOMATA_INCLUDED

#define NUM_STATES 8
#define NUM_INPUT_TYPES 11


typedef enum{
  IT_MISC = 0,
  IT_LETTER = 1,
  IT_NUMBER = 2,
  IT_DOT = 3,
  IT_HASHTAG = 4,
  IT_GT_LT = 5,
  IT_EQUAL = 6,
  IT_DIFF = 7,
  IT_SPACE = 8,
  IT_NEWLINE = 9,

  IT_EOF = 99
}INPUT_TYPE;

typedef enum{
  S1 = 0, //inicial

  S2 = 1, //identificador
  S3 = 2, //numero inteiro
  S4 = 3, //numero de ponto flutuante
  S5 = 4, //comparador 1 caracter
  S6 = 5, //comparador 2 caracteres e simbolos especiais
  S7 = 6, //comparador de diferente "!"
  S8 = 7 //comentario
}STATE;

typedef enum{
  TT_ID = 1,
  TT_INT = 2,
  TT_FLOAT = 3,
  TT_SYMBOL = 4,

  TT_IGNORE = 0,
  TT_EOF = 99
}TOKEN_TYPE;

typedef struct{
  char* string;
  TOKEN_TYPE type;
}TOKEN;

extern const int TRANSITION_TABLE[NUM_STATES][NUM_INPUT_TYPES];

typedef struct Automata{
  int state;
}AUTOMATA;

INPUT_TYPE get_input_type(char input);
STATE get_next_state(AUTOMATA automata, char input);
TOKEN_TYPE get_token_type(STATE previous_state);
char* get_token_type_name(TOKEN_TYPE type);

#endif