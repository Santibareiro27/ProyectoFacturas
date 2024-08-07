#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include "planeslib.h"
#define CLIENTESCSV "clientes.csv"

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

int GET_MAX(int a, int b) {
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
	y->height = GET_MAX(GET_HEIGHT(y->left), GET_HEIGHT(y->right)) + 1;
	x->height = GET_MAX(GET_HEIGHT(x->left), GET_HEIGHT(x->right)) + 1;
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
	x->height = GET_MAX(GET_HEIGHT(x->left), GET_HEIGHT(x->right))+1;
	y->height = GET_MAX(GET_HEIGHT(y->left), GET_HEIGHT(y->right))+1;
	// Devuelve nueva raiz
	return y;
} 

int GET_BALANCE(Nodo *N){
	//func: balancea el arbol
	if(N == NULL) {
		return 0;
	}
	return GET_HEIGHT(N->left) - GET_HEIGHT(N->right);
} 	
	
Nodo *INSERTAR_NODO(Nodo *tree, cliente cli) {
	//func: insercion nodo
	//pre: arbol de clientes, datos de un cliente
	//pos: nodo del arbol
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
	tree->height = 1 + GET_MAX(GET_HEIGHT(tree->left),GET_HEIGHT(tree->right));
	
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
	//func: genera árbol de clientes a partir del archivo .csv si este existe		
	//pos: nombre del archivo
	//pre: nodo del arbol
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
		dato[0] = toupper(dato[0]);
		for(int i = 0; i < strlen(dato)-1; i++) {
			if(dato[i] == ' ') {
				dato[i+1] = toupper(dato[i+1]);
			}
		}
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
	//func: muestra "en orden" todo el arbol
	//pos: raiz del arbol
	if(root != NULL) {
		PRINT_INORDER(root->left);
		printf("\n");
		printf(" |%lu", root->cli.dni);
		printf(" |%s", root->cli.cuit);
		printf(" |%s", root->cli.apellido);
		printf(" |%s", root->cli.nombre);
		printf(" |%s", root->cli.iva);
		printf(" |%s", root->cli.direccion);
		printf(" |%c", root->cli.zona);
		printf(" |%d", root->cli.mb);
		char fecha[11];
		time_ttoa(root->cli.fechaUltPago, fecha, sizeof(fecha));
		printf("|%s | ", fecha);
		printf("\n");
		PRINT_INORDER(root->right);
	}
}

Nodo *GET_MIN(Nodo *aux) {
	//func consigue el mínimo id de uno de los nodos del arbol 	
	Nodo *act = aux;
	while(act->left != NULL) {
		act = act->left;
	}
	return act;
}

void FREECLI(Nodo *aux) {
	//func: libera un cliente
	//pos: nodo del arbol
	free(aux->cli.cuit);
	free(aux->cli.apellido);
	free(aux->cli.nombre);
	free(aux->cli.iva);
	free(aux->cli.direccion);
}

Nodo *REMOVEN(Nodo *aux, char *cuit) {
	//func: elimina del arbol un hijo segun la key siendo el CUIT/CUIL
	//pos: raiz del arbol y la key
	//pre:raiz del arbol
	if(aux == NULL) {
		return aux;
	}
	if(strcmp(cuit,aux->cli.cuit) < 0) {
		aux->left = REMOVEN(aux->left, cuit);
	}
	else if(strcmp(cuit,aux->cli.cuit) > 0) {
		aux->right = REMOVEN(aux->right, cuit);
	}
	else {
		if((aux->left == NULL) || (aux->right == NULL)) {
			Nodo *temp = aux->left ? aux->left : aux->right;
			
			if (temp == NULL) {
				temp = aux;
				aux = NULL;
			}
			else {
				*aux = *temp;
			}
			free(temp);
		}
		else {
			Nodo* temp = GET_MIN(aux->right);
			FREECLI(aux);
			aux->cli.cuit = malloc(strlen(temp->cli.cuit) + 1);
			strcpy(aux->cli.cuit, temp->cli.cuit);
			aux->cli.apellido = malloc(strlen(temp->cli.apellido) + 1);
			strcpy(aux->cli.apellido, temp->cli.apellido);
			aux->cli.nombre = malloc(strlen(temp->cli.nombre) + 1);
			strcpy(aux->cli.nombre, temp->cli.nombre);
			aux->cli.iva = malloc(strlen(temp->cli.iva) + 1);
			strcpy(aux->cli.iva, temp->cli.iva);
			aux->cli.direccion = malloc(strlen(temp->cli.direccion) + 1);
			strcpy(aux->cli.direccion, temp->cli.direccion);
			aux->cli.dni = temp->cli.dni;
			aux->cli.zona = temp->cli.zona;
			aux->cli.mb = temp->cli.mb;
			aux->cli.fechaUltPago = temp->cli.fechaUltPago;
			
			aux->right = REMOVEN(aux->right, temp->cli.cuit);
		}
	}
	if (aux == NULL) {
		return aux;
	}
	aux->height = 1 + GET_MAX(GET_HEIGHT(aux->left), GET_HEIGHT(aux->right));
	int balance = GET_BALANCE(aux);
	if (balance > 1 && GET_BALANCE(aux->left) >= 0) {
		return rightRotate(aux);
	}
	if (balance > 1 && GET_BALANCE(aux->left) < 0) {
		aux->left = leftRotate(aux->left);
		return rightRotate(aux);
	}
	if (balance < -1 && GET_BALANCE(aux->right) <= 0) {
		return leftRotate(aux);
	}
	if (balance < -1 && GET_BALANCE(aux->right) > 0) {
		aux->right = rightRotate(aux->right);
		return leftRotate(aux);
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
	//func: Busca un cliente usando como dato de busqueda su CUIL/CUIT
	//pre: raiz del arbol y el CUIL/CUIT
	//pos: raiz del arbol
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

int EDITAR_CLIENTE(Nodo *tree, plan *planes){
	//func: edicion de un cliente del arbol
	//pre: raiz del arbol, la lista de planes
	//pos: codigo de exito/error
	char dato[100], opc;
	Nodo *nodocli;
	if(tree == NULL) {
		printf("\nNo hay clientes en el arbol");
		return 1;
	}
	do {
		opc = 't';
		printf("Ingrese el CUIT/CUIL de un cliente para editarlo: ");
		fflush(stdin);
		scanf("%s", dato);
		if(strlen(dato) != 11) {
			printf("\nERROR: Ingreso no valido\n");
			opc = 'f';
			PAUSE();
		}
		else {
			for(int i = 0; i < strlen(dato); i++) {
				if(isdigit(dato[i]) == 0) {
					printf("\nERROR: Ingreso no valido\n");
					opc = 'f';
					PAUSE();
					break;
				}
			}
		}
	} while(opc == 'f');
	nodocli = BUSCAR(tree,dato);
	if(nodocli == NULL) {
		printf("\nNo se encontro el cliente\n");
		return 1;
	}
	//muestra
	printf("\nSe encontro el cliente:\n");
	do {
		printf("\nCUIT/CUIL: %s",nodocli->cli.cuit);
		if(*(nodocli->cli.cuit) == '2') {
			printf("\nDNI: %ld",nodocli->cli.dni);
			printf("\nApellido: %s",nodocli->cli.apellido);
		}
		printf("\nNombre: %s",nodocli->cli.nombre);
		printf("\nCondicion IVA: ");
		switch(*(nodocli->cli.iva)) {
		case 'r':
			printf("Responsable Inscripto");
			break;
		case 'm':
			printf("Responsable Monotributo");
			break;
		case 'c':
			printf("Consumidor Final");
			break;
		case 's':
			printf("Sujeto Exento");
			break;
		}
		printf("\nDireccion: %s",nodocli->cli.direccion);
		printf("\nZona: %c",nodocli->cli.zona);
		printf("\nPlan: %d",nodocli->cli.mb);
		time_ttoa(nodocli->cli.fechaUltPago,dato,sizeof(dato));
		printf("\nFecha de ultimo pago: %s\n",dato);
		printf("\nDesea editarlo?(S/N): ");
		fflush(stdin);
		opc = toupper(getchar());
		CLEAR();
		if(opc != 'S' && opc != 'N') {
			printf("\nERROR: Ingreso no valido\n");
		}
	} while(opc != 'S' && opc != 'N');
	if(opc == 'N') {
		printf("\nSe cancelo la accion\n");
		return 1;
	}
	else {
		CLEAR();
	}
	//editar de CUIT/CUIL
	do {
		opc = 't';
		printf("\nCUIT/CUIL anterior: %s", nodocli->cli.cuit);
		printf("\nCUIT/CUIL nuevo: ");
		fflush(stdin);
		scanf("%s", dato);
		if(strlen(dato) != 11) {
			printf("\nERROR: Ingreso no valido\n");
			opc = 'f';
			PAUSE();
		}
		else {
			for(int i = 0; i < 11; i++) {
				if(isdigit(dato[i]) == 0) {
					printf("\nERROR: Ingreso no valido\n");
					opc = 'f';
					PAUSE();
					break;
				}
			}
		}
		if(strcmp(dato,nodocli->cli.cuit) == 0) {
			opc = 's';
		}
		else if(BUSCAR(tree,dato) != NULL) {
			opc = 'f';
			printf("\nEste CUIT/CUIL ya ha sido registrado\n");
			PAUSE();
		}
	} while(opc == 'f');
	if(opc != 's') {
		strcpy(nodocli->cli.cuit,dato);
	}
	CLEAR();
	//editar de DNI
	if(*(nodocli->cli.cuit) == '2') {
		strcpy(dato,(nodocli->cli.cuit + 2));
		dato[8] = 0;
		nodocli->cli.dni = atoi(dato);
		CLEAR();
	}
	else {
		nodocli->cli.dni = 0;
	}
	//editar Apellido
	if(*(nodocli->cli.cuit) == '2') {
		printf("\nApellido anterior: %s", nodocli->cli.apellido);
		printf("\nApellido nuevo: ");
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
	if(strcmp(dato,nodocli->cli.apellido) != 0) {
		free(nodocli->cli.apellido);
		nodocli->cli.apellido = (char*)malloc(strlen(dato)+1);
		strcpy(nodocli->cli.apellido,dato);
	}
	//editar Nombre
	printf("\nNombre anterior: %s", nodocli->cli.nombre);
	printf("\nNombre nuevo: ");
	fflush(stdin);
	gets(dato);
	for(int i=0;i<strlen(dato);i++){
		dato[i] = tolower(dato[i]);
	}
	dato[0] = toupper(dato[0]);
	for(int i = 0; i < strlen(dato)-1; i++) {
		if(dato[i] == ' ') {
			dato[i+1] = toupper(dato[i+1]);
		}
	}
	if(strcmp(dato, nodocli->cli.nombre) != 0) {
		free(nodocli->cli.nombre);
		nodocli->cli.nombre = (char*)malloc(strlen(dato)+1);
		strcpy(nodocli->cli.nombre,dato);
	}
	CLEAR();
	//editar IVA
	do {
		printf("\nCondicion frente al IVA anterior: ");
		switch(*(nodocli->cli.iva)) {
		case 'r':
			printf("Responsable Inscripto");
			break;
		case 'm':
			printf("Responsable Monotributo");
			break;
		case 'c':
			printf("Consumidor Final");
			break;
		case 's':
			printf("Sujeto Exento");
			break;
		}
		printf("\nCondicion frente al IVA nueva");
		printf("\n1) Responsable Inscripto");
		printf("\n2) Monotributista");
		printf("\n3) Consumidor Final");
		printf("\n4) Sujeto Exento");
		printf("\nOpcion: ");
		fflush(stdin);
		opc = getchar();
		switch(opc) {
			
		case '1':
			strcpy(dato,"ri");
			break;
			
		case '2':
			strcpy(dato,"mt");
			break;
			
		case '3':
			strcpy(dato,"cf");
			break;
			
		case '4':
			strcpy(dato,"se");
			break;
			
		default:
			printf("\nERROR: Ingreso no valido\n");
			PAUSE();
			break;
		}
	} while(opc < '1' || opc > '4');
	strcpy(nodocli->cli.iva,dato);
	CLEAR();
	//editar direccion
	printf("\nDireccion antigua: %s", nodocli->cli.direccion);
	printf("\nDireccion nueva: ");
	fflush(stdin);
	gets(dato);
	if(strcmp(dato,nodocli->cli.direccion) != 0) {
		free(nodocli->cli.direccion);
		nodocli->cli.direccion = (char*)malloc(strlen(dato)+1);
		strcpy(nodocli->cli.direccion,dato);
	}
	CLEAR();
	//editar zona
	do {
		printf("\nZona antigua: %c", nodocli->cli.zona);
		printf("\nZona nueva(a/b/c): ");
		fflush(stdin);
		nodocli->cli.zona = tolower(getchar());
		if(nodocli->cli.zona < 'a' || nodocli->cli.zona > 'c') {
			printf("\nERROR: Ingreso no valido\n");
		}
	} while(nodocli->cli.zona < 'a' || nodocli->cli.zona > 'c');
	nodocli->cli.zona = tolower(nodocli->cli.zona);
	CLEAR();
	plan *aux;
	opc = 'f';
	do {
		MOSTRAR_PLANES(planes);
		printf("\nPlan antiguo: %d", nodocli->cli.mb);
		printf("\nPlan nuevo (en MB): ");
		fflush(stdin);
		if(scanf("%d",&nodocli->cli.mb) == 0) {
			nodocli->cli.mb = 0;
		}
		if(nodocli->cli.mb > 0) {
			aux = planes;
			opc = 'f';
			while(aux != NULL && opc != 't') {
				if(aux->mb == nodocli->cli.mb) {
					opc = 't';
					break;
				}
				aux = aux->sig;
			}
			if(opc == 'f') {
				printf("\nNo se encontro el plan\n");
				PAUSE();
			}
			else if(nodocli->cli.zona > aux->zona) {
				opc = 'f';
				printf("\nEl plan seleccionado(%c) no es compatible con esta zona(%c)\n",nodocli->cli.zona,aux->zona);
				PAUSE();
			}
		}
		else {
			printf("\nERROR: Ingreso no valido\n");
			PAUSE();
		}
	} while(opc == 'f');
	CLEAR();
	char fechaux[11];
	time_ttoa(nodocli->cli.fechaUltPago,fechaux,sizeof(fechaux));
	do {
		printf("\nFecha anterior del ultimo pago: %s",fechaux);
		printf("\nFecha del ultimo pago (dd/mm/yyyy): ");
		fflush(stdin);
		gets(dato);
		nodocli->cli.fechaUltPago = atotime_t(dato);
		if(nodocli->cli.fechaUltPago == -1) {
			PAUSE();
		}
		else if(difftime(time(NULL),nodocli->cli.fechaUltPago) < 0) {
			nodocli->cli.fechaUltPago = -1;
			printf("\nERROR: Fecha no valida");
			PAUSE();
		}
	} while(nodocli->cli.fechaUltPago < 0);
	if(GUARDAR_CLIENTES(tree,CLIENTESCSV) != 0){
		printf("No se pudo guardar el cambio\n");
		return 2;
	}
	printf("Guardado correctamente\n");
	return 0;
}

int ELIMINAR_CLIENTE(Nodo *tree,int *band) {
	//func: Eliminación de un cliente existente
	//pre: raiz del arbol y bandera por referencia
	//pos: codigo de exito/error
	char key[12], opc;
	if(tree == NULL) {
		printf("\nNo hay clientes en el arbol");
		(*band)=0;
		return 2;
	}
	do {
		opc = 't';
		printf("Ingrese el CUIT/CUIL de un cliente para eliminarlo: ");
		fflush(stdin);
		scanf("%s", key);
		if(strlen(key) != 11) {
			printf("\nERROR: Ingreso no valido\n");
			opc = 'f';
			PAUSE();
		}
		else {
			for(int i = 0; i < strlen(key); i++) {
				if(isdigit(key[i]) == 0) {
					printf("\nERROR: Ingreso no valido\n");
					opc = 'f';
					PAUSE();
					break;
				}
			}
		}
	} while(opc == 'f');
	if(BUSCAR(tree,key) == NULL) {
		printf("\nNo se encontro el cliente\n");
		PAUSE();
		(*band)=0;
		return 1;
	}
	tree = REMOVEN(tree,key);
	return 0;
}

Nodo *INGRESAR_CLIENTE(Nodo *tree, plan *planes){
	//func: Ingreso de un cliente al arbol
	//pre: raiz del arbol, la lista de planes
	//pos: raiz del arbol con hijo agregado
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
			printf("\nERROR: Ingreso no valido\n");
			opc = 'f';
			PAUSE();
		}
		else {
			for(int i = 0; i < 11; i++) {
				if(isdigit(dato[i]) == 0) {
					printf("\nERROR: Ingreso no valido\n");
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
	dato[0] = toupper(dato[0]);
	for(int i = 0; i < strlen(dato)-1; i++) {
		if(dato[i] == ' ') {
			dato[i+1] = toupper(dato[i+1]);
		}
	}
	strcpy(datosCli.nombre,dato);
	CLEAR();
	do {
		printf("\nCondicion frente al IVA");
		printf("\n1) Responsable Inscripto");
		printf("\n2) Monotributista");
		printf("\n3) Consumidor Final");
		printf("\n4) Sujeto Exento");
		printf("\nOpcion: ");
		fflush(stdin);
		opc = getchar();
		switch(opc) {
			
		case '1':
			strcpy(dato,"ri");
			break;
			
		case '2':
			strcpy(dato,"mt");
			break;
			
		case '3':
			strcpy(dato,"cf");
			break;
			
		case '4':
			strcpy(dato,"se");
			break;
			
		default:
			printf("\nERROR: Ingreso no valido\n");
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
			printf("\nERROR: Ingreso no valido\n");
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
			printf("\nERROR: Ingreso no valido\n");
			PAUSE();
		}
	} while(opc == 'f');
	CLEAR();
	do {
		printf("\nFecha del ultimo pago (dd/mm/yyyy): ");
		fflush(stdin);
		gets(dato);
		datosCli.fechaUltPago = atotime_t(dato);
		if(datosCli.fechaUltPago == -1) {
			PAUSE();
		}
		else if(difftime(time(NULL),datosCli.fechaUltPago) < 0) {
			datosCli.fechaUltPago = -1;
			printf("\nERROR: Fecha no valida");
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
	
Nodo *ELIMINAR_MORADORES(Nodo *root, char *mesaux) {
	//func: elimina aquellos clientes que son moradores
	//pre: raiz del arbol, y un arreglo de caracteres del dia
	//pos: raiz del arbol
	if(root == NULL){
		return NULL;
	}
	root->left = ELIMINAR_MORADORES(root->left, mesaux);
	root->right = ELIMINAR_MORADORES(root->right, mesaux);
	char fechapago[11];
	time_ttoa(root->cli.fechaUltPago,fechapago,sizeof(fechapago));
	if(fechapago[3] != mesaux[0] || fechapago[4] != mesaux[1]){
		Nodo *temp = REMOVEN(root,root->cli.cuit);
		return temp;
	}
	return root;	
}

int COMPROBAR_CLIENTESCSV(char *archname) {
	//func: comprueba la existencia del archivo .csv de clientes
	//pre: nombre del archivo
	//pos: codigo de exito/error
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
	//func: intermediario para la eliminación de un cliente existente y revisa que se elimine correctamente
	//pre: raiz del arbol y bandera por referencia
	//pos: raiz del arbol con hijo eliminado
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

void LIMPIAR_ARBOL(Nodo *aux) {
	if (aux == NULL) {
		return;
	}
	LIMPIAR_ARBOL(aux->left);
	LIMPIAR_ARBOL(aux->right);
	FREECLI(aux);
	free(aux);
}
