#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Node{
	char id[10];
	int n;
	int length;
	char type[50];
	
	int height;
	Node* left;
	Node* right;
};

Node* root = NULL;
int count = 1;

Node* createNode(char id[], int n, int length, char type[]){
	Node* temp = (Node*)malloc(sizeof(Node));
	temp->n = n;
	strcpy(temp->id, id);
	temp->length = length;
	strcpy(temp->type, type);

	temp->height = 1;
	temp->left = temp->right = NULL;
	
	return temp;
}

int max(int a, int b){
	if(a > b){
		return a;
	}else{
		return b;
	}
}

int height(Node* curr){
	if(!curr){
		return 0;
	}
	
	return curr->height;
}

int updateHeight(Node* curr){
	if(!curr){
		return 0;
	}
	
	return max(height(curr->left), height(curr->right)) + 1;
}

int getBalanceFactor(Node* curr){
	if(!curr){
		return 0;
	}
	
	return height(curr->left) - height(curr->right);
}

Node* rightRotate(Node* t){
	Node* s = t->left;
	Node* b = s->right;
	
	s->right = t;
	t->left = b;
	
	t->height = updateHeight(t);
	s->height = updateHeight(s);
	
	return s;
}

Node* leftRotate(Node* t){
	Node* s = t->right;
	Node* b = s->left;
	
	s->left = t;
	t->right = b;
	
	t->height = updateHeight(t);
	s->height = updateHeight(s);
	
	return s;
}

int area4(int s){
	return s*s;
}

double area5(int s, int n){
	return (n * pow(s, 2)) / (4 * tan(3.14/n));
}

Node* insertNode(Node* curr, char id[], int n, int length, char type[]){
	if(!curr){
		return createNode(id, n, length, type);
	}else if(strcmp(id, curr->id) < 0){
		curr->left = insertNode(curr->left, id, n, length, type);
	}else if(strcmp(id, curr->id) > 0){
		curr->right = insertNode(curr->right, id, n, length, type);
	}
	
	curr->height = updateHeight(curr);
	int BF = getBalanceFactor(curr);
	
	if(BF > 1){
		if(strcmp(id, curr->left->id) > 0){
			curr->left = leftRotate(curr->left);
			return rightRotate(curr);
		}
		return rightRotate(curr);
	}else if(BF < -1){
		if(strcmp(id, curr->right->id) < 0){
			curr->right = rightRotate(curr->right);
			return leftRotate(curr);
		}
		return leftRotate(curr);
	}
	
	return curr;
}

Node* deleteNode(Node* curr, char id[]){
	if(!curr){
		return NULL;
	}else if(strcmp(id, curr->id) < 0){
		curr->left = deleteNode(curr->left, id);
	}else if(strcmp(id, curr->id) > 0){
		curr->right = deleteNode(curr->right, id);
	}else{
		if(!curr->left && !curr->right){
			free(curr);
			return NULL;
		}else if(!curr->left){
			Node* temp = curr;
			curr = curr->right;
			free(temp);
		}else if(!curr->right){
			Node* temp = curr;
			curr = curr->left;
			free(temp);
		}else{
			Node* temp = curr->left;
			while(temp->right){
				temp = temp->right;
			}
			
			strcpy(curr->type, temp->type);
			curr->n = temp->n;
			curr->length = temp->length;
			strcpy(curr->id, temp->id);
			curr->left = deleteNode(curr->left, temp->id);
		}
	}
	
	curr->height = updateHeight(curr);
	int BF = getBalanceFactor(curr);
	
	if(BF > 1){
		if(getBalanceFactor(curr->left) > 0){
			curr->left = leftRotate(curr->left);
			return rightRotate(curr);
		}
		return rightRotate(curr);
	}else if(BF < -1){
		if(getBalanceFactor(curr->right) < 0){
			curr->right = rightRotate(curr->right);
			return leftRotate(curr);
		}
		return leftRotate(curr);
	}
	
	return curr;
}

void preOrder(Node* curr){
	if(!curr){
		return;
	}
	
	int price = 0;
	int total = 0;
	
	if(strcmp(curr->type, "rural") == 0){
		price = 2000;
	}else if(strcmp(curr->type, "metro") == 0){
		price = 5500;
	}else if(strcmp(curr->type, "capital") == 0){
		price = 10000;
	}
	
	if(curr->n == 4){
		total = price * area4(curr->length);
	}else if(curr->n > 4 && curr->n < 11){
		total = price* area5(curr->length, curr->n);
	}
	
	printf("%s - %d - %d - %s - %d\n", curr->id, curr->n, curr->length, curr->type, total);
	preOrder(curr->left);
	preOrder(curr->right);
}

void inOrder(Node* curr){
	if(!curr){
		return;
	}
	
	inOrder(curr->left);
	int price = 0;
	int total = 0;
	
	if(strcmp(curr->type, "rural") == 0){
		price = 2000;
	}else if(strcmp(curr->type, "metro") == 0){
		price = 5500;
	}else if(strcmp(curr->type, "capital") == 0){
		price = 10000;
	}
	
	if(curr->n == 4){
		total = price * area4(curr->length);
	}else if(curr->n > 4 && curr->n < 11){
		total = price* area5(curr->length, curr->n);
	}
	
	printf("%s - %d - %d - %s - %d\n", curr->id, curr->n, curr->length, curr->type, total);
	inOrder(curr->right);
}

void printMenu(){
	printf("1. Insert Property\n");
	printf("2. Change View\n");
	printf("3. Delete Property\n");
	printf("4. Exit\n");
	printf(">> ");
}

int main(){
	int menu;
	do{
		printMenu();
		scanf("%d", &menu); getchar();
		
		switch(menu){
			case 1:{
				char type[50];
				do{
					printf("Type: ");
					scanf("%s", &type); getchar();
				}while(strcmp(type, "capital") != 0 && strcmp(type, "metro") != 0 && strcmp(type, "rural") != 0);
				
				int n;
				do{
					printf("Sides: ");
					scanf("%d", &n); getchar();
				}while(n < 4 || n > 10);
				
				int length;
				do{
					printf("Length: ");
					scanf("%d", &length); getchar();
				}while(length < 1 || length > 1000);
				
				char id[5];
				sprintf(id, "TX%03d", count);
				
				root = insertNode(root, id, n, length, type);
				count++;
				
				break;
			}
			
			case 2:{
				char input[5];
				do{
					printf("View: ");
					scanf("%s", & input);
				}while(strcmp(input, "pre") != 0 && strcmp(input, "in") != 0);
				
				if(strcmp(input, "pre") == 0){
					preOrder(root);
				}else if(strcmp(input, "in") == 0){
					inOrder(root);
				}
				break;
			}
			
			case 3:{
				char id_delete[10];
				scanf("%s", &id_delete); getchar();
				
				root = deleteNode(root, id_delete);
				
				break;
			}
		}
	}while(menu != 4);
	return 0;
}
