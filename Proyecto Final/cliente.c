#include <stdio.h>
#include <string.h>
#include "cliente.h"

#define ARCHIVO_CLIENTES "clientes.bin"

// --- Cargar un nuevo cliente de forma simple ---
Cliente cargar_persona()
{
    Cliente c;

    // DNI: Usamos scanf normal porque no lleva espacios
    printf("Ingrese DNI: ");
    fflush(stdin);
    scanf("%s", c.dni);

    // NOMBRE: Usamos " %[^\n]" para permitir espacios (ej: Juan Perez)
    printf("Ingrese Nombre: ");
    fflush(stdin);
    scanf(" %[^\n]", c.nombre);

    // TELEFONO: Sin espacios
    printf("Ingrese Telefono: ");
    fflush(stdin);
    scanf("%s", c.telefono);

    // DIRECCION: Con espacios (ej: Calle Falsa 123)
    printf("Ingrese Direccion: ");
    fflush(stdin);
    scanf(" %[^\n]", c.direccion);

    // ROL: Sin espacios (o con, depende de tu preferencia, mejor simple por ahora)
    printf("Ingrese Rol: ");
    fflush(stdin);
    scanf("%s", c.rol);

    return c;
}

// --- Guardar cliente en archivo ---
void guardar_cliente_en_archivo(Cliente c)
{
    FILE *file = fopen(ARCHIVO_CLIENTES, "ab"); // 'ab' para agregar al final
    if(file)
    {
        fwrite(&c, sizeof(Cliente), 1, file);
        fclose(file);
        printf("\nCliente guardado correctamente.\n");
        // Debug: Verificamos qué se acaba de guardar
        printf("--> Registro creado: DNI [%s] - Nombre [%s]\n", c.dni, c.nombre);
    }
    else
        printf("Error al abrir archivo para guardar.\n");
}

// --- Ver listado de clientes ---
void ver_listado_clientes()
{
    FILE *file = fopen(ARCHIVO_CLIENTES, "rb");
    if(file == NULL)
    {
        printf("No hay clientes registrados (Archivo no existe).\n");
        return;
    }

    Cliente c;
    printf("\n================ LISTADO DE CLIENTES ================\n");
    while(fread(&c, sizeof(Cliente), 1, file) == 1)
    {
        printf("DNI: %-10s | Nombre: %-20s | Rol: %s\n", c.dni, c.nombre, c.rol);
    }
    printf("=====================================================\n");
    fclose(file);
}

// --- Modificar cliente existente ---
void modificar_cliente()
{
    char dniBuscado[30]; // Agrandado para coincidir con la estructura
    printf("Ingrese DNI del cliente a modificar: ");
    scanf("%s", dniBuscado);

    FILE *f = fopen(ARCHIVO_CLIENTES, "r+b");
    if(f == NULL)
    {
        printf("No hay clientes registrados.\n");
        return;
    }

    Cliente c;
    int encontrado = 0;

    while(fread(&c, sizeof(Cliente), 1, f) == 1)
    {
        if(strcmp(c.dni, dniBuscado) == 0)
        {
            encontrado = 1;
            printf("\n--- CLIENTE ENCONTRADO ---\n");
            printf("Nombre actual: %s\n", c.nombre);
            printf("Telefono actual: %s\n", c.telefono);
            printf("Direccion actual: %s\n", c.direccion);
            printf("Rol actual: %s\n", c.rol);

            printf("\n--- INGRESE NUEVOS DATOS ---\n");

            printf("Nuevo Nombre: ");
            fflush(stdin);
            scanf(" %[^\n]", c.nombre);

            printf("Nuevo Telefono: ");
            fflush(stdin);
            scanf("%s", c.telefono);

            printf("Nueva Direccion: ");
            fflush(stdin);
            scanf(" %[^\n]", c.direccion);

            printf("Nuevo Rol: ");
            fflush(stdin);
            scanf("%s", c.rol);

            // Retrocedemos el cursor el tamaño de UN cliente para sobrescribir
            fseek(f, -sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, f);

            printf("\nCliente modificado correctamente.\n");
            break;
        }
    }

    if(encontrado == 0)
    {
        printf("No se encontro un cliente con el DNI: %s\n", dniBuscado);
    }

    fclose(f);
}
