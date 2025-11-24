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

// Valida formato de correo (al menos un @)
int es_correo_valido_gerente(char email[])
{
    int i;
    for(i=0; i<strlen(email); i++)
    {
        if(email[i] == '@') return 1;
    }
    return 0;
}

// Verifica si el correo ya existe en empleados
int existe_correo_empleado(char correo[])
{
    FILE *file = fopen("empleados.bin", "rb");
    if (file == NULL) return 0;

    stEmpleado e;
    while (fread(&e, sizeof(stEmpleado), 1, file) == 1)
    {
        if (strcmp(e.correo, correo) == 0)
        {
            fclose(file);
            return 1; // Ya existe
        }
    }
    fclose(file);
    return 0; // No existe
}

// Verifica si el DNI ya existe en empleados
int existe_dni_empleado(int dni)
{
    FILE *file = fopen("empleados.bin", "rb");
    if (file == NULL) return 0;

    stEmpleado e;
    while (fread(&e, sizeof(stEmpleado), 1, file) == 1)
    {
        if (e.dni == dni)
        {
            fclose(file);
            return 1; // Ya existe
        }
    }
    fclose(file);
    return 0; // No existe
}

// Valida que la contraseña tenga al menos una letra (LOGICA MANUAL)
int es_contrasenia_valida(char pass[])
{
    int i;
    int tieneLetra = 0;

    if(strlen(pass) < 4) return 0; // Minimo 4 caracteres por seguridad

    for(i=0; i<strlen(pass); i++)
    {
        // Verificamos manualmente si es mayuscula o minuscula
        if((pass[i] >= 'a' && pass[i] <= 'z') || (pass[i] >= 'A' && pass[i] <= 'Z'))
        {
            tieneLetra = 1;
        }
    }

    return tieneLetra;
}

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
        printf("9. Gestion Autos Stock (Agregar / Modificar)\n");
        printf("10. Ver lista de empleados\n");
        printf("11. Reportes\n");
        printf("12. Eliminar Cliente \n");
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
                printf("1. Cargar Nuevo Cliente\n");
                printf("2. Modificar Cliente Existente\n");
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
            mostrar_todos_autos_cliente();
            break;
        case 3:
            mostrar_todos_autos_disponibles();
            break;
        case 4:
            mostrar_todos_autos_disponibles();
            gestionDePagos();

            break;
        case 5:
            agregar_empleado();
            break;
        case 6:

            eliminar_empleado();
            break;
        case 7:
            printf("Cerrando sesion...\n");
            return;
        case 8:
            mostrarVentas();
            break;

        case 9:
        {
            int opAuto;
            printf("\n--- GESTION DE AUTOS STOCK ---\n");
            printf("1. Ingresar Nuevo auto\n");
            printf("2. Modificar auto existente\n");
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
        case 12:
            eliminar_cliente();
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

void agregar_empleado()
{
    FILE* file = fopen("empleados.bin", "ab");
    if (file == NULL)
    {
        printf("Error al abrir el archivo de empleados.\n");
        return;
    }

    char control = 's';
    stEmpleado nuevo;

    while (control == 's' || control == 'S')
    {
        system("cls");
        printf("\n---- REGISTRO DE NUEVO EMPLEADO ----\n");

        int dniValido = 0;
        char bufferDNI[20];

        do
        {
            dniValido = 1;
            printf("Ingrese DNI (solo numeros): ");
            fflush(stdin);
            scanf("%s", bufferDNI);

            for (int i = 0; i < strlen(bufferDNI); i++)
            {
                if (bufferDNI[i] < '0' || bufferDNI[i] > '9')
                {
                    dniValido = 0;
                    printf("Error: El DNI debe contener solo numeros.\n");
                    break;
                }
            }

            if (dniValido == 1)
            {
                if (strlen(bufferDNI) < 7 || strlen(bufferDNI) > 8)
                {
                    dniValido = 0;
                    printf("Error: El DNI debe tener 7 u 8 digitos.\n");
                }
                else
                {
                    int dniTemp = atoi(bufferDNI);
                    if (existe_dni_empleado(dniTemp) == 1)
                    {
                        dniValido = 0;
                        printf("Error: Ya existe un empleado con ese DNI.\n");
                    }
                    else
                    {
                        nuevo.dni = dniTemp;
                    }
                }
            }

        }
        while (dniValido == 0);

        // 2. VALIDACION CORREO (Formato y no repetido)
        int correoOk = 0;
        do
        {
            printf("Ingrese correo: ");
            fflush(stdin);
            scanf("%s", nuevo.correo);

            if(es_correo_valido_gerente(nuevo.correo) == 0)
            {
                printf("Error: El correo debe tener '@'.\n");
            }
            else if(existe_correo_empleado(nuevo.correo) == 1)
            {
                printf("Error: Ese correo ya esta registrado.\n");
            }
            else
            {
                correoOk = 1;
            }
        }
        while(correoOk == 0);

        int passOk = 0;
        do
        {
            printf("Ingrese contrasena (min 4 chars, debe tener letras): ");
            fflush(stdin);
            scanf("%s", nuevo.contrasena);

            if(es_contrasenia_valida(nuevo.contrasena) == 0)
            {
                printf("Error: La contrasena debe tener letras y ser mayor a 4 caracteres.\n");
            }
            else
            {
                passOk = 1;
            }
        }
        while(passOk == 0);


        int fechaValida = 0;
        printf("\n-- Fecha de Nacimiento --\n");
        do
        {
            fechaValida = 1;

            // Validar DIA
            int dOk = 0;
            do
            {
                nuevo.dia = ingresar_entero("Ingrese dia (1-31): ");
                if(nuevo.dia >= 1 && nuevo.dia <= 31)
                {
                    dOk = 1;
                }
                else
                {
                    printf("Dia invalido.\n");
                }
            }
            while(dOk == 0);

            // Validar MES
            int mOk = 0;
            do
            {
                nuevo.mes = ingresar_entero("Ingrese mes (1-12): ");
                if(nuevo.mes >= 1 && nuevo.mes <= 12)
                {
                    mOk = 1;
                }
                else
                {
                    printf("Mes invalido.\n");
                }
            }
            while(mOk == 0);

            // Validar ANIO
            int aOk = 0;
            do
            {
                nuevo.anios = ingresar_entero("Ingrese anio (1950-2007): ");
                if(nuevo.anios >= 1950 && nuevo.anios <= 2007)
                {
                    aOk = 1;
                }
                else
                {
                    printf("Anio invalido (debe ser mayor de edad).\n");
                }
            }
            while(aOk == 0);

            // Validacion extra febrero
            if (nuevo.mes == 2 && nuevo.dia > 29)
            {
                printf("Febrero no tiene mas de 29 dias.\n");
                fechaValida = 0;
            }

        }
        while (fechaValida == 0);

        strcpy(nuevo.rol, "empleado");
        printf("\nRol asignado automaticamente: %s\n", nuevo.rol);

        fwrite(&nuevo, sizeof(stEmpleado), 1, file);
        printf("\n Empleado registrado con exito.\n");

        printf("\nDesea registrar otro empleado? (s/n): ");
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

    stEmpleado emple;
    int contador = 0;

    printf("\n-------- LISTA DE EMPLEADOS --------\n");
    while (fread(&emple, sizeof(stEmpleado), 1, file) == 1)
    {
        printf("Empleado #%d | DNI: %d | Rol: %s | Correo: %s\n",
               ++contador, emple.dni, emple.rol, emple.correo);
    }
    fclose(file);
}

void eliminar_empleado()
{
    FILE *archivo = fopen("empleados.bin", "rb");
    if (archivo == NULL)
    {
        printf("No hay empleados registrados o error al abrir.\n");
        return;
    }

    stEmpleado empleados[100];
    int cantidad = 0;

    while (fread(&empleados[cantidad], sizeof(stEmpleado), 1, archivo) == 1)
    {
        cantidad++;
    }
    fclose(archivo);

    int dniBuscar;
    mostrar_empleados();
    printf("\nIngrese el DNI del empleado a eliminar: ");
    scanf("%d", &dniBuscar);

    archivo = fopen("empleados.bin", "wb");
    if (archivo == NULL)
    {
        printf("Error critico al reabrir el archivo.\n");
        return;
    }

    int encontrado = 0;
    for(int i = 0; i < cantidad; i++)
    {
        if (empleados[i].dni != dniBuscar)
        {
            fwrite(&empleados[i], sizeof(stEmpleado), 1, archivo);
        }
        else
        {
            encontrado = 1;
        }
    }
    fclose(archivo);

    if (encontrado == 1)
    {
        printf("Empleado eliminado correctamente.\n");
    }
    else
    {
        printf("No se encontro un empleado con ese DNI.\n");
    }
}

void eliminar_cliente()
{
    FILE *archivo = fopen("clientes.bin", "rb");
    if (archivo == NULL)
    {
        printf("No hay clientes registrados o error al abrir.\n");
        return;
    }

    Cliente listaClientes[100];
    int cantidad = 0;


    while (fread(&listaClientes[cantidad], sizeof(Cliente), 1, archivo) == 1)
    {
        cantidad++;
    }
    fclose(archivo);

    char dniBuscar[30];
    ver_listado_clientes();
    printf("\nIngrese el DNI del cliente a eliminar: ");
    fflush(stdin);
    scanf("%s", dniBuscar);

    archivo = fopen("clientes.bin", "wb");
    if (archivo == NULL)
    {
        printf("Error critico al reabrir el archivo.\n");
        return;
    }

    int encontrado = 0;
    for(int i = 0; i < cantidad; i++)
    {
        if (strcmp(listaClientes[i].dni, dniBuscar) != 0)
        {
            fwrite(&listaClientes[i], sizeof(Cliente), 1, archivo);
        }
        else
        {
            encontrado = 1;
        }
    }
    fclose(archivo);

    if (encontrado == 1)
    {
        printf("Cliente eliminado correctamente.\n");
    }
    else
    {
        printf("No se encontro un cliente con ese DNI.\n");
    }
}
