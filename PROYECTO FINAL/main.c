#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

typedef struct cli cli; 
//Define nodo de lista (cli)
struct cli {
	char *nombre; //Nombre del cliente
	int plan; //Velocidad en mbps
	cli *sig;
};

typedef struct nodo nodo;
//Define nodo de arbol
struct nodo {
	int plan; //ID de busqueda
	float precio; //Precio del plan
	nodo *left;
	nodo *right;
};

//Funcion para lipar pantalla
void CLEAR() {
	system("cls");
}

//Funcion que genera una pausa
void PAUSA() {
	printf("\n\nPresione una tecla para continuar...");
	fflush(stdin);
	getch();
	CLEAR();
}

//Genera arbol de planes
nodo *GEN_ARBOL() {
	
}

/*Genera la lista de clientes a partir
de un archivo de texto*/
cli *GEN_LISTA_CLIENTES() {
	cli *lista, *nn;
	char linea[64];
	FILE *arch;
	if((arch = fopen("Clientes.txt","r")) == 0) {
		printf("\nERROR: No se pudo abrir el archivo para su lectura");
		return NULL;
	}
	while(fgets(linea,64,arch)) {
		nn = (cli*)malloc(sizeof(cli));
		linea[strcspn(linea,"\n")] = 0;
		nn->nombre = (char*)malloc(strlen(linea)+1);
		strcpy(nn->nombre,linea);
		if(!fscanf(arch, "%d", &nn->plan)) {
			free(nn->nombre);
			free(nn);
			perror("\nERROR: No se pudo leer el archivo correctamente");
			return NULL;
		}
		fgetc(arch);
		nn->sig = lista;
		lista = nn;
	}
	fclose(arch);
	return lista;
}

//Genera las facturas de los clientes
void GEN_FACTURAS(nodo *arbol) {
	cli *clientes = GEN_LISTA_CLIENTES();
	if(clientes == NULL) {
		PAUSA();
		return;
	}
	
	PAUSA();
}

int main(int argc, char *argv[]) {
	nodo *arbol = GEN_ARBOL();
	for(;;) {
		printf("\nMenu");
		printf("\n1 - Generar facturas");
		printf("\nS - Salir");
		printf("\nOpcion: ");
		fflush(stdin);
		switch(getch()) {
			
		case '1':
			CLEAR();
			GEN_FACTURAS(arbol);
			break;
			
		case 's':
		case 'S':
			return 0;
			
		default:
			break;
		}
	}
}

