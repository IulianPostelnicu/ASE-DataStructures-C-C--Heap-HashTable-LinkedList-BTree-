#include<iostream>
#include<string.h>
//THIS IS A MAX HEAP C program
//for MIN HEAP just change the sign  here
//if (posLeft < h.dimension && h.vector[max].priority < h.vector[posLeft].priority) //whe check if our first item has the HIGHEST priority or not
//{
//	max = posLeft;
//}
//if (posRight < h.dimension && h.vector[max].priority < h.vector[posRight].priority)
//{
//	max = posRight;
//}

//define first structure that contains our Info
struct Person
{
	char* name;
	int priority; // attr very important for comparation 
};

//create 1 Person
Person initPerson(const char* name, int priority) {
	Person p;
	p.name = (char*)malloc(sizeof(char)*(strlen(name) + 1));
	strcpy(p.name, name);
	p.priority = priority;
	return p;
}

//display info
void showPerson(Person p) {
	printf("Name: %s with priority: %d \n", p.name, p.priority);
}

//define Heap structure
struct Heap
{
	Person* vector;
	int dimension;
};

Heap initHeap(int myWantedDimension) {
	Heap h;
	h.dimension = myWantedDimension;
	h.vector = (Person*)malloc(sizeof(Person)*myWantedDimension);
	return h;
}

void showHeap(Heap h) {
	if (h.dimension>0)
	{
		for (int i = 0; i < h.dimension; i++)
		{
			showPerson(h.vector[i]);
		}
	}
	else {
		printf("Empty HEAP! \n");
	}
}


//after this common steps in Data Structures we have to create a Function that sort our elements from the Heap
//so that the person with the HIGH priority is in the first place;
void filterHeap(Heap& h, int position) {
	if (h.dimension>0)
	{
		int max = position; //we consider this is our first item
		int posLeft = 2 * position + 1; //left children ->similar with the tree
		int posRight = 2 * position + 2;//right children ->similar with the tree
		if (posLeft<h.dimension && h.vector[max].priority<h.vector[posLeft].priority) //whe check if our first item has the HIGHEST priority or not
		{
			max = posLeft;
		}
		if (posRight<h.dimension && h.vector[max].priority<h.vector[posRight].priority)
		{
			max = posRight;
		}
		if (max != position) { //we check if we detect a change
			Person aux = h.vector[position];
			h.vector[position] = h.vector[max];
			h.vector[max] = aux;
			if (2 * max + 1 < h.dimension) { //we recall function to search into the full vector for the highes priority
				filterHeap(h, position);
			}
		}
	}
}

Person extractPerson(Heap &h) {
	if (h.dimension>0)
	{
		Person result = h.vector[0];
		Person *temp = (Person*)malloc(sizeof(Person)*(h.dimension - 1)); //we create a vector with reduced dimension
		for (int i = 1; i < h.dimension; i++)
		{
			temp[i - 1] = h.vector[i];//copy vectors value from the second one
		}
		free(h.vector);
		h.dimension -- ;//make vector with 1 value less
		h.vector = temp;//we have a new vector with -1 items
		for (int i = (h.dimension - 2) / 2;i >= 0;i--) {
			filterHeap(h, i);//we put the new HIGH priority Item on the first position
		}
		return result;
	}
	else {
		throw "Error in h.dimension";
	}
}

void insertInHeap(Heap &h, Person p) {
	if (h.dimension>0)
	{
		Person* aux = (Person*)malloc(sizeof(Person)*(h.dimension + 1));//we increase the Heap din +1 to add 1 mode Person
		for (int i = 0; i < h.dimension; i++)
		{
			aux[i] = h.vector[i];
		}
		aux[h.dimension] = p; //put the neu Person in the last position
		free(h.vector);
		h.dimension++;
		h.vector = aux;
		for (int i=(h.dimension-2)/2;i>=0;i--) //if the new Person has the highest priority we filter to put in on the first position
		{
			filterHeap(h, i);
		}
	}
	else
	{
		throw "Error h.dimension";
	}
}

void main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Heap h = initHeap(10);
	for (int i = 0; i < h.dimension; i++)
	{
		h.vector[i] = initPerson("JohNDoe", i + 1);
	}
	showHeap(h);
	printf("\n\n\n");
	//OUTPUT
	//Name: JohNDoe with priority : 1 <=first element!!
	//Name : JohNDoe with priority : 2
	//Name : JohNDoe with priority : 3
	//Name : JohNDoe with priority : 4
	//Name : JohNDoe with priority : 5
	//Name : JohNDoe with priority : 6
	//Name : JohNDoe with priority : 7
	//Name : JohNDoe with priority : 8
	//Name : JohNDoe with priority : 9
	//Name : JohNDoe with priority : 10
	//***now we have to move Person with priority 10 to first place!
	for (int i=(h.dimension-2)/2;i>=0;i--)
	{
		filterHeap(h, i);
	}
	showHeap(h);
	printf("\n\n\n");
	//OUTPUT
	//Name: JohNDoe with priority : 10 <=first element!!
	//Name : JohNDoe with priority : 1
	//Name : JohNDoe with priority : 7
	//Name : JohNDoe with priority : 9
	//Name : JohNDoe with priority : 2
	//Name : JohNDoe with priority : 6
	//Name : JohNDoe with priority : 3
	//Name : JohNDoe with priority : 8
	//Name : JohNDoe with priority : 4
	//Name : JohNDoe with priority : 5
	Person extracredPerson = extractPerson(h);
	showPerson(extracredPerson);
	printf("\n\n\n");
	//OUTPUT
	//Name: JohNDoe with priority : 10
	insertInHeap(h, initPerson("Hello ", 99));
	insertInHeap(h, initPerson("World", 4));
	showHeap(h);
	//OUTPUT
	//	Name: Hello  with priority : 99 << First Item inserted
	//	Name : JohNDoe with priority : 9
	//	Name : JohNDoe with priority : 8
	//	Name : JohNDoe with priority : 5
	//	Name : JohNDoe with priority : 7
	//	Name : JohNDoe with priority : 3
	//	Name : JohNDoe with priority : 1
	//	Name : JohNDoe with priority : 4
	//	Name : JohNDoe with priority : 2
	//	Name : JohNDoe with priority : 6
	//	Name : World with priority : 4 <<Second one
}
