#include "Utility.h"

struct frame {
    size_t i;
    String *Data;
};

Frame *CreateFrame(String *string) {
    Frame *result = malloc(sizeof(Frame));
    result->Data = string;

    return result;
}

String *FrameData(Frame *frame) {
    return frame->Data;
}

void FreeFrame(Frame *frame) {
    FreeString(frame->Data);
    free(frame);
}



struct iterator {
    Stack *Frames;
    Frame *CurrFrame;
};

Iterator *CreateIterator() {
    Iterator *result = malloc(sizeof(Iterator));
    if (result == NULL)
        return NULL;
    
    result->Frames = CreateStack(sizeof(Frame *));
    result->CurrFrame = NULL;

    return result;
}

char NextChar(Iterator *iterator) {
    Frame *frame = iterator->CurrFrame;
    if (frame == NULL)
        return 0;

    char c = GetChar(FrameData(frame), frame->i++);
    while (frame != NULL && frame->i == StringLength(FrameData(frame)))
    {
        FreeFrame(iterator->CurrFrame);
        iterator->CurrFrame = StackPop(iterator->Frames);
        frame = iterator->CurrFrame;
    }
    
    return c;
}

bool AddFrame(Iterator *iterator, String *string) {
    if (StringLength(string) == 0)
    {
        FreeString(string);
        return false;
    }
        
    Frame *frame = malloc(sizeof(Frame));
    if (frame == NULL)
        return false;
    
    frame->i = 0;
    frame->Data = string;
    
    if (iterator->CurrFrame != NULL)
        StackPush(iterator->Frames, iterator->CurrFrame);
    iterator->CurrFrame = frame;
    
    return true;
}

void Backtrack(Iterator *iterator) {
    if (iterator->CurrFrame->i != 0) {
        iterator->CurrFrame->i--;
    }
}

bool IsEmpty(Iterator *iterator) {
    return StackSize(iterator->Frames) == 0 && iterator->CurrFrame == NULL;
}

void FreeIterator(Iterator *iterator) {
    if (iterator->CurrFrame != NULL)
        FreeFrame(iterator->CurrFrame);
    
    FreeStack(iterator->Frames, &FreeFrame);
    
    free(iterator);
}
