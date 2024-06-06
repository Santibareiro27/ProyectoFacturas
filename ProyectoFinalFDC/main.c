#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "arbol.h"

#ifdef _WIN32
#define CLS "cls"
#else
#define CLS "clear"
#endif

typedef struct cli cli; 
//Define nodo de lista (cli)
struct cli {
	char *nombre; //Nombre del cliente
	int plan; //Velocidad en mbps
	cli *sig;
};

//Funcion para lipar pantalla
void CLEAR() {
	system(CLS);
}

//Funcion que genera una pausa
void PAUSE() {
	printf("\n\nPresione una tecla para continuar...");
	fflush(stdin);
	getch();
	CLEAR();
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
		PAUSE();
		return;
	}
	
	
	PAUSE();
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
			CLEAR();
			break;
		}
	}
}

