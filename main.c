#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_COM_LEN 30
#define MAX_LINE 1000000000

//struttura rb-tree
struct node{
    unsigned int value;
    unsigned int index;
    char color;
    struct node* parent;
    struct node* right;
    struct node* left;
};

struct rb_tree{
    struct node *root;
    struct node *nil;
};

//Albero
struct rb_tree *t;
//Puntatori ausiliari
struct node *max;
struct node *pre;

//Global var
char command[MAX_COM_LEN+1];
char line[MAX_LINE+1];
char *a;
int b;

unsigned int d; //numero nodi grafo
unsigned int k; //i migliori k grafi
unsigned int graph_index;

//funzioni del rb-tree tradotti dallo pseudocodice delle slides
struct node* new_node(unsigned int input, unsigned int index){
    struct node* new;
    new=(struct node*)malloc(sizeof(struct node));
    new->value = input;
    new->index = index;
    new->color = 'r';
    new->parent = t->nil;
    new->right = t->nil;
    new->left = t->nil;
    return new;
}

struct node* tree_min(struct node* tmp){
    if(tmp==t->nil)
        return tmp;
    while(tmp->left != t->nil){
        tmp = tmp->left;
    }
    return tmp;
}

struct node* tree_max(struct node* tmp){
    while (tmp->right != t->nil){
        tmp = tmp->right;
    }
    return tmp;
}

struct node* tree_successor(struct node* x){
    if(x->right != t->nil){
        return tree_min(x->right);
    }
    struct node* y = x->parent;
    while((y!=t->nil)&&(x == y->right)){
        x = y;
        y = y->parent;
    }
    return y;
}

struct node* tree_predecessor(struct node* x){
    if(x->left != t->nil){
        return tree_max(x->left);
    }
    struct node* y = x->parent;
    while((y!=t->nil)&&(x == y->left)){
        x = y;
        y = y->parent;
    }
    return y;
}

void left_rotate(struct node* x){
    struct node* y = x->right;
    x->right = y->left;
    if(y->left!=t->nil){
        y->left->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == t->nil){
        t->root = y;
    }
    else if (x == x->parent->left){
        x->parent->left = y;
    }
    else{
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void right_rotate(struct node* x){
    struct node* y = x->left;
    x->left = y->right;
    if(y->right!=t->nil){
        y->right->parent = x;
    }
    y->parent = x->parent;
    if(x->parent == t->nil){
        t->root = y;
    }
    else if (x == x->parent->right){
        x->parent->right = y;
    }
    else{
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void rb_insert_fixup(struct node* z){
    struct node* y;
    struct node* x;
    if (t->root == z)
        t->root->color = 'b';
    else{
        x = z->parent; //x = parent del nodo dato in input

        if (x->color == 'r'){ //caso 1
            if(x == x->parent->left){ //se x è il figlio sinistro
                y = x->parent->right; // y è uncle di z
                if(y->color == 'r'){
                    x->color = 'b';
                    y->color = 'b';
                    x->parent->color = 'r';
                    rb_insert_fixup(x->parent);
                }
                else{
                    if(z == x->right){ //caso 2
                        z = x;
                        left_rotate(z);
                        x = z->parent;
                    } //caso 3
                    x->color = 'b';
                    x->parent->color = 'r';
                    right_rotate(x->parent);
                }
            }
            else{ //x è  figlio destro
                y = x->parent->left;

                if(y->color == 'r'){
                    x->color = 'b';
                    y->color = 'b';
                    x->parent->color = 'r';
                    rb_insert_fixup(x->parent);
                }
                else{
                    if(z == x->left){
                        z = x;
                        right_rotate(z);
                        x = z->parent;
                    }
                    x->color = 'b';
                    x->parent->color = 'r';
                    left_rotate(x->parent);
                }
            }
        }
    }
}

void rb_insert(struct node* z){
    struct node* y = t->nil;
    struct node* x = t->root;
    while (x!=t->nil){
        y=x;
        if (z->value < x->value)
            x = x->left;
        else
            x = x->right;
    }
    z->parent = y;
    if (y == t->nil){
        t->root = z;
    }
    else if(z->value < y->value){
        y->left = z;
    }
    else{
        y->right = z;
    }
    z->left = t->nil;
    z->right = t->nil;
    z->color = 'r';
    rb_insert_fixup(z);
}

void rb_delete_fixup(struct node *x){
    struct node *w;
    if((x->color == 'r')||(x->parent == t->nil)){
        x->color = 'b';
        return;
    }
    else if(x == x->parent->left){
        w = x->parent->right;
        if(w->color == 'r'){
            w->color = 'b';
            x->parent->color = 'r';
            left_rotate(x->parent);
            w = x->parent->right;
        }
        if((w->left->color=='b')&&(w->right->color == 'b')){
            w->color = 'r';
            rb_delete_fixup(x->parent);
        }
        else{
            if(w->right->color =='b'){
                w->left->color ='b';
                w->color ='r';
                right_rotate(w);
                w = x->parent->right;
            }
            w->color = x->parent->color;
            x->parent->color = 'b';
            w->right->color = 'b';
            left_rotate(x->parent);
        }
    }
    else{
        w = x->parent->left;
        if(w->color == 'r'){
            w->color = 'b';
            x->parent->color = 'r';
            right_rotate(x->parent);
            w = x->parent->left;
        }
        if((w->right->color=='b')&&(w->left->color == 'b')){
            w->color = 'r';
            rb_delete_fixup(x->parent);
        }
        else{
            if(w->left->color =='b'){
                w->right->color ='b';
                w->color ='r';
                left_rotate(w);
                w = x->parent->left;
            }
            w->color = x->parent->color;
            x->parent->color = 'b';
            w->left->color = 'b';
            right_rotate(x->parent);
        }
    }
}

void rb_delete(struct node *z){
    struct node *y;
    struct node* x;
    if ((z->left == t->nil) || (z->right == t->nil))
        y = z;
    else
        y = tree_successor(z);
    if (y->left != t->nil){
        x = y->left;
    }
    else
        x = y->right;
    x->parent = y->parent;
    if (y->parent == t->nil)
        t->root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;
    if(y != z)
        z->value = y->value;
    if (y->color == 'b')
        rb_delete_fixup(x);
    free(y);
}

void int_to_str(unsigned int arr, char*str, int *index){
    if(arr==0){
        str[0] = arr%10 + '0';
        *index = *index + 1;
        return;
    }
    else{
        int len=0;
        unsigned int copy=arr;
        while(copy/10!=0){
            len++;
            copy = copy/10;
        }
        *index = *index + len + 1;
        while(arr!=0){
            str[len]=arr%10 +'0';
            len--;
            arr=arr/10;
        }
    }
}

//reverse inorder
void inorder(struct node* n, char *str, int *index) {
    if(n != t->nil) {
        inorder(n->left, str, index);
        /*char *output;
        output =
        fputs(" ", stdout);*/
        //printf("%d",n->index);
        int_to_str(n->index, &str[*index],index);
        if(n != max){
            str[*index]=' ';
            *index = *index + 1;
        }
        inorder(n->right, str, index);
    }
}

//funzione per ricavare integer da stringa
void str_to_int(char *str, unsigned int *arr){
    char *start = str;
    int len = 0;
    for(int j=0; j<d; j++){
        arr[j]=0;
    }

    for(int i = 0; i<d; i++){
        while((start[len]!=',')&&(start[len]!='\n')&&(start[len]|='\0')){
            arr[i]=start[len] - '0' + (10*arr[i]);
            len ++;
        }
        start =&start[len + 1];
        len = 0;
    }
}



//funzione per trovare la somma dei cammini minimi (una specie di dijkstra con array) complessità O(d^2)
unsigned int shortest_path(unsigned int matrix[d][d], bool is_minimun[]){
    unsigned int i;
    unsigned int min = 0;
    unsigned int min_index=0;
    unsigned int dist = 0;
    unsigned int check_count = d;
    unsigned int locker=0;
    is_minimun[0]= true;
    matrix[0][0]=0;
    for(i=1;i<d;i++){
        if(min==0){
            if(matrix[locker][i]>0){
                min = matrix[locker][i];
                min_index = i;
            }
        }
        else
            if((matrix[locker][i]<min)&&(matrix[locker][i]>0)){
                min = matrix[locker][i];
                min_index = i;
            }
    }
    locker = min_index;
    is_minimun[locker] = true;
    if(locker == 0){
        return dist;
    }

    while(check_count>0){
        --check_count;
        //matrix[locker][locker]=0;
        min = 0;
        min_index=0;
        for(i=1;i<d;i++){
            if(is_minimun[i]==true){
                continue;
            }
            else{
                if((matrix[locker][i]==0)){
                    continue;
                }
                else{
                    min = matrix[locker][i];
                    if((matrix[0][i]==0)||(matrix[0][i]>min+matrix[0][locker])){
                        matrix[0][i]= min+matrix[0][locker];
                    }
                }
            }
        }
        min = 0;
        min_index =0;
        for(i=1;i<d;i++){
            if(is_minimun[i]==true){
                continue;
            }
            if(min==0){
                if(matrix[0][i]>0){
                    min = matrix[0][i];
                    min_index = i;
                }
            }
            else
                if((matrix[0][i]<min)&&(matrix[0][i]>0)){
                    min = matrix[0][i];
                    min_index = i;
                }
        }
        locker = min_index;
        is_minimun[locker] = true;
        if(locker == 0){
            break;
        }
    }

    for(i=0;i<d;i++)
        dist = dist + matrix[0][i];
    //printf(" dist = %d", dist);
    return dist;
}

void add_graph(){
    unsigned int graph[d][d];
    bool checker[d];
    unsigned int i;
    /*unsigned int j;
    char *ptr;
    char *line_cut;*/

    for(i=0; i<d; ++i) {
        checker[i] = false;
        a = fgets(line, MAX_LINE +1 , stdin);

        str_to_int(a, graph[i]);
        /*j=0;
        line_cut = strtok(line, ",");
        graph[i][j]=strtoul(line_cut , &ptr, 10);
        while(line_cut!=NULL){
            ++j;
            if(j==d){
                break;
            }
            line_cut = strtok(NULL, ",");
            graph[i][j]=strtoul(line_cut , &ptr, 10);
        }*/
    }
    //print_graph(graph);

    //printf("graph index = %d\n", graph_index);

    unsigned int dist = shortest_path(graph, checker);
    //printf("la minima distanza è %d\n",dist);

    //aggiungo il grafo all'albero che sarà sempre grande k
    if(graph_index<k){ //caso che non ho ancora k grafi
        if(graph_index == 0){
            rb_insert(new_node(dist, graph_index));
            max = tree_max(t->root);
            pre = tree_predecessor(max);
        }
        else{
            rb_insert(new_node(dist, graph_index));
            if(max->value<dist){
                pre = max;
                max = tree_successor(max);
            }
            else{
                if(dist>pre->value){
                    pre = tree_predecessor(max);
                }
            }
        }
    }
    else{  //caso che ho più di k grafi
        if(dist<max->value){
            rb_insert(new_node(dist, graph_index));
            rb_delete(max);
            max= tree_max(t->root);
            /*if(dist>pre->value){

                max = pre;
                pre = tree_predecessor(max);
            }
            else if(dist==pre->value){

                max= tree_max(t->root);
                pre = tree_predecessor(max);
            }
            else{

                max = pre;
                pre = tree_predecessor(max);
            }*/
        }
    }
    graph_index++;
}

void top_k(){
    if(graph_index == 0){
        printf("\n");
        return;
    }
    else{
        char c[k*14];
        int index=0;
        //printLevelOrder(t->root);
        inorder(t->root,c,&index);
        c[index]='\0';
        fputs(c,stdout);
        printf("\n");
    }
}

void selection(){
    if (command[0]=='A'){
        add_graph();
    }
    else if (command[0]=='T'){
        top_k();
    }
}

int main() {
    graph_index=0;

    //inizializzo rb_tree
    t = malloc(sizeof(struct rb_tree));
    t->nil = malloc(sizeof(struct node));
    t->nil->parent = NULL;
    t->nil->right = NULL;
    t->nil->left = NULL;
    t->nil->color = 'b';
    t->nil->index = 0;
    t->nil->value = 0;
    t->root = t->nil;


    //ricava commando
    b = scanf("%d %d", &d, &k);

    while (fgets(command,  30, stdin)!=NULL){
        selection();
    }
    return 0;
}
