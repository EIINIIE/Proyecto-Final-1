#include <stdio.h>
#include <string.h>
#include "venta.h"
#include "auto.h"
#include "cliente.h"
#include "fecha.h"

#define ARCHIVO_VENTAS "ventas.bin"
#define ARCHIVO_AUTOS "autos.bin"

Venta cargarVenta()
{
    Venta v;
    Auto a;
    Cliente c;
    int encontradoAuto = 0;
    int encontradoCliente = 0;

    printf("---- CARGA DE VENTA ----\n");
    v.fecha = cargar_Fecha();

    // Buscar auto en stock
    do
    {
        printf("Ingrese patente del auto vendido: ");
        scanf("%s", v.patenteAutoVendido);

        FILE *fAuto = fopen(ARCHIVO_AUTOS, "rb");
        encontradoAuto = 0;
        if(fAuto != NULL)
        {
            while(fread(&a, sizeof(Auto), 1, fAuto) == 1)
            {
                if(strcmp(a.patente, v.patenteAutoVendido) == 0 &&
                   strcmp(a.titular.rol, "concesionaria") == 0)
                {
                    encontradoAuto = 1;
                    break;
                }
            }
            fclose(fAuto);
        }

        if(!encontradoAuto)
            printf("Auto no encontrado o no pertenece a la concesionaria.\n");

    } while(!encontradoAuto);

    printf("Precio de Costo: %.2f\n", a.precioDeAdquisicion);
    printf("Ingrese precio final de venta: ");
    scanf("%f", &v.precioVenta);

    v.ganancia = v.precioVenta - a.precioDeAdquisicion;

    // Buscar cliente
    do
    {
        printf("Ingrese DNI del comprador: ");
        scanf("%s", v.dniComprador);

        FILE *fCli = fopen("clientes.bin", "rb");
        encontradoCliente = 0;
        if(fCli != NULL)
        {
            while(fread(&c, sizeof(Cliente), 1, fCli) == 1)
            {
                if(strcmp(c.dni, v.dniComprador) == 0)
                {
                    encontradoCliente = 1;
                    break;
                }
            }
            fclose(fCli);
        }

        if(!encontradoCliente)
            printf("Cliente no encontrado.\n");

    } while(!encontradoCliente);

    printf("Ingrese DNI del vendedor: ");
    scanf("%s", v.dniVendedor);

    return v;
}

void transferirAutoAlComprador(char patente[], char dniComprador[])
{
    FILE *f = fopen(ARCHIVO_AUTOS, "rb+");
    if(f == NULL) return;

    Auto a;
    while(fread(&a, sizeof(Auto), 1, f) == 1)
    {
        if(strcmp(a.patente, patente) == 0)
        {
            Cliente comprador;
            FILE *fCli = fopen("clientes.bin", "rb");
            if(fCli == NULL) break;

            while(fread(&comprador, sizeof(Cliente), 1, fCli) == 1)
            {
                if(strcmp(comprador.dni, dniComprador) == 0)
                {
                    a.titular = comprador;
                    fseek(f, -sizeof(Auto), SEEK_CUR);
                    fwrite(&a, sizeof(Auto), 1, f);
                    fclose(fCli);
                    fclose(f);
                    return;
                }
            }
            fclose(fCli);
        }
    }
    fclose(f);
}

void registrarVenta()
{
    Venta v = cargarVenta();
    FILE *f = fopen(ARCHIVO_VENTAS, "ab");
    if(f != NULL)
    {
        fwrite(&v, sizeof(Venta), 1, f);
        fclose(f);
        transferirAutoAlComprador(v.patenteAutoVendido, v.dniComprador);
        printf("Venta registrada y auto transferido correctamente.\n");
    }
}

void mostrarVenta(Venta v)
{
    printf("\nFecha: %d/%d/%d\n", v.fecha.dia, v.fecha.mes, v.fecha.anio);
    printf("Patente: %s\n", v.patenteAutoVendido);
    printf("Precio: %.2f\n", v.precioVenta);
    printf("Ganancia: %.2f\n", v.ganancia);
    printf("DNI Comprador: %s\n", v.dniComprador);
    printf("DNI Vendedor: %s\n", v.dniVendedor);
}

void mostrarVentas()
{
    FILE *f = fopen(ARCHIVO_VENTAS, "rb");
    if(f == NULL)
    {
        printf("No hay ventas registradas.\n");
        return;
    }

    Venta v;
    while(fread(&v, sizeof(Venta), 1, f) == 1)
    {
        mostrarVenta(v);
    }
    fclose(f);
}
