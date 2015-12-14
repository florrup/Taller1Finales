#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <stdio.h>
#include <list>
#include <cstring>

using namespace std;

typedef struct {
	float xpi;
	float xpd;
	float ypi;
	float ypd;
	float energia;
} Personaje;

typedef struct {
	string id;
	float xii;
	float yii;
	float xsd;
	float ysd;
	float energia;
	string accion;
	bool colisiona;
	float distancia;
	float dejaAlPersonajeConEnergia;
} Ataque;

list<Ataque*> listadoAtaques;

Personaje* personaje;

Personaje* cargarPersonaje();
void cargarAtaques();
void procesarAtaque(Ataque* ataque);
void imprimirListadoAtaques();

int main() {

	personaje = cargarPersonaje();
	cargarAtaques();

	list<Ataque*>::iterator it = listadoAtaques.begin();
	list<Ataque*>::const_iterator itFIN = listadoAtaques.end();

	//cout << "Energia personaje " << personaje->energia << endl << endl;

	// Recorro la lista de ataques
	for (; it != itFIN; it++) {

		procesarAtaque((*it));

	}

	//cout << "Energia personaje " << personaje->energia << endl << endl;

	imprimirListadoAtaques();

	cin.get();
	return 0;
}

void imprimirListadoAtaques() {

	list<Ataque*>::iterator it = listadoAtaques.begin();
	list<Ataque*>::const_iterator itFIN = listadoAtaques.end();

	// Recorro la lista de ataques
	for (; it != itFIN; it++) {

		if ((*it)->colisiona == true) {
			cout << "IdAtaque " << ((*it)->id).c_str() << " Distancia " << (*it)->distancia << " Energia restante " << (*it)->dejaAlPersonajeConEnergia << endl << endl;
		} else {
			cout << "Id Ataque " << ((*it)->id).c_str() << " no colisiona " << endl << endl;
		}

	}

}

bool hayColision(Ataque* ataque) {
	// Si le pasa por arriba o le pasa por abajo
	if (ataque->yii > personaje->ypd || ataque->ysd < personaje->ypi) {
		return false;
	}
	// Si empieza después de la posición actual del personaje
	if (ataque->xii > personaje->xpd ) {
		return false;
	}
	return true;
}

void procesarAtaque(Ataque* ataque) {

	if (hayColision(ataque)) {
		if (personaje->energia <= 0) {
			ataque->colisiona = false;
			return;
		}
		personaje->energia -= ataque->energia;
		ataque->dejaAlPersonajeConEnergia = personaje->energia;

		ataque->colisiona = true;

		ataque->distancia = personaje->xpi - ataque->xii - (ataque->xsd - ataque->xii);

		// Si la accion es N
		if ( strcmp( (ataque->accion).c_str() , "N" ) == 0) {
			// no hago nada

		// Si la accion es S
		} else if ( strcmp( (ataque->accion).c_str() , "S" ) == 0) {
			personaje->ypi += 5;
			personaje->ypd +=5;

		// Si la accion es A
		} else if ( strcmp( (ataque->accion).c_str() , "N" ) == 0) {
			personaje->ypd -= 10;

		// Si la accion es C
		} else if ( strcmp( (ataque->accion).c_str() , "C" ) == 0) {
			personaje->ypi += 5;
			personaje->ypd -= 10;
		}

	} else {
		// Si no colisiona
		ataque->colisiona = false;
	}

}


Personaje* cargarPersonaje() {

	FILE* archivoPersonaje = fopen("personaje.txt", "r");
	float xpi, xpd, ypd;
	int linea = fscanf(archivoPersonaje, "%f %f %f", &xpi, &xpd, &ypd);

	Personaje* personaje = new Personaje();

	while ( linea > 0 || !feof(archivoPersonaje) ) {
		personaje->xpi = xpi;
		personaje->xpd = xpd;
		personaje->ypi = 0;
		personaje->ypd = ypd;
		personaje->energia = 100;

		linea = fscanf(archivoPersonaje, "%f %f %f", &xpi, &xpd, &ypd);
	}

	fclose(archivoPersonaje);

	return personaje;
}

void cargarAtaques() {

	FILE* archivoAtaques = fopen("ataques.txt", "r");

	char id[256];
	float xii, yii, xsd, ysd, energia;
	char accion[1];

	int linea = fscanf(archivoAtaques, "%s %f %f %f %f %f %c", id, &xii, &yii, &xsd, &ysd, &energia, accion);

	while (linea > 0 || !feof(archivoAtaques)) {
		Ataque* ataque = new Ataque();
		ataque->id = id;
		ataque->xii = xii;
		ataque->yii = yii;
		ataque->xsd = xsd;
		ataque->ysd = ysd;
		ataque->energia = energia;
		ataque->accion = accion;
		ataque->colisiona = false;
		ataque->distancia = 0;

		listadoAtaques.push_back(ataque);

		linea = fscanf(archivoAtaques, "%s %f %f %f %f %f %c", id, &xii, &yii, &xsd, &ysd, &energia, accion);
	}

	fclose(archivoAtaques);
}
