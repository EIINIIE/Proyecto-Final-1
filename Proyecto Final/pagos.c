#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagos.h"
#include "auto.h"
#include "auto_cliente.h"
#include "venta.h"
#include "fecha.h"


void transferir_auto_a_cliente(Auto autoVendido, char dniComprador[])
{
    FILE *file = fopen("autos_cliente.bin", "ab");
    if(file == NULL) return;

    AutoCliente autoNuevo;

    strcpy(autoNuevo.patente, autoVendido.patente);
    strcpy(autoNuevo.marca, autoVendido.marca);
    strcpy(autoNuevo.modelo, autoVendido.modelo);
    autoNuevo.anio = autoVendido.anio;
    autoNuevo.kms = autoVendido.kms;
    autoNuevo.precioDeAdquisicion = autoVendido.precioDeAdquisicion;
    autoNuevo.precioFinal = autoVendido.precioFinal;

    strcpy(autoNuevo.titular.dni, dniComprador);
    strcpy(autoNuevo.titular.nombre, "Propietario (Venta)");
    strcpy(autoNuevo.titular.rol, "comprador");

    fwrite(&autoNuevo, sizeof(AutoCliente), 1, file);
    fclose(file);
    printf("\nTransferencia de propiedad registrada.\n");
}


void eliminar_auto_stock(char patenteEliminar[])
{
    FILE *archivo = fopen("autos.bin", "rb");
    FILE *temporal = fopen("temp.bin", "wb");

    if (archivo == NULL || temporal == NULL)
    {
        return;
    }


    Auto a;
    int encontrado = 0;

    while(fread(&a, sizeof(Auto), 1, archivo) == 1)
    {
        if(strcmp(a.patente, patenteEliminar) != 0)
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

    if(encontrado == 1)
    {
        remove("autos.bin");
        rename("temp.bin", "autos.bin");
        printf("[SISTEMA] Auto eliminado del stock.\n");
    }
    else
    {
        remove("temp.bin");
    }
}


void registrar_venta_archivo(Auto autoVendido, char dniComprador[], char dniVendedor[])
{
    FILE *file = fopen("ventas.bin", "ab");
    if(file == NULL)
    {
        printf("[ERROR] No se pudo registrar la venta.\n");
        return;
    }

    Venta nuevaVenta;

    printf("\nIngrese la fecha de la venta:\n");
    nuevaVenta.fecha = cargar_Fecha();

    strcpy(nuevaVenta.patenteAutoVendido, autoVendido.patente);
    nuevaVenta.precioVenta = autoVendido.precioFinal;
    nuevaVenta.ganancia = autoVendido.precioFinal - autoVendido.precioDeAdquisicion;
    strcpy(nuevaVenta.dniComprador, dniComprador);
    strcpy(nuevaVenta.dniVendedor, dniVendedor);

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

        if(comparacion == 0) return medio;
        if(comparacion < 0) inicio = medio + 1;
        else fin = medio - 1;
    }
    return -1;
}


void gestionDePagos()
{
    FILE *file = fopen("autos.bin", "rb");
    if(file == NULL)
    {
        printf("\nError: No hay stock disponible.\n");
        return;
    }

    Auto listaAutos[100];
    int validos = 0;

    while(fread(&listaAutos[validos], sizeof(Auto), 1, file) == 1 && validos < 100)
    {
        validos++;
    }
    fclose(file);

    if(validos == 0)
    {
        printf("El stock est  vac o.\n");
        return;
    }

    ordenarPorPatente(listaAutos, validos);

    char patenteBusq[11];
    printf("\n--- VENTA DE UNIDAD ---\n");
    printf("Ingrese patente del auto: ");
    scanf("%s", patenteBusq);

    int pos = buscarPatenteBinaria(listaAutos, validos, patenteBusq);

    if(pos != -1)
    {
        printf("\n--- AUTO SELECCIONADO ---\n");
        printf("Modelo: %s %s\n", listaAutos[pos].marca, listaAutos[pos].modelo);
        printf("Precio: $%.2f\n", listaAutos[pos].precioFinal);
        printf("Ganancia estimada: $%.2f\n", listaAutos[pos].precioFinal - listaAutos[pos].precioDeAdquisicion);

        char confirmacion;
        printf("\n Confirmar venta? (s/n): ");
        fflush(stdin);
        scanf(" %c", &confirmacion);

        if(confirmacion == 's' || confirmacion == 'S')
        {
            char dniComprador[15];
            char dniVendedor[15];

            printf("\nDNI del Comprador: ");
            scanf("%s", dniComprador);

            printf("DNI del Vendedor: ");
            scanf("%s", dniVendedor);

            registrar_venta_archivo(listaAutos[pos], dniComprador, dniVendedor);
            transferir_auto_a_cliente(listaAutos[pos], dniComprador);
            eliminar_auto_stock(patenteBusq);

            printf("\n********************************\n");
            printf("   VENTA COMPLETADA CON EXITO!  \n");
            printf("********************************\n");
        }
        else
        {
            printf("\nOperacion cancelada.\n");
        }
    }
    else
    {
        printf("\n Auto no encontrado en stock.\n");
    }
}
