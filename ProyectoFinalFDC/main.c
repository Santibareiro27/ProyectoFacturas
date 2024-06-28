#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include "arbolib.h"
#define PLANESCSV "planes.csv"
#define CLIENTESCSV "clientes.csv"
#define TEMPCSV "temp.csv"

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
						exit(1);					}
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
			break;
			
		case '3':
			if(!ELIMINAR_CLIENTE(Clientes)) {
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
			break;
			
		case '4':
			break;
			
		case '5':
			printf("Procediendo a eliminar clientes moradores de mas de 31 dias\n");
			Clientes = ELIMINADODEMORADORES(Clientes);
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

void MENU_FACTURA() {
	char opc;
	for(;;){
		printf("\nAdministracion de facturas");
		printf("\n1 - Generar facturas");
		printf("\n2 - Modificar recargo por mora");
		printf("\n3 - Modificar fechas limite de pago");
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
			
			break;
			
		case '3':
			
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
			printf("\nError de ingreso\n");
			PAUSE();
			break;
		}
	}
}

