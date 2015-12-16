#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <stdio.h>
#include <list>
#include <fstream>

using namespace std;

typedef struct {
	int x0;
	int xf;
} Plataforma;

list<Plataforma*> listadoPlataformas;

int main() {

	// Leo una vez el archivo para contar la cantidad de campos por linea
	FILE* auxiliar = fopen("prueba.txt", "r");

	int contadorCampos;

	// Variable auxiliar para guardar la linea entera que leo	
	char aux[100];
	char valores[100];

	// Para el tokenizer
	char *pch;

	// Leo la línea entera hasta el \n
	int numero = fscanf(auxiliar, "%[^\n]s", aux);

	while (numero > 0 || !feof(auxiliar)) {

		cout << "Linea entera " << aux << endl << endl;

		contadorCampos = 0;

		// Corto cadena
		pch = strtok(aux," ");

		while (pch != NULL) {

			valores[contadorCampos] = (*pch);

			printf ("%s\n", pch);

			contadorCampos += 1;

			pch = strtok (NULL, " ");
		}

		cout << "Contador Campos: " << contadorCampos << endl << endl;

		// Son dos campos por plataforma
		int cantidadPlataformas = contadorCampos / 2;

		for (int i = 0; i < contadorCampos; i++) {
			//cout << "\t" << valores[i] << endl << endl;
			Plataforma* plataforma = new Plataforma();
			plataforma->x0 = valores[i]; 
			i++;
			plataforma->xf = valores[i];
			listadoPlataformas.push_back(plataforma);
		}
		
		// Leo newline
		numero = fscanf(auxiliar, "\n", aux);
		// Vuelvo a leer la siguiente linea
		numero = fscanf(auxiliar, "%[^\n]s", aux);
	}

	list<Plataforma*>::iterator it = listadoPlataformas.begin();
	list<Plataforma*>::const_iterator itFIN = listadoPlataformas.end();

	for (; it != itFIN; it++) {

		cout << "RESULTADO " << (*it)->x0 << " " << (*it)->xf << endl << endl;

	}

	fclose(auxiliar);

	cin.get();
	return 0;
}