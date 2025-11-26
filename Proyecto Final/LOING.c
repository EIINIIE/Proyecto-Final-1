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

        // LIMPIEZA: Borra el Enter del buffer
        fflush(stdin);

        system("cls");

        switch (opcion_login)
        {
        case 1:
            login_empresa();
            break;

        case 2:
        {
            // --- SUB-MENU PARA CLIENTES ---
            int subOp = 0;
            printf("\n=== ACCESO CLIENTES ===\n");
            printf("1. Iniciar Sesion\n");
            printf("2. Registrarse (Nuevo Cliente)\n");
            printf("0. Volver atras\n");
            printf("-----------------------\n");
            printf("Elija una opcion: ");
            scanf("%d", &subOp);

            fflush(stdin);
            system("cls");

            if (subOp == 1)
            {
                iniciarSesion();
            }
            else if (subOp == 2)
            {
                stUsuario u = registro_Usuario();

                if (strcmp(u.dni, "-1") != 0)
                {
                    guardar_Usuario(u);

                    Cliente c = obtener_datos_cliente(u.dni);

                    if(strcmp(c.dni, "0") == 0)
                    {
                        printf("\n--- COMPLETE SUS DATOS PERSONALES ---\n");
                        c = cargar_persona();
                        strcpy(c.dni, u.dni);
                        strcpy(c.rol, "usuario");
                        guardar_cliente_en_archivo(c);

                        printf("\nRegistro completado. Ahora puede Iniciar Sesion.\n");
                    }
                    else
                    {
                        printf("\nEl perfil de cliente ya existia. Usuario creado.\n");
                    }
                }
            }
            break;
        }

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

    system("cls");
    printf("--- ACCESO ADMINISTRADOR ---\n");

    // 1. Pedimos el correo con scanf
    printf("Correo: ");
    scanf("%s", correo);

    // 2. Pedimos la contraseña TAMBIÉN con scanf
    // Esto evita problemas de buffer basura y espacios fantasmas
    printf("Contrasena: ");
    scanf("%s", contrasena);


    // Verifica credenciales maestras
    if (strcmp(correo, "admin@gmail.com") == 0 && strcmp(contrasena, "admin101") == 0)
    {
        printf("\nCredenciales Correctas. Accediendo...\n");
        system("pause"); // Pausa para leer
        menu_gerente();
    }
    else
    {
        printf("\n[ERROR] Credenciales incorrectas.\n");
        printf("Asegurate de respetar mayusculas y minusculas.\n");
    }
}

void login_empresa()
{
    char correo[50];
    char contrasena[50];

    system("cls");
    printf("--- INICIAR SESION EMPRESA ---\n");

    // --- APLICAMOS LA MISMA CORRECCION POR SEGURIDAD ---
    printf("Correo: ");
    scanf("%s", correo);

    fflush(stdin); // Limpieza necesaria antes del gets

    printf("Contrasena: ");
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
            if (strcmp(emple.rol, "administrador") == 0)
            {
                printf("Detectado rol Administrador. Por favor ingrese por la opcion 3.\n");
            }
            else
            {
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
