#include <iostream>
#include <stdio.h>
#include <list>
#include <cstring>

using namespace std;

typedef struct {
	string id;
	float x0;
	float xf;
	float y;
	int resistencia;
} Ladrillo;

typedef struct {
	string id;
	float x;
	float y;
	int energia;
	string idLadrilloRoto;
	float distancia;
} Bala;

void cargarLadrillos();
void cargarBalas();
void procesarChoque(Bala* bala);
bool ordenarBalas(Bala* b1, Bala* b2);

list<Ladrillo*> listadoLadrillos;
list<Bala*> listadoBalas;
list<Bala*> listadoBalasChocaron;

int main() {

	cargarLadrillos();
	cargarBalas();

	// Recorro el listado de balas y veo a qué ladrillo va a chocar primero
	list<Bala*>::iterator it = listadoBalas.begin();
	list<Bala*>::const_iterator itFIN = listadoBalas.end();

	for (; it != itFIN; it++) {

		procesarChoque((*it));

	}

	// Encolo las balas que mataron a algún ladrillo
	it = listadoBalas.begin();
	itFIN = listadoBalas.end();

	for (; it != itFIN; it++) {
		// Si no rompí ningún ladrillo, salteo
		if ( strcmp( ((*it)->idLadrilloRoto).c_str() , "NINGUNO" ) == 0 ) {
			//cout << "No rompi ninguno" << endl;
		} else {
			// Si rompí un ladrillo, imprimo
			listadoBalasChocaron.push_back((*it));
		}

	}

	// Ordeno ese listado
	listadoBalasChocaron.sort(ordenarBalas);

	list<Bala*>::iterator itChoque = listadoBalasChocaron.begin();
	list<Bala*>::const_iterator itChoqueFIN = listadoBalasChocaron.end();

	cout << "Id Bala \t Id Ladrillo \t Distancia" << endl << endl;

	for (; itChoque != itChoqueFIN; itChoque++) {
		cout << ((*itChoque)->id).c_str() << " \t\t " << ((*itChoque)->idLadrilloRoto).c_str() << " \t\t " << (*itChoque)->distancia << endl << endl;
	}

	cin.get();
	return 0;
}

bool ordenarBalas(Bala* b1, Bala* b2) {

	return (b1->distancia < b2->distancia);

}


bool loChoca(Bala* bala, Ladrillo* ladrillo) {

	// Una bala choca al ladrillo si está dentro del rango de x0 y xf
	// y si la y de la bala es más chica que la del ladrillo (está antes)

	if ( ( bala->x > ladrillo->x0 ) && ( bala->x < ladrillo->xf ) && ( bala->y < ladrillo->y ) ) {
		return true;
	}

	return false;
}

void procesarChoque(Bala* bala) {

	list<Ladrillo*> ladrillosChocados;

	// Recorro cada ladrillo y veo si lo choca
	list<Ladrillo*>::iterator it = listadoLadrillos.begin();
	list<Ladrillo*>::const_iterator itFIN = listadoLadrillos.end();

	// Por cada ladrillo
	for (; it != itFIN; it++) {
		// Si la bala choca al ladrillo
		if ( loChoca(bala,(*it)) ) {
			//cout << "La bala " << (bala->id).c_str() << " choca al " << ((*it)->id).c_str() << endl;
			ladrillosChocados.push_back((*it));
		}

	}

	// Si no choco a ningún ladrillo
	if (ladrillosChocados.size() == 0) {
		return;
	}

	// Hasta acá tengo una lista con todos los ladrillos chocados
	list<Ladrillo*>::iterator itL = ladrillosChocados.begin();
	list<Ladrillo*>::const_iterator itLFIN = ladrillosChocados.end();

	Ladrillo* primerLadrillo = NULL;
	float yMenor = 99999;

	// Encuentro el ladrillo menor que aún siga vivo
	for (; itL != itLFIN; itL++) {
		if ((*itL)->y < yMenor && (*itL)->resistencia > 0) {
			primerLadrillo = (*itL);
			yMenor = (*itL)->y;
		}
	}

	if (primerLadrillo != NULL) {
		// Le resto energía al primer ladrillo que toco
		primerLadrillo->resistencia -= bala->energia;
		// Si maté al ladrillo
		if (primerLadrillo->resistencia <= 0) {
			bala->idLadrilloRoto = primerLadrillo->id;
			bala->distancia = primerLadrillo->y - bala->y;
		}

	}

	ladrillosChocados.clear();

}

void cargarLadrillos() {

	FILE* archivoLadrillos = fopen("plataformas.txt", "r");
	char id[256];
	float x0, xf, y;
	int resistencia;
	int linea = fscanf(archivoLadrillos, "%s %f %f %f %d", id, &x0, &y, &xf, &resistencia);

	while (linea > 0 || !feof(archivoLadrillos)) {
		//cout << id << " " << x0 << " " << y << " " << xf << " " << resistencia << endl << endl;

		Ladrillo* ladrillo = new Ladrillo();
		ladrillo->id = id;
		ladrillo->x0 = x0;
		ladrillo->xf = xf;
		ladrillo->y = y;
		ladrillo->resistencia = resistencia;

		listadoLadrillos.push_back(ladrillo);

		linea = fscanf(archivoLadrillos, "%s %f %f %f %d", id, &x0, &y, &xf, &resistencia);
	}

	//cout << "El tamanio de listadoLadrillos es " << listadoLadrillos.size();

	fclose(archivoLadrillos);

}

void cargarBalas() {

	FILE* archivoBalas = fopen("disparos.txt", "r");
	char id[256];
	float x, y;
	int energia;
	int linea = fscanf(archivoBalas, "%s %f %f %d", id, &x, &y, &energia);

	while (linea > 0 || !feof(archivoBalas)) {
		//cout << id << " " << x << " " << y  << " " << energia << endl << endl;

		Bala* bala = new Bala();
		bala->id = id;
		bala->x = x;
		bala->y = y;
		bala->energia = energia;
		bala->idLadrilloRoto = "NINGUNO";
		bala->distancia = 0;

		listadoBalas.push_back(bala);

		linea = fscanf(archivoBalas, "%s %f %f %d", id, &x, &y, &energia);
	}

	//cout << "El tamanio de listadoBalas es " << listadoBalas.size();

	fclose(archivoBalas);

}
