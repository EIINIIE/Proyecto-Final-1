#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LOING.h"
#include "usuario.h"
#include "empleado.h"
#include "gerente.h"
#include "cliente.h"


void menu_login()
{
    int opcion_login;

    do
    {
        printf("\n=====================================\n");
        printf("            MENU LOGIN\n");
        printf("=====================================\n");
        printf("1. Empresa (Empleado)\n");
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
    // FUNCIONALIDAD DE REGISTRO EN LOING.c
    stUsuario u = registro_Usuario();
    guardar_Usuario(u);

    // VERIFICAR Y CARGAR DATOS ADICIONALES DEL CLIENTE
    Cliente c = obtener_datos_cliente(u.dni); // Busca si ya existe un registro de cliente con ese DNI

    if(strcmp(c.dni, "0") == 0) // Si no existe (dni = "0")
    {
        // Se llama a cargar_persona, que ahora solo pide Nombre, Telefono y Direccion.
        c = cargar_persona();

        // Asignamos el DNI y el rol obtenidos del registro de usuario (cumpliendo con "agregarla en el registrar")
        strcpy(c.dni, u.dni); // Se asigna el DNI del Usuario
        strcpy(c.rol, "usuario");
        guardar_cliente_en_archivo(c);
        printf("\nRegistro de Cliente completado con exito. Puede iniciar sesion.\n");
    }
    else
    {
         printf("\nEl registro de cliente ya existe. Puede iniciar sesion.\n");
    }
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
        printf("No se pudo abrir o no hay empleados registrados.\n");
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
