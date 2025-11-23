#include <stdio.h>
#include <stdlib.h>
#include "auto_cliente.h"
#include "cliente.h"

/// FUNCION 1
AutoCliente cargar_auto_cliente()
{
    AutoCliente autos; /// Usamos la nueva estructura

    int valido = 0;
    char aux[50];

    printf("\n========================================\n");
    printf("           CARGA DE NUEVO AUTO          \n");
    printf("========================================\n");

    // 1. PATENTE
    do
    {
        printf("Patente (AA 123 CD): ");
        fflush(stdin);
        gets(aux);
        for(int i=0; i<strlen(aux); i++)
        {
            if(aux[i]>='a' && aux[i]<='z') aux[i]-=32;
        }

        if(strlen(aux)==9 && aux[0]>='A' && aux[0]<='Z' && aux[1]>='A' && aux[1]<='Z' && aux[2]==' ' &&
                aux[3]>='0' && aux[3]<='9' && aux[4]>='0' && aux[4]<='9' && aux[5]>='0' && aux[5]<='9' &&
                aux[6]==' ' && aux[7]>='A' && aux[7]<='Z' && aux[8]>='A' && aux[8]<='Z')
        {
            if(existe_patente_en_archivo(aux)==1)
            {
                printf("ERROR: Patente duplicada.\n");
                valido = 0;
            }
            else
            {
                strcpy(autos.patente, aux);
                valido = 1;
            }
        }
        else
        {
            printf("ERROR: Formato invalido. Use: AA 123 CD\n");
            valido = 0;
        }
    }
    while(valido == 0);

    // 2. MARCA
    valido = 0;
    do
    {
        printf("Marca: ");
        fflush(stdin);
        gets(aux);
        if(es_marca_valida_auto_cliente(aux)==1)
        {
            strcpy(autos.marca, aux);
            valido = 1;
        }
        else
        {
            printf("ERROR: Marca no reconocida (Ej: Ford, Fiat, BMW...)\n");
            valido = 0;
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
        if(es_modelo_valido_auto_cliente(autos.marca, aux)==1)
        {
            strcpy(autos.modelo, aux);
            valido = 1;
        }
        else
        {
            printf("ERROR: El modelo '%s' no corresponde a la marca '%s' o no existe.\n", aux, autos.marca);
            valido = 0;
        }
    }
    while(valido == 0);

    // 4. ANIO
    do
    {
        autos.anio = ingresar_entero_auto_cliente("Anio (1885 - 2025): ");
        if(autos.anio >= 1885 && autos.anio <= 2025)
        {
            valido = 1;
        }
        else
        {
            printf("ERROR: Anio fuera de rango.\n");
            valido = 0;
        }
    }
    while(valido == 0);

    autos.kms = ingresar_entero_auto_cliente("Kilometraje: ");
    autos.precioDeAdquisicion = ingresar_float_auto_cliente("Precio de Adquisicion: ");

    strcpy(autos.titular.nombre, "Concesionaria");
    strcpy(autos.titular.dni, "00000000");
    strcpy(autos.titular.telefono, "000000000");
    strcpy(autos.titular.direccion, "S/N");
    strcpy(autos.titular.rol, "concesionaria");

    autos.precioFinal = autos.precioDeAdquisicion * 1.20;
    return autos;

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

    /// Evito guardar cliente duplicado
    if (cliente_existente(nuevo_auto.titular.dni) == 0)
    {
        ///Guardo el cliente en clientes.bin
        guardar_cliente_en_archivo(nuevo_auto.titular);
    }
    else
    {
        printf("El cliente ya está registrado. No se vuelve a guardar.\n");
    }

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

/// ////////

int ingresar_entero_auto_cliente(char mensaje[])
{
    char buffer[50];
    int valido;
    int i;

    do
    {
        valido = 1;
        printf("%s", mensaje);
        fflush(stdin);
        gets(buffer);

        if(strlen(buffer) == 0) valido = 0;

        for(i = 0; i < strlen(buffer); i++)
        {
            if(buffer[i] < '0' || buffer[i] > '9')
            {
                valido = 0;
                break;
            }
        }

        if(valido == 0) printf("ERROR: Debe ingresar solo numeros enteros.\n");

    }
    while(valido == 0);

    return atoi(buffer);
}

float ingresar_float_auto_cliente(char mensaje[])
{
    char buffer[50];
    int valido;
    int i;
    int puntos;
    float numeroFinal = 0;

    do
    {
        valido = 1;
        puntos = 0;
        printf("%s", mensaje);
        fflush(stdin);
        gets(buffer);

        if(strlen(buffer) == 0) valido = 0;

        for(i = 0; i < strlen(buffer); i++)
        {
            if(buffer[i] == '.')
            {
                puntos++;
                if(puntos > 1)
                {
                    valido = 0;
                    break;
                }
            }
            else if(buffer[i] < '0' || buffer[i] > '9')
            {
                valido = 0;
                break;
            }
        }

        if(valido == 0)
        {
            printf("ERROR: Debe ingresar un precio valido (ej: 15000.50).\n");
        }

    }
    while(valido == 0);

    sscanf(buffer, "%f", &numeroFinal);

    return numeroFinal;
}

int es_marca_valida_auto_cliente(char m[])
{
    char temp[50];
    strcpy(temp, m);
    for(int i=0; i<strlen(temp); i++) temp[i] = toupper(temp[i]);

    if(strcmp(temp, "FORD") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "CHEVROLET") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "FIAT") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "VOLKSWAGEN") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "VW") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "TOYOTA") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "RENAULT") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "PEUGEOT") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "NISSAN") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "HONDA") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "CITROEN") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "JEEP") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "BMW") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "MERCEDES BENZ") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "MERCEDES") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "AUDI") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "PORSCHE") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "FERRARI") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "LAMBORGHINI") == 0)
    {
        return 1;
    }

    if(strcmp(temp, "MCLAREN") == 0)
    {
        return 1;
    }

    return 0;
}

int es_modelo_valido_auto_cliente(char marcaElegida[], char modeloIngresado[])
{
    char m[50], mod[50];
    strcpy(m, marcaElegida);
    strcpy(mod, modeloIngresado);

    for(int i=0; i<strlen(m); i++) m[i] = toupper(m[i]);
    for(int i=0; i<strlen(mod); i++) mod[i] = toupper(mod[i]);

    // 1. FORD
    if(strcmp(m, "FORD") == 0)
    {
        if(strcmp(mod, "RANGER")==0 || strcmp(mod, "FIESTA")==0 || strcmp(mod, "FOCUS")==0 || strcmp(mod, "KA")==0 || strcmp(mod, "MUSTANG")==0 || strcmp(mod, "ECOSPORT")==0)
        {
            return 1;
        }
        // 2. CHEVROLET
        else if(strcmp(m, "CHEVROLET") == 0)
        {
            if(strcmp(mod, "CRUZE")==0 || strcmp(mod, "ONIX")==0 || strcmp(mod, "S10")==0 || strcmp(mod, "CAMARO")==0 || strcmp(mod, "TRACKER")==0)
            {
                return 1;
            }

        }
        // 3. FIAT
        else if(strcmp(m, "FIAT") == 0)
        {
            if(strcmp(mod, "CRONOS")==0 || strcmp(mod, "ARGO")==0 || strcmp(mod, "TORO")==0 || strcmp(mod, "STRADA")==0 || strcmp(mod, "PULSE")==0 || strcmp(mod, "MOBI")==0)
            {
                return 1;
            }

        }
        // 4. PEUGEOT
        else if(strcmp(m, "PEUGEOT") == 0)
        {
            if(strcmp(mod, "208")==0 || strcmp(mod, "2008")==0 || strcmp(mod, "308")==0 || strcmp(mod, "PARTNER")==0)
            {
                return 1;
            }

        }
        // 5. VOLKSWAGEN
        else if(strcmp(m, "VOLKSWAGEN") == 0 || strcmp(m, "VW") == 0)
        {
            if(strcmp(mod, "GOL")==0 || strcmp(mod, "GOL TREND")==0 || strcmp(mod, "AMAROK")==0 || strcmp(mod, "POLO")==0 || strcmp(mod, "VENTO")==0 || strcmp(mod, "TAOS")==0)
            {
                return 1;
            }

        }
        // 6. TOYOTA
        else if(strcmp(m, "TOYOTA") == 0)
        {
            if(strcmp(mod, "HILUX")==0 || strcmp(mod, "COROLLA")==0 || strcmp(mod, "ETIOS")==0 || strcmp(mod, "YARIS")==0 || strcmp(mod, "SW4")==0)
            {
                return 1;
            }

        }
        // 7. RENAULT
        else if(strcmp(m, "RENAULT") == 0)
        {
            if(strcmp(mod, "SANDERO")==0 || strcmp(mod, "LOGAN")==0 || strcmp(mod, "KANGOO")==0 || strcmp(mod, "DUSTER")==0 || strcmp(mod, "KWID")==0 || strcmp(mod, "ALASKAN")==0)
            {
                return 1;
            }

        }
        // --- LAS QUE FALTABAN ---
        else if(strcmp(m, "NISSAN") == 0)
        {
            if(strcmp(mod, "VERSA")==0 || strcmp(mod, "SENTRA")==0 || strcmp(mod, "KICKS")==0 || strcmp(mod, "FRONTIER")==0)

            {
                return 1;
            }

        }
        else if(strcmp(m, "HONDA") == 0)
        {
            if(strcmp(mod, "CIVIC")==0 || strcmp(mod, "FIT")==0 || strcmp(mod, "HR-V")==0 || strcmp(mod, "CR-V")==0)
            {
                return 1;
            }

        }
        else if(strcmp(m, "CITROEN") == 0)
        {
            if(strcmp(mod, "C3")==0 || strcmp(mod, "C4")==0 || strcmp(mod, "BERLINGO")==0 || strcmp(mod, "C4 CACTUS")==0)
            {
                return 1;
            }

        }
        else if(strcmp(m, "JEEP") == 0)
        {
            if(strcmp(mod, "RENEGADE")==0 || strcmp(mod, "COMPASS")==0 || strcmp(mod, "WRANGLER")==0)
            {
                return 1;
            }

        }
        // DEPORTIVOS / LUJO
        else if(strcmp(m, "FERRARI") == 0)
        {
            if(strcmp(mod, "488")==0 || strcmp(mod, "F8")==0 || strcmp(mod, "PORTOFINO")==0)
            {
                return 1;
            }

        }
        else if(strcmp(m, "BMW") == 0)
        {
            if(strcmp(mod, "X5")==0 || strcmp(mod, "SERIE 3")==0 || strcmp(mod, "320")==0 || strcmp(mod, "118")==0)
            {
                return 1;
            }

        }
        else if(strcmp(m, "MERCEDES BENZ") == 0 || strcmp(m, "MERCEDES") == 0)
        {
            if(strcmp(mod, "CLASE A")==0 || strcmp(mod, "CLASE C")==0 || strcmp(mod, "GLA")==0 || strcmp(mod, "SPRINTER")==0)
            {
                return 1;
            }

        }

    }
    else if(strcmp(m, "AUDI") == 0)
    {
        if(strcmp(mod, "A1")==0 || strcmp(mod, "A3")==0 || strcmp(mod, "A4")==0 || strcmp(mod, "Q3")==0 || strcmp(mod, "Q5")==0)
        {
            return 1;
        }

    }
    else if(strcmp(m, "PORSCHE") == 0)
    {
        if(strcmp(mod, "911")==0 || strcmp(mod, "CAYENNE")==0 || strcmp(mod, "MACAN")==0 || strcmp(mod, "PANAMERA")==0)
        {
            return 1;
        }

    }
    else if(strcmp(m, "LAMBORGHINI") == 0)
    {
        if(strcmp(mod, "AVENTADOR")==0 || strcmp(mod, "HURACAN")==0 || strcmp(mod, "URUS")==0)
        {
            return 1;
        }

    }
    else if(strcmp(m, "MCLAREN") == 0)
    {
        if(strcmp(mod, "720S")==0 || strcmp(mod, "GT")==0 || strcmp(mod, "ARTURA")==0)
        {
            return 1;
        }

    }

    return 0;
}

void modificar_auto_stock_auto_cliente()
{
    char patenteBuscada[20];
    char aux[50];
    int valido;

    mostrar_todos_autos(ARCHIVO_AUTOS);

    do
    {
        printf("\nIngrese la patente del auto a modificar: ");
        fflush(stdin);
        gets(patenteBuscada);
    }
    while(strlen(patenteBuscada) == 0);

    for(int i=0; i<strlen(patenteBuscada); i++)
    {
        if(patenteBuscada[i] >= 'a' && patenteBuscada[i] <= 'z') patenteBuscada[i] -= 32;
    }

    FILE *f = fopen(ARCHIVO_AUTOS, "r+b");
    if(f == NULL)
    {
        printf("Error.\n");
        return;
    }

    Auto a;
    int encontrado = 0;

    while(fread(&a, sizeof(Auto), 1, f) == 1)
    {
        if(strcmp(a.patente, patenteBuscada) == 0)
        {
            encontrado = 1;
            int subOp = 0;

            do
            {
                system("cls");
                printf("\n--- EDITANDO AUTO: %s ---\n", a.patente);
                mostrar_auto(a);

                printf("\nSELECCIONE CAMPO A MODIFICAR:\n");
                printf("1. Modificar Patente\n");
                printf("2. Modificar Marca\n");
                printf("3. Modificar Modelo\n");
                printf("4. Modificar Anio\n");
                printf("5. Modificar Kilometraje\n");
                printf("6. Modificar Precio de Adquisicion\n");
                printf("0. GUARDAR Y SALIR\n");
                printf("-----------------------------\n");

                subOp = ingresar_entero_auto_cliente("Su eleccion: ");

                switch(subOp)
                {
                case 1:
                    valido = 0;
                    do
                    {
                        printf("\nNueva Patente (AA 123 CD): ");
                        fflush(stdin);
                        gets(aux);
                        for(int i=0; i<strlen(aux); i++)
                        {
                            if(aux[i]>='a' && aux[i]<='z') aux[i]-=32;
                        }

                        if(strlen(aux)==9 && aux[0]>='A' && aux[0]<='Z' && aux[1]>='A' && aux[1]<='Z' &&
                                aux[2]==' ' && aux[3]>='0' && aux[3]<='9' && aux[4]>='0' && aux[4]<='9' &&
                                aux[5]>='0' && aux[5]<='9' && aux[6]==' ' && aux[7]>='A' && aux[7]<='Z' &&
                                aux[8]>='A' && aux[8]<='Z')
                        {
                            strcpy(a.patente, aux);
                            valido = 1;
                            printf("Patente cambiada temporalmente.\n");
                        }
                        else
                        {
                            printf("Formato invalido.\n");
                        }
                    }
                    while(valido == 0);
                    system("pause");
                    break;

                case 2:
                    valido = 0;
                    do
                    {
                        printf("\nNueva Marca: ");
                        fflush(stdin);
                        gets(aux);
                        if(es_marca_valida_auto_cliente(aux)==1)
                        {
                            strcpy(a.marca, aux);
                            valido = 1;
                            printf("Marca actualizada.\n");
                        }
                        else
                        {
                            printf("Marca no valida.\n");
                        }
                    }
                    while(valido == 0);
                    system("pause");
                    break;

                case 3:
                    valido = 0;
                    do
                    {
                        printf("\nNuevo Modelo (para %s): ", a.marca);
                        fflush(stdin);
                        gets(aux);
                        if(es_modelo_valido_auto_cliente(a.marca, aux)==1)
                        {
                            strcpy(a.modelo, aux);
                            valido = 1;
                            printf("Modelo modificado correctamente.\n");
                        }
                        else
                        {
                            printf("ERROR: Modelo invalido para la marca %s.\n", a.marca);
                        }
                    }
                    while(valido == 0);
                    system("pause");
                    break;

                case 4:
                    valido = 0;
                    do
                    {
                        a.anio = ingresar_entero_auto_cliente("Nuevo Anio (1885-2025): ");
                        if(a.anio >= 1885 && a.anio <= 2025) valido = 1;
                        else printf("Anio fuera de rango.\n");
                    }
                    while(valido == 0);
                    break;

                case 5:
                    a.kms = ingresar_entero_auto_cliente("Nuevo Kilometraje: ");
                    break;

                case 6:
                    a.precioDeAdquisicion = ingresar_float_auto_cliente("Nuevo Precio: ");
                    a.precioFinal = a.precioDeAdquisicion * 1.20;
                    printf("Precio Final actualizado a: $%.2f\n", a.precioFinal);
                    system("pause");
                    break;

                case 0:
                    printf("Guardando cambios...\n");
                    break;

                default:
                    printf("Opcion invalida.\n");
                    break;
                }

            }
            while(subOp != 0);

            fseek(f, -sizeof(Auto), SEEK_CUR);
            fwrite(&a, sizeof(Auto), 1, f);
            printf("Auto modificado correctamente.\n");
            break;
        }
    }

    if(!encontrado) printf("No se encontro un auto con la patente %s en el stock.\n", patenteBuscada);
    fclose(f);
}

void agregar_auto_stock() /// ver
{
    Auto a = cargar_auto();
    FILE *f = fopen(ARCHIVO_AUTOS, "ab");
    if(f != NULL)
    {
        fwrite(&a, sizeof(Auto), 1, f);
        fclose(f);
        printf("\n Auto agregado al stock exitosamente.\n");
    }
    else
    {
        printf("Error al abrir el archivo.\n");
    }
}

int existe_patente_en_archivo(char patenteBuscada[]) /// Revisar este
{
    FILE *file = fopen(ARCHIVO_AUTOS, "rb");
    if(file == NULL) return 0;

    Auto a;
    while(fread(&a, sizeof(Auto), 1, file) == 1)
    {
        if(strcmp(a.patente, patenteBuscada) == 0)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

