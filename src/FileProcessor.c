#include "FileProcessor.h"
#include "String.h"
#include "HashTable.h"

String *ProcessInput(char *file_name)
{
    FILE *input = strcmp(file_name, "stdin") == 0 ? stdin : fopen(file_name, "r");
    if (input == NULL)
    {
        printf("can't open %s\n", file_name);
        exit(1);
    }
    
    String *string = ReadFileAndRemoveComments(input);

    return string;
}

String* ReadFileAndRemoveComments(FILE* input) {
    String *string = CreateString();

    char prev, c;
    while ((c = fgetc(input)) != EOF)
    {
        if (c == '%' && prev != '\\')
        {
            SkipComment(input);
        }
        else
        {
            AppendToString(string, c);
        }
        prev = c;
    }

    AppendToString(string, c);

    fclose(input);
    return string;
}

void SkipComment(FILE *input) {
    bool readNewLine = false;
    char c;
    while ((c = fgetc(input)) != EOF)
    {
        if (readNewLine && !isblank(c))
        {
            ungetc(c, input);
            return;
        }
        else if (!readNewLine && c == '\n')
        {
            readNewLine = true;
        }
    }
}
