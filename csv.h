#ifndef CSV_H
#define CSV_H

int CsvCheckSyntax(char *str);
char *ReadField(char **str);
int FieldReadLength(char *str);
char *WriteField(char *str);
int FieldWriteLength(char *str);
char *WriteRecord(char *fields[], int fieldCount);

#endif
