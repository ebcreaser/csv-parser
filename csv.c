#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csv.h"

/* Returns position of synax error if one is found, 0 otherwise */
int 
CsvCheckSyntax(char *str)
{
    if (str == NULL) {
        printf("Csv: null pointer passed as parameter\n");
        return -1;
    }

    int i;
    char inQuotes = 0;

    for(i = 0; str[i]; ++i) {
        switch (str[i]) {
        case '\n':
            if (str[i + 1])
                return i + 1;
            break;
        case '"':
            inQuotes = !inQuotes;
            break;
        case '\\':
            switch (str[++i]) {
            case '"': /* fallthough */
            case '\\': 
                break;
            default:
                return i + 1;
                break;
            }
            break;
        default:
            break;
        }
    }

    return inQuotes ? i + 1 : 0;
}

/* Returns the next field in the string pointed to by strPtr or
 * NULL if no more fields are read.
 * Sets the value pointed to by strPtr to the memory location of
 * the next field.
 */
char *
ReadField(char **str)
{
    if (str == NULL || *str == NULL) {
        printf("ReadField: null pointer passed as parameter\n");
        return NULL;
    }

    char *token;
    int i;
    int len;

    len = FieldReadLength(*str);
    if (len == 0)
        return NULL;
    token = malloc(len);
    if (token == NULL) {
        printf("csv: FieldToken: failed to allocate memory\n");
        return NULL;
    }
    token[len - 1] = '\0';
    for (i = 0; i < len - 1; ++(*str)) {
        switch (**str) {
        case '"':
            break;
        case '\\': /* fallthrough */
            ++(*str);
        default:
            token[i++] = **str;
            break;
        }
    }
    /* ignore all remaining non-literal characters */
    while(**str == '\n' || **str == '"' || **str == ',')
        ++(*str);

    return token;
}

/* Returns the length of a string large enough to store str after
 * parsing it as a csv field, including a terminting null byte.
 */
int
FieldReadLength(char *str)
{
    if (str == NULL) {
        printf("FieldLenth: null pointer passed as parameter\n");
        return 0;
    }

    int i;
    int len = 1;
    char inQuotes = 0;

    for(i = 0; str[i]; ++i) {
        switch (str[i]) {
        case '\n':
            return len;
            break;
        case ',':
            if (inQuotes)
                ++len;
            else
                return len;
            break;
        case '"':
            inQuotes = !inQuotes;
            break;
        case '\\': /* fallthrough */
            ++i;
        default:
            ++len;
            break;
        }
    }
    
    return 0;
}

/* Returns a string that stores str as a csv field. */
char *
WriteField(char *str)
{
    if (str == NULL) {
        printf("WriteField: null pointer passed as parameter\n");
        return NULL;
    }

    char *field;
    int i, j;
    int len = 3;

    len = FieldWriteLength(str);
    field = malloc(len);
    if (field == NULL) {
        printf("ReadField: failed to allocate memory\n");
        return NULL;
    }
    field[0] = '"';
    field[len - 2] = '"';
    field[len - 1] = '\0';
    for (i = 0, j = 1; j < len - 2; ++i) {
        switch (str[i]) {
        case '"': /* fallthrough */
        case '\\':
            field[j++] = '\\';
        default:
            field[j++] = str[i];
            break;
        }
    }
    
    return field;
}

/* Returns the length of a string large enough to store str as a 
 * csv field, including a terminting null byte.
 */
int
FieldWriteLength(char *str)
{
    int i = 0;
    int len = 3;

    for (i = 0; str[i]; ++i) {
        switch (str[i]) {
        case '\n':
            break;
        case '"': /* fallthrough */
        case '\\':
            ++len;
        default:
            ++len;
            break;
        }
    }

    return len;
}

/* Returns a csv record containing each field in fields. */
char *
WriteRecord(char *fields[], int fieldCount)
{
    if (fields == NULL) {
        printf("WriteRecord: null pointer passed as parameter\n");
        return NULL;
    }

    char *record;
    int i;
    int len = 1;

    for (i = 0; i < fieldCount; ++i)
        len += strlen(fields[i]) + 1;
    record = malloc(len);
    if (record == NULL) {
        printf("WriteRecord: failed to allocate memory\n");
        return NULL;
    }
    record[0] = '\0';
    for (i = 0; i < fieldCount; ++i) {
        strcat(record, fields[i]);
        if (i == fieldCount - 1)
            strcat(record, "\n");
        else
            strcat(record, ",");
    }

    return record;
}
