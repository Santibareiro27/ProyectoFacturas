#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "arbolib.h"
#define PLANESCSV "planes.csv"
#define CLIENTESCSV "clientes.csv"
#define CONFIGCSV "config.csv"

plan *Planes;
Nodo *Clientes;
int Recargo;
char Dia_Recargo[3];
char Fecha_Facturacion[11];
int FacturaN;

void GET_FECHA_FACTURACION() {
	//funct: obtiene la siguiente fecha de facturacion
	time_ttoa(time(NULL),Fecha_Facturacion,sizeof(Fecha_Facturacion));
	if(Fecha_Facturacion[4] == '9') {
		Fecha_Facturacion[4] = '0';
		Fecha_Facturacion[3]++;
	}
	else if(Fecha_Facturacion[3] == '1' && Fecha_Facturacion[4] == '2') {
		Fecha_Facturacion[3] = '0';
		Fecha_Facturacion[4] = '1';
		Fecha_Facturacion[9]++;
	}
	else {
		Fecha_Facturacion[4]++;
	}
	strncpy(Fecha_Facturacion,"01",2);
}

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
		fputs("numero de factura,0,\n",arch);
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
	//funct: obtiene las configuraciones de el archivo csv
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
		else if(strcmp(dato,"numero de factura") == 0) {
			dato = strtok(NULL, ",");
			FacturaN = atoi(dato);
		}
	}
	fclose(arch);
}

int GUARDAR_CONFIG() {
	//funct: guarda las configuraciones actuales en el csv
	FILE *arch = fopen(CONFIGCSV,"w");
	if(!arch) {
		printf("\nERROR: No se pudieron guardar los cambios");
		return 1;
	}
	fprintf(arch, "CLAVE,VALOR,\n");
	fprintf(arch, "recargo por mora,%d,\n",Recargo);
	fprintf(arch, "dia de recargo,%s,\n",Dia_Recargo);
	fprintf(arch, "numero de factura,%d,\n",FacturaN);
	fclose(arch);
	printf("\nLos cambios han sido guardados");
	return 0;
}

void MOD_RECARGO() {
	//funct: modifica el recargo por mora
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
	//modifica el dia en el que se aplica el recargo por mora
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

void GEN_FACT(Nodo *tree, time_t moratime) {
	//funct: genera la factura de 1 cliente en un archivo txt
	//pre: obtiene el nodo del cliente y la fecha en la
	//que se aplica el recargo por mora en formato time_t
	if(tree != NULL) {
		FacturaN++;
		char archname[21], N[9];
		double total;
		plan *aux = Planes;
		while(aux != NULL) {
			if(aux->mb == tree->cli.mb) {
				break;
			}
			aux = aux->sig;
		}
		if(aux == NULL) {
			return;
		}
		total = aux->precio * 1.21;
		//crea el nombre del archivo
		strcpy(archname,"facturaN");
		sprintf(N,"%d",FacturaN);
		for(int i = 0; i < (8 - strlen(N)); i++) {
			strcat(archname,"0");
		}
		strcat(archname,N);
		strcat(archname,".txt");
		//comienza la escritura
		FILE *arch = fopen(archname,"w");
		fprintf(arch," ");
		for(int i = 0; i < 100; i++) {
			fprintf(arch,"_");
		}
		fprintf(arch," \n");
		fprintf(arch,"|%100c|\n",' ');
		//datos de emisor y factura
		fprintf(arch,"| CHAQUE TU ROUTER SRL%69c",' ');
		fprintf(arch,"FACTURA ");
		if(strcmp(tree->cli.iva,"ri") == 0 || strcmp(tree->cli.iva,"mt") == 0) {
			fprintf(arch,"A |\n");
		}
		else {
			fprintf(arch,"B |\n");
		}
		archname[16] = 0;
		fprintf(arch,"|%80cComp. Nro: %s |\n",' ', strpbrk(archname,"0"));
		fprintf(arch,"|%71cFecha de Emision: %s |\n",' ',Fecha_Facturacion);
		fprintf(arch,"|%100c|\n",' ');
		fprintf(arch,"|Direccion Comercial: Av. Sarmiento 1060, Obera%34c",' ');
		fprintf(arch,"CUIT: 30-27122004-9 |\n");
		fprintf(arch,"|%100c|\n",' ');
		fprintf(arch,"|Condicion frente al IVA: Responsable Inscripto%20c",' ');
		fprintf(arch,"Inicio de Actividades: 01/03/2022 |\n");
		fprintf(arch,"|");
		for(int i = 0; i < 100; i++) {
			fprintf(arch,"_");
		}
		fprintf(arch,"|\n");
		strncpy(Fecha_Facturacion,Dia_Recargo,2);
		fprintf(arch,"|%76cVencimiento: %s |\n",' ',Fecha_Facturacion);
		strncpy(Fecha_Facturacion,"01",2);
		fprintf(arch,"|");
		for(int i = 0; i < 100; i++) {
			fprintf(arch,"_");
		}
		fprintf(arch,"|\n");
		//datos de cliente
		char completo[200];
		if(strcmp(tree->cli.iva,"ri") == 0 || strcmp(tree->cli.iva,"mt") == 0) {
			strcpy(completo,tree->cli.nombre);
		}
		else {
			strcpy(completo,tree->cli.apellido);
			strcat(completo," ");
			strcat(completo,tree->cli.nombre);
		}
		fprintf(arch,"|CUIT/CUIL: %s%78c|\n",tree->cli.cuit,' ');
		if(strlen(completo) >= 50) {
			completo[50] = 0;
		}
		fprintf(arch,"|Apellido y Nombre/Razon Social: %53s%15c|\n",completo,' ');
		if(strcmp(tree->cli.iva,"ri") == 0) {
			strcpy(completo,"Responsable Inscripto");
		}
		else if(strcmp(tree->cli.iva,"mt") == 0) {
			strcpy(completo,"Responsable Monotributo");
		}
		else if(strcmp(tree->cli.iva,"cf") == 0) {
			strcpy(completo,"Consumidor Final");
		}
		else {
			strcpy(completo,"Sujeto Exento");
		}
		fprintf(arch,"|Condicion frente al IVA: %60s%15c|\n",completo,' ');
		fprintf(arch,"|Domicilio: %74s%15c|\n",tree->cli.direccion,' ');
		fprintf(arch,"|");
		for(int i = 0; i < 100; i++) {
			fprintf(arch,"_");
		}
		fprintf(arch,"|\n");
		//calculo del total
		fprintf(arch,"| CANT. |  DESCRIPCION %46c|  VALOR UNIT.  |  VALOR TOTAL  |\n",' ');
		fprintf(arch,"|_______|____________________________________________________________|_______________|_______________|\n");
		if(strcmp(tree->cli.iva,"ri") == 0 || strcmp(tree->cli.iva,"mt") == 0) {
			fprintf(arch,"|   1    Servicio de internet %39c   %12.2lf    %12.2lf |\n",' ',aux->precio,aux->precio);
		}
		else {
			fprintf(arch,"|   1    Servicio de internet %39c   %12.2lf    %12.2lf |\n",' ',aux->precio * 1.21,aux->precio* 1.21);
		}
		for(int i = 0; i < 10; i++) {
			fprintf(arch,"|%100c|\n",' ');
		}
		fprintf(arch,"|");
		for(int i = 0; i < 100; i++) {
			fprintf(arch,"_");
		}
		fprintf(arch,"|\n");
		if(strcmp(tree->cli.iva,"ri") == 0 || strcmp(tree->cli.iva,"mt") == 0) {
			fprintf(arch,"|%67c|             IVA: $%13.2lf|\n",' ',aux->precio * 0.21);
		}
		else {
			fprintf(arch,"|                                                                   |                                |\n");
		}
		//mora
		if(moratime < tree->cli.fechaUltPago) {
			fprintf(arch,"|%67c|Recargo por mora: $%13.2lf|\n",' ',total * (Recargo)/100);
			total += total*((double)Recargo/100);
		}
		else {
			fprintf(arch,"|                                                                   |                                |\n");
		}
		fprintf(arch,"|%67c|           Total: $%13.2lf|\n",' ',total);
		fprintf(arch,"|___________________________________________________________________|________________________________|\n");
		fclose(arch);
		GEN_FACT(tree->left, moratime);
		GEN_FACT(tree->right, moratime);
	}
}

void SETUP_FACTURAS() {
	//funct: crea datos necesarios para las facturas
	char fechamora[11];
	time_ttoa(time(NULL),fechamora,sizeof(fechamora));
	strncpy(fechamora,Dia_Recargo,2);
	GEN_FACT(Clientes, atotime_t(fechamora));
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
		printf("\nS - Salir al menu principal");
		printf("\nOpcion: ");
		fflush(stdin);
		opc = getch();
		CLEAR();
		switch(opc) {
			
		case '1':
			do {
				printf("\nSeguro que desea generar las facturas?(S/N): ");
				fflush(stdin);
				opc = toupper(getchar());
				if(opc != 'S' && opc != 'N') {
					printf("\nERROR: Ingreso no valido\n");
				}
			} while(opc != 'S' && opc != 'N');
			SETUP_FACTURAS();
			PAUSE();
			break;
			
		case '2':
			MOD_RECARGO();
			PAUSE();
			break;
			
		case '3':
			MOD_FECHA();
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
	char opc, mesaux[3], fechaux[11];
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
			Clientes = ELIMINADO(Clientes,&band);
			PAUSE();
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
			time_ttoa(time(NULL),fechaux,sizeof(fechaux));
			mesaux[0] = fechaux[3];
			mesaux[1] = fechaux[4];
			printf("Procediendo a eliminar clientes moradores de mas de 31 dias\n");
			Clientes = ELIMINAR_MORADORES(Clientes,mesaux);
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
	GET_FECHA_FACTURACION();
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

