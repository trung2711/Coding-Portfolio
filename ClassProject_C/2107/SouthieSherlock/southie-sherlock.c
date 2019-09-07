#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//Trung Viet Nguyen
//04/02/2018
//CIS 2107 Lab assignment 9
//Modify the quotes of "sherlock.txt" into Southern accents and write into "southie-sherlock.txt"

void read(FILE *fin, FILE *fout);
void modifiedPrint(FILE *fout,char *sPtr);
void change(FILE *fout,char *sPtr);
int isVowel(char c);

int main()
{
    FILE *fin;
    FILE *fout;
    fin = fopen("sherlock.txt","r");
    fout = fopen("southie-sherlock.txt","w+");
    if((fin==NULL)||(fout==NULL)){
       puts("File could not be opened");
    } else {
       read(fin, fout);
	puts("Successfully writes the file \"southie-sherlock.txt\"");
    }
    fclose(fin);
    fclose(fout);

    return 0;
}

/** Reads from file pointer fin and writes into fout
 */
void read(FILE *fin, FILE *fout){
    char c;
    fscanf(fin,"%c",&c);
    int inQuote = 0; //0 for outside of quote, 1 for inside quote

    while(!feof(fin)){
        char quote[5000];
        fprintf(fout,"%c",c);
        fscanf(fin,"%c",&c);
        if(c==34){ // if detect a quotation mark, change inQuote to 1
            inQuote = 1;
            fprintf(fout,"%c",c);
        }

        int numchar = 0; // to append char
        int sentence = 0; // to separate words
        int connective = 1; // to check for connective punctuation between characters
        while (inQuote==1){
            if(!feof(fin)){
            fscanf(fin,"%c",&c);
            quote[numchar] = c; // append
            numchar++;
            if (c==34){
                inQuote = 0;
                sentence = 1;
                connective = 1;
            } else if (ispunct(c)&&connective==1){
                connective = 0;
            } else if ((!isalnum(c)&&!ispunct(c))||(ispunct(c)&&connective==0)){
                connective = 1;
                sentence = 1;
            } else if(isalnum(c)&&connective==0){
                connective = 1;
            }
            } else {
                inQuote = 0;
                sentence = 1;
                connective = 1;
            }
            if(sentence == 1){ // if a complete word has been read
                sentence = 0;
                if(quote[numchar-1]!='"'){
                quote[numchar] = '\0';
                } else {
                quote[numchar-1] = '\0';
                }
                modifiedPrint(fout,quote);
                numchar = 0;
            }

        }
    }
}

/** Writes the modified char *sPtr into fout
 */
void modifiedPrint(FILE *fout,char *sPtr){
    int length = strlen(sPtr);
    char arr[length+1]; // to separate punctuation
    char word[length+5]; // actual word
    strcpy(arr,sPtr);
    int i = 0;

    while(!isalnum(arr[i])&&(i<length)){
        fprintf(fout,"%c",arr[i]); // fprintf all punctuations before actual word
        i++;
    }
    int j = 0;
    while(i<length&&(isalnum(arr[i])||isalnum(arr[i+1]))){
        word[j] = arr[i];
        j++;
        i++;
    }
    word[j] = '\0';
    change(fout,word); // changes and prints the word

    while(i<length){
        fprintf(fout,"%c",arr[i]); // fprintf the rest of the punctuations
        i++;
    }
}

/** Changes sPtr into Southern accents according to rules, and writes into fout
 */
void change(FILE *fout, char *sPtr){
    int length = strlen(sPtr);
    char arr[length+5];
    char word[length+10];
    int capitalize = 0;

    strcpy(arr,sPtr);

    if(strcmp(arr,"a")==0||strcmp(arr,"A")==0){
        fprintf(fout,"%s",arr);
        return;
    }
    if(strcmp(arr,"very")==0){
        strcpy(arr,"wicked");
        fprintf(fout,"%s",arr);
        return;
    }

    if(strcmp(arr,"Very")==0){
        strcpy(arr,"Wicked");
        fprintf(fout,"%s",arr);
        return;
    }


    if(isupper(arr[0])){
        capitalize = 1; //String is capitalized
        arr[0] = tolower(arr[0]);
    }

    int i; //counter for arr
    int j; //counter for changedWord

    // Appending..
    for(i = 0, j = 0; arr[i]!='\0';i++){
        if(arr[i] == 'r'){
            if(i>=2&&arr[i-1]=='o'&&arr[i-2]=='o'&&(!isalnum(arr[i+1]))){
                word[j] = 'w';
                word[++j] = 'a';
                word[++j] = 'h';
                j++;
            } else if (i>=2&&arr[i-1]=='e'&&arr[i-2]=='e'&&(!isalnum(arr[i+1]))){
                word[j] = 'y';
                word[++j] = 'a';
                word[++j] = 'h';
                j++;
            } else if (i>=1&&isVowel(arr[i-1])){
                word[j] = 'h';
                j++;
            } else {
                word[j] = arr[i];
                j++;
            }
        } else if (arr[i] == 'a'&&!isalnum(arr[i+1])) {
            word[j] ='a';
            word[++j] = 'r';
            j++;
        } else {
            word[j] = arr[i];
            j++;
        }
    }
    word[j] = '\0';
    if(capitalize==1){
        word[0] = toupper(word[0]);
    }

    fprintf(fout,"%s",word); //final product
}

/** Returns 1 if char c is a vowel, else returns 0
 */
int isVowel(char c){
    c = tolower(c);
    if(c=='a'||c=='i'||c=='o'||c=='e'||c=='u'){
        return 1;
    }
    return 0;
}
