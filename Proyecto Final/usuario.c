#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "usuario.h"
#include "auto_cliente.h"
#include "autos_disponibles.h"
#include "cliente.h"
#include "pagos.h"
#include "auto.h"
#include "fecha.h"

#define ARCHIVO_USUARIOS "usuarios.bin"

// Funciones auxiliares
int es_contrasenia_segura(char pass[])
{
    if(strlen(pass)<4)
    {
        return 0;
    }
    int l=0, n=0;
    for(int i=0; i < strlen(pass); i++)
    {
        if(isalpha(pass[i]))
        {
            l=1;
        }
        if(isdigit(pass[i]))
        {
            n=1;
        }
    }
    return (l && n);
}
int es_correo_valido(char email[])
{
    for(int i=0; i<strlen(email); i++)
    {
        if(email[i]=='@')
        {
            return 1;
        }
    }
    return 0;
}
int usuario_Existente(char correo[])
{
    FILE* f=fopen(ARCHIVO_USUARIOS,"rb");
    if(f == NULL)
    {
        return 0;
    }
    stUsuario u;
    while(fread(&u,sizeof(stUsuario),1,f))
    {
        if(strcmp(u.correo,correo)==0)
        {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

// --- FUNCION PARA RECUPERAR DNI POR CORREO ---
void obtener_dni_por_correo(char correo[], char dniDestino[])
{
    FILE *file = fopen(ARCHIVO_USUARIOS, "rb");
    strcpy(dniDestino, "0"); // Por defecto
    if(file == NULL)
    {
        return;
    }

    stUsuario u;
    while(fread(&u, sizeof(stUsuario), 1, file))
    {
        if(strcmp(u.correo, correo) == 0)
        {
            strcpy(dniDestino, u.dni);
            break;
        }
    }
    fclose(file);
}

// --- AQUI ESTA TU FUNCION MODIFICADA ---
stUsuario registro_Usuario()
{
    stUsuario nuevo;
    int correoValido = 0;
    printf("\n--- REGISTRO DE CLIENTE ---\n");
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
            printf("Error: Falta '@'.\n");
        }
    }
    while(correoValido == 0);

    if(usuario_Existente(nuevo.correo))
    {
        printf("Usuario ya registrado.\n");
        strcpy(nuevo.dni, "-1");
        return nuevo;
    }

    int passValida = 0;
    do
    {
        printf("Ingrese su contrasenia (Min 4 chars, letras y numeros): ");
        fflush(stdin);
        scanf("%s", nuevo.contrasena);
        if(es_contrasenia_segura(nuevo.contrasena))
        {
            passValida = 1;
        }
        else
        {
            printf("Error: Contrasenia insegura.\n");
        }
    }
    while(passValida == 0);

    int dniValido = 0;
    while (dniValido == 0)
    {
        printf("Ingrese DNI (solo numeros): ");
        fflush(stdin);
        scanf("%s", nuevo.dni);
        dniValido = 1;

        for (int i = 0; i < strlen(nuevo.dni); i++)
        {
            if (!isdigit(nuevo.dni[i]))
            {
                dniValido = 0;
            }
        }
        if(dniValido && (strlen(nuevo.dni)<7 || strlen(nuevo.dni)>8))
        {
            dniValido = 0;
        }
        else if (dniValido && dni_Existente_usuario(nuevo.dni))
        {
            printf("DNI ya existe.\n");
            dniValido = 0;
        }
        if(!dniValido)
        {
            printf("Error en DNI.\n");
        }
    }

    // --- CAMBIO AQUI: LLAMADA A LA FUNCION ESTRICTA ---
    cargar_fecha_estricta(&nuevo.dia, &nuevo.mes, &nuevo.anios);

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
    printf("Correo: ");
    fflush(stdin);
    scanf("%s", correo);
    printf("Contrasena: ");
    fflush(stdin);
    scanf("%s", contrasena);
    system("cls");

    if(verificar_Usuario(correo, contrasena))
    {
        char dniLogueado[30];
        obtener_dni_por_correo(correo, dniLogueado);

        int opcion_sesion = -1;
        do
        {
            printf("Inicio de sesion exitoso - Rol: CLIENTE");
            printf("\n====================================================\n");
            printf("          PANEL DE CONTROL: CLIENTE      \n");
            printf("====================================================\n");
            printf("--- MENU DE CLIENTE ---\n");
            printf("1. Cargar mi Auto (Registrar vehiculo)\n");
            printf("2. Autos disponibles (Ver Stock)\n");
            printf("3. Pagos (Vender Auto)\n");
            printf("4. Volver al inicio\n");
            printf("0. Salir\n");
            printf("====================================================\n");
            printf("\nElija una opcion: ");

            fflush(stdin);
            scanf("%d", &opcion_sesion);
            system("cls");

            switch(opcion_sesion)
            {
            case 0:
                printf("Saliendo...\n");
                return;
            case 1:
                agregar_autos_cliente();
                break;
            case 2:
                mostrar_todos_autos_disponibles();
                break;

            case 3:
                gestionDePagos(dniLogueado);
                break;

            case 4:
                printf("Volviendo al inicio...\n");
                opcion_sesion = 0;
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
        system("pause");
    }
}

void mostrarTodosLosUsuarios()
{
    stUsuario u[100];
    int c = cargarUsuariosEnArreglo(u, 50);
    if(c==0) return;
    mostrarUsuariosRecursivo(u, 0, c);
}

int cargarUsuariosEnArreglo(stUsuario arr[], int tope)
{
    FILE* f = fopen(ARCHIVO_USUARIOS, "rb");
    if(!f) return 0;
    int i = 0;
    while(i < tope && fread(&arr[i], sizeof(stUsuario), 1, f) == 1) i++;
    fclose(f);
    return i;
}

void mostrarUsuariosRecursivo(stUsuario arr[], int pos, int total)
{
    if(pos == total) return;
    printf("Correo: %s | DNI: %s\n", arr[pos].correo, arr[pos].dni);
    mostrarUsuariosRecursivo(arr, pos + 1, total);
}

int dni_Existente_usuario(char dni[])
{
    FILE *file = fopen(ARCHIVO_USUARIOS, "rb");
    if(file == NULL) return 0;
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
