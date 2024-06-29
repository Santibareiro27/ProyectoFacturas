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
	nn->cli = cli;
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
	y->height = MAX(GET_HEIGHT(y->left), GET_HEIGHT(y->right)) + 1;
	x->height = MAX(GET_HEIGHT(x->left), GET_HEIGHT(x->right)) + 1;
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
	
Nodo *INSERTAR_NODO(Nodo *tree, cliente cli) {
	//func: insercion nodo
	if(tree == NULL) {
		return CREAR(cli);
	}
	if(strcmp(cli.cuit,tree->cli.cuit) < 0) {
		tree->left = INSERTAR_NODO(tree->left, cli);
	}
	else if(strcmp(cli.cuit,tree->cli.cuit) > 0) {
		tree->right = INSERTAR_NODO(tree->right, cli);
	}
	else {
		return tree;
	}
	tree->height = 1 + MAX(GET_HEIGHT(tree->left),GET_HEIGHT(tree->right));
	
	int balance = GET_BALANCE(tree);
	// Left Left Caso
	if (balance > 1 && strcmp(cli.cuit,tree->left->cli.cuit) < 0) {
		return rightRotate(tree);
	}
	// Right Right Caso
	if(balance < -1 && strcmp(cli.cuit,tree->right->cli.cuit) > 0) {
		return leftRotate(tree);
	}
	// Left Right Caso
	if (balance > 1 && strcmp(cli.cuit,tree->left->cli.cuit) > 0) {
		tree->left = leftRotate(tree->left);
		return rightRotate(tree);
	}
	// Right Left Caso
	if (balance < -1 && strcmp(cli.cuit,tree->right->cli.cuit) < 0) {
		tree->right = rightRotate(tree->right);
		return leftRotate(tree);
	} 
	return tree;
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


void PRINT_INORDER(Nodo *root) {
	if(root != NULL) {
		PRINT_INORDER(root->left);
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
		PRINT_INORDER(root->right);
	}
}

char *GETMIN(Nodo *aux) {
	while(aux->left != NULL) {
		aux = aux->left;
	}
	return aux->cli.cuit;
}

void FREECLI(Nodo *aux) {
	free(aux->cli.apellido);
	free(aux->cli.nombre);
	free(aux->cli.iva);
	free(aux->cli.direccion);
	free(aux->cli.cuit);
}

Nodo *REMOVEN(Nodo *aux, char *key) {
	if(aux == NULL) {
		return NULL;
	}
	if(strcmp(key,aux->cli.cuit) < 0) {
		aux->left = REMOVEN(aux->left, key);
	}
	else if(strcmp(key,aux->cli.cuit) > 0) {
		aux->right = REMOVEN(aux->right, key);
	}
	else {
		if(aux->left == NULL && aux->right == NULL) {
			FREECLI(aux);
			free(aux);
			return NULL;
		}
		else if(aux->left == NULL) {
			Nodo *temp = aux->right;
			FREECLI(aux);
			free(aux);
			return temp;
		}
		else if(aux->right == NULL) {
			Nodo *temp = aux->left;
			FREECLI(aux);
			free(aux);
			return temp;
		}
		else {
			char rmin[12];
			strcpy(rmin,GETMIN(aux->right));
			aux->cli.cuit = rmin;
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
	fprintf(arch, "DNI,CUIT/CUIL,Apellido,Nombre,CondicionIVA,Direccion,Zona,Plan,FechaUltimoPago,\n");
	GUARDADO(root, arch);
	fclose(arch);
	return 0;
}

Nodo *BUSCAR(Nodo *aux, char *cui) {
	//fun: Busca un cliente usando como dato de busqueda su CUIL/CUIT
	if(aux == NULL) {
		return NULL;
	}
	if(strcmp(cui,aux->cli.cuit) == 0) {
		return aux;
	}
	else if(strcmp(cui,aux->cli.cuit) < 0) {
		return BUSCAR(aux->left, cui);
	}
	else if(strcmp(cui,aux->cli.cuit) > 0) {
		return BUSCAR(aux->right, cui);
	}
	return NULL;
}

Nodo *BUSCAR_DNI(Nodo *aux, int dni) {
	//fun: Busca un cliente usando como dato de busqueda su DNI
	if(aux == NULL) {
		return NULL;
	}
	if(dni == aux->cli.dni) {
		return aux;
	}
	else if(dni < aux->cli.dni) {
		return BUSCAR_DNI(aux->left, dni);
	}
	else if(dni > aux->cli.dni) {
		return BUSCAR_DNI(aux->right, dni);
	}
	return NULL;
}

int ELIMINAR_CLIENTE(Nodo *tree,int *band) {
	char key[12], opc;
	if(tree == NULL) {
		printf("\nNo hay clientes en el arbol");
		(*band)=0;
		return 2;
	}
	do {
		opc = 't';
		printf("Ingrese el DNI o CUIT/CUIL de un cliente para eliminarlo: ");
		fflush(stdin);
		scanf("%s", key);
		if(strlen(key) != 11 && strlen(key) != 8 && strlen(key) != 7) {
			printf("\nError de ingreso\n");
			opc = 'f';
			PAUSE();
		}
		else {
			for(int i = 0; i < strlen(key); i++) {
				if(isdigit(key[i]) == 0) {
					printf("\nError de ingreso\n");
					opc = 'f';
					PAUSE();
					break;
				}
			}
		}
	} while(opc == 'f');
	if(strlen(key) == 8 || strlen(key) == 7) {
		Nodo *aux = BUSCAR_DNI(tree,atoi(key));
		if(aux == NULL) {
			printf("\nNo se encontro el cliente\n");
			PAUSE();
			(*band)=0;
			return 1;
		}
		strcpy(key, aux->cli.cuit);
	}
	else {
		if(BUSCAR(tree,key) == NULL) {
			printf("\nNo se encontro el cliente\n");
			PAUSE();
			(*band)=0;
			return 1;
		}
	}
	tree = REMOVEN(tree,key);
	return 0;
}

Nodo *INGRESAR_CLIENTE(Nodo *tree, plan *planes){
	printf("\nIngrese los datos del cliente\n");
	char dato[100], opc;
	cliente datosCli;
	//carga de CUIT/CUIL
	do {
		opc = 't';
		printf("\nCUIT/CUIL (sin espacios ni guiones): ");
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
		if(BUSCAR(tree,dato) != NULL) {
			opc = 'f';
			printf("\nEste CUIT/CUIL ya ha sido registrado\n");
			PAUSE();
		}
	} while(opc == 'f');
	datosCli.cuit = (char*)malloc(strlen(dato)+1);
	strcpy(datosCli.cuit,dato);
	CLEAR();
	//carga de DNI
	if(*(datosCli.cuit) == '2') {
		strcpy(dato,(datosCli.cuit + 2));
		dato[8] = 0;
		datosCli.dni = atoi(dato);
		CLEAR();
	}
	else {
		datosCli.dni = 0;
	}
	if(*(datosCli.cuit) == '2') {
		printf("\nApellido: ");
		fflush(stdin);
		gets(dato);
		for(int i=0;i<strlen(dato);i++){
			dato[i] = toupper(dato[i]);
		}
		CLEAR();
	}
	else {
		strcpy(dato,"0");
	}
	datosCli.apellido = (char*)malloc(strlen(dato)+1);
	strcpy(datosCli.apellido,dato);
	
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
		printf("\nCondicion frente al IVA");
		printf("\n1) Responsable Inscripto");
		printf("\n2) Monotributista");
		printf("\n3) Consumidor Final");
		printf("\n4) Sujeto Extento");
		printf("\nOpcion: ");
		fflush(stdin);
		opc = getchar();
		switch(opc) {
			
		case '1':
			strcpy(dato,"RI");
			break;
			
		case '2':
			strcpy(dato,"MT");
			break;
			
		case '3':
			strcpy(dato,"CF");
			break;
			
		case '4':
			strcpy(dato,"SE");
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
	printf("\nDireccion: ");
	fflush(stdin);
	gets(dato);
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
	time_t rawtime;
	time(&rawtime);
	do {
		printf("\nFecha del ultimo pago (dd/mm/yyyy): ");
		fflush(stdin);
		gets(dato);
		datosCli.fechaUltPago = atotime_t(dato);
		if(datosCli.fechaUltPago == -1) {
			PAUSE();
		}
		else if(difftime(rawtime,datosCli.fechaUltPago) < 0) {
			datosCli.fechaUltPago = -1;
			printf("\nFecha no valida");
			PAUSE();
		}
	} while(datosCli.fechaUltPago < 0);
	
	tree = INSERTAR_NODO(tree,datosCli);
	
	if(GUARDAR_CLIENTES(tree,CLIENTESCSV)==0){
		printf("Guardado correctamente\n");
	}else{
		printf("No se pudo guardar el cambio\n");
		tree = REMOVEN(tree,datosCli.cuit);
	}
	return tree;
}
	
	int EDITAR_CLIENTES(Nodo *tree){
		if(tree == NULL) {
			printf("\nNo hay clientes en el arbol");
			return 2;
		}
		char key[12], opc;
		if(tree == NULL) {
			printf("\nNo hay clientes en el arbol");
			return 2;
		}
		do {
			opc = 't';
			printf("Ingrese el DNI o CUIT/CUIL de un cliente para eliminarlo: ");
			fflush(stdin);
			scanf("%s", key);
			if(strlen(key) != 11 && strlen(key) != 8 && strlen(key) != 7) {
				printf("\nError de ingreso\n");
				opc = 'f';
				PAUSE();
			}
			else {
				for(int i = 0; i < strlen(key); i++) {
					if(isdigit(key[i]) == 0) {
						printf("\nError de ingreso\n");
						opc = 'f';
						PAUSE();
						break;
					}
				}
			}
		} while(opc == 'f');
		if(strlen(key) == 8 || strlen(key) == 7) {
			Nodo *aux = BUSCAR_DNI(tree,atoi(key));
			if(aux == NULL) {
				printf("\nNo se encontro el cliente\n");
				PAUSE();
				return 1;
			}
			strcpy(key, aux->cli.cuit);
		}
		else {
			if(BUSCAR(tree,key) == NULL) {
				printf("\nNo se encontro el cliente\n");
				PAUSE();
				return 1;
			}
		}
		tree = REMOVEN(tree,key);
		return 0;
	}

Nodo *ELIMINADODEMORADORES(Nodo *root) {
	double diff = 0;
	if(root == NULL){
		return NULL;
	}
	time_t rawtime;
	time (&rawtime);
	diff = difftime(rawtime,root->cli.fechaUltPago);
	printf("%lf", diff);
	if(diff>2678400){
		root=REMOVEN(root,root->cli.cuit);
		
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
		fputs("DNI,CUIT/CUIL,Apellido,Nombre,CondicionIVA,Direccion,Zona,Plan,FechaUltimoPago,\n",arch);
	}
	fclose(arch);
	return 0;
}
Nodo *ELIMINADO(Nodo *Clientes,int *band){
if(!ELIMINAR_CLIENTE(Clientes,&(*band))) {
	if(GUARDAR_CLIENTES(Clientes,CLIENTESCSV) == 0) {
		printf("Guardado correctamente\n");
	}
	else {
		printf("No se pudo guardar el cambio\n");
		while(Clientes != NULL) {
			REMOVEN(Clientes, Clientes->cli.cuit);
		}
		Clientes = GEN_ARBOL(CLIENTESCSV);
		if(Clientes == NULL) {
			printf("\nSe ha producido un error\a\n");
			PAUSE();
			exit(1);
		}
	}
}
return Clientes;
}
