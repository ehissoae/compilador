#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "submachines.h"
#include "automata.h"
#include "lexico.h"
#include "list.h"

typedef struct APE{
  int cur_submachine;
  int cur_state;
  STACK* stack;
}APE;

void semantico_tbd(){
  // printf("TODO\n");
}

int ape_make_transition(APE* ape, TOKEN* token){
  int input_num = symbol_table(token->string);

  int next_state = APE_TRANSITION_TABLE[ape->cur_submachine][ape->cur_state][input_num][0];
  int submachine_call = APE_TRANSITION_TABLE[ape->cur_submachine][ape->cur_state][input_num][1];

  if (next_state == -1) {
  	printf("ERROR!! NEXT STATE = -1!\n");
    getchar();

  } else if (next_state == -2) {
    // retorno de submaquina
    semantico_tbd();

    int return_state;
    int return_submachine;
    stack_pop(ape->stack, &return_state);
    stack_pop(ape->stack, &return_submachine);

    printf("TODO - retorno! M%d -> M%d (S%d)\n", ape->cur_submachine, return_submachine, return_state);

    ape->cur_state = return_state;
    ape->cur_submachine = return_submachine;

  } else if (submachine_call == -1) {
    // transicao q consome token
    semantico_tbd();

    printf("TODO - consome token '%s'!\n", token->string);
    ape->cur_state = next_state;

    return 1;

  } else {
    // chamada de submaquina
    semantico_tbd();

    printf("TODO - submachine call M%d -> M%d! Push(%d, %d)\n", ape->cur_submachine, submachine_call, ape->cur_submachine, next_state);
    stack_push(ape->stack, &ape->cur_submachine);
    stack_push(ape->stack, &next_state);

    ape->cur_state = 0;
    ape->cur_submachine = submachine_call;
  }

  return 0;
}

int main() {
  FILE *file = fopen("ENTRADA.txt", "r");
  if(!file) {
    printf("Arquivo não existe!!!\n");
    return 0;
  }
  char str_buffer[100];

  AUTOMATA lex_automata = {S1};

  TOKEN next_token;
  next_token.string = str_buffer;
  next_token.type = 0;


  STACK* stack = new_list();
  APE ape;
  ape.cur_submachine = INITIAL_SUBMACHINE;
  ape.cur_state = 0;
  ape.stack = stack;


  int next_token_flag = 1;
  int i = 0;
  while (next_token.type != TT_EOF) {
    if (next_token_flag){
      get_next_token(file, &lex_automata, &next_token);
      printf("\nRecebendo token: '%s' (%s)\n", next_token.string, get_token_type_name(next_token.type));
    }

  	if (next_token.type == TT_EOF) {
  		break;
  	}

  	next_token_flag = ape_make_transition(&ape, &next_token);
    // getchar();
    // next_token_flag = 1;
    i++;
  }
}


// int main() {
//   FILE *file = fopen("ENTRADA.txt", "r");
//   if(!file) {
//     printf("Arquivo não existe!!!\n");
//     return 0;
//   }
//   char str_buffer[100];

//   AUTOMATA automata = {S1};

//   TOKEN next_token;
//   next_token.string = str_buffer;
//   do {
//     get_next_token(file, &automata, &next_token);
//     printf("%s (%s)\n", next_token.string, get_token_type_name(next_token.type));

//     // getchar();

//   } while (next_token.type != TT_EOF);
// }