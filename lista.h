#include <stdlib.h>
#include <stdbool.h>

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                        PRIMITIVAS LISTA
 * *****************************************************************/

// Crea una lista nueva.
// POST: Devuelve una lista vacía, NULL si hubo un error.
lista_t *lista_crear(void);

// PRE: La lista fue creada.
// POST: Devuelve true si la lista está vacía, false en caso contrario.
bool lista_esta_vacia(const lista_t *lista);

// Inserta un dato pasado por parametro en la primera posición de la lista y devuelve
// true. En caso contrario devuelve false.
// PRE: La lista fue creada.
// POST: Inserta el elemento en la primera posición de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Inserta un dato en la última posición de la lista y devuelve true. Caso contrario
// devuelve false.
// PRE: La lista fue creada.
// POST: Insera el elemento en la última posición de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Elimina el primer elemento de la lista y devuelve su valor, si no tiene elementos
// devuelve NULL.
// PRE: La lista fue creada.
// POST: Elimina el primer elemento de la lista.
void *lista_borrar_primero(lista_t *lista);

// Devuelve el valor del primer elemento de la lista, si no tiene elementos devuelve NULL.
// PRE: La lista fue creada.
// POST: Se devolvió el primer elemento de la lista cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Devuelve el dato del ultimo elemento de la lista. Si tiene un elemento devuelve el dato 
// del primero que también será el último. Y si no tiene elementos devuelve NULL.
// PRE: La lista fue creada.
// POST: Devuelve el dato del último elemento de la lista si no está vacía. 
void *lista_ver_ultimo(const lista_t* lista);

// Devuelve la cantidad de elementos que tiene la lista.
// PRE: La lista fue creada.
// POST: Devuelve la cantidad de elementos de la lista.
size_t lista_largo(const lista_t *lista);

// Destruye la lista junto con todos sus elementos. Si recibe la función 'destruir_dato'
// aplica la función para el dato de cada uno de los elementos de la lista.
// PRE: La lista fue creada. 'destruir_dato' es una función capaz de destruir
//      los datos de la lista, o NULL en caso de que no se la utilice.
// POST: Se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));


/* ******************************************************************
 *                        ITERADOR INTERNO
 * *****************************************************************/

// Recorre la lista hasta el final y aplica la función 'visitar' a cada uno de los  
// datos de la lista. Si 'extra' no es NULL, la función visitar indicará el punto de corte
// según el valor 'extra' que se reciba.
// PRE: La lista fue creada.
// POST: Recorre la lista.
void lista_iterar(lista_t* lista, bool visitar(void* dato, void* extra), void* extra);


/* ******************************************************************
 *                        ITERADOR EXTERNO
 * *****************************************************************/

// PRE: Recibe una lista.
// POST: Crea un iterador para esa lista, devuelve NULL si hubo un error.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza una posición de la lista y devuelve true, en caso que no pueda avanzar devuelve
// false.
// PRE: El iterador fue creado.
// POST: Avanza una posición en la lista.
bool lista_iter_avanzar(lista_iter_t *iter);

// Muestra el dato actual en donde se encuentra el iterador.
// PRE: El iterador fue creado.
// POST: Devuelve el valor del dato donde se enucnetra el iterador.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Si el iterador llegó hasta el final de la lista devuelve true, en caso contrario false.
// PRE: El iterador fue creado.
// POST: Devuelve true si el iterador llegó al final.
bool lista_iter_al_final(const lista_iter_t *iter);

// PRE: El iterador fue creado.
// POST: Destruye el iterador.
void lista_iter_destruir(lista_iter_t *iter);

// Inserta el dato ingresado por parámetro sobre la lista en la posición actual del
// iterador. Devuelve true si insertó correctamente.
// PRE: El iterador fue creado.
// POST: Inserta el dato en la posición actual del iterador.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Borra el dato en la posición actual del iterador y devuelve su valor. Si la lista está
// vacía devuelve NULL.
// PRE: El iterador fue creado.
// POST: Borra el dato en la posición actual del iterador.
void *lista_iter_borrar(lista_iter_t *iter);
