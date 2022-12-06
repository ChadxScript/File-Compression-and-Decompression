/*Project 2 - File Compression and Decompression in C using the Huffman Algorithm
By Andrew R Oloroso and Armand Angelo C Barrios*/
#include <stdio.h>
#include <stdlib.h>
#define MAX 256

int main(){
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
            printf("%d : \t '%c' \t : %d\n",c[x-1],c[x-1],freq[i]);
        }
    }
    //sort
    for(int i=0;i<MAX;i++){
        for(int j=0;j<MAX;j++){
            if(freq[j]>freq[j+1]){
                temp = freq[j];
                freq[j] = freq[j+1];
                freq[j+1] = temp;
            }
        }
    }
    printf("\nsorted\n");
    for(int i=0;i<MAX;i++){
        if(freq[i]!=0){
            x--; printf("%d : \t '%c' \t : %d\n",c[x],c[x],freq[i]);
        }
    }
    system("pause");
    return 0;
}
