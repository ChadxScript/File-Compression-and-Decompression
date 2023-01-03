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
}tree;

tree *heap[100];
int heapsize=0;

int sortt(int fr[], int ind[], int cou[]){
    int i=0,temp;
    printf("\nFrequency:\n");
    for(int x=0; x<MAX; x++){
        if(fr[x]!=0){
            ind[i]=x;
            cou[i]=fr[x]; i++;
            printf("%d : \t '%c' \t : %d\n",ind[i-1],ind[i-1],cou[i-1]);
        }
    }system("pause");system("cls");
    //sort
    for(int k=0;k<i;k++){
        for(int j=0;j<i;j++){
            if(cou[j]!=0){
                if(cou[j]>cou[j+1]){
                    temp = cou[j];
                    cou[j] = cou[j+1];
                    cou[j+1] = temp;

                    temp = ind[j];
                    ind[j] = ind[j+1];
                    ind[j+1] = temp;
                }
            }
        }
    }printf("\nSorted:\n");
    for(int x=0; x<=i; x++){
        if(cou[x]!=0){
            printf("%d : \t '%c' \t : %d\n",ind[x],ind[x],cou[x]);
        }
    }system("pause");system("cls"); return i;
}

void iinsert(tree *n){
    heapsize++;
    heap[heapsize] = n;
    int current = heapsize;
    while(heap[current/2]->freq > n->freq){
        heap[current] = heap[current/2];
        current /= 2;
    }heap[current] = n;
}

tree *removeMin(){
    tree *minN, *lastN;
    int child,current;
    minN = heap[1];
    lastN = heap[heapsize--];
    for(current=1; current*2<=heapsize; current=child){
        child = current*2;
        if(child != heapsize && heap[child+1]->freq < heap[child]->freq){
            child++;
        }if(lastN->freq > heap[child]->freq){
            heap[current]=heap[child];
        }else{
            break;
        }
    }
    heap[current] = lastN;
    return minN;
}

void display(tree *temp,char *code){
    if(temp->left==NULL && temp->right==NULL){
        printf("%c \t : \t %s\n",temp->c,code); return;
    }
    int len = strlen(code);
    char leftcode[10],rightcode[10];
    strcpy(leftcode,code);
    strcpy(rightcode,code);
    leftcode[len] = '0';
    leftcode[len+1] = '\0';
    rightcode[len] = '1';
    rightcode[len+1] = '\0';
    display(temp->left,leftcode);
    display(temp->right,rightcode);
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
    int freqq[256]={0},index[MAX],count[MAX],chh;
    char bufferr[1024];
    switch(menu()){
        case 1: heap[0] = (tree *) malloc(sizeof(tree));
                heap[0]->freq = 0;
                printf("Input filename: ");
                scanf(" %[^\n]s",filename);
                fp=fopen(filename,"r");
                if (fp==NULL){
                    printf("File error.\n");system("pause");break;
                }else{
                    while(!feof(fp)){
                        chh = fgetc(fp);
                        freqq[chh]++;
                    }
                    fclose(fp);
                }
                printf("\nSorted: \n");
                int a=sortt(freqq,index,count);
                for(int x=0; x<=a;x++){
                    printf("index: %c count: %d a: %d\n",(char)index[x],count[x],a);
                }system("pause");

                for(int x=0;x<=a;x++){
                    tree *temp = (tree*) malloc(sizeof(tree));
                    temp->c = (char)index[x];
                    temp->freq = count[x];
                    temp->left = temp->right = NULL;
                    iinsert(temp);
                }
                if(a==1){
                    printf("%c \t : \t 0\n",index[a]); return 0;
                }
                for(int x=0; x<a; x++){
                    tree *left = removeMin();
                    tree *right = removeMin();
                    tree *temp = (tree*) malloc(sizeof(tree));
                    temp->c = 0;
                    temp->left = left;
                    temp->right = right;
                    temp->freq = left->freq + right->freq;
                    iinsert(temp);
                }

                printf("\nEncoded: \n");
                tree *huffman = removeMin();
                char code[10];
                code[0] = '\0';
                display(huffman,code);
                break;
        case 2: //decode(bufferr, que[1]);
            break;
        case 3: exit(0);
    }return 0;
}
