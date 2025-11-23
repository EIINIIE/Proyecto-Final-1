#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "usuario.h"
#include "auto_cliente.h"
#include "autos_disponibles.h"
#include "cliente.h"
#include "pagos.h"
#include "auto.h" // Necesario para usar ingresar_entero

#define ARCHIVO_USUARIOS "usuarios.bin"

// --- VALIDACION SIMPLE DE CORREO ---
int es_correo_valido(char email[])
{
    int i;
    for(i=0; i<strlen(email); i++)
    {
        if(email[i] == '@')
        {
            return 1; // Debe tener al menos un arroba
        }
    }
    return 0;
}

int usuario_Existente(char correo[])
{
    FILE* file = fopen(ARCHIVO_USUARIOS, "rb");
    if(file == NULL)
    {
        return 0;
    }

    stUsuario usuario;
    while(fread(&usuario, sizeof(stUsuario), 1, file) == 1)
    {
        if(strcmp(usuario.correo, correo) == 0)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

stUsuario registro_Usuario()
{
    stUsuario nuevo;
    int correoValido = 0;

    printf("\n--- REGISTRO DE USUARIO ---\n");

    // 1. Validacion de Correo
    do
    {
        printf("Ingrese su correo: ");
        fflush(stdin);
        scanf("%s", nuevo.correo);

        if(es_correo_valido(nuevo.correo))
        {
            correoValido = 1;
        }
        else
        {
            printf("Error: El correo debe contener '@'. Intente nuevamente.\n");
        }
    }
    while(correoValido == 0);

    if(usuario_Existente(nuevo.correo))
    {
        printf("Este usuario ya esta registrado.\n");
        strcpy(nuevo.dni, "-1");
        return nuevo;
    }

    printf("Ingrese su contrasenia: ");
    fflush(stdin);
    scanf("%s", nuevo.contrasena);

    // 2. Validacion de DNI
    int dniValido = 0;
    while (dniValido == 0)
    {
        printf("Ingrese DNI (solo numeros): ");
        fflush(stdin);
        scanf("%s", nuevo.dni);

        dniValido = 1;
        // Validar que sean numeros
        for (int i = 0; i < strlen(nuevo.dni); i++)
        {
            if (nuevo.dni[i] < '0' || nuevo.dni[i] > '9')
            {
                dniValido = 0;
                break;
            }
        }

        if (dniValido == 0)
        {
            printf("Error: El DNI solo puede contener numeros.\n");
        }
        else if (strlen(nuevo.dni) < 7 || strlen(nuevo.dni) > 8)
        {
            dniValido = 0;
            printf("Error: El DNI debe tener 7 u 8 digitos.\n");
        }
        else if (dni_Existente_usuario(nuevo.dni))
        {
            dniValido = 0;
            printf("Error: El DNI ya existe.\n");
        }
    }

    // 3. Validacion Fecha (Usando ingresar_entero para que no explote con letras)
    printf("\n-- Fecha de Nacimiento --\n");

    int diaOk = 0;
    do
    {
        nuevo.dia = ingresar_entero("Dia (1-31): ");
        if(nuevo.dia >= 1 && nuevo.dia <= 31)
        {
            diaOk = 1;
        }
        else printf("Dia invalido.\n");
    }
    while(!diaOk);

    int mesOk = 0;
    do
    {
        nuevo.mes = ingresar_entero("Mes (1-12): ");
        if(nuevo.mes >= 1 && nuevo.mes <= 12)
        {
            mesOk = 1;
        }
        else
        {
            printf("Mes invalido.\n");
        }
    }
    while(!mesOk);

    int anioOk = 0;
    do
    {
        nuevo.anios = ingresar_entero("Anio (1900-2024): ");
        if(nuevo.anios >= 1900 && nuevo.anios <= 2024) anioOk = 1;
        else printf("Anio invalido.\n");
    }
    while(!anioOk);

    printf("\nUsuario registrado con exito!\n");
    return nuevo;
}

void guardar_Usuario(stUsuario usuario)
{
    if (strcmp(usuario.dni, "-1") == 0)
    {
        return;
    }

    FILE *file = fopen(ARCHIVO_USUARIOS, "ab");
    if (file == NULL)
    {
        printf("Error al guardar usuario.\n");
        return;
    }
    fwrite(&usuario, sizeof(stUsuario), 1, file);
    fclose(file);
}

int verificar_Usuario(char correo[], char contrasena[])
{
    FILE *file = fopen(ARCHIVO_USUARIOS, "rb");
    if(file == NULL)
    {
        return 0;
    }

    stUsuario usuario;
    int encontrado = 0;

    while(fread(&usuario, sizeof(stUsuario), 1, file) == 1)
    {
        if(strcmp(usuario.correo, correo) == 0 && strcmp(usuario.contrasena, contrasena) == 0)
        {
            encontrado = 1;
            break;
        }
    }
    fclose(file);
    return encontrado;
}

void iniciarSesion()
{
    char correo[50], contrasena[50];
    printf("\n------ INICIAR SESION ------\n");

    // Usamos fflush + gets para limpiar buffers previos
    printf("Correo: ");
    fflush(stdin);
    gets(correo);

    printf("Contrasena: ");
    fflush(stdin);
    gets(contrasena);

    system("cls");

    if(verificar_Usuario(correo, contrasena))
    {
        printf("\nSesion iniciada correctamente \n\n");

        int opcion_sesion = -1; // Iniciamos en valor seguro

        do
        {
            printf("-------------------------------------\n");
            printf("1. Datos del cliente (Cargar y Guardar)\n");
            printf("2. Dato del auto del cliente (Cargar)\n");
            printf("3. Autos disponibles (Ver Stock)\n");
            printf("4. Pagos (Vender Auto)\n");
            printf("5. Volver al inicio\n");
            printf("0. Salir\n");
            printf("-------------------------------------\n");
            printf("\nElija una opcion: ");

            fflush(stdin);
            scanf("%d", &opcion_sesion);

            system("cls");

            switch(opcion_sesion)
            {
            case 0:
                printf("Saliendo...\n");
                return; // Sale de la funcion

            case 1:
            {
                // AHORA SI SE GUARDA EL CLIENTE
                printf("\n--- CARGA DE DATOS PERSONALES ---\n");
                Cliente c = cargar_persona();
                guardar_cliente_en_archivo(c);
            }
            break;

            case 2:
                agregar_autos_cliente();
                break;

            case 3:
                mostrar_todos_autos_disponibles();
                break;

            case 4:
                gestionDePagos();
                break;

            case 5:
                printf("Volviendo al inicio...\n");
                opcion_sesion = 0; // Fuerza salida del bucle
                break;

            default:
                printf("Opcion no valida\n");
                break;
            }

            if(opcion_sesion != 0)
            {
                system("pause");
                system("cls");
            }
        }
        while(opcion_sesion != 0);
    }
    else
    {
        printf("\nCorreo o contrasenia incorrectos.\n");
        system("pause"); // Pausa para que leas el error antes de salir
    }
}

// Funciones auxiliares restantes (no cambiaron)
void mostrarTodosLosUsuarios()
{
    stUsuario usuarios[100];
    int cantidad = cargarUsuariosEnArreglo(usuarios, 50);
    if(cantidad == 0)
    {
        printf("No hay usuarios.\n");
        return;
    }
    printf("\n--- LISTA DE USUARIOS ---\n");
    mostrarUsuariosRecursivo(usuarios, 0, cantidad);
}

int cargarUsuariosEnArreglo(stUsuario arr[], int tope)
{
    FILE* file = fopen(ARCHIVO_USUARIOS, "rb");
    if(file == NULL)
    {
        return 0;
    }
    int i = 0;
    while(i < tope && fread(&arr[i], sizeof(stUsuario), 1, file) == 1)
    {
        i++;
    }
    fclose(file);
    return i;
}

void mostrarUsuariosRecursivo(stUsuario arr[], int pos, int total)
{
    if(pos == total)
    {
        return;
    }
    printf("Correo: %s | DNI: %s\n", arr[pos].correo, arr[pos].dni);
    mostrarUsuariosRecursivo(arr, pos + 1, total);
}

int dni_Existente_usuario(char dni[])
{
    FILE *file = fopen(ARCHIVO_USUARIOS, "rb");
    if(file == NULL)
    {
        return 0;
    }
    stUsuario aux;
    while (fread(&aux, sizeof (stUsuario), 1, file))
    {
        if (strcmp (aux.dni, dni) == 0)
        {
            fclose (file);
            return 1;
        }
    }
    fclose (file);
    return 0;
}
