#include<iostream>
#include<string.h>

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
	Nod* next;
};

Nod* initNod(Persoana info, Nod* next) {
	Nod* n = (Nod*)malloc(sizeof(Nod));
	n->info = info;//shallow copy
	n->next = next;
	return n;
}

//inserare final

Nod* inserareFinal(Nod* &cap, Persoana info) {
	Nod* n = initNod(info, NULL);
	if (cap)
	{
		Nod* temp = cap;
		while (temp->next !=cap)
		{
			temp = temp->next;
		}
		temp->next = n;
		n->next = cap;
	}
	else
	{
		cap = n;
		cap->next = cap;
	}
	return cap;
}

void afisareListaCirculara(Nod* cap) {
	if (cap)
	{
		Nod* temp = cap;
		while (temp->next!=cap)
		{
			afisarePersoana(temp->info);
			temp = temp->next;
		}
		afisarePersoana(temp->info);
	}
}

//util pentru alocare memorie vector
int numarElementListaCirculara(Nod* cap) {
	if (cap)
	{
		int n = 1;
		Nod* temp = cap;
		while (temp->next != cap)
		{
			n++;
			temp = temp->next;
		}
		return n;
	}
}

//trecere din LSC in VEector prin deep copy
Persoana*  transformareInVector(Nod* cap) {
	if (cap)
	{
		Persoana *p = (Persoana*)malloc(sizeof(Persoana)*numarElementListaCirculara(cap));
		Nod* temp = cap;
		int i = 0;
		while (temp->next!=cap)
		{
			p[i] = initPersoana(temp->info.id, temp->info.nume);
			i++;
			temp = temp->next;
		}
		p[i] = temp->info;
		return p;
	}
	else
	{
		return NULL;
	}

}

void afisarevector(Persoana* p, int dimensiune) {
	for (int i = 0; i < dimensiune; i++)
	{
		afisarePersoana(p[i]);
	}
}

Nod* stergereLista(Nod* &cap) {
	if (cap)
	{
		Nod* temp = cap;
		while (temp->next != cap) {
			Nod* temp2 = temp;
			temp = temp->next;
			free(temp2->info.nume);
			free(temp2);
			
		}
		free(temp->info.nume);
		free(temp);
		return NULL;
	}
	else {
		throw "lista nu a fost initializata";
	}
}

bool isEmpty(Nod* cap) {
	return cap != NULL;
}



void main() {
	Nod* cap = NULL;
	cap = inserareFinal(cap, initPersoana(10, "ion"));
	cap = inserareFinal(cap, initPersoana(11, "ion2"));
	cap = inserareFinal(cap, initPersoana(12, "ion3"));
	afisareListaCirculara(cap);
	printf("%d \n\n\n", numarElementListaCirculara(cap));
	Persoana *vector = (Persoana*)malloc(sizeof(Persoana)*numarElementListaCirculara(cap));
	vector = transformareInVector(cap);
	afisarevector(vector, numarElementListaCirculara(cap));
	cap = stergereLista(cap);
	bool isEmp = isEmpty(cap);

}