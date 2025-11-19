// cliente.h
#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct {
    char dni[30];
    char nombre[50];
    char telefono[20];
    char direccion[100];
    char rol[30];
} Cliente;

// Prototipos
Cliente cargar_persona();
void guardar_cliente_en_archivo(Cliente c);
void modificar_cliente();
void ver_listado_clientes();

#endif // CLIENTE_H_INCLUDED
