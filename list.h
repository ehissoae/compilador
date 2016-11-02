#ifndef LIST_INCLUDED
#define LIST_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct LIST_NODE LIST_NODE;

struct LIST_NODE {
  void* data;
  LIST_NODE* next;
};

typedef struct LIST LIST;
typedef struct LIST STACK;

struct LIST{
  int length;
  // int node_size;
  LIST_NODE* head;
};

LIST* new_list(){
	LIST* list = malloc(sizeof(LIST));
	list->length = 0;
	// list->node_size = node_size;
	list->head = NULL;

	return list;
}

void list_prepend(LIST* list, void* elem){
	LIST_NODE *node = malloc(sizeof(LIST_NODE));
  // node->data = malloc(list->node_size);
  node->data = malloc(sizeof(elem));
  // memcpy(node->data, elem, list->node_size);
  memcpy(node->data, elem, sizeof(elem));

  node->next = list->head;
  list->head = node;

  list->length++;
}

void stack_push(LIST* list, void* elem){
	list_prepend(list, elem);
}

void list_get_first(LIST* list, void* elem){
	if (list->head){
		// return list->head->data;
  	// memcpy(node->data, elem, list->node_size);
  	memcpy(elem, list->head->data, sizeof(elem));
	} else {
		elem = NULL;
	}
}

void stack_peek(LIST* list, void* elem){
	list_get_first(list, elem);
}


void list_remove_first(LIST* list){
	if (list->head){
		LIST_NODE* old_head = list->head;

    list->head = old_head->next;
    list->length--;

		free(old_head->data);
    free(old_head);
	} else {
		printf("ERROR! STACK IS EMPTY!!\n");
	}
}


void list_pop_first(LIST* list, void* elem){
	list_get_first(list, elem);
	// printf("first: %d\n", *(int*)elem);
	list_remove_first(list);
}

void stack_pop(LIST* list, void* elem){
	list_pop_first(list, elem);
}


// yay = []; jQuery("h4:contains('Minimized DFA ')").next().find('pre').each(function(){yay.push('Automata - '+jQuery(this).closest(".automatas").prev().text());yay.push(jQuery(this).text())}) ; yay.join('\n')


// int main(int argc, char const *argv[])
// {
// 	int table[2][2] =
// 	{
// 	  {0, 1},
// 		{2, },
// 	};

// 	printf("%d\n", table[0][0]);
// 	printf("%d\n", table[0][1]);
// 	printf("%d\n", table[1][0]);
// 	printf("%d\n", table[1][1]);
// 	printf("yay\n");
// 	LIST* list = new_list(sizeof(int));

// 	int yay = 1;
// 	list_prepend(list, &yay);
// 	yay = 9;
// 	list_prepend(list, &yay);

// 	int yay2;
// 	list_pop_first(list, &yay2);
// 	printf("value1: %d\n", yay2);
// 	printf("length1: %d\n", list->length);

// 	list_pop_first(list, &yay2);

// 	printf("value2: %d\n", yay2);
// 	printf("length2: %d\n", list->length);

// 	return 0;
// }
#endif