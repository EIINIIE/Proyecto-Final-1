#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "empleado.h"
#include "auto.h"
#include "auto_cliente.h"
#include "autos_disponibles.h"
#include "cliente.h"
#include "pagos.h"

// Esta función se llama DESPUÉS de validar usuario/pass en LOING.c
// Por eso entra directo al menú sin pedir credenciales de nuevo.
void menu_empleado_directo()
{
    int opcion_sesion;

    printf("\n====================================================\n");
    printf("          SESION INICIADA: EMPLEADO DE PLANTA       \n");
    printf("====================================================\n");

    do
    {
        printf("\n--- MENU DE EMPLEADO ---\n");
        printf("1. Datos del cliente (Cargar)\n");
        printf("2. Dato del auto del cliente (Cargar)\n");
        printf("3. Autos disponibles (Ver Stock)\n");
        printf("4. Pagos (Vender Auto)\n");
        printf("5. Volver al inicio\n");
        printf("0. Salir (Volver al Login)\n");
        printf("-------------------------------------\n");

        printf("Elija una opcion: ");
        scanf("%d", &opcion_sesion);
        system("cls");

        switch(opcion_sesion)
        {
        case 1:
            printf("\n--- CARGA DE CLIENTE ---\n");
            Cliente nuevoC = cargar_persona();
            guardar_cliente_en_archivo(nuevoC);
            break;

        case 2:
            printf("\n--- CARGA DE AUTO DE CLIENTE ---\n");
            agregar_autos_cliente();
            break;

        case 3:
            mostrar_todos_autos_disponibles();
            break;

        case 4:
            gestionDePagos();
            break;

        case 5:
            printf("Volviendo al menu principal...\n");
            opcion_sesion = 0; // Esto fuerza la salida del bucle
            break;

        case 0:
            printf("Cerrando sesion y volviendo al inicio...\n");
            // Regresa automaticamente a LOING.c
            break;

        default:
            printf("Opcion no valida. Intente nuevamente.\n");
            break;
        }

        // Pausa para ver los resultados, excepto si elige volver (5) o salir (0)
        if (opcion_sesion != 0)
        {
            system("pause");
            system("cls");
        }
    }
    while(opcion_sesion != 0);
}
