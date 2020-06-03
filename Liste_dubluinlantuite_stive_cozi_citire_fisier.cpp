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
	Nod* prev;
}Nod;

Nod* initNod(Student info, Nod* next, Nod* prev) {
	Nod* n = (Nod*)malloc(sizeof(Nod));
	n->info = info;
	n->next = next;
	n->prev = prev;
	return n;
}

typedef struct Lista
{
	Nod* primulNod;
	Nod* ultimulNod;
}Lista;

Lista pushCoada(Lista l, Student s) {
	Nod* n = initNod(s, NULL, NULL);
	if (l.ultimulNod)
	{
		n->prev = l.ultimulNod;
		l.ultimulNod->next = n;
		l.ultimulNod = n;
	}
	else {
		l.primulNod = l.ultimulNod = n;
	}
	return l;
}

Student popCoadaStiva(Lista &l) {
	if (l.primulNod)
	{
		Student s = l.primulNod->info;
		Nod* temp = l.primulNod;
		l.primulNod = l.primulNod->next;
		free(temp);
		return s;
	}
	else {
		return initStudent("", 0, 0, NULL);
	}
}

void afisareLista(Lista &l) {
	if (l.primulNod)
	{
		Lista listaTemp;
		listaTemp.primulNod = listaTemp.ultimulNod = NULL;
		while (l.primulNod)
		{
			Student s = popCoadaStiva(l);
			afisareStudent(s);
			listaTemp = pushCoada(listaTemp, s);
		}
		l.primulNod = l.ultimulNod = NULL;
		while (listaTemp.primulNod)
		{
			Student s = popCoadaStiva(listaTemp);
			l = pushCoada(l, s);
		}
	}
	else {
		printf("Structura este goala!");
	}
}

int counterElemente = 0;
int counterElementeCoada(Lista &l) {
	if (l.primulNod)
	{
		Lista listaTemp;
		listaTemp.primulNod = listaTemp.ultimulNod = NULL;
		while (l.primulNod)
		{
			Student s = popCoadaStiva(l);
			//afisareStudent(s);
			listaTemp = pushCoada(listaTemp, s);
			counterElemente++;
		}
		l.primulNod = l.ultimulNod = NULL;
		while (listaTemp.primulNod)
		{
			Student s = popCoadaStiva(listaTemp);
			l = pushCoada(l, s);
		}
		return counterElemente;
	}
	else {
		return -1;
	}
}

void stergereStructura(Lista &lista) {
	if (lista.primulNod)
	{
		while (lista.primulNod) {
			Nod* temp = lista.primulNod;
			lista.primulNod = lista.primulNod->next;
			free(temp->info.nume);
			free(temp->info.vectorNote);
			free(temp);
		}
		lista.primulNod = lista.ultimulNod = NULL;
	}
}

Student* conversieCoadaInVector(Lista &l) {
	if (l.primulNod)
	{
		int dimensiuneVector = counterElementeCoada(l);
		Student* vectorStudenti = (Student*)malloc(sizeof(Student)*dimensiuneVector);
		for (int i = 0; i < dimensiuneVector; i++)
		{
			vectorStudenti[i] = popCoadaStiva(l);
		}
		return vectorStudenti;
	}
	else {
		return NULL;
	}
}

void conversieCoadaInVectorVersiunea2(Lista &l,Student* vector, int dimVect) {
	if (l.primulNod)
	{
		for (int i = 0; i < dimVect; i++)
		{
			vector[i] = popCoadaStiva(l);
		}
	}
	else {
		printf("Structura este goala!\n");
	}
}

void afisareVector(Student*s, int dimensiuneVector) {
	for (int i = 0; i < dimensiuneVector; i++)
	{
		printf("vector[%d]:", i);
		afisareStudent(s[i]);
	}
}

Student* stergeVector(Student* vector, int dimensiune) {
	for (int i = 0; i < dimensiune; i++)
	{
		free(vector[i].nume);
		free(vector[i].vectorNote);
	}
	free(vector);
	return NULL;
}

void main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Lista lista;
	lista.primulNod = lista.ultimulNod = NULL;
	FILE* f = fopen("lista_simpla_stive_cozi_citire_fisier.txt", "r");
	while (!feof(f))
	{
		lista = pushCoada(lista, citesteDinFisierStudentul(f));
	}
	fclose(f);
	//afisareStudent(popCoadaStiva(lista)); //memory leaks aici daca lasam asa
	//Student s = popCoadaStiva(lista);
	//afisareStudent(s);
	//free(s.nume);
	//free(s.vectorNote);
	//
	//printf("- - - - - -\n");
	//int counter = counterElementeCoada(lista);
	//printf("**In lista sunt %d elemente", counter);
	//printf("- - - - - -\n");
	/*int dimensiuneVector = counterElementeCoada(lista);
	Student* vector = conversieCoadaInVector(lista);*/
	//printf("Afisare Vector \n");
	//afisareVector(vector, dimensiuneVector);
	//vector = stergeVector(vector, dimensiuneVector);
	int dim =counterElementeCoada(lista);
	Student* vector2 = (Student*)malloc(sizeof(Student)*dim);
	conversieCoadaInVectorVersiunea2(lista, vector2, dim);
	afisareVector(vector2,dim);
	vector2 = stergeVector(vector2, dim);
	afisareLista(lista);
	stergereStructura(lista);
} 