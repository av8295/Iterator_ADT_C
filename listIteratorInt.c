/*
  listIteratorInt.c : list Iterator ADT implementation
  Written by Anchit Verma
  Date: January 2019
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "listIteratorInt.h"

typedef struct Node {
  int data;
  struct Node *forward;
  struct Node *backward;

} Node;


typedef struct IteratorIntRep {
  Node *head;
  Node *next;
  Node *previous;
  int flag; // To implement precondition for delete and set

} IteratorIntRep;


static Node *IteratorIntNode(int v) {
  Node *new;
  new = malloc(sizeof(Node));
  assert(new != NULL);
  new->data = v;
  new->forward = new->backward = NULL;

  return new;
}


IteratorInt IteratorIntNew(){
  IteratorInt it = malloc(sizeof(IteratorIntRep));
  it->head = NULL;
  it->next = NULL;
  it->previous = NULL;
  it->flag = 0;

  return it;
}


// Time complexity: O(1)
void reset(IteratorInt it){
  assert(it != NULL);
  it->flag = 0;

  it->next = it->head;
  it->previous = NULL;

  return;
}


// Time complexity: O(1)
int add(IteratorInt it, int v){
  assert(it != NULL);
  it->flag = 0;

  // Creating new node and input value
  Node *new = IteratorIntNode(v);
  assert(new != NULL);

  // If the list is empty
  if (it->head == NULL) {
    it->head = new;
    it->previous = new;
  }

  // If the iterator is at the start of the list
  else if (it->previous == NULL) {
    new->forward = it->next;  // New node's next
    it->next->backward = new; // Next node's previous
    it->head = new;           // Changing the head of the list
  }

  // If the iterator is at the end of the list
  else if (it->next == NULL) {
    new->backward = it->previous; // New node's previous
    it->previous->forward = new;  // Previous node's next
  }

  // If the iterator is somewhere in the middle
  else {
    new->forward = it->next;      // New node's next
    it->next->backward = new;     // Next node's previous
    new->backward = it->previous; // New node's previous
    it->previous->forward = new;  // Previous node's next
  }

  // Moving the iterator
  it->previous = new;

  return 1;
}


// Time complexity: O(1)
int hasNext(IteratorInt it){
  assert(it != NULL);
  it->flag = 0;

  // If next is Null, return 0, else 1.
  if (it->next == NULL) {
    return 0;
  }

  return 1;
}


// Time complexity: O(1)
int hasPrevious(IteratorInt it){
  assert(it != NULL);
  it->flag = 0;

  // If previous is Null, return 0, else 1.
  if (it->previous == NULL) {
    return 0;
  }

  return 1;
}


// Time complexity: O(1)
int *next(IteratorInt it){
  assert(it != NULL);

  // If at the end of the list
  if (it->next == NULL) {
    it->flag = 0;
    return NULL;
  }

  // Moving the iterator position
  it->previous = it->next;
  it->next = it->next->forward;

  it->flag = 1;

  return &it->previous->data;
}


// Time complexity: O(1)
int *previous(IteratorInt it){
  assert(it != NULL);

  // If at the start of the list
  if (it->previous == NULL) {
    it->flag = 0;
    return NULL;
  }

  // Moving the iterator position
  it->next = it->previous;
  it->previous = it->previous->backward;

  it->flag = 2;

  return &it->next->data;
}


// Time complexity: O(1)
int deleteElm(IteratorInt it){
  assert(it != NULL);

  // If Precondition is not met, flag is 0
  if (it->flag == 0) {
    return 0;
  }

  Node *temp;

  // If call was made to next or findNext, the iterator is moved to previous position
  // so that the element to be deleted becomes the next element, which becomes
  // the same condition as call made to previous or findPrevious.
  if (it->flag == 1) {
    previous(it);
  }

  temp = it->next;

  // If only 1 element in list
  if (it->next == it->head && it->next->forward == NULL) {
    it->head = NULL;
    it->next = NULL;
    it->previous = NULL;
    it->flag = 0;
    return 1;
  }

  // If at start of the list
  if (it->previous == NULL) {
    temp->forward->backward = NULL;
    it->head = temp->forward;
    it->next = temp->forward;
    it->flag = 0;
    return 1;
  }

  // If at end of the list
  if (it->next->forward == NULL) {
    it->previous->forward = NULL;
    it->next = NULL;
    it->flag = 0;
    return 1;
  }

  // If somewhere in the middle of the list
  temp->forward->backward = it->previous;
  it->previous->forward = temp->forward;
  it->next = temp->forward;

  free(temp);
  it->flag = 0;

  return 1;
}


// Time complexity: O(1)
int set(IteratorInt it, int v){
  assert(it != NULL);

  // If Precondition is not met, flag is 0
  if (it->flag == 0) {
    return 0;
  }

  // If a successful call made to next or findNext immediately before
  if (it->flag == 1) {
    it->previous->data = v;
  }

  // If a successful call made to previous or findPrevious immediately before
  if (it->flag == 2) {
    it->next->data = v;
  }

  // Reset flag
  it->flag = 0;

  return 1;
}


// Time complexity: O(n)
int *findNext(IteratorInt it, int v){
  assert(it != NULL);

  // If at end of the list
  if (it->next == NULL) {
    it->flag = 0;
    return NULL;
  }

  Node *current = it->next;

  while (current != NULL && current->data != v) {
    current = current->forward;
  }

  // If value not found
  if (current == NULL) {
    it->flag = 0;
    return NULL;
  }

  // Moving the iterator
  it->previous = current;
  it->next = it->previous->forward;

  it->flag = 1;

  return &current->data;
}


// Time complexity: O(n)
int *findPrevious(IteratorInt it, int v){
  assert(it != NULL);

  // If at start of the list
  if (it->previous == NULL) {
    it->flag = 0;
    return NULL;
  }

  Node *current = it->previous;

  while (current != NULL && current->data != v) {
    current = current->backward;
  }

  // If value not found
  if (current == NULL) {
    it->flag = 0;
    return NULL;
  }

  // Moving the iterator
  it->next = current;
  it->previous = it->next->backward;

  it->flag = 2;

  return &current->data;
}


// Time complexity: O(n)
void freeIt(IteratorInt it){
  assert(it != NULL);
  it->flag = 0;

  // Create temporary nodes
  Node *temp, *del;
  temp = it->head;

  // Iterating over the list and deleting nodes one by one
  while (temp != NULL) {
    del = temp;
    temp = temp->forward;
    free(del);
  }

  free(temp);
  free(it);

  return;
}


// Time complexity: O(n)
void disp(IteratorInt it) {
  assert(it != NULL);

  Node *temp;
  temp = it->head;

  while (temp != NULL) {
    if (it->next == temp) {
      printf(" ^ ");
    }
    
    printf("%d ", temp->data);
    temp = temp->forward;
  }

  if (it->next == NULL) {
    printf(" ^ ");
  }

  printf("\n");

  return;
}
