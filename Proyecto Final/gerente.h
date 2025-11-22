#ifndef GERENTE_H_INCLUDED
#define GERENTE_H_INCLUDED

// ------- Estructura Gerente -------
typedef struct
{
    char correo[50];
    char contrasena[50];
    int dni;
    int dia;
    int mes;
    int anios;
    char rol[50];
    int activo; // 1 = activo, 0 = eliminado
} stGerente;

// ------- Prototipo -------
int iniciarSesionGerente();
void menu_gerente();
stGerente cargar_un_empleado();
void agregar_empleado();
void eliminar_empleado();
void eliminar_cliente();
void mostrar_empleados();

#endif // GERENTE_H_INCLUDED

