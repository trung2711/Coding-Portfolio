#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Trung Viet Nguyen
// 03/16/2018
// CIS 2107 Lab Assignment 8
// Given a 7 digit phone number, writes to a file every possible seven-letter word corresponding to that number

void wordsGenerator(long telNum);

int main()
{
    puts("Enter a 7-digit number not containing 0s and 1s: ");
    long num;
    scanf("%ld", &num);
    wordsGenerator(num);
    return 0;
}

/** Inputs a 7-digit long integer representing a telephone number
 *  Writes to a file called "telephoneNum.txt" all possible seven-letter word corresponding to the number
 *  Prints out an error message if the integer is not 7-digit long, or if the integer contains 0s and 1s
 */
void wordsGenerator(long telNum){
    if(telNum<2222222||telNum>9999999){ //Number from 1000000 and 2222221 will surely contain 0s and 1s
        puts("The number must be 7-digit long, and cannot contain 0s and 1s.");
        return;
    }
    char *array[10] = {"","","ABC","DEF","GHI","JKL","MNO","PQRS","TUV","WXYZ"};

    FILE *tel;
    if((tel = fopen("telephoneNum.txt","w"))==NULL){
        puts("File could not be opened");
    } else{
        char *s1, *s2, *s3, *s4, *s5, *s6, *s7;
        int digit = telNum%10;
        if(digit==0||digit==1){
            puts("Cannot convert the number to strings: number contains 0 or 1.");
            return;
        }
        s7 = array[digit];

        telNum/=10;
        digit = telNum%10;
        if(digit==0||digit==1){
            puts("Cannot convert the number to strings: number contains 0 or 1.");
            return;
        }
        s6 = array[digit];

        telNum/=10;
        digit = telNum%10;
        if(digit==0||digit==1){
            puts("Cannot convert the number to strings: number contains 0 or 1.");
            return;
        }
        s5 = array[digit];

        telNum/=10;
        digit = telNum%10;
        if(digit==0||digit==1){
            puts("Cannot convert the number to strings: number contains 0 or 1.");
            return;
        }
        s4 = array[digit];

        telNum/=10;
        digit = telNum%10;
        if(digit==0||digit==1){
            puts("Cannot convert the number to strings: number contains 0 or 1.");
            return;
        }
        s3 = array[digit];

        telNum/=10;
        digit = telNum%10;
        if(digit==0||digit==1){
            puts("Cannot convert the number to strings: number contains 0 or 1.");
            return;
        }
        s2 = array[digit];

        digit = telNum/10;
        if(digit==0||digit==1){
            puts("Cannot convert the number to strings: number contains 0 or 1.");
            return;
        }
        s1 = array[digit];

        size_t len1 = strlen(s1), len2 = strlen(s2), len3 = strlen(s3), len4 = strlen(s4), len5 = strlen(s5), len6 = strlen(s6), len7 = strlen(s7);
        size_t i,j,k,l,m,n,o;
        for(i=0;i<len1;i++){
            for(j=0;j<len2;j++){
                for(k=0;k<len3;k++){
                    for(l=0;l<len4;l++){
                        for(m=0;m<len5;m++){
                            for(n=0;n<len6;n++){
                                for(o=0;o<len7;o++){
                                    char result[8] = {*(s1+i),*(s2+j),*(s3+k),*(s4+l),*(s5+m),*(s6+n),*(s7+o),'\0'};
                                    fprintf(tel,"%s\n",result);
                                }
                            }
                        }
                    }
                }
            }
        }
        puts("Successfully write to the file \"telephoneNum.txt\" all possible seven-letter words corresponding to the number!");
    }
}
