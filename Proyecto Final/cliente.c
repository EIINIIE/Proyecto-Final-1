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
    char aux[100]; // Buffer para fgets
    int existe = 0; // Se mantiene por si era parte del cuerpo original, pero ya no se usa para DNI.

    printf("\n---- DATOS ADICIONALES DEL CLIENTE (Nombre, Telefono, Direccion) ----\n");

    // NOTA: Se ha eliminado la carga de DNI para evitar la duplicacion en el registro de usuario.
    // El DNI debe ser cargado y asignado por la funcion que llame a esta.
    strcpy(c.dni, "0"); // Inicializamos el DNI a "0"

    // --- CARGA NOMBRE ---
    do
    {
        valido = 1;
        printf("Ingrese Nombre y Apellido: ");
        fflush(stdin);
        if (fgets(aux, sizeof(aux), stdin) == NULL || strlen(aux) <= 1)
        {
            valido = 0;
        }
        else
        {
            // Eliminar el salto de linea al final si existe
            size_t len = strlen(aux);
            if (len > 0 && aux[len - 1] == '\n')
            {
                aux[len - 1] = '\0';
            }

            if (strlen(aux) < 4)
            {
                printf("Nombre y Apellido deben tener al menos 4 caracteres.\n");
                valido = 0;
            }
            else
            {
                strcpy(c.nombre, aux);
            }
        }
    }
    while(valido == 0);

    // --- CARGA TELEFONO ---
    do
    {
        valido = 1;
        printf("Ingrese Telefono (solo numeros, 8-15 digitos): ");
        fflush(stdin);
        scanf("%s", c.telefono);

        if(strlen(c.telefono) < 8 || strlen(c.telefono) > 15)
        {
            printf("El telefono debe tener entre 8 y 15 digitos.\n");
            valido = 0;
        }
        else
        {
            for(i = 0; i < strlen(c.telefono); i++)
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
                // Se asume que telefono_Existente existe en cliente.h/c
                if (telefono_Existente(c.telefono) == 1)
                {
                    printf("Ya existe un cliente con este telefono. Ingrese otro.\n");
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
