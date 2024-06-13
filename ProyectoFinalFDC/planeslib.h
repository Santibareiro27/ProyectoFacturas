#include <stdio.h>
#include <stdlib.h>

typedef struct plan{ //define estructura de lista de planes
	int mb; //velocidad en mbps
	char zona; //zona de alcance
	double precio; //precio del plan
	struct plan *sig;
}plan;

void MOSTRAR_PLANES(plan *aux) {
	if(aux == NULL) {
		printf("\nLa lista esta vacia");
	}
	while(aux != NULL) {
		printf("\nPlan: %d mb\nZona: %c \nPrecio: %.2lf\n", aux->mb, aux->zona, aux->precio);
		aux = aux->sig;
	}
}

plan *LIMPIAR_PLANES(plan *aux) {
	plan *memory;
	while(aux != NULL) {
		memory = aux;
		aux = aux->sig;
		free(memory);
	}
	return aux;
}

int COMPROBAR_CSV(char *archname) {
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

plan *GUARDAR_PLANES(plan *lista, char *archname) {
	FILE *arch = fopen(archname,"w");
	if(!arch) {
		printf("\nERROR: No se pudieron guardar los cambios");
		lista = LIMPIAR_PLANES(lista);
		return NULL;
	}
	else {
		fprintf(arch, "MB,ZONA,PRECIO,\n");
		plan *aux = lista;
		while(aux != NULL) {
			fprintf(arch, "%d,%c,%.2lf,\n", aux->mb, aux->zona, aux->precio);
			aux = aux->sig;
		}
		fclose(arch);
	}
	return lista;
}

void EDITAR_PLAN(plan *aux) {
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
		printf("\nIngrese la nueva zona disponible del plan (a,b,c,d): ");
		fflush(stdin);
		aux->zona = tolower(getchar());
		if(aux->zona < 'a' || aux->zona > 'd') {
			printf("\nERROR: Ingreso no valido");
		}
	} while(aux->zona < 'a' || aux->zona > 'd');
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

plan *ELIMINAR_PLAN(plan *lista) {
	int elim;
	do {
		printf("\nIngrese los mb del plan que desea eliminar: ");
		fflush(stdin);
		if(!scanf("%d", &elim)) {
			elim = 0;
		}
		if(elim <= 0) {
			printf("\nERROR: Ingreso no valido");
		}
	} while(elim <= 0);
	plan *ant = NULL, *act = lista;
	while(act != NULL) {
		if(act->mb == elim) {
			if(ant == NULL) {
				lista = lista->sig;
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
		printf("\nNo se encontro el plan a eliminar");
	}
	else {
		printf("\nEl plan se ha eliminado con exito");
	}
	return lista;
}

plan *CREAR_PLAN(plan *lista) {
	//fun: crea plan nuevo
	//pre: ubicacion de lista
	//pos: ubicacion nueva de lista
	plan *nn = (plan*)malloc(sizeof(plan));
	if(nn == NULL) {
		printf("\nERROR: Memoria insuficiente");
		return lista;
	}
	do {
		printf("\nIngrese los mb del plan (numero mayor a 0): ");
		fflush(stdin);
		if(!scanf("%d", &nn->mb)) {
			nn->mb = 0;
		}
		if(nn->mb <= 0) {
			printf("\nERROR: Ingreso no valido\n");
		}
		else {
			plan *aux = lista;
			while(aux != NULL) {
				if(nn->mb == aux->mb) {
					printf("\nERROR: Ya existe un plan con esta cantidad de mb\n");
					nn->mb = 0;
					break;
				}
				aux = aux->sig;
			}
		}
	} while(nn->mb <= 0);
	do {
		printf("\nIngrese la zona disponible del plan (a,b,c,d): ");
		fflush(stdin);
		nn->zona = tolower(getchar());
		if(nn->zona < 'a' || nn->zona > 'd') {
			printf("\nERROR: Ingreso no valido\n");
		}
	} while(nn->zona < 'a' || nn->zona > 'd');
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
	nn->sig = lista;
	lista = nn;
	return lista;
}

plan *GEN_LISTA_PLANES(char *archname) {
	plan *lista = NULL, *nn;
	FILE *arch;
	char linea[50], *dato;
	arch = fopen(archname,"r");
	if(!arch) {
		printf("\nERROR: No se encontro \"%s\"", archname);
		return NULL;
	}
	fgets(linea, sizeof(linea), arch);
	while(fgets(linea, sizeof(linea), arch)) {
		nn = (plan*)malloc(sizeof(plan));
		if(nn == NULL) {
			printf("\nERROR: Memoria insuficiente");
			lista = LIMPIAR_PLANES(lista);
			return NULL;
		}
		dato = strtok(linea, ",");
		nn->mb = atoi(dato);
		dato = strtok(NULL, ",");
		nn->zona = tolower(*dato);
		dato = strtok(NULL, ",");
		nn->precio = atof(dato);
		nn->sig = lista;
		lista = nn;
	}
	fclose(arch);
	return lista;
}
