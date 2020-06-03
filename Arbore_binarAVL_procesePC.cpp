#include"stdlib.h"
#include"string.h"
#include"stdio.h"
#include"conio.h"

#define _CRTDBG_MAP_ALLOC
#include"crtdbg.h"

typedef struct ProcesCalculator
{
	int idPC;
	char* denumire;
	char* descriere;
	int memorieOcupata;
	char* user;
};

ProcesCalculator initProces(int idPC, const char* denumire, const char* descriere, int memorieOcupata, const char* user) {
	ProcesCalculator p;
	p.idPC = idPC;
	p.denumire = (char*)malloc(sizeof(char)*(strlen(denumire) + 1));
	strcpy(p.denumire, denumire);
	p.descriere = (char*)malloc(sizeof(char)*(strlen(descriere) + 1));
	strcpy(p.descriere, descriere);
	p.memorieOcupata = memorieOcupata;
	p.user = (char*)malloc(sizeof(char)*(strlen(user) + 1));
	strcpy(p.user, user);
	return p;
}

ProcesCalculator citireProces(FILE* f) {
	ProcesCalculator p;
	fscanf(f, "%d", &p.idPC);
	char aux[20];
	fscanf(f, "%s", aux);
	p.denumire = (char*)malloc(sizeof(char)*(strlen(aux) + 1));
	strcpy(p.denumire, aux);
	char aux1[20];
	fscanf(f, "%s", aux1);
	p.descriere = (char*)malloc(sizeof(char)*(strlen(aux1) + 1));
	strcpy(p.descriere, aux1);
	fscanf(f, "%d", &p.memorieOcupata);
	char aux2[20];
	fscanf(f, "%s", aux2);
	p.user = (char*)malloc(sizeof(char)*(strlen(aux2) + 1));
	strcpy(p.user, aux2);
	return p;
}

void afisareProces(ProcesCalculator p) {
	printf("ID: %d, Denumire: %s, Descriere: %s, MemOcupata: %d, USER: %s \n",
		p.idPC, p.denumire, p.denumire, p.memorieOcupata, p.user);
}

typedef	struct Nod
{
	ProcesCalculator info;
	Nod* st;
	Nod* dr;
};

Nod* initNod(ProcesCalculator info, Nod* st, Nod* dr) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = info;
	nou->st = st;
	nou->dr = dr;
	return nou;
}

Nod* inserareInArbore(Nod* rad, ProcesCalculator info) {
	Nod* nou = initNod(info, NULL, NULL);
	if (rad)
	{
		if (rad->info.idPC > info.idPC) {
			rad->st = inserareInArbore(rad->st, info);
		}
		else {
			rad->dr = inserareInArbore(rad->dr, info);
		}
		return rad;
	}
	else {
		rad = nou;
		return rad;
	}
}

int inaltimeArbore(Nod* rad) {
	if (rad)
	{
		int hSt = inaltimeArbore(rad ->st);
		int hDr = inaltimeArbore(rad->dr);
		return 1 + (hSt > hDr ? hSt : hDr);
	}
	else {
		return 0;
	}
}

int factorEchilibrare(Nod* rad) {
	if (rad)
	{
		int hSt = inaltimeArbore(rad->st);
		int hDr = inaltimeArbore(rad->dr);
		return hSt - hDr;
	}
}

Nod* rotatieDr(Nod* rad) {
	if (rad)
	{
		Nod* temp = rad->st;
		rad->st = temp->dr;
		temp->dr = rad;
		return temp;
	}
	else {
		return rad;
	}
}

Nod* rotatieSt(Nod* rad) {
	if (rad)
	{
		Nod* temp = rad->dr;
		rad->dr = temp->st;
		temp->st = rad;
		return temp;
	}
	else {
		return rad;
	}
}

Nod* AVLinserareInArbore(Nod* rad, ProcesCalculator info) {
	Nod* nou = initNod(info, NULL, NULL);
	if (rad)
	{
		if (rad->info.idPC > info.idPC) {
			rad->st = AVLinserareInArbore(rad->st, info);
		}
		else {
			rad->dr = AVLinserareInArbore(rad->dr, info);
		}
		int fEch = factorEchilibrare(rad);
		if (fEch==2)
		{
			if (factorEchilibrare(rad->st) != 1) {
				rad->st = rotatieSt(rad->st);
			}
			rad = rotatieDr(rad);
		}
		else {
			if (fEch==-2)
			{
				if (factorEchilibrare(rad->dr)!=-1)
				{
					rad->dr = rotatieDr(rad->dr);
				}
				rad = rotatieSt(rad);
			}
		}

		return rad;
	}
	else {
		rad = nou;
		return rad;
	}
}

int numarProceseLansate = 0;
void afisareUserCautat(Nod* rad, const char* nume) {
	
	if (rad)
	{
		afisareUserCautat(rad->st, nume);
		if (strcmp(rad->info.user,nume)==0)
		{
			afisareProces(rad->info);
			numarProceseLansate++;
		}
		afisareUserCautat(rad->dr, nume);
	}
}

void afisareNumarProcesePerUser(Nod* rad, const char* nume) {
	numarProceseLansate = 0;
	afisareUserCautat(rad, nume);
	printf("Userul %s are lansate %d procese \n", nume, numarProceseLansate);
}


void afisareArbore(Nod* rad) {
	if (rad)
	{
		afisareArbore(rad->st);
		afisareProces(rad->info);
		afisareArbore(rad->dr);
	}
}

void afisarePeNivelIndicat(Nod* rad, int nivel, int nivelCurent) {
	if (rad)
	{
		if (nivel==nivelCurent)
		{
			afisareProces(rad->info);
		}
		else {
			afisarePeNivelIndicat(rad->st, nivel, nivelCurent + 1);
			afisarePeNivelIndicat(rad->dr, nivel, nivelCurent + 1);
		}
	}
}

int counterElementeMemorie = 0;
int counterElementeMemorieMaiMica(Nod* rad, int nivelIndicat) {
	if (rad)
	{
		counterElementeMemorieMaiMica(rad->st, nivelIndicat);
		if (rad->info.memorieOcupata<=nivelIndicat)
		{
			counterElementeMemorie++;
		}
		counterElementeMemorieMaiMica(rad->dr, nivelIndicat);
	}
	return counterElementeMemorie;
}

int index = 0;

int* creareVectorIdProcese(Nod* rad, int memoriOcupata,int* &vector) {
	if (rad)
	{
		creareVectorIdProcese(rad->st, memoriOcupata, vector);
		if (rad->info.memorieOcupata <= memoriOcupata, vector)
		{
			vector[index] = rad->info.idPC;
			index++;
		}
		creareVectorIdProcese(rad->dr, memoriOcupata, vector);
	}
	return vector;
}

void afisareVector(int* vector, int dimensiune) {
	printf("Vectorul obtinut in urma filtrarii este: \n");
	for (int i = 0; i < dimensiune; i++)
	{
		printf("%d ,", vector[i]);
	}
}

void modificareMemorieOcupata(Nod* rad, int idCautat, int memorieModificata) {
	if (rad)
	{
		modificareMemorieOcupata(rad->st, idCautat, memorieModificata);
		if (rad->info.idPC == idCautat)
		{
			rad->info.memorieOcupata = memorieModificata;
		}
		modificareMemorieOcupata(rad->dr, idCautat, memorieModificata);
	}
}

//tabela de dispersie

struct NodSimplu
{
	ProcesCalculator info;
	NodSimplu* next;
};

typedef struct HashTable
{
	int dimensiune;
	NodSimplu** vector;
};

HashTable initHashTable(int dimensiune) {
	if (dimensiune>0)
	{
		HashTable h;
		h.dimensiune = dimensiune;
		h.vector = (NodSimplu**)malloc(sizeof(NodSimplu*)*dimensiune);
		for (int i = 0; i < h.dimensiune; i++)
		{
			h.vector[i] = NULL;
		}
		return h;
	}
}

int hashFunction(int dimensiuneHT, int cod) {
	int codHash = cod * 17;
	return codHash % dimensiuneHT;
}

NodSimplu* inserareLaFinal(NodSimplu* &cap, ProcesCalculator p) {
	NodSimplu* n = (NodSimplu*)malloc(sizeof(NodSimplu));
	n->info = p;
	n->next = NULL;
	if (cap)
	{
		NodSimplu* temp = cap;
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = n;
		return cap;
	}
	else {
		cap = n;
		return cap;
	}
}

void inserareInHashTable(HashTable h, ProcesCalculator p) {
	if (h.dimensiune>0)
	{
		int codhash = hashFunction(h.dimensiune, p.idPC);
		if (codhash>=0 && codhash<h.dimensiune)
		{
			if (h.vector[codhash])
			{
				h.vector[codhash] = inserareLaFinal(h.vector[codhash], p);
			}
			else {
				h.vector[codhash] = (NodSimplu*)malloc(sizeof(NodSimplu));
				h.vector[codhash]->info = p;
				h.vector[codhash]->next = NULL;
			}
		}
	}
}

void afisareListaSimpluInlantuita(NodSimplu* cap) {
	if (cap)
	{
		while (cap)
		{
			afisareProces(cap->info);
			cap = cap->next;
		}
	}
}

void afisareHT(HashTable h) {
	if (h.dimensiune>0)
	{
		for (int i = 0; i < h.dimensiune; i++)
		{
			if (h.vector[i])
			{
				afisareListaSimpluInlantuita(h.vector[i]);
			}
		}
	}
}

//to do extragere frunza din arbore


//referinta la nivelul superior frunzei
//extrage radacinile pe nivel
Nod* extragereFrunza(Nod* rad, Nod* *referinta_cap) {
	if (rad==NULL)
	{
		return NULL;
	}
	else {
		if (rad->st==NULL && rad->dr==NULL)
		{
			afisareProces(rad->info);
			rad->st = *referinta_cap;
			if (*referinta_cap != NULL) {
				(*referinta_cap)->st = rad;
			}
			*referinta_cap = rad;
			return NULL;
		}
	}
	rad->st = extragereFrunza(rad->st, referinta_cap);
	rad->dr = extragereFrunza(rad->dr, referinta_cap);
	return rad;
}



void main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//retinta1
	Nod* rad = NULL;

	FILE* f = fopen("Arbore_binarAVL_procesePC.txt", "r");
	while (!feof(f))
	{
		rad = AVLinserareInArbore(rad, citireProces(f));
	}
	fclose(f);
	afisareArbore(rad);
	printf("inaltimea arborelui este %d \n", inaltimeArbore(rad));

	//cerinta2
	printf("*2- - - - - - \n");
	afisareNumarProcesePerUser(rad, "Iulian");

	//cerinta3
	printf("*3- - - afisare pe nivel- - - \n");
	afisarePeNivelIndicat(rad, 3, 1);

	//cerinta4
	printf("*4- - - afisareVector- - - \n");
	int dimVect = counterElementeMemorieMaiMica(rad, 300);
	int* vector = (int*)malloc(sizeof(int)*dimVect);
	vector = creareVectorIdProcese(rad, 300, vector);
	afisareVector(vector, dimVect);
	printf("\n");

	//cerinta5 modificare informatie din nod
	printf("*5- - -afisareArbore - - - \n");
	afisareArbore(rad);
	printf("*5- - -afisareArbore dupa modificare memorie- - - \n");
	modificareMemorieOcupata(rad, 2, 99999);
	afisareArbore(rad);

	//cerinta 6 //crapa aici
	printf("*6--Extragere frunze \n");
	Nod* referinta = NULL;
	extragereFrunza(rad, &referinta);
	//extragereFrunza(rad, &referinta);
	//extragereFrunza(rad, &referinta);
	//afisareProces(extras->info);
	printf("-afisare Arbore- \n");
	afisareArbore(rad);

}