#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{
    char username[100];
    int job;
    int score;
    int match;
    int win;
    int lose;
    int draw;
    int height;

    Node* left;
    Node* right;
    Node* next;
};

Node* root = NULL;

Node* createNode(char username[], int job, int score, int match, int win, int lose, int draw){
    Node* temp = (Node*)malloc(sizeof(Node));
    strcpy(temp->username, username);
    temp->job = job;
    temp->score = score;
    temp->match = match;
    temp->win = win;
    temp->lose = lose;
    temp->draw = draw;
    temp->height = 1;
    temp->left = temp->right = NULL;
    temp->next = NULL;
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
    if(curr == NULL){
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
    if(curr == NULL){
        return 0;
    }

    return height(curr->left) - height(curr->right);
}

int winRate(int win, int match){
    return win * 100 / match;
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

Node* insertNode(Node* curr, char name[], int job, int score, int match, int win, int lose, int draw){
    if(!curr){
        return createNode(name, job, score, match, win, lose, draw);
    }else if(score < curr->score){
        curr->left = insertNode(curr->left, name, job, score, match, win, lose, draw);
    }else if (score > curr->score){
        curr->right = insertNode(curr->right, name, job, score, match, win, lose, draw);
    }else{
        Node* temp = curr;
        while(temp->next){
            temp = temp->next;
        }

        temp->next = createNode(name, job, score, match, win, lose, draw);
        temp->next->next = NULL;
    }

    curr->height = updateHeight(curr);
    int BF = getBalanceFactor(curr);

    if(BF > 1){
        if(score > curr->left->score){
            curr->left = leftRotate(curr->left);
            return rightRotate(curr);
        }
        return rightRotate(curr);
    }else if(BF < -1){
        if(score < curr->right->score){
            curr->right = rightRotate(curr->right);
            return leftRotate(curr);
        }
        return leftRotate(curr);
    }

    return curr;
}

void deleteLinkedList(Node* curr){
    if(curr){
        deleteLinkedList(curr->next);
        free(curr);
    }
}

Node* deleteNode(Node* curr, int score){
    if(!curr){
        return NULL;
    }else if(score < curr->score){
        curr->left = deleteNode(curr->left, score);
    }else if(score > curr->score){
        curr->right = deleteNode(curr->right, score);
    }else{
        if(curr->next){
           deleteLinkedList(curr->next);
        }
        
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

                strcpy(temp->username, curr->username);
                temp->job = curr->job;
                temp->score = curr->score;
                temp->match = curr->match;
                temp->win = curr->win;
                temp->lose = curr->lose;
                temp->draw = curr->draw;
                curr->left = deleteNode(curr->left, temp->score);
            }
        
    }

    if(curr == NULL){
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
    }else if (BF < -1){
        if(getBalanceFactor(curr->right) > 0){
            curr->right = rightRotate(curr->right);
            return leftRotate(curr);
        }
        return leftRotate(curr);
    }
    return curr;
}

Node* searchNode(Node* curr, int score){
    if(!curr){
        printf("No data found for %d", score);
    }else if(score < curr->score){
        return searchNode(curr->left, score);
    }else if(score > curr->score){
        return searchNode(curr->right, score);
    }else{
        Node* temp = curr;
        printf("%d\n", temp->score);
        while(temp){
            if(temp->score == score){
                printf("%s ", temp->username);
                if(temp->job == 0){
                    printf("[Novice] ");
                }else if(temp->job == 1){
                    printf("[Knight] ");
                }else if(temp->job == 2){
                    printf("[Snipper] ");
                }else if(temp->job == 3){
                    printf("[Wizard] ");
                }else if(temp->job == 4){
                    printf("[Druid] ");
                }else if(temp->job == 5){
                    printf("[Assasin] ");
                }
                printf("(%d %)\n", winRate(temp->win, temp->match));
                temp = temp->next;
            }else{
                temp = temp->next;
            }
        }
    }

    return curr;
}

void scanFile(){
    FILE* f = fopen("dummy.txt", "r");
    while(!feof(f)){
        char name[255];
        int job, score, match, win, lose, draw;
        fscanf(f, "%[^#]#%d#%d#%d#%d#%d#%d\n", name, &job, &score, &match, &win, &lose, &draw);
        root = insertNode(root, name, job, score, match, win, lose, draw);
    }
    fclose(f);
}

void showAll(Node* curr){
    if(!curr){
        return;
    }

    showAll(curr->left);

    Node* iter = curr;
    printf("Score %d: \n", curr->score);
    while(iter){
        printf("%s ", iter->username);
        if(iter->job == 0){
            printf("[Novice]");
        }else if(iter->job == 1){
            printf("[Knight]");
        }else if(iter->job == 2){
            printf("[Snipper]");
        }else if(iter->job == 3){
            printf("[Wizard]");
        }else if(iter->job == 4){
            printf("[Druid]");
        }else if(iter->job == 5){
            printf("[Assasin]");
        }
        printf(" (%d%)\n", winRate(iter->win, iter->match));
        iter = iter->next;
    }
    showAll(curr->right);
}

int main(){
    scanFile();
    int n;
    scanf("%d", &n);

    char command[20];
    for(int i = 0; i < n; i++){
        scanf("%s", &command);
        
        if(strcmp(command, "FIND") == 0){
            int find_score;
            scanf("%d", &find_score); getchar();
            root = searchNode(root, find_score);
            printf("\n");
        }else if(strcmp(command, "INSERT") == 0){
            int m;
            scanf("%d", &m); getchar();
            
            char username[100];
            int job;
            int score;
            int match;
            int win;
            int lose;
            int draw;
            for(int j = 0; j < m; j++){
                printf("Username: ");
                scanf("%s", &username); getchar();
                printf("Job: ");
                scanf("%d", &job); getchar();
                printf("Score: ");
                scanf("%d", &score); getchar();
                printf("Match: ");
                scanf("%d", &match); getchar();
                printf("Win: ");
                scanf("%d", &win); getchar();
                printf("Lose: ");
                scanf("%d", &lose); getchar();
                printf("Draw: ");
                scanf("%d", &draw); getchar();

                printf("\n");

                root = insertNode(root, username, job, score, match, win, lose, draw);
            }
        }else if(strcmp(command, "DELETE") == 0){
            int delete_score;
            scanf("%d", &delete_score); getchar();
            root = deleteNode(root, delete_score);

            printf("Delete %d success\n", delete_score);
            showAll(root);
            printf("\n");
        }else if(strcmp(command, "SHOWALL") == 0){
            showAll(root);
            printf("\n");
        }
    }
    return 0;
}
