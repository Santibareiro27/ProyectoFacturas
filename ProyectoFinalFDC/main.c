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
	cli *lista = NULL, *nn;
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

int DIGITOS(int num) {
	int dig = 0;
	while(num != 0) {
		dig++;
		num /= 10;
	}
	return dig;
}

//Genera las facturas de los clientes
void GEN_FACTURAS(Nodo *arbol) {
	//variable entera c, contador para enumerar la cantidad de facturas
	//cli
	int c=1;
	char fact[15]="factura_";
	cli *clientes = GEN_LISTA_CLIENTES(),*aux=NULL;
	if(clientes != NULL) {
		aux=clientes;
		while(aux!=NULL){
			fact[8] = (char)(c/100+48);
			fact[9] = (char)(c/10+48);
			fact[10] = (char)(c+48);
			strcat(fact,".txt");
			fact[15] = '\0';
			FILE *factura=fopen(fact,"w");
			if (factura == NULL) {
				perror("Error en la creación del archivo, procediendo a cerrar\n\n");
				exit(1);
			}
			Nodo *planAux=BUSCAR(arbol,aux->plan);
			if(planAux!=NULL){
				fprintf(factura, " ________________________________________\n");
				fprintf(factura, "| Cliente: %s", aux->nombre);
				for(int i = 0; i < 30 - strlen(aux->nombre); i++) {
					fprintf(factura, " ");
				}
				fprintf(factura, "|\n");
				fprintf(factura, "| Plan: %d MB", planAux->plan.mb);
				for(int i = 0; i < 30 - DIGITOS(planAux->plan.mb); i++) {
					fprintf(factura, " ");
				}
				fprintf(factura, "|\n");
				fprintf(factura, "| Precio: %d", planAux->plan.precio);
				for(int i = 0; i < 31 - DIGITOS(planAux->plan.precio); i++) {
					fprintf(factura, " ");
				}
				fprintf(factura, "|\n");
				fprintf(factura, "|________________________________________|\n");
				fclose(factura);
				c++;
			} else {
				//fprintf(factura, "Cliente: %s\nPlan: %d\nPlan no encontrado\n\n", aux->nombre, aux->plan);
				printf("Plan no encontrado");
				fclose(factura);
				remove(fact);
				PAUSE();
			}
			aux=aux->sig;
			strcpy(fact, "factura_");
		}
	}else{
		perror("Error en la creacion del archivo, procediendo a cerrar\n\n");
		exit(1);
	}
	printf("facturas generadas");
	PAUSE();
}

int main(int argc, char *argv[]) {
	Nodo *arbol = GEN_ARBOL();
	if(arbol == NULL) {
		return 1;
	}
	//SPREORDER(arbol);
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

