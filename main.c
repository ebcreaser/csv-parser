#include <stdio.h>
#include <stdlib.h>

#include "csv.h"

#define MAXLINE 100

int
main(int argc, char *argv[])
{
    char **fields;
    char *str;
    char *record;
    char *field;
    int i = 0;

    fields = malloc(sizeof(char *) * argc);
    if (fields == NULL) {
        printf("failed to allocate memory\n");
        return 1;
    }
    for (i = 0; i < argc; ++i)
        fields[i] = WriteField(argv[i]);
    record = WriteRecord(fields, argc);
    for (i = 0; i < argc; ++i)
        free(fields[i]);
    free(fields);
    printf("%s", record);
    CsvCheckSyntax(record);
    field = record;
    while((str = ReadField(&field))) {
        printf("%s\n", str);
        free(str);
    }
    free(record);

    return 0;
}
