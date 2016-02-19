#define _CRT_SECURE_NO_DEPRECATE

#include <iostream>
#include <stdio.h>
#include <cstring>
#include <string>
#include <list>

using namespace std;

typedef struct {
	string id;
	int x;
	int y;
	float energia;
	float escudo;

	bool vivo;
	int ciclos;
	int casilleros;
} Personaje;

typedef struct {
	int x;
	int y;
	float ataque;
	int alcance;
} Enemigo;

int sizeMapa = 100;

list<Personaje*> listadoPersonajes;
list<Enemigo*> listadoEnemigos;

void cargarEnemigos() {

	FILE* archivo = fopen("enemigos.txt", "r");

	int x;
	int y;
	float ataque;
	int alcance;

	int linea = fscanf(archivo, "%d %d %f %d", &x, &y, &ataque, &alcance);

	while ( linea > 0 || !feof(archivo) ) {
		
		cout << x << " " << y << " " << ataque << " " << alcance << endl << endl;

		Enemigo* enemigo = new Enemigo();

		enemigo->x = x;
		enemigo->y = y;
		enemigo->ataque = ataque;
		enemigo->alcance = alcance;

		listadoEnemigos.push_back(enemigo);
		
		linea = fscanf(archivo, "%d %d %f %d", &x, &y, &ataque, &alcance);
	}

	fclose(archivo);

	cout << "Cantidad enemigos " << listadoEnemigos.size() << endl << endl;

}

void cargarPersonajes() {

	FILE* archivo = fopen("personajes.txt", "r");

	char id[256];
	int x;
	int y;
	float energia;
	float escudo;

	int linea = fscanf(archivo, "%s %d %d %f %f", id, &x, &y, &energia, &escudo);

	while ( linea > 0 || !feof(archivo) ) {

		cout << id << " " << x << " " << y << " " << energia << " " << escudo << endl << endl;

		Personaje* personaje = new Personaje();
		personaje->id = id;
		personaje->x = x;
		personaje->y = y;
		personaje->energia = energia;
		personaje->escudo = escudo;

		personaje->vivo = true;
		personaje->ciclos = 1;
		personaje->casilleros = 0;

		listadoPersonajes.push_back(personaje);

		linea = fscanf(archivo, "%s %d %d %f %f", id, &x, &y, &energia, &escudo);

	}

	cout << " Cantidad de personajes " << listadoPersonajes.size() << endl << endl;

	fclose(archivo);

}

bool ordenarCasilleros(Personaje* p1, Personaje* p2) {

	return (p1->casilleros < p2->casilleros);

}


void mostrarResultados() {

	cout << "ID\tCiclos\tCasilleros" << endl;

	// Ordeno por casilleros
	listadoPersonajes.sort(ordenarCasilleros);

	list<Personaje*>::iterator it = listadoPersonajes.begin();
	list<Personaje*>::const_iterator itFIN = listadoPersonajes.end();

	for (; it != itFIN ; it++) {

		// Si murió
		if ( !(*it)->vivo ) {

			cout << (*it)->id.c_str() << "\t" << (*it)->ciclos << "\t" << (*it)->casilleros << endl << endl;

		}

	} // fin personajes

}

void procesar() {

	bool hayAtaques = true;
	bool hayMovimientos = true;

	while ( hayAtaques || hayMovimientos ) {


		//			ATAQUES

		hayAtaques = false;

		list<Enemigo*>::iterator itE = listadoEnemigos.begin();
		list<Enemigo*>::const_iterator itEFIN = listadoEnemigos.end();

		// Por cada enemigo
		for (; itE != itEFIN; itE++) {

			// Recorro cada personaje
			list<Personaje*>::iterator itP = listadoPersonajes.begin();
			list<Personaje*>::const_iterator itPFIN = listadoPersonajes.end();

			int x0 = (*itE)->x - (*itE)->alcance;
			int xf = (*itE)->x + (*itE)->alcance;
			int y0 = (*itE)->y - (*itE)->alcance;
			int yf = (*itE)->y + (*itE)->alcance;

			for (; itP != itPFIN; itP++) {

				// Si está vivo
				if ( (*itP)->vivo ) {

					// Veo si está dentro del rango de ataque
					if ( (*itP)->x >= x0 && (*itP)->x <= xf && (*itP)->y >= y0 && (*itP)->y <= yf ) {
						
						// Veo si puedo atacarlo
						if ( (*itP)->escudo < (*itE)->ataque ) {

							float restar = (*itE)->ataque - (*itP)->escudo;
							(*itP)->energia -= restar;
							hayAtaques = true;

						}

						// Veo si lo maté
						if ( (*itP)->energia <= 0 ) {

							(*itP)->vivo = false;

						}

					} // fin if rango

				} // fin vivo

			} // fin personajes

		} // fin enemigos


		//			MOVIMIENTOS

		hayMovimientos = false;

		// Recorro todos los personajes
		list<Personaje*>::iterator itPMov = listadoPersonajes.begin();
		list<Personaje*>::const_iterator itPMovFIN = listadoPersonajes.end();

		for (; itPMov != itPMovFIN; itPMov++) {

			bool seMueve = true;

			// Solo si está vivo
			if ( (*itPMov)->vivo ) {

				// Vive un ciclo más
				(*itPMov)->ciclos += 1;

				// Recorro todos los enemigos
				list<Enemigo*>::iterator itEMov = listadoEnemigos.begin();
				list<Enemigo*>::const_iterator itEMovFIN = listadoEnemigos.end();

				for (; itEMov != itEMovFIN; itEMov++) {
					// Me fijo si puede moverse
					// Si personaje x + 1 == enemigo x Y personaje y == enemigo y
					// O personaje x + 1 == fuera del mapa
					// NO ME MUEVO
					if (
						( ( (*itPMov)->x + 1 == (*itEMov)->x ) && ( (*itPMov)->y == (*itEMov)->y ) ) ||
						( (*itPMov)->x + 1 >= sizeMapa )
						) {
					
							seMueve = false;

					} // fin condicion larga

				} // fin enemigos

			} else {

				// Está muerto, no se mueve
				seMueve = false;

			} // fin vivo


			if (seMueve) {
				(*itPMov)->x += 1;
				(*itPMov)->casilleros += 1;
				hayMovimientos = true;
			}


		} // fin personajes

	} // fin while

}

int main() {

	cargarEnemigos();
	cargarPersonajes();

	procesar();

	mostrarResultados();

	cout << "TERMINA DE PROCESAR " << endl << endl;

	cin.get();
	return 0;
}