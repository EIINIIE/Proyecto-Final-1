#include <stdio.h>
#include <string.h>
#include "cliente.h"

#define ARCHIVO_CLIENTES "clientes.bin"

Cliente cargar_persona()
{
    Cliente c;
    int valido = 0; // CORREGIDO: Usamos 'valido' (sin s) en todo el codigo
    int i;

    printf("\n---- DATOS DE LA PERSONA ----\n");

    // 1. DNI
    do
    {
        printf("Ingrese DNI (solo numeros): ");
        fflush(stdin);
        scanf("%s", c.dni);

        valido = 1; // Asumimos valido

        // Validar que sean numeros
        for(i = 0; i < strlen(c.dni); i++)
        {
            if(c.dni[i] < '0' || c.dni[i] > '9')
            {
                valido = 0;
                break;
            }
        }

        if(valido == 0)
        {
            printf("Error: El DNI solo puede contener numeros.\n");
        }
        else if (strlen(c.dni) < 7 || strlen(c.dni) > 8)
        {
            valido = 0;
            printf("Error: El DNI debe tener 7 o 8 digitos.\n");
        }
        else if (dni_Existente_cliente(c.dni))
        {
            valido = 0;
            printf("Error: Ese DNI ya esta registrado.\n");
        }

    }
    while(valido == 0);

    // 2. NOMBRE
    printf("Ingrese Nombre : ");
    fflush(stdin);
    scanf("%s",c.nombre);

    // 3. TELEFONO
    int telValido = 0;
    do
    {
        printf("Ingrese Telefono (10 digitos): ");
        fflush(stdin);
        scanf("%s", c.telefono);

        telValido = 1; // Asumimos valido

        // Validar numeros
        for(i = 0; i < strlen(c.telefono); i++)
        {
            if(c.telefono[i] < '0' || c.telefono[i] > '9')
            {
                telValido = 0;
                break;
            }
        }

        if(telValido == 0)
        {
            printf("Error: El telefono solo puede contener numeros.\n");
        }
        else if (strlen(c.telefono) != 10)
        {
            telValido = 0;
            printf("Error: El telefono debe tener 10 digitos.\n");
        }
        else if (telefono_Existente(c.telefono))
        {
            telValido = 0;
            printf("Error: Ese telefono ya existe.\n");
        }

    }
    while(telValido == 0);

    // 4. DIRECCION
    printf("Ingrese Direccion: ");
    fflush(stdin);
    scanf("%s", c.direccion);

    // 5. ROL
    strcpy(c.rol, "cliente");
    printf("Rol asignado automaticamente: cliente\n");

    return c;
}
int telefono_Existente (char telefono[])
{
    FILE *file = fopen(ARCHIVO_CLIENTES, "rb");

    if(file == NULL)
    {
        return 0;
    }

    Cliente aux;

    while (fread(&aux, sizeof (Cliente), 1, file))
    {
        if (strcmp (aux.telefono, telefono) == 0)
        {
            fclose (file);
            return 1;
        }
    }
    fclose (file);
    return 0;
}

int cliente_existente(char dniBuscado[])
{
    FILE *file = fopen(ARCHIVO_CLIENTES, "rb");
    if (file == NULL)
    {
        return 0;
    }

    Cliente c;

    while (fread(&c, sizeof(Cliente), 1, file) > 0)
    {
        if (strcmp(c.dni, dniBuscado) == 0)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void guardar_cliente_en_archivo(Cliente c)
{
    FILE *file = fopen(ARCHIVO_CLIENTES, "ab");
    if(file != NULL)
    {
        fwrite(&c, sizeof(Cliente), 1, file);
        fclose(file);
        printf("\nCliente guardado correctamente.\n");
    }
    else
    {
        printf("DNI guardado: [%s]\n", c.dni); /// verifico

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
        printf("DNI: %s | Nombre: %s | Tel: %s | Rol: %s\n", c.dni, c.nombre,c.telefono,c.rol);
    }
    fclose(file);
}



int dni_Existente_cliente(char dni[])
{
    FILE *file = fopen(ARCHIVO_CLIENTES, "rb");
    if (file == NULL) return 0;

    Cliente aux;
    while (fread(&aux, sizeof(Cliente), 1, file))
    {
        if (strcmp(aux.dni, dni) == 0)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
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
    int i;

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

                    // Validar solo n�meros
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
                {
                    int valido = 0;
                    do
                    {
                        printf("Nuevo Telefono (10 digitos): ");
                        scanf("%s", c.telefono);

                        valido = 1;
                        // Validacion manual
                        for(i=0; i<strlen(c.telefono); i++)
                        {
                            if(c.telefono[i] < '0' || c.telefono[i] > '9') valido = 0;
                        }

                        if(valido == 1 && strlen(c.telefono) == 10)
                        {
                            // OK
                        }
                        else
                        {
                            valido = 0;
                            printf("Error: Solo numeros, 10 digitos.\n");
                        }
                    }
                    while(valido==0);
                }
                break;
            }

            case 4:
                printf("Nueva direccion: ");
                scanf("%s", c.direccion);
                break;

            default:
                printf("Opcion invalida.\n");
            }

            // Mover puntero atr�s para sobrescribir
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
