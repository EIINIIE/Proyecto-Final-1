#include <stdio.h>
#include <string.h>
#include "venta.h"
#include "auto.h"
#include "cliente.h"
#include "fecha.h"

#define ARCHIVO_VENTAS "ventas.bin"
#define ARCHIVO_AUTOS "autos.bin"
#define ARCHIVO_CLIENTES "clientes.bin" // Agregado para consistencia

Venta cargarVenta()
{
    Venta v;
    Auto a;
    Cliente c;
    int encontradoAuto = 0;
    int encontradoCliente = 0;

    printf("---- CARGA DE VENTA ----\n");
    v.fecha = cargar_Fecha();

    // 1. Buscar auto en stock (Validamos que sea de la Empresa)
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
                // CORRECCION: Coincidir con lo que carga auto.c ("Empresa")
                // Se agrega strcasecmp o comparaciones multiples por seguridad
                if(strcmp(a.patente, v.patenteAutoVendido) == 0 &&
                   (strcmp(a.titular.rol, "Empresa") == 0 || strcmp(a.titular.rol, "concesionaria") == 0))
                {
                    encontradoAuto = 1;
                    break;
                }
            }
            fclose(fAuto);
        }

        if(!encontradoAuto)
            printf("Error: Auto no encontrado o ya fue vendido (No pertenece a la Empresa).\n");

    } while(!encontradoAuto);

    printf("Vehiculo: %s %s | Costo Base: $%.2f\n", a.marca, a.modelo, a.precioDeAdquisicion);
    printf("Ingrese precio final de venta: ");
    scanf("%f", &v.precioVenta);

    v.ganancia = v.precioVenta - a.precioDeAdquisicion;

    // 2. Buscar cliente comprador
    do
    {
        printf("Ingrese DNI del comprador: ");
        scanf("%s", v.dniComprador);

        FILE *fCli = fopen(ARCHIVO_CLIENTES, "rb");
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
            printf("Cliente no encontrado. Registrelo antes de vender.\n");

    } while(!encontradoCliente);

    printf("Ingrese DNI del vendedor (Empleado): ");
    scanf("%s", v.dniVendedor);

    return v;
}

void transferirAutoAlComprador(char patente[], char dniComprador[])
{
    FILE *f = fopen(ARCHIVO_AUTOS, "r+b"); // Lectura y Escritura
    if(f == NULL) return;

    Auto a;
    while(fread(&a, sizeof(Auto), 1, f) == 1)
    {
        if(strcmp(a.patente, patente) == 0)
        {
            // Buscamos datos del comprador para actualizar el titular
            Cliente comprador;
            FILE *fCli = fopen(ARCHIVO_CLIENTES, "rb");
            if(fCli != NULL)
            {
                int foundCli = 0;
                while(fread(&comprador, sizeof(Cliente), 1, fCli) == 1)
                {
                    if(strcmp(comprador.dni, dniComprador) == 0)
                    {
                        a.titular = comprador; // Transferencia
                        foundCli = 1;
                        break;
                    }
                }
                fclose(fCli);

                if(foundCli)
                {
                    fseek(f, -sizeof(Auto), SEEK_CUR);
                    fwrite(&a, sizeof(Auto), 1, f);
                    printf(" Transferencia de titular realizada con exito.\n");
                }
            }
            break;
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

        // Actualizamos el archivo de autos (Punto 3j)
        transferirAutoAlComprador(v.patenteAutoVendido, v.dniComprador);

        printf("\n===================================\n");
        printf("   VENTA REGISTRADA EXITOSAMENTE   \n");
        printf("   Ganancia: $%.2f\n", v.ganancia);
        printf("===================================\n");
    }
    else
    {
        printf("Error al abrir archivo de ventas.\n");
    }
}

void mostrarVenta(Venta v)
{
    printf("\nFecha: %d/%d/%d\n", v.fecha.dia, v.fecha.mes, v.fecha.anio);
    printf("Patente: %s\n", v.patenteAutoVendido);
    printf("Precio Venta: $%.2f\n", v.precioVenta);
    printf("Ganancia: $%.2f\n", v.ganancia);
    printf("Comprador (DNI): %s\n", v.dniComprador);
    printf("Vendedor (DNI): %s\n", v.dniVendedor);
    printf("----------------------------------\n");
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
