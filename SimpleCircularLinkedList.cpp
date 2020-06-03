#include<iostream>
#include<string.h>

//circular list has the last node that points to the first node
//in case of simple list, last node->NULL
//so we have some problems in working with circular list and we have to put some conditions to avoid infinite loop..in reading the list for example...

struct Dog
{
	char* name;
	int age;
	bool isAggressive;
};

//we create a Dog
Dog initDog(const char* name, int age, bool isAggressive) {
	Dog d;
	d.name = (char*)malloc(sizeof(char)*(strlen(name) + 1));
	strcpy(d.name, name);
	d.age = age;
	d.isAggressive = isAggressive;
	return d;
}

void showDog(Dog d) {
	printf("The dog with the name %s has %d years and ", d.name, d.age);
	if (d.isAggressive==true)
	{
		printf(" is very dangerous >.<\n");
	}
	else {
		printf(" is very frendly O_O\n");
	}
}

struct Node
{
	Dog info;
	Node* next;
};

Node* initNode(Dog info, Node* next) { //we can use this function to insert at the beginning 
	Node* n = (Node*)malloc(sizeof(Node));
	n->info = info;
	n->next = next;
	return n;
}

//depends if we need a Stack or a queue
//stack inserts at the beginning
//queue inserts at the end
//we will make a queue because this one needs a new function of insertion
Node* insertInList(Node* &head, Dog info) {
	Node* newNode = initNode(info, NULL);
	if (head)
	{
		Node* temp = head;
		while (temp->next!=head)
		{
			temp = temp->next;
		}
		temp->next = newNode;
		newNode->next = head;
	}
	else
	{
		head = newNode;
		head->next = head;
	}
	return head;
}

void showList(Node* head) {
	if (head)
	{
		Node* temp = head;
		while (temp->next!=head)//we stop on the last NODE
		{
			showDog(temp->info);
			temp = temp->next;
		}
		showDog(temp->info);//we show info from the last node to complete the list
	}
	else 
	{
		printf("List is empty \n");
	}
}

Node* deleteList(Node* &head) {
	if (head)
	{
		Node* temp = head;
		while (temp->next != head) {
			Node* aux = temp;
			temp = temp->next;
			free(aux->info.name); //delete the name
			free(aux);//delete the node
		}
		free(temp->info.name);
		free(temp);
 		return head = NULL; //set head to NULL 
	}
	else {
		return NULL;
	}
}

//show only the aggressive dogs with sum of age
//I know that this function does 2 things in the same thime and this is not Clean Code but.. we can try it
int calculateAggressiveDogs(Node* head) {
	if (head)
	{
		int sumAge = 0;
		Node* temp = head;
		while (temp->next != head)//we stop on the last NODE
		{
			if (temp->info.isAggressive==1)
			{
				showDog(temp->info);
				sumAge += temp->info.age;
			}
			temp = temp->next;
		}
		if (temp->info.isAggressive == 1)//we check the last node to see the condition
		{
			showDog(temp->info);
			sumAge += temp->info.age;
		}
		temp = temp->next;
		return sumAge;
	}
	else
	{
		printf("List is empty \n");
	}
}

//here we delete node inside the list
//we have to remake the links between the previous and the next node after the extraction
Dog extractDog(Node* &head, const char* searchName) {
	if (head)
	{
		Node* temp = head;
		while (temp->next != head)
		{
			if (strcmp(temp->next->info.name, searchName) == 0) {
				Node* aux = temp->next;
				Dog result = aux->info;
				temp->next = temp->next->next;
				free(aux);
				return result;
			}
			temp = temp->next;
		}
		if (strcmp(temp->next->info.name, searchName) == 0) {
			Node* aux = head;
			Dog result = aux->info;
			head = head->next;
			temp->next = head;
			free(aux);
			return result;
		}
		else
		{
			return initDog("No dog found", -1, false);
		}

	}
	else 
	{
		throw "Error ";
	}
}

void main() {
	Node* head = NULL;
	head = insertInList(head, initDog("Polish Greyhound", 10, true));
	head = insertInList(head, initDog("Mudhol Hound", 12, false));
	head = insertInList(head, initDog("Miniature Schnauzer", 15, true));
	head = insertInList(head, initDog("Pachon navarro", 7, true));
	head = insertInList(head, initDog("Ratonero Bodeguero Andaluz", 20, true));
 	showList(head);
	//OUTPUT
	//	The dog with the name Polish Greyhound has 10 years and  is very dangerous > .<
	//	The dog with the name Mudhol Hound has 12 years and  is very frendly O_O
	//	The dog with the name Miniature Schnauzer has 15 years and  is very dangerous >.<
	//	The dog with the name Pach≤n navarro has 7 years and  is very dangerous >.<
	//	The dog with the name Ratonero Bodeguero Andaluz has 20 years and  is very dangerous >. <
	printf("\n\n");
	int sumAge = calculateAggressiveDogs(head);
	printf("They have %d years in total\n", sumAge);
	printf("\n\n");
	Dog myDOG=extractDog(head, "Mudhol Hound");
	Dog myDOG1 = extractDog(head, "Mudhol");
	Dog myDOG2 = extractDog(head, "Polish Greyhound");
	Dog myDOG3 = extractDog(head, "Ratonero Bodeguero Andaluz");
	showList(head);

	//OUTPUT
	//The dog with the name Polish Greyhound has 10 years and  is very dangerous > .<
	//	The dog with the name Mudhol Hound has 12 years and  is very frendly O_O
	//	The dog with the name Miniature Schnauzer has 15 years and  is very dangerous >.<
	//	The dog with the name Pachon navarro has 7 years and  is very dangerous >.<
	//	The dog with the name Ratonero Bodeguero Andaluz has 20 years and  is very dangerous >.<


	//	The dog with the name Polish Greyhound has 10 years and  is very dangerous >.<
	//	The dog with the name Miniature Schnauzer has 15 years and  is very dangerous >.<
	//	The dog with the name Pachon navarro has 7 years and  is very dangerous >.<
	//	The dog with the name Ratonero Bodeguero Andaluz has 20 years and  is very dangerous >.<
	//	They have 52 years in total

	//after 4 extrection (3 with success 1 not)
	//this is our list
	//	The dog with the name Miniature Schnauzer has 15 years and  is very dangerous >.<
	//	The dog with the name Pachon navarro has 7 years and  is very dangerous >.<
	
	printf("\n\n Delete list\n");
	head = deleteList(head);
	showList(head);
	//OUTPUT
	/*Delete list
		List is empty*/
	
}

