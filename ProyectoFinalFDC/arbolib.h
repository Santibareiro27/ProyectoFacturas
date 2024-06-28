#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include "planeslib.h"
#define CLIENTESCSV "clientes.csv"
//Eliminar a los usuarios por medio de eleccion de dni
//Hacer una funcion de busqueda en el arbol que revise la fecha de los clientes 
// y si la dif con respecto al de la maquina es mayor o igual a 31 dias entonces lo elimina

//Define estructura de cli
typedef struct cliente {
	unsigned long int dni;
	int mb;
	char *apellido,*nombre,*iva,*direccion,*cuit;
	char zona;
	time_t fechaUltPago;
}cliente;

//Define nodo de arbol
typedef struct Nodo {
	cliente cli;
	struct Nodo *left;
	struct Nodo *right;
	int height;
}Nodo;

int GET_HEIGHT(Nodo *N) {
	//Obtener altura del Nodo
	if(N == NULL) {
		return 0;
	}
	return N->height;
} 

int MAX(int a, int b) {
	//Funcion para conseguir el maximo entre dos enteros
	return (a > b)? a : b;
} 

Nodo *CREAR(cliente cli) {
	//func: crea un nodo
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

Nodo *rightRotate(Nodo *y) {
	// Funcion para rotar a la derecha el subárbol con raiz
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

Nodo *leftRotate(Nodo *x) {
	// Funcion para rotar a la izquierda el subárbol con raiz x
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
	
Nodo *INSERTAR_NODO(Nodo *aux, cliente cli) {
	//func: nsercion nodo
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
	//func: convierte de fecha en un array a formato time_t
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
	//func: convierte de formato time_t a fecha en un array
	struct tm *tm_info;
	tm_info = localtime(&tiempo);
	strftime(buffer, buffer_size, "%d/%m/%Y", tm_info);
}

Nodo *GEN_ARBOL(char *archname) {
	//func: genera arbol de clientes
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
		cli.dni = atol(dato);
		
		dato = strtok(NULL, ",");
		cli.cuit = (char*)malloc(strlen(dato)+1);
		strcpy(cli.cuit,dato);
		
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
		cli.mb = atoi(dato);
		
		dato = strtok(NULL, ",");
		cli.fechaUltPago=atotime_t(dato);
		
		tree = INSERTAR_NODO(tree, cli);
	}
	fclose(arch);
	return tree;
}


void PREORDER(struct Nodo *root) {
	if(root != NULL) {
		printf("\n%lu,", root->cli.dni);
		printf("%s,", root->cli.cuit);
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

void GUARDADO(struct Nodo *root, FILE *arch) {
	//fun: recorre el arbol cargando datos de clientes al archivo de clientes
	//pre: arbol que recorre y nombre del archivo destino
	if(root != NULL) {
		char fecha[11];
		time_ttoa(root->cli.fechaUltPago,fecha,sizeof(fecha));
		fprintf(arch, "%lu,%s,%s,%s,%s,%s,%c,%d,%s,\n", root->cli.dni,root->cli.cuit,root->cli.apellido,root->cli.nombre,root->cli.iva,root->cli.direccion,root->cli.zona,root->cli.mb,fecha);
		GUARDADO(root->left, arch);
		GUARDADO(root->right, arch);
	}
} 

int GUARDAR_CLIENTES(Nodo *root, char *archname) {
	//fun: guarda los clientes en la base de datos
	//pre: arbol a guardar y nombre del archivo destino
	//pos: codigo de exito/error
	FILE *arch = fopen(archname,"w");
	if(!arch) {
		printf("\nERROR: No se pudieron guardar los cambios");
		return 1;
	}
	fprintf(arch, "DNI,CUIT,Apellido,Nombre,CondicionIVA,Direccion,Zona,Plan,FechaUltimoPago,\n");
	GUARDADO(root, arch);
	fclose(arch);
	return 0;
}

Nodo *BUSCAR(Nodo *aux, int dnibus) {
	//fun: Busca un cliente usando como dato de busqueda su dni
	if(aux == NULL) {
		return NULL;
	}
	if(dnibus == aux->cli.dni) {
		return aux;
	}
	else if(dnibus < aux->cli.dni) {
		return BUSCAR(aux->left, dnibus);
	}
	else if(dnibus > aux->cli.dni) {
		return BUSCAR(aux->right, dnibus);
	}
	return NULL;
}

Nodo *INGRESAR_CLIENTE(Nodo *c, plan *planes){
	printf("\nIngrese los datos del cliente\n");
	char dato[100], opc;
	cliente datosCli;
	do {
		printf("\nDNI: ");
		fflush(stdin);
		if(scanf("%lu",&datosCli.dni) == 0) {
			datosCli.dni = -1;
		}
		if(datosCli.dni < 0 || datosCli.dni > 99999999) {
			printf("\nError de ingreso\n");
			PAUSE();
		}
		else if(BUSCAR(c,datosCli.dni) != NULL) {
			datosCli.dni = -1;
			printf("\nEste dni ya ha sido registrado\n");
			PAUSE();
		}
	} while(datosCli.dni < 0 || datosCli.dni > 99999999);
	CLEAR();
	do {
		opc = 't';
		printf("\nCuit (sin espacios): ");
		fflush(stdin);
		scanf("%s", dato);
		if(strlen(dato) != 11) {
			printf("\nError de ingreso\n");
			opc = 'f';
			PAUSE();
		}
		else {
			for(int i = 0; i < 11; i++) {
				if(isdigit(dato[i]) == 0) {
					printf("\nError de ingreso\n");
					opc = 'f';
					PAUSE();
					break;
				}
			}
		}
	} while(opc == 'f');
	datosCli.cuit = (char*)malloc(strlen(dato)+1);
	strcpy(datosCli.cuit,dato);
	CLEAR();
	printf("\nApellido: ");
	fflush(stdin);
	gets(dato);
	for(int i=0;i<strlen(dato);i++){
		dato[i] = toupper(dato[i]);
	}
	datosCli.apellido = (char*)malloc(strlen(dato)+1);
	strcpy(datosCli.apellido,dato);
	CLEAR();
	printf("\nNombre: ");
	fflush(stdin);
	gets(dato);
	for(int i=0;i<strlen(dato);i++){
		dato[i] = tolower(dato[i]);
	}
	datosCli.nombre = (char*)malloc(strlen(dato)+1);
	strcpy(datosCli.nombre,dato);
	CLEAR();
	do {
		printf("\nCondicion IVA");
		printf("\n1) IVA1");
		printf("\n2) IVA2");
		printf("\n3) IVA3");
		printf("\n4) IVA4");
		printf("\nOpcion: ");
		fflush(stdin);
		opc = getchar();
		switch(opc) {
			
		case '1':
			strcpy(dato,"iva1");
			break;
			
		case '2':
			strcpy(dato,"iva2");
			break;
			
		case '3':
			strcpy(dato,"iva3");
			break;
			
		case '4':
			strcpy(dato,"iva4");
			break;
			
		default:
			printf("\nError de ingreso\n");
			PAUSE();
			break;
		}
	} while(opc < '1' || opc > '4');
	datosCli.iva = (char*)malloc(strlen(dato)+1);
	strcpy(datosCli.iva,dato);
	CLEAR();
	printf("\nDireccion (sin espacios): ");
	fflush(stdin);
	gets(dato);
	for(int i=0;i<strlen(dato);i++){
		dato[i] = tolower(dato[i]);
	}
	datosCli.direccion = (char*)malloc(strlen(dato)+1);
	strcpy(datosCli.direccion,dato);
	CLEAR();
	do {
		printf("\nZona(a/b/c): ");
		fflush(stdin);
		datosCli.zona = tolower(getchar());
		if(datosCli.zona < 'a' || datosCli.zona > 'c') {
			
		}
	} while(datosCli.zona < 'a' || datosCli.zona > 'c');
	datosCli.zona = tolower(datosCli.zona);
	CLEAR();
	plan *aux;
	opc = 'f';
	do {
		MOSTRAR_PLANES(planes);
		printf("\nPlan en MB: ");
		fflush(stdin);
		if(scanf("%d",&datosCli.mb) == 0) {
			datosCli.mb = 0;
		}
		if(datosCli.mb > 0) {
			aux = planes;
			opc = 'f';
			while(aux != NULL && opc != 't') {
				if(aux->mb == datosCli.mb) {
					opc = 't';
					break;
				}
				aux = aux->sig;
			}
			if(opc == 'f') {
				printf("\nNo se encontro el plan\n");
				PAUSE();
			}
			else if(datosCli.zona > aux->zona) {
				opc = 'f';
				printf("\nEl plan seleccionado(%c) no es compatible con esta zona(%c)\n",datosCli.zona,aux->zona);
				PAUSE();
			}
		}
		else {
			printf("\nError de ingreso\n");
			PAUSE();
		}
	} while(opc == 'f');
	CLEAR();
	do {
		printf("\nFecha del ultimo pago (dd/mm/yyyy): ");
		fflush(stdin);
		gets(dato);
		datosCli.fechaUltPago = atotime_t(dato);
	} while(datosCli.fechaUltPago == -1);
	
	c = INSERTAR_NODO(c,datosCli);
	if(GUARDAR_CLIENTES(c,CLIENTESCSV)==0){
		printf("Guardado correctamente\n");
	}else{
		c=REMOVEN(c,datosCli.dni);
	}
	return c;
}

Nodo *ELIMINADODEMORADORES(Nodo *root) {
	double diff=0;
	if(root == NULL){
		return NULL;
	}
	time_t rawtime;
	time (&rawtime);
	diff = difftime(rawtime,root->cli.fechaUltPago);
	printf("%lf", diff);
	if(diff>2678400){
		root=REMOVEN(root,root->cli.dni);
		
	}
	if(root!=NULL){
	root->left=ELIMINADODEMORADORES(root->left);
	}
	if(root!=NULL){
	root->right=ELIMINADODEMORADORES(root->right);
	}
	return root;	
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
