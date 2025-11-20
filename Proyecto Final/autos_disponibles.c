#include <stdio.h>
#include <stdlib.h>
#include "autos_disponibles.h"
#include "auto.h" // <--- IMPORTANTE: Aquí está la estructura 'Auto' real

void mostrar_un_auto(Auto a)
{
    printf("--------------------------\n");
    printf("Patente: %s\n", a.patente);
    printf("marca: %s\n", a.marca);
    printf("modelo: %s\n", a.modelo);
    printf("kilometraje: %d", a.kms);
    printf("precio: $%.2f\n", a.precioFinal);
    printf("--------------------------\n\n");
}

void mostrar_auto_recursivo( FILE* file,  int pos, int total)
{
    if(pos >= total)
    {
        return;
    }

    fseek(file, pos* sizeof(Auto), SEEK_SET);

    Auto a;

    if(fread(&a, sizeof(Auto), 1, file) == 1)
    {
        mostrar_un_auto(a);
    }

    mostrar_auto_recursivo(file,pos + 1, total);
}

void mostrar_todos_autos_disponibles()
{
    // Abrimos el MISMO archivo donde escribe el Gerente/Admin
    // Asegurate que en auto.h esté definido ARCHIVO_AUTOS como "autos.bin"
    FILE *file = fopen("autos.bin", "rb");

    if(file == NULL)
    {
        printf("\nNo hay autos disponibles en stock por el momento.\n");
        printf("(El gerente debe cargar autos primero).\n");
        return;
    }

    fseek(file, 0, SEEK_END); /// el indice se posiciona al final

    int total = ftell(file) / sizeof(Auto); /// como el inidice esta al final del archivo saca el total de datos que hay

    rewind(file); /// Lo uso para que el inidice al principio

    if(total == 0)
    {
        printf("El archivo esta vacio\n");
        fclose(file);
        return;
    }

    printf("\n==========================================================\n");
    printf("               AUTOS DISPONIBLES (Stock Real)             \n");
    printf("==========================================================\n");
    printf("PATENTE      | MARCA        | MODELO       | PRECIO       \n");
    printf("----------------------------------------------------------\n");

    mostrar_auto_recursivo(file, 0, total);

    printf("==========================================================\n");
    printf("NOTA: Copie la PATENTE exacta para proceder a la compra.\n");

    fclose(file);
}
