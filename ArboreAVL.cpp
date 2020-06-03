#include<iostream>
#include<string.h>
#define _CRTDBG_MAP_ALLOC

struct Persoana
{
	int id;
	char* nume;
};

Persoana initPersoana(int id, const char* nume) {
	Persoana p;
	p.id = id;
	p.nume = (char*)malloc(sizeof(char)*(strlen(nume) + 1));
	strcpy(p.nume, nume);
	return p;
}

void afisarePersoana(Persoana p) {
	printf("%d %s \n", p.id, p.nume);
}

struct Nod
{
	Persoana info;
	Nod* st;
	Nod* dr;
};

Nod* initNod(Persoana info, Nod* st, Nod* dr) {
	Nod* n = (Nod*)malloc(sizeof(Nod));
	n->info = info;//shallow copy6
	n->dr = n->st = NULL;
	return n;
}

int calculInaltimeArbore(Nod* rad) {
	if (rad)
	{
		int hSt = calculInaltimeArbore(rad->st);
		int hDr = calculInaltimeArbore(rad->dr);
		return 1 + (hSt > hDr ? hSt : hDr);
	}
	else
	{
		return 0;
	}
}

int calculFactorEchilibru(Nod* rad) {
	if (rad)
	{
		int hSt = calculInaltimeArbore(rad->st);
		int hDr = calculInaltimeArbore(rad->dr);
		return hSt - hDr;
	}
	else {
		throw "Nu exista un arbore";
	}
}

Nod* rotireSt(Nod* rad) {
	Nod* aux = rad->dr;
	rad->dr = aux->st;
	aux->st = rad;
	return aux;
}

Nod* rotireDr(Nod* rad) {
	Nod* aux = rad->st;
	rad->st = aux->dr;
	aux->dr = rad;
	return aux;
}

Nod* inserareInArboreAVL(Nod* rad, Persoana p) {
	Nod* n = initNod(p, NULL, NULL);
	if (rad)
	{
		if (rad->info.id > p.id) {
			rad->st = inserareInArboreAVL(rad->st, p);
		}
		else {
			rad->dr = inserareInArboreAVL(rad->dr, p);
		}
		int fEch = calculFactorEchilibru(rad);
		if (fEch==-2)
		{
			if (calculFactorEchilibru(rad->dr) != -1) {
				rad->dr = rotireDr(rad->dr);
			}
			rad = rotireSt(rad);
		}
		else {
			if (fEch==2)
			{
				if (calculFactorEchilibru(rad->st) != 1) {
					rad->st = rotireSt(rad->st);
				}
				rad = rotireDr(rad);
			}
		}
		return rad;
	}
	else {
		rad = n;
		return rad;
	}
}

void afisareArbore(Nod* rad) {
	if (rad)
	{
		afisareArbore(rad->st);
		afisarePersoana(rad->info);
		afisareArbore(rad->dr);
	}
}

void stergereArbore(Nod* &rad) {
	if (rad)
	{
		while (rad) {
			stergereArbore(rad->st);
			stergereArbore(rad->dr);
			free(rad->info.nume);
			free(rad);
			rad = NULL;
		}
	}
}

void verificareArboreGol(Nod* rad) {
	if (rad)
	{
		printf("Arborele nu e gool");
	}
	else {
		printf("Arborele este gol!\n");
	}
}



int counterNoduriIdentificate = 0;
int filtruNoduri(Nod* rad, int filtruID) {
	if (rad) {
		if (rad->info.id > filtruID) {
			counterNoduriIdentificate++;
		}
		filtruNoduri(rad->st, filtruID);
		filtruNoduri(rad->dr, filtruID);
	}
	return counterNoduriIdentificate;
}

int filtruNou(Nod* rad, int x) {
	int r = filtruNoduri(rad, x);
	return r;
}

Nod* valMinimaPerLevel(Nod* rad)
{
	Nod* temp = rad;
	while (temp->st) {
		temp = temp->st;
	}
	return temp;
}

Nod* stergereNodDupaID(Nod* rad, int idCautat)
{
	if (rad) {
		if (rad->info.id > idCautat) { //identific nodul pe care sa il sterg, caut pe fiecare ramura stanga-dreapta
			rad->st = stergereNodDupaID(rad->st, idCautat);
		}
		else if (rad->info.id < idCautat) {
			rad->dr = stergereNodDupaID(rad->dr, idCautat);
		}
		else
		{ //am identificat nodul de sters
			printf("Nodul cautat cu ID %d a fost identificat: \n", rad->info.id);
			afisarePersoana(rad->info);
			printf("* * * \n");
			if (rad->st && rad->dr == NULL) //daca are un copil
			{
				Nod* temp = rad->st;
				*rad = *temp;
				free(temp);
			}
			else if (rad->dr && rad->st == NULL)
			{
				Nod *temp = rad->dr;
				*rad = *temp;
				free(temp);
			}
			else if (rad->dr && rad->st) //daca are 2 copii
			{
				Nod* temp = valMinimaPerLevel(rad->dr);
				rad->info.id = temp->info.id;
				rad->dr = stergereNodDupaID(rad->dr, temp->info.id);
			}
			else //daca este nod frunza
			{
				Nod* temp = rad;
				free(temp->info.nume);
				free(temp);
				rad = NULL;
			}

		}
		return rad;
	}
	else
	{
		return NULL;
	}
}



void main() {
	Nod* rad = NULL;
	for (int i = 0; i < 10; i++)
	{
		rad = inserareInArboreAVL(rad, initPersoana(i+1, "ion"));
	}
	afisareArbore(rad);
	printf("- - - - -- - -- - - -- \n");
	printf(" Inal;time arbore %d \n", calculInaltimeArbore(rad));
	printf("- - - - -- - -- - - -- \n");
	int filtru = filtruNoduri(rad, 5);
	stergereNodDupaID(rad,5);
	printf("- - - - -- - -- - - -- \n");
	afisareArbore(rad);
	printf("- - - - -- - -- - - -- \n");
	stergereArbore(rad);
	printf("- - - - -- - -- - - -- \n");
	afisareArbore(rad);
	verificareArboreGol(rad);
}