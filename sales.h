#ifndef SALES_H
#define SALES_H

#include <stdio.h>

struct sales {
    int id;
    char sellerName[50];
    int day;
    int month;
    int year;
    char category[30];
    int quantity;
    float unitPrice;
    float discount;
    float subtotal;
    float tax;
    float total;
    int active;
};


void clearScreen();
char userSelect();
void createFile(FILE *file);
void newSale(FILE *file);
void listMenu(FILE *file);
void viewList(FILE *file, char *filterCategory);
void searchSale(FILE *file);
void modifSale(FILE *file);
void disableSale(FILE *file);
void deleteSale(FILE *file);
void viewTxt(FILE *file);

#endif