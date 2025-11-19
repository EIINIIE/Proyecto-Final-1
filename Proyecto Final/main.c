#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "usuario.h"
#include "cliente.h"
#include "auto.h"
#include "empleado.h"
#include "LOING.h"

// Función temporal para limpiar archivos corruptos
void reiniciar_sistema() {
    remove("clientes.bin");
    remove("autos.bin");
    remove("autos_cliente.bin");
    remove("ventas.bin");
    printf("\n[SISTEMA] Archivos reseteados correctamente. Inicie la carga nuevamente.\n");
    system("pause");
}

int main()
{
    srand(time(NULL));

     reiniciar_sistema();


    int opcion;

    do
    {
        printf("\n===================================================\n");
        printf("     SISTEMA DE GESTION INTEGRAL - RUEDA VELOZ \n");
        printf("===================================================\n\n");
        printf("1. INGRESAR AL SISTEMA (Login)\n");
        printf("0. Salir\n");
        printf("----------------------------------------------------\n");

        printf("\nElija una opcion: ");
        scanf("%d", &opcion);
        system("cls");

        switch(opcion)
        {
        case 0:
            printf("Gracias por usar el sistema. Adios.\n");
            break;

        case 1:
            menu_login();
            break;

        default:
            printf("Opcion no valida\n");
            system("pause");
            break;
        }

        system("cls");

    }
    while(opcion != 0);

    return 0;
}
