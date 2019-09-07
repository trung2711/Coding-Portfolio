#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 76

// Trung Viet Nguyen
// 03/05/2018
// CIS 2107 Lab Assignment 6
// Recreate the race between tortoise and hare


// Function prototype
void play(char *cPtr);
void printPos(char *cPtr);

char arr[SIZE] = {' '};

int main()
{
    srand(time(NULL));
    play(arr);
    return 0;
}

// Plays the game
void play(char *cPtr){
    int T = 0;
    int H = 0;


    puts("ON YOUR MARK, GET SET");
    puts("BANG               !!!!");
    puts("AND THEY'RE OFF    !!!!");

    while(T!=69&&H!=69){ // While no one has won yet
    sleep(1); // Pauses between each loop
    int i = 1 + rand()%10; // Random int for T roll
    int j = 1 + rand()%10; // Random int for H roll

    // T moves according to the roll
    if(i<=5){
        T+=3;
    } else if (i<=7){
        T-=6;
    } else {
        T+=1;
    }

    // H moves according to the roll
    if(j<=2){
    } else if(j<=4){
        H+=9;
    } else if (j==5){
        H-=12;
    } else if(j<=8){
        H+=1;
    } else {
        H-=2;
    }

    //checks for out-of-bound position
    if(T<0){
        T = 0;
    }
    if(T>69){
        T = 69;
    }
    if(H<0){
        H = 0;
    }
    if(H>69){
        H = 69;
    }

    // If tortoise and hare are in the same position
    if (T==H&&T!=69){
        cPtr[T]='O';
        cPtr[T+1]='U';
        cPtr[T+2]='C';
        cPtr[T+3]='H';
        cPtr[T+4]='!';
        cPtr[T+5]='!';
        cPtr[T+6]='!';
        printPos(cPtr);
        cPtr[T]=' ';
        cPtr[T+1]=' ';
        cPtr[T+2]=' ';
        cPtr[T+3]=' ';
        cPtr[T+4]=' ';
        cPtr[T+5]=' ';
        cPtr[T+6]=' ';
    } else if (T==H&&T==69){ // If hare and tortoise meet at the 70th square, continue
    } else { // Registers and prints out tortoise and hare position
        cPtr[T] = 'T';
        cPtr[H] = 'H';
        printPos(cPtr);
        cPtr[T] = ' ';
        cPtr[H] = ' ';
    }
    }
    if (T==H&&T==69){ // Tie
        puts("It's a tie!");
    } else if (T==69){ // Tortoise wins
        puts("TORTOISE WINS!!! YAY!!!");
    } else { // Hare wins
        puts("Hare wins. Yuch.");
    }
}

// Prints out the array
void printPos(char *cPtr){
    size_t i;
    for(i=0;i<SIZE;i++){
        printf("%c",cPtr[i]);
    }
    puts("");
}
