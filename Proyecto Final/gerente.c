#include <stdio.h>
#include <stdlib.h>
#include "gerente.h"
#include "cliente.h"
#include "empleado.h"
#include "auto.h"
#include "reportes.h"


#include "autos_disponibles.h"
#include "pagos.h"
#include "auto_cliente.h"
#include "venta.h"


//-----------------------------------------------------
// Menu principal del gerente
//-----------------------------------------------------
void menu_gerente()
{
    int opcion;
    do
    {
        printf("\n==================================================\n");
        printf("       SESION INICIADA: ADMINISTRADOR (GERENTE)     \n");
        printf("==================================================\n");
        printf("1. Gestion de Clientes (Agregar / Modificar)\n");
        printf("2. Autos de cliente (Cargar)\n");
        printf("3. Autos disponibles (Stock Empresa)\n");
        printf("4. Pagos (Vender Auto)\n");
        printf("5. Registrar empleado\n");
        printf("6. Eliminar empleado\n");
        printf("7. Cerrar Sesion (Volver al Login)\n");
        printf("8. Ventas (Ver historial)\n");
        printf("9. Ingresar nuevo auto (Cargar Stock)\n");
        printf("10. Ver lista de empleados\n");
        printf("11. Reportes\n");
        printf("0. Volver al inicio\n");
        printf("--------------------------------------------------\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        system("cls");

        switch (opcion)
        {
        case 1:
        {
            int subop;
            do
            {
                printf("\n--- GESTION DE CLIENTES ---\n");
                printf("1. Cargar NUEVO Cliente\n");
                printf("2. MODIFICAR Cliente Existente\n");
                printf("3. Ver listado de clientes\n");
                printf("0. Volver al menu anterior\n");
                printf("Elija opcion: ");
                scanf("%d", &subop);
                system("cls");

                switch(subop)
                {
                case 1:
                {
                    Cliente nuevoC = cargar_persona();
                    guardar_cliente_en_archivo(nuevoC);
                    break;
                }
                case 2:
                    modificar_cliente();
                    break;
                case 3:
                    ver_listado_clientes();
                    break;
                case 0:
                    break;
                default:
                    printf("Opcion invalida.\n");
                    break;
                }
                if(subop != 0)
                {
                    printf("\nPresione una tecla para continuar...");
                    getchar();
                    getchar();
                    system("cls");
                }
            }
            while(subop != 0);
            break;
        }

        case 2:
            agregar_autos_cliente();  // tu funci n para autos de clientes
            break;
        case 3:
            mostrar_todos_autos_disponibles();  // tu funci n stock
            break;
        case 4:
            gestionDePagos();  // tu funci n de pagos
            break;
        case 5:
            agregar_empleado();  // tu funci n de empleados
            break;
        case 6:
            eliminar_empleado();  // tu funci n eliminar empleado
            break;
        case 7:
            printf("Cerrando sesion...\n");
            return;
        case 8:
            mostrarVentas();  // tu funci n de ventas
            break;
        case 9:
            agregar_auto_stock();  // tu funci n stock de autos
            break;
        case 10:
            mostrar_empleados();  // tu funci n mostrar empleados
            break;
        case 11:
            menu_reportes();  // tu funci n reportes
            break;
        case 0:
            printf("Volviendo al menu principal...\n");
            break;
        default:
            printf("Opcion invalida. Intente nuevamente.\n");
            break;
        }

        if(opcion != 7 && opcion != 0)
        {
            printf("\nPresione una tecla para continuar...");
            getchar();
            getchar();  // Pausa simple
            system("cls");
        }

    }
    while(opcion != 7 && opcion != 0);
}

//-----------------------------------------------------
// FUNCIONES AUXILIARES
//-----------------------------------------------------
stGerente cargar_un_empleado()
{
    stGerente nuevo;
    printf("---- REGISTRO DE NUEVO EMPLEADO ----\n");
    printf("Ingrese correo: \n");
    scanf("%s", nuevo.correo);
    printf("Ingrese contrasena: \n");
    scanf("%s", nuevo.contrasena);
    printf("Ingrese DNI: \n");
    scanf("%d", &nuevo.dni);
    printf("Ingrese dia de nacimiento: \n");
    scanf("%d", &nuevo.dia);
    printf("Ingrese mes de nacimiento: \n");
    scanf("%d", &nuevo.mes);
    printf("Ingrese anio de nacimiento: \n");
    scanf("%d", &nuevo.anios);
    printf("Ingrese rol (ej: vendedor, mecanico, administrador): \n");
    scanf("%s", nuevo.rol);
    return nuevo;
}

void agregar_empleado()
{
    FILE* file = fopen("empleados.bin", "ab");
    if (file == NULL)
    {
        printf("Error al abrir archivo empleados.\n");
        return;
    }

    char control = 's';
    while (control == 's' || control == 'S')
    {
        stGerente nuevo = cargar_un_empleado();
        fwrite(&nuevo, sizeof(stGerente), 1, file);
        printf("Empleado registrado con exito.\n");
        printf("Desea registrar otro empleado? (s/n): ");
        fflush(stdin);
        scanf(" %c", &control);
    }
    fclose(file);
}

void eliminar_auto_stock(char patenteEliminar[])
{
    FILE *archivo = fopen("autos.bin", "rb");
    if (archivo == NULL)
    {
             printf("No hay autos en stock.\n"); return;
    }

    Auto lista[100]; // suponemos máximo 100 autos
    int cant = 0;
    Auto a;

    // Leer todos los autos
    while (fread(&a, sizeof(Auto), 1, archivo))
    {
        lista[cant++] = a;
    }

    fclose(archivo);

    int encontrado = 0;
    for (int i = 0; i < cant; i++)
    {
        if (strcmp(lista[i].patente, patenteEliminar) == 0)
        {
            encontrado = 1;
            // Mover los posteriores una posición atrás
            for (int j = i; j < cant-1; j++)
            {
                lista[j] = lista[j+1];
            }
            cant--;
            break;
        }
    }

    if (encontrado == 0)
    {
        printf("No se encontró el auto.\n");
        return;
    }

    // Reescribir el archivo con los autos restantes
    archivo = fopen("autos.bin", "wb");
    for (int i = 0; i < cant; i++)
    {
        fwrite(&lista[i], sizeof(Auto), 1, archivo);
    }

    fclose(archivo);
    printf("Auto eliminado del stock.\n");
}
void mostrar_empleados()
{
    FILE* file = fopen("empleados.bin", "rb");
    if (file == NULL)
    {
        printf("No hay empleados registrados aun.\n");
        return;
    }

    stGerente emple;
    int contador = 0;

    printf("\n-------- LISTA DE EMPLEADOS --------\n");
    printf("------------------------------------\n");

    while (fread(&emple, sizeof(stGerente), 1, file) == 1)
    {
        printf("Empleado #%d\n", ++contador);
        printf("DNI: %d\n", emple.dni);
        printf("Correo: %s\n", emple.correo);
        printf("Rol: %s\n", emple.rol);
        printf("------------------------------------\n");
    }
    fclose(file);
}
