#include <stdio.h>
#include <stdlib.h>

typedef struct Plan Plan;
//Define estructura de plan
struct Plan {
	int mb; //MB del plan(ID de busqueda)
	int precio; //Precio del plan
};

typedef struct Nodo Nodo;
//Define nodo de arbol
struct Nodo {
	Plan plan;
	Nodo *left;
	Nodo *right;
};

//Crea un nodo
Nodo *CREAR(Plan plan) {
	Nodo *nn = (Nodo*)malloc(sizeof(Nodo));
	nn->plan.mb = plan.mb;
	nn->plan.precio = plan.precio;
	nn->left = NULL;
	nn->right = NULL;
	return nn;
}

//Insercion nodo
Nodo *INSERTAR_NODO(Nodo *aux, Plan plan) {
	if(aux == NULL) {
		return CREAR(plan);
	}
	if(plan.mb < aux->plan.mb) {
		aux->left = INSERTAR_NODO(aux->left, plan);
	}
	if(plan.mb > aux->plan.mb) {
		aux->right = INSERTAR_NODO(aux->right, plan);
	}
	return aux;
}

//Genera arbol de planes
Nodo *GEN_ARBOL() {
	Nodo *tree = NULL;
	Plan plan;
	FILE *arch;
	if((arch = fopen("Planes.txt","r")) == 0) {
		printf("\nERROR: No se pudo abrir el archivo para su lectura");
		return NULL;
	}
	while (fscanf(arch, "%d %d", &plan.mb, &plan.precio) == 2) {
		tree = INSERTAR_NODO(tree, plan);
	}
	fclose(arch);
	return tree;
}


void SPREORDER(Nodo *aux) {
	if(aux == NULL) return;
	printf("\nPlan %d mb: $%d.00", aux->plan.mb, aux->plan.precio);
	SPREORDER(aux->left);
	SPREORDER(aux->right);
}

/*Busca el precio del plan usando como
dato de busqueda el mb del cliente*/
Nodo *BUSCAR(Nodo *aux, int val) {
	if(aux == NULL) {
		return NULL;
	}
	if(val == aux->plan.mb) {
		return aux;
	}
	else if(val < aux->plan.mb) {
		return BUSCAR(aux->left, val);
	}
	else if(val > aux->plan.mb) {
		return BUSCAR(aux->right, val);
	}
	return NULL;
}
