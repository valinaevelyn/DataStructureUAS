#include<stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node{
	char title[50];
	char type[50];
	int stock;
	
	Node* left;
	Node* right;
	int height;
};

Node* root = NULL;

Node* createNode(char title[], char type[], int stock){
	Node* temp = (Node*)malloc(sizeof(Node));
	strcpy(temp->title, title);
	strcpy(temp->type, type);
	temp->stock = stock;
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

Node* insertNode(Node* curr, char title[], char type[], int stock){
	if(!curr){
		return createNode(title, type, stock);
	}else if(strcmp(title, curr->title) < 0){
		curr->left = insertNode(curr->left, title, type, stock);
	}else if(strcmp(title, curr->title) > 0){
		curr->right = insertNode(curr->right, title, type, stock);
	}
	
	curr->height = updateHeight(curr);
	int BF = getBalanceFactor(curr);
	
	if(BF > 1){
		if(strcmp(title, curr->left->title) > 0){
			curr->left = leftRotate(curr->left);
			return rightRotate(curr);
		}
		return rightRotate(curr);
	}else if(BF < -1){
		if(strcmp(title, curr->right->title) < 0){
			curr->right = rightRotate(curr->right);
			return leftRotate(curr);
		}
		return leftRotate(curr);
	}
	
	return curr;
}

Node* deleteNode(Node* curr, char title[]){
	if(!curr){
		return NULL;
	}else if(strcmp(title, curr->title) < 0){
		curr->left = deleteNode(curr->left, title);
	}else if(strcmp(title, curr->title) > 0){
		curr->right = deleteNode(curr->right, title);
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
			
			strcpy(curr->title, temp->title);
			strcpy(curr->type, temp->title);
			curr->stock = temp->stock;
			curr->left = deleteNode(curr->left, temp->title);
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

Node* findNode(Node* curr, char title[]){
	if(!curr){
		return NULL;
	}else if(strcmp(title, curr->title) < 0){
		return findNode(curr->left, title);
	}else if(strcmp(title, curr->title) > 0){
		return findNode(curr->right, title);
	}
	
	return curr;
}

void inOrder(Node* curr){
	if(!curr){
		return;
	}
	
	inOrder(curr->left);
	printf("%s - %s - %d\n", curr->title, curr->type, curr->stock);
	inOrder(curr->right);
}

void printMenu(){
	printf("Bluejack GShop\n");
	printf("===============\n");
	printf("1. Insert Game\n");
	printf("2. View Game\n");
	printf("3. Update Stock\n");
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
				char title[100];
				do{
					printf("title: ");
					scanf("%[^\n]", &title); getchar();
				}while(findNode(root, title) != NULL || strlen(title) < 5 || strlen(title) > 25);
				
				char type[50];
				do{
					printf("type: ");
					scanf("%[^\n]", &type); getchar();
				}while(strcmp(type, "Action") != 0 && strcmp(type, "RPG") != 0 && strcmp(type, "Adventure") != 0 && strcmp(type, "Card Game") != 0);
				
				int stock;
				do{
					printf("stock: ");
					scanf("%d", &stock); getchar();
				}while(stock < 0);
				
				root = insertNode(root, title, type, stock);
				
				break;
			}
			
			case 2:{
				inOrder(root);
				break;
			}
			
			case 3:{
				char titles[100];
				
				printf("input title: ");
				scanf("%[^\n]", &titles);
				
				Node* temp = findNode(root, titles);
				
				if(temp == NULL){
					printf("Not Found\n");
					break;
				}else{
					printf("stock: %d\n\n", temp->stock);
					
					char command[10];
					do{
						printf("update type: ");
						scanf("%s", &command); getchar();
					}while(strcmp(command, "add") != 0 && strcmp(command, "remove") != 0);
					
					if(strcmp(command, "add") == 0){
						int add;
						
						printf("update stock to add: ");
						scanf("%d", &add); getchar();
						
						temp->stock += add;
						
					}else if(strcmp(command, "remove") == 0){
						int remove;
						do{
							printf("update stock to remove: ");
							scanf("%d", &remove); getchar();	
						}while(remove > temp->stock);
						
						if(remove == temp->stock){
							root = deleteNode(root, titles);
						}else{
							temp->stock -= remove;
						}
					}
				}
				
				
				
				break;
			}
		}
		
	}while(menu != 4);
	return 0;
}
