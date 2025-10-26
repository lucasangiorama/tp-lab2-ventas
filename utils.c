#include <stdio.h>
#include <stdlib.h>
#include "sales.h"
#include "utils.h"

void closeFile(FILE *file) {
  if (file != NULL) {
    fclose(file);
  }
}

void clearScreen(){
  #ifdef _WIN32
    system("cls");
  #else
    system("clear");
  #endif
}

// Imrpime todas las partes de la tabla 
void printTable(int option, struct sales sale) {
  switch (option){
  case 1:
    printf("┌───────┬──────────┬──────────┬───────────┬──────────┬──────────────────────┬─────────────┬──────────────┬───────────┬─────────────┬────────┐\n");
    printf("│ Orden │ Vendedor │   Fecha  │ Categoria │ Cantidad │   Precio Unitario    │  Descuento  │   Subtotal   │   I.V.A   │    Total    │ Activo │\n");
    printf("├───────┼──────────┼──────────┼───────────┼──────────┼──────────────────────┼─────────────┼──────────────┼───────────┼─────────────┼────────┤\n");
    break;
  case 2:
    printf("├───────┼──────────┼──────────┼───────────┼──────────┼──────────────────────┼─────────────┼──────────────┼───────────┼─────────────┼────────┤\n");
    break;
  case 3:
    printf("└───────┴──────────┴──────────┴───────────┴──────────┴──────────────────────┴─────────────┴──────────────┴───────────┴─────────────┴────────┘\n");
    break;
  case 4:
    if (sale.id == 0) {
      printf("│%7d│%10d│%10d│%11d│%10d│%22d│%13d│%14d│%11d│%13d│%8d│\n",0,0,0,0,0,0,0,0,0,0,0);
    } else {
      printf("│%7d│%10s│  %02d/%02d/%02d│%11s│%10d│              $%7.2f│%12.1f%%│     $%8.2f│   $%7.2f│    $%8.2f│%8d│\n",sale.id, sale.sellerName, sale.day, sale.month, sale.year,sale.category, sale.quantity, sale.unitPrice, sale.discount,sale.subtotal, sale.tax, sale.total, sale.active);
    }
    break;
  default:
    break;
  }
}

int userOrderSelect() {
  int orderId;
  int inputValid;
  do {
    printf("Ingrese número de orden: ");
    //valido si el usurioi escribio solmanete numeros decimales
    inputValid = scanf("%d", &orderId);
    clearInputBuffer();
    
    if (inputValid != 1) {
      printf("Debe ingresar un número entero.\n");
      orderId = -1;
    }
    else if (orderId < 1) {
      printf("El número de orden debe ser mayor o igual a 1.\n");
    }
  } while (orderId < 1);
  return orderId;
}

//acumulador de registros
int getRegisterCount(FILE *file) {
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  int registerQuantity = size / sizeof(struct sales);
  fseek(file, 0, SEEK_SET);
  return registerQuantity;
}

// Lo cree proque lo uso cada vez que le pido al usuario ingresar un string para limpiar el '\n'
void clearInputBuffer() {
  int c;
  while ((c = getchar()) != '\n' && c != EOF);
}

void priceData(struct sales *sale) { 
  sale->subtotal = sale->quantity * sale->unitPrice;
  sale->tax = sale->subtotal * 0.21;
  
  float descuentoMonto = sale->subtotal * (sale->discount / 100.0);
  
  sale->total = sale->subtotal + sale->tax - descuentoMonto;
  sale->active = 1;
}