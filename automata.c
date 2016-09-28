#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "automata.h"

const int TRANSITION_TABLE[NUM_STATES][NUM_INPUT_TYPES] =
{       
          /* 0   1   2   3   4   5   6   7   8   9*/
  /* S1 */ {S6, S2, S3, S6, S8, S5, S5, S7, S1, S1},
  /* S2 */ {S1, S2, S1, S1, S1, S1, S1, S1, S1, S1},
  /* S3 */ {S1, S1, S3, S4, S1, S1, S1, S1, S1, S1},
  /* S4 */ {S1, S1, S4, S1, S1, S1, S1, S1, S1, S1},
  /* S5 */ {S1, S1, S1, S1, S1, S1, S6, S1, S1, S1},
  /* S6 */ {S1, S1, S1, S1, S1, S1, S1, S1, S1, S1},
  /* S7 */ {S1, S1, S1, S1, S1, S1, S6, S1, S1, S1},
  /* S8 */ {S8, S8, S8, S8, S8, S8, S8, S8, S8, S1},
};

INPUT_TYPE get_input_type(char input){
  if(isalpha((int)input))
    return IT_LETTER;
  if(isdigit((int)input))
    return IT_NUMBER;
  if(input=='.')
    return IT_DOT;
  if(input=='#')
    return IT_HASHTAG;
  if(input=='>' || input=='<')
    return IT_GT_LT;
  if(input=='=')
    return IT_EQUAL;
  if(input=='!')
    return IT_DIFF;
  if(input==' ' || input=='\t')
    return IT_SPACE;
  if(input=='\n')
    return IT_NEWLINE;

  return IT_MISC;
}

STATE get_next_state(AUTOMATA automata, char input){
  INPUT_TYPE input_type = get_input_type(input);

  STATE next_state = TRANSITION_TABLE[automata.state][input_type];

  return next_state;
}

TOKEN_TYPE get_token_type(STATE previous_state){
  if(previous_state == S2)
    return TT_ID;
  if(previous_state == S3)
    return TT_INT;
  if(previous_state == S4)
    return TT_FLOAT;
  if(previous_state == S5 || previous_state == S6)
    return TT_SYMBOL;
  if(previous_state == S8)
    return TT_IGNORE;

  printf("** Estado %d não deveria gerar token **", previous_state);
  return TT_IGNORE;
}

char* get_token_type_name(TOKEN_TYPE type){
  if(type == TT_ID)
    return "Identificador";
  if(type == TT_INT)
    return "Inteiro";
  if(type == TT_FLOAT)
    return "Ponto Flutuante";
  if(type == TT_SYMBOL)
    return "Simbolo";
  if(type == TT_EOF)
    return "EOF";

  return "";
}