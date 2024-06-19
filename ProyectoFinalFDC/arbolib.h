#include <time.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>

typedef struct cliente cliente;
//Define estructura de cli
struct cliente {
	int cuit,mb;
	char *apellido,*nombre,*iva,*direccion;
	char zona;
	time_t fechaUltPago;
};

typedef struct Nodo Nodo;
//Define nodo de arbol
struct Nodo {
	cliente cli;
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
Nodo *CREAR(cliente cli) {
	Nodo *nn = (Nodo*)malloc(sizeof(Nodo));
	if(nn == NULL) {
		printf("\nERROR: Memoria insuficiente");
		return NULL;
	}
	nn->cli=cli;
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
Nodo *INSERTAR_NODO(Nodo *aux, cliente cli) {
	if(aux == NULL) {
		return CREAR(cli);
	}
	if(cli.cuit < aux->cli.cuit) {
		aux->left = INSERTAR_NODO(aux->left, cli);
	}
	else if(cli.cuit > aux->cli.cuit) {
		aux->right = INSERTAR_NODO(aux->right, cli);
	}
	else {
		return aux;
	}
	aux->height = 1 + MAX(GET_HEIGHT(aux->left),GET_HEIGHT(aux->right));
	int balance = GET_BALANCE(aux);
	// Left Left Caso
	if (balance > 1 && cli.cuit < aux->left->cli.cuit) {
		return rightRotate(aux);
	}
	// Right Right Caso
	if(balance < -1 && cli.cuit > aux->right->cli.cuit) {
		return leftRotate(aux);
	}
	// Left Right Caso
	if (balance > 1 && cli.cuit > aux->left->cli.cuit)
	{
		aux->left = leftRotate(aux->left);
		return rightRotate(aux);
	}
	// Right Left Caso
	if (balance < -1 && cli.cuit < aux->right->cli.cuit)
	{
		aux->right = rightRotate(aux->right);
		return leftRotate(aux);
	} 
	return aux;
}

time_t atotime_t(char *fecha) {
	struct tm tm = {0}; // Inicializar la estructura tm con ceros
	time_t t;
	
	// Analizar la cadena de fecha manualmente
	int dia, mes, anio;
	if (sscanf(fecha, "%d/%d/%d", &dia, &mes, &anio) != 3) {
		printf("Error al analizar la fecha\n");
		return -1;
	}
	
	// Llenar la estructura tm
	tm.tm_mday = dia;
	tm.tm_mon = mes-1;  // Los meses en struct tm son 0-11
	tm.tm_year = anio-1900;  // Los años en struct tm son años desde 1900
	
	
	// Convertir la estructura tm a time_t
	
	t = mktime(&tm);
	
	if (t == -1) {
		printf("Error al convertir la fecha a time_t\n");
		return -1;
	}
	
	return t;
	}


//Genera arbol de clientes
Nodo *GEN_ARBOL(char *archname) {
	Nodo *tree = NULL;
	cliente cli;
	FILE *arch;
	char linea[180], *dato;
	arch = fopen(archname,"r");
	if(!arch) {
		printf("\nERROR: No se encontro \"%s\"", archname);
		return NULL;
	}
	fgets(linea, sizeof(linea), arch);
	while (fgets(linea, sizeof(linea), arch)) {
		
		dato = strtok(linea, ",");
		cli.cuit = atoi(dato);
		printf("%d,", cli.cuit);
		
		dato = strtok(NULL, ",");
		for(int i=0;i<strlen(dato);i++){
			dato[i] = toupper(dato[i]);
		}
		cli.apellido = dato;
		printf("%s", cli.apellido);
		
		dato = strtok(NULL, ",");
		for(int i=0;i<strlen(&dato);i++){
			dato[i] = tolower(dato[i]);
		}
		cli.nombre = dato;
		printf("%s", cli.nombre);
		
		dato = strtok(NULL, ",");
		for(int i=0;i<strlen(&dato);i++){
			dato[i] = tolower(dato[i]);
		}
		cli.iva = dato;
		
		dato = strtok(NULL, ",");
		for(int i=0;i<strlen(&dato);i++){
			dato[i] = tolower(dato[i]);
		}
		cli.direccion = dato;
		dato = strtok(NULL, ",");
		cli.zona=tolower(*dato);
		dato = strtok(NULL, ",");
		cli.cuit = atof(dato);
		
		dato = strtok(NULL, ",");
		cli.fechaUltPago=atotime_t(dato);
		
		tree = INSERTAR_NODO(tree, cli);
	}
	fclose(arch);
	return tree;
}


void PREORDER(struct Nodo *root) {
	if(root != NULL) {
		printf("%d,", root->cli.cuit);
		printf("%s", root->cli.apellido);
		printf("%s", root->cli.nombre);
		printf("%s", root->cli.iva);
		printf("%s", root->cli.direccion);
		printf("%c,", root->cli.zona);
		printf("%d,", root->cli.mb);
		
		PREORDER(root->left);
		PREORDER(root->right);
	}
} 

/*Busca el precio del cli usando como
dato de busqueda el cuit del cliente*/
Nodo *BUSCAR(Nodo *aux, int val) {
	if(aux == NULL) {
		return NULL;
	}
	if(val == aux->cli.cuit) {
		return aux;
	}
	else if(val < aux->cli.cuit) {
		return BUSCAR(aux->left, val);
	}
	else if(val > aux->cli.cuit) {
		return BUSCAR(aux->right, val);
	}
	return NULL;
}

int COMPROBAR_CLIENTESCSV(char *archname) {
	FILE *arch;
	arch = fopen(archname,"r");
	if(!arch) {
		arch = fopen(archname,"w");
		if(!arch) {
			printf("\nERROR: No se pudo crear el archivo de planes");
			return 1;
		}
		fputs("cuit,Apellido,Nombre,CUIT,CondicionIVA,Direccion,Zona,Plan,FechaUltimoPago,\n",arch);
	}
	fclose(arch);
	return 0;
}
