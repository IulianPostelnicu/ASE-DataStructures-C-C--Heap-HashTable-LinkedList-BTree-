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

typedef struct Heap
{
	int dimensiune;
	Student* vector;
};

Heap initHeap(int dimensiune) {
	if (dimensiune>0)
	{
		Heap h;
		h.dimensiune = dimensiune;
		h.vector = (Student*)malloc(sizeof(Student)*dimensiune);
		return h;
	}
}

void filtrareHeap(Heap h, int pozitie) {
	if (h.dimensiune)
	{
		int max = pozitie;
		int pozST = 2 * pozitie + 1;
		int pozDR = 2 * pozitie + 2;
		if (pozST<h.dimensiune && h.vector[max].id<h.vector[pozST].id)
		{
			max = pozST;
		}
		if (pozDR<h.dimensiune && h.vector[max].id<h.vector[pozDR].id)
		{
			max = pozDR;
		}
		if (max!=pozitie)
		{
			Student temp = h.vector[pozitie];
			h.vector[pozitie] = h.vector[max];
			h.vector[max] = temp;
			if ((2*max+1) <h.dimensiune)
			{
				filtrareHeap(h, max);
			}
		}
	}
}

void afisareHeap(Heap h) {
	if (h.dimensiune>0)
	{
		for (int i = 0; i < h.dimensiune; i++)
		{
			afisareStudent(h.vector[i]);
		}
	}
}

void stergereHeap(Heap &h) {
	if (h.dimensiune)
	{
		for (int i = 0; i < h.dimensiune; i++)
		{
			free(h.vector[i].nume);
		}
		free(h.vector);
		h.dimensiune = 0;
	}
}

Student extragereHeap(Heap &h) {
	if (h.dimensiune)
	{
		Student rezultat = h.vector[0];
		Student* temp = (Student*)malloc(sizeof(Student)*(h.dimensiune - 1));
		for (int i = 0; i < h.dimensiune-1; i++)
		{
			temp[i] = h.vector[i + 1];
		}
		free(h.vector);
		h.dimensiune--;
		h.vector = temp;
		for (int i = (h.dimensiune - 2) / 2;i >= 0;i--) {
			filtrareHeap(h, i);
		}
		return rezultat;
	}
}

void inserareInHeap(Heap &h, Student s) {
	if (h.dimensiune>0)
	{
		Student* temp = (Student*)malloc(sizeof(Student)*(h.dimensiune + 1));
		for (int i = 0; i < h.dimensiune; i++)
		{
			temp[i] = h.vector[i];
		}
		temp[h.dimensiune] = s;//pun pe ultima pozitie intrarea noua
		free(h.vector);
		h.dimensiune++;
		h.vector = temp;
		for (int i=(h.dimensiune-2)/2;i>=0;i--)
		{
			filtrareHeap(h, i);
		}
	}
}

typedef struct Nod
{
	Student info;
	Nod* next;
};

Nod* initNod(Student info, Nod* next) {
	Nod* n = (Nod*)malloc(sizeof(Nod));
	n->info = info;
	n->next = next;
	return n;
}

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
		return n;
	}
}

Student popCoada(Nod* &cap) {
	if (cap)
	{
		Student rezultat = cap->info;
		Nod* temp = cap;
		cap = cap->next;
		free(temp);
		return rezultat;
	}
}

void afisareCoada(Nod* &cap) {
	if (cap)
	{
		Nod* temp = NULL;
		while (cap)
		{
			Student s = popCoada(cap);
			afisareStudent(s);
			temp = pushCoada(temp, s);
		}
		while (temp)
		{
			Student s = popCoada(temp);
			cap = pushCoada(cap, s);
		}
	}
}

Nod* inserareCrescatoareInCoada(Nod* &cap, Student info) {
	Nod* n = initNod(info, NULL);
	if (cap)
	{
		if (cap->info.id<info.id)
		{
			cap->next = n;
		}
		else {
			n->next = cap;
			return n;
		}
	}
	else {
		return n;
	}
}

void extragereheapInserareCoada(Heap &h, Nod* &cap) {
	if (h.dimensiune>0)
	{
		while (h.dimensiune>0)
		{
			cap = pushCoada(cap, extragereHeap(h));
			//cap = inserareCrescatoareInCoada(cap, extragereHeap(h));
		}
	}
}

void stergereCoada(Nod* cap) {
	if (cap)
	{
		while (cap)
		{
			Nod* temp = cap;
			cap = cap->next;
			free(temp->info.nume);
			free(temp);
		}
	}
}

typedef struct HashTable
{
	int dimensiune;
	Nod** vector;
};

HashTable initHashTable(int dimensiune) {
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

int functieHAshCode(int dimensiuneHT, int ID) {
	int cod = ID * 17;
	return cod % dimensiuneHT;
}

void inserareHashtable(HashTable &h, Student s) {
	if (h.dimensiune>0)
	{
		int codHash = functieHAshCode(h.dimensiune, s.id);
		if (codHash>=0 && codHash<h.dimensiune)
		{
			if (h.vector[codHash])
			{
				h.vector[codHash] = pushCoada(h.vector[codHash], s);
			}
			else {
				h.vector[codHash] = (Nod*)malloc(sizeof(Nod));
				h.vector[codHash]->info = s;
				h.vector[codHash]->next = NULL;
			}
		}
	}
}

void afisareLista(Nod* cap) {
	if (cap)
	{
		while (cap)
		{
			afisareStudent(cap->info);
			cap = cap->next;
		}
	}
}

void afisareHashTable(HashTable h) {
	if (h.dimensiune>0)
	{
		for (int i = 0; i < h.dimensiune; i++)
		{
			printf("Nivelul %d: \n", i);
			if (h.vector[i])
			{
				afisareLista(h.vector[i]);
			}
		}
	}
}

void stergereHashTable(HashTable &h) {
	if (h.dimensiune)
	{
		for (int i = 0; i < h.dimensiune; i++)
		{
			if (h.vector[i])
			{
				stergereCoada(h.vector[i]);
			}
		}
		free(h.vector);
		h.dimensiune = 0;
	}
}

Student cautareStudentInHashtable(HashTable h, int idCautat) {
	if (h.dimensiune>0)
	{
		int codHash = functieHAshCode(h.dimensiune, idCautat);
		if (codHash>=0 && codHash<h.dimensiune)
		{
			if (h.vector[codHash])
			{
				Nod* temp = h.vector[codHash];
				while (temp)
				{
					if (temp->info.id==idCautat)
					{
						Student rezultat = initStudent(temp->info.id, temp->info.nume);
						return rezultat;
					}
					temp = temp->next;
				}
			}
		}
	}
}

void main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	Heap heap = initHeap(10);
	for (int i = 0; i < 10; i++)
	{
		heap.vector[i] = initStudent(i + 1, "Dummy");
	}
	for (int i = (heap.dimensiune - 2) / 2;i >= 0;i--) {
		filtrareHeap(heap, i);
	}
	//afisareHeap(heap);
	Student s = extragereHeap(heap);
	free(s.nume);

	inserareInHeap(heap, initStudent(20, "SuperStudent"));
	inserareInHeap(heap, initStudent(10, "StudentExtrasAnterior"));

	Nod* coada = NULL;
	extragereheapInserareCoada(heap, coada);
	//afisareCoada(coada);
	//printf("- - -- -\n");
	//afisareCoada(coada);
	HashTable hashtable = initHashTable(3);

	while (coada)
	{
		inserareHashtable(hashtable, popCoada(coada));
	}
	//afisareHashTable(hashtable);

	Student cautat = cautareStudentInHashtable(hashtable, 20);
	afisareStudent(cautat);
	free(cautat.nume);

	stergereHashTable(hashtable);
	stergereCoada(coada);
	stergereHeap(heap);
}

