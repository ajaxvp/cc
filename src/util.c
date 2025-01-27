#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))

#define _DEFAULT_SOURCE 1

#include <libgen.h>

#elif defined(_WIN32) || defined(__CYGWIN__)

#endif

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#include "cc.h"

#define max(x, y) ((x) > (y) ? (x) : (y))

const bool debug_m = true;

// identical malloc'd copy of the parameter
char* strdup(const char* str)
{
    size_t len = strlen(str);
    char* dup = malloc(len + 1);
    dup[len] = '\0';
    strncpy(dup, str, len + 1);
    return dup;
}

bool contains_substr(char* str, char* substr)
{
    size_t len_substr = strlen(substr);
    if (!len_substr)
        return true;
    size_t found = 0;
    for (; *str; ++str)
    {
        if (*str == substr[found])
            ++found;
        else
            found = 0;
        if (found == len_substr)
            return true;
    }
    return false;
}

bool streq(char* s1, char* s2)
{
    if (s1 == NULL && s2 == NULL)
        return true;
    return !strcmp(s1, s2);
}

bool in_debug(void)
{
    return debug_m;
}

int int_array_index_max(int* array, size_t length)
{
    if (!length)
        return -1;
    int mi = -1, max_value = -0x80000000;
    for (size_t i = 0; i < length; ++i)
    {
        if (array[i] > max_value)
        {
            max_value = array[i];
            mi = i;
        }
    }
    return mi;
}

void print_int_array(int* array, size_t length)
{
    printf("[");
    for (int i = 0; i < length; ++i)
    {
        if (i != 0)
            printf(", ");
        printf("%d", array[i]);
    }
    printf("]\n");
}

bool starts_ends_with_ignore_case(char* str, char* substr, bool ends)
{
    if (!str && !substr)
        return true;
    if (!str || !substr)
        return false;
    size_t str_len = strlen(str);
    size_t substr_len = strlen(substr);
    if (substr_len > str_len)
        return false;
    if (!substr_len)
        return true;
    size_t i = 0;
    for (; i < substr_len; ++i)
    {
        if (tolower(str[ends ? (str_len - 1 - i) : i]) != tolower(substr[ends ? (substr_len - 1 - i) : i]))
            return false;
    }
    return true;
}

// not finished but idrc
void repr_print(char* str, int (*printer)(const char* fmt, ...))
{
    for (; *str; ++str)
    {
        switch (*str)
        {
            case '\n':
                printer("\\n");
                break;
            case '\t':
                printer("\\t");
                break;
            case '\b':
                printer("\\b");
                break;
            default:
                printer("%c", *str);
                break;
        }
    }
}

// not mine
unsigned long hash(char* str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

// result is malloc'd
char* get_directory_path(char* path)
{
    if (!path)
        return strdup("");

    #if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))

    char* fullpath = realpath(path, NULL);
    char* dir = strdup(dirname(fullpath));
    free(fullpath);
    return dir;

    #elif defined(_WIN32) || defined(__CYGWIN__)

    return NULL;

    #else

    return NULL;

    #endif
}

// m determines whether or not the filename uses an internal buffer (overwritten with successive calls) or gets malloc'd
char* get_file_name(char* path, bool m)
{
    static char result[LINUX_MAX_PATH_LENGTH];

    if (!path)
        return m ? strdup("") : (result[0] = '\0', result);

    #if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))

    char* fullpath = realpath(path, NULL);
    char* name = m ? strdup(basename(fullpath)) : strncpy(result, basename(fullpath), LINUX_MAX_PATH_LENGTH);
    free(fullpath);
    return name;

    #elif defined(_WIN32) || defined(__CYGWIN__)

    return NULL;

    #else

    return NULL;

    #endif
}