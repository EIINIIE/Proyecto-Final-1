#ifndef USUARIO_H_INCLUDED
#define USUARIO_H_INCLUDED

#define ARCHIVO_USUARIOS "usuarios.bin"

typedef struct
{
    char correo[50];
    char contrasena[50];
    char dni[30];
    int dia;
    int mes;
    int anios;
} stUsuario;

int usuario_Existente(char correo[]);
stUsuario registro_Usuario();
void guardar_Usuario(stUsuario usuario);
int verificar_Usuario(char correo[], char contrasena[]);
void iniciarSesion();
void mostrarTodosLosUsuarios();
int dni_Existente_usuario(char dni[]);
int cargarUsuariosEnArreglo(stUsuario arr[], int tope);
void mostrarUsuariosRecursivo(stUsuario arr[], int pos, int total);

#endif // USUARIO_H_INCLUDED
