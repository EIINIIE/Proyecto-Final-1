#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "fecha.h"

// --- FUNCION 1: Mostrar fecha ---
void mostrar_Fecha(Fecha f)
{
    printf("%d/%d/%d\n", f.dia, f.mes, f.anio);
}

// --- FUNCION 2: Obtener fecha de hoy ---
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

// --- FUNCION 3: Carga Estricta (SOLUCION PUNTO 1) ---
void cargar_fecha_estricta(int *dia, int *mes, int *anio)
{
    int d, m, a;
    char auxAnio[50];
    int valido = 0;

    printf("\n-- FECHA DE NACIMIENTO --\n");

    // 1. MES (1 a 12)
    do {
        printf("Mes (1-12): ");
        fflush(stdin);
        // Validamos que ingrese un numero
        if(scanf("%d", &m) != 1) {
            m = 0; // Forzamos invalidez
            while(getchar() != '\n'); // Limpiamos buffer
        }

        if(m < 1 || m > 12) {
            printf("Error: El mes debe ser entre 1 y 12.\n");
        }
    } while(m < 1 || m > 12);

    // 2. AÑO (Estricto 4 digitos y rango 1928-2025)
    do {
        printf("Anio (Ej: 1999): ");
        fflush(stdin);
        scanf("%s", auxAnio);

        a = atoi(auxAnio);

        // AQUI EL CAMBIO: Rango 1928 a 2025 y longitud exacta 4
        if(strlen(auxAnio) == 4 && a >= 1928 && a <= 2025) {
            valido = 1;
        } else {
            printf("[ERROR] Ingrese un anio de 4 digitos valido (1928-2025).\n");
            valido = 0;
        }
    } while(valido == 0);

    // 3. DIA (Segun el mes)
    int maxDias = 31;
    if(m == 4 || m == 6 || m == 9 || m == 11) maxDias = 30;
    if(m == 2) {
        // Calculo de bisiesto para febrero
        if((a % 4 == 0 && a % 100 != 0) || (a % 400 == 0))
            maxDias = 29;
        else
            maxDias = 28;
    }

    do {
        printf("Dia (1-%d): ", maxDias);
        fflush(stdin);
        if(scanf("%d", &d) != 1) {
            d = 0;
            while(getchar() != '\n');
        }
        if(d < 1 || d > maxDias) {
            printf("Error: Dia invalido para el mes seleccionado.\n");
        }
    } while(d < 1 || d > maxDias);

    // Guardamos en los punteros para devolver los valores
    *dia = d;
    *mes = m;
    *anio = a;
}
