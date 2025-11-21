#include <stdio.h>
#include <string.h>
#include "venta.h"
#include "auto.h" // Incluye auto.h para usar ingresar_float
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

    v.fecha = hoy();
    printf("Fecha de Venta: ");
    mostrar_Fecha(v.fecha);

    // Buscar auto en stock
    do
    {
        printf("Ingrese patente del auto vendido: ");
        fflush(stdin);
        scanf("%s", v.patenteAutoVendido);

        FILE *fAuto = fopen(ARCHIVO_AUTOS, "rb");
        encontradoAuto = 0;

        if(fAuto != NULL)
        {
            while(fread(&a, sizeof(Auto), 1, fAuto) == 1)
            {
                // Buscamos patente Y que el dueño sea la concesionaria (no vendido aun)
                if(strcmp(a.patente, v.patenteAutoVendido) == 0 &&
                   strcmp(a.titular.rol, "concesionaria") == 0)
                {
                    encontradoAuto = 1;
                    break;
                }
            }
            fclose(fAuto);
        }

        if(encontradoAuto == 0)
        {
            printf("Auto no encontrado o ya fue vendido.\n");
        }

    } while(encontradoAuto == 0);

    printf("Precio de Costo: %.2f\n", a.precioDeAdquisicion);
    
    // USAMOS LA VALIDACION DE AUTO.C PARA PRECIO SEGURO
    v.precioVenta = ingresar_float("Ingrese precio final de venta: ");

    v.ganancia = v.precioVenta - a.precioDeAdquisicion;

    // Buscar cliente
    do
    {
        printf("Ingrese DNI del comprador: ");
        fflush(stdin);
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

        if(encontradoCliente == 0)
        {
            printf("Cliente no encontrado. Debe registrarlo primero.\n");
        }

    } while(encontradoCliente == 0);

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
                    a.titular = comprador; // Cambiamos el titular
                    fseek(f, -sizeof(Auto), SEEK_CUR);
                    fwrite(&a, sizeof(Auto), 1, f); // Sobrescribimos
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
        // Importante: Transferir la propiedad del auto
        transferirAutoAlComprador(v.patenteAutoVendido, v.dniComprador);
        printf("Venta registrada y auto transferido correctamente.\n");
    }
}

void mostrarVenta(Venta v)
{
    printf("\nFecha: %d/%d/%d\n", v.fecha.dia, v.fecha.mes, v.fecha.anio);
    printf("Patente: %s\n", v.patenteAutoVendido);
    printf("Precio: $%.2f\n", v.precioVenta);
    printf("Ganancia: $%.2f\n", v.ganancia);
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
    printf("\n--- HISTORIAL DE VENTAS ---\n");
    while(fread(&v, sizeof(Venta), 1, f) == 1)
    {
        mostrarVenta(v);
    }
    fclose(f);
}