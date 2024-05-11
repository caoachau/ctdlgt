#include <bits/stdc++.h>		
using namespace std;
 
struct node
{
char  mssv[100];
char  hovaten[100];
int  tuoi;
struct node* next;

};

typedef struct node node;

node* makenode(char mssv[]; char hovaten[]; int tuoi ) //tao node moi
{
	node *newnode= (node*)malloc(sizeof(node));
	strcpy(newnode->mssv,mssv);
	strcpy(newnode->hovaten,hovaten);
	newnode->age=age;
	newnode->next=NULL;
	return newnode;
}

void themsv( node** head, char mssv[], char hovaten[], int age)
{
	node* newnode=makenode(mssv,name,tuoi);
	if(*head == NULL)
	{
		*head=newnode;
		return;
	}
}

int main()
{
	
}

