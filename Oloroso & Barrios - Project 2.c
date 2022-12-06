/*Project 2 - File Compression and Decompression in C using the Huffman Algorithm
By Andrew R Oloroso and Armand Angelo C Barrios*/
#include <stdio.h>
#include <stdlib.h>
#define MAX 256

int main(){
    FILE *fp;
    char filename[45];
    int freq[MAX]={0}, index[MAX], count[MAX], x=0;
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
        if(freq[i]!=0){ //store the value of chars
            index[x] = i;
            count[x] = freq[i]; x++;
            printf("%d : \t '%c' \t : %d\n",index[x-1],index[x-1],count[x-1]);
        }
    }
    //sort
    for(int i=0;i<x;i++){
        for(int j=0;j<x;j++){
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
    printf("\nsorted\n");
    for(int i=0;i<=x;i++){
        if(count[i]!=0){
            printf("%d : \t '%c' \t : %d\n",index[i],index[i],count[i]);
        }
    }
    system("pause");
    return 0;
}
