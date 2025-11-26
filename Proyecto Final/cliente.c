#include <stdio.h>
#include <string.h>
#include "cliente.h"
#include "usuario.h"

#define ARCHIVO_CLIENTES "clientes.bin"

Cliente obtener_datos_cliente(char dni[])
{
    Cliente c;
    strcpy(c.dni, "0");
    strcpy(c.nombre, "Desconocido");

    FILE *file = fopen(ARCHIVO_CLIENTES, "rb");
    if(file == NULL)
    {
        return c;
    }

    Cliente aux;
    while(fread(&aux, sizeof(Cliente), 1, file))
    {
        if(strcmp(aux.dni, dni) == 0)
        {
            fclose(file);
            return aux;
        }
    }
    fclose(file);
    return c;
}

// --- FUNCION MODIFICADA Y ROBUSTA ---
Cliente cargar_persona(char dniExterno[])
{
    Cliente c;
    int valido = 0;
    int i;
    char aux[100];

    printf("\n---- DATOS DE CONTACTO DEL CLIENTE ----\n");

    // 1. LOGICA DEL DNI
    if (strcmp(dniExterno, "-1") == 0)
    {
        do
        {
            valido = 1;
            printf("Ingrese DNI (solo numeros, 7-8 digitos): ");
            fflush(stdin);
            scanf("%s", aux);

            int len = strlen(aux);
            if (len < 7 || len > 8)
            {
                printf("El DNI debe tener entre 7 y 8 digitos.\n");
                valido = 0;
            }

            if (valido == 1)
            {
                for(i = 0; i < len; i++)
                {
                    if(aux[i] < '0' || aux[i] > '9')
                    {
                        printf("El DNI solo debe contener numeros.\n");
                        valido = 0;
                        break;
                    }
                }
            }

            if (valido == 1)
            {
                if (dni_Existente_cliente(aux) || dni_Existente_usuario(aux))
                {
                    printf("ERROR: Ya existe un registro con ese DNI.\n");
                    valido = 0;
                }
                else
                {
                    strcpy(c.dni, aux);
                }
            }
        }
        while(valido == 0);
    }
    else
    {
        strcpy(c.dni, dniExterno);
        printf("DNI Asignado: %s\n", c.dni);
    }

    // --- CARGA NOMBRE ---
    do
    {
        valido = 1;
        printf("Ingrese Nombre y Apellido: ");
        fflush(stdin);
        scanf(" %[^\n]", aux);

        if (strlen(aux) < 4)
        {
            printf("Nombre y Apellido deben tener al menos 4 caracteres.\n");
            valido = 0;
        }
        else
        {
            strncpy(c.nombre, aux, 49);
            c.nombre[49] = '\0';
        }
    }
    while(valido == 0);

    // --- CARGA TELEFONO ---
    do
    {
        valido = 1;
        printf("Ingrese Telefono (solo numeros, 10 digitos): ");
        fflush(stdin);
        scanf("%s", c.telefono);

        // Validacion estricta de longitud
        if(strlen(c.telefono) != 10)
        {
            // Usamos cast a int para evitar el tipo size_t
            printf("Longitud invalida: Ingreso %d digitos (Debe ser de 10).\n", (int)strlen(c.telefono));
            valido = 0;
        }
        else
        {
            for(i = 0; i < 10; i++)
            {
                if(c.telefono[i] < '0' || c.telefono[i] > '9')
                {
                    valido = 0;
                    printf("El telefono solo debe contener numeros.\n");
                    break;
                }
            }
            if(valido == 1)
            {
                if (telefono_Existente(c.telefono) == 1)
                {
                    printf("Ya existe un cliente con este telefono.\n");
                    valido = 0;
                }
            }
        }
    }
    while(valido == 0);

    // --- CARGA DIRECCION ---
    char direccion1[100];
    int direccionInt = 0;

    do
    {
        printf("Ingrese Direccion (calle y numero, min 4 letras y 3 numeros): ");
        fflush(stdin);
        scanf(" %[^\n]", direccion1);

        int letras = 0;
        int numeros = 0;

        for(int i = 0; direccion1[i] != '\0'; i++)
        {
            if( (direccion1[i] >= 'A' && direccion1[i] <= 'Z') ||
                (direccion1[i] >= 'a' && direccion1[i] <= 'z') )
            {
                letras++;
            }

            if(direccion1[i] >= '0' && direccion1[i] <= '9')
            {
                numeros++;
            }
        }

        if(letras >= 4 && numeros >= 3)
        {
            direccionInt = 1;
            strcpy(c.direccion, direccion1);
        }
        else
        {
            printf("Direccion invalida: debe tener al menos 4 letras y minimo 3 o mas numeros.\n");
        }
    }
    while(direccionInt == 0);

    strcpy(c.rol, "cliente");
    printf("Rol asignado automaticamente: CLIENTE\n");

    return c;
}

int telefono_Existente (char telefono[])
{
    FILE *file = fopen(ARCHIVO_CLIENTES, "rb");
    if(file == NULL) return 0;

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
    if (file == NULL) return 0;

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
        printf("Error al guardar cliente.\n");
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
    printf("\n======================================================================================\n");
    printf("%-12s %-25s %-15s %-10s\n", "DNI", "NOMBRE", "TELEFONO", "ROL");
    printf("======================================================================================\n");

    while(fread(&c, sizeof(Cliente), 1, file) == 1)
    {
        printf("%-12s %-25s %-15s %-10s\n", c.dni, c.nombre, c.telefono, c.rol);
    }
    printf("======================================================================================\n");
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
    fflush(stdin);
    scanf("%s", dniBuscado);

    Cliente c;
    int encontrado = 0;

    while (fread(&c, sizeof(Cliente), 1, arch) == 1)
    {
        if (strcmp(c.dni, dniBuscado) == 0)
        {
            encontrado = 1;
            printf("\nCLIENTE ENCONTRADO: %s\n", c.nombre);

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
                    for (int i = 0; i < strlen(c.dni); i++)
                    {
                        if (c.dni[i] < '0' || c.dni[i] > '9')
                        {
                            valido = 0;
                            printf("El DNI solo puede contener numeros.\n");
                            break;
                        }
                    }
                    int largo = strlen(c.dni);
                    if (valido == 1 && (largo < 7 || largo > 8))
                    {
                        valido = 0;
                        printf("El DNI debe tener 7 o 8 digitos.\n");
                    }
                }
                printf("DNI modificado exitosamente.\n");
                break;
            }
            case 2:
            {
                int nombreValido = 0;
                do
                {
                    printf("Nuevo nombre: ");
                    fflush(stdin);
                    scanf(" %[^\n]", c.nombre);
                    nombreValido = 1;
                    if(strlen(c.nombre) == 0) nombreValido = 0;
                }
                while(nombreValido == 0);
                break;
            }
            case 3:
            {
                int valido = 0;
                do
                {
                    printf("Nuevo Telefono (10 digitos): ");
                    fflush(stdin);
                    scanf("%s", c.telefono);
                    valido = 1;
                    for(int i=0; i<strlen(c.telefono); i++)
                    {
                        if(c.telefono[i] < '0' || c.telefono[i] > '9') valido = 0;
                    }
                    if(strlen(c.telefono) != 10) valido = 0;
                }
                while(valido==0);
                break;
            }
            case 4:
            {
                printf("Ingrese nueva direccion: ");
                fflush(stdin);
                scanf(" %[^\n]", c.direccion);
                break;
            }
            default:
                printf("Opcion invalida.\n");
            }

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
