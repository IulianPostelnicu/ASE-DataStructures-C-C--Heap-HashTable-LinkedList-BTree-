#include"conio.h"
#include"stdlib.h"
#include"string.h"
#include"stdio.h"

#define _CRTDBG_MAP_ALLOC
#include"crtdbg.h"

typedef struct Produs
{
	char* denumireProdus;
	int cantitate;
	char* um;
};


typedef struct Comanda
{
	int idComanda;
	char* denumireClient;
	char* dataLivrare;
	int nrProd;
	Produs* vectorProduse;
};

Comanda citesteComandaDinFisier(FILE* f) {
	Comanda c;
	fscanf(f, "%d", &c.idComanda);

	char aux[20];
	fscanf(f, "%s", aux);
	c.denumireClient = (char*)malloc(sizeof(char)*(strlen(aux) + 1));
	strcpy(c.denumireClient, aux);

	char aux1[20];
	fscanf(f, "%s", aux1);
	c.dataLivrare = (char*)malloc(sizeof(char)*(strlen(aux1) + 1));
	strcpy(c.dataLivrare, aux1);

	fscanf(f, "%d", &c.nrProd);

	c.vectorProduse = (Produs*)malloc(sizeof(Produs)*c.nrProd);
	for (int i = 0; i < c.nrProd; i++)
	{
		Produs p;

		char aux3[20];
		fscanf(f, "%s", aux3);
		p.denumireProdus= (char*)malloc(sizeof(char)*(strlen(aux3) + 1));
		strcpy(p.denumireProdus, aux3);

		fscanf(f, "%d", &p.cantitate);

		char aux4[20];
		fscanf(f, "%s", aux4);
		p.um = (char*)malloc(sizeof(char)*(strlen(aux4) + 1));
		strcpy(p.um, aux4);

		c.vectorProduse[i] = p;
	}
	return c;
}

void afisareProdus(Produs p) {
	printf("      Produs: %s %d %s \n", p.denumireProdus, p.cantitate, p.um);
}

void afisareComanda(Comanda c) {
	printf("Id: %d Nume: %s Data %s Numar produse: %d \n", c.idComanda, c.denumireClient, c.dataLivrare, c.nrProd);
	for (int i = 0; i < c.nrProd; i++)
	{
		afisareProdus(c.vectorProduse[i]);
	}
	printf("\n");
}

typedef struct Nod
{
	Comanda info;
	Nod* st;
	Nod* dr;
};

Nod* initNod(Comanda info, Nod* st, Nod* dr) {
	Nod* n = (Nod*)malloc(sizeof(Nod));
	n->info = info;
	n->st = st;
	n->dr = dr;
	return n;
}

Nod* inserareNod(Nod* rad, Comanda info) {
	Nod* n = initNod(info, NULL, NULL);
	if (rad)
	{
		if (rad->info.idComanda>info.idComanda)
		{
			rad->st = inserareNod(rad->st, info);
		}
		else {
			rad->dr = inserareNod(rad->dr, info);
		}
		return rad;
	}
	else {
		return n;
	}
}

void afisareArbore(Nod* rad) {
	if (rad)
	{
		afisareArbore(rad->st);
		afisareComanda(rad->info);
		afisareArbore(rad->dr);
	}
}

int counterComenzi = 0;
int counterElemente(Nod* rad, const char* data) {
	if (rad)
	{
		if (strcmp(rad->info.dataLivrare,data)==0)
		{
			counterComenzi++;
		}
		counterElemente(rad->st,data);
		counterElemente(rad->dr, data);
	}
	return counterComenzi;
}

int functieCounter(Nod* rad, const char* data) {
	counterComenzi = 0; //resetez valoarea variabilei globale
	counterComenzi = counterElemente(rad, data);
	return counterComenzi;
}

int index = 0;
int* vectorDeComenziFiltrate(Nod* rad, const char* data, int* &vector) {
	if (rad)
	{
		vectorDeComenziFiltrate(rad->st, data, vector);
		if (strcmp(rad->info.dataLivrare,data)==0)
		{
			vector[index] = rad->info.idComanda;
			index++;
		}
		vectorDeComenziFiltrate(rad->dr, data, vector);
	}
	return vector;
}

int* functieVector(Nod* rad, const char* data) {
	int dimensiuneVector = functieCounter(rad, data);
	int* vector = (int*)malloc(sizeof(int)*dimensiuneVector);
	vector = vectorDeComenziFiltrate(rad, data, vector);
	printf("Au fost identificate %d elemente: ", dimensiuneVector);
	for (int i = 0; i < dimensiuneVector; i++)
	{
		printf("%d, ", vector[i]);
	}
	printf("\n");
	return vector;
}

int numarNoduriFrunza = 0;
void cautareNoduriFrunza(Nod* rad, const char* client) {
	if (rad)
	{
		if (strcmp(rad->info.denumireClient,client)==0 && rad->st == NULL && rad->dr == NULL)
		{
			numarNoduriFrunza++;
		}
		cautareNoduriFrunza(rad->st, client);
		cautareNoduriFrunza(rad->dr, client);
	}
}

int functieNumarNoduriFrunzaClientIndicat(Nod* rad, const char* client) {
	if (rad)
	{
		numarNoduriFrunza = 0;
		cautareNoduriFrunza(rad, client);
		return numarNoduriFrunza;
	}
	else {
		return -1;
	}
}


void modificareProdus(Nod* &rad, int IDcautat, Produs* vectorNou) {
	if (rad)
	{
		if (rad->info.idComanda>IDcautat)
		{
			modificareProdus(rad->st, IDcautat, vectorNou);
		}
		else if (rad->info.idComanda < IDcautat)
		{
			modificareProdus(rad->dr, IDcautat, vectorNou);
		}
		else {
			/*Comanda temp = rad->info;*/
			for (int i = 0; i < rad->info.nrProd; i++)
			{
				free(rad->info.vectorProduse[i].denumireProdus);
				free(rad->info.vectorProduse[i].um);
			}
			free(rad->info.vectorProduse);
			for (int i = 0; i < rad->info.nrProd; i++)
			{
				rad->info.vectorProduse[i] = vectorNou[i];
			}
		}
		
	}
}

Produs initProd(const char* denumire, int cantitate, const char* um) {
	Produs p;
	p.denumireProdus = (char*)malloc(sizeof(char)*(strlen(denumire) + 1));
	strcpy(p.denumireProdus, denumire);
	p.cantitate = cantitate;
	p.um = (char*)malloc(sizeof(char)*(strlen(um) + 1));
	strcpy(p.um, um);
	return p;
}

void main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	FILE* file = fopen("Arbore_comenzi.txt", "r");
	Nod* rad = NULL;
	if (file)
	{
		while (!feof(file))
		{
			Comanda c = citesteComandaDinFisier(file);
			rad = inserareNod(rad, c);
		}
	}
	else {
		printf("Eroare file");
	}
	fclose(file);
	afisareArbore(rad);

	int* vector = functieVector(rad, "10:08:2020");

	printf("Numar noduri frunza %d \n", functieNumarNoduriFrunzaClientIndicat(rad, "Ionel1"));

	printf("Ultima functie \n");
	Produs prod1 = initProd("XXX", 10, "XXX");
	Produs prod2 = initProd("YYY", 10, "YYY");
	Produs p[2] = { prod1,prod2 };

	modificareProdus(rad, 106, p);
	afisareArbore(rad);
}

