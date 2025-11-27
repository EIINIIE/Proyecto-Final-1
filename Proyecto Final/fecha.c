#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fecha.h"

// --- FUNCION 1 ---
void mostrar_Fecha(Fecha f)
{
    printf("%d/%d/%d\n", f.dia, f.mes, f.anio);
}

// --- FUNCION 2 ---
Fecha hoy()
{
    Fecha f;
    time_t t = time(NULL);
    struct tm *fechaLocal = localtime(&t);
    f.dia  = fechaLocal->tm_mday;
    f.mes  = fechaLocal->tm_mon + 1;
    f.anio = fechaLocal->tm_year + 1900;

    return f;
}

// --- FUNCION 3 ---
void cargar_fecha_estricta(int *dia, int *mes, int *anio)
{
    int d, m, a;
    char auxAnio[50];
    int validoAnio = 0;
    int fechaCorrecta = 0; // Bandera para validar la fecha completa al final

    printf("\n-- FECHA DE NACIMIENTO --\n");

    // Bucle principal: Se repite si la combinacion de fecha no existe (ej: 30 de Febrero)
    do
    {
        // 1. DIA (Validacion generica 1-31)
        do
        {
            printf("Dia (1-31): ");
            fflush(stdin);
            if(scanf("%d", &d) != 1)
            {
                d = 0;
                while(getchar() != '\n');
            }
            if(d < 1 || d > 31)
            {
                printf("Error: Ingrese un dia entre 1 y 31.\n");
            }
        }
        while(d < 1 || d > 31);

        // 2. MES (1-12)
        do
        {
            printf("Mes (1-12): ");
            fflush(stdin);
            if(scanf("%d", &m) != 1)
            {
                m = 0;
                while(getchar() != '\n');
            }
            if(m < 1 || m > 12)
            {
                printf("Error: El mes debe ser entre 1 y 12.\n");
            }
        }
        while(m < 1 || m > 12);

        // 3. AÑO (1928-2025)
        do
        {
            validoAnio = 0;
            printf("Anio (Ej: 1999): ");
            fflush(stdin);
            scanf("%s", auxAnio);

            a = atoi(auxAnio);

            if(strlen(auxAnio) == 4 && a >= 1928 && a <= 2025)
            {
                validoAnio = 1;
            }
            else
            {
                printf("[ERROR] Ingrese un anio valido (1928-2025).\n");
            }
        }
        while(validoAnio == 0);

        // --- VALIDACION FINAL DE COHERENCIA ---
        // Ahora que tenemos Dia, Mes y Anio, revisamos si la fecha existe.
        int maxDias = 31;
        if(m == 4 || m == 6 || m == 9 || m == 11)
        {
            maxDias = 30;
        }
        if(m == 2)
        {
            if((a % 4 == 0 && a % 100 != 0) || (a % 400 == 0))
            {
                maxDias = 29;
            }
            else
            {
                maxDias = 28;
            }
        }

        if (d > maxDias)
        {
            printf("\n[ERROR] Fecha invalida: El mes %d del anio %d solo tiene %d dias.\n", m, a, maxDias);
            printf("Por favor, ingrese la fecha nuevamente.\n\n");
            fechaCorrecta = 0;
        }
        else
        {
            fechaCorrecta = 1;
        }

    }
    while (fechaCorrecta == 0);

    // Asignamos los valores finales
    *dia = d;
    *mes = m;
    *anio = a;
}
