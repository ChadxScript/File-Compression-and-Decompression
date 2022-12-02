/*Project 2 - File Compression and Decompression in C using the Huffman Algorithm
By Andrew R Oloroso and Armand Angelo C Barrios*/
#include <stdio.h>
#include <stdlib.h>
#define MAX 256

typedef struct TREENODE {
    int data;
    struct TREENODE *left;
    struct TREENODE *right;
}treeNode;

treeNode *newNode(int x){//create new node
    treeNode *cr;
    cr = (treeNode*) malloc(sizeof(treeNode));
    cr->data = x;
    cr->left = NULL;  //initialize left and right to null
    cr->right = NULL;
}
void preOrder(treeNode *r){
    if(r==NULL){
        return;
    }
    printf("%d ", r->data);
    preOrder(r->left);
    preOrder(r->right);
}
void inOrder(treeNode *r){
    if(r==NULL){
        return;
    }
    inOrder(r->left);
    printf("%d ", r->data);
    inOrder(r->right);
}
void postOrder(treeNode *r){
    if(r==NULL){
        return;
    }
    postOrder(r->left);
    postOrder(r->right);
    printf("%d ", r->data);
}

int main(){
    treeNode *p;
    FILE *fp;
    char filename[45];
    int freq[MAX]={0}, c[MAX], x=0;
    int temp, ch;
    printf("Input filename: ");
    scanf(" %[^\n]s",filename);
    fp=fopen(filename,"r");
    if (fp==NULL){
        printf("File error.\n");system("pause");
    }else{
        while(!feof(fp)){
            ch = fgetc(fp);
            freq[ch]++;
        }
        fclose(fp);
    }
    for(int i=0;i<MAX;i++){
        if(freq[i]!=0){
            c[x] = i; x++; //store the value of chars
            printf("%d : \t '%c' \t : %d\n",i,c[x-1],freq[i]);
        }
    }
    //sort
    x=0;
    for(int i=0;i<MAX;i++){
        for(int j=1;j<MAX;j++){
            if(freq[i]!=0 && freq[j]!=0){
                if(freq[i]<freq[j]){
                    temp = freq[j];
                    freq[j] = freq[i];
                    freq[i] = temp;

                    temp = c[x]; printf("\ntemp=%d c x+1=%d c x=%d\n",temp,c[x+1],c[x]);
                    c[x] = c[x+1];
                    c[x+1] = temp;
                    x++;
                }
            }
        }
    }
    x=0;
    printf("\n\n");
    for(int i=0;i<MAX;i++){
        if(freq[i]!=0){
            printf("%d : \t '%c' \t : %d\n",c[x],c[x],freq[i]);
            x++;
        }
    }
    system("pause");
    return 0;
}
