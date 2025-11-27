#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pagos.h"
#include "auto.h"
#include "auto_cliente.h"
#include "venta.h"
#include "fecha.h"
#include "autos_disponibles.h"


void transferir_auto_a_cliente(Auto autoVendido, char dniComprador[])
{
    FILE *file = fopen("autos_cliente.bin", "ab");
    if(file == NULL)
    {
        return;
    }

    AutoCliente autoNuevo;

    strcpy(autoNuevo.patente, autoVendido.patente);
    strcpy(autoNuevo.marca, autoVendido.marca);
    strcpy(autoNuevo.modelo, autoVendido.modelo);
    autoNuevo.anio = autoVendido.anio;
    autoNuevo.kms = autoVendido.kms;
    autoNuevo.precioDeAdquisicion = autoVendido.precioDeAdquisicion;
    autoNuevo.precioFinal = autoVendido.precioFinal;

    // Buscamos datos reales del comprador para poner en la tarjeta del auto
    Cliente c = obtener_datos_cliente(dniComprador);
    if(strcmp(c.dni, "0") != 0)
    {
        autoNuevo.titular = c;
    }
    else
    {
        strcpy(autoNuevo.titular.dni, dniComprador);
        strcpy(autoNuevo.titular.nombre, "Propietario (Venta)");
        strcpy(autoNuevo.titular.rol, "comprador");
    }

    fwrite(&autoNuevo, sizeof(AutoCliente), 1, file);
    fclose(file);

    printf("\nTransferencia de propiedad registrada a nombre de %s.\n", autoNuevo.titular.nombre);
}

void eliminar_auto_stock(char patenteEliminar[])
{
    FILE *archivo = fopen("autos.bin", "rb");
    if (archivo == NULL)
    {
        return;
    }

    FILE *temporal = fopen("temp.bin", "wb");
    if (temporal == NULL)
    {
        fclose(archivo);
        return;
    }

    Auto a;
    int encontrado = 0;

    while (fread(&a, sizeof(Auto), 1, archivo) == 1)
    {
        if (strcmp(a.patente, patenteEliminar) != 0)
        {
            fwrite(&a, sizeof(Auto), 1, temporal);
        }
        else
        {
            encontrado = 1;
        }
    }

    fclose(archivo);
    fclose(temporal);

    archivo = fopen("autos.bin", "wb");
    temporal = fopen("temp.bin", "rb");

    if (archivo && temporal)
    {
        while (fread(&a, sizeof(Auto), 1, temporal) == 1)
        {
            fwrite(&a, sizeof(Auto), 1, archivo);
        }
    }

    if(archivo)
    {
        fclose(archivo);
    }
    if(temporal)
    {
        fclose(temporal);
    }

    if (encontrado == 1)
    {
        printf("Auto eliminado del stock.\n");
    }
}

void registrar_venta_archivo(Auto autoVendido, char dniComprador[])
{
    FILE *file = fopen("ventas.bin", "ab");
    if(file == NULL)
    {
        printf("No se pudo registrar la venta.\n");
        return;
    }

    Venta nuevaVenta;
    Fecha fechaActual = hoy();

    printf("Fecha de operacion: ");
    mostrar_Fecha(fechaActual);

    nuevaVenta.fecha = fechaActual;
    strcpy(nuevaVenta.patenteAutoVendido, autoVendido.patente);
    nuevaVenta.precioVenta = autoVendido.precioFinal;
    nuevaVenta.ganancia = autoVendido.precioFinal - autoVendido.precioDeAdquisicion;
    strcpy(nuevaVenta.dniComprador, dniComprador);

    fwrite(&nuevaVenta, sizeof(Venta), 1, file);
    fclose(file);

    printf("\nVenta registrada exitosamente.\n");
    printf("Ganancia obtenida: $%.2f\n", nuevaVenta.ganancia);
}

// --- ORDENAMIENTO ---
void ordenarPorPatente(Auto autos[], int validos)
{
    Auto aux;
    for(int i = 0; i < validos - 1; i++)
    {
        for(int j = i + 1; j < validos; j++)
        {
            if(strcmp(autos[i].patente, autos[j].patente) > 0)
            {
                aux = autos[i];
                autos[i] = autos[j];
                autos[j] = aux;
            }
        }
    }
}

int buscarPatenteBinaria(Auto autos[], int validos, char patenteBuscada[])
{
    int inicio = 0;
    int fin = validos - 1;

    while(inicio <= fin)
    {
        int medio = inicio + (fin - inicio) / 2;
        int comparacion = strcmp(autos[medio].patente, patenteBuscada);

        if(comparacion == 0)
        {
            return medio;
        }
        if(comparacion < 0)
        {
            inicio = medio + 1;
        }
        else
        {
            fin = medio - 1;
        }
    }
    return -1;
}

// --- FUNCION MODIFICADA PARA DETECTAR DNI AUTOMATICO O MANUAL ---
void gestionDePagos(char dniSesion[])
{
    // 1. Mostrar todos los autos disponibles
    mostrar_todos_autos_disponibles();

    FILE *file = fopen("autos.bin", "rb");
    if(file == NULL)
    {
        printf("\nError: No hay stock disponible.\n");
        return;
    }

    Auto listaAutos[100];
    int validos = 0;

    // Cargar autos al arreglo
    while(fread(&listaAutos[validos], sizeof(Auto), 1, file) == 1 && validos < 100)
    {
        validos++;
    }
    fclose(file);

    if(validos == 0)
    {
        printf("El stock esta vacio.\n");
        return;
    }

    // Ordenar para busqueda binaria
    ordenarPorPatente(listaAutos, validos);

    char patenteBusq[20];

    // --- BUCLE DE SELECCION DE AUTO (SIMPLIFICADO) ---
    do
    {
        printf("\n--- VENTA DE UNIDAD ---\n");
        printf("Ingrese patente del auto (o '0' para salir): ");

        // --- ACA ESTA EL CAMBIO MAS SIMPLE ---
        fflush(stdin);      // Limpia el teclado
        gets(patenteBusq);  // Lee la patente con espacios (Ej: "AA 123 CD")

        // Si escribio '0', salimos
        if(strcmp(patenteBusq, "0") == 0)
        {
            return;
        }

    } while(strlen(patenteBusq) == 0);

    // Convertir a mayusculas
    for(int i=0; i<strlen(patenteBusq); i++)
    {
        patenteBusq[i] = toupper(patenteBusq[i]);
    }

    // Buscar en el arreglo
    int pos = buscarPatenteBinaria(listaAutos, validos, patenteBusq);

    if(pos != -1)
    {
        printf("\n--- AUTO SELECCIONADO ---\n");
        printf("Modelo: %s %s\n", listaAutos[pos].marca, listaAutos[pos].modelo);
        printf("Precio: $%.2f\n", listaAutos[pos].precioFinal);

        char confirmacion;
        printf("\n Confirmar venta? (s/n): ");
        fflush(stdin);
        scanf(" %c", &confirmacion);

        if(confirmacion == 's' || confirmacion == 'S')
        {
            char dniComprador[30];
            int ventaHabilitada = 0;

            // --- VALIDACION DE COMPRADOR ---
            if(strcmp(dniSesion, "0") != 0)
            {
                // CASO 1: CLIENTE LOGUEADO
                strcpy(dniComprador, dniSesion);
                printf("\n[INFO] Comprador identificado automaticamente: DNI %s\n", dniComprador);
                ventaHabilitada = 1;
            }
            else
            {
                // CASO 2: EMPLEADO VENDIENDO (Manual)
                int dniValido = 0;
                char buffer[50];

                do
                {
                    dniValido = 1;
                    printf("\nDNI del Comprador (Solo numeros): ");

                    // Lectura simple
                    fflush(stdin);
                    scanf("%s", buffer);

                    // A. Validar numeros
                    for(int k=0; k<strlen(buffer); k++)
                    {
                        if(buffer[k] < '0' || buffer[k] > '9')
                        {
                            dniValido = 0;
                            printf("Error: Ingrese solo numeros.\n");
                            break;
                        }
                    }

                    // B. Validar largo
                    if(dniValido && (strlen(buffer) < 7 || strlen(buffer) > 8))
                    {
                        dniValido = 0;
                        printf("Error: DNI debe tener 7 u 8 digitos.\n");
                    }

                    // C. Validar existencia del cliente
                    if(dniValido == 1)
                    {
                        if(cliente_existente(buffer) == 0)
                        {
                            // AVISO SIMPLE Y REPETICION AUTOMATICA
                            printf("\n[ERROR] Cliente NO registrado.\n");
                            dniValido = 0; // Vuelve a pedir sin preguntar nada mas
                        }
                        else
                        {
                            strcpy(dniComprador, buffer);
                            ventaHabilitada = 1;
                        }
                    }

                } while(dniValido == 0);
            }

            // --- EJECUTAR VENTA SI TODO OK ---
            if(ventaHabilitada == 1)
            {
                registrar_venta_archivo(listaAutos[pos], dniComprador);
                transferir_auto_a_cliente(listaAutos[pos], dniComprador);
                eliminar_auto_stock(patenteBusq);

                printf("\n********************************\n");
                printf("   VENTA COMPLETADA CON EXITO!  \n");
                printf("********************************\n");
            }
        }
        else
        {
            printf("\nOperacion cancelada.\n");
        }
    }
    else
    {
        printf("\n Auto no encontrado en stock (Verifique la patente).\n");
    }
}
