#include"stdlib.h"
#include"string.h"
#include"stdio.h"
#include"conio.h"

#define _CRTDBG_MAP_ALLOC
#include"crtdbg.h"

typedef struct Film
{
	int codFilm; //cheie
	char* denumire;
	int durata;
	float pretBilet;
};

Film initFilm(int codFilm, const char* denumire, int durata, float pret) {
	Film f;
	f.codFilm = codFilm;
	f.denumire = (char*)malloc(sizeof(char)*(strlen(denumire) + 1));
	strcpy(f.denumire, denumire);
	f.durata = durata;
	f.pretBilet = pret;
	return f;
}

Film citireDinFisier(FILE* f) {
	Film film;
	fscanf(f, "%d", &film.codFilm);
	char aux[20];
	fscanf(f, "%s", aux);
	film.denumire = (char*)malloc(sizeof(char)*(strlen(aux) + 1));
	strcpy(film	.denumire, aux);
	fscanf(f, "%d", &film.durata);
	fscanf(f, "%f", &film.pretBilet);
	return film;
}

typedef struct Heap
{
	int dimensiune;
	Film* vector;
};

void afisareFilm(Film f) {
	printf("Cheie: %d - %s, %d minute, %5.2f lei \n", f.codFilm, f.denumire, f.durata, f.pretBilet);
}

Heap initHeap(int dimensiune) {
	if (dimensiune>0)
	{
		Heap h;
		h.dimensiune = dimensiune;
		h.vector = (Film*)malloc(sizeof(Film)*dimensiune);
		return h;
	}
}

void afisareHeap(Heap h) {
	if (h.dimensiune)
	{
		for (int i = 0; i < h.dimensiune; i++)
		{
			afisareFilm(h.vector[i]);
		}
	}
}

void filtrareHeap(Heap h, int pozitie) {
	if (h.dimensiune)
	{
		int max = pozitie;
		int pozSt = 2 * pozitie + 1;
		int pozDr = 2 * pozitie + 2;
		if (pozSt<h.dimensiune && h.vector[pozSt].codFilm>h.vector[max].codFilm)
		{
			max = pozSt;
		}
		if (pozDr<h.dimensiune && h.vector[pozDr].codFilm>h.vector[max].codFilm) {
			max = pozDr;
		}
		if (max != pozitie) {
			Film temp = h.vector[pozitie];
			h.vector[pozitie] = h.vector[max];
			h.vector[max] = temp;
			if (2 * max + 1 < h.dimensiune) {
				filtrareHeap(h, max);
			}
		}
	}
}

void afisareElementeConditionate(Heap h, float filtru) {
	if (h.dimensiune>0)
	{
		printf("Elemente filtrate dupa %5.2f \n", filtru);
		for (int i = 0; i < h.dimensiune; i++)
		{
			if (h.vector[i].pretBilet<=filtru)
			{
				afisareFilm(h.vector[i]);
			}
		}
	}
}

float medieRang(Heap h) {
	if (h.dimensiune)
	{
		float sum = 0;
		for (int i = 0; i < h.dimensiune; i++)
		{
			sum += h.vector[i].codFilm;
		}
		return sum / h.dimensiune;
	}
}

float sumaPreturiFilme(Heap h) {
	if (h.dimensiune > 0)
	{
		float suma = 0;
		for (int i = 0; i < h.dimensiune; i++)
		{
			suma += h.vector[i].pretBilet;
		}
		return suma;
	}
}

void afisarePreturiBiletePercategorie(Heap h) {
	if (h.dimensiune>0)
	{
		int medie = (int)medieRang(h);
		printf("Media este %d \n", medie);
		float sumaRangInferior = 0;
		float sumaRangSuperior = 0;
		for (int i = 0; i < h.dimensiune; i++)
		{
			if (h.vector[i].codFilm>= medie)
			{
				sumaRangSuperior += h.vector[i].pretBilet;
			}
			else {
				sumaRangInferior += h.vector[i].pretBilet;
			}
		}
		printf("Suma pentru filmele cu rang *Inferior este %5.2f \n", sumaRangInferior);
		printf("Suma pentru filmele cu rang **Superior este %5.2f \n", sumaRangSuperior);
		printf("Suma tuturor filmelor %5.2f \n", sumaRangInferior+sumaRangSuperior);
		printf("Suma totala a filmelor este de %5.2f (verificare) \n", sumaPreturiFilme(h));
	}
}

Film extragereHeap(Heap &h) {
	if (h.dimensiune>0)
	{
		Film rezultat = h.vector[0];
		Film* vectorTemp = (Film*)malloc(sizeof(Film)*(h.dimensiune - 1));
		for (int i = 0; i < h.dimensiune-1; i++)
		{
			vectorTemp[i] = h.vector[i + 1];
		}
		free(h.vector);
		h.dimensiune--;
		h.vector = vectorTemp;
		for (int i=(h.dimensiune-2)/2;i>=0;i--)
		{
			filtrareHeap(h, i);
		}
		return rezultat;
	}
}

void modificareCod(Heap &h, int codCautat, int codDeInlocuit) {
	if (h.dimensiune > 0)
	{
		for (int i = 0; i < h.dimensiune; i++)
		{
			if (h.vector[i].codFilm == codCautat)
			{
				Film modificat = initFilm(codDeInlocuit, h.vector[i].denumire, h.vector[i].durata, h.vector[i].pretBilet);
				free(h.vector[i].denumire);
				h.vector[i] = modificat;
			}
		}
		for (int i = (h.dimensiune - 2) / 2;i >= 0;i--)
		{
			filtrareHeap(h, i);
		}
	}
}

typedef struct Nod
{
	Film info;
	Nod* next;
};

Nod* inserarelaInceput(Nod* cap, Film info) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = info;
	nou->next = NULL;
	if (cap)
	{
		nou->next = cap;
	}
	return nou;
}

Nod* conversieInStiva(Heap &h) {
	if (h.dimensiune>0)
	{
		Nod* stiva = NULL;
		while (h.dimensiune>0)
		{
			stiva = inserarelaInceput(stiva, extragereHeap(h));
		}
		return stiva;
	}
}

void afisareStiva(Nod* &cap) {
	if (cap)
	{
		while (cap)
		{
			Nod* temp = cap;
			afisareFilm(temp->info);
			cap = cap->next;
			free(temp->info.denumire);
			free(temp);
		}
	}
}

void main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	int dimensiuneInitiala = 6;
	Heap h = initHeap(dimensiuneInitiala);

	FILE* fisier = fopen("heap_filme.txt", "r");
	if (fisier)
	{
		for (int i = 0; i < dimensiuneInitiala; i++)
		{
			h.vector[i] = citireDinFisier(fisier);
		}
	}
	fclose(fisier);
	afisareHeap(h);
	printf("- - - - - \n");
	printf("- - HEAP SORTAT- - - \n");
	for (int i=(h.dimensiune-2)/2;i>=0;i--)
	{
		filtrareHeap(h, i);
	}
	afisareHeap(h);
	printf("- - - - - \n");
	afisareElementeConditionate(h, 12.0);
	printf("- - - - - \n");
	afisarePreturiBiletePercategorie(h);
	printf("- - - - - \n");
	Film extras = extragereHeap(h);
	afisareFilm(extras);
	free(extras.denumire);
	printf("- - - - - \n");
	afisareHeap(h);
	printf("- - - - - \n");
	printf("- - -Modificate CodFilm - - \n");
	modificareCod(h, 14, 30);
	modificareCod(h, 1, 99);
	afisareHeap(h);
	printf("- - -Extragere in stiva - - \n");
	Nod* stiva = NULL;
	stiva = conversieInStiva(h);
	afisareStiva(stiva);
	afisareHeap(h);
}