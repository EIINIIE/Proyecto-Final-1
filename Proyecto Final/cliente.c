#include <stdio.h>
#include <string.h>
#include "cliente.h"

#define ARCHIVO_CLIENTES "clientes.bin"

Cliente cargar_persona()
{
    Cliente c;
    int validos = 0;

    while (validos == 0)
    {
        printf("Ingrese DNI (solo numeros): ");
        scanf("%s", c.dni);

        validos = 1; // suponemos OK

        // Revisar caracter por caracter
        for (int i = 0; c.dni[i] != '\0'; i++)
        {
            if (c.dni[i] < '0' || c.dni[i] > '9') /// '0' y '9' son caracteres ASCII
            {
                validos = 0;
                printf("El DNI solo puede contener numeros.\n");
                break;
            }
        }

        if (validos == 1 && strlen(c.dni) < 7 || strlen(c.dni) > 8)
        {
            validos = 0;
            printf("El DNI debe tener 7 o 8 digitos.\n");
        }
    }

    printf("Ingrese Nombre: ");
    scanf("%s", c.nombre);

    printf("Ingrese Telefono: ");
    scanf("%s", c.telefono);

    printf("Ingrese Direccion: ");
    scanf("%s", c.direccion);

    strcpy(c.rol, "cliente"); /// ya le asignamos el rol de cliente

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
        printf("DNI: %s | Nombre: %s | Rol: %s\n", c.dni, c.nombre, c.rol);
    }
    fclose(file);
}

void modificar_cliente()
{
    FILE *arch = fopen(ARCHIVO_CLIENTES, "r+b");
    if (arch == NULL)
    {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    char dniBuscado[20];
    printf("Ingrese DNI del cliente a modificar: ");
    scanf("%s", dniBuscado);

    Cliente c;
    int encontrado = 0;

    while (fread(&c, sizeof(Cliente), 1, arch) == 1)
    {
        if (strcmp(c.dni, dniBuscado) == 0)
        {
            encontrado = 1;

            printf("\nCLIENTE ENCONTRADO\n");
            /// mostrar cliente

            int opcion;
            printf("\nQue desea modificar?\n");
            printf("1 - DNI\n");
            printf("2 - Nombre\n");
            printf("3 - Telefono\n");
            printf("4 - Direccion\n");
            printf("Opcion: ");
            scanf("%d", &opcion);

            switch (opcion)
            {
            case 1:
            {
                int valido = 0;

                while (valido == 0)
                {
                    printf("Nuevo DNI: ");
                    fflush(stdin);
                    scanf("%s", c.dni);

                    valido = 1;

                    // Validar solo números
                    for (int i = 0; c.dni[i] != '\0'; i++)
                    {
                        if (c.dni[i] < '0' || c.dni[i] > '9') /// '0' y '9' son caracteres ASCII
                        {
                            valido = 0;
                            printf("El DNI solo puede contener numeros.\n");
                            break;
                        }
                    }

                    // Validar largo
                    int largo = strlen(c.dni); /// esto lo que hace es que largo va a guardar cuantos numeros tiene el DNI que escribio el usuario

                    if (valido == 1 && (largo < 7 || largo > 8))
                    {
                        valido = 0;
                        printf("El DNI debe tener 7 o 8 digitos.\n");
                    }
                }

                printf("DNI cargado exitosamente.\n");
                break;
            }

            case 2:
                printf("Nuevo nombre: ");
                scanf("%s", c.nombre);
                break;

            case 3:
            {
                int telValido = 0;

                while (telValido == 0)
                {
                    printf("Nuevo telefono (solo numeros): ");
                    scanf("%s", c.telefono);

                    telValido = 1; // asumimos correcto

                    // Validar que sean solo números
                    for (int i = 0; c.telefono[i] != '\0'; i++)
                    {
                        if (c.telefono[i] < '0' || c.telefono[i] > '9') /// '0' y '9' son caracteres ASCII
                        {
                            telValido = 0;
                            printf("El telefono solo puede contener numeros.\n");
                            break;
                        }
                    }

                    if (telValido == 1 && strlen(c.telefono) < 6) /// aca hago que el strlen verifique que sea al menos 6 diguitos
                    {
                        telValido = 0;
                        printf("El telefono debe tener al menos 6 digitos\n");
                    }
                }

                printf("Telefono modificado exitosamente.\n");

                break;
            }

            case 4:
                printf("Nueva direccion: ");
                scanf("%s", c.direccion);
                break;

            default:
                printf("Opcion invalida.\n");
            }

            // Mover puntero atrás para sobrescribir
            fseek(arch, -sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, arch);

            printf("\nCliente modificado con exito.\n");
            break;
        }
    }

    if (encontrado == 0)
    {
        printf("No se encontro el cliente con ese DNI\n");
    }

    fclose(arch);
}
