#include"conio.h"
#include"stdlib.h"
#include"string.h"
#include"stdio.h"

#define _CRTDBG_MAP_ALLOC
#include"crtdbg.h"

typedef	struct Student
{
	char* nume;
	float puncte;
	int codExamen;
};

Student initStudent(const char* nume, float puncte, int codExamen) {
	Student s;
	s.nume = (char*)malloc(sizeof(char)*(strlen(nume) + 1));
	strcpy(s.nume, nume);
	s.puncte = puncte;
	s.codExamen = codExamen;
	return s;
}

void afisareStudent(Student s) {
	printf("%s %5.2f puncte, cod examen: %d \n", s.nume, s.puncte, s.codExamen);
}

typedef struct Examen
{
	int codExamen;
	float punctajMinim;
	int numarParticipanti;
	Student* vectorStudenti;
};

Examen initExamen(int codExamen, float punctajMinim, int nrPart, Student* vector) {
	Examen e;
	e.codExamen = codExamen;
	e.punctajMinim = punctajMinim;
	e.numarParticipanti = nrPart;
	e.vectorStudenti = (Student*)malloc(sizeof(Student)*nrPart);
	for (int i = 0; i < nrPart; i++)
	{
		e.vectorStudenti[i] = vector[i];
	}
	return e;
}

void afisareExamen(Examen e) {
	printf("COD %d minim %5.2f puncte %d participanti:\n", e.codExamen, e.punctajMinim, e.numarParticipanti);
	for (int i = 0; i < e.numarParticipanti; i++)
	{
		afisareStudent(e.vectorStudenti[i]);
	}
	printf("\n");
}

typedef struct Nod
{
	Examen info;
	Nod* next;
};

Nod* initNod(Examen info, Nod* next) {
	Nod* n = (Nod*)malloc(sizeof(Nod));
	n->info = info;
	n->next = next;
	return n;
}

Nod* inserareFinal(Nod* &cap, Examen info) {
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
		cap = n;
		return cap;
	}
}

void afisareLista(Nod* cap) {
	if (cap)
	{
		while (cap)
		{
			afisareExamen(cap->info);
			cap = cap->next;
		}
	}
}
//inserare student in fuctie de cod examen

void inserareStudent(Nod* &cap, Student info) {
	if (cap)
	{
		Nod* temp = cap;
		while (temp)
		{
			if (temp->info.codExamen==info.codExamen)
			{
				Student* vTemp = (Student*)malloc(sizeof(Student)*(temp->info.numarParticipanti + 1));
				for (int i = 0; i < temp->info.numarParticipanti; i++)
				{
					vTemp[i] = temp->info.vectorStudenti[i];
				}
				vTemp[temp->info.numarParticipanti] = info;
				free(temp->info.vectorStudenti);
				temp->info.numarParticipanti++;
				temp->info.vectorStudenti = vTemp;
			}
			temp = temp->next;
		}
	}
}

void afisareCodExamenSiNumeStudent(Nod* cap) {
	if (cap)
	{
		while (cap)
		{
			printf("COD: %d: ", cap->info.codExamen);
			for (int i = 0; i < cap->info.numarParticipanti; i++)
			{
				printf("%s, ", cap->info.vectorStudenti[i].nume);
			}
			printf("\n");
			cap = cap->next;
		}
	}
}

void stergereStudent(Student s) {
	free(s.nume);
}

void stergereExamen(Examen e) {
	for (int i = 0; i < e.numarParticipanti; i++)
	{
		stergereStudent(e.vectorStudenti[i]);
	}
	free(e.vectorStudenti);
}

void extragereExamen(Nod* &cap, int codCautat) {
	if (cap)
	{
		if (cap->info.codExamen==codCautat)
		{
			printf("A fost identificat examenul ca fiind primul element \n");
			afisareExamen(cap->info);
			printf("*** se sterge......\n");
			Nod* temp = cap;
			cap = cap->next;
			stergereExamen(temp->info);
			free(temp);
		}
		else {
			Nod* temp = cap;
			while (temp)
			{
				if (temp->next)
				{
					if (temp->next->info.codExamen==codCautat)
					{
						printf("A fost identificat examenul\n");
						afisareExamen(temp->next->info);
						printf("*** se sterge......\n");
						Nod* temp2 = temp->next;
						temp->next = temp->next->next;
						stergereExamen(temp2->info);
						free(temp2);
					}
				}
					temp = temp->next;
			}


		}
	}
}

void main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Student s0 = initStudent("Ion0", 6.5, 5);
	Student s1 = initStudent("Ion1", 4.5, 5);
	Student s2 = initStudent("Ion2", 3.5, 5);
	Student s3 = initStudent("Ion3", 7.5, 5);
	Student s4 = initStudent("Ion4", 9.5, 5);/*
	Student* v1 = (Student*)malloc(sizeof(Student) * 5);
	v1[0] = s0;
	v1[1] = s1;
	v1[2] = s2;
	v1[3] = s3;
	v1[4] = s4;*/
	Student v1[5] = { s0,s1,s2,s3,s4 };
	Examen examen1 = initExamen(5, 5, 5, v1);

	Student e0 = initStudent("Dan0", 6.5, 6);
	Student e1 = initStudent("Dan0", 4.5, 6);
	Student e2 = initStudent("Dan2", 3.5, 6);
	Student e3 = initStudent("Dan3", 7.5, 6);
	Student v2[4] = { e0,e1,e2,e3 };
	Examen examen2 = initExamen(6, 4.5, 4, v2);

	Student d0 = initStudent("Vasile0", 7.5, 7);
	Student d1 = initStudent("Vasile1", 2.5, 7);
	Student v3[2] = { d0,d1 };
	Examen examen3 = initExamen(7, 5, 2, v3);

	Nod* cap = NULL;
	cap = inserareFinal(cap, examen1);
	cap = inserareFinal(cap, examen2);
	cap = inserareFinal(cap, examen3);

	Student deInserat = initStudent("XYZ", 2.5, 7);
	inserareStudent(cap, deInserat);
	afisareLista(cap);
	
	afisareCodExamenSiNumeStudent(cap);
	
	printf("----\n");
	printf("Introduceti codul examenului pentru a fi extras din lista:\n");
	int cod;
	scanf("%d", &cod);
	printf("Se cauta examenul cu nr. %d . . . .\n", cod);
	extragereExamen(cap, cod);
	afisareLista(cap);

}