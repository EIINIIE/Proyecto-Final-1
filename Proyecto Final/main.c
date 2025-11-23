#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "usuario.h"
#include "cliente.h"
#include "auto.h"
#include "empleado.h"
#include "LOING.h"

int main()
{
    srand(time(NULL));
    int opcion;

    do
    {
        printf("\n===================================================\n");
        printf("     CONSESIONARIA DE AUTOS - UTN \n");
        printf("===================================================\n\n");
        printf("1. Ingresar Sistemas (Login)\n");
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
