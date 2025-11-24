#include <stdio.h>
#include <stdlib.h>
#include <string.h> // <--- FALTA ESTA LIBRERIA
#include "auto_cliente.h"
#include "cliente.h"

/// FUNCION 1
AutoCliente cargar_auto_cliente()
{
    AutoCliente autos; /// Usamos la nueva estructura

    printf("----DATOS DEL AUTO DEL CLIENTE ----\n");
    printf("Patente: ");
    scanf("%s", autos.patente);
    printf("Marca: ");
    scanf("%s", autos.marca);
    printf("Modelo: ");
    scanf("%s", autos.modelo);
    printf("Año: ");
    scanf("%d", &autos.anio);
    printf("Kilometros: ");
    scanf("%d", &autos.kms);
    printf("Precio: ");
    scanf("%f", &autos.precioDeAdquisicion);

    /// Logica de precio simple por ahora para evitar conflictos circulares
    autos.precioFinal = autos.precioDeAdquisicion;

    printf("Precio original: $%.2f\n", autos.precioDeAdquisicion);
    printf("Precio final: $%.2f\n", autos.precioFinal);

    return autos;
}

/// FUNCION 2
void agregar_autos_cliente()
{
    FILE* file = fopen(ARCHIVO_AUTOS_CLIENTE, "ab");
    if(file == NULL)
    {
        printf("No se pudo abrir el archivo de autos cliente\n");
        return;
    }

    AutoCliente nuevo_auto = cargar_auto_cliente();
    nuevo_auto.titular = cargar_persona();

    ///Guardo el cliente en clientes.bin
    guardar_cliente_en_archivo(nuevo_auto.titular);

    fwrite(&nuevo_auto, sizeof(AutoCliente), 1, file);
    fclose(file);
    printf("Auto y titular cargados correctamente en autos_cliente.bin\n");
}

/// FUNCION 3
void mostrar_auto_cliente(AutoCliente a)
{
    printf("---- DATOS DEL VEHICULO (CLIENTE) ----\n");
    printf("Patente: %s\n", a.patente);
    printf("Marca: %s\n", a.marca);
    printf("Modelo: %s\n", a.modelo);
    printf("Año: %d\n", a.anio);
    printf("Kilometraje: %d\n", a.kms);
    printf("Precio: $%.2f\n", a.precioFinal);

    printf("---- DATOS DEL TITULAR ----\n");
    printf("Nombre: %s\n", a.titular.nombre);
    printf("DNI: %s\n", a.titular.dni);
    printf("---------------------------\n");
}

/// FUNCION 4
void mostrar_todos_autos_cliente()
{
    FILE* file = fopen(ARCHIVO_AUTOS_CLIENTE, "rb");
    if(file == NULL)
    {
        printf("Error al abrir el archivo de autos cliente\n");
        return;
    }

    AutoCliente a;
    while(fread(&a, sizeof(AutoCliente), 1, file) == 1)
    {
        mostrar_auto_cliente(a);
        printf("\n");
    }
    fclose(file);
}

/// FUNCION 5
void modificar_auto_cliente_por_dni(char dniBuscado[])
{
    FILE *file = fopen(ARCHIVO_AUTOS_CLIENTE, "r+b");
    if (file == NULL)
    {
        printf("No se pudo abrir.\n");
        return;
    }

    AutoCliente aux;
    int encontrado = 0;
    while (fread(&aux, sizeof(AutoCliente), 1, file) == 1)
    {
        if (strcmp(aux.titular.dni, dniBuscado) == 0)
        {
            encontrado = 1;
            printf("\nAUTO ENCONTRADO\n");
            mostrar_auto_cliente(aux);

            printf("\n--- MODIFICACIÓN DE DATOS DEL AUTO ---\n");

            printf("Nueva marca: ");
            scanf("%s", aux.marca);

            printf("Nuevo modelo: ");
            scanf("%s", aux.modelo);

            printf("Nuevo anio: ");
            scanf("%d", &aux.anio);

            printf("Nuevo kilometro: ");
            scanf("%d", &aux.kms);

            printf("Nuevo precio original: ");
            scanf("%f", &aux.precioDeAdquisicion);

            aux.precioFinal = aux.precioDeAdquisicion;

            fseek(file, -sizeof(AutoCliente), SEEK_CUR);
            fwrite(&aux, sizeof(AutoCliente), 1, file);

            printf("\nAuto modificado correctamente.\n");
            break;
        }
    }

    if (encontrado == 0)
    {
        printf("No se encontro este DNI: %s.\n", dniBuscado);
    }

    fclose(file);
}

/// FUNCION 6
int cargar_autos_cliente_din(AutoCliente **listaAutos)
{
    FILE *file = fopen(ARCHIVO_AUTOS_CLIENTE, "rb");
    if (file == NULL)
    {
        printf("No se pudo abrir el archivo.\n");
        return 0;
    }

    AutoCliente temp;
    int cantidad = 0;


    while (fread(&temp, sizeof(AutoCliente), 1, file) == 1)
    {
        cantidad++;
    }

    rewind(file);

    *listaAutos = (AutoCliente *) malloc (cantidad * sizeof(AutoCliente));

    if (*listaAutos == NULL)
    {
        printf("Error al asignar memoria.\n");
        fclose(file);
        return 0;
    }

    for (int i = 0; i < cantidad; i++)
    {
        fread(&(*listaAutos)[i], sizeof(AutoCliente), 1, file); /// aca lo que hago es copiar cada auto en el arreglos Dinamicos
    }

    fclose(file);
    return cantidad;
}
