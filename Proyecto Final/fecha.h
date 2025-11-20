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
void mostrar_Fecha(Fecha f);
Fecha hoy();

#endif // FECHA_H_INCLUDED
///Esto va al final porque #endif es el cierre de una condición
