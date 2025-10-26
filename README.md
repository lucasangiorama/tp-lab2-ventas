# TP 1 - LABORATORIO DE COMPUTACIÓN II - CATEDRA A

**Autor:** Lucas Angiorama  
**Fecha de Entrega:** [DD/MM/AAAA]

## Archivos del Proyecto

- `main.c` - Programa principal y menú
- `functions.c` - Implementación de funciones principales
- `utils.c` - Funciones auxiliares y utilidades
- `sales.h` - Definición de estructuras y prototipos
- `utils.h` - Prototipos de funciones auxiliares
- `README.md` - Documentación del proyecto

## Inicializar Proyecto

```bash
gcc main.c functions.c utils.c -o tp
./tp
```

## Funciones del Menú

### `createFile()` - Opción 1
Crea archivo `ventas.dat` vacío. Inicializa el sistema.

### `newSale()` - Opción 2
Agrega nueva venta. Pide: ID, vendedor, fecha, categoría, cantidad, precio, descuento. Calcula automáticamente subtotal, IVA (21%) y total. Guarda en archivo.

### `listMenu()` - Opción 3
Muestra todas las ventas o filtra por categoría (Zapato/Deportivo/Sandalia). Formato tabla.

### `searchSale()` - Opción 4
Busca ventas por número de orden o nombre de vendedor. Solo muestra activas.

### `modifSale()` - Opción 5
Modifica precio unitario de una venta. Recalcula totales automáticamente.

### `disableSale()` - Opción 6
Desactiva una venta (baja lógica). Cambia estado a inactivo sin eliminar. Muestra tabla con ventas activas restantes.

### `deleteSale()` - Opción 7
Baja física de ventas inactivas. Crea archivo respaldo `bajas_DD-MM-AAAA.xyz` con los datos, luego reemplaza registros con ceros en archivo principal.

### `viewTxt()` - Opción 8
Muestra contenido de archivo de respaldo en formato tabla.