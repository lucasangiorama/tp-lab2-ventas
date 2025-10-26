#ifndef UTILS_H
#define UTILS_H

#include "sales.h"

FILE* openFile(const char *mode);
void closeFile(FILE *file);
void printTable(int option, struct sales sale);
void clearScreen();
int userOrderSelect();
int getRegisterCount(FILE *file);
void clearInputBuffer();
void priceData(struct sales *sale);

#endif