#include<iostream>
#include<string.h>
//HashTable is a structure that contains one vector of Items
//the Items are in fact Nodes that forms LinkedLists
//in this example we will use Simple Linked List

struct Car
{
	char* model;
	float price;
	int year;
};

Car initCar(const char* model, float price, int year) {
	Car c;
	c.model = (char*)malloc(sizeof(char)*(strlen(model) + 1));
	strcpy(c.model, model);
	c.price = price;
	c.year = year;
	return c;
}

void showCar(Car c) {
	printf("The car model: %s, costs %5.2f and was produced in %d \n", c.model, c.price, c.year);
}

struct Node //this is simple node, you can use double node if you want, just add Node* prev;
{
	Car info;
	Node* next; 
};

Node* initNode(Car info, Node* next) {
	Node* n = (Node*)malloc(sizeof(Node));
	n->info = info; //shallow copy
	n->next = next;
	return n;
}

void showList(Node* head) {
	if (head)
	{
		while (head) {
			showCar(head->info);
			head = head->next;
		}
	}
	else
	{
		printf("List is empty\n");
	}
}

//because we don t want to change the hashTable structure we have to insert at the end of the simple list

Node* insertAtTheEnd(Node* head, Car c) {
	Node* newNode = initNode(c, NULL);
	if (head!=NULL)
	{
		Node* temp = head; //we have to take a temporary Node to move into the LinkedList
		while (temp->next) { //we stop at the end of the list on the last node
			temp = temp->next;
		}
		temp->next = newNode;
	}
	else
	{
		head = newNode;
	}
	return head;//we dont modify the head !
}

struct HashTable
{
	Node** vector; //if you saw the Heap example from this repository the Heap Structure is the same, just that it has only simple vector not **
	int dimension;
};

HashTable initHashTable(int dimensionWanted) {
	HashTable h;
	h.dimension = dimensionWanted;
	h.vector = (Node**)malloc(sizeof(Node*)*(dimensionWanted));
	for (int i = 0; i < dimensionWanted; i++)
	{
		h.vector[i] = NULL;
	}
	return h;
}

//HashTable has HashCode that is similar with an index for fast search
int calculateHashCode(int hashTableDimension, const char* model) {
	int code = strlen(model);
	code *= 7;
	return code % hashTableDimension;
}


void insertInHashTable(HashTable h, Car newCar) {
	if (h.dimension>0)
	{
		int hashCode = calculateHashCode(h.dimension, newCar.model);
		if (hashCode < h.dimension &&hashCode >= 0) {
			if (h.vector[hashCode] != NULL) {
				insertAtTheEnd(h.vector[hashCode],newCar);
			}
			else {
				h.vector[hashCode] = (Node*)malloc(sizeof(Node));
				h.vector[hashCode]->info = newCar;
				h.vector[hashCode]->next = NULL;
			}
		}
		else
		{
			throw "Error bad hashCode";
		}
	}
	else
	{
		throw "Error h.dimension";
	}
}

void showHashTable(HashTable h) {
	if (h.dimension>0)
	{
		for (int i = 0;i < h.dimension;i++) {
			printf("HastTable levle: %d\n", i);
			if (h.vector[i]!=NULL)
			{
				showList(h.vector[i]);
			}
			printf("\n");
		}
	}
	else
	{
		printf("HashTable h.dimensio=0 \n");
	}
}

Car searchForCarInHashTable(HashTable h, const char* seachModel) {
	if (h.dimension>0)
	{
		int hashCode = calculateHashCode(h.dimension, seachModel);
		if (hashCode < h.dimension &&hashCode >= 0) {
			Node* aux = h.vector[hashCode];
			if (aux!=NULL)
			{
				while (aux) {
					if (strcmp(aux->info.model,seachModel)==0)
					{
						Car result= initCar(aux->info.model, aux->info.price, aux->info.year);
						return result;
					}
					aux = aux->next;

				}
				throw "Errror in the search step";
			}
			else
			{
				throw "Errror in the search step";
			}
		}
		else
		{
			throw "hashCodeFunction error";
		}
	}
	else
	{
		throw "HashTable h.dimensio=0";
	}
}

void deteleHashTable(HashTable &ht) {
	if (ht.dimension>0)
	{
		for (int i = 0; i < ht.dimension;i++) {
			if (ht.vector[i])
			{
				Node* temp = ht.vector[i];
				ht.vector[i] = ht.vector[i]->next;
				free(temp->info.model);
				free(temp);
			}
		}
		free(ht.vector);
		ht.vector = NULL;
		ht.dimension = 0;
	}
}

void main() {
	HashTable h = initHashTable(5);
	insertInHashTable(h, initCar("BMW X1", 15.87, 2000));
	insertInHashTable(h, initCar("BMW 320", 29.76, 2020));
	insertInHashTable(h, initCar("BMW M3", 49.345, 2010));
	insertInHashTable(h, initCar("FORD MUSTANG", 19.76, 2012));
	insertInHashTable(h, initCar("AUDI TT1.6", 39.56, 2019));
	showHashTable(h);
	//OUTPUT 
	//HastTable levle : 0
	//	The car model : AUDI TT1.6, costs 39.56 and was produced in 2019

	//	HastTable levle : 1

	//	HastTable levle : 2
	//	The car model : BMW X1, costs 15.87 and was produced in 2000
	//	The car model : BMW M3, costs 49.35 and was produced in 2010

	//	HastTable levle : 3

	//	HastTable levle : 4
	//	The car model : BMW 320, costs 29.76 and was produced in 2020
	//	The car model : FORD MUSTANG, costs 19.76 and was produced in 2012
	printf("\n\n***Your car here: ");
	Car searchModel = searchForCarInHashTable(h, "BMW X1");
	showCar(searchModel);
	//OUTPUT
	//The car model : BMW X1, costs 15.87 and was produced in 2000
	deteleHashTable(h);
	showHashTable(h);
}