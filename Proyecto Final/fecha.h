#ifndef FECHA_H_INCLUDED
#define FECHA_H_INCLUDED

// ------- Estructura Fecha -------
typedef struct stFecha
{
    int dia;
    int mes;
    int anio;
} Fecha;

// ------- Prototipo -------
Fecha cargar_Fecha(); /// probar si funciona
void mostrar_Fecha(Fecha f); /// probar si funciona

#endif // FECHA_H_INCLUDED
///Esto va al final porque #endif es el cierre de una condición
