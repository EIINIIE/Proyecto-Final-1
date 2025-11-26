#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>      // Necesario para toupper
#include "auto_cliente.h"
#include "cliente.h"
#include "auto.h"       // Necesario para validar marca y modelo

/// FUNCION 1: Cargar Auto Cliente (SOLUCION FINAL - LIMPIEZA FORZADA)
AutoCliente cargar_auto_cliente()
{
    AutoCliente autos;
    char aux[50];
    int valido = 0;

    // --- LIMPIEZA CLAVE ---
    // Limpiamos el teclado apenas entramos para borrar el Enter del menu
    fflush(stdin);

    printf("---- DATOS DEL AUTO DEL CLIENTE ----\n");

    // 1. PATENTE
    do
    {
        valido = 0;
        printf("Patente (AA 123 CD): ");
        gets(aux);

        // Si el usuario aprieta Enter sin querer (vacio), repetimos el bucle
        // pero sin mostrar errores, simplemente vuelve a pedir.
        if(strlen(aux) > 0)
        {
            // Convertir a mayusculas
            for(int i=0; i<strlen(aux); i++)
            {
                aux[i] = toupper(aux[i]);
            }

            // Validacion estricta de formato (AA 123 CD)
            if(strlen(aux)==9 &&
               aux[0]>='A' && aux[0]<='Z' &&
               aux[1]>='A' && aux[1]<='Z' &&
               aux[2]==' ' &&
               aux[3]>='0' && aux[3]<='9' &&
               aux[4]>='0' && aux[4]<='9' &&
               aux[5]>='0' && aux[5]<='9' &&
               aux[6]==' ' &&
               aux[7]>='A' && aux[7]<='Z' &&
               aux[8]>='A' && aux[8]<='Z')
            {
                 if(existe_patente_en_archivo(aux) == 1)
                 {
                     printf("ERROR: Esa patente ya existe en el sistema.\n");
                 }
                 else
                 {
                     strcpy(autos.patente, aux);
                     valido = 1;
                 }
            }
            else
            {
                printf("ERROR: Formato invalido. Use: AA 123 CD (respetando espacios).\n");
            }
        }
    }
    while(valido == 0);

    // 2. MARCA
    valido = 0;
    do
    {
        printf("Marca: ");
        fflush(stdin); // Aseguramos limpieza antes de leer
        gets(aux);

        if(strlen(aux) > 0)
        {
            if(es_marca_valida(aux) == 1)
            {
                strcpy(autos.marca, aux);
                valido = 1;
            }
            else
            {
                printf("ERROR: Marca no reconocida en el sistema.\n");
            }
        }
    }
    while(valido == 0);

    // 3. MODELO
    valido = 0;
    do
    {
        printf("Modelo: ");
        fflush(stdin);
        gets(aux);

        if(strlen(aux) > 0)
        {
            if(es_modelo_valido(autos.marca, aux) == 1)
            {
                strcpy(autos.modelo, aux);
                valido = 1;
            }
            else
            {
                printf("ERROR: Modelo no valido para la marca %s.\n", autos.marca);
            }
        }
    }
    while(valido == 0);

    int anio_actual = 2025;

    // 4. OTROS DATOS
    do
    {
        autos.anio = ingresar_entero("Anio: ");
        if (autos.anio < 1900 || autos.anio > anio_actual)
        {
            printf("ERROR: Anio invalido (1900-%d).\n", anio_actual);
        }
    }
    while (autos.anio < 1900 || autos.anio > anio_actual);

    autos.kms = ingresar_entero("Kilometros: ");
    autos.precioDeAdquisicion = ingresar_float("Precio estimado: ");

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

    // Guardo el cliente en clientes.bin tambien por si no existe
    guardar_cliente_en_archivo(nuevo_auto.titular);

    fwrite(&nuevo_auto, sizeof(AutoCliente), 1, file);
    fclose(file);
    printf("Auto y titular cargados correctamente\n");
}

/// FUNCION 3
void mostrar_auto_cliente(AutoCliente a)
{
    printf("---- DATOS DEL VEHICULO (CLIENTE) ----\n");
    printf("Patente: %s\n", a.patente);
    printf("Marca:   %s\n", a.marca);
    printf("Modelo:  %s\n", a.modelo);
    printf("Anio:    %d\n", a.anio);
    printf("Kms:     %d\n", a.kms);
    printf("Precio:  $%.2f\n", a.precioFinal);

    printf("---- DATOS DEL TITULAR ----\n");
    printf("Nombre:  %s\n", a.titular.nombre);
    printf("DNI:     %s\n", a.titular.dni);
    printf("---------------------------\n");
}

/// FUNCION 4
void mostrar_todos_autos_cliente()
{
    FILE* file = fopen(ARCHIVO_AUTOS_CLIENTE, "rb");
    if(file == NULL)
    {
        printf("Error al abrir el archivo de autos cliente (o no existen registros).\n");
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

/// FUNCION 5: Modificar Auto Cliente (CORREGIDA CON VALIDACIONES)
void modificar_auto_cliente_por_dni(char dniBuscado[])
{
    FILE *file = fopen(ARCHIVO_AUTOS_CLIENTE, "r+b");
    if (file == NULL)
    {
        printf("No se pudo abrir el archivo.\n");
        return;
    }

    AutoCliente aux;
    int encontrado = 0;
    char buffer[50];
    int valido = 0;

    while (fread(&aux, sizeof(AutoCliente), 1, file) == 1)
    {
        if (strcmp(aux.titular.dni, dniBuscado) == 0)
        {
            encontrado = 1;
            printf("\nAUTO ENCONTRADO PARA EL DNI: %s\n", dniBuscado);
            mostrar_auto_cliente(aux);

            printf("\n--- MODIFICACION DE DATOS DEL AUTO ---\n");

            // 0. PATENTE (Nueva)
            valido = 0;
            do
            {
                printf("Nueva Patente (Actual: %s): ", aux.patente);
                fflush(stdin);
                gets(buffer);

                // Convertir a mayusculas
                for(int i=0; i<strlen(buffer); i++)
                {
                    buffer[i] = toupper(buffer[i]);
                }

                if(strlen(buffer) > 5) // Validacion basica de largo
                {
                    strcpy(aux.patente, buffer);
                    valido = 1;
                }
                else
                {
                    printf("ERROR: Patente muy corta o invalida.\n");
                    valido = 0;
                }
            }
            while(valido == 0);

// 1. MARCA
            valido = 0;
            do
            {
                printf("Nueva marca (Actual: %s): ", aux.marca);
                fflush(stdin);
                gets(buffer);
                if(es_marca_valida(buffer) == 1)
                {
                    strcpy(aux.marca, buffer);
                    valido = 1;
                }
                else
                {
                    printf("Marca invalida.\n");
                }
            }
            while(valido == 0);

            // 2. MODELO
            valido = 0;
            do
            {
                printf("Nuevo modelo (Actual: %s): ", aux.modelo);
                fflush(stdin);
                gets(buffer);
                if(es_modelo_valido(aux.marca, buffer) == 1)
                {
                    strcpy(aux.modelo, buffer);
                    valido = 1;
                }
                else
                {
                    printf("Modelo invalido para %s.\n", aux.marca);
                }
            }
            while(valido == 0);

            // 3. ANIO y KMS
            aux.anio = ingresar_entero("Nuevo anio: ");
            aux.kms = ingresar_entero("Nuevo kilometraje: ");

            // 4. PRECIO
            aux.precioDeAdquisicion = ingresar_float("Nuevo precio original: ");
            aux.precioFinal = aux.precioDeAdquisicion;

            fseek(file, -sizeof(AutoCliente), SEEK_CUR);
            fwrite(&aux, sizeof(AutoCliente), 1, file);

            printf("\nAuto modificado correctamente.\n");
            break; // Salimos despues de modificar el primero encontrado
        }
    }

    if (encontrado == 0)
    {
        printf("No se encontro auto registrado bajo el DNI: %s.\n", dniBuscado);
    }
    fclose(file);
}

/// FUNCION 6
int cargar_autos_cliente_din(AutoCliente **listaAutos)
{
    FILE *file = fopen(ARCHIVO_AUTOS_CLIENTE, "rb");
    if (file == NULL)
    {
        return 0;
    }

    fseek(file, 0, SEEK_END);
    int cantidad = ftell(file) / sizeof(AutoCliente);
    rewind(file);

    if (cantidad == 0)
    {
        fclose(file);
        return 0;
    }

    *listaAutos = (AutoCliente *) malloc (cantidad * sizeof(AutoCliente));

    if (*listaAutos == NULL)
    {
        printf("Error al asignar memoria.\n");
        fclose(file);
        return 0;
    }

    fread(*listaAutos, sizeof(AutoCliente), cantidad, file);
    fclose(file);
    return cantidad;
}
