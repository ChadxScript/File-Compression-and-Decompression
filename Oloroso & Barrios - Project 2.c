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
    char dch;
    int treeleaf;
    struct que *dleft, *dright;
    struct que *next;
}pque; pque *H;

//Global Var
int chfreq[MAX], code[MAX];
int count,isNull,ascCount,ascRem,loc=0,size=0,chnum=-1,shift=7,space=8;
char chCode[MAX], memory='\0',ascmemory='\0',tempMemory='\0';

int dshift=0,fr,sc=0,rb;
char temp;

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
                memory = memory << 1;
            }if(loc==8){
                //fprintf(compressedFile,"%d",memory);
                fputc(memory, compressedFile);
                loc=0;
                memory='\0';
            }
        }
    }
    while(loc!=7){
        memory = memory << 1;
        loc++;
    }
    //fprintf(compressedFile,"%d",memory);
    fputc(memory, compressedFile);
    ftell(compressedFile);
    fclose(compressedFile);
}
void saveBinary(FILE *tr,tree *n){
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
    }saveBinary(tr,n->left);
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
    }saveBinary(tr,n->right);
}
//decompress
void makeDTree(pque *n, FILE *ptr){
    pque *newNode;
    if(temp==1){
        if(dshift<8){
            ascmemory |= memory << dshift;
            ascRem = dshift;
        }else{
            rb = fgetc(ptr);
            ascmemory = rb;
            ascRem = 0;
        }if(ascRem>0){
            rb = fgetc(ptr);
            memory = rb;
            temp = memory;
            temp >>= 8-dshift;
            ascmemory |= temp;
        }
        n->dch = ascmemory;
        n->treeleaf = 1;
        ascmemory = '\0';
        return;
    }if(dshift<8){
        temp = memory;
    }else{
        rb = fgetc(ptr);
        memory = rb;
        temp = memory;
        dshift = 0;
    }
    temp <<= dshift;
    temp >>= 7;
    dshift++;

    newNode = (pque*) malloc(sizeof(pque));
    newNode->dch = '\0';
    newNode->treeleaf = 0;
    n->dleft=newNode;
    makeDTree(n->dright,ptr);
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
    pque *dtree = (pque*) malloc(sizeof(pque));
    pque *dnode;
    tree *huffTree;
    FILE *fp,*fpq,*fpc,*fpp;
    char filename[45];
    makeNull();
    initArray();
    while(1){
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
                    fp = fopen("tree.txt","wb");
                    memory='\0';
                    saveBinary(fp,huffTree);
                    fputc(memory,fp);
                    fclose(fp);
                    fpq = fopen("frequency.txt","w");
                    fprintf(fp,"%d",huffTree->freq);
                    fclose(fpq);
                    break;
            case 2: printf("Make sure you have 'compress.txt','frequency.txt', and 'tree.txt' files.\n");system("pause");
                    fp=fopen("tree.txt","rb");
                    if (fp==NULL){
                        printf("tree.txt not found.\n");exit(0);
                    }else{
                        fpq = fopen("frequency.txt","r");
                    if(fpq==NULL){
                            printf("frequency.txt not found.\n");exit(0);
                        }else{
                            fscanf(fpq,"%d",&fr);
                        }fclose(fpq);
                        rb=fgetc(fp);
                        memory = rb;
                        makeDTree(dtree,fp);
                    }fclose(fp);
                    fpc=fopen("compress.txt","rb");
                    fpp=fopen("decompress.txt","w");
                    if(fpc==NULL){
                        printf("compress.txt not found.\n");exit(0);
                    }else{
                        printf("\nDecoded text: \n\n");
                        dnode = dtree;
                        while(!feof(fpc)){
                            rb = fgetc(fpc);
                            memory = rb;
                            dshift = 0;
                            while(dshift<0){
                                temp = memory;
                                temp<<=dshift;
                                temp>>=7;
                                if(temp==0){
                                    dnode = dnode->dleft;
                                }else if(temp==1){
                                    dnode = dnode->dright;
                                }if(dnode->treeleaf==1){
                                    putchar(dnode->dch);
                                    fputc(dnode->dch,fpp);
                                    dnode = dtree;
                                    sc++;
                                }if(sc==fr){ //stop if all chars in the file are decompressed
                                    break;
                                }
                            }
                            if(sc==fr){
                                break;
                            }
                        }
                    }
                    fclose(fpc);
                    fclose(fpp);
                    printf("\n\nCheck 'decompressed.txt' file for the decompressed text.\n");system("pause");
                    free(dtree);
                    break;
            case 3: exit(0);
        }
    }
    return 0;
}
