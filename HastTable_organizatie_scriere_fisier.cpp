#include"stdlib.h"
#include"string.h"
#include"stdio.h"
#include"conio.h"

#define _CRTDBG_MAP_ALLOC
#include<crtdbg.h>

typedef struct Organizatie {
	char* idOrganizatie;
	char* denumireOrganizatie;
	char* director;
	float buget;
	unsigned int nrOrganizatiiSubordonate;
}Organizatie;

Organizatie initOrganizatie(const char* idOrganizatie, const char* denumireOrganizatie, const char* director, float buget, unsigned int nrOrganizatiiSubordonate) {
	Organizatie o;
	o.idOrganizatie = (char*)malloc(sizeof(char)*(strlen(idOrganizatie) + 1));
	strcpy(o.idOrganizatie, idOrganizatie);
	o.denumireOrganizatie = (char*)malloc(sizeof(char)*(strlen(denumireOrganizatie) + 1));
	strcpy(o.denumireOrganizatie, denumireOrganizatie);
	o.director = (char*)malloc(sizeof(char)*(strlen(director) + 1));
	strcpy(o.director, director);
	o.buget = buget;
	o.nrOrganizatiiSubordonate = nrOrganizatiiSubordonate;
	return o;
}

void afisareOrganizatie(Organizatie o) {
	printf("%s %s %s %5.2f organizatii subordonate %d \n", 
		o.idOrganizatie, o.denumireOrganizatie, o.director, o.buget, o.nrOrganizatiiSubordonate);
}

Organizatie citireDinFisier(FILE* f) {
	Organizatie o;
	char aux[20];
	fscanf(f, "%s", aux);
	o.idOrganizatie = (char*)malloc(sizeof(char)*(strlen(aux) + 1));
	strcpy(o.idOrganizatie, aux);
	char aux2[20];
	fscanf(f, "%s", aux2);
	o.denumireOrganizatie = (char*)malloc(sizeof(char)*(strlen(aux2) + 1));
	strcpy(o.denumireOrganizatie, aux2);
	char aux3[20];
	fscanf(f, "%s", aux3);
	o.director = (char*)malloc(sizeof(char)*(strlen(aux3) + 1));
	strcpy(o.director, aux3);
	fscanf(f, "%f", &o.buget);
	fscanf(f, "%d", &o.nrOrganizatiiSubordonate);
	return o;
}

typedef struct Nod
{
	Organizatie info;
	Nod* next;
}Nod;

Nod* initNod(Organizatie info, Nod* next) {
	Nod* n = (Nod*)malloc(sizeof(Nod));
	n->info = info;
	n->next = next;
	return n;
}

Nod* inserareLaFinal(Nod* &cap, Organizatie info) {
	Nod* n = initNod(info, NULL);
	if (cap)
	{
		Nod* temp = cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = n;
		return cap;
	}else{
		cap = n;
		return cap;
	}
}

void afisareLista(Nod* cap) {
	if (cap)
	{
		while (cap) {
			afisareOrganizatie(cap->info);
			cap = cap->next;
		}
	}
	else {
		printf("lista este goala\n");
	}
}

void stergereOrganitie(Organizatie o) {
	if (o.idOrganizatie)
	{
		free(o.idOrganizatie);
		free(o.denumireOrganizatie);
		free(o.director);
	}
}

Nod* stergereLista(Nod* &cap) {
	if (cap)
	{
		while (cap) {
			Nod* temp = cap;
			cap = cap->next;
			stergereOrganitie(temp->info);
			free(temp);
		}
		return NULL;
	}
	else {
		return cap;
	}
}

typedef struct HashTable
{
	int dimensiuneHT;
	Nod** vector;
}HashTable;

HashTable initHashtable(int dimensiune) {
	HashTable h;
	h.dimensiuneHT = dimensiune;
	h.vector = (Nod**)malloc(sizeof(Nod*)*dimensiune);
	for (int i = 0; i < h.dimensiuneHT; i++)
	{
		h.vector[i] = NULL;
	}
	return h;
}

void stergereHashTable(HashTable h) {
	if (h.dimensiuneHT>0)
	{
		for (int i = 0; i < h.dimensiuneHT; i++)
		{
			if (h.vector[i] != NULL) {
				stergereLista(h.vector[i]);
			}
		}
		free(h.vector);
		h.dimensiuneHT = 0;
	}
	else {
		printf("Dimensiunea HashTable este ZERO! \n");
	}
}

int hashFunction(int dimensiuneHT, const char* valoare) {
	int cod = strlen(valoare);
	cod *= 17;
	return cod % dimensiuneHT;
}

void inserareHashTable(HashTable h, Organizatie o) {
	if (h.dimensiuneHT>0)
	{
		int codHash = hashFunction(h.dimensiuneHT, o.idOrganizatie);
		if (codHash>=0 && codHash<h.dimensiuneHT)
		{
			if (h.vector[codHash] != NULL) {
				h.vector[codHash] = inserareLaFinal(h.vector[codHash], o);
			}
			else {
				h.vector[codHash] = (Nod*)malloc(sizeof(Nod));
				h.vector[codHash]->info = o;
				h.vector[codHash]->next = NULL;
			}
		}
		else
		{
			printf("*Eroare in calcul hashCod!\n");
		}
	}
	else {
		printf("*Eroare Initializare HT\n");
	}
}

void afisareHashtable(HashTable h) {
	if (h.dimensiuneHT>0)
	{
		for (int i = 0; i < h.dimensiuneHT; i++)
		{
			printf("NIVELUL %d \n", i);
			if (h.vector[i]!=NULL)
			{
				afisareLista(h.vector[i]);
			}
			printf("\n");
		}
	}
	else {
		printf("*Eroare Initializare HT\n");
	}
}

void exportaDateDInNod(FILE* g, HashTable h, const char* cuvantCheie) {
	if (h.dimensiuneHT>0)
	{
		int codHash = hashFunction(h.dimensiuneHT,cuvantCheie);
		if (codHash>=0 && codHash<h.dimensiuneHT)
		{
			if (h.vector[codHash] != NULL) {
				Nod* temp = h.vector[codHash];
				fprintf(g, "* * * Date exportate din nodul %d \n", codHash);
				while (temp) {
					fprintf(g, "%s ",temp->info.idOrganizatie);
					fprintf(g, "%s ", temp->info.denumireOrganizatie);
					fprintf(g, "%s ", temp->info.director);
					fprintf(g, "%5.2f ", temp->info.buget);
					fprintf(g, "%d \n", temp->info.nrOrganizatiiSubordonate);
					temp = temp->next;
				}
			}
			else {
				fprintf(g, "* Nu au fost identificate date in nodul %d",codHash);
			}
		}
		else {
			printf("Eroare de calcul codHash\n");
		}
	}
	else {
		printf("Eroare HashTable \n");
	}
}

Organizatie cautaOrganbizatie(HashTable h, const char* idCautat) {
	if (h.dimensiuneHT>0)
	{
		int codHash = hashFunction(h.dimensiuneHT, idCautat);
		if (codHash>=0 && codHash<h.dimensiuneHT)
		{
			if (h.vector[codHash] != NULL) {
				Nod* temp = h.vector[codHash];
				while (temp)
				{
					if (strcmp(temp->info.idOrganizatie, idCautat) == 0) {
						return initOrganizatie(temp->info.idOrganizatie, temp->info.denumireOrganizatie, temp->info.director, temp->info.buget, temp->info.nrOrganizatiiSubordonate);
					}
					temp = temp->next;
				}
				printf("Nu a fost identificata o organizatie ! \n");
			}
			else {
				printf("Nu a fost identificata o organizatie ! \n");
			}
		}
		else
		{
			printf("Eroare ! \n");
		}
	}
	else
	{
		printf("Eroare ! \n");
	}
}

void stergereNodDinLista(Nod* &cap, const char* idCautat) {
	if (cap)
	{
		Nod* temp = cap;
		while (temp) {
			if (temp->next) {
				if (strcmp(temp->next->info.idOrganizatie, idCautat) == 0)
				{
					printf("* * * A fost identificat nodul \n");
					afisareOrganizatie(temp->next->info);
					Nod* aux = temp->next;
					temp->next = temp->next->next;
					stergereOrganitie(aux->info);
					free(aux);
				}
			}
			temp = temp->next;
		}
		if (strcmp(cap->info.idOrganizatie, idCautat) == 0)
		{
			printf("* * * A fost identificat nodul \n");
			afisareOrganizatie(cap->info);
			Nod* aux = cap;
			cap = cap->next;
			stergereOrganitie(aux->info);
			free(aux);
		}
	}
}

void cautaSiStergereOrganizatie(HashTable h, const char* idCautat) {
	if (h.dimensiuneHT > 0)
	{
		int codHash = hashFunction(h.dimensiuneHT, idCautat);
		if (codHash >= 0 && codHash < h.dimensiuneHT)
		{
			if (h.vector[codHash] != NULL) {
				stergereNodDinLista(h.vector[codHash], idCautat);
			}
			else {
				printf("Nu a fost identificata o organizatie ! \n");
			}
		}
		else
		{
			printf("Eroare ! \n");
		}
	}
	else
	{
		printf("Eroare ! \n");
	}
}



void main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	HashTable h = initHashtable(5);
	FILE* f = fopen("hastTable_organizatie.txt", "r");
	if (f)
	{
		while (!feof(f)) {
			inserareHashTable(h, citireDinFisier(f));
		}
	}
	fclose(f);
	//afisareHashtable(h);

	FILE* g = fopen("export_hastTable_organizatie.txt", "w");
	exportaDateDInNod(g, h, "123456789Microsoft");
	fclose(g);

	printf("- - -- - -- - \n");
	//Organizatie oCautata = cautaOrganbizatie(h, "123Microsoft");
	//afisareOrganizatie(oCautata);
	//stergereOrganitie(oCautata);

	cautaSiStergereOrganizatie(h, "XYZ");
	printf("- -- - - --\n");
	

	afisareHashtable(h);
	stergereHashTable(h);

}