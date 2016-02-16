#include <iostream>
#include <stdio.h>
#include <list>
#include <string>

using namespace std;

typedef struct {
	int x;
	int y;
	float ataque;
} Enemigo;

typedef struct {
	string id;
	int x;
	int y;
	float energia;
	bool vivo;
	int casilleros;
	int ciclos;
} Personaje;

list<Enemigo*> listadoEnemigos;
list<Personaje*> listadoPersonajes;

int sizeMapa = 100;

void cargarEnemigos() {

	FILE* archivo = fopen("enemigos.txt", "r");

	int x;
	int y;
	float ataque;

	int linea = fscanf(archivo, "%d %d %f", &x, &y, &ataque);

	while ( linea > 0 || !feof(archivo) ) {

		cout << x << " " << y << " " << ataque << endl << endl;

		Enemigo* enemigo = new Enemigo();

		enemigo->x = x;
		enemigo->y = y;
		enemigo->ataque = ataque;

		listadoEnemigos.push_back(enemigo);

		linea = fscanf(archivo, "%d %d %f", &x, &y, &ataque);

	}

	fclose(archivo);

	cout << "Cantidad de enemigos: " << listadoEnemigos.size() << endl << endl << endl;

}

void cargarPersonajes() {

	FILE* archivo = fopen("personajes.txt", "r");

	char id[256];
	int x;
	int y;
	float energia;

	int linea = fscanf(archivo, "%s %d %d %f", id, &x, &y, &energia);

	while (linea > 0 || !feof(archivo)) {

		cout << id << " " << x << " " << y << " " << energia << endl << endl;

		Personaje* personaje = new Personaje();

		personaje->id = id;
		personaje->x = x;
		personaje->y = y;
		personaje->energia = energia;
		personaje->vivo = true;
		personaje->casilleros = 0;
		personaje->ciclos = 1;

		listadoPersonajes.push_back(personaje);

		linea = fscanf(archivo, "%s %d %d %f", id, &x, &y, &energia);

	}

	fclose(archivo);
	
	cout << "Cantidad de personajes: " << listadoPersonajes.size() << endl << endl << endl;

}

void procesar() {

	bool hayAtaque = true;
	bool hayMovimiento = true;

	while ( hayAtaque || hayMovimiento ) {

		/*********************************/
		// ATAQUES

		hayAtaque = false;

		list<Enemigo*>::iterator itE = listadoEnemigos.begin();
		list<Enemigo*>::const_iterator itEFIN = listadoEnemigos.end();

		// Recorro los enemigos para que ataquen uno por uno
		for (; itE != itEFIN; itE++) {
	
			// Recorro los personajes a atacar
			list<Personaje*>::iterator itP = listadoPersonajes.begin();
			list<Personaje*>::const_iterator itPFIN = listadoPersonajes.end();

			for (; itP != itPFIN; itP++) {

				// Si el personaje está vivo
				if ( (*itP)->vivo ) {

					// Veo si puedo atacarlo (si está dentro del rango)
					int x0 = (*itE)->x - 1;
					int xf = (*itE)->x + 1;
					int y0 = (*itE)->y - 1;
					int yf = (*itE)->y + 1;

					if ( (*itP)->x >= x0 && (*itP)->x <= xf && (*itP)->y >= y0 && (*itP)->y <=yf ) {
				
						hayAtaque = true;
						(*itP)->energia -= (*itE)->ataque;

					}

					// TODO Fijarme si lo saco del if de vivo sigue funcionando ??
					if ( (*itP)->energia <= 0 ) {

						(*itP)->vivo = false;

					}

				} // fin vivo

			} // fin personajes

		} // fin enemigos


		/*********************************/
		// MOVIMIENTOS

		hayMovimiento = false;

		// Recorro los personajes para moverlos
		list<Personaje*>::iterator itPMov = listadoPersonajes.begin();
		list<Personaje*>::const_iterator itPMovFIN = listadoPersonajes.end();

		for (; itPMov != itPMovFIN; itPMov++) {

			bool seMueve = true;

			// Si está vivo, puede llegar a moverse
			if ( (*itPMov)->vivo ) {

				// Vive un ciclo más
				(*itPMov)->ciclos += 1;

				list<Enemigo*>::iterator itEMov = listadoEnemigos.begin();
				list<Enemigo*>::const_iterator itEMovFIN = listadoEnemigos.end();

				// Recorro los para ver si lo deja moverse
				for (; itEMov != itEMovFIN; itEMov++) {

					// No se mueve si x + 1 está ocupado en el MISMO Y
					// O si el proximo mov me saca del mapa
					if (
						( (*itPMov)->x + 1 == (*itEMov)->x && (*itPMov)->y == (*itEMov)->y )
						||
						( (*itPMov)->x + 1 >= sizeMapa )
						) {

							seMueve = false;

					} // fin if condicion larga

				} // fin enemigos

			} // fin vivo 

			else {

				// Si está muerto, no se mueve
				seMueve = false;

			}
	
			if (seMueve) {
				(*itPMov)->x += 1;
				(*itPMov)->casilleros += 1;
				hayMovimiento = true;
			}

		} // fin personajes



	} // fin del while


	cout << "Termina de procesar" << endl << endl;
}

bool ordenarCasilleros(Personaje* p1, Personaje* p2) {

	return (p1->casilleros < p2->casilleros);

}

void mostrarResultados() {

	cout << "ID \t Ciclos \t Casilleros" << endl << endl;

	listadoPersonajes.sort(ordenarCasilleros);

	list<Personaje*>::iterator it = listadoPersonajes.begin();
	list<Personaje*>::const_iterator itFIN = listadoPersonajes.end();

	for (; it != itFIN; it++) {

		if ( !(*it)->vivo ) {

			cout << (*it)->id.c_str() << "\t" << (*it)->ciclos << "\t" << (*it)->casilleros << endl << endl;

		}

	}

	cout << "Fin mostrar" << endl<< endl;
}

int main() {

	cargarEnemigos();
	cargarPersonajes();
	procesar();
	mostrarResultados();

	cout << "\n\tTermina el programa" << endl;
	cin.get();
	return 0;
}