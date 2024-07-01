#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct plan{ //define estructura de lista de planes
	int mb; //velocidad en mbps
	char zona; //zona de alcance
	double precio; //precio del plan
	struct plan *sig;
}plan;

void CLEAR() {
	//func: limpia pantalla
	system("cls");
}

void PAUSE() {
	//func: genera una pausa
	printf("\n\nPresione una tecla para continuar...");
	fflush(stdin);
	getch();
	CLEAR();
}

void MOSTRAR_PLANES(plan *lista) {
	//func: muestra la lista
	//pre: lista a leer
	if(lista == NULL) {
		printf("\nLa lista esta vacia");
	}
	while(lista != NULL) {
		printf("\nPlan: %d mb\nZona: %c \nPrecio: %.2lf\n", lista->mb, lista->zona, lista->precio);
		lista = lista->sig;
	}
}

void LIMPIAR_PLANES(plan **aux) {
	//func: elimina la lista y limpia la memoria
	//pre: lista por referencia
	plan *memory;
	while(*aux != NULL) {
		memory = *aux;
		*aux = (*aux)->sig;
		free(memory);
	}
}

int COMPROBAR_PLANESCSV(char *archname) {
	//func: comprueba si el archivo csv
	//de planes existe, sino lo crea
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
		fputs("MB,ZONA,PRECIO,\n",arch);
	}
	fclose(arch);
	return 0;
}

int GUARDAR_PLANES(plan *lista, char *archname) {
	//func: guarda la lista en la base de datos
	//pre: lista a guardar y nombre del archivo destino
	//pos: codigo de exito/error
	FILE *arch = fopen(archname,"w");
	if(!arch) {
		printf("\nERROR: No se pudieron guardar los cambios");
		return 1;
	}
	fprintf(arch, "MB,ZONA,PRECIO,\n");
	while(lista != NULL) {
		fprintf(arch, "%d,%c,%.2lf,\n", lista->mb, lista->zona, lista->precio);
		lista = lista->sig;
	}
	fclose(arch);
	return 0;
}

void EDITAR_PLAN(plan *aux) {
	//func: edita un plan existente
	//pre: lista por referencia
	int num;
	do {
		printf("\nIngrese los mb del plan que desea editar: ");
		fflush(stdin);
		if(!scanf("%d", &num)) {
			num = 0;
		}
		if(num <= 0) {
			printf("\nERROR: Ingreso no valido");
		}
	} while(num <= 0);
	while(aux != NULL && num != aux->mb) {
		aux = aux->sig;
	}
	if(aux == NULL) {
		printf("\nNo se encontro el plan");
		return;
	}
	printf("\nPlan de: %d gb\nZona: %c\nPrecio: %.2lf\n", aux->mb, aux->zona, aux->precio);
	do {
		printf("\nIngrese la nueva zona disponible del plan (a,b,c): ");
		fflush(stdin);
		aux->zona = tolower(getchar());
		if(aux->zona < 'a' || aux->zona > 'c') {
			printf("\nERROR: Ingreso no valido");
		}
	} while(aux->zona < 'a' || aux->zona > 'c');
	do {
		printf("\nIngrese el nuevo precio del plan (numero mayor a 0): ");
		fflush(stdin);
		if(!scanf("%lf", &aux->precio)) {
			aux->precio = 0;
		}
		if(aux->precio <= 0) {
			printf("\nERROR: Ingreso no valido");
		}
	} while(aux->precio <= 0);
}

int ELIMINAR_PLAN(plan **lista) {
	//func: elimina un plan existente
	//pre: lista por referencia
	//pos: codigo de exito/error
	int elim;
	do {
		printf("\nIngrese los mb del plan que desea eliminar('0' para cancelar): ");
		fflush(stdin);
		if(!scanf("%d", &elim)) {
			elim = -1;
		}
		if(elim == 0) {
			return 0;
		}
		if(elim <= -1) {
			printf("\nERROR: Ingreso no valido");
		}
	} while(elim <= -1);
	plan *ant = NULL, *act = *lista;
	while(act != NULL) {
		if(act->mb == elim) {
			if(ant == NULL) {
				*lista = (*lista)->sig;
			}
			else {
				ant->sig = act->sig;
			}
			free(act);
			elim = 0;
			break;
		}
		ant = act;
		act = act->sig;
	}
	if(elim != 0) {
		return 1;
	}
	return 0;
}

int CREAR_PLAN(plan **lista) {
	//func: crea plan nuevo
	//pre: lista por referencia
	//pos: codigo de exito/error
	plan *nn = (plan*)malloc(sizeof(plan));
	if(nn == NULL) {
		printf("\nERROR: Memoria insuficiente");
		return 1;
	}
	do {
		printf("\nIngrese los mb del plan ('0' para cancelar): ");
		fflush(stdin);
		if(!scanf("%d", &nn->mb)) {
			nn->mb = -1;
		}
		if(nn->mb == 0) {
			free(nn);
			return 0;
		}
		if(nn->mb <= -1) {
			printf("\nERROR: Ingreso no valido\n");
		}
		else {
			plan *aux = *lista;
			while(aux != NULL) {
				if(nn->mb == aux->mb) {
					printf("\nERROR: Ya existe un plan con esta cantidad de mb\n");
					nn->mb = -1;
					break;
				}
				aux = aux->sig;
			}
		}
	} while(nn->mb <= -1);
	do {
		printf("\nIngrese la zona disponible del plan (a,b,c): ");
		fflush(stdin);
		nn->zona = tolower(getchar());
		if(nn->zona < 'a' || nn->zona > 'c') {
			printf("\nERROR: Ingreso no valido\n");
		}
	} while(nn->zona < 'a' || nn->zona > 'c');
	do {
		printf("\nIngrese el precio del plan (numero mayor a 0): ");
		fflush(stdin);
		if(!scanf("%lf", &nn->precio)) {
			nn->precio = 0;
		}
		if(nn->precio <= 0) {
			printf("\nERROR: Ingreso no valido\n");
		}
	} while(nn->precio <= 0);
	nn->sig = *lista;
	*lista = nn;
	return 0;
}

int GEN_LISTA_PLANES(plan **lista, char *archname) {
	//func: genera la lista de planes a partir del archivo
	//pre: lista por referencia y archivo fuente
	//pos: codigo de exito/error
	*lista = NULL;
	plan *nn;
	char linea[50], *dato;
	FILE *arch = fopen(archname,"r");
	if(!arch) {
		printf("\nERROR: No se encontro \"%s\"", archname);
		return 1;
	}
	fgets(linea, sizeof(linea), arch);
	while(fgets(linea, sizeof(linea), arch)) {
		nn = (plan*)malloc(sizeof(plan));
		if(nn == NULL) {
			printf("\nERROR: Memoria insuficiente");
			return 2;
		}
		dato = strtok(linea, ",");
		nn->mb = atoi(dato);
		dato = strtok(NULL, ",");
		nn->zona = tolower(*dato);
		dato = strtok(NULL, ",");
		nn->precio = atof(dato);
		nn->sig = *lista;
		*lista = nn;
	}
	fclose(arch);
	return 0;
}
