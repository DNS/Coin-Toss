#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <stdio.h>

typedef struct CsvRow {
	char **fields_;
	int numOfFields_;
} CsvRow;

typedef struct CsvParser {
	char *filePath_;
	char delimiter_;
	int firstLineIsHeader_;
	char *errMsg_;
	CsvRow *header_;
	FILE *fileHandler_;
} CsvParser;


// Public
CsvParser *CsvParser_new(const char *filePath, const char *delimiter, int firstLineIsHeader);
void CsvParser_destroy(CsvParser *csvParser);
void CsvParser_destroy_row(CsvRow *csvRow);
CsvRow *CsvParser_getHeader(CsvParser *csvParser);
CsvRow *CsvParser_getRow(CsvParser *csvParser);
int CsvParser_getNumFields(CsvRow *csvRow);
char **CsvParser_getFields(CsvRow *csvRow);
const char* CsvParser_getErrorMessage(CsvParser *csvParser);
int CsvParser_getNumOfFields(const CsvRow *csvRow);

// Private
CsvRow *_CsvParser_getRow(CsvParser *csvParser);
int _CsvParser_delimiterIsAccepted(const char *delimiter);
void _CsvParser_setErrorMessage(CsvParser *csvParser, const char *errorMessage);


#endif