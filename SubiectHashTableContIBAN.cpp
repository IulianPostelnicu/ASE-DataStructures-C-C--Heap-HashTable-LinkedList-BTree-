#include"stdlib.h"
#include"string.h"
#include"stdio.h"
#include"conio.h"

typedef struct ContBancar
{
	char codIBAN[24];
	char* denumireClient;
	float suma;
	char* modPlata;
};

ContBancar initContbancar(const char* codIBAN, const char* denumireClient, float suma,const  char* modPlata) {
	ContBancar c;
	strcpy(c.codIBAN, codIBAN);
	c.denumireClient = (char*)malloc(sizeof(char)*(strlen(denumireClient) + 1));
	strcpy(c.denumireClient, denumireClient);
	c.suma = suma;
	c.modPlata = (char*)malloc(sizeof(char)*(strlen(modPlata) + 1));
	strcpy(c.modPlata, modPlata);
	return c;
}

void afisareContBancar(ContBancar c) {
	printf("IBAN %s CLIENT:%s Suma %5.2f lei Plata: %s \n", c.codIBAN, c.denumireClient, c.suma, c.modPlata);
}

typedef struct Nod {
	ContBancar info;
	Nod* next;
};

Nod* initNod(ContBancar info, Nod* next) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = info;
	nou->next = next;
	return nou;
}

Nod* inserareFinal(Nod* &cap, ContBancar info) {
	Nod* nou = initNod(info, NULL);
	if (cap)
	{
		Nod* temp = cap;
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = nou;
		return cap;
	}
	else {
		return nou;
	}
}

void afisareLista(Nod* cap) {
	if (cap)
	{
		while (cap)
		{
			afisareContBancar(cap->info);
			cap = cap->next;
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

int hashFunction(int dimensiune, const char* IBAN) {
	if (dimensiune>0)
	{
		int cod = 0;
		for (int i = 0; i < 5; i++)
		{
			cod += IBAN[i];
		}
		cod *= 17;
		return cod % dimensiune;
	}
}

void inserareInHashTable(HashTable h, ContBancar c) {
	if (h.dimensiune>0)
	{
		int cod = hashFunction(h.dimensiune, c.codIBAN);
		if (cod>=0 && cod<h.dimensiune)
		{
			if (h.vector[cod])
			{
				h.vector[cod] = inserareFinal(h.vector[cod], c);
			}
			else {
				h.vector[cod] = (Nod*)malloc(sizeof(Nod));
				h.vector[cod]->info = c;
				h.vector[cod]->next = NULL;
			}
		}
		else {
			printf("eroare calcul cod hash!");
		}
	}
}

void afisareHashTable(HashTable h) {
	if (h.dimensiune>0)
	{
		for (int i = 0; i < h.dimensiune; i++)
		{
			printf("Nivel %i: \n", i);
			if (h.vector[i])
			{
				afisareLista(h.vector[i]);
			}
		}
	}
	else {
		printf("Tabela are dimensiunea 0 \n");
	}
}

Nod* stergereLista(Nod* &cap) {
	if (cap)
	{
		while (cap)
		{
			Nod* temp = cap;
			cap = cap->next;
			free(temp->info.denumireClient);
			free(temp->info.modPlata);
			free(temp);
		}
		return NULL;

	}
}

void dezalocareHashTable(HashTable &h) {
	if (h.dimensiune>0)
	{
		for (int i = 0; i < h.dimensiune; i++)
		{
			if (h.vector[i])
			{
				h.vector[i]=stergereLista(h.vector[i]);
			}
		}
		free(h.vector);
		h.dimensiune = 0; //important sa anunt stergerea listei
	}
}

void functieCalculMemorieOcupata(HashTable h) {
	if (h.dimensiune>0)
	{
		int totalmemorieOcupata = 0;
		for (int i = 0; i < h.dimensiune; i++)
		{
			Nod* temp = h.vector[i];
			if (temp)
			{
				while (temp)
				{
					totalmemorieOcupata += sizeof(temp->info.codIBAN) + sizeof(temp->info.denumireClient) + sizeof(temp->info.modPlata) + sizeof(temp->info.suma);
					temp = temp->next;
				}
			}
		}
		printf("*Memoria ocupata de datele din Hashtable este de %d \n", totalmemorieOcupata);
	}
}


int counterPlati50000 = 0;
int dimensiuneVectorPlatiPeste5000(HashTable h, float filtru) {
	if (h.dimensiune>0)
	{
		for (int i = 0; i < h.dimensiune; i++)
		{
			Nod* temp = h.vector[i];
			if (temp)
			{
				while (temp)
				{
					if (temp->info.suma>filtru)
					{
						counterPlati50000++;
					}
					temp = temp->next;
				}
			}
		}
		return counterPlati50000;
	}
}

void functieVectorDePlati(HashTable h, float filtru) {
	if (h.dimensiune>0)
	{
		counterPlati50000 = 0;
		counterPlati50000 = dimensiuneVectorPlatiPeste5000(h, filtru);
		float* vectorDinamic = (float*)malloc(sizeof(float)*counterPlati50000);
		int index = 0;
		for (int i = 0; i < h.dimensiune; i++)
		{
			if (h.vector[i])
			{
				Nod* temp = h.vector[i];
				if (temp)
				{
					while (temp)
					{
						if (temp->info.suma > filtru)
						{
							vectorDinamic[index] = temp->info.suma;
							index++;
						}
						temp = temp->next;
					}
				}
			}
		}
		if (counterPlati50000>0)//verifica daca am ce sa arat la printare
		{
			printf("**Urmatoarele plati sunt mai mari decat %5.2f: \n", filtru);
			for (int i = 0; i < counterPlati50000; i++)
			{
				printf("%5.2f, ", vectorDinamic[i]);
			}
		}
		free(vectorDinamic);
	}
}

void scriereListaInFisier(Nod* cap, FILE* g) {
	if (cap)
	{
		fprintf(g, "- - - - G A T E G O R I E- - - - \n");
		while (cap)
		{
			fprintf(g,"IBAN %s CLIENT:%s Suma %5.2f lei Plata: %s \n", cap->info.codIBAN, cap->info.denumireClient, cap->info.suma, cap->info.modPlata);
			cap = cap->next;
		}
	}
}


//cod banca=5 caractere+1 final sir de caractere
void generareRaportBanca(HashTable h,const char banca[6]) {
	FILE* g = fopen("raport.txt", "w");
	Nod* listaCash = NULL;
	Nod* listaElectronic = NULL;
	if (h.dimensiune>0)
	{
		int codHashBancaDeCautat = hashFunction(h.dimensiune, banca);
		if (codHashBancaDeCautat>=0 && codHashBancaDeCautat<h.dimensiune)
		{
			if (h.vector[codHashBancaDeCautat])
			{
				Nod* temp = h.vector[codHashBancaDeCautat];
				while (temp)
				{
					char fragmentIban[5];
					strncpy(fragmentIban, temp->info.codIBAN+0,5);//copiere fragment de string  :tinta+index
					int cod;
					sscanf(fragmentIban, "%d", &cod);
					int codBanca;
					sscanf(banca, "%d", &codBanca);
					if (cod==codBanca)
					{
						if (strcmp(temp->info.modPlata,"electronic")==0)
						{
							listaElectronic = inserareFinal(listaElectronic, temp->info);
						}
						else {
							listaCash = inserareFinal(listaCash, temp->info);
						}
					}
					temp = temp->next;
				}
			}
		}
		scriereListaInFisier(listaCash, g);
		scriereListaInFisier(listaElectronic, g);
		fclose(g);
	}
	else {
		fprintf(g, "Eroare dimensiune tabela ZERO!");
	}
	fclose(g);

}

//ultimul punct stergere nod din lista simpla
//2 cazuri este cap sau  nod intermediar
void stergereNodIndicat(Nod* &cap, const char* IBAN) {
	if (cap)
	{
		if (strcmp(cap->info.codIBAN,IBAN)==0)
		{
			Nod* temp = cap;
			cap = cap->next;
			free(temp->info.denumireClient);
			free(temp->info.modPlata);
			free(temp);
		}
		else {
			Nod* temp = cap;
			while (temp)
			{
				if (temp->next)
				{
					if (strcmp(temp->next->info.codIBAN, IBAN) == 0)
					{
						Nod* temp2 = temp->next;
						temp->next = temp->next->next;
						free(temp2->info.codIBAN);
						free(temp2->info.denumireClient);
						free(temp2->info.modPlata);
						free(temp2);
					}
				}
				else {
					if (strcmp(temp->info.codIBAN, IBAN) == 0)
					{
						Nod* temp2 = temp;
						temp = temp->next;
						free(temp2->info.codIBAN);
						free(temp2->info.denumireClient);
						free(temp2->info.modPlata);
						free(temp2);
					}
				}
				temp = temp->next;
			}
		}
	}
}


int counterNoduriTinta(Nod* &cap, const char* IBAN) {
	if (cap)
	{
		int counter = 0;
		while (cap)
		{
			if (strcmp(cap->info.codIBAN, IBAN) == 0)
			{
				counter++;
			}
			cap = cap->next;
		}
		return counter;
	}
}

void stergeElementeDinHashTable(HashTable &h, const char* IBANCautat) {
	if (h.dimensiune>0)
	{
		int codHash = hashFunction(h.dimensiune, IBANCautat);
		if (codHash>=0 && codHash<h.dimensiune)
		{
			if (h.vector[codHash])
			{
				int counter = counterNoduriTinta(h.vector[codHash], IBANCautat);
				for (int i = 0; i < counter; i++)
				{
					stergereNodIndicat(h.vector[codHash], IBANCautat);
				}
			}
		}
	}
}

void main() {
	ContBancar c0 = initContbancar("11111BCR000000000000000", "Ionel1", 99.92, "cash");
	ContBancar c1 = initContbancar("22222ING000000000000000", "Ionel1", 50001.93, "cash");
	ContBancar c2 = initContbancar("11111BCR000000000000000", "Ionel2", 769.91, "cash");
	ContBancar c3 = initContbancar("22222ING000000000000000", "Ionel3", 2439.93, "cash");
	ContBancar c4 = initContbancar("11111BCR000000000000000", "Ionel4", 50000.92, "electronic");
	ContBancar c5 = initContbancar("12345RRR000000000000000", "Ionel5", 523.92, "cash");
	ContBancar c6 = initContbancar("12346CEC000000000000000", "Ionel6", 50002.91, "cash");
	HashTable h = initHashTable(3);
	inserareInHashTable(h, c0);
	inserareInHashTable(h, c1);
	inserareInHashTable(h, c2);
	inserareInHashTable(h, c3);
	inserareInHashTable(h, c4);
	inserareInHashTable(h, c5);
	inserareInHashTable(h, c6);
	afisareHashTable(h);
	//dezalocareHashTable(h);
	//afisareHashTable(h);
	functieCalculMemorieOcupata(h);
	functieVectorDePlati(h, 50000);
	generareRaportBanca(h, "11111");
	printf("\n Stergere din HT \n");
	stergeElementeDinHashTable(h, "11111BCR000000000000000");
	afisareHashTable(h);
	
}