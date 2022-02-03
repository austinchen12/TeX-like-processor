#include "Stack.h"

struct stack {
    size_t ValueSize;
    size_t Size;
    size_t Capacity;
    void **Data;
};

Stack *CreateStack(size_t valueSize) {
    Stack *result = malloc(sizeof(Stack));
    if (result == NULL)
        return NULL;

    result->ValueSize = valueSize;
    result->Size = 0;
    result->Capacity = DEFAULT_CAPACITY;
    result->Data = calloc(DEFAULT_CAPACITY, valueSize);
    if (result->Data == NULL)
        return NULL;

    return result;
}

size_t StackSize(Stack *stack) {
    return stack->Size;
}

size_t StackCapacity(Stack *stack) {
    return stack->Capacity;
}

void *StackData(Stack *stack) {
    return stack->Data;
}

void StackPush(Stack *stack, void *value) {
    if (stack->Size == stack->Capacity)
    {
        stack->Data = realloc(stack->Data, stack->ValueSize * stack->Capacity * 2);
        stack->Capacity *= 2;
    }

    stack->Data[stack->Size++] = value;
}

void *StackPop(Stack *stack) {
    if (stack->Size == 0)
        return NULL;

    void *temp = stack->Data[stack->Size - 1];
    stack->Data[stack->Size - 1] = NULL;
    stack->Size -= 1;
    
    return temp;
}

void *StackPeek(Stack *stack) {
    if (stack->Size == 0)
        return NULL;
        
    return stack->Data[stack->Size - 1];
}

void FreeStack(Stack *stack, void (*f)(void *)) {
    for (int i = 0; i < stack->Size; i++)
    {
        if (stack->Data[i] != NULL)
            f(stack->Data[i]);
    }
    free(stack->Data);
    free(stack);
}
