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

// --- FUNCIONES PRINCIPALES ---
Auto cargar_auto();
void agregar_auto_stock();
void modificar_auto_stock();
void mostrar_auto(Auto a);
void mostrar_todos_autos(char archivo[]);
// --- FUNCIONES DE VALIDACION (Globales) ---
int ingresar_entero(char mensaje[]);
float ingresar_float(char mensaje[]);
int es_marca_valida(char m[]);
// El nombre debe coincidir con el de auto.c
int es_modelo_valido(char marca[], char modelo[]);
int existe_patente_en_archivo(char patente[]);

#endif // AUTO_H_INCLUDED
