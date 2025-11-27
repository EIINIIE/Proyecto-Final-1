#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct
{
    char dni[30];
    char nombre[50];
    char telefono[20];
    char direccion[100];
    char rol[30];
} Cliente;

Cliente cargar_persona(char dniExterno[]);
Cliente obtener_datos_cliente(char dni[]);
void guardar_cliente_en_archivo(Cliente c);
void modificar_cliente();
void ver_listado_clientes();
int telefono_Existente (char telefono[]);
int cliente_existente(char dniBuscado[]);
int dni_Existente_cliente(char dni[]);

#endif // CLIENTE_H_INCLUDED
