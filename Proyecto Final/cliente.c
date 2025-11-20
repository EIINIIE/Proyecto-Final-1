#include <stdio.h>
#include <string.h>
#include "cliente.h"

#define ARCHIVO_CLIENTES "clientes.bin"


Cliente cargar_persona()
{
    Cliente c;

    printf("Ingrese DNI: ");
    fflush (stdin);
    scanf("%s", c.dni);

    printf("Ingrese Nombre: ");
    fflush (stdin);
    scanf("%s", c.nombre);

    printf("Ingrese Telefono: ");
    fflush (stdin);
    scanf("%s", c.telefono);

    printf("Ingrese Direccion: ");
    fflush (stdin);
    scanf("%s", c.direccion);

    printf("Ingrese Rol: ");
    fflush (stdin);
    scanf("%s", c.rol);

    return c;
}


void guardar_cliente_en_archivo(Cliente c)
{
    FILE *file = fopen(ARCHIVO_CLIENTES, "ab");
    if(file)
    {
        fwrite(&c, sizeof(Cliente), 1, file);
        fclose(file);
        printf("\nCliente guardado correctamente.\n");
        printf("DNI guardado: [%s]\n", c.dni); /// verifico
    }
    else
        printf("Error al abrir archivo.\n");
}


void ver_listado_clientes()
{
    FILE *file = fopen(ARCHIVO_CLIENTES, "rb");
    if(file == NULL)
    {
        printf("No hay clientes registrados.\n");
        return;
    }

    Cliente c;
    printf("\n--- LISTADO DE CLIENTES ---\n");
    while(fread(&c, sizeof(Cliente), 1, file) == 1)
    {
        printf("DNI: %s | Nombre: %s | Rol: %s\n", c.dni, c.nombre, c.rol);
    }
    fclose(file);
}


void modificar_cliente()
{
    char dniBuscado[50];
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
            printf("\nCliente encontrado:\n");
            printf("Nombre: %s\nTelefono: %s\nDireccion: %s\nRol: %s\n",
                   c.nombre, c.telefono, c.direccion, c.rol);

            printf("\nNuevo Nombre: ");
            scanf("%s", c.nombre);

            printf("Nuevo Telefono: ");
            scanf("%s", c.telefono);

            printf("Nueva Direccion: ");
            scanf("%s", c.direccion);

            printf("Nuevo Rol: ");
            scanf("%s", c.rol);

            fseek(f, -sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, f);

            printf("\nCliente modificado.\n");
            break;
        }
    }

    if(encontrado == 0)
    {
        printf("No se encontro un cliente con ese DNI.\n");
    }

    fclose(f);
}
