#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "reportes.h"
#include "auto.h"
#include "venta.h"
#include "cliente.h"
#include "fecha.h"

#define ARCHIVO_VENTAS "ventas.bin"
#define ARCHIVO_AUTOS "autos.bin"
#define ARCHIVO_CLIENTES "clientes.bin"

/// ===============================================
/// MENU PRINCIPAL DE REPORTES
/// ===============================================
void menu_reportes()
{
    int opcion;
    do
    {
        system("cls");
        printf("\n========== MENU DE REPORTES ==========\n");
        printf("1. Recaudacion mensual\n");
        printf("2. Venta con mayor ganancia\n");
        printf("3. Autos con menos de 10 anios (stock joven)\n");
        printf("4. Listado de personas\n");
        printf("5. Buscar persona por DNI\n");
        printf("6. Buscar auto por patente\n");
        printf("7. Ver detalle de una venta\n");
        printf("8. Alerta de autos con venta lenta\n");
        printf("0. Volver\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        system("cls");

        switch(opcion)
        {
        case 1:
            recaudacion_mensual();
            break;
        case 2:
            venta_mayor_ganancia();
            break;
        case 3:
            stock_joven();
            break;
        case 4:
            ver_listado_personas();
            break;
        case 5:
            buscar_persona_dni();
            break;
        case 6:
            buscar_auto_patente();
            break;
        case 7:
            ver_venta_detalle();
            break;
        case 8:
            alerta_stock_lento();
            break;
        case 0:
            printf("Volviendo al menu anterior...\n");
            break;
        default:
            printf("Opcion invalida.\n");
            break;
        }
        system("pause");
    }
    while(opcion != 0);
}

/// --- RECAUDACI N MENSUAL ---
void recaudacion_mensual()
{
    FILE* f = fopen(ARCHIVO_VENTAS, "rb");
    if(f == NULL)
    {
        printf("No hay ventas registradas.\n");
        return;
    }

    int mes, anio;
    float total = 0;
    Venta v;

    printf("Ingrese mes (1-12): ");
    scanf("%d", &mes);
    printf("Ingrese anio: ");
    scanf("%d", &anio);

    while(fread(&v, sizeof(Venta), 1, f) == 1)
    {
        if(v.fecha.mes == mes && v.fecha.anio == anio)
        {
            total += v.precioVenta;
        }
    }
    fclose(f);
    printf("\nRecaudacion del %02d/%d = $%.2f\n", mes, anio, total);
}

/// --- VENTA CON MAYOR GANANCIA ---
void venta_mayor_ganancia()
{
    FILE* f = fopen(ARCHIVO_VENTAS, "rb");
    if(f == NULL)
    {
        printf("No hay ventas cargadas.\n");
        return;
    }

    Venta v, max;
    int primero = 1;

    while(fread(&v, sizeof(Venta), 1, f) == 1)
    {
        if(primero || v.ganancia > max.ganancia)
        {
            max = v;
            primero = 0;
        }
    }
    fclose(f);
    if(!primero)
    {
        printf("\n--- Venta con mayor ganancia ---\n");
        mostrarVenta(max);
    }
    else
    {
        printf("No hay ventas registradas.\n");
    }
}

/// --- STOCK JOVEN (Autos < 10 a os) ---
void stock_joven()
{
    FILE *f = fopen(ARCHIVO_AUTOS, "rb");
    if(f == NULL)
    {
        printf("No hay autos cargados.\n");
        return;
    }

    Auto lista[100];
    Auto a;
    int validos = 0;
    int anioActual = 2025;


    while(fread(&a, sizeof(Auto), 1, f) == 1 && validos < 100)
    {
        if((anioActual - a.anio) < 10 && strcmp(a.titular.rol,"concesionaria")==0)
        {
            lista[validos] = a;
            validos++;
        }
    }
    fclose(f);


    Auto aux;
    for(int i=0; i < validos-1; i++)
        for(int j=i+1; j < validos; j++)
            if(lista[i].anio < lista[j].anio)
            {
                aux = lista[i];
                lista[i] = lista[j];
                lista[j] = aux;
            }

    printf("\n--- Autos con menos de 10 anios (Orden Descendente) ---\n");
    printf("%s %s %s\n", "ANIO", "MARCA", "MODELO");
    for(int i=0; i<validos; i++)
        printf("%d %s %s\n", lista[i].anio, lista[i].marca, lista[i].modelo);
}

/// --- LISTADO DE PERSONAS ---
void ver_listado_personas()
{
    FILE *f = fopen(ARCHIVO_CLIENTES, "rb");
    if(f == NULL)
    {
        printf("No hay clientes registrados.\n");
        return;
    }

    Cliente c;
    printf("\n--- LISTA DE PERSONAS ---\n");
    while(fread(&c, sizeof(Cliente), 1, f) == 1)
        printf("DNI: %s | Nombre: %s | Rol: %s\n", c.dni, c.nombre, c.rol);

    fclose(f);
}

/// --- BUSCAR PERSONA POR DNI ---
void buscar_persona_dni()
{
    FILE *f = fopen(ARCHIVO_CLIENTES, "rb");
    if(f == NULL)
    {
        printf("No hay archivo de clientes.\n");
        return;
    }

    char dniBuscado[20];
    Cliente c;
    int encontrado = 0;

    printf("Ingrese DNI a buscar: ");
    scanf("%s", dniBuscado);

    while(fread(&c, sizeof(Cliente), 1, f) == 1)
    {
        if(strcmp(c.dni, dniBuscado) == 0)
        {
            encontrado = 1;
            printf("\nCliente encontrado:\nNombre: %s\nTelefono: %s\nDireccion: %s\nRol: %s\n",
                   c.nombre, c.telefono, c.direccion, c.rol);
        }
    }
    fclose(f);
    if(encontrado == 0)
    {
        printf("No existe una persona con ese DNI.\n");
    }
}

/// --- BUSCAR AUTO POR PATENTE ---
void buscar_auto_patente()
{
    FILE *f = fopen(ARCHIVO_AUTOS, "rb");
    if(f == NULL)
    {
        printf("No hay autos registrados.\n");
        return;
    }

    char pat[20];
    Auto a;
    int ok = 0;
    printf("Ingrese patente: ");
    scanf("%s", pat);

    while(fread(&a, sizeof(Auto), 1, f) == 1)
    {
        if(strcmp(a.patente, pat) == 0)
        {
            ok = 1;
            mostrar_auto(a);
        }
    }
    fclose(f);
    if(ok == 0)
    {
        printf("No existe un auto con esa patente.\n");
    }
}

/// --- VER DETALLE DE VENTA ---
void ver_venta_detalle()
{
    FILE *f = fopen(ARCHIVO_VENTAS, "rb");
    if(f == NULL)
    {
        printf("No hay ventas registradas.\n");
        return;
    }

    char pat[20];
    Venta v;
    int flag = 0;
    printf("Ingrese patente vendida: ");
    scanf("%s", pat);

    while(fread(&v, sizeof(Venta), 1, f) == 1)
    {
        if(strcmp(v.patenteAutoVendido, pat) == 0)
        {
            flag = 1;
            mostrarVenta(v);
        }
    }
    fclose(f);
    if(flag == 0)
    {
        printf("No existe una venta con esa patente.\n");
    }
}

/// --- ALERTA DE AUTOS CON VENTA LENTA ---
void alerta_stock_lento()
{
    FILE *fA = fopen(ARCHIVO_AUTOS, "rb");
    FILE *fV = fopen(ARCHIVO_VENTAS, "rb");

    if(fA == NULL)
    {
        printf("No hay autos cargados.\n");
        return;
    }

    Auto a;
    Venta v;
    int vendido;

    printf("\n--- Autos que aun NO se vendieron ---\n");

    while(fread(&a, sizeof(Auto), 1, fA) == 1)
    {
        vendido = 0;
        if (fV != NULL)
        {
            rewind(fV);
            while(fread(&v, sizeof(Venta), 1, fV) == 1)
            {
                if(strcmp(a.patente, v.patenteAutoVendido) == 0)
                {
                    vendido = 1;
                    break;
                }
            }
        }
        if(vendido == 0)
        {
            mostrar_auto(a);
        }
    }

    fclose(fA);
    if(fV != NULL)
    {
        fclose(fV);
    }
}
