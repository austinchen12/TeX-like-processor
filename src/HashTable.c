#include "HashTable.h"

typedef struct node
{
    char *Key;
    void *Value;
} Node;

struct table
{
    size_t Size;
    size_t Capacity;
    Node *Data;
};

Table *CreateTable()
{
    Table *table = malloc(sizeof(Table));
    if (table != NULL) {
        table->Size = 0;
        table->Data = calloc(DEFAULT_CAPACITY, sizeof(Node));
        table->Capacity = (table->Data != NULL ? DEFAULT_CAPACITY : 0);
    }

    return table;
}

size_t TableSize(Table *table) {
    return table == NULL ? 0 : table->Size;
}

size_t TableCapacity(Table *table) {
    return table == NULL ? 0 : table->Capacity;
}

size_t hash53(char *key)
{
    char *s = key;
    size_t sum = 0;
    size_t factor = 53;
    while (s != NULL && *s != '\0')
    {
        sum += *s * factor;
        s++;
        factor *= 53;
    }

    return sum;
}

size_t computeIndex(Table *table, char *key) {
    size_t i = hash53(key) % table->Capacity;
    
    while (table->Data[i].Key != NULL && strcmp(table->Data[i].Key, key) != 0)
    {
        i = (i + 1) % table->Capacity;
    }
    
    return i;
}

bool KeyExists(Table *table, char *key) {
    size_t i = computeIndex(table, key);

    return table->Data[i].Key != NULL && strcmp(table->Data[i].Key, key) == 0;
}

void *GetFromTable(Table *table, char *key) {
    size_t i = computeIndex(table, key);
    
    if (table->Data[i].Key != NULL && strcmp(table->Data[i].Key, key) == 0)
    {
        return table->Data[i].Value;
    }

    return NULL;
}

void tableEmbiggen(Table *table)
{
    Node *bigger = calloc(table->Capacity * 2, sizeof(Node));
    for (size_t i = 0; i < table->Capacity; i++)
    {
        if (table->Data[i].Key != NULL)
        {
            size_t new_i = hash53(table->Data[i].Key) % (table->Capacity * 2);
            while (bigger[new_i].Key != NULL && strcmp(bigger[new_i].Key, table->Data[i].Key) != 0)
            {
                new_i = (new_i + 1) % (table->Capacity * 2);
            }
            
            bigger[new_i] = table->Data[i];
        }
    }
    free(table->Data);
    table->Data = bigger;
    table->Capacity *= 2;
}

char *duplicate(char *key)
{
    char *s = malloc(strlen(key) + 1);
    if (s != NULL)
    {
        strcpy(s, key);
    }

    return s;
}

bool AddToTable(Table *table, char *key, void *value) {
    if (table->Size + 1 == table->Capacity)
        tableEmbiggen(table);
        
    size_t i = computeIndex(table, key);

    char *copy = duplicate(key);
    if (copy == NULL)
        return false;
    
    table->Data[i].Key = copy;
    table->Data[i].Value = value;
    table->Size++;
    return true;
}

void *RemoveFromTable(Table *table, char *key) {
    size_t i = computeIndex(table, key);

    if (table->Data[i].Key == NULL)
        return NULL;
    
    free(table->Data[i].Key);
    table->Data[i].Key = NULL;
    table->Size--;

    return table->Data[i].Value;
}

void FreeTable(Table *table, void (*f)(void *)) {
    for (int i = 0; i < table->Capacity; i++)
    {
        if (table->Data[i].Key != NULL)
        {
            free(table->Data[i].Key);
            if (f != NULL)
            {
                f(table->Data[i].Value);
            }
        }
    }
    free(table->Data);
    free(table);
}
