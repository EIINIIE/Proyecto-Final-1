#ifndef AUTO_CLIENTE_H_INCLUDED
#define AUTO_CLIENTE_H_INCLUDED

#include "cliente.h"

#define ARCHIVO_AUTOS_CLIENTE "autos_cliente.bin"

// --- CAMBIO CLAVE: Renombramos a AutoCliente ---
typedef struct
{
    char patente[11];
    char marca[20];
    char modelo[20];
    int anio;
    int kms;
    Cliente titular;
    float precioDeAdquisicion;
    float precioFinal;
} AutoCliente;

// ------- Prototipos -------
AutoCliente cargar_auto_cliente();
void agregar_autos_cliente();
void mostrar_auto_cliente(AutoCliente autos);
void mostrar_todos_autos_cliente();
// Nueva funcion para el menu de empleado
void modificar_auto_cliente_por_dni(char dniBuscado[]); /// flata crear la funcion y llamarlo
int cargar_autos_cliente_din(AutoCliente **listaAutos); /// ver en donde llamarlo

#endif // AUTO_CLIENTE_H_INCLUDED
