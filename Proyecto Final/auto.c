#include <stdio.h>
#include <string.h>
#include "auto.h"

#define ARCHIVO_AUTOS "autos.bin"

// Cargar auto en memoria
Auto cargar_auto()
{
    Auto a;
    printf("---- DATOS DEL AUTO ----\n");
    printf("Patente: ");
    scanf("%s", a.patente);
    printf("Marca: ");
    scanf(" %[^\n]", a.marca);
    printf("Modelo: ");
    scanf(" %[^\n]", a.modelo);
    printf("Año: ");
    scanf("%d", &a.anio);
    printf("Kilometraje: ");
    scanf("%d", &a.kms);
    printf("Precio de Adquisicion: ");
    scanf("%f", &a.precioDeAdquisicion);

    // Titular default: concesionaria
    strcpy(a.titular.nombre, "Concesionaria");
    strcpy(a.titular.dni, "00000000");
    strcpy(a.titular.telefono, "000000000");
    strcpy(a.titular.direccion, "S/N");
    strcpy(a.titular.rol, "Empresa");

    return a;
}

// Agregar auto al stock
void agregar_auto_stock()
{
    Auto a = cargar_auto();
    FILE *f = fopen(ARCHIVO_AUTOS, "ab");
    if(f != NULL)
    {
        fwrite(&a, sizeof(Auto), 1, f);
        fclose(f);
        printf("Auto agregado al stock.\n");
    }
    else
    {
        printf("Error al abrir archivo.\n");
    }
}

// Mostrar un auto
void mostrar_auto(Auto a)
{
    printf("\nPatente: %s\nMarca: %s\nModelo: %s\nAnio: %d\nKms: %d\nPrecio: %.2f\n",
           a.patente, a.marca, a.modelo, a.anio, a.kms, a.precioDeAdquisicion);
    printf("Titular: %s\n", a.titular.nombre);
}

// Mostrar todos los autos
void mostrar_todos_autos(char archivo[])
{
    FILE *f = fopen(archivo, "rb");
    if(f == NULL)
    {
        printf("No hay autos.\n");
        return;
    }

    Auto a;
    while(fread(&a, sizeof(Auto), 1, f) == 1)
    {
        mostrar_auto(a);
    }
    fclose(f);
}

// Buscar auto por patente
void mostrarAutoPorPatente()
{
    char pat[11];
    printf("Ingrese patente: ");
    fflush (stdin);
    scanf("%s", pat);

    FILE *f = fopen(ARCHIVO_AUTOS, "rb");
    if(f == NULL)
    {
        printf("No hay autos.\n");
        return;
    }

    Auto a;
    int encontrado = 0;
    while(fread(&a, sizeof(Auto), 1, f) == 1)
    {
        if(strcmp(a.patente, pat) == 0)
        {
            mostrar_auto(a);
            encontrado = 1;
            break;
        }
    }
    fclose(f);
    if(encontrado == 0)
    {
        printf("No se encontro auto con esa patente.\n");
    }
}
