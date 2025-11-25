#include <stdio.h>
#include <string.h>
#include "cliente.h"

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

Cliente cargar_persona()
{
    Cliente c;
    int valido = 0;
    int i;
    int existe = 0;

    printf("\n---- DATOS DE LA PERSONA ----\n");

    do
    {
        printf("Ingrese DNI (solo numeros): ");
        fflush(stdin);
        scanf("%s", c.dni);

        valido = 1;
        existe = 0;

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
        else
        {
            if (dni_Existente_cliente(c.dni))
            {
                existe = 1;
                valido = 1;
            }
        }

    }
    while(valido == 0);

    if (existe == 1)
    {
        c = obtener_datos_cliente(c.dni);
        printf("\nCLIENTE ENCONTRADO EN BASE DE DATOS\n");
        printf("Nombre:    %s\n", c.nombre);
        printf("Telefono:  %s\n", c.telefono);
        printf("Direccion: %s\n", c.direccion);
        printf("-------------------------------------------\n");
    }
    else
    {
        int nombreValido = 0;
        do
        {
            printf("Ingrese Nombre: ");
            fflush(stdin);
            gets(c.nombre);

            nombreValido = 1;

            // Si está vacío, error
            if(strlen(c.nombre) == 0)
            {
                nombreValido = 0;
            }

            // Recorremos letra por letra
            for(int j=0; j<strlen(c.nombre); j++)
            {
                // Verificamos si es letra minúscula, mayúscula o espacio
                int esMinuscula = (c.nombre[j] >= 'a' && c.nombre[j] <= 'z');
                int esMayuscula = (c.nombre[j] >= 'A' && c.nombre[j] <= 'Z');
                int esEspacio   = (c.nombre[j] == ' ');

                // Si NO es ninguna de las tres, es un carácter inválido
                if( !esMinuscula && !esMayuscula && !esEspacio )
                {
                    nombreValido = 0;
                    break;
                }
            }

            if(nombreValido == 0)
            {
                printf("Error: El nombre solo puede contener letras y espacios. Intente nuevamente.\n");
            }
        }
        while(nombreValido == 0);

        // --- CARGA DE TELEFONO ---
        int telValido = 0;
        do
        {
            printf("Ingrese Telefono (10 digitos): ");
            fflush(stdin);
            scanf("%s", c.telefono);

            telValido = 1;

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
                printf("Error: El telefono solo puede contener numeros positivos.\n");
            }
            else if (strlen(c.telefono) != 10)
            {
                telValido = 0;
                printf("Error: El telefono debe tener EXACTAMENTE 10 digitos.\n");
            }
            else if (telefono_Existente(c.telefono))
            {
                telValido = 0;
                printf("Error: Ese telefono ya esta registrado por otro cliente.\n");
            }
        }
        while(telValido == 0);

        // --- CARGA DE DIRECCION ---

        int direccionInt = 0;
        char direccion[100];

        do
        {
            printf("Ingrese direccion: ");
            fflush(stdin);
            gets(direccion);

            int letras = 0;
            int numeros = 0;

            for(int i = 0; direccion[i] != '\0'; i++)
            {
                if( (direccion[i] >= 'A' && direccion[i] <= 'Z') ||
                        (direccion[i] >= 'a' && direccion[i] <= 'z') )
                {
                    letras++;
                }

                if(direccion[i] >= '0' && direccion[i] <= '9')
                {
                    numeros++;
                }
            }

            if(letras >= 4 && numeros >= 3)
            {
                direccionInt = 1;
            }
            else
            {
                printf("Direccion invalida: debe tener al menos 4 letras y minimo 3 o mas numeros.\n");
            }

        }
        while(direccionInt == 0);

        strcpy(c.rol, "cliente");
        printf("Rol asignado automaticamente: cliente\n");
    }

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
        printf("\nCliente guardado correctamente en el sistema.\n");
    }
    else
    {
        printf("Error al guardar cliente DNI: [%s]\n", c.dni);
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
    if (file == NULL)
    {
        return 0;
    }

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
                    gets(c.nombre);

                    nombreValido = 1;
                    if(strlen(c.nombre) == 0) nombreValido = 0;

                    for(int j=0; j<strlen(c.nombre); j++)
                    {
                        int esMinuscula = (c.nombre[j] >= 'a' && c.nombre[j] <= 'z');
                        int esMayuscula = (c.nombre[j] >= 'A' && c.nombre[j] <= 'Z');
                        int esEspacio   = (c.nombre[j] == ' ');

                        if(!esMinuscula && !esMayuscula && !esEspacio)
                        {
                            nombreValido = 0;
                            break;
                        }
                    }
                    if(nombreValido == 0)
                    {
                        printf("Error: Solo letras y espacios.\n");
                    }

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
                        if(c.telefono[i] < '0' || c.telefono[i] > '9')
                        {
                            valido = 0;
                        }
                    }

                    if(valido == 1 && strlen(c.telefono) != 10)
                    {
                        valido = 0;
                        printf("Error: El telefono debe tener solo 10 digitos.\n");
                    }
                    else if (valido == 0)
                    {
                        printf("Error: Solo numeros.\n");
                    }
                }
                while(valido==0);
                break;
            }

            case 4:
            {
                int direccionInt = 0;
                char direccion1[100];

                do
                {
                    printf("Ingrese direccion: ");
                    fflush(stdin);
                    gets(direccion1);

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
