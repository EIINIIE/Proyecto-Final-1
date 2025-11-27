#ifndef EMPLEADO_H_INCLUDED
#define EMPLEADO_H_INCLUDED

#define ARCHIVO_EMPLEADOS "empleados.bin"

typedef struct
{
    char correo[50];
    char contrasena[50];
    int dni;
    int dia;
    int mes;
    int anios;
    char rol[50];
} stEmpleado;

void menu_empleado_directo();

#endif // EMPLEADO_H_INCLUDED
