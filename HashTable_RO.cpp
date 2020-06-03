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
	n->info = info;
	n->next = next;
	return n;
}

Nod* inserareInListaLaFinal(Nod* &cap, Persoana info) {
	Nod* n = initNod(info, NULL);
	if (cap)
	{
		Nod* temp = cap;
		while (temp->next) {
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
		while (cap) {
			afisarePersoana(cap->info);
			cap = cap->next;
		}
	}
	else {
		throw "nu exista lista";
	}
}

Nod* stergereLista(Nod* &cap) {
	if (cap) {
		while (cap)
		{
			Nod* aux = cap;
			cap = cap->next;
			free(aux->info.nume);
			free(aux);
		}
		return NULL;
	}
}

struct HashTable
{
	int dimensiune;
	Nod **vector;
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

int functieHash(int dimensiuneHashtable, const char* nume) {
	int cod = strlen(nume);
	cod *= 17;
	return cod % dimensiuneHashtable;
}

HashTable inserareInHashTable(HashTable h, Persoana p) {
	if (h.dimensiune>0)
	{
		int hashCode = functieHash(h.dimensiune, p.nume);
		if (hashCode>=0 && hashCode<h.dimensiune)
		{
			if (h.vector[hashCode] != NULL) {
				inserareInListaLaFinal(h.vector[hashCode], p);
			}
			else {
				h.vector[hashCode] = (Nod*)malloc(sizeof(Nod));
				h.vector[hashCode]->info = p;
				h.vector[hashCode]->next = NULL;
			}
			return h;
		}
		else {
			throw "eroare";
		}
	}
	else {
		throw "eroare";
	}
}

void stergereHashTable(HashTable &h) {
	if (h.dimensiune>0)
	{
		for (int i =0 ;i < h.dimensiune;i++) {
			if (h.vector[i])
			{
				stergereLista(h.vector[i]);
			}
		}
		free(h.vector);
		h.vector = NULL;
		h.dimensiune = 0;
	}
	else {
		throw "eroare";
	}
}

void afisareHashTable(HashTable h) {
	if (h.dimensiune>0)
	{
		for (int i = 0;i < h.dimensiune;i++) {
			printf("Nivel  %d \n", i);
			if (h.vector[i])
			{
				afisareLista(h.vector[i]);
			}
			else {
				printf("Nivelul %d nu are elemente in el \n", i);
			}
			printf("\n");
		}
	}
	else {
		throw "eroare";
	}
}

Persoana cautarePersoanaInLista(Nod* cap, const char* nume) {
	if (cap)
	{
		while (cap) {
			if (strcmp(cap->info.nume,nume)==0)
			{
				return cap->info;
			}
			cap = cap->next;
		}
	}
}

Persoana cautareInHashTable(HashTable h, const char* numeCautat) {
	if (h.dimensiune>0)
	{
		int cod = functieHash(h.dimensiune, numeCautat);
		if (cod>=0 && cod<h.dimensiune)
		{
			if (h.vector[cod] != NULL) {
				//Persoana rezultat = cautarePersoanaInLista(h.vector[cod], numeCautat);//face shallow copy
				while (h.vector[cod])
				{
					if (strcmp(h.vector[cod]->info.nume, numeCautat) == 0) {
						return initPersoana(h.vector[cod]->info.id, h.vector[cod]->info.nume);
					}
					h.vector[cod] = h.vector[cod]->next;
				}
				throw "nu a fost gasit numele cautat";

			}
			else {
				throw "nu a fost gasit numele cautat";
			}
		}
		else {
			throw "eroare calcul codHash";
		}
	}
	else {
		throw"eroare h.dim=0";
	}
}

void stergePersoanaInHashTable(HashTable h, const char* numeCautat) {
	if (h.dimensiune > 0)
	{
		int cod = functieHash(h.dimensiune, numeCautat);
		if (cod >= 0 && cod < h.dimensiune)
		{
			if (h.vector[cod] != NULL) {
				//Persoana rezultat = cautarePersoanaInLista(h.vector[cod], numeCautat);//face shallow copy
				if (strcmp(h.vector[cod]->info.nume,numeCautat)==0)
				{
					Nod* aux = h.vector[cod];
					h.vector[cod] = h.vector[cod]->next;
					free(aux->info.nume);
					free(aux);
				}
				else {
					while (h.vector[cod])
					{
						Nod* aux = h.vector[cod];
						if (strcmp(aux->info.nume, numeCautat) == 0) {
							Nod* aux2 = aux;
							aux = aux->next;
							free(aux2->info.nume);
							free(aux2);
						}
						aux = aux->next;
					}
					throw "nu a fost gasit numele cautat";
				}

			}
			else {
				throw "nu a fost gasit numele cautat";
			}
		}
		else {
			throw "eroare calcul codHash";
		}
	}
	else {
		throw"eroare h.dim=0";
	}
}


void main() {
	HashTable h = initHashTable(5);
	h = inserareInHashTable(h, initPersoana(10, "danDAnadwad"));
	h = inserareInHashTable(h, initPersoana(11, "danDAnadwa"));
	h = inserareInHashTable(h, initPersoana(12, "danDAnssssssssss"));
	h = inserareInHashTable(h, initPersoana(13, "dan"));
	h = inserareInHashTable(h, initPersoana(14, "danDAnsadwad"));
	h = inserareInHashTable(h, initPersoana(15, "danDAnadwghtrtrjtrt"));
	afisareHashTable(h);
	//stergereHashTable(h);
	//afisareHashTable(h);
	printf("--- -- - - - - - -- \n");
	stergePersoanaInHashTable(h, "dan");
	afisareHashTable(h);



}