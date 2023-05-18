#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Node{
	int id;
	char title[100];
	char name[50];
	int year;
	int rating;
	
	Node* left;
	Node* right;
	int height;
};

Node* root = NULL;
int count = 1;

int left = 0;
int right = 0;

Node* createNode(int id, char title[], char name[], int year, int rating){
	Node* temp = (Node*)malloc(sizeof(Node));
	strcpy(temp->title, title);
	strcpy(temp->name, name);
	temp->year = year;
	temp->rating = rating;
	temp->height = 1;
	temp->id = id;
	
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

Node* insertNode(Node* curr, int id, char title[], char name[], int year, int rating){
	if(!curr){
		return createNode(count, title, name, year, rating);
	}else if(id < curr->id){
		curr->left = insertNode(curr->left, id, title, name, year, rating);
	}else if(id > curr->id){
		curr->right = insertNode(curr->right, id, title, name, year, rating);
	}
	
	curr->height = updateHeight(curr);
	int BF = getBalanceFactor(curr);
	
	if(BF > 1){
		if(id > curr->left->id){
			curr->left = leftRotate(curr->left);
			return rightRotate(curr);
		}
		return rightRotate(curr);
	}else if(BF < -1){
		if(id < curr->right->id){
			curr->right = rightRotate(curr->right);
			return leftRotate(curr);
		}
		return leftRotate(curr);
	}
	
	return curr;
}

Node* deleteNode(Node* curr, int id){
	if(!curr){
		return NULL;
	}else if(id < curr->id){
		curr->left = deleteNode(curr->left, id);
	}else if(id > curr->id){
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
			
			strcpy(curr->title, temp->title);
			strcpy(curr->name, temp->name);
			curr->id = temp->id;
			curr->year = temp->year;
			curr->rating = temp->rating;
			curr->left = deleteNode(curr->left, temp->id);
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

void inOrder(Node *curr){
	if(!curr){
		return;
	}
	
	inOrder(curr->left);
	printf("ID: %d\n", curr->id);
	printf("Title: %s\n", curr->title);
	printf("Author: %s\n", curr->name);
	printf("Year: %d\n", curr->year);
	printf("Rating: %d\n", curr->rating);

	printf("\n");

	inOrder(curr->right);
}

Node* findNode(Node* curr, int id){
	if(!curr){
		return NULL;
	}else if(id < curr->id){
		return findNode(curr->left, id);
	}else if(id > curr->id){
		return findNode(curr->right, id);
	}else{
		return curr;
	}
	
	return curr;
}

void checkLeft(Node* curr){
	if(!curr){
		return;
	}
	
	if(curr->left){
		checkLeft(curr->left);
	}
	
	if(curr->right){
		checkLeft(curr->right);
	}
	
	left += 1;
}

void checkRight(Node* curr){
	if(!curr){
		return;
	}
	
	if(curr->left){
		checkRight(curr->left);
	}
	
	if(curr->right){
		checkRight(curr->right);
	}
	
	right += 1;
}

void checkChild(Node* curr){
	if(!curr){
		return;
	}
	
	if(curr->left){
		checkChild(curr->left);
	}
	
	if(curr->right){
		checkChild(curr->right);
	}
	
	left = 0;
	right = 0;
	
	checkLeft(curr->left);
	checkRight(curr->right);
}

void readFile(){
	FILE* f = fopen("library.txt", "r");
	while(!feof(f)){
		char title[100];
		char name[100];
		int year;
		int rating;
		
		fscanf(f, "%[^,], %[^,], %d, %d\n", title, name, &year, &rating); 
		root = insertNode(root, count, title, name, year, rating);
		count++;
	}
	
	fclose(f);
}

int main(){
	readFile();
	
	int n;
	scanf("%d", &n); getchar();
	
	char command[20];
	for(int i = 0; i < n; i++){
		scanf("%s", &command); getchar();
		
		if(strcmp(command, "INSERT") == 0){
			int m;
			scanf("%d", &m); getchar();
			
			char string[200];
			for(int i = 0; i < m; i++){
				scanf("%[^\n]", &string); getchar();
				
				char* token = strtok(string, "#");
				char title[100];
				char name[100];
				int year;
				int rating;
				
				strcpy(title, token);
				token = strtok(NULL, "#");
				strcpy(name, token);
				token = strtok(NULL, "#");
				year = atoi(token);
				token = strtok(NULL, "#");
				rating = atoi(token);
				
				root = insertNode(root, count, title, name, year, rating);
				count++;
			}
		}else if(strcmp(command, "FIND") == 0){
			int find_id;
			scanf("%d", &find_id);
			
			Node* temp = findNode(root, find_id);
			
			if(temp == NULL){
				printf("Not Found\n\n");
			}else{
				printf("ID: %d\n", temp->id);
				printf("Title: %s\n", temp->title);
				printf("Author: %s\n", temp->name);
				printf("Year: %d\n", temp->year);
				printf("Rating: %d\n", temp->rating);	
				printf("\n");
			}
		}else if(strcmp(command, "DELETE") == 0){
			int deletes;
			scanf("%d", &deletes);
			
			root = deleteNode(root, deletes);
		}else if(strcmp(command, "SHOWALL") == 0){
			inOrder(root);
			printf("\n");
		}else if(strcmp(command, "CHECKLEFTROOT") == 0){
			checkChild(root);
			printf("%d", left);
		}else if(strcmp(command, "CHECKRIGHTROOT") == 0){
			checkChild(root);
			printf("%d", right);
		}
	}
	
	inOrder(root);
}
