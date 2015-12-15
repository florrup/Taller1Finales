// OJO. Calculé distancia mínima en cada plataforma, no la total

#include <iostream>
#include <stdio.h>
#include <list>

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
	//cout << "La cantidad de plataformas es de " << listadoPlataformas.size() << endl << endl;
	//cout << "La cantidad de barriles es de " << listadoBarriles.size() << endl << endl;

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

		cout << ((*it)->id).c_str() << "\t" << (*it)->distancia << endl << endl;

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

void procesarBarril(Barril* barril) {
	list<Plataforma*>::iterator it = listadoPlataformas.begin();
	list<Plataforma*>::const_iterator itFIN = listadoPlataformas.end();

	Plataforma* plataformaSobreLaQueCae;
	float distanciaRecorrida = 0;
	float aIzquierda = 0;
	float aDerecha = 0;

	// Por cada plataforma empezando por la de más arriba de todo
	for (; it != itFIN; it++) {

		// Si el barril está más arriba de la plataforma
		if (estaArriba(barril, (*it))) {

			// Si su x está entre los x de la plataforma, o sea, cae sobre esa plataforma
			if (caeEnPlataforma(barril, (*it))) {

				// Desde el y del barril hasta la plataforma
				barril->distancia += (barril->y - (*it)->y);

				// La y del barril ahora es el de la plataforma
				barril->y = (*it)->y;

				// Si el barril llegó al piso, me detengo
				if (barril->y == 0) {
					break;
				}

				aIzquierda = barril->x - (*it)->x0;
				aDerecha = (*it)->xf - barril->x;

				// Cae por la derecha
				if (aIzquierda >= aDerecha) {
					barril->distancia += aDerecha;
					barril->x = (*it)->xf;
				} else {
				// Cae por la izquierda incluye centro
					barril->distancia += aIzquierda;
					barril->x = (*it)->x0;
				}

				//cout << (barril->id).c_str() << " cae sobre y " << (*it)->y << " con x0 " << (*it)->x0 << " distancia " << barril->distancia << endl << endl;

			} // fin del if del cae sobre
		} // fin del está arriba

	}

	//cout << "DISTANCIA " << barril->distancia << endl << endl << endl;
	// FALTA CALCULAR DESDE LA ULTIMA PLATAFORMA HASTA EL PISO

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
