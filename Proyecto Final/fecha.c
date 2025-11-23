#include <stdio.h>
#include <string.h>
#include <time.h>

#include "fecha.h"

/// FUNCION 1
void mostrar_Fecha(Fecha f)
{
    printf("%d/%d/%d\n", f.dia, f.mes, f.anio);
}

/// FUNCION 2
Fecha hoy()
{
    Fecha f;
    time_t t = time(NULL);
    struct tm *fechaLocal = localtime(&t);

    f.dia  = fechaLocal->tm_mday;
    f.mes  = fechaLocal->tm_mon + 1;   // tm_mon va de 0 a 11
    f.anio = fechaLocal->tm_year + 1900;

    return f;
}


