#include <stdio.h>
#include "sales.h"
#include "utils.h"

int main(){
  char option;
  FILE *file;

  do{
    clearScreen();
    option = userSelect();
    printf("\n");
    switch (option){
      case '1':
        createFile(file);
        break;
      case '2':
        newSale(file);
        break;
      case '3':
        listMenu(file);
        break;
      case '4':
        searchSale(file);
        break;
      case '5':
        modifSale(file);
        break;
      case '6':
        disableSale(file);
        break;
      case '7':
        deleteSale(file);
        break;
      case '8':
        viewTxt(file);
        break;
    }
    
    if(option != '9'){
      printf("\n Presionar Enter para continuar.");
      while(getchar() != '\n'); 
      getchar();
    }
    
  } while (option != '9');
  return 0;
}