#ifndef PAGOS_H_INCLUDED
#define PAGOS_H_INCLUDED

#include "auto.h"

void transferir_auto_a_cliente(Auto autoVendido, char dniComprador[]);
void eliminar_auto_stock(char patenteEliminar[]);
void registrar_venta_archivo(Auto autoVendido, char dniComprador[], char dniVendedor[]);
void ordenarPorPatente(Auto autos[], int validos);
int buscarPatenteBinaria(Auto autos[], int validos, char patenteBuscada[]);
void gestionDePagos();

#endif // PAGOS_H_INCLUDED
