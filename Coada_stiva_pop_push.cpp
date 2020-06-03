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

Nod* initNod(Persoana Info, Nod* next) {
	Nod* n = (Nod*)malloc(sizeof(Nod));
	n->info = Info;
	n->next = next;
	return n;
}

void coada_push(Nod* &cap, Persoana Info) {
	Nod* n = initNod(Info, NULL);
	if (cap)
	{
		Nod* temp = cap;
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = n;
	}
	else
	{
		cap = n;
	}

}

void stiva_push(Nod* &cap, Persoana info) {
	Nod* n = initNod(info, NULL);
	if (cap)
	{
		n->next = cap;
		cap = n;
	}
	else
	{
		cap = n;
	}
}

Persoana coada_stiva_pop(Nod* &cap) {
	if (cap)
	{
		Nod* temp = cap;
		Persoana rezultat = initPersoana(cap->info.id, cap->info.nume);
		cap = cap->next;
		free(temp->info.nume);
		free(temp);
		return rezultat;
	}
	else {
		return initPersoana(-1, " ");
	}
}

void afisareElemente_Coada_Stiva(Nod* &cap) {
	if (cap)
	{
		while (cap) {
			Persoana p = coada_stiva_pop(cap);
			afisarePersoana(p);
		}
	}
	else
	{
		printf("Coada este goala!\n");
	}
}

void isEmpty(Nod* cap) {
	if (cap)
	{
		printf("Structura nu este Empty!\n");
	}
	else {
		printf("Structura ESTE Empty!\n");
	}
}

void main() {
	Nod* p = NULL;
	coada_push(p, initPersoana(10, "s"));
	coada_push(p, initPersoana(11, "s2"));
	coada_push(p, initPersoana(12, "s3"));
	afisareElemente_Coada_Stiva(p);
	printf("- -- - - - - - - - - - - - -\n");
	afisareElemente_Coada_Stiva(p);
	printf("- -- - - - - - - - - - - - -\n");
	//repopulare coada
	coada_push(p, initPersoana(10, "s"));
	coada_push(p, initPersoana(11, "s2"));
	coada_push(p, initPersoana(12, "s3"));
	afisarePersoana(coada_stiva_pop(p));
	printf("- -- - - - - - - - - - - - -\n");
	afisareElemente_Coada_Stiva(p);
	printf("- -- - - - - - - - - - - - -\n");
	stiva_push(p, initPersoana(99, "popescu"));
	stiva_push(p, initPersoana(10, "s"));
	stiva_push(p, initPersoana(11, "s2"));
	stiva_push(p, initPersoana(12, "s3"));
	afisareElemente_Coada_Stiva(p);
	printf("- -- - - - - - - - - - - - -\n");
	isEmpty(p);
	//OUTPUT
	 //   10 s
		//11 s2
		//12 s3
		//- -- - -----------
		//Coada este goala!
		//- -- - -----------
		//10 s
		//- -- - -----------
		//11 s2
		//12 s3
		//- -- - -----------
		//12 s3
		//11 s2
		//10 s
		//99 popescu
		//- -- - -----------
		//Structura ESTE Empty!


}