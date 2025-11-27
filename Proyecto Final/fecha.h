#ifndef FECHA_H_INCLUDED
#define FECHA_H_INCLUDED

typedef struct stFecha
{
    int dia;
    int mes;
    int anio;
} Fecha;

void mostrar_Fecha(Fecha f);
Fecha hoy();
// --- FUNCION DE VALIDACION ESTRICTA ---
// Carga completa de una fecha (Dia/Mes/Anio) con validacion 1928-2025
void cargar_fecha_estricta(int *dia, int *mes, int *anio);

#endif // FECHA_H_INCLUDED
