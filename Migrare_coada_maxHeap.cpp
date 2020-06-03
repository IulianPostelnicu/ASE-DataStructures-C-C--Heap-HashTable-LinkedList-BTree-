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

void afisareStudent(Student s) {
	printf("Studentul cu id:%d -%s are %d note: ", s.id, s.nume, s.nrNote);
	for (int i = 0; i < s.nrNote; i++)
	{
		printf("%d, ", s.vectorNote[i]);
	}
	printf("\n");
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
		cap = n;
		return cap;
	}
}

Student popCoada(Nod* &cap) {
	if (cap)
	{
		Student s = initStudent(cap->info.id, cap->info.nume, cap->info.nrNote, cap->info.vectorNote);
		Nod* temp = cap;
		cap = cap->next;
		free(temp->info.nume);
		free(temp->info.vectorNote);
		free(temp);
		return s;
	}
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

void afisareCoada(Nod* &cap) {
	if (cap)
	{
		Nod* capTemp = NULL;
		while (cap)
		{
			Student s = popCoada(cap);
			afisareStudent(s);
			capTemp = pushCoada(capTemp, s);
		}
		while (capTemp)
		{
			Student s = popCoada(capTemp);
			cap = pushCoada(cap, s);
		}
	}
}

int afisareCoadaCuReturnareNumarElemente(Nod* &cap) {
	if (cap)
	{
		int counter = 0;
		Nod* capTemp = NULL;
		while (cap)
		{
			Student s = popCoada(cap);
			afisareStudent(s);
			capTemp = pushCoada(capTemp, s);
			counter++;
		}
		while (capTemp)
		{
			Student s = popCoada(capTemp);
			cap = pushCoada(cap, s);
		}
		return counter;
	}
}

void stergereCoada(Nod* &cap) {
	if (cap)
	{
		while (cap)
		{
			Nod* temp = cap;
			cap = cap->next;
			free(temp->info.nume);
			free(temp->info.vectorNote);
			free(temp);
		}
	}
}

typedef struct Heap
{
	int dimensiune;
	Student* vectorStudenti;
};

Heap initHeap(int dimensiune) {
	Heap h;
	h.dimensiune = dimensiune;
	h.vectorStudenti = (Student*)malloc(sizeof(Student)*dimensiune);
	return h;
}

void sortareHeap(Heap h, int pozitie) {
	if (h.dimensiune>0)
	{
		int max = pozitie;
		int hSt = 2 * pozitie + 1;
		int hDr = 2 * pozitie + 2;
		if (hSt<h.dimensiune && h.vectorStudenti[max].id<h.vectorStudenti[hSt].id)
		{
			max = hSt;
		}
		if (hDr<h.dimensiune && h.vectorStudenti[max].id<h.vectorStudenti[hDr].id)
		{
			max = hDr;
		}
		if (max!=pozitie)
		{
			Student aux = h.vectorStudenti[pozitie];
			h.vectorStudenti[pozitie] = h.vectorStudenti[max];
			h.vectorStudenti[max] = aux;
			if (2*max+1<h.dimensiune)
			{
				sortareHeap(h, max);
			}
		}
	}
}

void afisareHeap(Heap h) {
	if (h.dimensiune>0)
	{
		for (int i = 0; i < h.dimensiune; i++)
		{
			afisareStudent(h.vectorStudenti[i]);
		}
	}
}

void stergereHeap(Heap h) {
	if (h.dimensiune)
	{
		for (int i = 0; i < h.dimensiune; i++)
		{
			free(h.vectorStudenti[i].nume);
			free(h.vectorStudenti[i].vectorNote);
		}
		free(h.vectorStudenti);
		h.dimensiune = 0;
	}
}

Nod* eliminaDinCoada(Nod* &cap, int idCautat) {
	if (cap)
	{
		Nod* temp = NULL;
		while (cap)
		{
			Student s = popCoada(cap);
			if (s.id!=idCautat)
			{
				temp = pushCoada(temp, s);
			}
			else {
				free(s.nume);
				free(s.vectorNote);
			}
		}
		while (temp)
		{
			Student s = popCoada(temp);
			cap = pushCoada(cap, s);
		}
		return cap;
	}
}

void stergereVector(Student* vector, int dimensiune) {
	for (int i = 0; i < dimensiune; i++)
	{
		free(vector[i].nume);
		free(vector[i].vectorNote);
	}
	vector = NULL;
}

void inserareInHeap(Heap &h, Student s) {
	if (h.dimensiune>0)
	{
		Student *aux = (Student*)malloc(sizeof(Student)*(h.dimensiune + 1));
		for (int i = 0; i < h.dimensiune; i++)
		{
			aux[i] = h.vectorStudenti[i];
		}
		aux[h.dimensiune] = s;
		free(h.vectorStudenti);
		h.dimensiune++;
		h.vectorStudenti = aux;
		for (int i = (h.dimensiune - 2) / 2;i >= 0;i--) {
			sortareHeap(h, i);
		}
	}
}



void main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Nod* cap = NULL;

	FILE* f = fopen("Migrare_coada_maxHeap.txt", "r");
	if (f)
	{
		while (!feof(f))
		{
			cap = pushCoada(cap, citesteStudent(f));
		}
	}
	else {
		printf("nu a fost identificat fisierul \n");
	}
	fclose(f);
	//Student s = popCoada(cap);
	//afisareStudent(s);
	//free(s.nume);
	//free(s.vectorNote);
	printf("*Afisare Coada \n");
	cap = eliminaDinCoada(cap, 14);
	int dimHeap = afisareCoadaCuReturnareNumarElemente(cap);
	Heap heap = initHeap(dimHeap);
	int i = 0;
	while (cap)
	{
		heap.vectorStudenti[i] = popCoada(cap);
		i++;
	}
	
	printf("*Afisare Heap nesortat \n");
	afisareHeap(heap);
	printf("*Afisare Heap Sortat \n");
	for (int i = (heap.dimensiune-2)/2;i>=0;i--)
	{
		sortareHeap(heap, i);
	}
	afisareHeap(heap);

	printf("***Inserare in heap \n");
	int vector[2] = { 5,6 };
	inserareInHeap(heap, initStudent(99, "SuperStudent", 2, vector));
	afisareHeap(heap);


	stergereHeap(heap);
	stergereCoada(cap);

}