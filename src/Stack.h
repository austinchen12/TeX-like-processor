#ifndef __STACK_H__
#define __STACK_H__

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEFAULT_CAPACITY 8

typedef struct stack Stack;

/**
 * Creates an instance
 */
Stack *CreateStack(size_t valueSize);

/**
 * Return the size
 */
size_t StackSize(Stack *stack);

/**
 * Return the capacity
 */
size_t StackCapacity(Stack *stack);

/**
 * Return the data
 */
void *StackData(Stack *stack);

/**
 * Appends a value
 */
void StackPush(Stack *stack, void *value);

/**
 * Pops off the top value
 */
void *StackPop(Stack *stack);

/**
 * Returns the top value without removing
 */
void *StackPeek(Stack *stack);

/**
 * Frees all space used by the Stack
 */
void FreeStack(Stack *stack, void (*f)(void *));

#endif