#include <iostream>
#include <stdio.h>
#include <list>
#include <vector>
#include <cstring>

using namespace std;

typedef struct {
	float y;
	float x0;
	float xf;
} Plataforma;

typedef struct {
	string id;
	float x;
	float y;
	float distancia;
} Barril;

list<Plataforma*> listadoPlataformas;
list<Barril*> listadoBarriles;

void cargarPlataformas();
void cargarBarriles();
void procesarBarril(Barril* barril);
void imprimirBarriles();

bool ordenarPlataformas(Plataforma* p1, Plataforma* p2) {
	return (p1->y > p2->y);
}

bool ordenarPorDistancias(Barril* b1, Barril* b2) {
	return (b1->distancia < b2->distancia);
}

int main() {

	cargarPlataformas();
	cargarBarriles();

	// Ordenar plataformas desde y mas grandes a las mas chicas
	listadoPlataformas.sort(ordenarPlataformas);

	// Veo si están ordenadas
	list<Plataforma*>::iterator itPlat = listadoPlataformas.begin();
	list<Plataforma*>::const_iterator itPlatFIN = listadoPlataformas.end();

	for (; itPlat != itPlatFIN; itPlat++) {
		//cout << (*itPlat)->y << endl << endl;
	}

	list<Barril*>::iterator it = listadoBarriles.begin();
	list<Barril*>::const_iterator itFIN = listadoBarriles.end();

	for (; it != itFIN; it++) {

		procesarBarril((*it));

	}

	imprimirBarriles();

	cin.get();
	return 0;
}

void imprimirBarriles() {

	// Ordeno los barriles por distancia
	listadoBarriles.sort(ordenarPorDistancias);

	list<Barril*>::iterator it = listadoBarriles.begin();
	list<Barril*>::const_iterator itFIN = listadoBarriles.end();

	cout << "Id Barril \t Distancia" << endl << endl;
	for (; it != itFIN; it++) {

		cout << ((*it)->id).c_str() << "\t\t" << (*it)->distancia << endl << endl;

	}
}

bool estaArriba(Barril* barril, Plataforma* plataforma) {

	if (barril->y > plataforma->y) {
		return true;
	}
	return false;
}

bool caeEnPlataforma(Barril* barril, Plataforma* plataforma) {

	if (barril->x > plataforma->x0 && barril->x < plataforma->xf) {
		return true;
	}
	return false;
}

int compararDistancias(Barril* b1, Barril* b2) {
	if (b1->distancia < b2->distancia)
		return -1;
	if (b1->distancia == b2->distancia)
		return 0;
	if (b1->distancia > b2->distancia)
		return 1;
}

void procesarBarril(Barril* barril) {

	string idBarril = barril->id;

	// Aca voy a ir pusheando los casos en los que se va abriendo al caer sobre una plataforma
	vector<Barril*> barriles;
	barriles.push_back(barril);

	int cantidadOpciones;

	list<Plataforma*>::iterator it = listadoPlataformas.begin();
	list<Plataforma*>::const_iterator itFIN = listadoPlataformas.end();

	// Por cada plataforma empezando por la de más arriba
	for (; it != itFIN; it++) {

		cantidadOpciones = (int)barriles.size();

		for (int i = 0; i < cantidadOpciones; i++) {

			// Si ya fue procesado, continúo
			if ( strcmp( (barriles[i]->id).c_str() , "PROC") == 0) {
				continue;
			}

			// Si el barril está más arriba que la plataforma
			if (estaArriba(barriles[i], (*it))) {

				// Si el barril cae sobre la plataforma
				if (caeEnPlataforma(barriles[i], (*it))) {

					// La distancia que recorre es la que cae mas la que traía de antes
					barriles[i]->distancia += barriles[i]->y - (*it)->y;

					// Si llegué al piso, me detengo
					if ((*it)->y == 0) {
						barriles[i]->y = 0;
						//cout << "Llegué al piso" << endl << endl;
						continue;
					}

					// Me voy a la izquierda
					Barril* b1 = new Barril();
					b1->x = (*it)->x0;
					b1->distancia = barriles[i]->distancia + (barriles[i]->x - (*it)->x0);
					b1->y = (*it)->y;

					// Me voy a la derecha
					Barril* b2 = new Barril();
					b2->x = (*it)->xf;
					b2->distancia = barriles[i]->distancia + ((*it)->xf - barriles[i]->x);
					b2->y = (*it)->y;

					barriles.push_back(b1);
					barriles.push_back(b2);

					//cout << (barriles[i]->id).c_str() << " izquierda " << b1->x << " derecha " << b2->x << endl << endl;

					// Marco que ya fue procesado
					barriles[i]->id = "PROC";

				} // fin del cae en plataforma

			} // fin del está arriba

		} // termino de loopear las opciones

	} // termino de loopear los niveles

	list<Barril*> listaBarrilesAComparar;
	cantidadOpciones = (int)barriles.size();
	for (int i = 0; i < cantidadOpciones; i++) {
		// Pusheo los que llegaron al piso
		if (barriles[i]->y == 0) {
			listaBarrilesAComparar.push_back(barriles[i]);
		}
	}

	listaBarrilesAComparar.sort(ordenarPorDistancias);

	//cout << "Distancias " << listaBarrilesAComparar.front()->distancia << " " << listaBarrilesAComparar.back()->distancia << endl << endl;
	barril->distancia = listaBarrilesAComparar.front()->distancia;
	barril->id = idBarril;

}


void cargarBarriles() {

	FILE* archivoBarriles = fopen("personajes.txt", "r");

	char id[256];
	float x, y;

	int linea = fscanf(archivoBarriles, "%s %f %f", id, &x, &y);

	while (linea > 0 || !feof(archivoBarriles)) {

		Barril* barril = new Barril();
		barril->id = id;
		barril->x = x;
		barril->y = y;
		barril->distancia = 0;

		listadoBarriles.push_back(barril);
		linea = fscanf(archivoBarriles, "%s %f %f", id, &x, &y);
	}

	fclose(archivoBarriles);

}

void cargarPlataformas() {

	FILE* archivoPlataformas = fopen("escenario.txt", "r");

	int cantidadPlataformas;
	float y;

	float x0Menor = 99999;
	float xfMayor = -99999;

	int linea = fscanf(archivoPlataformas, "%i %f", &cantidadPlataformas, &y);

	while (linea > 0 || !feof(archivoPlataformas)) {
		for (int i = 0; i < cantidadPlataformas; i++) {
			float x0, xf;
			linea = fscanf(archivoPlataformas, "%f %f", &x0, &xf);

			if (x0 < x0Menor) {
				x0Menor = x0;
			}

			if (xf > xfMayor) {
				xfMayor = xf;
			}

			Plataforma* plataforma = new Plataforma();
			plataforma->y = y;
			plataforma->x0 = x0;
			plataforma->xf = xf;

			listadoPlataformas.push_back(plataforma);

		}

		linea = fscanf(archivoPlataformas, "%i %f", &cantidadPlataformas, &y);
	}

	//cout << "xMenor " << x0Menor << " y " << xfMayor << endl << endl;

	// Este es el piso
	Plataforma* plataforma = new Plataforma();
	plataforma->y = 0;
	plataforma->x0 = x0Menor - 0.1;
	plataforma->xf = xfMayor +0.1;

	listadoPlataformas.push_back(plataforma);

	fclose(archivoPlataformas);

}
