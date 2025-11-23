#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct {
    char dni[30];
    char nombre[50];
    char telefono[20];
    char direccion[100];
    char rol[30];
} Cliente;

// Alias para cumplir estrictamente con la pauta que pide "Persona"
typedef Cliente Persona;

// Prototipos
Cliente cargar_persona();
void guardar_cliente_en_archivo(Cliente c);
void modificar_cliente();
void ver_listado_clientes();
int telefono_Existente (char telefono[]);
int cliente_existente(char dniBuscado[]);
int dni_Existente_cliente(char dni[]);




#endif // CLIENTE_H_INCLUDED
