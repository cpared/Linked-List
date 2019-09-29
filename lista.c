#include "lista.h"
#include <stdlib.h>

#define LARGO_INICIAL 0


/* ******************************************************************
 *                             STRUCT
 * *****************************************************************/

typedef struct nodo {
    void* dato;
    struct nodo* prox;
} nodo_t;

struct lista {
    size_t largo;
    nodo_t* primero;
    nodo_t* ultimo;
};

struct lista_iter{
    lista_t* lista;
    nodo_t* actual;
    nodo_t* anterior; 
};

/* ******************************************************************
 *                        PRIMITIVAS NODO
 * *****************************************************************/

nodo_t* crear_nodo(void* dato){
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (!nodo) return NULL;

    nodo->dato = dato;
    nodo->prox = NULL;
    return nodo;
}


/* ******************************************************************
 *                        PRIMITIVAS LE
 * *****************************************************************/

lista_t *lista_crear(void){
    lista_t* lista = malloc(sizeof(lista_t));
    if (!lista) return NULL;

    lista->largo = LARGO_INICIAL;
    lista->primero = NULL;
    lista->ultimo = NULL;
    return lista;
}

bool lista_esta_vacia(const lista_t *lista){
    return lista->largo == LARGO_INICIAL;
}

// Primitiva para no reutilizar código
bool insetar_caso_borde(lista_t *lista, nodo_t* nuevo){
    lista->primero = nuevo;
    lista->ultimo = nuevo;
    lista->largo ++;
    return true;
}

bool lista_insertar_primero(lista_t *lista, void *dato){
    nodo_t* nodo = crear_nodo(dato);

    if(lista->largo == LARGO_INICIAL) return insetar_caso_borde(lista,nodo);

    nodo->prox = lista->primero;
    lista->primero = nodo;
    lista->largo ++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
    nodo_t* nodo = crear_nodo(dato);

    if(lista->largo == LARGO_INICIAL) return insetar_caso_borde(lista,nodo);

    lista->ultimo->prox = nodo;
    lista->ultimo = nodo;
    lista->largo ++;
    return true;
}

void *lista_borrar_primero(lista_t *lista){
    if(lista->largo == LARGO_INICIAL) return NULL;

    nodo_t* eliminar = lista->primero;
    void* dato = lista->primero->dato;

    if(lista->largo == 1) lista->ultimo = NULL;

    lista->primero = lista->primero->prox;
    free(eliminar);
    lista->largo --;
    return dato;
}

void *lista_ver_primero(const lista_t *lista){
    if(lista->largo == LARGO_INICIAL) return NULL;

    return lista->primero->dato;
}

void *lista_ver_ultimo(const lista_t* lista){
    if(lista->largo == LARGO_INICIAL) return NULL;

    return lista->ultimo->dato;
}

size_t lista_largo(const lista_t *lista){
    return lista->largo;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)){
    nodo_t* actual = lista->primero;;

    while(actual){
        if(destruir_dato){
            destruir_dato(actual->dato);
        }
        lista->primero = lista->primero->prox;
        free(actual);
        actual = lista->primero;
    }
    free(lista);
}

/* ******************************************************************
 *                        ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t* lista, bool visitar(void* dato, void* extra), void* extra){
    nodo_t* actual = lista->primero;
    bool condicion_corte = true;

    while(actual && condicion_corte){
        condicion_corte = visitar(actual->dato, extra);
        actual = actual->prox;
    }
}

/* ******************************************************************
 *                        PRIMITIVAS EXTERNO
 * *****************************************************************/

lista_iter_t *lista_iter_crear(lista_t *lista){
    lista_iter_t* iter = malloc(sizeof(lista_iter_t));
    if (!iter) return NULL;

    iter->actual = lista->primero;
    iter->anterior = NULL;
    iter->lista = lista;
    return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter){
    if (!iter->actual) return false;

    iter->anterior = iter->actual;
    iter->actual = iter->actual->prox;
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
    if (!iter->actual) return NULL;
    return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
    return iter->actual == NULL;
}

void lista_iter_destruir(lista_iter_t *iter){
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
    if (lista_esta_vacia(iter->lista) || !iter->anterior){
        lista_insertar_primero(iter->lista, dato);
        iter->actual = iter->lista->primero;
        return true;
    }
    if (!iter->actual){
        lista_insertar_ultimo(iter->lista, dato);
        iter->actual = iter->lista->ultimo;
        return true;
    }
    nodo_t* nuevo = crear_nodo(dato);
    iter->anterior->prox = nuevo;
    nuevo->prox = iter->actual;
    iter->actual = nuevo;
    iter->lista->largo ++;
    return true;
}


void *lista_iter_borrar(lista_iter_t *iter){
    if (!iter->actual) return NULL;
    nodo_t* eliminado = iter->actual;
    void* dato = iter->actual->dato;

    if (iter->actual == iter->lista->primero){//Estoy en la primera posición
        dato = lista_borrar_primero(iter->lista);
        iter->actual = iter->lista->primero;
        return dato;
    } 
    if (iter->actual == iter->lista->ultimo){ //Estoy en la última posición
        iter->lista->ultimo = iter->anterior;
    }
    iter->anterior->prox = iter->actual->prox;
    iter->actual = iter->actual->prox;
    free(eliminado);
    iter->lista->largo --;
    return dato;
}