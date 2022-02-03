#ifndef __FILEPROCESSOR_H__
#define __FILEPROCESSOR_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <ctype.h>

#include "String.h"
#include "HashTable.h"

/**
 * Parses the input file
 */
String *ProcessInput(char *file_name);

/**
 * Removes comments from an input
 */
String *ReadFileAndRemoveComments(FILE* input);

/**
 * Skips the comment in a string
 */
void SkipComment(FILE *input);

#endif