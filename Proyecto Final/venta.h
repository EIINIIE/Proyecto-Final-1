#ifndef VENTA_H_INCLUDED
#define VENTA_H_INCLUDED
#include "fecha.h"

// ------- Estructura Venta -------
typedef struct stVenta
{
    Fecha fecha;
    char patenteAutoVendido[11];
    float precioVenta;
    float ganancia;
    char dniComprador[15];
    char dniVendedor[15];
} Venta;

// ------- Prototipos -------
Venta cargarVenta();
void registrarVenta();
void mostrarVenta(Venta ventas);
void mostrarVentas();

#endif // VENTA_H_INCLUDED
