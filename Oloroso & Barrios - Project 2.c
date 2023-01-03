/*Project 2 - File Compression and Decompression in C using the Huffman Algorithm
By Andrew R Oloroso and Armand Angelo C Barrios*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 256

typedef struct tree{
    int freq;
    char c;
    struct tree *left, *right;
}*tree;

struct tree chars[256] = {{0}}; //initialize chars
tree qqq[255], *que = qqq - 1;
int nodesNum = 0, endQueue = 0;
char *num[128] = {0}, buffer[1024];

tree newNode(int fr, char ccc, tree l, tree r){
    tree n = chars+nodesNum++;
    if(fr){ //if no root
        n->c = ccc;
        n->freq = fr;
    }else{ //merging two nodes to have a tree
        n->left = l, n->right = r;
        n->freq = l->freq + r->freq;
    }
}

void insertQueue(tree n){
    int y, x = endQueue++;
    while(y = x/2){
        if(que[y]->freq <= n->freq){
            break;
        }
        que[x] = que[y];
        x = y;
    }
    que[x] = n;
}

tree removeQueue(){
    int x,y;
    tree n = que[y = 1];
    if(endQueue < 2){
        return 0;
    }endQueue--;

    while((x = y*2)<endQueue){
        if(x+1 < endQueue && que[x+1]->freq < que[x]->freq){
            x++;
        } que[y] = que[x]; y=x;
    }
    que[y] = que[endQueue];
    return n;
}

void makeTree(tree n, char *str, int len){
    static char *buf = buffer; //static is used to remain the value of buf
    if(n->c){
        str[len] = 0;
        strcpy(buf ,str);
        num[n->c] = buf;
        buf += len + 1;
        return;
    }
    str[len] = '0'; makeTree(n->left, str, len+1);
    str[len] = '1'; makeTree(n->right, str, len+1);
}

void init(int fr[]){
    char cc[16];

    for(int x=0; x<256; x++){
        if(fr[x]){
            insertQueue(newNode(fr[x],x,0,0));
        }
    }

    while(endQueue > 2){
        insertQueue(newNode(0,0,removeQueue(),removeQueue()));
    }

    makeTree(que[1],cc,0);
}


void encode(){

}

void decode(const char *str, tree temp){
    tree n = temp;
    printf("Decoded: ");
    while(*str){
        if(*str++ == '0'){
            n = n->left;
        }else{
            n = n->right;
        }
        if(n->c){
            putchar(n->c);
            n = temp;
        }
    }
    putchar('\n');
    if(temp != n){
        printf("Error.\n");
    }
}

int menu(){
    int choice;
    printf("\nMenu\n");
    printf("[1] Encode\n");
    printf("[2] Decode\n");
    printf("[3] Exit\n");
    printf("\nEnter Number: "); scanf("%d",&choice);
    return choice;
}

int main(){
    FILE *fp;
    char filename[45];
    int freqq[256]={0},index[MAX],count[MAX],chh,i=0,temp;
    char bufferr[1024];
    switch(menu()){
        case 1: printf("Input filename: ");
                scanf(" %[^\n]s",filename);
                fp=fopen(filename,"r");
                if (fp==NULL){
                    printf("File error.\n");system("pause");
                }else{
                    while(!feof(fp)){
                        chh = fgetc(fp);
                        freqq[chh]++;
                    }
                    fclose(fp);
                }

                printf("\nFrequency:\n");
                for(int x=0; x<MAX; x++){
                    if(freqq[x]!=0){
                        index[i]=x;
                        count[i]=freqq[x]; i++;
                        printf("%d : \t '%c' \t : %d\n",index[i-1],index[i-1],count[i-1]);
                    }
                }system("pause");system("cls");
                //sort
                for(int k=0;k<i;k++){
                    for(int j=0;j<i;j++){
                        if(count[j]!=0){
                            if(count[j]>count[j+1]){
                                temp = count[j];
                                count[j] = count[j+1];
                                count[j+1] = temp;

                                temp = index[j];
                                index[j] = index[j+1];
                                index[j+1] = temp;
                            }
                        }
                    }
                }
                printf("\nSorted:\n");
                for(int x=0; x<=i; x++){
                    if(count[x]!=0){
                        printf("%d : \t '%c' \t : %d\n",index[x],index[x],count[x]);
                    }
                }system("pause");system("cls");
                init(index);
                for(int x=0; x<256; x++){
                    if(num[x]){
                        printf("'%c' : %s\n",x,num[x]);
                    }
                }printf("Encoded: %s\n",bufferr);
                break;
        case 2: decode(bufferr, que[1]);break;
        case 3: exit(0);
    }return 0;
}
