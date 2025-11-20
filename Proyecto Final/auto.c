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
    scanf("%s", a.marca);
    printf("Modelo: ");
    scanf("%s", a.modelo);
    printf("Anio: ");
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

    // Calculo simple de precio final (ej: +20% ganancia)
    a.precioFinal = a.precioDeAdquisicion * 1.20;

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
        printf("Auto agregado al stock exitosamente.\n");
    }
    else
    {
        printf("Error al abrir archivo.\n");
    }
}

// --- NUEVO: Requisito 3a (Modificar Auto) ---
void modificar_auto_stock()
{
    char patenteBuscada[20];
    printf("\nIngrese la patente del auto a modificar: ");
    scanf("%s", patenteBuscada);

    FILE *f = fopen(ARCHIVO_AUTOS, "r+b");
    if(f == NULL)
    {
        printf("Error: No se puede acceder al archivo de autos.\n");
        return;
    }

    Auto a;
    int encontrado = 0;

    while(fread(&a, sizeof(Auto), 1, f) == 1)
    {
        if(strcmp(a.patente, patenteBuscada) == 0)
        {
            encontrado = 1;
            mostrar_auto(a);

            printf("\n--- MODIFICAR DATOS ---\n");
            printf("1. Precio de Adquisicion\n");
            printf("2. Kilometraje\n");
            printf("3. Cancelar\n");
            printf("Opcion: ");
            int op;
            scanf("%d", &op);

            if(op == 1)
            {
                printf("Nuevo Precio: ");
                scanf("%f", &a.precioDeAdquisicion);
                // Recalculamos precio final
                a.precioFinal = a.precioDeAdquisicion * 1.20;
            }
            else if(op == 2)
            {
                printf("Nuevo Kilometraje: ");
                scanf("%d", &a.kms);
            }
            else
            {
                printf("Modificacion cancelada.\n");
                fclose(f);
                return;
            }

            // Volvemos atras el puntero para sobrescribir
            fseek(f, -sizeof(Auto), SEEK_CUR);
            fwrite(&a, sizeof(Auto), 1, f);
            printf("Auto modificado correctamente.\n");
            break;
        }
    }

    if(!encontrado)
    {
        printf("No se encontro un auto con la patente %s en el stock.\n", patenteBuscada);
    }

    fclose(f);
}

// Mostrar un auto
void mostrar_auto(Auto a)
{
    printf("\n------------------------------\n");
    printf("Patente: %s\n", a.patente);
    printf("Marca:   %s\n", a.marca);
    printf("Modelo:  %s\n", a.modelo);
    printf("Anio:    %d\n", a.anio);
    printf("Kms:     %d\n", a.kms);
    printf("Costo:   $%.2f\n", a.precioDeAdquisicion);
    printf("Venta:   $%.2f\n", a.precioFinal);
    printf("Titular: %s\n", a.titular.nombre);
    printf("------------------------------\n");
}

// Mostrar todos los autos
void mostrar_todos_autos(char archivo[])
{
    FILE *f = fopen(archivo, "rb");
    if(f == NULL)
    {
        printf("No hay autos registrados.\n");
        return;
    }

    Auto a;
    printf("\n--- LISTADO COMPLETO DE AUTOS (Stock) ---\n");
    while(fread(&a, sizeof(Auto), 1, f) == 1)
    {
        mostrar_auto(a);
    }
    fclose(f);
}
