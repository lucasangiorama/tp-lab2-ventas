#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "sales.h"
#include "utils.h"

char userSelect(){
  char option, exitValue;
  printf("\n=== MENU DE SELECCIÓN ===\n");
  printf("1) Crear archivo\n");
  printf("2) Agregar venta\n");
  printf("3) Listado de ventas\n");
  printf("4) Buscar por vendedor/número de orden\n");
  printf("5) Modificar precio\n");
  printf("6) Desactivar venta\n");
  printf("7) Eliminar venta (crear archivo respaldo)\n");
  printf("8) Lista de archivo respaldo\n");
  printf("9) Salir\n");
  printf("=========================\n");
  printf("Seleccione una opcion: ");
  scanf(" %c", &option);

  if (option >= '1' && option <= '8'){
    return option;
  }
  else if (option == '9'){
    printf("\nSeguro que quiere salir del programa? si(Y) o no(N): ");
    scanf(" %c", &exitValue);
    if (exitValue == 'y' || exitValue == 'Y'){
      return '9';
    }
    else{
      return userSelect();
    }
  }
  printf("\nOpcion invalida. Intente nuevamente.\n");
  return userSelect();
}

void createFile(FILE *file){
  printf("\n--- Creando archivo de ventas ---\n");
  file = fopen("ventas.dat", "w+b");
  if (file == NULL){
    printf("No se puede crear el archivo.\n");
  }
  else{
    printf("Se creo el archivo.\n");
    closeFile(file);
    file = NULL;
  }
}

void newSale(FILE *file){
  int aux;
  struct sales sale = {0};
  int optionCategory;

  printf("\n--- Agregar venta ---\n");
  file = fopen("ventas.dat", "r+b");

  if (file == NULL){
    printf("Archivo no existe.\n");
    return;
  }

  printf("Ingresar id de venta: ");
  scanf("%d", &sale.id);
  aux = sale.id;

  clearInputBuffer();

  if (aux >= 1){
    struct sales tempSale;
    fseek(file, aux * sizeof(struct sales), SEEK_SET);
    int read = fread(&tempSale, sizeof(struct sales), 1, file);

    if (read > 0 && tempSale.id == aux){
      printf("El id ya existe\n");
      closeFile(file);
      return;
    }
    else{
      sale.id = aux;
      printf("Se cargo el id %d con exito.\n", aux);

      // === CARGA VENDEDOR ===
      printf("Ingrese nombre del vendedor: ");
      scanf("%s", sale.sellerName);

      // === VALIDAR DÍA ===
      do{
        printf("\nDia (1-31): ");
        scanf("%d", &sale.day);
        if (sale.day < 1 || sale.day > 31){
          printf("El dia debe estar entre 1 y 31.\n");
        }
      } while (sale.day < 1 || sale.day > 31);

      // === VALIDAR MES ===
      do{
        printf("Mes (1-12): ");
        scanf("%d", &sale.month);
        if (sale.month < 1 || sale.month > 12){
          printf("El mes debe estar entre 1 y 12.\n");
        }
      } while (sale.month < 1 || sale.month > 12);

      // === VALIDAR AÑO ===
      do{
        printf("Año (20-27): ");
        scanf("%d", &sale.year);
        if (sale.year < 20 || sale.year > 27){
          printf("El año debe estar entre 20 y 27.\n");
        }
      } while (sale.year < 20 || sale.year > 27);

      // === SELECCIONAR CATEGORÍA ===
      do{
        printf("\n=== SELECCIONE CATEGORIA ===\n");
        printf("1) Zapato\n");
        printf("2) Deportivo\n");
        printf("3) Sandalia\n");
        printf("Opcion: ");
        scanf("%d", &optionCategory);

        switch (optionCategory){
          case 1:
            strcpy(sale.category, "Zapato");
            break;
          case 2:
            strcpy(sale.category, "Deportivo");
            break;
          case 3:
            strcpy(sale.category, "Sandalia");
            break;
          default:
            printf("Opcion invalida. Seleccione 1, 2 o 3.\n");
            optionCategory = 0;
        }
      } while (optionCategory < 1 || optionCategory > 3);

      // === VALIDAR CANTIDAD ===
      do{
        printf("\nCantidad: ");
        if (scanf("%d", &sale.quantity) != 1){
          printf("Debe ingresar un número entero.\n");
          clearInputBuffer();
          sale.quantity = -1;
        }
        else if (sale.quantity <= 0){
          printf("La cantidad debe ser mayor a 0.\n");
        }
      } while (sale.quantity <= 0);

      // === VALIDAR PRECIO UNITARIO ===
      do{
        printf("\nPrecio unitario: $");
        if (scanf("%f", &sale.unitPrice) != 1){
          printf("Debe ingresar un número.\n");
          clearInputBuffer();
          sale.unitPrice = -1;
        }
        else if (sale.unitPrice <= 0){
          printf("El precio debe ser mayor a 0.\n");
        }
      } while (sale.unitPrice <= 0);

      // === VALIDAR DESCUENTO ===
      do{
        printf("\nDescuento (0-100) (0 no tiene descuento): ");
        if (scanf("%f", &sale.discount) != 1){
          printf("Debe ingresar un número.\n");
          clearInputBuffer();
          sale.discount = -1;
        }
        else if (sale.discount < 0 || sale.discount > 100){
          printf("El descuento debe estar entre 0 y 100.\n");
        }
      } while (sale.discount < 0 || sale.discount > 100);

      // === CALCULAR TOTALES ===
      priceData(&sale);

      // === RESUMEN ===
      printf("\n=== RESUMEN DE LA VENTA ===\n");
      printf("ID: %d\n", sale.id);
      printf("Vendedor: %s\n", sale.sellerName);
      printf("Fecha: %02d/%02d/%02d\n", sale.day, sale.month, sale.year);
      printf("Categoria: %s\n", sale.category);
      printf("Cantidad: %d\n", sale.quantity);
      printf("Precio Unitario: $%.2f\n", sale.unitPrice);
      printf("Descuento: $%.2f\n", sale.discount);
      printf("Subtotal: $%.2f\n", sale.subtotal);
      printf("IVA (21%%): $%.2f\n", sale.tax);
      printf("TOTAL: $%.2f\n", sale.total);
      printf("Activo: %d\n", sale.active);
      printf("===========================\n");

      // === GUARDAR EN ARCHIVO ===
      fseek(file, aux * sizeof(struct sales), SEEK_SET);
      fwrite(&sale, sizeof(struct sales), 1, file);
      printf("\n[Venta guardada en el archivo]\n");

      closeFile(file);
      return;
    }
  }
  closeFile(file);
}

void listMenu(FILE *file){
  char option;
  int categoryOption;
  char filterCategory[30];
  
  printf("\n=== OPCIONES DE LISTADO ===\n");
  printf("1) Listar todos (activos e inactivos)\n");
  printf("2) Listar por categoría\n");
  printf("Seleccione opción: ");
  scanf(" %c", &option);
  
  if (option == '1'){
    viewList(file, NULL);
  } 
  else if (option == '2'){
    printf("\n=== SELECCIONE CATEGORIA ===\n");
    printf("1) Zapato\n");
    printf("2) Deportivo\n");
    printf("3) Sandalia\n");
    printf("Opción: ");
    scanf(" %d", &categoryOption);

    switch(categoryOption){
      case 1:
        strcpy(filterCategory, "Zapato");
        break;
      case 2:
        strcpy(filterCategory, "Deportivo");
        break;
      case 3:
        strcpy(filterCategory, "Sandalia");
        break;
      default:
        printf("Opción de categoría inválida.\n");
        return;
    }
    viewList(file, filterCategory);
  }
  else{
    printf("Opción inválida.\n");
  }
}

void viewList(FILE *file, char *filterCategory){
  struct sales sale;
  int i = 1;
  int found = 0;

  printf("\n=== CONTENIDO DEL ARCHIVO ===\n");
  
  if (filterCategory != NULL){
    printf("Filtrando por categoría: %s\n\n", filterCategory);
  }

  file = fopen("ventas.dat", "rb");

  if (file == NULL){
    printf("No se puede abrir el archivo.\n");
    return;
  }

  int registerQuantity = getRegisterCount(file);

  if (registerQuantity == 0){
    printf("El archivo está vacío.\n");
    closeFile(file);
    return;
  }

  printTable(1, sale);

  while (i < registerQuantity){
    fseek(file, i * sizeof(struct sales), SEEK_SET);
    fread(&sale, sizeof(struct sales), 1, file);

    int view = 0;
    
    if (filterCategory == NULL){
      view = 1;
    } else{
      if (sale.id != 0 && strcmp(sale.category, filterCategory) == 0){
        view = 1;
      }
    }

    if (view){
      if (found > 0){
        printTable(2, sale);
      }
      printTable(4, sale);
      found++;
    }
    i++;
  }
  
  printTable(3, sale);
  
  if (filterCategory != NULL && found == 0){
    printf("\nNo se encontraron ventas de la categoría '%s'.\n", filterCategory);
  }
  
  closeFile(file);
}

void searchSale(FILE *file){
  int option, orderId;
  struct sales sale;

  file = fopen("ventas.dat", "rb");

  if (file == NULL){
    printf("No se puede abrir el archivo.\n");
    return;
  }
  
  printf("\n=== BUSCAR VENTA ===\n");
  printf("1) Buscar por número de orden\n");
  printf("2) Buscar por nombre del vendedor\n");
  printf("0) Volver al menú principal\n");
  printf("Opción: ");
  scanf("%d", &option);
  clearInputBuffer();
  
  switch(option){
    case 1:
      orderId = userOrderSelect();
      if (orderId < 1){
        printf("El número de orden debe ser mayor o igual a 1.\n");
        closeFile(file);
        return;
      }
      
      fseek(file, orderId * sizeof(struct sales), SEEK_SET);
      
      if (fread(&sale, sizeof(struct sales), 1, file) > 0){
        if (sale.id == orderId && sale.active == 1){
          printTable(1, sale);
          printTable(4, sale);
          printTable(3, sale);    
        } 
        else{
          printf("La venta esta inactiva o no existe.\n");
        }
      } 
      else{
        printf("No se encontró venta con ID %d.\n", orderId);
      }
      break;

    case 2: {
      char sellerNameSelect[30];
      printf("Ingrese nombre del vendedor:\n");
      scanf("%s", sellerNameSelect);
      clearInputBuffer();
      
      int registerQuantity = getRegisterCount(file);
      int found = 0;
      
      for (int i = 0; i < registerQuantity; i++){
        fseek(file, i * sizeof(struct sales), SEEK_SET);
        fread(&sale, sizeof(struct sales), 1, file);
        
        if (sale.id != 0 && sale.active == 1 && 
            strcasecmp(sale.sellerName, sellerNameSelect) == 0){
          
          if (found == 0){
            printf("\n=== VENTAS DE %s ===\n", sellerNameSelect);
            printTable(1, sale);
          } 
          else{
            printTable(2, sale);
          }
          printTable(4, sale);
          found++;
        }
      }
      
      if (found > 0){
        printTable(3, sale);
      } 
      else{
        printf("No se encontraron ventas del vendedor '%s'.\n", sellerNameSelect);
      }
      break;
    }
    case 0:
      printf("Volviendo al menú principal...\n");
      break;
        
    default:
      printf("Opción inválida.\n");
      break;
  }
  closeFile(file);
}

void modifSale(FILE *file){
  int orderId;
  char optionUser;
  struct sales sale;

  printf("\n=== MODIFICAR VENTA ===\n");
  file = fopen("ventas.dat", "r+b");
  if (file == NULL){
    printf("No se puede abrir el archivo.\n");
    return;
  }
  
  orderId = userOrderSelect();
  
  // Buscar la venta
  fseek(file, orderId * sizeof(struct sales), SEEK_SET);
  int read = fread(&sale, sizeof(struct sales), 1, file);
  
  if (read == 0 || sale.id != orderId) {
    printf("No se encontró la venta con orden %d.\n", orderId);
    closeFile(file);
    return;
  }
  
  printf("\n¿Estás seguro que querés modificar el precio de la venta %d? (y/n): ", orderId);
  scanf(" %c", &optionUser);
  clearInputBuffer();
  
  switch (optionUser){
    case 'y':
    case 'Y':
      do{
        printf("\nPrecio actual: $%.2f\n", sale.unitPrice);
        printf("Ingrese el nuevo precio por unidad: $");
        // valido "scanf("%f", &sale.unitPrice) != 1" me devuelve 0 si cargan texto me devuelve 1 sin caragn un numero entero
        if (scanf("%f", &sale.unitPrice) != 1){
          printf("Debe ingresar un número.\n");
          clearInputBuffer();
          sale.unitPrice = -1;
        }
        else if (sale.unitPrice <= 0){
          printf("El precio debe ser mayor a 0.\n");
        }
      } while (sale.unitPrice <= 0);
      
      priceData(&sale);
      
      printf("\nPrecio nuevo: $%.2f\n", sale.unitPrice);
      printf("Subtotal nuevo: $%.2f\n", sale.subtotal);
      printf("I.V.A nuevo: $%.2f\n", sale.tax);
      printf("Total nuevo: $%.2f\n", sale.total);
      
      fseek(file, orderId * sizeof(struct sales), SEEK_SET);
      fwrite(&sale, sizeof(struct sales), 1, file);
      
      printf("\n[Venta guardada exitosamente]\n");
      break;
    
    case 'n':
    case 'N':
      printf("Operación cancelada.\n");
      break;
      
    default:
      printf("Opción inválida.\n");
      break;
  }
  closeFile(file);
}

void disableSale(FILE *file){
  int orderId;
  char optionUser;
  struct sales sale;

  printf("\n=== DESACTIVAR VENTA ===\n");
  file = fopen("ventas.dat", "r+b");
  if (file == NULL){
    printf("No se puede abrir el archivo.\n");
    return;
  }
  
  orderId = userOrderSelect();
  
  // Buscar la venta
  fseek(file, orderId * sizeof(struct sales), SEEK_SET);
  int read = fread(&sale, sizeof(struct sales), 1, file);
  
  if (read == 0 || sale.id != orderId) {
    printf("No se encontró la venta con orden %d.\n", orderId);
    closeFile(file);
    return;
  }

  if (sale.active == 0) {
    printf("La venta %d ya está inactiva.\n", orderId);
    closeFile(file);
    return;
  }
  
  printf("\n--- Información de la venta ---\n");
  printf("Vendedor: %s\n", sale.sellerName);
  printf("Total: $%.2f\n", sale.total);
  printf("Estado actual: ACTIVA\n");

  printf("\n¿Estás seguro que querés desactivar la venta con numero de orden %d? (y/n): ", orderId);
  scanf(" %c", &optionUser);
  clearInputBuffer();
  
  switch (optionUser){
    case 'y':
    case 'Y':
      sale.active = 0;
      
      fseek(file, orderId * sizeof(struct sales), SEEK_SET);
      fwrite(&sale, sizeof(struct sales), 1, file);
      
      printf("\nVenta actualizada exitosamente\n");
      printf("\n=== VENTAS ACTIVAS ===\n");
      int registerQuantity = getRegisterCount(file);
      int found = 0;
      
      printTable(1, sale);
      for (int i = 0; i < registerQuantity; i++){
        struct sales tempSale;
        fseek(file, i * sizeof(struct sales), SEEK_SET);
        fread(&tempSale, sizeof(struct sales), 1, file);
        
        if (tempSale.id != 0 && tempSale.active == 1){
          if (found > 0){
            printTable(2, tempSale);
          }
          printTable(4, tempSale);
          found++;
        }
      }
      printTable(3, sale);
      if (found == 0){
        printf("No hay ventas activas.\n");
      }
      break;
    case 'n':
    case 'N':
      printf("Operación cancelada.\n");
      break;
    default:
      printf("Opción inválida.\n");
      break;
  }
  closeFile(file);
}

void deleteSale(FILE *file){
  struct sales sale;
  struct sales zeroSale = {0};
  char filename[50];
  time_t t;
  struct tm *tm_info;
  FILE *backupFile;
  FILE *temp;
  int found = 0;
  
  printf("\n=== BAJA FÍSICA DE VENTAS INACTIVAS ===\n");

  file = fopen("ventas.dat", "rb");
  if (file == NULL){
    printf("No se puede abrir el archivo.\n");
    return;
  }
  
  temp = fopen("ventas.temp", "w+b");
  if (temp == NULL){
    printf("No se pudo crear archivo temporal.\n");
    fclose(file);
    return;
  }
  
  t = time(NULL);
  tm_info = localtime(&t);
  sprintf(filename, "bajas_%02d-%02d-%04d.xyz",tm_info->tm_mday,1 + tm_info->tm_mon, 1900 + tm_info->tm_year);
  
  backupFile = fopen(filename, "w");
  if (backupFile == NULL){
    printf("No se pudo crear archivo de respaldo.\n");
    fclose(file);
    fclose(temp);
    return;
  }
  
  // Procesar cada venta
  while(fread(&sale, sizeof(struct sales), 1, file) == 1){
    if (sale.id != 0 && sale.active == 0){
      // GUARDAR TODOS LOS DATOS
      fprintf(backupFile, "%d|%s|%02d/%02d/%02d|%s|%d|%.2f|%.2f|%.2f|%.2f|%.2f\n",
              sale.id, 
              sale.sellerName, 
              sale.day, sale.month, sale.year,
              sale.category, 
              sale.quantity,
              sale.unitPrice,
              sale.discount,
              sale.subtotal,
              sale.tax,
              sale.total);
      
      fwrite(&zeroSale, sizeof(struct sales), 1, temp);
      found++;
    }
    else{
      fwrite(&sale, sizeof(struct sales), 1, temp);
    }
  }
  
  fclose(backupFile);
  fclose(temp);
  fclose(file);

  remove("ventas.dat");
  rename("ventas.temp", "ventas.dat");
  
  printf("\n%d ventas dadas de baja\n", found);
  printf("Respaldo: %s\n", filename);
}

void viewTxt(FILE *file){
  char filename[50];
  char line[256];
  FILE *backupFile;
  struct sales sale;
  int found = 0;
  
  printf("\n=== LISTAR ARCHIVO DE RESPALDO ===\n");
  printf("Ingrese el nombre del archivo (ejemplo: bajas_25-10-2025.xyz): ");
  scanf("%s", filename);
  clearInputBuffer();
  
  backupFile = fopen(filename, "r");
  if (backupFile == NULL){
    printf("No se pudo abrir el archivo '%s'.\n", filename);
    printf("Verifique que el nombre sea correcto.\n");
    return;
  }
  
  printf("\n=== CONTENIDO DE %s ===\n", filename);
  
  printTable(1, sale);
  
  while(fgets(line, sizeof(line), backupFile) != NULL){
    int result = sscanf(line, "%d|%30[^|]|%d/%d/%d|%30[^|]|%d|%f|%f|%f|%f|%f",
           &sale.id, 
           sale.sellerName,
           &sale.day, &sale.month, &sale.year,
           sale.category,
           &sale.quantity,
           &sale.unitPrice,
           &sale.discount,
           &sale.subtotal,
           &sale.tax,
           &sale.total);
    
    if (result != 12) {
      continue;
    }
    
    sale.active = 0;
    
    if (found > 0){
      printTable(2, sale);
    }
    printTable(4, sale);
    found++;
  }
  
  printTable(3, sale);
  
  if (found == 0){
    printf("El archivo está vacío o tiene formato incorrecto.\n");
  }
  
  fclose(backupFile);
}
