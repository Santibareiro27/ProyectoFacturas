#include <time.h>
#include <string.h>
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
//Eliminar a los usuarios por medio de eleccion de dni
//Hacer una funcion de busqueda en el arbol que revise la fecha de los clientes 
// y si la dif con respecto al de la maquina es mayor o igual a 31 dias entonces lo elimina

typedef struct cliente cliente;
//Define estructura de cli
struct cliente {
	int dni,cuit,mb;
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
	if(cli.dni < aux->cli.dni) {
		aux->left = INSERTAR_NODO(aux->left, cli);
	}
	else if(cli.dni > aux->cli.dni) {
		aux->right = INSERTAR_NODO(aux->right, cli);
	}
	else {
		return aux;
	}
	aux->height = 1 + MAX(GET_HEIGHT(aux->left),GET_HEIGHT(aux->right));
	int balance = GET_BALANCE(aux);
	// Left Left Caso
	if (balance > 1 && cli.dni < aux->left->cli.dni) {
		return rightRotate(aux);
	}
	// Right Right Caso
	if(balance < -1 && cli.dni > aux->right->cli.dni) {
		return leftRotate(aux);
	}
	// Left Right Caso
	if (balance > 1 && cli.dni > aux->left->cli.dni)
	{
		aux->left = leftRotate(aux->left);
		return rightRotate(aux);
	}
	// Right Left Caso
	if (balance < -1 && cli.dni < aux->right->cli.dni)
	{
		aux->right = rightRotate(aux->right);
		return leftRotate(aux);
	} 
	return aux;
}

time_t atotime_t(char *fecha) {
	struct tm tm = {0};
	time_t t;
	int dia, mes, anio;
	if (sscanf(fecha, "%d/%d/%d", &dia, &mes, &anio) != 3) {
		printf("Error al analizar la fecha\n");
		return -1;
	}
	tm.tm_mday = dia;
	tm.tm_mon = mes-1;
	tm.tm_year = anio-1900;
	t = mktime(&tm);
	if (t == -1) {
		printf("Error al convertir la fecha a time_t\n");
		return -1;
	}
	return t;
}

void time_ttoa(time_t tiempo, char *buffer, size_t buffer_size) {
	struct tm *tm_info;
	tm_info = localtime(&tiempo);
	strftime(buffer, buffer_size, "%d/%m/%Y", tm_info);
}

//Genera arbol de clientes
Nodo *GEN_ARBOL(char *archname) {
	Nodo *tree = NULL;
	cliente cli;
	FILE *arch;
	char linea[180], *dato;
	if((arch= fopen(archname,"r")) == 0) {
		printf("\nERROR: No se encontro \"%s\"", archname);
		return NULL;
	}
	fgets(linea, sizeof(linea), arch);
	while (fgets(linea, sizeof(linea), arch)) {
		dato = strtok(linea, ",");
		cli.dni = atoi(dato);
		
		dato = strtok(NULL, ",");
		cli.cuit = atoi(dato);
		
		dato = strtok(NULL, ",");
		for(int i=0;i<strlen(dato);i++){
			dato[i] = toupper(dato[i]);
		}
		cli.apellido = (char*)malloc(strlen(dato)+1);
		strcpy(cli.apellido,dato);
		
		
		dato = strtok(NULL, ",");
		for(int i=0;i<strlen(dato);i++){
			dato[i] = tolower(dato[i]);
		}
		cli.nombre = (char*)malloc(strlen(dato)+1);
		strcpy(cli.nombre,dato);
		
		
		dato = strtok(NULL, ",");
		for(int i=0;i<strlen(dato);i++){
			dato[i] = tolower(dato[i]);
		}
		cli.iva = (char*)malloc(strlen(dato)+1);
		strcpy(cli.iva,dato);
		
		dato = strtok(NULL, ",");
		for(int i=0;i<strlen(dato);i++){
			dato[i] = tolower(dato[i]);
		}
		cli.direccion = (char*)malloc(strlen(dato)+1);
		strcpy(cli.direccion,dato);
		
		dato = strtok(NULL, ",");
		cli.zona = tolower(*dato);
		dato = strtok(NULL, ",");
		cli.mb = atof(dato);
		
		dato = strtok(NULL, ",");
		cli.fechaUltPago=atotime_t(dato);
		
		tree = INSERTAR_NODO(tree, cli);
	}
	fclose(arch);
	return tree;
}


void PREORDER(struct Nodo *root) {
	if(root != NULL) {
		printf("%d,", root->cli.dni);
		printf("%d,", root->cli.cuit);
		printf("%s,", root->cli.apellido);
		printf("%s,", root->cli.nombre);
		printf("%s,", root->cli.iva);
		printf("%s,", root->cli.direccion);
		printf("%c,", root->cli.zona);
		printf("%d,", root->cli.mb);
		char fecha[11];
		time_ttoa(root->cli.fechaUltPago, fecha, sizeof(fecha));
		printf("%s,", fecha);
		PREORDER(root->left);
		PREORDER(root->right);
	}
} 

int GETMIN(Nodo *aux) {
	while(aux->left != NULL) {
		aux = aux->left;
	}
	return aux->cli.dni;
}


Nodo *REMOVEN(Nodo *aux, int val) {
	if(aux == NULL) {
		return NULL;
	}
	
	
	
	if(val < aux->cli.dni) {
		aux->left = REMOVEN(aux->left, val);
	}
	else if(val > aux->cli.dni) {
		aux->right = REMOVEN(aux->right, val);
	}
	else {
		if(aux->left == NULL && aux->right == NULL) {
			free(aux);
			return NULL;
		}
		else if(aux->left == NULL) {
			Nodo *temp = aux->right;
			free(aux);
			return temp;
		}
		else if(aux->right == NULL) {
			Nodo *temp = aux->left;
			free(aux);
			return temp;
		}
		else {
			int rmin = GETMIN(aux->right);
			aux->cli.dni = rmin;
			aux->right = REMOVEN(aux->right, rmin);
		}
	}
	return aux;
}

void ELIMINADODEMORADORES(Nodo *root) {
	double diff=0;
	if(root != NULL) {
	time_t rawtime;
	struct tm *timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	diff=difftime(root->cli.fechaUltPago,timeinfo);
		if(diff>2678400){
			root=REMOVEN(root,root->cli.dni);
		}
		if(root!=NULL){
		ELIMINADODEMORADORES(root->left);
		if(root!=NULL){
		ELIMINADODEMORADORES(root->right);
		}
	}
	}
}


/*Busca el precio del cli usando como
dato de busqueda el cuit del cliente*/
Nodo *BUSCAR(Nodo *aux, int val) {
	if(aux == NULL) {
		return NULL;
	}
	if(val == aux->cli.dni) {
		return aux;
	}
	else if(val < aux->cli.dni) {
		return BUSCAR(aux->left, val);
	}
	else if(val > aux->cli.dni) {
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
		fputs("DNI,CUIT,Apellido,Nombre,CondicionIVA,Direccion,Zona,Plan,FechaUltimoPago,\n",arch);
	}
	fclose(arch);
	return 0;
}
