#ifndef AUTO_H_INCLUDED
#define AUTO_H_INCLUDED

#include "cliente.h"

#define ARCHIVO_AUTOS "autos.bin"

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
} Auto;

Auto cargar_auto();
void agregar_autos();
void mostrar_auto(Auto autos);
void mostrar_todos_autos(char archivo[]);
float medioDPago(float precioDeAdquisicion);
void agregar_auto_stock();
void modificar_auto_stock(); // <--- NUEVO (Req 3a)

#endif // AUTO_H_INCLUDED
