#include"conio.h"
#include"stdlib.h"
#include"string.h"
#include"stdio.h"

#define _CRTDBG_MAP_ALLOC
#include"crtdbg.h"

typedef struct Student
{
	int id;
	char* nume;
};

Student initStudent(int id, const char* nume) {
	Student s;
	s.id = id;
	s.nume = (char*)malloc(sizeof(char)*(strlen(nume) + 1));
	strcpy(s.nume, nume);
	return s;
}

void afisareStudent(Student s) {
	printf("%d %s \n", s.id, s.nume);
}

typedef struct Nod
{
	Student info;
	Nod* st;
	Nod* dr;
};

Nod* initNod(Student info, Nod* st, Nod* dr) {
	Nod* n = (Nod*)malloc(sizeof(Nod));
	n->info = info;//shallow copy
	n->st = st;
	n->dr = dr;
	return n;
}

int nivelArbore(Nod* rad) {
	if (rad)
	{
		int hSt = nivelArbore(rad->st);
		int hDr = nivelArbore(rad->dr);
		return 1 + (hSt > hDr ? hSt : hDr);
	}
	else {
		return 0;
	}
}

int functieFactorEchilibru(Nod* rad) {
	if (rad)
	{
		int hSt = nivelArbore(rad->st);
		int hDr = nivelArbore(rad->dr);
		return hSt - hDr;
	}
}

Nod* rotatieDR(Nod* rad) {
	if (rad)
	{
		Nod* temp = rad->st;
		rad->st = temp->dr;
		temp->dr = rad;
		return temp;
	}
	else {
		return NULL;
	}
}

Nod* rotatieST(Nod* rad) {
	if (rad)
	{
		Nod* temp = rad->dr;
		rad->dr = temp->st;
		temp->st = rad;
		return temp;
	}
	else {
		return NULL;
	}
}




Nod* inserareInArbore(Nod* rad, Student info) {
	Nod* n = initNod(info, NULL, NULL);
	if (rad)
	{
		if (rad->info.id > info.id) {
			rad->st = inserareInArbore(rad->st, info);
		}
		else {
			rad->dr = inserareInArbore(rad->dr, info);
		}
		int facttorEchilibru = functieFactorEchilibru(rad);
		if (facttorEchilibru==2)
		{
			if (functieFactorEchilibru(rad->st) != 1) {
				rad->st = rotatieST(rad->st);
			}
			rad = rotatieDR(rad);
		}
		else {
			if (facttorEchilibru == -2) {
				if (functieFactorEchilibru(rad->dr)!=-1)
				{
					rad->dr = rotatieDR(rad->dr);
				}
				rad = rotatieST(rad);
			}
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
		afisareStudent(rad->info);
		afisareArbore(rad->dr);
	}
}

void stergereArbore(Nod* &rad) {
	if (rad)
	{
		stergereArbore(rad->st);
		stergereArbore(rad->dr);
		free(rad->info.nume);
		free(rad);
		rad = NULL;
	}
}

Student extragereFrunza(Nod* &rad) {
	if (rad)
	{
		if (rad->st==NULL && rad->dr==NULL)
		{
			Student rezultat = rad->info;
			free(rad);
			rad = NULL;
			return rezultat;
		}
		else {
			Nod* temp = rad;
			if (temp->st)
			{
				extragereFrunza(temp->st);
			}
			else
			{
				extragereFrunza(temp->dr);
			}
		}
	}
}

Nod* valoareMinimaPeNivel(Nod* rad) {
	if (rad)
	{
		Nod* temp = rad;
		while (temp->st)
		{
			temp = temp->st;
		}
		return temp;
	}
}

Nod* stergereStudentDupaID(Nod* rad, int IDcautat) {
	if (rad)
	{
		if (rad->info.id>IDcautat)
		{
			rad->st= stergereStudentDupaID(rad->st, IDcautat);
		}
		else if (rad->info.id<IDcautat)
		{
			rad->dr= stergereStudentDupaID(rad->dr, IDcautat);
		}
		else
		{
			//3 cazuri
			//copil stanga
			if (rad->st && rad->dr==NULL)
			{
				Nod* temp = rad->st;
				*rad = *temp;
				/*free(temp->info.nume);*/
				free(temp);
			}
			else if (rad->st==NULL && rad->dr)
			{
				Nod* temp = rad->dr;
				*rad = *temp;
				/*free(temp->info.nume);*/
				free(temp);

			
			}
			else if (rad->st && rad->dr)
			{
				Nod* temp = valoareMinimaPeNivel(rad->dr);
				rad->info.id = temp->info.id;
				rad->dr= stergereStudentDupaID(rad->dr, temp->info.id);
			}
			else {
				Nod* temp = rad;
				free(temp->info.nume);
				free(temp);
				rad = NULL;
			}
		}
	return rad;
	}
}

void extragereFrunzeCuAfisare(Nod* &rad) {
	while (rad)
	{
		Student s = extragereFrunza(rad);
		afisareStudent(s);
		free(s.nume);
	}
}

void afisareRadacina(Nod* rad) {
	if (rad)
	{
		afisareStudent(rad->info);
	}
	else {
		printf("Radacina nula! \n");
	}
}

void main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Nod* rad = NULL;
	//rad = inserareInArbore(rad,initStudent(10, "Ion") );
	//rad = inserareInArbore(rad, initStudent(9, "Gigi"));
	//rad = inserareInArbore(rad, initStudent(8, "Mimi"));
	//rad = inserareInArbore(rad, initStudent(7, "Didi"));
	//rad = inserareInArbore(rad, initStudent(6, "jimi"));
	//rad = inserareInArbore(rad, initStudent(5, "Lili"));
	//rad = inserareInArbore(rad, initStudent(20, "Sisiv1"));
	//rad = inserareInArbore(rad, initStudent(21, "Sisiv2"));
	
	for (int i = 1; i < 20; i++)
	{
		rad = inserareInArbore(rad, initStudent(i, "Dummy"));
	}

	rad=stergereStudentDupaID(rad, 16);

	printf("**Afisare radacina \n");
	afisareRadacina(rad);
	printf("**........\n");
	
	afisareArbore(rad);
	printf("Nivel arbore %d \n", nivelArbore(rad));
	printf("** Extragere si afisare frunze arbore \n");
	extragereFrunzeCuAfisare(rad);
	printf("****Afisare arbore \n");
	afisareArbore(rad);

	stergereArbore(rad);
}