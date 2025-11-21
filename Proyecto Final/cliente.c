#include <stdio.h>
#include <string.h>
#include "cliente.h"

#define ARCHIVO_CLIENTES "clientes.bin"

Cliente cargar_persona()
{
    Cliente c;

    printf("\n---- DATOS DE LA PERSONA ----\n");

    printf("Ingrese DNI: ");
    fflush (stdin);
    gets(c.dni); // Usamos gets para texto seguro

    printf("Ingrese Nombre y Apellido: ");
    fflush (stdin);
    gets(c.nombre); // Permite espacios

    printf("Ingrese Telefono: ");
    fflush (stdin);
    gets(c.telefono);

    printf("Ingrese Direccion: ");
    fflush (stdin);
    gets(c.direccion); // Permite espacios (ej: San Martin 200)

    printf("Ingrese Rol (cliente/admin): ");
    fflush (stdin);
    gets(c.rol);

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
    }
    else
    {
        printf("Error al abrir.\n");
    }
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
        printf("DNI: %-10s | Nombre: %-20s | Rol: %s\n", c.dni, c.nombre, c.rol);
    }
    fclose(file);
}


void modificar_cliente()
{
    char dniBuscado[50];
    printf("Ingrese DNI del cliente a modificar: ");
    fflush(stdin);
    gets(dniBuscado);

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
            printf("\nCliente encontrado: %s\n", c.nombre);

            printf("Nuevo Nombre: ");
            fflush(stdin); gets(c.nombre);

            printf("Nuevo Telefono: ");
            fflush(stdin); gets(c.telefono);

            printf("Nueva Direccion: ");
            fflush(stdin); gets(c.direccion);

            // El rol generalmente no se cambia aqui, pero lo dejamos si quieres
            // printf("Nuevo Rol: "); fflush(stdin); gets(c.rol);

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
