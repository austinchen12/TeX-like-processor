#include "String.h"

struct string {
    size_t Length;
    size_t Capacity;
    char *Data;
};

String *CreateString() {
    String *string = malloc(sizeof(String));
    if (string != NULL) {
        string->Length = 0;
        string->Capacity = DEFAULT_STR_LEN;
        string->Data = malloc(sizeof(char) * DEFAULT_STR_LEN);

        string->Data[0] = '\0';
    }

    return string;
}

void AppendToString(String *string, char c) {
    if (string->Length + 1 == string->Capacity)
    {
        string->Capacity *= 2;
        string->Data = realloc(string->Data, string->Capacity);
    }

    string->Data[string->Length++] = c;
    string->Data[string->Length] = '\0';
}

void FreeString(void *string) {
    free(((String *)string)->Data);
    free(string);
}

void PrintString(String *string) {
    printf("%s", string->Data);
}

size_t StringLength(String *string) {
    return string->Length;
}

size_t StringCapacity(String *string) {
    return string->Capacity;
}

char *StringData(String *string) {
    return string->Data;
}

char GetChar(String *string, size_t i) {
    return string->Data[i];
}
