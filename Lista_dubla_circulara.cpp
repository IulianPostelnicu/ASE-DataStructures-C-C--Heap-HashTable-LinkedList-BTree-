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

//lista circulara contine 2 pointeri ce ponteaza catre urmatorul si anteriorul nod
//primul nod pointeaza catre el insusi, la afisare, stergere sau introducere date trebuie introdus un nod TEMP ce are rol de counter
//fara TEMP se produce infinite looop....

struct Nod
{
	Persoana info;
	Nod* next;
	Nod* prev;
};

Nod* initNod(Persoana info, Nod* next, Nod* prev) {
	Nod* n = (Nod*)malloc(sizeof(Nod));
	n->info = info; //shallow copy
	n->next = next;
	n->prev = prev;
	return n;
}

struct Lista
{
	Nod* primulNod;
	Nod* ultimuNod;
};

//Lista inserareInLista(Lista l, Persoana info) {
//	Nod* n = initNod(info, NULL, NULL);
//	if (l.primulNod)
//	{
//		Nod* temp = l.primulNod;
//		while (temp->next!=l.primulNod)
//		{
//			temp = temp->next;
//		}
//		temp->next = n;
//		n->next = l.primulNod;
//		l.primulNod->prev = n;
//	}
//	else
//	{
//		l.primulNod = n;
//		n->next = n;
//		n->prev = n;
//	}
//}

Lista inserareInListaLaInceput(Lista l, Persoana info) {
	Nod* n = initNod(info, NULL, NULL);
	if (l.primulNod)
	{
		//similar cu lista dubla simpla
		n->next = l.primulNod;
		n->prev = l.ultimuNod;
		//legatura pentru lista circulara
		l.primulNod->prev = n;
		l.ultimuNod->next = n;
		l.primulNod = n;
	}
	else
	{
		l.primulNod = l.ultimuNod = n;
		n->next = n->prev = n;
	}
	return l;
}

Lista inserareInListaLaFInal(Lista l, Persoana info) {
	Nod* n = initNod(info, NULL, NULL);
	if (l.ultimuNod)
	{
		n->prev = l.ultimuNod;
		l.ultimuNod->next = n;
		n->next = l.primulNod;
		l.primulNod->prev = n;
		l.ultimuNod = n;
	}
	else
	{
		l.primulNod = l.ultimuNod = n;
		n->prev = n->next = n;
	}
	return l;
}

void afisareListaDubla(Lista l) {
	Nod* temp = l.primulNod;
	if (l.primulNod)
	{
		while (temp->next != l.primulNod) {
			afisarePersoana(temp->info);
			temp = temp->next;
		}
		afisarePersoana(temp->info);
	}
	else {
		printf("Lista este goala! \n");
	}
}

void stergereListaDublaCIrculara(Lista &l) {
	if (l.primulNod)
	{
		Nod* temp = l.primulNod;
		while (l.primulNod->next!=temp) {
			Nod* temp2 = l.primulNod;
			l.primulNod = l.primulNod->next;
			free(temp2->info.nume);
			free(temp2);
		}
		free(l.primulNod->info.nume);
		free(l.primulNod);
		l.primulNod = l.ultimuNod = NULL;
	}
	else
	{
		printf("Lista a fost stearsa!");
	}
}

//Persoana extragerePersoanaDinLista(Lista &l, const char* numeCautat) {
//	if (l.primulNod)
//	{
//		Nod* temp = l.primulNod;
//		while (temp->next!=l.primulNod)
//		{
//			if (strcmp(temp->info.nume, numeCautat) == 0) {
//				Persoana rezultat = initPersoana(temp->info.id, temp->info.nume);//deep copy
//				Nod* temp2 = temp;
//				temp->prev->next= temp->next;
//				temp->next->prev = temp->prev;
//				temp = temp->next;
//				free(temp2->info.nume);
//				free(temp2);
//				return rezultat; //returnez rezultatul 
//			}
//			temp = temp->next;//trec la urmatorul nod
//		}
//		if (strcmp(temp->info.nume, numeCautat) == 0) { //caut si pe ultima pozitie a listei
//			Persoana rezultat = initPersoana(temp->info.id, temp->info.nume);//deep copy
//			Nod* temp2 = temp;
//			temp->prev->next = temp->next;
//			temp->next->prev = temp->prev;
//			temp = l.primulNod;
//			free(temp2->info.nume);
//			free(temp2);
//			return rezultat; //returnez rezultatul 
//		}
//		else
//		{
//			throw "nu Exista in list persoana cautata";
//		}
//	}
//	else
//	{
//		throw "lista este goala";
//	}
//}

Persoana extragerePersoanaDinLista(Lista &l, const char* numeCautat) {
	if (l.primulNod)
	{
		if (strcmp(l.primulNod->info.nume,numeCautat)==0) //daca este fix primul element
		{
			Persoana rezultat = initPersoana(l.primulNod->info.id, l.primulNod->info.nume);//deep copy
			Nod* temp = l.primulNod;
			l.primulNod = l.primulNod->next;
			l.ultimuNod->next = l.primulNod;
			free(temp->info.nume);
			free(temp);
			return rezultat;
		}
		else {
			Nod* temp = l.primulNod->next;
			while (temp->next != l.primulNod)
			{
				if (strcmp(temp->info.nume, numeCautat) == 0) {
					Persoana rezultat = initPersoana(temp->info.id, temp->info.nume);//deep copy
					Nod* temp2 = temp;
					temp->prev->next = temp->next;
					temp->next->prev = temp->prev;
					temp = temp->next;
					free(temp2->info.nume);
					free(temp2);
					return rezultat; //returnez rezultatul 
				}
				temp = temp->next;//trec la urmatorul nod
			}
			if (strcmp(temp->info.nume, numeCautat) == 0) { //caut si pe ultima pozitie a listei
				Persoana rezultat = initPersoana(temp->info.id, temp->info.nume);//deep copy
				Nod* temp2 = temp;
				temp->prev->next = temp->next;
				temp->next->prev = temp->prev;
				temp = l.primulNod;
				free(temp2->info.nume);
				free(temp2);
				return rezultat; //returnez rezultatul 
			}
			else
			{
				throw "nu Exista in list persoana cautata";
			}
		}
	}
	else
	{
		throw "lista este goala";
	}
}

void main() {
	Lista lista;
	lista.primulNod = lista.primulNod = NULL;
	lista = inserareInListaLaInceput(lista, initPersoana(1, "dan1"));
	lista = inserareInListaLaInceput(lista, initPersoana(2, "dan2"));
	lista = inserareInListaLaInceput(lista, initPersoana(3, "dan3"));
	lista = inserareInListaLaInceput(lista, initPersoana(4, "dan4"));
	lista = inserareInListaLaInceput(lista, initPersoana(5, "dan5"));
	afisareListaDubla(lista);
	printf("- - - -- - - -- - - -\n");
	//stergereListaDublaCIrculara(lista);
	printf("- - - -- - - -- - - -\n");
	afisareListaDubla(lista);
	printf("- - - -- - - -- - - -\n");
	Persoana cautat = extragerePersoanaDinLista(lista, "dan1");
	afisarePersoana(cautat);
	printf("- - - -- - - -- - - -\n");
	afisareListaDubla(lista);
	printf("- - - -- - - -- - - -\n");
}

