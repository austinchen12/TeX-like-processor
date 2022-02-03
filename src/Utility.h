#ifndef __FRAME_H__
#define __FRAME_H__

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "String.h"
#include "Stack.h"

// A frame is 
typedef struct frame Frame;
// An iterator is
typedef struct iterator Iterator;

/**
 * Creates a default instance
 */
Frame *CreateFrame();

/**
 * Frees all space used by the Frame
 */
void FreeFrame(Frame *frame);

/**
 * Returns the String stored by the Frame
 */
String *FrameData(Frame *frame);

/**
 * Creates a default instance
 */
Iterator *CreateIterator();

/**
 * Returns the next character in the String of a Frame
 */
char NextChar(Iterator *iterator);

/**
 * Add a new frame to the Iterator
 */
bool AddFrame(Iterator *iterator, String *string);

/**
 * Checks if the Iterator has no Frames
 */
bool IsEmpty(Iterator *iterator);

/**
 * Moves back a character in the current Frame
 */
void Backtrack(Iterator *iterator);

/**
 * Frees all space used by the Iterator
 */
void FreeIterator(Iterator *iterator);

#endif