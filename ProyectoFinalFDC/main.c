#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include "arbolib.h"
#define PLANESCSV "planes.csv"
#define CLIENTESCSV "clientes.csv"
#define CONFIGCSV "config.csv"

/*
Crear un apartado de gestion de clientes:
*Dar de baja cliente, lo borra del arbol AVL y del archivo
*Cambiar plan del cliente, dice el DNI y nombre, y si su zona lo permite cambiar el plan
*Analisis de cantidad de clientes deudores

Crear un apartado de gestion de planes:
*Agregar mas datos a la generacion de las facturas
*Agregar un recargo si en el dato del cliente "FechaUltimoPago" es entre 11 y 20 y si supera los 20 dias, lo da de baja
*Cambiar porcentajes de recargo
*/

//nodo Arbol = NULL; //arbol de clientes
plan *Planes = NULL;
Nodo *Clientes;
int Recargo;
char Dia_Recargo[3];

int COMPROBAR_CONFIGCSV() {
	//func: comprueba si el archivo csv de
	//configuraciones existe, sino lo crea
	//pos: codigo de exito/error
	FILE *arch;
	arch = fopen(CONFIGCSV,"r");
	if(!arch) {
		arch = fopen(CONFIGCSV,"w");
		if(!arch) {
			printf("\nERROR: No se pudo crear el archivo de configuraciones");
			return 1;
		}
		fputs("CLAVE,VALOR,\n",arch);
		fputs("recargo por mora,10,\n",arch);
		fputs("dia de recargo,07,\n",arch);
		Recargo = 10;
		strcpy(Dia_Recargo,"07");
		printf("\nADVERTENCIA: No se encontraron configuraciones, por lo ");
		printf("tanto se establecieron las opciones predeterminadas");
		fclose(arch);
		PAUSE();
		return 2;
	}
	fclose(arch);
	return 0;
}

void CARGAR_CONFIG() {
	char linea[50], *dato;
	FILE *arch = fopen(CONFIGCSV,"r");
	if(!arch) {
		printf("\nSe ha producido un error\a\n");
		PAUSE();
		exit(11);
	}
	fgets(linea, sizeof(linea), arch);
	while(fgets(linea, sizeof(linea), arch)) {
		dato = strtok(linea, ",");
		if(strcmp(dato,"recargo por mora") == 0) {
			dato = strtok(NULL, ",");
			Recargo = atoi(dato);
		}
		else if(strcmp(dato,"dia de recargo") == 0) {
			dato = strtok(NULL, ",");
			strcpy(Dia_Recargo,dato);
		}
	}
	fclose(arch);
}

int GUARDAR_CONFIG() {
	FILE *arch = fopen(CONFIGCSV,"w");
	if(!arch) {
		printf("\nERROR: No se pudieron guardar los cambios");
		return 1;
	}
	fprintf(arch, "CLAVE,VALOR,\n");
	fprintf(arch, "recargo por mora,%d,\n",Recargo);
	fprintf(arch, "dia de recargo,%s,\n",Dia_Recargo);
	fclose(arch);
	printf("\nLos cambios han sido guardados");
	return 0;
}

void MOD_RECARGO() {
	do {
		printf("\nPorcentaje de recargo por mora actual: %d", Recargo);
		printf("\nIngrese el nuevo porcentaje de recargo: ");
		if(scanf("%d", &Recargo) == 0) {
			Recargo = -1;
		}
		if(Recargo < 1 || Recargo > 100) {
			printf("\nERROR: Ingreso no valido\n");
		}
	} while(Recargo < 1 || Recargo > 100);
	if(GUARDAR_CONFIG()) {
		CARGAR_CONFIG();
	}
}

void MOD_FECHA() {
	int dia;
	do {
		printf("\nFecha de recargo por mora actual: dia %s de cada mes", Dia_Recargo);
		printf("\nIngrese el nuevo dia que se aplicara el recargo por mora: ");
		if(scanf("%d", &dia) == 0) {
			dia = 0;
		}
		if(dia < 1 || dia > 28) {
			printf("\nERROR: Ingreso no valido\n");
		}
	} while(dia < 1 || dia > 28);
	if(dia < 10) {
		sprintf(Dia_Recargo,"0%d",dia);
	}
	else {
		sprintf(Dia_Recargo,"%d",dia);
	}
	if(GUARDAR_CONFIG()) {
		CARGAR_CONFIG();
	}
}

void MENU_FACTURA() {
	char opc;
	for(;;){
		printf("\nAdministracion de facturas");
		printf("\n1 - Generar facturas");
		printf("\n2 - Modificar recargo por mora");
		printf("\n3 - Modificar fecha limite de pago");
		printf("\n4 - Analisis de clientes deudores");
		printf("\nS - Salir al menu principal");
		printf("\nOpcion: ");
		fflush(stdin);
		opc = getch();
		CLEAR();
		switch(opc) {
			
		case '1':
			
			break;
			
		case '2':
			MOD_RECARGO();
			PAUSE();
			break;
			
		case '3':
			MOD_FECHA();
			PAUSE();
			break;
			
		case '4':
			
			break;
			
		case 's':
		case 'S':
			return;
			
		default:
			printf("\nError de ingreso\n");
			PAUSE();
			break;
		}
	}
}

void MENU_PLANES() {
	char opc;
	for(;;){
		printf("\nAdministracion de planes");
		printf("\n1 - Mostrar planes");
		printf("\n2 - Crear plan");
		printf("\n3 - Eliminar plan");
		printf("\n4 - Editar plan");
		printf("\nS - Salir al menu principal");
		printf("\nOpcion: ");
		fflush(stdin);
		opc = getch();
		CLEAR();
		switch(opc) {
			
		case '1':
			MOSTRAR_PLANES(Planes);
			PAUSE();
			break;
			
		case '2':
			if(CREAR_PLAN(&Planes) == 0) {
				if(GUARDAR_PLANES(Planes,PLANESCSV) != 0) {
					LIMPIAR_PLANES(&Planes);
					if(GEN_LISTA_PLANES(&Planes, PLANESCSV) != 0) {
						LIMPIAR_PLANES(&Planes);
						printf("\nSe ha producido un error\a\n");
						PAUSE();
						exit(1);					
					}
				}
				else {
					printf("\nEl plan se ha creado con exito");
				}
			}
			PAUSE();
			break;
			
		case '3':
			if(Planes == NULL) {
				printf("\nNo hay planes en la lista");
			}
			else {
				if(ELIMINAR_PLAN(&Planes) == 0) {
					if(GUARDAR_PLANES(Planes,PLANESCSV) != 0) {
						LIMPIAR_PLANES(&Planes);
						if(GEN_LISTA_PLANES(&Planes, PLANESCSV) != 0) {
							LIMPIAR_PLANES(&Planes);
							printf("\nSe ha producido un error\a\n");
							PAUSE();
							exit(1);
						}
					}
					else {
						printf("\nEl plan se ha eliminado con exito");
					}
				}
				else {
					printf("\nNo se encontro el plan a eliminar");
				}
				if(Planes == NULL) {
					printf("\n\nADVERTENCIA: No hay ningun plan en la lista");
				}
			}
			PAUSE();
			break;
			
		case '4':
			if(Planes == NULL) {
				printf("\nNo hay planes en la lista");
			}
			else {
				EDITAR_PLAN(Planes);
				if(GUARDAR_PLANES(Planes,PLANESCSV) != 0) {
					LIMPIAR_PLANES(&Planes);
					if(GEN_LISTA_PLANES(&Planes, PLANESCSV) != 0) {
						LIMPIAR_PLANES(&Planes);
						printf("\nSe ha producido un error\a\n");
						PAUSE();
						exit(1);
					}
				}
			}
			PAUSE();
			break;
			
		case 's':
		case 'S':
			return;
			
		default:
			printf("\nError de ingreso\n");
			PAUSE();
			break;
		}
	}
}

void MENU_CLIENTES() {
	char opc;
	int band = 1;
	for(;;){
		printf("\nAdministracion de clientes");
		printf("\n1 - Mostrar clientes");
		printf("\n2 - Agregar cliente");
		printf("\n3 - Eliminar cliente");
		printf("\n4 - Editar cliente");
		printf("\n5 - Realizar un limpiado de clientes moradores");
		printf("\nS - Salir al menu principal");
		printf("\nOpcion: ");
		fflush(stdin);
		opc = getch();
		CLEAR();
		switch(opc) {
			
		case '1':
			if(Clientes == NULL){
				printf("No hay clientes en el arbol\n");
			}else{
				printf("Mostrando datos\n");
				printf("DNI,CUIT,Apellido,Nombre,CondicionIVA,Direccion,Zona,Plan,FechaUltimoPago\n");
				PRINT_INORDER(Clientes);
				PAUSE();
			}
			break;
			
		case '2':
			if(Planes!=NULL){
				do {
					Clientes = INGRESAR_CLIENTE(Clientes,Planes);
					PAUSE();
					do {
						printf("\nDesea agregar otro cliente?(S/N): ");
						fflush(stdin);
						opc = getch();
						opc = toupper(opc);
						if(opc != 'S' && opc != 'N') {
							printf("\nError de ingreso\n");
							PAUSE();
						}
					} while(opc != 'S' && opc != 'N');
					CLEAR();
				} while(opc == 'S');
			}else{
				printf("\nNo hay planes en la lista, primero agregue un plan");
				MENU_PLANES();
			}
			break;
			
		case '3':
			band = 1;
			Clientes=ELIMINADO(Clientes,&band);
			break;
		case '4':
			band = 1;
			Clientes = ELIMINADO(Clientes,&band);
			if(band){
				printf("\nEdite sus datos: \n ");
				Clientes = INGRESAR_CLIENTE(Clientes,Planes);
			}
			break;
			
		case '5':
			printf("Procediendo a eliminar clientes moradores de mas de 31 dias\n");
			Clientes = ELIMINADODEMORADORES(Clientes, Dia_Recargo);
			printf("Elimado completado\n");
			PAUSE();
			break;
			
		case 's':
		case 'S':
			return;
			
		default:
			printf("\nError de ingreso\n");
			PAUSE();
			break;
		}
	}
}

int main() {
	char opc;
	if(COMPROBAR_PLANESCSV(PLANESCSV) == 0) {
		if(GEN_LISTA_PLANES(&Planes, PLANESCSV) == 2) {
			LIMPIAR_PLANES(&Planes);
		}
		if(Planes == NULL) {
			printf("\nADVERTENCIA: No hay ningun plan en la lista");
			PAUSE();
		}
	}
	else {
		printf("\nSe ha producido un error\a\n");
		PAUSE();
		exit(1);
	}
	if(COMPROBAR_CLIENTESCSV(CLIENTESCSV) == 0) {
		Clientes = GEN_ARBOL(CLIENTESCSV);
		if(Clientes == NULL) {
			printf("\nADVERTENCIA: No hay ningun cliente en la lista");
			PAUSE();
		}
	}
	else {
		printf("\nSe ha producido un error\a\n");
		PAUSE();
		exit(1);
	}
	switch(COMPROBAR_CONFIGCSV()) {
		
	case 0:
		CARGAR_CONFIG();
		break;
		
	case 1:
		printf("\nSe ha producido un error\a\n");
		PAUSE();
		exit(10);
		break;
	}
	for(;;){
		printf("\nMenu");
		printf("\n1 - Administrar planes");
		printf("\n2 - Administrar clientes");
		printf("\n3 - Facturacion");
		printf("\nS - Salir");
		printf("\nOpcion: ");
		fflush(stdin);
		opc = getch();
		CLEAR();
		switch(opc) {
			
		case '1':
			MENU_PLANES();
			break;
			
		case '2':
			MENU_CLIENTES();
			break;
			
		case '3':
			MENU_FACTURA();
			break;
			
		case 's':
		case 'S':
			LIMPIAR_PLANES(&Planes);
			while(Clientes!=NULL){
				Clientes=REMOVEN(Clientes,Clientes->cli.cuit);		
			}
			return 0;
			
		default:
			printf("\nError de ingreso\n");
			PAUSE();
			break;
		}
	}
}

