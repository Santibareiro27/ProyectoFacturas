#include <stdio.h>
#include <stdlib.h>

typedef struct nodo nodo;

struct nodo {
	int data;
	nodo *left;
	nodo *right;
};

nodo *CREAR(int val) {
	nodo *nn = (nodo*)malloc(sizeof(nodo));
	nn->data = val;
	nn->left = NULL;
	nn->right = NULL;
	return nn;
}

nodo *INSERTAR(nodo *aux, int val) {
	if(aux == NULL) {
		return CREAR(val);
	}
	if(val < aux->data) {
		aux->left = INSERTAR(aux->left, val);
	}
	if(val > aux->data) {
		aux->right = INSERTAR(aux->right, val);
	}
	return aux;
}

void SPREORDER(nodo *aux) {
	if(aux == NULL) return;
	printf("\n%d", aux->data);
	SPREORDER(aux->left);
	SPREORDER(aux->right);
}

nodo *BUSCAR(nodo *aux, int val) {
	if(aux == NULL) {
		return NULL;
	}
	if(val == aux->data) {
		return aux;
	}
	else if(val < aux->data) {
		return BUSCAR(aux->left, val);
	}
	else if(val > aux->data) {
		return BUSCAR(aux->right, val);
	}
	return NULL;
}

int GETMIN(nodo *aux) {
	while(aux->left != NULL) {
		aux = aux->left;
	}
	return aux->data;
}

nodo *REMOVEN(nodo *aux, int val) {
	if(aux == NULL) {
		return NULL;
	}
	if(val < aux->data) {
		aux->left = REMOVEN(aux->left, val);
	}
	else if(val > aux->data) {
		aux->right = REMOVEN(aux->right, val);
	}
	else {
		if(aux->left == NULL && aux->right == NULL) {
			free(aux);
			return NULL;
		}
		else if(aux->left == NULL) {
			nodo *temp = aux->right;
			free(aux);
			return temp;
		}
		else if(aux->right == NULL) {
			nodo *temp = aux->left;
			free(aux);
			return temp;
		}
		else {
			int rmin = GETMIN(aux->right);
			aux->data = rmin;
			aux->right = REMOVEN(aux->right, rmin);
		}
	}
	return aux;
}

int main(int argc, char *argv[]) {
	nodo *tree = NULL;
	int val;
	system("color 09");
	for(;;) {
		printf("\n\nMENU");
		printf("\n1 - Crear nuevo nodo.");
		printf("\n2 - Mostrar arbol.");
		printf("\n3 - Buscar nodo.");
		printf("\n4 - Eliminar nodo.");
		printf("\n5 - Salir.");
		printf("\nOpcion: ");
		fflush(stdin);
		switch(getchar()) {
		case'1':
			do {
				printf("\nIngrese un valor numerico positivo y entero: ");
				if(!scanf("%d",&val)) {
					fflush(stdin);
					val = 0;
				}
				if(val < 1) {
					printf("\nERROR: Carga no valida.\n");
				}
			}while(val < 1);
			tree = INSERTAR(tree, val);
			break;
			
		case'2':
			SPREORDER(tree);
			break;
			
		case'3':
			do {
				printf("\nIngrese un valor a buscar: ");
				if(!scanf("%d",&val)) {
					fflush(stdin);
					val = 0;
				}
				if(val < 1) {
					printf("\nERROR: Ingreso no valido.\n");
				}
			}while(val < 1);
			if (BUSCAR(tree, val) != NULL) {
				printf("\nNodo encontrado.\n");
			}
			else {
				printf("\nNodo no encontrado.\n");
			}
			break;
			
		case'4':
			do {
				printf("\nIngrese un valor a remover: ");
				if(!scanf("%d",&val)) {
					fflush(stdin);
					val = 0;
				}
				if(val < 1) {
					printf("\nERROR: Ingreso no valido.\n");
				}
			}while(val < 1);
			tree = REMOVEN(tree, val);
			break;
			
		case'5':
			return 0;
			
		default:
			printf("\nERROR: Opcion no valida.\n");
			break;
		}
	}
}

