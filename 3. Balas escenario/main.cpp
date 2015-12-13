#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <list>
#include <stdio.h>
#include <string>
#include <math.h>

using namespace std;

typedef struct {
	string id;
	float x0;
	float y0;
	float xf;
	float yf;
	float pendiente;
	float ordenada;
	float longitud;
	string borde;
} Bala;

list<Bala*> listadoBalas;

bool chocaBordeSuperior(Bala* bala);
bool chocaBordeInferior(Bala* bala);
bool chocaBordeUno(Bala* bala);
bool chocaBordeDos(Bala* bala);
bool chocaBordeTres(Bala* bala);

bool ordenarBalas(Bala* bala1, Bala* bala2) {
	return (bala1->longitud < bala2->longitud);
}

int main() {

	// Cargo balas

	FILE* archivoBalas = fopen("balas.txt", "r");
	char id[256];
	float x0, y0, xf, yf;
	int linea = fscanf(archivoBalas, "%s %f %f %f %f", id, &x0, &y0, &xf, &yf);

	while (linea > 0 || !feof(archivoBalas)) {
		//cout << id << " " << x0 << " " << y0 << " " << xf << " " << yf << endl << endl;

		Bala* bala = new Bala();
		bala->id = id;
		bala->x0 = x0;
		bala->y0 = y0;
		bala->xf = xf;
		bala->yf = yf;
		bala->longitud = 0;
		bala->borde = "NINGUNO";

		// Calculo la pendiente

		bala->pendiente = (yf - y0) / (xf - x0);
		bala->ordenada = y0 - ( bala->pendiente * x0 );

		// Ahora recorro cada bala y veo en donde choca

		bool choca = false;

		if (!choca)
			choca = chocaBordeSuperior(bala);
		if (!choca)
			choca = chocaBordeInferior(bala);
		if (!choca)
			choca = chocaBordeUno(bala);
		if (!choca)
			choca = chocaBordeDos(bala);
		if (!choca)
			choca = chocaBordeTres(bala);

		listadoBalas.push_back(bala);

		linea = fscanf(archivoBalas, "%s %f %f %f %f", id, &x0, &y0, &xf, &yf);
	}

	fclose(archivoBalas);

	//cout << "En total hay " << listadoBalas.size() << " balas" << endl << endl;

	// Ordeno la lista
	listadoBalas.sort(ordenarBalas);

	list<Bala*>::iterator it = listadoBalas.begin();
	list<Bala*>::const_iterator itFIN = listadoBalas.end();

	// Imprimo resultados
	cout << "Longitud \t Id \t\t Destino" << endl << endl;

	for (; it != itFIN; it++) {

		cout << (*it)->longitud << "\t\t " << ((*it)->id).c_str() <<  " \t\t" << ((*it)->borde).c_str() << endl << endl;

	}


	cin.get();
	return 0;
}

float calcularRecorrido(float x, float y) {

	return ( sqrt( pow(x,2) + pow(y,2) ) );

}

// Devuelve true si choca el techo del escenario, false si no
bool chocaBordeSuperior(Bala* bala) {

	float x = 60;
	float y = 60;

	if (bala->pendiente > 0) {

		float xTocado = (y - bala->ordenada) / bala->pendiente;

		if ( xTocado < x && xTocado > 0) {

			bala->longitud = calcularRecorrido(xTocado - bala->x0, y - bala->y0);
			bala->borde = "SUPERIOR";

			return true;

		}

	}
	return false;
}

// Devuelve true si choca el piso del escenario, false si no
bool chocaBordeInferior(Bala* bala) {

	float x = 60;
	float y = 0;

	if (bala->pendiente < 0) {
		float xTocado = (y - bala->ordenada) / bala->pendiente;
		if ( xTocado < x && xTocado > 0) {

			bala->longitud = calcularRecorrido(xTocado - bala->x0, y - bala->y0);
			bala->borde = "INFERIOR";

			return true;

		}

	}
	return false;
}


// y = -4 x + 300
// 60 < x < 65
// 40 < y < 60
bool chocaBordeUno(Bala* bala) {

	float xTocado = (bala->ordenada - 300) / (-4 - bala->pendiente);
	float yTocado = (-4) * xTocado + 300;

	if ( xTocado > 60 && xTocado < 65 && yTocado > 40 && yTocado < 60) {

		bala->longitud = calcularRecorrido(xTocado - bala->x0, yTocado - bala->y0);
		bala->borde = "UNO";

		return true;
	}

	return false;
}

// x = 65
// 20 < y < 40
bool chocaBordeDos(Bala* bala) {

	float x = 65;
	float yTocado = bala->pendiente * x + bala->ordenada;

	if ( yTocado < 40 && yTocado > 20 ) {

		bala->longitud = calcularRecorrido(x - bala->x0, yTocado - bala->y0);
		bala->borde = "DOS";

		return true;
	}

	return false;
}


// y = 4 x - 240
// 60 < x < 65
// 0 < y < 20
bool chocaBordeTres(Bala* bala) {

	float xTocado = (bala->ordenada + 240) / (4 - bala->pendiente);
	float yTocado = 4 * xTocado - 240;

	if ( xTocado > 60 && xTocado < 65 && yTocado > 0 && yTocado < 20) {

		bala->longitud = calcularRecorrido(xTocado - bala->x0, yTocado - bala->y0);
		bala->borde = "TRES";

		return true;
	}

	return false;
}
