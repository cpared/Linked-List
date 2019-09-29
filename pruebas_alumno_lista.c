#include "lista.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>


#define MAX_VOLUMEN 1000
#define MAX_ELEMENTOS 500
#define CANT_CARACTERES_ASCII 255
#define CANT_ELEMENTOS_ARR 7

//Creo un puntero a un entero dinámico con un valor
int* crear_entero_dinamico(void){
    int* entero_dinamico = malloc(sizeof(int));
    if (entero_dinamico == NULL){
        return NULL;
    }
    *entero_dinamico = 2;
    return entero_dinamico;
}

//Creo un puntero a un caracter dinámico con un valor
char* crear_caracter_dinamico(void){
    char* caracter_dinamico = malloc(sizeof(char));
    if (caracter_dinamico == NULL){
        return NULL;
    }
    *caracter_dinamico = 'C';
    return caracter_dinamico;
}

/* ******************************************************************
 *                     PRUEBAS LISTA ENLAZADA
 * *****************************************************************/

void prueba_crear_lista(void){
     lista_t* lista = lista_crear();

    print_test("La lista fue creada", lista != NULL);
    print_test("La lista no tiene elementos", lista_esta_vacia(lista));
    print_test("La lista no tiene primero", lista_ver_primero(lista) == NULL);
    print_test("La lista no tiene ultimo", lista_ver_ultimo(lista) == NULL);
    print_test("No se puede borrar primero", lista_borrar_primero(lista) == NULL);

    lista_destruir(lista, NULL);
}

void prueba_volumen_lista(void){
    lista_t* lista = lista_crear();

    int vector_enteros[MAX_ELEMENTOS];
    char vector_caracteres[MAX_ELEMENTOS];
    bool ok_primero = true;
    bool ok_ultimo = true;

    // Lleno el vector con enteros
    for (int i = 0; i < MAX_ELEMENTOS; i++){
        vector_enteros[i] = i;
    }

    // Lleno el vector con cadenas
    for (int i = 0; i < MAX_ELEMENTOS; i++){
        vector_caracteres[i] = (char)(i % CANT_CARACTERES_ASCII);
    }


    // Agrego los elementos al final de la lista  y voy validando en cada iteración
    // que se mantenga siempre el mismo primer elemento y que se vaya actualizando el
    // último.
    for (int i = 0; ok_primero && ok_ultimo && i < MAX_VOLUMEN; i++){
        if (i < MAX_ELEMENTOS){
            lista_insertar_ultimo(lista,&vector_enteros[i]);
            ok_ultimo = lista_ver_ultimo(lista) == &vector_enteros[i];
        }else {
            lista_insertar_ultimo(lista,&vector_caracteres[i - MAX_ELEMENTOS]);
            ok_ultimo = lista_ver_ultimo(lista) == &vector_caracteres[i - MAX_ELEMENTOS];
        }
        ok_primero = lista_ver_primero(lista) == &vector_enteros[0];
    }

    print_test("Se mantuvo el primer elemento",ok_primero);
    print_test("Se fue actualizano el último elemento",ok_ultimo);


    // Valido que el último elemento se mantenga constante y voy eliminando el primero
    // en cada iteración. Valido que se actualice el primero.
    ok_primero = true;
    ok_ultimo = true;
    for (int i = 0; ok_primero && ok_ultimo && i < MAX_VOLUMEN; i++){
        ok_ultimo = lista_ver_ultimo(lista) == &vector_caracteres[MAX_ELEMENTOS - 1];
        if (i < MAX_ELEMENTOS){
            ok_primero = lista_borrar_primero(lista) == &vector_enteros[i];
        } else {
            ok_primero = lista_borrar_primero(lista) == &vector_caracteres[i - MAX_ELEMENTOS];
        }
    }

    print_test("Se fue actualizando el primer elemento",ok_primero);
    print_test("Se mantuvo el ultimo elemento",ok_ultimo);

    lista_destruir(lista,NULL);
    print_test("La lista fue destruida", true);

}

void prueba_insertar_null(){

    lista_t* lista = lista_crear();

    void* n1 = NULL;
    int entero = 7;
    char caracter = 'c';
    bool ok_primero = true;
    bool ok_ultimo = true;

    print_test("La lista fue creada", lista != NULL);
    print_test("La lista no tiene elementos", lista_esta_vacia(lista));

    lista_insertar_primero(lista,&n1);
    ok_primero = lista_ver_primero(lista) == &n1;
    ok_ultimo = lista_ver_ultimo(lista) == &n1;

    print_test("Se insertó NULL el principio",ok_primero);
    print_test("Se insertó NULL al final",ok_ultimo);

    lista_insertar_ultimo(lista,&entero);
    ok_primero = lista_ver_primero(lista) == &n1;
    ok_ultimo = lista_ver_ultimo(lista) == &entero;

    print_test("Se mantuvo NULL el principio",ok_primero);
    print_test("Se insertó entero al final",ok_ultimo);

    lista_insertar_ultimo(lista,&caracter);
    ok_primero = lista_ver_primero(lista) == &n1;
    ok_ultimo = lista_ver_ultimo(lista) == &caracter;

    print_test("Se mantuvo NULL el principio",ok_primero);
    print_test("Se insertó caracter al final",ok_ultimo);

    ok_primero = lista_borrar_primero(lista) == &n1;
    print_test("Borramos el valor NULL del principio",ok_primero);

    lista_destruir(lista,NULL);
    print_test("La lista fue destruida", true);

}

void prueba_destruir_lista(void){

    lista_t* lista = lista_crear();
    int entero = 2;

    print_test("La lista fue creada", lista != NULL);
    print_test("La lista no tiene elementos", lista_esta_vacia(lista));

    lista_insertar_ultimo(lista, &entero);    

    lista_destruir(lista,NULL); 
    print_test("La lista fue destruida", true);

}

void prueba_destruir_dato_dinamico(void){


    lista_t* lista = lista_crear();

    print_test("La lista fue creada", lista != NULL);
    print_test("La lista no tiene elementos", lista_esta_vacia(lista));

    void* entero_dinamico = crear_entero_dinamico();
    void* caracter_dinamico = crear_caracter_dinamico();

    lista_insertar_ultimo(lista, entero_dinamico);
    lista_insertar_ultimo(lista, caracter_dinamico);

    print_test("Se insertó entero dinámico",lista_ver_primero(lista) == entero_dinamico);
    print_test("Se insertó caracter dinámico",lista_ver_ultimo(lista) == caracter_dinamico);

    lista_destruir(lista,free);
    print_test("La lista fue destruida", true);

}

/* ******************************************************************
 *                      PRUEBAS ITERADOR INTERNO
 * *****************************************************************/

bool sumar(void* dato, void* extra){
    *(int*) extra += *(int*) dato;
    return true;
}

void prueba_iterador_interno(void){


    lista_t* lista = lista_crear();
    int arr_enteros[] = {2,6,7,10,-4,-3,11};
    int suma_real = 29; //La suma real de los elementos del arreglo
    int suma_iter = 0;

    print_test("La lista fue creada", lista != NULL);
    print_test("La lista no tiene elementos", lista_esta_vacia(lista));

    for(int i = 0; i < CANT_ELEMENTOS_ARR ; i++){
        lista_insertar_ultimo(lista,&arr_enteros[i]);
    }

    lista_iterar(lista,sumar,&suma_iter);

    print_test("Se sumaron todos los elementos", suma_real == suma_iter);

    lista_destruir(lista,NULL); 
    print_test("La lista fue destruida", true);

}

/* ******************************************************************
 *                      PRUEBAS ITERADOR EXTERNO
 * *****************************************************************/

void prueba_iterador_externo(void){

    lista_t* lista = lista_crear();
    lista_iter_t* iter = lista_iter_crear(lista);
    int arr_enteros[] = {2,6,7,10,-4,-3,11};
    int suma_real = 29;
    int suma_iter = 0;
    bool ok = true;

    print_test("La lista fue creada", lista != NULL);
    print_test("El iterador fue creado", iter != NULL);
    print_test("La lista no tiene elementos", lista_esta_vacia(lista));
    
    for(int i = 0; ok && i < CANT_ELEMENTOS_ARR; i++){
        lista_iter_insertar(iter,&arr_enteros[i]);
        ok = lista_iter_ver_actual(iter) == &arr_enteros[i];
    }

    print_test("Se insertaron los valores", ok);

    lista_iter_destruir(iter);
    lista_destruir(lista,NULL);
    print_test("La lista fue destruida", true);
    print_test("El iterador fue destruido", true);

    //---------------------------------------

    lista_t* lista1 = lista_crear();

    print_test("Una nueva lista fue creada", lista1 != NULL);

    for (int i = 0; i < CANT_ELEMENTOS_ARR; i++){
        lista_insertar_ultimo(lista1, &arr_enteros[i]);
    }

    lista_iter_t* iter1 = lista_iter_crear(lista1);

    print_test("Un nuevo iterador fue creado", iter1 != NULL);

    while(!lista_iter_al_final(iter1)){
        sumar(lista_iter_ver_actual(iter1),&suma_iter);
        lista_iter_avanzar(iter1);
    }

    print_test("La suma real es igual a la suma iterador", suma_iter == suma_real);

    lista_iter_destruir(iter1);
    lista_destruir(lista1,NULL);
    print_test("La lista fue destruida", true);
    print_test("El iterador fue destruido", true);

    //---------------------------------

    lista_t* lista2 = lista_crear();
    print_test("Una nueva lista fue creada", lista2 != NULL);

    lista_iter_t* iter2 = lista_iter_crear(lista2);
    print_test("Un nuevo iterador fue creado", iter2 != NULL);

    ok = true;
    for(int i = 0; i < CANT_ELEMENTOS_ARR; i++){
        lista_iter_insertar(iter2,&arr_enteros[i]);
        ok = lista_iter_ver_actual(iter2) == &arr_enteros[i];
        lista_iter_avanzar(iter2);
    }

    print_test("Se insertaron los elementos", ok);

    lista_iter_destruir(iter2);
    lista_destruir(lista2,NULL);
    print_test("La lista fue destruida", true);
    print_test("El iterador fue destruido", true);

    //---------------------------------

    lista_t* lista3 = lista_crear();
    print_test("Una nueva lista fue creada", lista3 != NULL);

    int entero = 3;
    char caracter = 'c';

    lista_insertar_ultimo(lista3, &entero);
    lista_insertar_ultimo(lista3, &caracter);

    lista_iter_t* iter3 = lista_iter_crear(lista3);
    print_test("Un nuevo iterador fue creado", iter3 != NULL);

    ok = entero == *(int*)lista_iter_borrar(iter3);
    print_test("Se eliminó el entero", ok);
    ok = caracter == *(char*)lista_iter_borrar(iter3);
    print_test("Se eliminó el caracter", ok);

    lista_iter_destruir(iter3);
    lista_destruir(lista3,NULL);
    print_test("La lista fue destruida", true);
    print_test("El iterador fue destruido", true);

}

void pruebas_lista_alumno(void){
    printf("\n");
    printf("-----PRUEBAS CREAR LISTA-----\n");
    prueba_crear_lista();

    printf("\n");
    printf("-------PRUEBAS VOLUMEN-------\n");
    prueba_volumen_lista();

    printf("\n");
    printf("-----PRUEBAS INSERTA NULL-----\n");
    prueba_insertar_null();

    printf("\n");
    printf("----PRUEBAS DESTRUIR LISTA----\n");
    prueba_destruir_lista();

    printf("\n");
    printf("---PRUEBAS DESTRUIR DINAMICO---\n");
    prueba_destruir_dato_dinamico();

    printf("\n");
    printf("-----PRUEBAS ITERADOR INT----\n");
    prueba_iterador_interno();

    printf("\n");
    printf("-----PRUEBAS ITERADOR EXT----\n");
    prueba_iterador_externo();
}