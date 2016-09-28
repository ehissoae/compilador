#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "lexico.h"
#include "automata.h"

int get_next_char_flag = 1;
TOKEN get_next_token(FILE* file, AUTOMATA *automata){
  char c;
  char buffer[100];
  int buffer_index = 0;
  do {
    if(get_next_char_flag){
      c = fgetc(file);
    }
    // printf("%d = %c - type: %d\n", c, c, get_input_type(c));
    get_next_char_flag = 1;

    // printf("INPUT_TYPE: %d\n", get_input_type(c));

    STATE current_state = automata->state;
    STATE next_state = get_next_state(*automata, c);
    // printf("MUDANCA DE ESTADO: S%d -> S%d\n", current_state+1, next_state+1);
    if(next_state != S1){
      buffer[buffer_index] = c;
      buffer_index++;
    } else {
      if(buffer_index > 0){
        TOKEN_TYPE token_type = get_token_type(current_state);

        if(token_type != TT_IGNORE){
          get_next_char_flag = 0;

          char* token_string = (char*)malloc(sizeof(char) * (buffer_index + 1));
          for (int i = 0; i < buffer_index; ++i) {
            token_string[i] = buffer[i];
          }
          token_string[buffer_index] = '\0';
          // printf("%s (%s)\n", token_string, get_token_type_name(token_type));
          TOKEN new_token = {token_string, token_type};
          automata->state = next_state;
          return new_token;
        }

        buffer_index = 0;
      }
    }

    automata->state = next_state;
  } while (c != EOF);

  TOKEN eof_token = {"", TT_EOF};
  return eof_token;
}

int main() {
  FILE *file = fopen("ENTRADA.txt", "r");
  if(!file) {
    printf("Arquivo não existe!!!\n");
    return 0;
  }

  AUTOMATA automata = {S1};

  TOKEN next_token;
  do {
    next_token = get_next_token(file, &automata);
    printf("%s (%s)\n", next_token.string, get_token_type_name(next_token.type));

  } while (next_token.type != TT_EOF);
}