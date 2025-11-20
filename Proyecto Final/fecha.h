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
Fecha cargar_Fecha();
void mostrar_Fecha(Fecha f);
#endif // FECHA_H_INCLUDED
