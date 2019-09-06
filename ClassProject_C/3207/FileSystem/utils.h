#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>

#ifndef UTILS_UTILS_H
#define UTILS_UTILS_H

#define BSIZE 512
#define NUMB 256
#define MAXLEN 20

typedef struct {
    char name[MAXLEN];
    int startBlock;
    int fileSize;
    int isDir;
    char timeCreated[9];
    char dateCreated[9];
    char timeModified[9];
    char dateModified[9];
    int isOpen;
} entry; //File (directory) metadata entry

entry NULLENTRY = {0}; //Use for error checking
entry ROOT = {
	.isDir = 1,
        .startBlock = 0,
}; //Root entry

typedef union {
    char fileContent[BSIZE]; //content of file
    entry dir[BSIZE / sizeof(entry)]; //Alternatively, a block can also hold multiple entries
} block;

typedef struct {
    int FAT[NUMB];
    block data[NUMB];
} fileDrive; //The drive consists of 256 blocks and an array to keep track of them

typedef struct {
    entry *meta; //Meta data of file
    int ptr; //Current position within the current block
    int currentBlock; //Current block of file
} fP; //File Pointer

entry *getEntry(char *, fileDrive *);
int createF (char *, int, fileDrive *);
int writeF (fP*, char*, int, fileDrive *);
int readF (char *, fP *, int , fileDrive *);
fP * openF(char *, fileDrive *);
void closeF (fP *);
int deleteF (char *, fileDrive *);
void rewindF (fP *);
int allocateBlock(fileDrive *);
void initializeDrive(fileDrive *);
int createDir(char *, fileDrive *);
void getTime(char *);
void getDate(char *);
void printEntryInfo(entry *);
void printPointerInfo(fP *);

#endif
