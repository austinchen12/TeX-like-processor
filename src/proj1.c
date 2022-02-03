#include "proj1.h"
#include "String.h"
#include "HashTable.h"
#include "Stack.h"
#include "FileProcessor.h"
#include "Utility.h"

typedef enum definedMacro
{
    DEF,
    UNDEF,
    IF,
    IFDEF,
    INCLUDE,
    EXPANDAFTER
} DefinedMacro;

typedef enum state
{
    PLAINTEXT, 
    ESCAPE,
    MACRO
} State;

const size_t ESCAPE_CHARS_COUNT = 5;
const char ESCAPE_CHARS[5] = {'\\', '#', '%', '{', '}'};

Table *CreatePredefinedMacroTable();
void ExitProgram(char *reason);
String *Tick(Iterator *iterator);
void HandlePlaintext(char c, State *state, String *result);
bool IsEscapeChar(char c);
void HandleEscape(char c, State *state, String *result);
void HandleMacro(char c, Iterator *iterator, State *state, String *result);

String *ReadParameter(Iterator *iterator, bool nonEmptyAndAlnum);
String **ReadParameters(size_t n, Iterator *iterator);

void HandleDef(Iterator *iterator);
void HandleUndef(Iterator *iterator);
void HandleIf(Iterator *iterator);
void HandleIfdef(Iterator *iterator);
void HandleInclude(Iterator *iterator);
void HandleExpandafter(Iterator *iterator);

Table *macros, *predefinedMacros;
String *output;
int main(int argc, char *argv[]) {
    macros = CreateTable();
    predefinedMacros = CreatePredefinedMacroTable();
    
    String *result;
    Iterator *iterator = CreateIterator();
    if (argc == 1)
    {
        result = ProcessInput("stdin");
        AddFrame(iterator, result);
    }
    else
    {
        for (size_t i = argc - 1; i > 0; i--)
        {
            result = ProcessInput(argv[i]);
            AddFrame(iterator, result);
        }
    }
    
    output = Tick(iterator);
    printf("%s", StringData(output));

    FreeString(output);
    FreeTable(macros, &FreeString);
    FreeTable(predefinedMacros, NULL);
    FreeIterator(iterator);
}

Table *CreatePredefinedMacroTable() {
    Table *result = CreateTable();
    AddToTable(result, "def", &HandleDef);
    AddToTable(result, "undef", &HandleUndef);
    AddToTable(result, "if", &HandleIf);
    AddToTable(result, "ifdef", &HandleIfdef);
    AddToTable(result, "include", &HandleInclude);
    AddToTable(result, "expandafter", &HandleExpandafter);
    
    return result;
}

// State manager
String *Tick(Iterator *iterator) {
    State state = PLAINTEXT;
    
    String *result = CreateString();
    while (!IsEmpty(iterator))
    {
        char c = NextChar(iterator);
        if (c == 0)
            exit(1);
        switch (state)
        {
            case PLAINTEXT:
                HandlePlaintext(c, &state, result);
                break;

            case ESCAPE:
                HandleEscape(c, &state, result);
                if (state == MACRO)
                    goto INITIAL_MACRO;
                break;

            case MACRO:
            INITIAL_MACRO:
                HandleMacro(c, iterator, &state, result);
                break;
            
        }
    }

    return result;
}

void HandlePlaintext(char c, State *state, String *result) {
    if (c == '\\')
    {
        *state = ESCAPE;
    }
    else
    {
        if (c != EOF)
        {
            AppendToString(result, c);
        }
    }
}

bool IsEscapeChar(char c) {
    for (size_t i = 0; i < ESCAPE_CHARS_COUNT; i++)
    {
        if (c == ESCAPE_CHARS[i])
        {
            return true;
        }
    }

    return false;
}

void HandleEscape(char c, State *state, String *result) {
    if (IsEscapeChar(c))
    {
        AppendToString(result, c);
        *state = PLAINTEXT;
    }
    else if (isalnum(c))
    {
        *state = MACRO;
    }
    else
    {
        if (c == EOF)
        {
            AppendToString(result, '\\');
        }
        else
        {
            AppendToString(result, '\\');
            AppendToString(result, c);
        }
        
        *state = PLAINTEXT;
    }
}

void HandleDef(Iterator *iterator) {
    String *name = ReadParameter(iterator, true);
    if (KeyExists(macros, StringData(name)))
    {
        fprintf(stderr, "cannot redefine %s\n", StringData(name));
        exit(1);
    }
    String *value = ReadParameter(iterator, false);
    
    AddToTable(macros, StringData(name), value);
    FreeString(name);
}

void HandleUndef(Iterator *iterator) {
    String *name = ReadParameter(iterator, true);

    if (!KeyExists(macros, StringData(name)))
    {
        fprintf(stderr, "Cannot undef %s\n", StringData(name));
        exit(1);
    }
    String *macro = RemoveFromTable(macros, StringData(name));
    
    FreeString(macro);
    FreeString(name);
}

void HandleIf(Iterator *iterator) {
    String **parameters = ReadParameters(3, iterator);
    if (strlen(StringData(parameters[0])) != 0)
    {
        AddFrame(iterator, parameters[1]);
        FreeString(parameters[2]);
    }
    else
    {
        AddFrame(iterator, parameters[2]);
        FreeString(parameters[1]);
    }

    FreeString(parameters[0]);
    free(parameters);
}

void HandleIfdef(Iterator *iterator) {
    String *cond = ReadParameter(iterator, true);
    String **parameters = ReadParameters(2, iterator);

    if (KeyExists(macros, StringData(cond)))
    {
        AddFrame(iterator, parameters[0]);
        FreeString(parameters[1]);
    }
    else
    {
        AddFrame(iterator, parameters[1]);
        FreeString(parameters[0]);
    }

    FreeString(cond);
    free(parameters);
}

void HandleInclude(Iterator *iterator) {
    String *parameter = ReadParameter(iterator, false);
    String *content = ProcessInput(StringData(parameter));

    AddFrame(iterator, content);
    
    FreeString(parameter);
}

void HandleExpandafter(Iterator *iterator) {
    String **parameters = ReadParameters(2, iterator);
    Iterator *newIterator = CreateIterator();

    AddFrame(newIterator, parameters[1]);
    String *result = Tick(newIterator);
    
    AddFrame(iterator, result);
    AddFrame(iterator, parameters[0]);

    FreeIterator(newIterator);
    free(parameters);
}

void HandleCustom(Iterator *iterator, String *macro) {
    String *parameter = ReadParameter(iterator, false);
    String *result = CreateString();
    char c;
    bool inEscape = false;

    for (int i = 0; i < StringLength(macro); i++)
    {
        c = GetChar(macro, i);

        if (inEscape)
        {
            AppendToString(result, '\\');
            AppendToString(result, c);
            inEscape = false;
        }
        else
        {
            if (c == '#')
            {
                for (int j = 0; j < StringLength(parameter); j++)
                {
                    AppendToString(result, GetChar(parameter, j));
                }
            }
            else if (c == '\\')
            {
                inEscape = true;
            }
            else
            {
                AppendToString(result, c);
            }
        }
    }
    
    AddFrame(iterator, result);

    FreeString(parameter);
}

void HandleMacro(char c, Iterator *iterator, State *state, String *result) {
    String *macroString = CreateString();

    while (c != '{')
    {
        if (!isalnum(c))
        {
            fprintf(stderr, "expected {\n");
            exit(1);
        }
            
        AppendToString(macroString, c);
        c = NextChar(iterator);
    }
    Backtrack(iterator);

    char *macroName = StringData(macroString);
    
    if (KeyExists(predefinedMacros, macroName))
    {
        void (*f)(Iterator *) = GetFromTable(predefinedMacros, macroName);
        f(iterator);
    }
    else if (KeyExists(macros, macroName))
    {
        String *macro = GetFromTable(macros, macroName);
        HandleCustom(iterator, macro);
    }
    else
    {
        free(macroString);
        fprintf(stderr, "%s not defined\n", macroName);
        exit(1);
    }

    *state = PLAINTEXT;
    FreeString(macroString);
}

String *ReadParameter(Iterator *iterator, bool nonEmptyAndAlnum) {
    String *string = CreateString();
    
    char c = NextChar(iterator);
    if (c != '{')
    {
        fprintf(stderr, "expected {\n");
        exit(1);
    }

    size_t braceCount = 1;
    bool inEscape = false;
    while (!((c = NextChar(iterator)) == '}' && !inEscape && braceCount == 1))
    {
        if (inEscape)
        {
            AppendToString(string, '\\');
            AppendToString(string, c);
            inEscape = false;
        }
        else
        {
            if (nonEmptyAndAlnum && !isalnum(c))
            {
                fprintf(stderr, "%c is not alphanumeric\n", c);
                exit(1);
            }
            else if (c == '{')
                braceCount++;
            else if (c == '}')
                braceCount--;
            else if (c == '\\')
            {
                inEscape = true;
                continue;
            }
            else if (c == EOF)
            {
                if (!IsEmpty(iterator))
                    continue;
                
                fprintf(stderr, "expected }, found EOF\n");
                exit(1);
            }
            
            AppendToString(string, c);
        }
    }

    if (nonEmptyAndAlnum && StringLength(string) == 0)
    {
        fprintf(stderr, "%c is not alphanumeric\n", c);
        exit(1);
    }
    
    return string;
}

String **ReadParameters(size_t n, Iterator *iterator) {
    String **parameters = malloc(sizeof(String *) * n);
    for (int i = 0; i < n; i++)
    {
        parameters[i] = ReadParameter(iterator, false);
    }

    return parameters;
}
