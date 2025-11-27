#ifndef AUTO_CLIENTE_H_INCLUDED
#define AUTO_CLIENTE_H_INCLUDED

#include "cliente.h"

#define ARCHIVO_AUTOS_CLIENTE "autos_cliente.bin"

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

/// ------- Prototipos -------
AutoCliente cargar_auto_cliente();
void agregar_autos_cliente();
void mostrar_auto_cliente(AutoCliente autos);
void mostrar_todos_autos_cliente();
void modificar_auto_cliente_por_dni(char dniBuscado[]);
int cargar_autos_cliente_din(AutoCliente **listaAutos);
int ingresar_entero_auto_cliente(char mensaje[]);
float ingresar_float_auto_cliente(char mensaje[]);
int es_marca_valida_auto_cliente(char m[]);
int es_modelo_valido_auto_cliente(char marcaElegida[], char modeloIngresado[]);
void modificar_auto_stock_auto_cliente();
void agregar_auto_stock();
int existe_patente_en_archivo(char patenteBuscada[]);



#endif // AUTO_CLIENTE_H_INCLUDED
