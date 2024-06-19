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
	int height;
};

//Obtener altura del Nodo
int GET_HEIGHT(Nodo *N) {
	if(N == NULL) {
		return 0;
	}
	return N->height;
} 

//Funcion para conseguir el maximo entre dos enteros
int MAX(int a, int b) {
	return (a > b)? a : b;
} 

//Crea un nodo
Nodo *CREAR(Plan plan) {
	Nodo *nn = (Nodo*)malloc(sizeof(Nodo));
	nn->plan.mb = plan.mb;
	nn->plan.precio = plan.precio;
	nn->left = NULL;
	nn->right = NULL;
	nn->height = 1;
	return nn;
}

// Funcion para rotar a la derecha el subárbol con raiz
Nodo *rightRotate(Nodo *y)
{
	Nodo *x = y->left;
	Nodo *T2 = x->right;
	// Realizar rotacion
	x->right = y;
	y->left = T2;
	// Actualizar alturas
	y->height = MAX(GET_HEIGHT(y->left), GET_HEIGHT(y->right))+1;
	x->height = MAX(GET_HEIGHT(x->left), GET_HEIGHT(x->right))+1;
	// Devuelve nueva raiz
	return x;
} 

// Funcion para rotar a la izquierda el subárbol con raiz x
Nodo *leftRotate(Nodo *x)
{
	Nodo *y = x->right;
	Nodo *T2 = y->left;
	// Realizar rotacion
	y->left = x;
	x->right = T2;
	// Actualizar alturas
	x->height = MAX(GET_HEIGHT(x->left), GET_HEIGHT(x->right))+1;
	y->height = MAX(GET_HEIGHT(y->left), GET_HEIGHT(y->right))+1;
	// Devuelve nueva raiz
	return y;
} 

int GET_BALANCE(Nodo *N){
	if(N == NULL) {
		return 0;
	}
	return GET_HEIGHT(N->left) - GET_HEIGHT(N->right);
} 

//Insercion nodo
Nodo *INSERTAR_NODO(Nodo *aux, Plan plan) {
	if(aux == NULL) {
		return CREAR(plan);
	}
	if(plan.mb < aux->plan.mb) {
		aux->left = INSERTAR_NODO(aux->left, plan);
	}
	else if(plan.mb > aux->plan.mb) {
		aux->right = INSERTAR_NODO(aux->right, plan);
	}
	else {
		return aux;
	}
	aux->height = 1 + MAX(GET_HEIGHT(aux->left),GET_HEIGHT(aux->right));
	int balance = GET_BALANCE(aux);
	// Left Left Caso
	if (balance > 1 && plan.mb < aux->left->plan.mb) {
		return rightRotate(aux);
	}
	// Right Right Caso
	if(balance < -1 && plan.mb > aux->right->plan.mb) {
		return leftRotate(aux);
	}
	// Left Right Caso
	if (balance > 1 && plan.mb > aux->left->plan.mb)
	{
		aux->left = leftRotate(aux->left);
		return rightRotate(aux);
	}
	// Right Left Caso
	if (balance < -1 && plan.mb < aux->right->plan.mb)
	{
		aux->right = rightRotate(aux->right);
		return leftRotate(aux);
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


void PREORDER(struct Nodo *root) {
	if(root != NULL) {
		printf("%d ", root->plan.mb);
		PREORDER(root->left);
		PREORDER(root->right);
	}
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
