#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <stdio.h>
#include <list>
#include <string>
#include <math.h>
#include <cstring>

#define SIZE 100

using namespace std;

typedef struct {
	string id;
	float xInf0;
	float yInf0;
	float xSupf;
	float ySupf;
	string id1;
	string id2;
	string id3;
	float area;
	float distancia;
} Rectangulo;

// Vector con todos los rectangulos
Rectangulo rectangulos[SIZE];

// Lista con todas las superposiciones
list<Rectangulo*> superposiciones;
list<Rectangulo*> superposicionesFinal;

int cargarArchivoRectangulos();

// Le paso la cantidad total de rectangulos en la lista
void compararSuperposicion(int i);

float calcularArea(float base, float altura);

bool estaSupFinal(string id);

bool ordenaDistancia(Rectangulo* r1, Rectangulo* r2) {
	return (r1->distancia < r2->distancia);
}

int main() {

	int i = cargarArchivoRectangulos();

	compararSuperposicion(i);

	//cout << "El tamanio de la lista de superposicionesFinal es : " << superposicionesFinal.size() << endl << endl << endl << endl;

	// Ordeno los resultados finales

	superposicionesFinal.sort(ordenaDistancia);

	// Imprimo los resultados finales

	list<Rectangulo*>::iterator itSup = superposicionesFinal.begin();
	list<Rectangulo*>::const_iterator itSupFIN = superposicionesFinal.end();

	cout << "Distancia\tArea\tId_Rec\tId_Rec\tId_Rec" << endl << endl;

	for (; itSup != itSupFIN; itSup++) {
		cout << (*itSup)->distancia << "\t\t" << (*itSup)->area << "\t" << ((*itSup)->id1).c_str() << "\t" << ((*itSup)->id2).c_str() << "\t" << ((*itSup)->id3).c_str() << endl << endl;
	}

	superposiciones.clear();
	superposicionesFinal.clear();
	//system("pause");
	cin.get();
	return 0;
}


int cargarArchivoRectangulos() {

	// Abro y parseo el archivo de rectángulos

	FILE* archivoRectangulos = fopen("rectangulos.txt", "r");

	char id[256];
	float xInf, yInf, xSup, ySup;
	int linea = fscanf(archivoRectangulos, "%s %f %f %f %f", id, &xInf, &yInf, &xSup, &ySup);

	int i = 0;

	while (linea > 0 || !feof(archivoRectangulos)) {

		//cout << id << " " << xInf << " " << yInf << " " << xSup << " " << ySup << endl << endl;

		rectangulos[i].id = id;
		rectangulos[i].xInf0 = xInf;
		rectangulos[i].yInf0 = yInf;
		rectangulos[i].xSupf = xSup;
		rectangulos[i].ySupf = ySup;

		i++;

		linea = fscanf(archivoRectangulos, "%s %f %f %f %f", id, &xInf, &yInf, &xSup, &ySup);
	}

	fclose(archivoRectangulos);

	return i;
}


float calcularArea(float base, float altura) {
	return (base*altura);
}


bool seSuperponen(Rectangulo r1, Rectangulo r2) {

	//cout << "Rect " << r1.id << " Rect2 " << r2.id << endl << endl;
	if ( r2.yInf0 > r1.ySupf || r2.xSupf < r1.yInf0 ) {
		return false;
	}

	if ( r2.xSupf < r1.xInf0 || r1.xSupf < r2.xInf0) {
		return false;
	}

	return true;
}

void compararSuperposicion(int total) {

	// Busco una superposicion entre dos rectangulos

	// Tomo un rectangulo
	for (int i = 0; i < total; i++) {
		//cout << "El primero es " << (rectangulos[i].id).c_str() << endl;
		// Tomo un segundo rectangulo
		for (int j = 0; j < total; j++) {
			// Si estoy comparando el mismo rectangulo, lo salteo
			if ( strcmp( (rectangulos[i].id).c_str() , (rectangulos[j].id).c_str() ) == 0 ) {

				// Paso al siguiente rectangulo
				continue;

			} else {

				// Si se superponen los primeros dos rectangulos, creo uno nuevo con su intersección
				if (seSuperponen(rectangulos[i], rectangulos[j])) {

					//cout << (rectangulos[j].id).c_str() << endl;

					Rectangulo* interseccion = new Rectangulo();

					float nuevoXInf = max(rectangulos[i].xInf0, rectangulos[j].xInf0);
					float nuevoYInf = max(rectangulos[i].yInf0, rectangulos[j].yInf0);
					float nuevoXSup = min(rectangulos[i].xSupf, rectangulos[j].xSupf);
					float nuevoYSup = min(rectangulos[i].ySupf, rectangulos[j].ySupf);

					interseccion->id = "Interseccion";
					interseccion->xInf0 = nuevoXInf;
					interseccion->yInf0 = nuevoYInf;
					interseccion->xSupf = nuevoXSup;
					interseccion->ySupf = nuevoYSup;

					list<Rectangulo*>::iterator it = superposiciones.begin();
					list<Rectangulo*>::const_iterator itFIN = superposiciones.end();

					bool esta = false;

					for (; it != itFIN; it++) {
						// Si la interseccion ya estaba contenida:
						// Si el id1 era rI e id2 era rJ
						// o id1 era rJ e id2 era rI
						if ( ( ( strcmp( ((*it)->id1).c_str(), (rectangulos[i].id).c_str() ) == 0 ) && ( strcmp( ((*it)->id2).c_str(), (rectangulos[j].id).c_str() ) == 0 ) )
							|| ( ( strcmp( ((*it)->id1).c_str(), (rectangulos[j].id).c_str() ) == 0 ) && ( strcmp( ((*it)->id2).c_str(), (rectangulos[i].id).c_str() ) == 0 ) ) ) {
							//cout << "Ya estaba contenido" << endl << endl;
							esta = true;
							break;
						}
					}

					if (!esta) {
						interseccion->id1 = rectangulos[i].id;
						interseccion->id2 = rectangulos[j].id;
						superposiciones.push_back(interseccion);
						//cout << nuevoXInf << " " << nuevoYInf << " " << nuevoXSup << " " << nuevoYSup << endl << endl;
					}

				}

			}

		} // fin del segundo for j
	} // fin del primer for i

	//cout << "El tamanio de la lista de superposiciones es : " << superposiciones.size() << endl << endl << endl << endl;

	// Ahora tengo que superponer esto con la lista original otra vez
	list<Rectangulo*>::iterator itSEGUNDO = superposiciones.begin();
	list<Rectangulo*>::const_iterator itSEGUNDOFIN = superposiciones.end();

	for (; itSEGUNDO != itSEGUNDOFIN; itSEGUNDO++) {

		//cout << "La interseccion dada por " << ((*itSEGUNDO)->id1).c_str() << " y " << ((*itSEGUNDO)->id2).c_str() << endl << endl << endl;

		// Recorro la lista original
		for (int k = 0; k < total; k++) {

			// Si el id1 es igual al k o el id2 es igual al k, sigo
			if ( (strcmp( ((*itSEGUNDO)->id1).c_str(), (rectangulos[k].id).c_str() ) == 0 ) ||
				 (strcmp( ((*itSEGUNDO)->id2).c_str(), (rectangulos[k].id).c_str() ) == 0 ) ) {
				//cout << (rectangulos[k].id).c_str() << " ya estaba en la interseccion, lo salteo " << endl << endl << endl;
				continue;
			} else {
				// Si no esta entre los rectangulos, me fijo si se superponen
				if (  seSuperponen(*(*itSEGUNDO), rectangulos[k])  ) {

					//cout << (rectangulos[k].id).c_str() << " se superpone" << endl << endl;

					if (!estaSupFinal((rectangulos[k].id).c_str())) {

						float nuevoXInfS = max((*itSEGUNDO)->xInf0, rectangulos[k].xInf0);
						float nuevoYInfS = max((*itSEGUNDO)->yInf0, rectangulos[k].yInf0);
						float nuevoXSupS = min((*itSEGUNDO)->xSupf, rectangulos[k].xSupf);
						float nuevoYSupS = min((*itSEGUNDO)->ySupf, rectangulos[k].ySupf);

						Rectangulo* supFINAL = new Rectangulo();
						supFINAL->id1 = ((*itSEGUNDO)->id1).c_str();
						supFINAL->id2 = ((*itSEGUNDO)->id2).c_str();
						supFINAL->id3 = (rectangulos[k].id).c_str();
						supFINAL->xInf0 = nuevoXInfS;
						supFINAL->yInf0 = nuevoYInfS;
						supFINAL->xSupf = nuevoXSupS;
						supFINAL->ySupf = nuevoYSupS;
						supFINAL->area = calcularArea( (supFINAL->xSupf - supFINAL->xInf0) , (supFINAL->ySupf - supFINAL->yInf0) );

						supFINAL->distancia = sqrt( pow((supFINAL->xInf0 - 0),2) + pow((supFINAL->ySupf - 0),2) );

						superposicionesFinal.push_back(supFINAL);

					} // fin del si no está en superposicionesFinal

				}
			}

		}

	}

}

bool estaSupFinal(string id) {
	list<Rectangulo*>::iterator it = superposicionesFinal.begin();
	list<Rectangulo*>::const_iterator itFIN = superposicionesFinal.end();

	for (; it != itFIN; it++) {
		if ( (strcmp( ((*it)->id1).c_str() , id.c_str() ) == 0) ||
			 (strcmp( ((*it)->id2).c_str() , id.c_str() ) == 0) ) {
			return true;
		}
	}

	return false;

}
