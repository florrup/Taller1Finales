#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <stdio.h>
#include <list>

#define SIZE_MAPA 5

using namespace std;

typedef struct {
	string id;
	int x;
	int y;
	int vida;
	int cuadradosRecorridos;
} Personaje;

typedef struct {
	int x;
	int y;
	int poderDeAtaque;
} Enemigo;

list<Personaje*> listadoPersonajes;
list<Enemigo*> listadoEnemigos;

void cargarPersonajes();
void cargarEnemigos();
void procesarPersonaje(Personaje* personaje);
void imprimirResultados();

int main() {

	cargarPersonajes();
	cargarEnemigos();

	list<Personaje*>::iterator it = listadoPersonajes.begin();
	list<Personaje*>::const_iterator itFIN = listadoPersonajes.end();

	for (; it != itFIN; it++) {
		procesarPersonaje((*it));
	}

	imprimirResultados();

	cin.get();
	return 0;
}

bool ordenoRecorrido(Personaje* p1, Personaje* p2) {

	return (p1->cuadradosRecorridos < p2->cuadradosRecorridos);
}

void imprimirResultados() {
	// Ordeno personajes
	listadoPersonajes.sort(ordenoRecorrido);

	list<Personaje*>::iterator it = listadoPersonajes.begin();
	list<Personaje*>::const_iterator itFIN = listadoPersonajes.end();

	for (; it != itFIN; it++) {
		cout << ((*it)->id).c_str() << "\t" << (*it)->cuadradosRecorridos << endl << endl;
	}

}

bool pasaPorRangoAtaque(Personaje* personaje, Enemigo* enemigo) {

	// Si le pasa muy por arriba o muy por abajo
	if (personaje->y >= enemigo->y + 2 || personaje->y <= enemigo->y - 2) {
		return false;
	}

	return true;
}

bool estaAntes(Personaje* personaje, Enemigo* enemigo) {

	if (personaje->x + 2 > enemigo->x ) {
		return false;
	}
	return true;
}

bool ordenarEnemigos(Enemigo* e1, Enemigo* e2) {
	return (e1->x < e2->x);
}

void procesarPersonaje(Personaje* personaje) {

	cout << "\t" << (personaje->id).c_str() << " empieza con vida " << personaje->vida << " en pos " << personaje->x << endl << endl;

	list<Enemigo*> enemigosQueAtacan;

	list<Enemigo*>::iterator it = listadoEnemigos.begin();
	list<Enemigo*>::const_iterator itFIN = listadoEnemigos.end();

	for (; it != itFIN; it++) {

		// Me fijo si paso por y + 2, y, y - 2
		if (pasaPorRangoAtaque(personaje, (*it))) {

			// Me fijo si estoy antes que el enemigo (si estoy en x + 2 no me afecta)
			if (estaAntes(personaje, (*it))) {

				enemigosQueAtacan.push_back((*it));

			}

		}
	}

	// Ordeno el listado de enemigos que me atacan
	enemigosQueAtacan.sort(ordenarEnemigos);

	list<Enemigo*>::iterator itE = enemigosQueAtacan.begin();
	list<Enemigo*>::const_iterator itEFIN = enemigosQueAtacan.end();

	for (; itE != itEFIN; itE++) {
		cout << (personaje->id).c_str() << " contra enemigo en y " << (*itE)->y << endl << endl;

		// Los tiles que recorro hasta llegar afuera del rango enemigo
		int hastaLlegar = ((*itE)->x - 2) - personaje->x;

		if (personaje->vida > 0) {
			personaje->cuadradosRecorridos += hastaLlegar;
			personaje->x = (*itE)->x - 2;
		} else {
			// Si está muerto dejo de recorrer enemigos
			break;
		}

		// Recorro los tres tiles (anterior, enemigo, siguiente)
		for (int i = 0; i < 3; i++) {
		
			if (personaje->vida > 0) {
				personaje->cuadradosRecorridos += 1;
				personaje->x += 1;
				personaje->vida -= (*itE)->poderDeAtaque;
			} else {
				break;
			}
		}

	}

	cout << "Cuando pasa enemigos, vida " << personaje->vida << " en pos " << personaje->x << endl << endl;

	int hastaFinal = 0;
	// Lo hago ir hasta el final del mapa solo si sigue vivo
	if (personaje->vida > 0) {
		// Si el personaje está antes de que termine el mapa
		if (personaje->x <= SIZE_MAPA) {
			hastaFinal = SIZE_MAPA - personaje->x;
			personaje->cuadradosRecorridos += hastaFinal;
			personaje->x = SIZE_MAPA;
		}
	} 

	cout << "Si sigue con vida al final queda en pos " << personaje->x << " y recorrio " << personaje->cuadradosRecorridos << endl << endl;

}

void cargarPersonajes() {

	FILE* archivoPersonajes = fopen("personajes.txt", "r");

	char id[256];
	int x, y, vida;
	int linea = fscanf(archivoPersonajes, "%s %i %i %i", id, &x, &y, &vida);

	while (linea > 0 || !feof(archivoPersonajes)) {

		//cout << id << " " << x << " " << y << " " << vida << endl << endl;
		if (x <= SIZE_MAPA && y <= SIZE_MAPA) {
			Personaje* personaje = new Personaje();
			personaje->id = id;
			personaje->x = x;
			personaje->y = y;
			personaje->vida = vida;
			personaje->cuadradosRecorridos = 0;

			listadoPersonajes.push_back(personaje);
		}

		linea = fscanf(archivoPersonajes, "%s %i %i %i", id, &x, &y, &vida);

	}

	fclose(archivoPersonajes);

}

void cargarEnemigos() {

	FILE* archivoEnemigos = fopen("enemigos.txt", "r");

	int x, y, poder;
	int linea = fscanf(archivoEnemigos, "%i %i %i", &x, &y, &poder);

	while (linea > 0 || !feof(archivoEnemigos)) {

		if (x <= SIZE_MAPA && y <= SIZE_MAPA) {
			Enemigo* enemigo = new Enemigo();
			enemigo->x = x;
			enemigo->y = y;
			enemigo->poderDeAtaque = poder;
			cout << x << " " << y << " " << poder << endl << endl;
			listadoEnemigos.push_back(enemigo);
		}
		linea = fscanf(archivoEnemigos, "%i %i %i", &x, &y, &poder);
	}

	fclose(archivoEnemigos);

}