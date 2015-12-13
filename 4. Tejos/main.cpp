#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <list>
#include <math.h>

using namespace std;

typedef struct {
	string id;
	float xInfIzq;
	float yInfIzq;
	float alto;
	float ancho;
} Dispersor;


typedef struct {
	string id;
	float x;
	float y;
	float radio;
} Tejo;

typedef struct {
	string idTejo;
	string idDispersor;
	float distancia;
} Mensaje;

list<Dispersor*> listaDispersores;
list<Mensaje*> listaMensajes;

void choqueTejo(Tejo tejo);
void cargarArchivoRectangulos();
bool ordenarDispersores(Dispersor* d1, Dispersor* d2);

bool ordenarMensajes(Mensaje* m1, Mensaje* m2) {

	return (m1->distancia < m2->distancia);

}

int main() {

	cargarArchivoRectangulos();

	FILE* archivoTejos = fopen("circulos.txt", "r");
	char fidTejo[256];
	float fx;
	float fy;
	float fradio;
	
	int linea = fscanf(archivoTejos, "%s %f %f %f", fidTejo, &fx, &fy, &fradio);

	Tejo tejo;

	while (linea > 0 || !feof(archivoTejos) ) {
		
		//cout << fidTejo << " " << fx << " " << fy << " " << fradio << endl << endl;

		tejo.id = fidTejo;
		tejo.x = fx;
		tejo.y = fy;
		tejo.radio = fradio;

		choqueTejo(tejo);

		linea = fscanf(archivoTejos, "%s %f %f %f", fidTejo, &fx, &fy, &fradio);
	}

	fclose(archivoTejos);

	// Ordeno lista de mensajes
	listaMensajes.sort(ordenarMensajes);

	list<Mensaje*>::iterator mensajeIni = listaMensajes.begin();
	list<Mensaje*>::const_iterator mensajeFIN = listaMensajes.end();

	cout << "IdTejo\tIdDispersor\tDistancia" << endl << endl;
	for (; mensajeIni != mensajeFIN; mensajeIni++) {
		cout << ((*mensajeIni)->idTejo).c_str() << "\t\t" << ((*mensajeIni)->idDispersor).c_str() << "\t" << (*mensajeIni)->distancia << endl << endl;
	}


	cin.get();
	return 0;

}

void choqueTejo(Tejo tejo) {

	list<Dispersor*>::iterator disp = listaDispersores.begin();
	list<Dispersor*>::const_iterator dispFIN = listaDispersores.end();

	// Lista auxiliar para quedarme con los dispersores que son chocados por el tejo
	list<Dispersor*> dispersoresTocados;

	// Recorro todos los dispersores
	for (; disp != dispFIN; disp++) {

		// Si el tejo llega a chocar al dispersor
		if ( ( tejo.y - tejo.radio ) <= ( (*disp)->yInfIzq + (*disp)->alto ) &&
			 ( tejo.y + tejo.radio ) >= ( (*disp)->yInfIzq ) ) {

			//cout << "Chocara con " << ((*disp)->id).c_str() << endl << endl;

			dispersoresTocados.push_back((*disp));	

		}

	}
	
	// Me quedo con el dispersor que choqué más cercano
	dispersoresTocados.sort(ordenarDispersores);

	float distancia = 0;

	Dispersor* dispersorChocado = dispersoresTocados.front();

	//cout << "El de menor valor es " << (dispersorChocado->id).c_str() << endl << endl;

	Mensaje* mensaje = new Mensaje();
	mensaje->idTejo = tejo.id;
	mensaje->idDispersor = dispersorChocado->id;

	// Calculo la distancia
	float deltaX = 0;
	float deltaY = 0;

	// Si el circulo choca por su lado inferior
	if (tejo.y > dispersorChocado->yInfIzq + dispersorChocado->alto) {
			
		deltaY = tejo.y - (dispersorChocado->yInfIzq + dispersorChocado->alto);
		deltaX = sqrt( pow(tejo.radio, 2) - pow(deltaY, 2) );				
		distancia = (dispersorChocado->xInfIzq - deltaX) - tejo.x;

	// Si el circulo choca por su lado superior
	} else if (tejo.y < dispersorChocado->yInfIzq) {
	
		deltaY = dispersorChocado->yInfIzq - tejo.y;
		deltaX = sqrt( pow(tejo.radio, 2) - pow(deltaY, 2) );
		distancia = (dispersorChocado->xInfIzq - deltaX) - tejo.x;

	} else {
		distancia = (dispersorChocado->xInfIzq - tejo.radio) - tejo.x;
	}

	mensaje->distancia = distancia;

	listaMensajes.push_back(mensaje);

}

bool ordenarDispersores(Dispersor* d1, Dispersor* d2) {

	return (d1->xInfIzq < d2->xInfIzq);

}

void cargarArchivoRectangulos() {

	FILE* archivoDisp = fopen("rectangulos.txt", "r");

	char fid[235];
	float fxD;
	float fyD;
	float fAlto;
	float fAncho;

	int linea = fscanf(archivoDisp, "%s %f %f %f %f", fid, &fxD, &fyD, &fAlto, &fAncho);

	while (linea > 0 || !feof(archivoDisp) ) {

		//cout << fid << " " << fxD << " " << fyD << " " << fAncho << " " << fAlto << endl << endl;

		Dispersor* dispersor = new Dispersor();
		dispersor->id = fid;
		dispersor->xInfIzq = fxD;
		dispersor->yInfIzq = fyD;
		dispersor->alto = fAlto;
		dispersor->ancho = fAncho;
		
		listaDispersores.push_back(dispersor);

		linea = fscanf(archivoDisp, "%s %f %f %f %f", fid, &fxD, &fyD, &fAlto, &fAncho);
	}

	fclose(archivoDisp);

}