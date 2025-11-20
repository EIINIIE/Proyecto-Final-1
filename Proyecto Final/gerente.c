#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        printf("2. Autos de cliente (Historial ventas)\n");
        printf("3. Autos disponibles (Ver Stock Recursivo)\n");
        printf("4. Realizar Venta (Pagos)\n");
        printf("5. Registrar empleado\n");
        printf("6. Eliminar empleado\n");
        printf("7. Cerrar Sesion\n");
        printf("8. Ventas (Ver historial)\n");
        printf("9. GESTION AUTOS STOCK (Agregar / Modificar)\n"); // <--- ACTUALIZADO
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
            int op;
            do
            {
                printf("\n--- GESTION DE CLIENTES ---\n");
                printf("1. Cargar NUEVO Cliente\n");
                printf("2. MODIFICAR Cliente Existente\n");
                printf("3. Ver listado de clientes\n");
                printf("0. Volver al menu anterior\n");
                printf("Elija opcion: ");
                scanf("%d", &op);
                system("cls");

                switch(op)
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
                if(op != 0) system("pause");
                system("cls");
            }
            while(op != 0);
            break;
        }

        case 2:
            mostrar_todos_autos_cliente(); // Muestra el archivo autos_cliente.bin
            break;
        case 3:
            mostrar_todos_autos_disponibles();  // Recursivo
            break;
        case 4:
            gestionDePagos();
            break;
        case 5:
            agregar_empleado();
            break;
        case 6:
            // Lógica de eliminar empleado...
            eliminar_empleado();
            break;
        case 7:
            printf("Cerrando sesion...\n");
            return;
        case 8:
            mostrarVentas();
            break;

        case 9: // --- SUBMENU GESTION AUTOS ---
        {
            int opAuto;
            printf("\n--- GESTION DE AUTOS STOCK ---\n");
            printf("1. Ingresar NUEVO auto\n");
            printf("2. MODIFICAR auto existente\n");
            printf("0. Cancelar\n");
            printf("Opcion: ");
            scanf("%d", &opAuto);

            if(opAuto == 1) agregar_auto_stock();
            else if(opAuto == 2) modificar_auto_stock();
            break;
        }

        case 10:
            mostrar_empleados();
            break;
        case 11:
            menu_reportes();
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
            getchar();
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
    printf("Ingrese correo: ");
    scanf("%s", nuevo.correo);
    printf("Ingrese contrasena: ");
    scanf("%s", nuevo.contrasena);
    printf("Ingrese DNI: ");
    scanf("%d", &nuevo.dni);
    printf("Ingrese dia de nacimiento: ");
    scanf("%d", &nuevo.dia);
    printf("Ingrese mes de nacimiento: ");
    scanf("%d", &nuevo.mes);
    printf("Ingrese anio de nacimiento: ");
    scanf("%d", &nuevo.anios);
    printf("Ingrese rol: ");
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
    while (fread(&emple, sizeof(stGerente), 1, file) == 1)
    {
        printf("Empleado #%d | DNI: %d | Rol: %s | Correo: %s\n",
               ++contador, emple.dni, emple.rol, emple.correo);
    }
    fclose(file);
}

void eliminar_empleado()
{
    FILE *archivo = fopen("empleados.bin", "rb");
    FILE *temporal = fopen("temp_empleados.bin", "wb");

    if (archivo == NULL || temporal == NULL)
    {
        printf("\nERROR: No se pudo abrir el archivo de empleados o crear el temporal.\n");
        if(archivo != NULL) fclose(archivo);
        if(temporal != NULL) fclose(temporal);
        return;
    }

    stGerente emple;
    int dniBuscar;
    int encontrado = 0;

    printf("\n--- ELIMINAR EMPLEADO ---\n");
    mostrar_empleados(); // Mostramos la lista para que sepa qué DNI elegir
    printf("\nIngrese el DNI del empleado a eliminar: ");
    scanf("%d", &dniBuscar);

    // Leemos el archivo original
    while(fread(&emple, sizeof(stGerente), 1, archivo) == 1)
    {
        if (emple.dni != dniBuscar)
        {
            // Si el DNI NO es el que buscamos, lo copiamos al temporal
            fwrite(&emple, sizeof(stGerente), 1, temporal);
        }
        else
        {
            // Si es igual, NO lo escribimos (efectivamente lo borramos)
            encontrado = 1;
        }
    }

    fclose(archivo);
    fclose(temporal);

    if (encontrado == 1)
    {
        // Borramos el viejo y renombramos el nuevo
        remove("empleados.bin");
        rename("temp_empleados.bin", "empleados.bin");
        printf("\n>>> Empleado con DNI %d eliminado correctamente.\n", dniBuscar);
    }
    else
    {
        // Si no lo encontramos, borramos el temporal que no sirve
        remove("temp_empleados.bin");
        printf("\n No se encontro ningun empleado con ese DNI.\n");
    }
}
