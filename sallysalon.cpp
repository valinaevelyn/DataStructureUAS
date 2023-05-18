#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node{
	char treat[100];
	char category[100];
	int price;
	char service[100];
	
	int height;
	Node* left;
	Node* right;
};

Node* root = NULL;

Node* createNode(char treat[], char category[], int price, char service[]){
	Node* temp = (Node*)malloc(sizeof(Node));
	strcpy(temp->treat, treat);
	strcpy(temp->category, category);
	temp->price = price;
	strcpy(temp->service, service);
	
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
	
	return max(height(curr->left), height(curr->right));
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

Node* insertNode(Node* curr, char treat[], char category[], int price, char service[]){
	if(!curr){
		return createNode(treat, category, price, service);
	}else if(strcmp(treat, curr->treat) < 0){
		curr->left = insertNode(curr->left, treat, category, price, service);
	}else if(strcmp(treat, curr->treat) > 0){
		curr->right = insertNode(curr->right, treat, category, price, service);
	}
	
	curr->height = updateHeight(curr);
	int BF = getBalanceFactor(curr);
	
	if(BF > 1){
		if(strcmp(treat, curr->left->treat) > 0){
			curr->left = leftRotate(curr->left);
			return rightRotate(curr);
		}
		return rightRotate(curr);
	}else if(BF < -1){
		if(strcmp(treat, curr->right->treat) < 0){
			curr->right = rightRotate(curr->right);
			return leftRotate(curr);
		}
		return leftRotate(curr);
	}
	
	return curr;
}

Node* deleteNode(Node* curr, char treat[]){
	if(!curr){
		return NULL;
	}else if(strcmp(treat, curr->treat) < 0){
		curr->left = deleteNode(curr->left, treat);
	}else if(strcmp(treat, curr->treat) > 0){
		curr->right = deleteNode(curr->right, treat);
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
			
			strcpy(curr->treat, temp->treat);
			strcpy(curr->category, temp->category);
			curr->price = temp->price;
			strcpy(curr->service, temp->service);
			
			curr->left = deleteNode(curr->left, temp->treat);
		}
	}
	
	if(!curr){
		return NULL;
	}
	
	
	curr->height = updateHeight(curr);
	int BF = getBalanceFactor(curr);
	
	if(BF > 1){
		if(getBalanceFactor(curr->left) < 0){
			curr->left = leftRotate(curr->left);
			return rightRotate(curr);
		}
		return rightRotate(curr);
	}else if(BF < -1){
		if(getBalanceFactor(curr->right) > 0){
			curr->right = rightRotate(curr->right);
			return leftRotate(curr);
		}
		return leftRotate(curr);
	}
		
	
	return curr;
}

void postOrder(Node* curr){
	if(!curr){
		return;
	}
	
	postOrder(curr->left);
	postOrder(curr->right);
	printf("%s - %s - %d - %s\n", curr->treat, curr->category, curr->price, curr->service);
}

void inOrder(Node* curr){
	if(!curr){
		return;
	}
	
	inOrder(curr->left);
	printf("%s - %s - %d - %s\n", curr->treat, curr->category, curr->price, curr->service);
	inOrder(curr->right);
}

void preOrder(Node* curr){
	if(!curr){
		return;
	}
	
	printf("%s - %s - %d - %s\n", curr->treat, curr->category, curr->price, curr->service);
	preOrder(curr->left);
	preOrder(curr->right);	
}

void printMenu(){
	printf("Sally Salon\n");
	printf("Menu: \n");
	printf("1. View Available Treatment\n");
	printf("2. Insert New Treatment\n");
	printf("3. Delete Treatment\n");
	printf("4. Exit\n");
	printf(">> ");
}

int validateSally(char treat[]){
	if(treat[0] == 'S' && treat[1] == 'a' && treat[2] == 'l' && treat[3] == 'l' && treat[4] == 'y'){
		return 1;
	}else{
		return 0;
	}
}

int main(){
	int menu;
	do{
		printMenu();
		scanf("%d", &menu); getchar();
		
		switch(menu){
			case 1:{
				char command[5];
				do{
					printf("input view: ");
					scanf("%s", &command); getchar();
				}while(strcmp(command, "pre") != 0 && strcmp(command, "in") != 0 && strcmp(command, "post") != 0);
				
				if(strcmp(command, "pre") == 0){
					preOrder(root);
					printf("\n");
				}else if(strcmp(command, "in") == 0){
					inOrder(root);
					printf("\n");
				}else if(strcmp(command, "post") == 0){
					postOrder(root);
					printf("\n");
				}
				break;
			}
			
			case 2:{
				char treat[20];
				do{
					printf("treatment: ");
					scanf("%[^\n]", &treat); getchar();
				}while(!validateSally(treat) || strlen(treat) < 5 || strlen(treat) > 20);
				
				int price;
				do{
					printf("price: ");
					scanf("%d", &price); getchar();
				}while(price < 50000 || price > 1000000);
				
				char category[200];
				do{
					printf("category: ");
					scanf("%[^\n]", &category); getchar();	
				}while(strcmp(category, "Hair Care") != 0 && strcmp(category, "Nail Care") != 0 && strcmp(category, "Body Care"));
				
				char service[100];
				do{
					printf("service: ");
					scanf("%[^\n]", &service); getchar();
				}while(strcmp(service, "Available") != 0 && strcmp(service, "Unavailable") != 0);
				
				root = insertNode(root, treat, category, price, service);
				printf("\n");
				
				break;
			}
			
			case 3:{
				char deletes[100];
				scanf("%[^\n]", &deletes); getchar();
				
				root = deleteNode(root, deletes);
				break;
			}
			
			case 4:{
				printf("Thank You!\n");
				break;
			}
		}
	}while(menu != 4);
	return 0;
}
