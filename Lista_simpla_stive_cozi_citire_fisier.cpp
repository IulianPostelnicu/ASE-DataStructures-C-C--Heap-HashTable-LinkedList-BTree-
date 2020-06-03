#define _CRTDBG_MAP_ALLOC
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<crtdbg.h>

typedef struct Student
{
	char* nume;
	unsigned int grupa;
	int numarNote;
	int* vectorNote;
};

Student citesteDinFisierStudentul(FILE* f) {
	if (f)
	{
		Student s;
		char aux[20];
		fscanf(f, "%s", aux);
		s.nume = (char*)malloc(sizeof(char*)*(strlen(aux) + 1));
		strcpy(s.nume, aux);
		fscanf(f, "%d", &s.grupa);
		fscanf(f, "%d", &s.numarNote);
		s.vectorNote = (int*)malloc(sizeof(int)*s.numarNote);
		for (int i = 0; i < s.numarNote; i++)
		{
			fscanf(f, "%d", &s.vectorNote[i]);
		}
		return s;
	}
}

Student initStudent(const char* nume, unsigned int grupa, int numarNote, int* vector) {
	Student s;
	s.nume = (char*)malloc(sizeof(char*)*(strlen(nume) + 1));
	strcpy(s.nume, nume);
	s.grupa = grupa;
	s.numarNote = numarNote;
	s.vectorNote = (int*)malloc(sizeof(int)*s.numarNote);
	for (int i = 0; i < s.numarNote; i++)
	{
		s.vectorNote[i] = vector[i];
	}
	return s;
}

void afisareStudent(Student s) {
	printf("Studentul %s %d are %d note: ", s.nume, s.grupa, s.numarNote);
	for (int i = 0; i < s.numarNote; i++)
	{
		printf(" %d, ", s.vectorNote[i]);
	}
	printf("\n");
}

typedef struct Nod
{
	Student info;
	Nod* next;
}Nod;

Nod* initNod(Student info, Nod* next) {
	Nod* n = (Nod*)malloc(sizeof(Nod));
	n->info = info;
	n->next = next;
	return n;
}

//coada
Nod* pushCoada(Nod* &cap, Student info) {
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


///stiva/
Nod* pushStiva(Nod* &cap, Student info) {
	Nod* n = initNod(info, NULL);
	if (cap)
	{
		n->next = cap;
	}
	return n;
}

Student popCoadaStiva(Nod* &cap) {
	if (cap)
	{
		Student s = cap->info;
		Nod* temp = cap;
		cap = cap->next;
		//free(temp);
		return s;
	}
	else {
		return initStudent(" ",0,0,NULL);
	}
}

void afisareCoadaLista(Nod* &cap) {
	if (cap)
	{
		Nod* coadaTemp = NULL;
		while (cap)
		{
			Student s = popCoadaStiva(cap);
			afisareStudent(s);
			coadaTemp = pushCoada(coadaTemp, s);
		}
		while (coadaTemp) {
			Student s = popCoadaStiva(coadaTemp);
			cap = pushCoada(cap, s);
		}
		
	}
	else
	{
		printf(" - Structura nu a fost initializata - ");
	} 
}

Nod* stergereStiva(Nod* &cap) {
	if (cap)
	{
		while (cap) {
			Nod* temp = cap;
			cap = cap->next;
			free(temp->info.nume);
			free(temp->info.vectorNote);
			free(temp);
		}
		return NULL;
	}
}

void isEmpty(Nod* cap) {
	if (cap)
	{
		printf("Coada nu e goala\n");
	}
	else {
		printf("Coada ESTE goala\n");
	}
}

void main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Nod* cap = NULL;
	FILE* f = fopen("lista_simpla_stive_cozi_citire_fisier.txt", "r");
	while (!feof(f))
	{
		cap = pushCoada(cap, citesteDinFisierStudentul(f));
	}
	fclose(f);
	//afisareCoadaLista(cap);
	//isEmpty(cap);
	//afisareCoadaLista(cap);
	//isEmpty(cap);
	//afisareCoadaLista(cap);
	//isEmpty(cap);
	//afisareCoadaLista(cap);
	//isEmpty(cap);
	afisareCoadaLista(cap);
	cap = stergereStiva(cap);
	afisareCoadaLista(cap);

}