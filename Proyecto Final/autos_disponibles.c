#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "autos_disponibles.h"
#include "auto.h"

void mostrar_un_auto(Auto a)
{
    printf("--------------------------\n");
    printf("Patente: %s\n", a.patente);
    printf("Marca: %s\n", a.marca);
    printf("Modelo: %s\n", a.modelo);
    printf("Kms: %d\n", a.kms);
    printf("Precio Venta: $%.2f\n", a.precioFinal);
    printf("--------------------------\n");
}

void mostrar_auto_recursivo( FILE* file,  int pos, int total)
{
    if(pos >= total) return;

    fseek(file, pos * sizeof(Auto), SEEK_SET);
    Auto a;

    if(fread(&a, sizeof(Auto), 1, file) == 1)
    {
        // FILTRO CRITICO: Solo mostrar si el titular es la Empresa
        // Esto distingue "Stock Disponible" de "Historial Completo"
        if(strcmp(a.titular.rol, "Empresa") == 0 || strcmp(a.titular.rol, "concesionaria") == 0)
        {
            mostrar_un_auto(a);
        }
    }

    mostrar_auto_recursivo(file, pos + 1, total);
}

void mostrar_todos_autos_disponibles()
{
    FILE *file = fopen("autos.bin", "rb");

    if(file == NULL)
    {
        printf("\nNo hay autos registrados en el sistema.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    int total = ftell(file) / sizeof(Auto);
    rewind(file);

    if(total == 0)
    {
        printf("El archivo de autos esta vacio.\n");
        fclose(file);
        return;
    }

    printf("\n==========================================================\n");
    printf("      STOCK DISPONIBLE (A la venta en Concesionaria)      \n");
    printf("==========================================================\n");

    mostrar_auto_recursivo(file, 0, total);

    printf("\n(Fin del listado disponible)\n");
    fclose(file);
}
