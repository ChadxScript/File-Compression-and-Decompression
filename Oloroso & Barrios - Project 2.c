/*Project 2 - File Compression and Decompression in C using the Huffman Algorithm
By Andrew R Oloroso and Armand Angelo C Barrios*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 256

typedef struct tree{
    char ch;
    int freq;
    struct tree *left, *right;
}tree;

typedef struct que{
    tree *qtree;
    struct que *next;
}pque; pque *H;

//Global Var
int chfreq[MAX], code[MAX];
int count,isNull,ascCount,ascRem,loc=0,size=0,chnum=-1,shift=7,space=8;
char chCode[MAX], memory='\0';

void makeNull(){
    H = NULL;
}
void initArray(){
    for(int x=0; x<MAX; x++){
        chfreq[x]=0;
    }
}
pque *makeNode(int i, int j, tree *leftt, tree *rightt){
    pque *newNode = (pque*) malloc(sizeof(pque));
    newNode->qtree = (tree*) malloc(sizeof(tree));
    newNode->qtree->left = leftt;
    newNode->qtree->right = rightt;
    newNode->next = NULL;
    if(j!=0){
        newNode->qtree->ch = i;
        newNode->qtree->freq = chfreq[i];
    }else{
        newNode->qtree->ch = '\0';
        newNode->qtree->freq = i;
    }return newNode;
}
void deque(pque *currNode){
    if(currNode->next !=NULL){
        free(currNode->next);
    }free(currNode);
}
void enqueue(pque *newNode){ //(sort)
    pque *p, *q;
    p=q=H;
    if(H==NULL){ //first element
        newNode->next=H;
        H = newNode;
    }else{
        if(newNode->qtree->freq < H->qtree->freq){ //will put the next element to the next node if it has lower freq that newNode
            newNode->next=H;
            H = newNode;
        }else{
            while(p!=NULL){
                q=p; p=p->next;
                if(p!=NULL){
                    if(newNode->qtree->freq < p->qtree->freq){ //will insert the node if the freq is less than the next node
                        newNode->next = p;
                        q->next = newNode; break;
                    }
                }else{
                    q->next = newNode; //will insert the newnode if it has greater freq that the previous nodes
                    newNode->next = p;
                }
            }
        }
    }
}
void makeTree(pque *leftt, pque *rightt){
    pque *newNode, *temp = H;
    newNode = makeNode((leftt->qtree->freq)+(rightt->qtree->freq),0,leftt->qtree,rightt->qtree); //will add the freq of the 2 lowest freq and make a new node
    if(rightt->next!=NULL){
        H = rightt->next;
    }else{
        H = newNode;
    }
    deque(temp);
    enqueue(newNode);
    if(H->next!=NULL){
        makeTree(H, H->next);
    }
}
void trav(tree *newBranch){
    if(newBranch->left==NULL && newBranch->right==NULL){
        printf("\n%d\t: '%c'\t: %d\t: ",newBranch->ch,newBranch->ch,newBranch->freq);
        for(int x=0; x<=chnum; x++){
            printf("%d",code[x]);
        }return;
    }
    chnum++;
    code[chnum] = 0;
    trav(newBranch->left);
    code[chnum] = 1;
    trav(newBranch->right);
    chnum--;
}
void findd(tree *n, char c){
    if(n->left==NULL && n->right==NULL){
        if(n->ch==c){
            isNull=0;
        }return;
    }
    chnum++;
    chCode[chnum] = 0;
    findd(n->left,c);
    if(isNull!=0){
        chCode[chnum] = 1;
        findd(n->right,c);
    }if(isNull!=0){
        chnum--;
    }
}
void createBinary(FILE *f2p, tree *treeNode){
    FILE *compressedFile;
    compressedFile=fopen("compress.txt","wb");
    while(!feof(f2p)){
        chnum = -1;
        isNull = 1;
        count = fgetc(f2p);
        findd(treeNode,count);
        for(int x=0; x<=chnum; x++){
            loc++;
            memory = memory | chCode[x]; //will store the value of 1 if any of the two var is 1
            if(loc<8){
                loc = memory << 1;
            }if(loc==8){
                fputc(memory,compressedFile);
                loc=0;
                memory='\0';
            }
        }
    }
    while(loc!=7){
        memory = loc << 1;
        loc++;
    }
    fputc(memory,compressedFile);
    ftell(compressedFile);
    fclose(compressedFile);
}
void saveBinary(tree *n){
    FILE *tr;
    tr = fopen("tree.txt","wb");
    char temp;
    if(n->left==NULL && n->right==NULL){
        temp = 1;
        temp <<= shift;
        memory |= temp;
        space--;
        ascCount = 8-shift;
        ascRem = ascCount;
        if(space>0){
            temp = n->ch;
            temp >>= ascCount;
            memory |= temp;
        }
        fputc(memory,tr);
        memory='\0';
        shift=7;
        space=8;
        if(ascRem>0){
            temp = n->ch;
            temp >>= ascRem;
            memory |= temp;
            space = 8-ascRem;
            shift = space-1;
            if(ascRem==8){
                fputc(memory,tr);
                memory='\0';
                shift=7;
                space=8;
            }
        }
        ascCount=8;
        ascRem=8;
        return;
    }
    if(n->left->ch == '\0'){
        temp=0;
        temp <<= shift;
        memory |= temp;
        shift--;
        space--;
        if(space==0){
            fputc(memory,tr);
            memory='\0';
            shift=7;
            space=8;
        }
    }saveBinary(n->left);
    if(n->right->ch == '\0'){
        temp=0;
        temp <<= shift;
        memory |= temp;
        shift--;
        space--;
        if(space==0){
            fputc(memory,tr);
            memory='\0';
            shift=7;
            space=8;
        }
    }saveBinary(n->right);
    fputc(memory,tr);
    fclose(tr);
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
    pque *newNode;
    tree *huffTree;
    FILE *fp,*fpq;
    char filename[45];
    makeNull();
    initArray();
    switch(menu()){
        case 1: printf("Input .txt filename: ");
                scanf(" %[^\n]s",filename);
                fp=fopen(filename,"r");
                if (fp==NULL){
                    printf("File error.\n");exit(0);
                }else{
                    while(!feof(fp)){
                        count = fgetc(fp);
                        chfreq[count]++;
                        putchar(count);
                        size++;
                    }
                    fclose(fp);
                }
                for(int x=0; x<MAX; x++){
                    if(chfreq[x]>0){
                        newNode = makeNode(x,1,NULL,NULL);
                        enqueue(newNode);
                    }
                }
                makeTree(H, H->next);
                huffTree = H->qtree;
                free(H);
                printf("\n\nAscii\tChar\tFreq\tCode");
                trav(huffTree);

                //create binary and save to file
                fp = fopen(filename,"r");
                createBinary(fp,huffTree);
                fclose(fp);
                saveBinary(huffTree);
                fpq = fopen("frequency.txt","w");
                fprintf(fp,"%d",huffTree->freq);
                fclose(fpq);
                break;
        case 2: //decode(bufferr, que[1]);break;
        case 3: exit(0);
    }return 0;
}
