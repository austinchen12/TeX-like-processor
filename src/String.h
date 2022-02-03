#ifndef __STRING_H__
#define __STRING_H__

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEFAULT_STR_LEN 32

typedef struct string String;

/**
 * Creates a default instance
 */
String *CreateString();

/**
 * Append a char to the end of the string, handles increasing size of the array
 */
void AppendToString(String *string, char c);

/**
 * Frees all space used by the String
 */
void FreeString(void *string);

/**
 * Prints out the string
 */
void PrintString(String *string);

/**
 * Return the length
 */
size_t StringLength(String *string);

/**
 * Return the capacity
 */
size_t StringCapacity(String *string);

/**
 * Return the string
 */
char *StringData(String *string);

/**
 * Return the char at the given index
 */
char GetChar(String *string, size_t i);

#endif