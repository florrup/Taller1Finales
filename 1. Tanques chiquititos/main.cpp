#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <stdio.h>
#include <list>
#include <math.h>
#include <cstring>

using namespace std;

typedef struct {
	string id;
	float x;
	float y;
	float radio;
} Pozo;

typedef struct {
	string id;
	float x0;
	float y0;
	string direccion;
	string idPozo;
	float distancia;
} Tanque;

void cargarArchivoPozos();
void cargarArchivoTanques();
bool tanqueCaeEnPozo(Tanque* tanque, Pozo* pozo);
float calcularDistancia(Tanque* tanque, Pozo* pozo);

list<Pozo*> listadoPozos;
list<Tanque*> listadoTanques;

bool ordenarTanques(Tanque* tanque1, Tanque* tanque2) {

	return (tanque1->distancia < tanque2->distancia);

}

int main() {

	cargarArchivoPozos();
	cargarArchivoTanques();

	list<Tanque*>::iterator itT = listadoTanques.begin();
	list<Tanque*>::const_iterator itTFIN = listadoTanques.end();

	// Por cada tanque
	for (; itT != itTFIN; itT++) {

		Pozo* pozoAlQueCae = NULL;
		float menorDistancia = 9999;

		list<Pozo*>::iterator itP = listadoPozos.begin();
		list<Pozo*>::const_iterator itPFIN = listadoPozos.end();

		for (; itP != itPFIN; itP++) {

			// Si el tanque cae en el pozo
			if (  tanqueCaeEnPozo((*itT), (*itP))  ) {

				//cout << "El tanque " << ((*itT)->id).c_str() << " puede caer en " << ((*itP)->id).c_str() << endl << endl;

				// Me fijo en qué dirección viene
				// Si viaja horizontalmente
				if ( strcmp(((*itT)->direccion).c_str(), "H" ) == 0 ) {
					// Me fijo si es el más cercano
					if (menorDistancia >= ( (*itP)->x - (*itP)->radio) ) {
						menorDistancia = (*itP)->x - (*itP)->radio;
						pozoAlQueCae = (*itP);
					}

				// Si viaja verticalmente
				} else if ( strcmp(((*itT)->direccion).c_str(), "V") == 0 ) {
					// Me fijo si es el más cercano
					if (menorDistancia >= ( (*itP)->y - (*itP)->radio) ) {
						menorDistancia = (*itP)->y - (*itP)->radio;
						pozoAlQueCae = (*itP);
					}
				}

			}

		} // fin del for iterador de pozos

		if (pozoAlQueCae != NULL) {
			//cout << ((*itT)->id).c_str() << " cae en " << ((pozoAlQueCae)->id).c_str() << endl << endl;
			(*itT)->idPozo = ((pozoAlQueCae)->id).c_str();
			float distancia = calcularDistancia((*itT), pozoAlQueCae);
			(*itT)->distancia = distancia;
		}

	} // fin del for iterador de tanques

	// Ordeno la lista de tanques
	listadoTanques.sort(ordenarTanques);

	// Imprimo listado
	list<Tanque*>::iterator itTanque = listadoTanques.begin();
	list<Tanque*>::const_iterator itTanqueFIN = listadoTanques.end();

	cout << "IdTanque\tIdPozo\tDistancia" << endl << endl;

	for (; itTanque != itTanqueFIN; itTanque++) {
		cout << ((*itTanque)->id).c_str() << "\t\t" << ((*itTanque)->idPozo).c_str() << "\t" << (*itTanque)->distancia << endl << endl;
	}

	cin.get();
	return 0;
}

float calcularDistancia(Tanque* tanque, Pozo* pozo) {

	float distancia = 0;

	// Si se mueve horizontalmente
	if ( strcmp((tanque->direccion).c_str(), "H" ) == 0 ) {

		float deltaY = tanque->y0 - pozo->y;
		float deltaX = sqrt( pow(pozo->radio,2) - pow(deltaY,2) );
		distancia = (pozo->x - deltaX) - tanque->x0;

	// Si se mueve verticalmente
	} else if ( strcmp((tanque->direccion).c_str(), "V") == 0 ) {

		float deltaX = pozo->x - tanque->x0;
		float deltaY = sqrt( pow(pozo->radio,2) - pow(deltaX,2) );
		distancia = (pozo->y - deltaY) - tanque->y0;

	}

	return distancia;

}

bool tanqueCaeEnPozo(Tanque* tanque, Pozo* pozo) {

	// Si se mueve horizontalmente
	if ( strcmp((tanque->direccion).c_str(), "H" ) == 0 ) {
		// El tanque se encuentra antes de que el pozo termine
		if ( tanque->x0 <= (pozo->x + pozo->radio) ) {
			// El tanque se mueve derecho por el eje x, entre los valores del pozo
			if ( ( tanque->y0 >= (pozo->y - pozo->radio) ) && ( tanque->y0 <= (pozo->y + pozo->radio) ) ) {
				return true;
			}
		}

	// Si se mueve verticalmente
	} else if ( strcmp((tanque->direccion).c_str(), "V") == 0 ) {
		// El tanque se encuentra antes de que el pozo termine
		if ( tanque->y0 <= (pozo->y + pozo->radio) ) {
			// El tanque se mueve derecho por el eje y, entre los valores del pozo
			if ( ( tanque->x0 >= (pozo->x - pozo->radio) ) && ( tanque->x0 <= (pozo->x + pozo->radio) ) ) {
				return true;
			}

		}
	}

	return false;
}

void cargarArchivoPozos() {

	FILE* archivoPozos = fopen("pozos.txt", "r");

	char id[256];
	float x, y, radio;

	int linea = fscanf(archivoPozos, "%s %f %f %f", id, &x, &y, &radio);

	while (linea > 0 || !feof(archivoPozos) ) {
		//cout << id << " " << x << " " << y << " " << radio << endl << endl;

		Pozo* pozo = new Pozo();
		pozo->id = id;
		pozo->x = x;
		pozo->y = y;
		pozo->radio = radio;

		listadoPozos.push_back(pozo);

		linea = fscanf(archivoPozos, "%s %f %f %f", id, &x, &y, &radio);
	}

	//cout << "La cantidad de pozos en la lista es " << listadoPozos.size() << endl << endl;

	fclose(archivoPozos);
}

void cargarArchivoTanques() {

	FILE* archivoTanques = fopen("tanques.txt", "r");

	char id[256];
	float x, y;
	char direccion[256];

	int linea = fscanf(archivoTanques, "%s %f %f %s", id, &x, &y, direccion);

	while (linea > 0 || !feof(archivoTanques) ) {
		//cout << id << " " << x << " " << y << " " << direccion << endl << endl;

		Tanque* tanque = new Tanque();
		tanque->id = id;
		tanque->x0 = x;
		tanque->y0 = y;
		tanque->direccion = direccion;
		tanque->idPozo = "NINGUNO";
		tanque->distancia = 0;

		listadoTanques.push_back(tanque);

		linea = fscanf(archivoTanques, "%s %f %f %s", id, &x, &y, direccion);
	}

	//cout << "La cantidad de tanques en la lista es " << listadoTanques.size() << endl << endl;

	fclose(archivoTanques);

}
