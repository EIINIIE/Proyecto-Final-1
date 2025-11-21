#include <stdio.h>
#include <stdlib.h>
#include "autos_disponibles.h"
#include "auto.h"



#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void modificar_patente(Auto* a)
{
    char aux[50];
    int valida = 0;

    printf("\n--- MODIFICAR PATENTE ---\n");
    printf("Formato requerido: AA 123 CD (con espacios)\n");

    do
    {
        printf("\nIngrese la nueva patente (USAR MAYUSCULAS): ");
        fflush(stdin);
        gets(aux);

        // Validacion simple de formato (Fuerza Bruta)
        if(strlen(aux) == 9 &&
           aux[0] >= 'A' && aux[0] <= 'Z' &&
           aux[1] >= 'A' && aux[1] <= 'Z' &&
           aux[2] == ' ' &&
           aux[3] >= '0' && aux[3] <= '9' &&
           aux[4] >= '0' && aux[4] <= '9' &&
           aux[5] >= '0' && aux[5] <= '9' &&
           aux[6] == ' ' &&
           aux[7] >= 'A' && aux[7] <= 'Z' &&
           aux[8] >= 'A' && aux[8] <= 'Z')
        {
            strcpy(a->patente, aux);
            valida = 1;
            printf(">> Patente modificada correctamente: %s\n", a->patente);
        }
        else
        {
            printf("ERROR: Formato incorrecto. (Ej: AA 123 CD)\n");
        }

    } while(valida == 0);
}

void mostrar_un_auto(Auto a)
{
    printf("----------------------------------------\n");
    printf(" Patente:      %s\n", a.patente);
    printf(" Marca:        %s\n", a.marca);
    printf(" Modelo:       %s\n", a.modelo);
    printf(" Anio:         %d\n", a.anio);
    printf(" Kilometraje:  %d kms\n", a.kms);
    printf(" Precio Final: $%.2f\n", a.precioFinal);
    printf("----------------------------------------\n\n");
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
      if(strcmpi(a.titular.rol, "concesionaria") == 0)
        {
             mostrar_un_auto(a);
        }
    }

    mostrar_auto_recursivo(file,pos + 1, total);
}

void mostrar_todos_autos_disponibles()
{

    FILE *file = fopen("autos.bin", "rb");

    if(file == NULL)
    {
        printf("\nNo hay autos disponibles en stock por el momento.\n");
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

    mostrar_auto_recursivo(file, 0, total);

    printf("==========================================================\n");

    fclose(file);
}
