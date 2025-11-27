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
#include "usuario.h"
#include "fecha.h"

// Valida formato de correo (al menos un @)
int es_correo_valido_gerente(char email[])
{
    int i;
    for(i=0; i<strlen(email); i++)
    {
        if(email[i] == '@')
        {
            return 1;
        }
    }
    return 0;
}

// Verifica si el correo ya existe en empleados
int existe_correo_empleado(char correo[])
{
    FILE *file = fopen("empleados.bin", "rb");
    if (file == NULL)
    {
        return 0;
    }

    stEmpleado e;
    while (fread(&e, sizeof(stEmpleado), 1, file) == 1)
    {
        if (strcmp(e.correo, correo) == 0)
        {
            fclose(file);
            return 1; // Si existe
        }
    }
    fclose(file);
    return 0; // No existe
}

int existe_dni_empleado(int dni)
{
    FILE *file = fopen("empleados.bin", "rb");
    if (file == NULL)
    {
        return 0;
    }

    stEmpleado e;
    while (fread(&e, sizeof(stEmpleado), 1, file) == 1)
    {
        if (e.dni == dni)
        {
            fclose(file);
            return 1; // Si existe
        }
    }
    fclose(file);
    return 0; // No existe
}

void menu_gerente()
{
    int opcion;
    do
    {
        printf("Inicio de sesion exitoso - Rol: ADMINISTRADOR");
        printf("\n==================================================\n");
        printf("       PANEL DE CONTROL: ADMINISTRADOR (GERENTE)     \n");
        printf("==================================================\n");
        printf("--- MENU DE GERENTE ---\n");
        printf(" --- GESTION DE CLIENTES ---\n");
        printf(" 1. Clientes (Menu: Cargar / Modificar / Listar)\n");
        printf(" 2. Eliminar Cliente\n");
        printf(" 3. Ver Autos de clientes (Historial)\n");

        printf("\n --- GESTION DE EMPLEADOS ---\n");
        printf(" 4. Registrar Empleado\n");
        printf(" 5. Modificar Empleado\n");
        printf(" 6. Eliminar Empleado\n");
        printf(" 7. Ver Lista de Empleados\n");

        printf("\n --- GESTION DE AUTOS (STOCK) ---\n");
        printf(" 8. Gestion Autos Stock (Agregar / Modificar)\n");
        printf(" 9. Ver Stock Disponible\n");

        printf("\n --- VENTAS Y REPORTES ---\n");
        printf(" 10. Realizar Venta (Pagos)\n");
        printf(" 11. Historial de Ventas\n");
        printf(" 12. Reportes Generales\n");

        printf("\n 0. Cerrar Sesion\n");
        printf("==================================================\n");
        printf("Seleccione una opcion: ");

        fflush(stdin);
        scanf("%d", &opcion);
        system("cls");

        switch (opcion)
        {
        // --- CLIENTES ---
        case 1:
        {
            int op;
            do
            {
                printf("\n--- GESTION DE CLIENTES ---\n");
                printf("1. Cargar Nuevo Cliente (Crear Usuario Completo)\n");
                printf("2. Modificar Cliente Existente\n");
                printf("3. Ver listado de clientes\n");
                printf("0. Volver al menu anterior\n");
                printf("Elija opcion: ");
                fflush(stdin);
                scanf("%d", &op);
                system("cls");

                switch(op)
                {
                case 1:
                {
                    stUsuario nuevoU = registro_Usuario();

                    if(strcmp(nuevoU.dni, "-1") != 0)
                    {
                        guardar_Usuario(nuevoU); // Guardamos el usuario (login)

                        Cliente nuevoC = cargar_persona(nuevoU.dni);

                        guardar_cliente_en_archivo(nuevoC);
                        printf("\nCliente registrado exitosamente (Login habilitado).\n");
                    }
                    else
                    {
                        printf("Operacion cancelada o usuario existente.\n");
                    }
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
                if(op != 0)
                {
                    system("pause");
                    system("cls");
                }
            }
            while(op != 0);
            break;
        }
        case 2:
            eliminar_cliente();
            break;
        case 3:
            mostrar_todos_autos_cliente();
            break;

        // --- EMPLEADOS ---
        case 4:
            agregar_empleado();
            break;
        case 5:
            modificar_empleado();
            break;
        case 6:
            eliminar_empleado();
            break;
        case 7:
            mostrar_empleados();
            break;

        // --- AUTOS ---
        case 8:
        {
            int opAuto;
            printf("\n--- GESTION DE AUTOS STOCK ---\n");
            printf("1. Ingresar Nuevo auto\n");
            printf("2. Modificar auto existente\n");
            printf("0. Cancelar\n");
            printf("Opcion: ");
            fflush(stdin);
            scanf("%d", &opAuto);

            if(opAuto == 1) agregar_auto_stock();
            else if(opAuto == 2)
            {
                modificar_auto_stock();
            }
            break;
        }
        case 9:
            mostrar_todos_autos_disponibles();
            break;

        // --- VENTAS ---
        case 10:
            mostrar_todos_autos_disponibles();
            gestionDePagos("0");
            break;
        case 11:
            mostrarVentas();
            break;
        case 12:
            menu_reportes();
            break;

        // --- SALIDA ---
        case 0:
            printf("Cerrando sesion...\n");
            return;

        default:
            printf("Opcion invalida. Intente nuevamente.\n");
            break;
        }

        if(opcion != 0)
        {
            printf("\nPresione una tecla para continuar...");
            getchar();
            getchar();
            system("cls");
        }

    }
    while(opcion != 0);
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

        // 2. VALIDACION CORREO
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

        // 3. VALIDACION CONTRASEÑA
        int passOk = 0;
        do
        {
            printf("Ingrese contrasena (min 4 chars, letras y numeros): ");
            fflush(stdin);
            scanf("%s", nuevo.contrasena);

            int largo = strlen(nuevo.contrasena);
            int tieneLetra = 0;
            int tieneNum = 0;

            for(int k=0; k < largo; k++)
            {
                if((nuevo.contrasena[k] >= 'a' && nuevo.contrasena[k] <= 'z') ||
                        (nuevo.contrasena[k] >= 'A' && nuevo.contrasena[k] <= 'Z'))
                {
                    tieneLetra = 1;
                }
                if(nuevo.contrasena[k] >= '0' && nuevo.contrasena[k] <= '9')
                {
                    tieneNum = 1;
                }
            }

            if(largo >= 4 && tieneLetra == 1 && tieneNum == 1)
            {
                passOk = 1;
            }
            else
            {
                printf("Error: La contrasenia debe tener mas de 4 caracteres, al menos una letra y un numero.\n");
            }
        }
        while(passOk == 0);

        cargar_fecha_estricta(&nuevo.dia, &nuevo.mes, &nuevo.anios);

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
        printf("Error al abrir.\n");
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
    char dniBorrar[30];
    int borreCliente = 0;
    int borreUsuario = 0;

    // Mostramos la lista para que el usuario elija
    ver_listado_clientes();

    printf("\nIngrese DNI del cliente a eliminar TOTALMENTE: ");
    fflush(stdin);
    scanf("%s", dniBorrar);

    // ========================================================
    // PASO 1: ELIMINAR DE CLIENTES.BIN
    // ========================================================
    FILE *fCli = fopen("clientes.bin", "rb");
    FILE *fTempC = fopen("tempC.bin", "wb");

    if (fCli == NULL)
    {
        printf("No se pudo abrir el archivo de clientes.\n");
        if (fTempC != NULL) fclose(fTempC);
    }
    else if (fTempC == NULL)
    {
        printf("Error al crear archivo temporal de clientes.\n");
        fclose(fCli);
    }
    else
    {
        Cliente c;
        // 1. Copiamos al temporal SOLO los que NO coinciden con el DNI
        while (fread(&c, sizeof(Cliente), 1, fCli) == 1)
        {
            if (strcmp(c.dni, dniBorrar) != 0)
            {
                fwrite(&c, sizeof(Cliente), 1, fTempC);
            }
            else
            {
                borreCliente = 1; // Encontramos al que queriamos borrar
            }
        }

        // Cerramos para guardar cambios en el temporal
        fclose(fCli);
        fclose(fTempC);

        // 2. Si encontramos el cliente, SOBRESCRIBIMOS el original con el temporal limpio
        if (borreCliente == 1)
        {
            fTempC = fopen("tempC.bin", "rb");    // Lectura del limpio
            fCli = fopen("clientes.bin", "wb");   // Escritura (borra contenido viejo)

            if (fTempC != NULL && fCli != NULL)
            {
                while (fread(&c, sizeof(Cliente), 1, fTempC) == 1)
                {
                    fwrite(&c, sizeof(Cliente), 1, fCli);
                }
            }

            if (fTempC != NULL)
            {
                fclose(fTempC);
            }
            if (fCli != NULL)
            {
                fclose(fCli);
            }
        }
        else
        {
            printf("[INFO] DNI no encontrado en Clientes.\n");
        }
    }

    // ========================================================
    // PASO 2: ELIMINAR DE USUARIOS.BIN (Liberar Login)
    // ========================================================
    FILE *fUsu = fopen("usuarios.bin", "rb");

    // Si no existe el archivo de usuarios, no hacemos nada
    if (fUsu != NULL)
    {
        FILE *fTempU = fopen("tempU.bin", "wb");

        if (fTempU != NULL)
        {
            stUsuario u;
            // 1. Copiamos al temporal SOLO los usuarios que NO coinciden con el DNI
            while (fread(&u, sizeof(stUsuario), 1, fUsu) == 1)
            {
                if (strcmp(u.dni, dniBorrar) != 0)
                {
                    fwrite(&u, sizeof(stUsuario), 1, fTempU);
                }
                else
                {
                    borreUsuario = 1; // Encontramos el usuario asociado
                }
            }

            fclose(fUsu);
            fclose(fTempU);

            // 2. Si se borro algo, actualizamos el archivo original manualmente
            if (borreUsuario == 1)
            {
                fTempU = fopen("tempU.bin", "rb");
                fUsu = fopen("usuarios.bin", "wb"); // "wb" limpia el archivo original

                if (fTempU != NULL && fUsu != NULL)
                {
                    while (fread(&u, sizeof(stUsuario), 1, fTempU) == 1)
                    {
                        fwrite(&u, sizeof(stUsuario), 1, fUsu);
                    }
                }

                if (fTempU != NULL)
                {
                    fclose(fTempU);
                }
                if (fUsu != NULL)
                {
                    fclose(fUsu);
                }
            }
        }
        else
        {
            fclose(fUsu); // Cerramos fUsu si no pudimos abrir el temporal
        }
    }

    // ========================================================
    // PASO 3: CONFIRMACION FINAL
    // ========================================================
    if (borreCliente == 1 || borreUsuario == 1)
    {
        printf("\n[EXITO] Registro eliminado. El DNI %s ha sido liberado del sistema.\n", dniBorrar);
    }
    else
    {
        printf("\n[ALERTA] No se realizaron cambios (DNI no encontrado).\n");
    }
}

void modificar_empleado()
{
    FILE *file = fopen("empleados.bin", "r+b");
    if (file == NULL)
    {
        printf("No se pudo abrir el archivo de empleados.\n");
        return;
    }

    int dniBuscado;
    printf("\nIngrese DNI del empleado a modificar: ");
    scanf("%d", &dniBuscado);

    stEmpleado e;
    int encontrado = 0;

    while (fread(&e, sizeof(stEmpleado), 1, file) == 1)
    {
        if (e.dni == dniBuscado)
        {
            encontrado = 1;
            printf("\n--- EMPLEADO ENCONTRADO ---\n");
            printf("Rol actual: %s\n", e.rol);
            printf("Correo actual: %s\n", e.correo);

            int opMod;
            printf("\n1. Modificar Correo\n");
            printf("2. Modificar Contrasenia\n");

            printf("0. Cancelar\n");
            printf("Opcion: ");
            scanf("%d", &opMod);

            switch(opMod)
            {
            case 1:
                printf("Nuevo Correo: ");
                fflush(stdin);
                scanf("%s", e.correo);
                break;
            case 2:
                printf("Nueva Contrasenia: ");
                fflush(stdin);
                scanf("%s", e.contrasena);
                break;
            case 0:
                printf("Cancelado.\n");
                fclose(file);
                return;
            default:
                printf("Opcion invalida.\n");
            }

            // Movemos el puntero hacia atras para sobrescribir
            fseek(file, -sizeof(stEmpleado), SEEK_CUR);
            fwrite(&e, sizeof(stEmpleado), 1, file);

            printf("Empleado modificado correctamente.\n");
            break;
        }
    }

    if (encontrado == 0)
    {
        printf("No se encontro empleado con DNI %d.\n", dniBuscado);
    }

    fclose(file);
}
