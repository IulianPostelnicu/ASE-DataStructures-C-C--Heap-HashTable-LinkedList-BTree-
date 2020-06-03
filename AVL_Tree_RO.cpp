#include<stdio.h>
#include<string>


struct Tara {
	int id;
	char* nume;
	int nrLoc;
};

struct Nod {
	Tara info;
	Nod*st;
	Nod* dr;
};

Tara citesteDinFisier(FILE * fis) {
	Tara t;
	fscanf(fis, "%d", &t.id);
	fscanf(fis, "%d", &t.nrLoc);
	char aux[20];
	fscanf(fis, "%s", aux);
	t.nume = (char*)malloc(sizeof(char)*(strlen(aux) + 1));
	strcpy(t.nume, aux);
	return t;
}

int maxim(int a, int b) {
	return ((a > b) ? a : b);
}

int inaltimeArbore(Nod* rad) {
	if (rad) {
		int Hst = inaltimeArbore(rad->st);
		int Hdr = inaltimeArbore(rad->dr);
		return maxim(Hst, Hdr) + 1;
	}
	else {
		return 0;
	}
}

int calculGradEchilibru(Nod* rad) {
	if (rad) {
		int Hst = inaltimeArbore(rad->st);
		int Hdr = inaltimeArbore(rad->dr);

		return Hst - Hdr;
	}
	else {
		return 0;
	}
}

//Nod* inserareTaraInArbore(Nod* rad, Tara t) {
//	if (rad) {
//		if (rad->info.id < t.id) {
//			rad->dr = inserareTaraInArbore(rad->dr, t);
//		}
//		else if (rad->info.id > t.id) {
//			rad->st = inserareTaraInArbore(rad->st, t);
//		}
//		else {
//			throw "Tara cu acest id exista deja";
//		}
//		return rad;
//	}
//	else {
//		Nod* nou = (Nod*)malloc(sizeof(Nod));
//		nou->info = t;
//		nou->dr = NULL;
//		nou->st = NULL;
//		return nou;
//	}
//}


Nod* rotireStanga(Nod* rad) {
	if (rad) {
		Nod*temp = rad->dr;
		rad->dr = temp->st;
		temp->st = rad;
		return temp;
	}
	else {
		return rad;
	}
}

Nod* rotireDreapta(Nod* rad) {
	if (rad) {
		Nod* temp = rad->st;
		rad->st = temp->dr;
		temp->dr = rad;
		return temp;
	}
	else {
		return NULL;
	}
}


Nod* inserareTaraInArboreAVL(Nod* rad, Tara t) {
	if (rad) {
		if (rad->info.id < t.id) {
			rad->dr = inserareTaraInArboreAVL(rad->dr, t);
		}
		else if (rad->info.id > t.id) {
			rad->st = inserareTaraInArboreAVL(rad->st, t);
		}
		else {
			throw "Tara cu acest id exista deja";
		}
		int GE = calculGradEchilibru(rad);
		if (GE == -2) {
			//dezechilibru in dreapta
			if (calculGradEchilibru(rad->dr) == -1) {
				rad = rotireStanga(rad);
			}
			else {
				rad->dr = rotireDreapta(rad->dr);
				rad = rotireStanga(rad);
			}
		}
		else {
			if (GE == 2) {
				//dezchilibru in stanga
				if (calculGradEchilibru(rad->st) == -1) {
					rad->st = rotireStanga(rad->st);
				}
				rad = rotireDreapta(rad);
			}
		}

		return rad;
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = t;
		nou->dr = NULL;
		nou->st = NULL;
		return nou;
	}
}

void afisareTara(Tara t) {
	printf("Tara %s cu id-ul %d are %d locuitori \n", t.nume, t.id, t.nrLoc);
}

void afisareInordine(Nod* rad) {
	if (rad) {
		afisareInordine(rad->st);
		afisareTara(rad->info);
		afisareInordine(rad->dr);
	}
}

Tara cautareTaraDupaId(Nod* rad, int id) {
	if (rad) {
		if (rad->info.id == id) { 
		}
		else if (rad->info.id > id) {
			return cautareTaraDupaId(rad->st, id);
		}
		else {
			return cautareTaraDupaId(rad->dr, id);
		}
	}
	else {
		throw "Nu exista tara cu id-ul cautat";
	}
}


void stergereArbore(Nod* & rad) {
	if (rad) {
		stergereArbore(rad->st);
		stergereArbore(rad->dr);
		free(rad->info.nume);
		free(rad);
		rad = NULL;
	}
}


int counterTariFiltrate = 0;
int filtruTari(Nod* rad, int filtruPopulatie) {
	if(rad) {
		if (rad->info.nrLoc > filtruPopulatie) {
			counterTariFiltrate++;
		}
			filtruTari(rad->st, filtruPopulatie);
			filtruTari(rad->dr, filtruPopulatie);
	}
	return counterTariFiltrate;
}

Nod* valMinimaPerLevel(Nod* rad)
{
	Nod* temp = rad;
	while (temp->st) {
		temp = temp->st;
	}
	return temp;
}

Nod* stergereNodDupaID(Nod* rad, int idCautat)
{
	if(rad) {
		if (rad->info.id > idCautat) { //identific nodul pe care sa il sterg, caut pe fiecare ramura stanga-dreapta
			rad->st = stergereNodDupaID(rad->st, idCautat);
		}
		else if (rad->info.id < idCautat) {
			rad->dr = stergereNodDupaID(rad->dr, idCautat);
		}
		else
		{ //am identificat nodul de sters
			printf("Nodul cautat cu ID %d a fost identificat: \n", rad->info.id);
			afisareTara(rad->info);
			printf("* * * \n");
			if (rad->st && rad->dr==NULL) //daca are un copil
			{
				Nod* temp = rad->st; 
					*rad = *temp; 
					free(temp);
			}
			else if (rad->dr && rad->st==NULL)
			{
				Nod *temp = rad->dr;
				*rad = *temp; 
				free(temp);
			}
			else if(rad->dr && rad->st) //daca are 2 copii
			{
				Nod* temp = valMinimaPerLevel(rad->dr);
				rad->info.id = temp->info.id;
				rad->dr = stergereNodDupaID(rad->dr, temp->info.id);
			}
			else //daca este nod frunza
			{
				Nod* temp = rad;
				free(temp->info.nume);
				free(temp);
				rad = NULL;
			}

		}
		return rad;
	}
	else
	{
		return NULL;
	}
}

int countN = 0;
int counterNoduri(Nod* rad) {
	
	if (rad)
	{
		countN++;
		counterNoduri(rad->st);
		counterNoduri(rad->dr);
	}
	return countN;
}
//final tema 2

void main() {
	Nod *rad = NULL;
	FILE* f = fopen("TariEuropene.txt", "r");
	while (!feof(f)) {
		Tara t = citesteDinFisier(f);
		rad = inserareTaraInArboreAVL(rad, t);
	}
	fclose(f);

	afisareInordine(rad);

	printf("\n\n");
	printf("* * * Tema 2 Postelnicu Iulian ID1150 * * * \n");
	printf("\n\n");
	afisareTara(cautareTaraDupaId(rad, 2));
	printf("\n\n");
	int popCautat = 5000;
	printf("Tari cu populatia mai mare ca %d sunt %d \n", popCautat,filtruTari(rad,popCautat));
	printf("\n\n");
	stergereNodDupaID(rad,1); //stergere nod frunza
	stergereNodDupaID(rad, 5); //stergere nod intermediar
	stergereNodDupaID(rad, 10);//stergere nod frunza

	afisareInordine(rad);

	printf("\nAu ramas %d noduri in arbore! \n", counterNoduri(rad));

	//stergereArbore(rad);

}