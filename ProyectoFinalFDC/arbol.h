#include <stdio.h>
#include <stdlib.h>

typedef struct nodo nodo;
//Define nodo de arbol
struct nodo {
	int plan; //ID de busqueda
	float precio; //Precio del plan
	nodo *left;
	nodo *right;
};

//Genera arbol de planes
nodo *GEN_ARBOL() {
	nodo *tree = NULL;
	return tree;
}
