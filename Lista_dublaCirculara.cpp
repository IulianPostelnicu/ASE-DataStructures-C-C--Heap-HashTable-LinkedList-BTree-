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
	int nrNote;
	int* vectorNote;
};

Student initStudent(int id, const char* nume, int nrNote, int* vectorNote) {
	Student s;
	s.id = id;
	s.nume = (char*)malloc(sizeof(char)*(strlen(nume) + 1));
	strcpy(s.nume, nume);
	s.nrNote = nrNote;
	s.vectorNote = (int*)malloc(sizeof(int)*nrNote);
	for (int i = 0; i < nrNote; i++)
	{
		s.vectorNote[i] = vectorNote[i];
	}
	return s;
}

Student citesteStudent(FILE* f) {
	Student s;
	fscanf(f, "%d", &s.id);
	char aux[20];
	fscanf(f, "%s", aux);
	s.nume = (char*)malloc(sizeof(char)*(strlen(aux) + 1));
	strcpy(s.nume, aux);
	fscanf(f, "%d", &s.nrNote);
	s.vectorNote = (int*)malloc(sizeof(int)*s.nrNote);
	for (int i = 0; i < s.nrNote; i++)
	{
		fscanf(f, "%d", &s.vectorNote[i]);
	}
	return s;
}

void afisareStudent(Student s) {
	printf("Studentul cu id:%d -%s are %d note: ", s.id, s.nume, s.nrNote);
	for (int i = 0; i < s.nrNote; i++)
	{
		printf("%d, ", s.vectorNote[i]);
	}
	printf("\n");
}

typedef	struct NodDublu
{
	Student info;
	NodDublu* next;
	NodDublu* prev;
};

NodDublu* initNod(Student info, NodDublu* next, NodDublu* prev) {
	NodDublu* n = (NodDublu*)malloc(sizeof(NodDublu));
	n->info = info;
	n->next = next;
	n->prev = prev;
	return n;
}

typedef struct ListaCirculara
{
	NodDublu* primulNod;
	NodDublu* ultimulNod;
};

void inserareInLista(ListaCirculara &lista, Student info) {
	NodDublu* n = initNod(info, NULL, NULL);
	if (lista.ultimulNod)
	{
		//NodDublu* temp = lista.ultimulNod;
		lista.ultimulNod->next = n;
		lista.primulNod->prev = n;
		n->next = lista.primulNod;
		n->prev = lista.ultimulNod;
		lista.ultimulNod = n;
	}
	else {
		n->prev = n->prev = n;
		lista.primulNod = lista.ultimulNod = n;
	}
}

void afisareListaDubla(ListaCirculara lista) {
	if (lista.primulNod)
	{
		NodDublu* temp = lista.primulNod;
		while (temp->next!=lista.primulNod)
		{
			afisareStudent(temp->info);
			temp = temp->next;
		}
		afisareStudent(temp->info);
	}
}

void stergereListaDubla(ListaCirculara &lista) {
	if (lista.primulNod)
	{
		NodDublu* temp = lista.primulNod;
		while (lista.primulNod->next!=temp)
		{
			NodDublu* temp2 = lista.primulNod;
			lista.primulNod = lista.primulNod->next;
			free(temp2->info.nume);
			free(temp2->info.vectorNote);
			free(temp2);
		}
		free(lista.primulNod->info.nume);
		free(lista.primulNod->info.vectorNote);
		free(lista.primulNod);
		lista.primulNod = lista.ultimulNod = NULL;
	}
}

Student extragerePrimulElement(ListaCirculara &lista) {
	if (lista.primulNod)
	{
		Student rezultat = lista.primulNod->info;
		NodDublu* temp = lista.primulNod->next;
		free(lista.primulNod);
		lista.primulNod = temp;
		lista.ultimulNod->next = temp;
		return rezultat;
	}
}

void stergereElementCautat(ListaCirculara &lista, const char* nume) {
	if (lista.primulNod)
	{
		if (strcmp(lista.primulNod->info.nume,nume)==0)
		{
			NodDublu* temp = lista.primulNod;
			lista.primulNod = lista.primulNod->next;
			lista.ultimulNod->next = lista.primulNod;
			free(temp->info.nume);
			free(temp->info.vectorNote);
			free(temp);
			return;
		}
		else {
			NodDublu* temp = lista.primulNod;
			while (temp->next!=lista.primulNod)
			{
				if (strcmp(temp->info.nume,nume)==0)
				{
					NodDublu* temp2 = temp;
					temp->prev->next = temp->next; //sar peste nodul identificat
					temp->next->prev = temp->prev;
					temp = temp->next;
					free(temp2->info.nume);
					free(temp2->info.vectorNote);
					free(temp2);
					return;
				}
				temp = temp->next;
			}
			if (strcmp(temp->info.nume, nume) == 0)
			{
				NodDublu* temp2 = temp;
				temp->prev->next = temp->next; //sar peste nodul identificat
				temp->next->prev = temp->prev;
				temp = temp->next;
				free(temp2->info.nume);
				free(temp2->info.vectorNote);
				free(temp2);
				return;
			}
		}
	}
}



void main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ListaCirculara lista;
	lista.primulNod = lista.ultimulNod = NULL;

	FILE* f = fopen("Migrare_coada_maxHeap.txt", "r");
	if (f)
	{
		while (!feof(f))
		{
			inserareInLista(lista, citesteStudent(f));
		}
	}
	else {
		printf("nu a fost identificat fisierul \n");
	}
	fclose(f);
	Student s = extragerePrimulElement(lista);
	free(s.nume);
	free(s.vectorNote);
	//afisareListaDubla(lista);
	printf("- - - - \n");
	stergereElementCautat(lista, "Bibescu");
	afisareListaDubla(lista);

	stergereListaDubla(lista);
}