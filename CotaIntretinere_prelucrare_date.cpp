#include"conio.h"
#include"stdlib.h"
#include"string.h"
#include"stdio.h"

#define _CRTDBG_MAP_ALLOC
#include"crtdbg.h"

typedef struct CotaIntretinere
{
	char* adresa;
	int nrApartament; //cheie
	int nrPersoanePerApartament;
	int anul;
	int luna;
	float valaore;
};

CotaIntretinere initCotaIntretinere(const char* adresa, int nrApartament, int nrPersoanePerApartament, int anul, int luna, float valoare) {
	CotaIntretinere c;
	c.adresa = (char*)malloc(sizeof(char)*(strlen(adresa) + 1));
	strcpy(c.adresa, adresa);
	c.nrApartament = nrApartament;
	c.nrPersoanePerApartament = nrPersoanePerApartament;
	c.anul = anul;
	c.luna = luna;
	c.valaore = valoare;
	return c;
}

void afisareCotaIntretinere(CotaIntretinere c) {
	printf("%s: apartament %d: persoana: %d data: %d %d valoare: %5.2f lei \n", c.adresa, c.nrApartament, c.nrPersoanePerApartament, c.anul, c.luna, c.valaore);
}

typedef struct Nod
{
	CotaIntretinere info;
	Nod* next;
};

Nod* initNod(CotaIntretinere info, Nod* next) {
	Nod* n = (Nod*)malloc(sizeof(Nod));
	n->info = info;
	n->next = next;
	return n;
}

Nod* inserareFinal(Nod* &cap, CotaIntretinere info) {
	Nod* n = initNod(info, NULL);
	if (cap)
	{
		Nod* temp = cap;
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = n;
		return cap;
	}
	else {
		return n;
	}
}



void afisareLista(Nod* cap) {
	if (cap)
	{
		while (cap) {
			afisareCotaIntretinere(cap->info);
			cap = cap->next;
		}
	}
}

Nod* stergereLista(Nod* &cap) {
	if (cap)
	{
		while (cap) {
			Nod* temp = cap;
			cap = cap->next;
			free(temp->info.adresa);
			free(temp);
		}
		return NULL;
	}
}

typedef struct HashTable
{
	int dimensiune;
	Nod** vector;
};

HashTable initHastTable(int dimensiune) {
	if (dimensiune>0)
	{
		HashTable h;
		h.dimensiune = dimensiune;
		h.vector = (Nod**)malloc(sizeof(Nod*)*dimensiune);
		for (int i = 0; i < dimensiune; i++)
		{
			h.vector[i] = NULL;
		}
		return h;
	}
}

int hashCodeFunction(int dimensiuneHT, int cod) {
	return cod * 17 % dimensiuneHT;
}

void inserareHT(HashTable h, CotaIntretinere info) {
	if (h.dimensiune>0)
	{
		int cod = hashCodeFunction(h.dimensiune, info.nrApartament);
		if (cod>=0 && cod<h.dimensiune)
		{
			if (h.vector[cod])
			{
				h.vector[cod]=inserareFinal(h.vector[cod], info);
			}
			else {
				h.vector[cod] = (Nod*)malloc(sizeof(Nod));
				h.vector[cod]->info = info;
				h.vector[cod]->next = NULL;
			}
		}
	}
}

void afisareHashtable(HashTable h) {
	if (h.dimensiune>0)
	{
		for (int i = 0; i < h.dimensiune; i++)
		{
			printf("\n");
			if (h.vector[i])
			{
				afisareLista(h.vector[i]);
			}
		}
	}
}

void afisareTotalPlataPerAn(HashTable h, const char* adresaApartamentCautat, int numarApartamentCAutat, int anulDorit) {
	if (h.dimensiune>0)
	{
		int codHash = hashCodeFunction(h.dimensiune, numarApartamentCAutat);
		if (codHash>=0 && codHash<h.dimensiune)
		{
			if (h.vector[codHash])
			{
				Nod* temp = h.vector[codHash];
				float sumaPlata = 0;
				while (temp)
				{
					if (strcmp(temp->info.adresa,adresaApartamentCautat)==0 && temp->info.nrApartament==numarApartamentCAutat && temp->info.anul==anulDorit)
					{
						sumaPlata += temp->info.valaore;
					}
					temp = temp->next;
				}
				printf("Aveti de plata %5.2f lei pe anul %d \n", sumaPlata, anulDorit);
			}
		}
	}
}

int pragCoteIntretinere(HashTable h, float prag) {
	if (h.dimensiune>0)
	{
		int numarCOteDeReturnat = 0;
		for (int i = 0; i < h.dimensiune; i++)
		{
			if (h.vector[i])
			{
				Nod* temp = h.vector[i];
				while (temp)
				{
					if (temp->info.valaore>prag)
					{
						numarCOteDeReturnat++;
					}
					temp = temp->next;
				}
			}
		}
		return numarCOteDeReturnat;
	}
}

Nod* stergereNodLista(Nod* &cap, const char* adresa, int numar) {
	if (cap)
	{
		if (cap->info.nrApartament==numar && strcmp(cap->info.adresa,adresa)==0)
		{
			Nod* temp = cap;
			cap = cap->next;
			free(temp->info.adresa);
			free(temp);
			return cap;
		}
		else {
			Nod* temp = cap;
			while (temp)
			{
				if (temp->next)
				{
					if (temp->next->info.nrApartament == numar && strcmp(temp->next->info.adresa, adresa) == 0)
					{
						Nod* temp2 = temp->next;
						temp->next = temp->next->next;
						free(temp2->info.adresa);
						free(temp2);

					}
				}
				else
				{
					if (temp->info.nrApartament == numar && strcmp(temp->info.adresa, adresa) == 0)
					{
						Nod* temp2 = temp;
						temp->next = temp->next;
						free(temp2->info.adresa);
						free(temp2);

					}
				}
				temp = temp->next;
			}
		}
	}
}

int counterNoduriDeSters(Nod* cap, const char* adresa, int numarAp) {
	if (cap)
	{
		int counter = 0;
		while (cap)
		{
			if (cap->info.nrApartament == numarAp && strcmp(cap->info.adresa, adresa) == 0)
			{
				counter++;
			}
			cap = cap->next;
		}
		return counter;
	}
}

void stergereApartamentDinHT(HashTable h, const char* adresa, int numarAp) {
	if (h.dimensiune > 0)
	{
		int codHash = hashCodeFunction(h.dimensiune, numarAp);
		if (codHash >= 0 && codHash < h.dimensiune)
		{
			if (h.vector[codHash])
			{
				int noduriDeSters = counterNoduriDeSters(h.vector[codHash],adresa,numarAp);
				if (noduriDeSters>0)
				{
					for (int i = 0; i < noduriDeSters; i++)
					{
						h.vector[codHash] = stergereNodLista(h.vector[codHash], adresa, numarAp);
					}
				}
			}
		}
	}
}

void stergereHashTable(HashTable h) {
	if (h.dimensiune>0)
	{
		for (int i = 0; i < h.dimensiune; i++)
		{
			if (h.vector[i])
			{
				stergereLista(h.vector[i]);
			}
		}
		free(h.vector);
		h.dimensiune = 0;
	}
}

void main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	HashTable hashTable = initHastTable(3);

	CotaIntretinere c0 = initCotaIntretinere("Bucuresti 12", 30, 2, 2020, 12, 15.5);
	CotaIntretinere c1 = initCotaIntretinere("Bucuresti 12", 30, 2, 2020, 11, 14.5);
	CotaIntretinere c2 = initCotaIntretinere("Bucuresti 22", 12, 2, 2020, 12, 32.1);
	CotaIntretinere c3 = initCotaIntretinere("Bucuresti 323", 1, 2, 2020, 12, 40.3);
	CotaIntretinere c4 = initCotaIntretinere("Bucuresti 243", 56, 2, 2020, 12, 111.1);
	CotaIntretinere c5 = initCotaIntretinere("Bucuresti 8645", 98, 2, 2020, 12, 122.2);
	CotaIntretinere c6 = initCotaIntretinere("Bucuresti 12", 30, 2, 2011, 12, 15.5);
	CotaIntretinere vector[7] = { c0,c1,c2,c3,c4,c5,c6 };
	for (int i = 0; i < 7; i++)
	{
		inserareHT(hashTable, vector[i]);
	}

	stergereApartamentDinHT(hashTable, "Ciresar 12", 30);


	afisareHashtable(hashTable);

	//afisareTotalPlataPerAn(hashTable, "Ciresar 12", 30, 2020);

	float prag = 3;
	printf("Numarul cotelor ce depasesc %5.2f este de: %d", prag, pragCoteIntretinere(hashTable, prag));

	stergereHashTable(hashTable);

}

