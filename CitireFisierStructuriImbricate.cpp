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

typedef struct Examen
{
	char* denumire;
	int id;
	int numarStudenti;
	Student* vectorStudenti;
};

Examen citireExamenDinFisier(FILE* f) {
	Examen e;
	
	char aux[20];
	fscanf(f, "%s", aux);
	e.denumire = (char*)malloc(sizeof(char)*(strlen(aux) + 1));
	strcpy(e.denumire, aux);

	fscanf(f, "%d", &e.id);

	fscanf(f, "%d", &e.numarStudenti);

	e.vectorStudenti = (Student*)malloc(sizeof(Student)*e.numarStudenti);
	for (int i = 0; i < e.numarStudenti; i++)
	{
		fscanf(f, "%d", &e.vectorStudenti[i]);

		char aux2[20];
		fscanf(f, "%s", aux2);
		e.vectorStudenti[i].nume = (char*)malloc(sizeof(char)*(strlen(aux2) + 1));
		strcpy(e.vectorStudenti[i].nume, aux2);
	}
	return e;
}

void afisareExamen(Examen e) {
	printf("%s %d %d studenti: \n", e.denumire, e.id, e.numarStudenti);
	for (int i = 0; i < e.numarStudenti; i++)
	{
		afisareStudent(e.vectorStudenti[i]);
	}
}


void main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	FILE* fisier = fopen("CitireFisierStructuriImbricate.txt", "r");
	if (fisier)
	{
		while (!feof(fisier))
		{
			Examen e = citireExamenDinFisier(fisier);
			afisareExamen(e);
		}
	}
	else {
		printf("nu a fost gasit fisierul");
	}
}

