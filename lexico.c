#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "lexico.h"
#include "automata.h"

int get_next_char_flag = 1;
char get_next_token_c = 0;

void get_next_token(FILE* file, AUTOMATA* automata, TOKEN* token){
  char buffer[100];
  int buffer_size = 0;
  do {
    if(get_next_char_flag){
      get_next_token_c = fgetc(file);
      // printf("got char '%c'\n", c);
    }
    // printf("%d = %c - type: %d\n", c, c, get_input_type(c));
    get_next_char_flag = 1;

    // printf("INPUT_TYPE: %d\n", get_input_type(c));

    STATE current_state = automata->state;
    STATE next_state = get_next_state(*automata, get_next_token_c);
    // printf("- LEX %d -> %d ('%c' = %d)\n", current_state, next_state, get_next_token_c, get_next_token_c);
    // getchar();
    // printf("- LEX next state %d\n", next_state);
    // printf("- LEX next state %d\n", next_state);
    // printf("MUDANCA DE ESTADO: S%d -> S%d\n", current_state+1, next_state+1);
    if(next_state != S1){
      buffer[buffer_size] = get_next_token_c;
      buffer_size++;
    } else {
      if(buffer_size > 0){
        TOKEN_TYPE token_type = get_token_type(current_state);

        if(token_type != TT_IGNORE){
          get_next_char_flag = 0;

          // char* token_string = (char*)malloc(sizeof(char) * (buffer_size + 1));
          for (int i = 0; i < buffer_size; ++i) {
            token->string[i] = buffer[i];
          }
          token->string[buffer_size] = '\0';
          // printf("%s (%s)\n", token_string, get_token_type_name(token_type));
          // TOKEN new_token = {token_string, token_type};
          // token = {token_string, token_type};
          token->type = token_type;
          automata->state = next_state;
          // return new_token;
          return;
        }

        buffer_size = 0;
      }
    }

    automata->state = next_state;
  } while (get_next_token_c != EOF);

  // TOKEN eof_token = {"", TT_EOF};
  // return eof_token;

  token->string[0] = '\0';
  token->type = TT_EOF;
}
