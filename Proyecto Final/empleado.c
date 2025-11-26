#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "empleado.h"
#include "auto.h"
#include "auto_cliente.h"
#include "autos_disponibles.h"
#include "cliente.h"
#include "pagos.h"


void menu_empleado_directo()
{
    int opcion_sesion;

    printf("Inicio de sesion exitoso - Rol: EMPLEADO");
    printf("\n====================================================\n");
    printf("          PANEL DE CONTROL: EMPLEADO      \n");
    printf("====================================================");
    do
    {
        printf("\n--- MENU DE EMPLEADO ---\n");
        printf("1. Dato del auto del cliente (Cargar)\n");
        printf("2. Autos disponibles (Ver Stock)\n");
        printf("3. Pagos (Vender Auto)\n");
        printf("4. Volver al inicio\n");
        printf("0. Salir (Volver al Login)\n");
        printf("-------------------------------------\n");

        printf("Elija una opcion: ");
        scanf("%d", &opcion_sesion);
        system("cls");

        switch(opcion_sesion)
        {
        case 1:
            printf("\n--- CARGA DE AUTO DE CLIENTE ---\n");
            agregar_autos_cliente();
            break;

        case 2:
            mostrar_todos_autos_disponibles();
            break;

        case 3:
            gestionDePagos();
            break;

        case 4:
            printf("Volviendo al menu principal...\n");
            opcion_sesion = 0;
            break;

        case 0:
            printf("Cerrando sesion y volviendo al inicio...\n");

            break;

        default:
            printf("Opcion no valida. Intente nuevamente.\n");
            break;
        }

        if (opcion_sesion != 0)
        {
            system("pause");
            system("cls");
        }
    }
    while(opcion_sesion != 0);
}
