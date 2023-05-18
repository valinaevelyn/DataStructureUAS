#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{
	int value;
	char color[10];
	int height;
	
	Node* left;
	Node* right;
};

Node* root = NULL;
int white = 0;
int yellow = 0;
int pink = 0;
int left = 0;
int right = 0;

Node* createNode(int value){
	Node* temp = (Node*)malloc(sizeof(Node));
	temp->value = value;
	temp->height = 1;
	strcpy(temp->color, "white");
	
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

Node* insertNode(Node* curr, int value){
	if(!curr){
		return createNode(value);
	}else if(value < curr->value){
		curr->left = insertNode(curr->left, value);
	}else if(value > curr->value){
		curr->right = insertNode(curr->right, value);
	}
	
	curr->height = updateHeight(curr);
	int BF = getBalanceFactor(curr);
	
	if(BF > 1){
		if(value > curr->left->value){
			curr->left = leftRotate(curr->left);
			return rightRotate(curr);
		}
		return rightRotate(curr);
	}else if(BF < -1){
		if(value < curr->right->value){
			curr->right = rightRotate(curr->right);
			return leftRotate(curr);
		}
		return leftRotate(curr);
	}
		
	return curr;
}

Node* deleteNode(Node* curr, int value){
	if(!curr){
		return NULL;	
	}else if(value < curr->value){
		curr->left = deleteNode(curr->left, value);
	}else if(value > curr->value){
		curr->right = deleteNode(curr->right, value);
	}else{
		if(!curr->left && !curr->right){
			free(curr);
			return NULL;
		}else if(!curr->left){
			Node* temp = curr;
			curr = curr->right;
			free(temp);
		}else if(!curr->right){
			Node* temp  =curr;
			curr = curr->left;
			free(temp);
		}else{
			Node* temp = curr->left;
			if(temp->right){
				temp = temp->right;
			}
			
			strcpy(curr->color, temp->color);
			curr->value = temp->value;
			curr->left = deleteNode(curr->left, temp->value);
		}
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

void checkValueLeft(Node* curr){
	if(!curr){
		return;
	}
	
	if(curr->left){
		checkValueLeft(curr->left);
	}
	
	if(curr->right){
		checkValueLeft(curr->right);
	}
	
	left += curr->value;
}

void checkValueRight(Node *curr){
	if(!curr){
		return;
	}
	
	if(curr->left){
		checkValueRight(curr->left);
	}
	
	if(curr->right){
		checkValueRight(curr->right);
	}
	
	right += curr->value;
}

void recolor(Node* curr){
	if(!curr){
		return;
	}
	
	if(curr->left){
		recolor(curr->left);
	}
	
	if(curr->right){
		recolor(curr->right);
	}
	
	left = 0;
	right = 0;
	
	checkValueLeft(curr->left);
	checkValueRight(curr->right);
	
	if(right > left){
		strcpy(curr->color, "pink");
	}else if(right < left){
		strcpy(curr->color, "yellow");
	}else if(right == left){
		strcpy(curr->color, "white");
	}
}

void describe(Node* curr){
	if(!curr){
		return;
	}

	if(strcmp(curr->color, "white") == 0){
		white++;
	}else if(strcmp(curr->color, "pink") == 0){
		pink++;
	}else if(strcmp(curr->color, "yellow") == 0){
		yellow++;
	}
	
	if(curr->left){
		describe(curr->left);
	}
	
	if(curr->right){
		describe(curr->right);
	}
}

void find(Node* curr, int value){
	if(!curr){
		return;
	}else if(value < curr->value){
		return find(curr, value);
	}else if(value > curr->value){
		return find(curr, value);
	}else{
		white = 0;
		yellow = 0;
		pink = 0;
		
		describe(curr->left);
		describe(curr->right);
		
		printf("value: %d\n", value);
		printf("keyword: %s\n", curr->color);
		printf("pink child: %d\n", pink);
		printf("white child: %d\n", white);
		printf("yellow child: %d\n", yellow);
	}
}

void inOrder(Node* curr){
	if(!curr){
		return;
	}
	
	inOrder(curr->left);
	printf("%d %s\n", curr->value, curr->color);
	inOrder(curr->right);
}

int main(){
	int n;
	scanf("%d", &n); getchar();
	
	char command[20];
	for(int i = 0; i < n; i++){
		scanf("%s", &command); getchar();
		
		
		if(strcmp(command, "INSERT") == 0){
			int insert;
			scanf("%d", &insert); getchar();
			root = insertNode(root, insert);
			recolor(root);
		}else if(strcmp(command, "VIEW") == 0){
			inOrder(root);
		}else if(strcmp(command, "DESCRIBE") == 0){
			int value_find;
			scanf("%d", &value_find); getchar();
			find(root, value_find);
		}else if(strcmp(command, "DELETE") == 0){
			int value_delete;
			scanf("%d", &value_delete); getchar();
			root = deleteNode(root, value_delete);
			recolor(root);
		}
		
	}
}
