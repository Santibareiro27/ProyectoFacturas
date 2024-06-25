#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include "planeslib.h"
#include "arbolib.h"

#define PLANESCSV "planes.csv"
#define CLIENTESCSV "clientes.csv"
#define TEMPCSV "temp.csv"

/*
Crear un apartado de gestion de clientes:
*Donde los clientes estaran guardados en un arbol AVL
*Datos en el archivo csv:DNI,Apellido,Nombre,CUIT,CondicionIVA,Direccion,Zona,Plan,FechaUltimoPago
*Añadir clientes
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
Nodo *Clientes = NULL;

void CLEAR() {
	//fun: limpia pantalla
	system("cls");
}

void PAUSE() {
	//fun: genera una pausa
	printf("\n\nPresione una tecla para continuar...");
	fflush(stdin);
	getch();
	CLEAR();
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
			break;
		}
	}
}

void MENU_CLIENTES() {
	char opc;
	for(;;){
		printf("\nAdministracion de clientes");
		printf("\n1 - Mostrar clientes");
		printf("\n2 - Crear cliente");
		printf("\n3 - Eliminar cliente");
		printf("\n4 - Editar cliente");
		printf("\nS - Salir al menu principal");
		printf("\nOpcion: ");
		fflush(stdin);
		opc = getch();
		CLEAR();
		switch(opc) {
			
		case '1':
			printf("Mostrando datos\n");
			printf("CUIT,Apellido,Nombre,CondicionIVA,Direccion,Zona,Plan,FechaUltimoPago\n");
			PREORDER(Clientes);
			PAUSE();
			break;
			
		case '2':
			
			break;
			
		case '3':
			
			break;
			
		case '4':
			
			break;
			
		case 's':
		case 'S':
			return;
			
		default:
			break;
		}
	}
}

void MENU_FACTURA() {
	char opc;
	for(;;){
		printf("\nAdministracion de facturas");
		printf("\n1 - Mostrar formato de factura");
		printf("\n2 - Generar facturas");
		printf("\n3 - Modificar recargo por mora");
		printf("\n4 - Modificar fechas limite de pago");
		printf("\nS - Salir al menu principal");
		printf("\nOpcion: ");
		fflush(stdin);
		opc = getch();
		CLEAR();
		switch(opc) {
			
		case '1':
			
			break;
			
		case '2':
			
			break;
			
		case '3':
			
			break;
			
		case '4':
			
			break;
			
		case 's':
		case 'S':
			return;
			
		default:
			break;
		}
	}
}

int main() {
	char opc;
	if(COMPROBAR_CSV(PLANESCSV) == 0) {
		if(GEN_LISTA_PLANES(&Planes, PLANESCSV) == 2) {
			LIMPIAR_PLANES(&Planes);
		}
		if(Planes == NULL) {
			printf("\nADVERTENCIA: No hay ningun plan en la lista");
			PAUSE();
		}
	}
	else {
		exit(1);
	}
	Clientes = GEN_ARBOL(CLIENTESCSV);
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
			return 0;
			
		default:
			break;
		}
	}
}

