#include <stdio.h>
#include <stdlib.h>
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
        printf("No se pudo abrir\n");
        return;
    }

    AutoCliente nuevo_auto = cargar_auto_cliente();
    nuevo_auto.titular = cargar_persona();

    ///Guardo el cliente en clientes.bin
    guardar_cliente_en_archivo(nuevo_auto.titular);


    fwrite(&nuevo_auto, sizeof(AutoCliente), 1, file);
    fclose(file);

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
        printf("Error al abrir\n");
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
    /// Falta...
}
/// FUNCION 6
int cargar_autos_cliente_din(AutoCliente **listaAutos)
{
    FILE *file = fopen(ARCHIVO_AUTOS_CLIENTE, "rb");
    if (file == NULL)
    {
        printf("No se pudo abrir.\n");
        return 0;
    }

    AutoCliente temp;
    int cantidad = 0;

    // Primera pasada: contamos cuántos autos hay
    while (fread(&temp, sizeof(AutoCliente), 1, file) == 1)
    {
        cantidad++;
    }

    rewind(file); // este era para volver al principio

    // Reservamos memoria dinámica (puntero doble)
    *listaAutos = (AutoCliente *) malloc (cantidad * sizeof(AutoCliente));

    if (*listaAutos == NULL)
    {
        printf("Error al asignar memoria.\n");
        fclose(file);
        return 0;
    }

    // Segunda pasada: copiamos los datos
    for (int i = 0; i < cantidad; i++)
    {
        fread(&(*listaAutos)[i], sizeof(AutoCliente), 1, file); /// aca lo que hago es copiar cada auto en el arreglos Dinamicos
    }

    fclose(file);
    return cantidad; // devolvemos cuántos autos cargamos
}
