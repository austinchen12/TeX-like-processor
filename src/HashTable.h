#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEFAULT_CAPACITY 8

typedef struct table Table;

/**
 * Creates a default isntance
 */
Table *CreateTable();

/**
 * Return the size
 */
size_t TableSize(Table *table);

/**
 * Return the capacity
 */
size_t TableCapacity(Table *table);

/**
 * Checks if a key exists in the table
 */
bool KeyExists(Table *table, char *key);

/**
 * Retrieve a value defined by a key in the table
 */
void *GetFromTable(Table *table, char *key);

/**
 * Add a key, value pair to the table
 */
bool AddToTable(Table *table, char *key, void *value);

/**
 * Removes an entry specified by key from the table
 */
void *RemoveFromTable(Table *table, char *key);

/**
 * Frees all space used by the HashTable
 */
void FreeTable(Table *table, void (*f)(void *));

#endif