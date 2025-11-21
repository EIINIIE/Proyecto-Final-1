#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LOING.h"
#include "usuario.h"
#include "empleado.h"
#include "gerente.h"

void menu_login()
{
    int opcion_login;

    do
    {
        printf("\n=====================================\n");
        printf("            MENU LOGIN\n");
        printf("=====================================\n");
        printf("1. Empresa (Planta)\n");
        printf("2. Cliente (Usuarios)\n");
        printf("3. Administrador (Gerencia)\n");
        printf("0. Salir del sistema\n");
        printf("-------------------------------------\n");

        printf("\nElija una opcion: ");
        scanf("%d", &opcion_login);
        getchar();
        system("cls");

        switch (opcion_login)
        {
        case 1:
            login_empresa();
            break;

        case 2:
        {
            int opcion_cliente;
            do
            {
                printf("\n---------- ACCESO CLIENTE ----------\n");
                printf("1. Iniciar sesion\n");
                printf("2. Registrarse\n");
                printf("0. Volver atras\n");
                printf("------------------------------------\n");
                printf("Opcion: ");
                scanf("%d", &opcion_cliente);
                getchar();
                system("cls");

                switch(opcion_cliente)
                {
                case 1:
                    iniciarSesion();
                    break;
                case 2:
                {
                    stUsuario n = registro_Usuario();
                    if(n.dni != -1)
                    {
                        guardar_Usuario(n);
                        printf("Registrado. Inicie sesion.\n");
                    }
                    break;
                }
                case 0:
                    break;
                default:
                    printf("Opcion invalida\n");
                }
                if(opcion_cliente!=0)
                {
                    system("pause");
                    system("cls");
                }
            }
            while(opcion_cliente != 0);
        }
        break;

        case 3: // ADMINISTRADOR
            login_administrador();
            break;

        case 0:
            printf("Saliendo...\n");
            break;

        default:
            printf("Opcion no valida.\n");
            break;
        }

        if(opcion_login != 0)
        {
            system("pause");
            system("cls");
        }

    }
    while (opcion_login != 0);
}


void login_administrador()
{
    char correo[50];
    char contrasena[50];

    printf("--- ACCESO ADMINISTRADOR ---\n");
    printf("Correo: ");
    fflush(stdin);
    gets(correo);

    printf("Contrasena: ");
    fflush(stdin);
    gets(contrasena);

    system("cls");

    // Verifica credenciales maestras
    if (strcmp(correo, "admin@gmail.com") == 0 && strcmp(contrasena, "admin101") == 0)
    {
        printf("Inicio de sesion exitoso - Rol: ADMINISTRADOR\n");
        menu_gerente(); // Va a gerente.c
    }
    else
    {
        printf("Credenciales incorrectas.\n");
    }
}

/// --- LOGIN DE EMPLEADO (Opci n 1 - DIRECTO) ---
void login_empresa()
{
    char correo[50];
    char contrasena[50];

    printf("--- INICIAR SESION EMPRESA ---\n");
    printf("Correo: ");
    fflush(stdin);
    gets(correo);

    printf("Contrasena: ");
    fflush(stdin);
    gets(contrasena);

    system("cls");

    FILE* file = fopen("empleados.bin", "rb");
    if (file == NULL)
    {
        printf("No se pudo abrir el archivo de empleados o no hay empleados registrados.\n");
        return;
    }

    stEmpleado emple;
    int encontrado = 0;

    while (fread(&emple, sizeof(stEmpleado), 1, file) == 1)
    {
        if (strcmp(correo, emple.correo) == 0 && strcmp(contrasena, emple.contrasena) == 0)
        {
            encontrado = 1;

            /// Si es un administrador intentando entrar como empleado, lo redirigimos
            if (strcmp(emple.rol, "administrador") == 0)
            {
                printf("Detectado rol Administrador. Por favor ingrese por la opcion 3.\n");
            }
            else
            {
                printf("Bienvenido, %s (Rol: %s)\n", emple.correo, emple.rol);
               /// LLAMADA AL MENU DIRECTO (Sin pedir clave otra vez)
                menu_empleado_directo();
            }
            break;
        }
    }

    fclose(file);

    if (encontrado == 0)
    {
        printf("Correo o contrasena incorrectos.\n");
    }
}
