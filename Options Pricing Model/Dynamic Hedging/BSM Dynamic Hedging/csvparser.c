
#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "csvparser.h"

CsvParser *CsvParser_new(const char *filePath, const char *delimiter, int firstLineIsHeader) {
	CsvParser *csvParser = malloc(sizeof(CsvParser));
	if (filePath == NULL) {
		csvParser->filePath_ = NULL;
	} else {
		int filePathLen = strlen(filePath);
		csvParser->filePath_ = malloc((filePathLen + 1));
		strcpy(csvParser->filePath_, filePath);
	}
	csvParser->firstLineIsHeader_ = firstLineIsHeader;
	csvParser->errMsg_ = NULL;
	if (delimiter == NULL) {
		csvParser->delimiter_ = ',';
	} else if (_CsvParser_delimiterIsAccepted(delimiter)) {
		csvParser->delimiter_ = *delimiter;
	} else {
		csvParser->delimiter_ = '\0';
	}
	csvParser->header_ = NULL;
	csvParser->fileHandler_ = NULL;

	return csvParser;
}

void CsvParser_destroy(CsvParser *csvParser) {
	if (csvParser == NULL) {
		return;
	}
	if (csvParser->filePath_ != NULL) {
		free(csvParser->filePath_);
	}
	if (csvParser->errMsg_ != NULL) {
		free(csvParser->errMsg_);
	}
	if (csvParser->fileHandler_ != NULL) {
		fclose(csvParser->fileHandler_);
	}
	if (csvParser->header_ != NULL) {
		CsvParser_destroy_row(csvParser->header_);
	}
	free(csvParser);
}

void CsvParser_destroy_row(CsvRow *csvRow) {
	int i;
	for (i = 0 ; i < csvRow->numOfFields_ ; i++) {
		free(csvRow->fields_[i]);
	}
	free(csvRow);
}

CsvRow *CsvParser_getHeader(CsvParser *csvParser) {
	if (! csvParser->firstLineIsHeader_) {
		_CsvParser_setErrorMessage(csvParser, "Cannot supply header, as current CsvParser object does not support header");
		return NULL;
	}
	if (csvParser->header_ == NULL) {
		csvParser->header_ = _CsvParser_getRow(csvParser);
	}
	return csvParser->header_;
}

CsvRow *CsvParser_getRow(CsvParser *csvParser) {
	if (csvParser->firstLineIsHeader_ && csvParser->header_ == NULL) {
		csvParser->header_ = _CsvParser_getRow(csvParser);
	}
	return _CsvParser_getRow(csvParser);
}

int CsvParser_getNumFields(CsvRow *csvRow) {
	return csvRow->numOfFields_;
}

char **CsvParser_getFields(CsvRow *csvRow) {
	return csvRow->fields_;
}

CsvRow *_CsvParser_getRow(CsvParser *csvParser) {
	int accceptedFields = 64;
	int acceptedCharsInField = 64;
	CsvRow *csvRow = malloc(sizeof(CsvRow));
	int fieldIter = 0;
	char *currField = malloc(acceptedCharsInField);
	int inside_complex_field = 0;
	int currFieldCharIter = 0;
	int seriesOfQuotesLength = 0;
	int lastCharIsQuote = 0;
	int isEndOfFile = 0;

	csvRow->fields_ = malloc(accceptedFields * sizeof(char*));
	csvRow->numOfFields_ = 0;

	if (csvParser->filePath_ == NULL) {
		_CsvParser_setErrorMessage(csvParser, "Supplied CSV file path is NULL");
		return NULL;
	}
	if (csvParser->delimiter_ == '\0') {
		_CsvParser_setErrorMessage(csvParser, "Supplied delimiter is not supported");
		return NULL;
	}
	if (csvParser->fileHandler_ == NULL) {
		csvParser->fileHandler_ = fopen(csvParser->filePath_, "r");
		if (csvParser->fileHandler_ == NULL) {
			int errorNum = errno;
			const char *errStr = strerror(errorNum);
			char *errMsg = malloc(1024 + strlen(errStr));
			strcpy(errMsg, "");
			sprintf(errMsg, "Error opening CSV file for reading: %s : %s", csvParser->filePath_, errStr);
			_CsvParser_setErrorMessage(csvParser, errMsg);
			free(errMsg);
			return NULL;
		}
	}
	
	while (1) {
		char currChar = fgetc(csvParser->fileHandler_);
		if (feof(csvParser->fileHandler_)) {
			if (currFieldCharIter == 0 && fieldIter == 0) {
				_CsvParser_setErrorMessage(csvParser, "Reached EOF");
				return NULL;
			}
			currChar = '\n';
			isEndOfFile = 1;
		}
		if (currChar == '\r') {
			continue;
		}
		if (currFieldCharIter == 0	 && ! lastCharIsQuote) {
			if (currChar == '\"') {
				inside_complex_field = 1;
				lastCharIsQuote = 1;
				continue;
			}
		} else if (currChar == '\"') {
			seriesOfQuotesLength++;
			inside_complex_field = (seriesOfQuotesLength % 2 == 0);
			if (inside_complex_field) {
				currFieldCharIter--;
			}
		} else {
			seriesOfQuotesLength = 0;
		}
		if (isEndOfFile || ((currChar == csvParser->delimiter_ || currChar == '\n') && ! inside_complex_field)) {
			currField[lastCharIsQuote ? currFieldCharIter - 1 : currFieldCharIter] = '\0';
			csvRow->fields_[fieldIter] = malloc(currFieldCharIter + 1);
			strcpy(csvRow->fields_[fieldIter], currField);
			free(currField);
			csvRow->numOfFields_++;
			if (currChar == '\n') {
				return csvRow;
			}
			acceptedCharsInField = 64;
			currField = malloc(acceptedCharsInField);
			currFieldCharIter = 0;
			fieldIter++;
			inside_complex_field = 0;
		} else {
			currField[currFieldCharIter] = currChar;
			currFieldCharIter++;
			if (currFieldCharIter == acceptedCharsInField - 1) {
				acceptedCharsInField *= 2;
				currField = realloc(currField, acceptedCharsInField);
			}
		}
		lastCharIsQuote = (currChar == '\"') ? 1 : 0;
	}
}

int CsvParser_getNumOfFields(const CsvRow *csvRow) {
	return csvRow->numOfFields_;
}

int _CsvParser_delimiterIsAccepted(const char *delimiter) {
	char actualDelimiter = *delimiter;
	if (actualDelimiter == '\n' || actualDelimiter == '\r' || actualDelimiter == '\0' ||
			actualDelimiter == '\"') {
		return 0;
	}
	return 1;
}

void _CsvParser_setErrorMessage(CsvParser *csvParser, const char *errorMessage) {
	int errMsgLen = strlen(errorMessage);
	if (csvParser->errMsg_ != NULL) {
		free(csvParser->errMsg_);
	}
	csvParser->errMsg_ = malloc(errMsgLen + 1);
	strcpy(csvParser->errMsg_, errorMessage);
}

const char *CsvParser_getErrorMessage(CsvParser *csvParser) {
	return csvParser->errMsg_;
}