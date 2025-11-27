#ifndef GERENTE_H_INCLUDED
#define GERENTE_H_INCLUDED

typedef struct
{
    char correo[50];
    char contrasena[50];
    int dni;
    int dia;
    int mes;
    int anios;
    char rol[50];
    int activo;
} stGerente;

int iniciarSesionGerente();
void menu_gerente();
stGerente cargar_un_empleado();
void agregar_empleado();
void eliminar_empleado();
void eliminar_cliente();
void mostrar_empleados();
int existe_dni_empleado();
void modificar_empleado();

#endif // GERENTE_H_INCLUDED
