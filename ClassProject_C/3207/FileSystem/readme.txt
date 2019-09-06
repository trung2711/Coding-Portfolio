Trung Viet Nguyen
CIS 3207
File System Documentation (Updated for the final Version)

Description: This project is a simple implementation of the linux file system. I have added extensive function testing to the main function to display all of the program's functionality. Comments are also provided in the code.

************************************************************************************UPDATE***********************************************************************************************

To create, open, delete file, include the path and file name in the functions' parameters. Name must have a '/' at the beginning (e.g. "/testingDir/myFile.txt").

In my main function, I have included testings for all the functions. Feel free to experiment with the functions!

My metadata entry struct for the final version:
typedef struct {
    char name[MAXLEN]; //max length is 20
    int startBlock;
    int fileSize;
    int isDir;
    char timeCreated[9];
    char dateCreated[9];
    char timeModified[9];
    char dateModified[9];
    int isOpen;
} entry;

List of functions in the final version:
entry *getEntry(char *, fileDrive *); 		//Get metadata of file
int createF (char *, int, fileDrive *); 	//Create file
int writeF (fP*, char*, int, fileDrive *); 	//Write to file
int readF (char *, fP *, int , fileDrive *); 	//Read from file
fP * openF(char *, fileDrive *); 		//Open file
void closeF (fP *); 				//Close file pointer
int deleteF (char *, fileDrive *);		//Delete file
void rewindF (fP *);				//Rewind file pointer
int allocateBlock(fileDrive *);			//Allocate empty block for files (or directories)
void initializeDrive(fileDrive *);		//Format the drive
int createDir(char *, fileDrive *);		//Create directory (a wrapper function of createF)
void getTime(char *);				//Get current time
void getDate(char *);				//Get current date	
void printEntryInfo(entry *);			//Print metadata of file
void printPointerInfo(fP *);			//Same as printEntryInfo but used for file pointers

*****************************************************************************************************************************************************************************************

I. Directory Structure
	a. File Allocation:  
	I will implement my File Allocation Structure using the FAT model. My drive will consist of 2 arrays, one for the block numbers and another for the memory blocks themselves. Each
block will contain either the content of a file or a list of entries to other file. The entry will contain the file (or directory) name, and meta data of the file (or directory). The maximum
size of each block is 512 bytes. The size of the entry will be the size of a predefined C struct. An example of the implemented meta data entry would be:
struct entry{
    char name[20]; //Name of file
    int startBlock; //Mark the starting block of the file 
    int fileSize; //size of file
    int type; //0 for file, 1 for directory
    int isOpen; //0 if file is not being opened. 1 if it is being opened.
}

	b. Physical Allocation:
	My file system will contain about 20000 blocks of memory. Along with the table of the block numbers, the drive size is about 10 MB. The two table of blocks and blocks number are the main structure of my drive.
	The storage units are the blocks, which will each has a maximum size of 512 bytes and contain either the content of a file or a list of entries to other file (as in the case of directories). This is because an entry might
take up less memory than actual file contents (as described by the struct above).
	The storage units on the disk will be addressed based on the FAT model. In the block number table, an element at index i can either be 0 (free), or pointing to another block number (occupied).
	This implementation allows for storing non-contiguous files and protects integrity of the blocks. If a block is being used by one file, it should not be touched by other files, and the block numbers of other files cannot reference to
the used block's number.

II. Implementation
	Here are the list of functions that I plan to implement for my file system. Before executing these functions, I will use mmap() to map the whole drive into memory. After finish executing the
functions, I will use munmap() to unmap the drive.
	1. Open file: filePtr *openFile(char *path, drive *D);
	The open file function will return a pointer to a file descriptor that represents the file or directory specified by path, or will return NULL if the file or directory does not exist.
	I will implement my own file pointer. It will probably look like this struct:
	typedef struct {
    		entry *metaData; //entry containing metadata of the file
    		int offset; //Offset
    		int current; //mark the block currently being accessed in the file
	} filePointer;
	
	2. Close file: closeFile(filePointer *);
	This will free up the pointer and change the isOpen in the file's meta data back to 0
	
	3. Create file: createFile(char * pathAndName, int type, drive * D);
	My create file function will creates a new file with the specified directory path and returns a pointer to a handle to that file, or return NULL if the file already exists. The file will
be written to one or more blocks if needed (if there are still free blocks available).

	4. Create Directory:
	To create directory, I will use the create file function and change the type to directory type (1). This is because in my file system, a directory is treated mostly the same as a file.
This is only my proposed solution. If needed, I will make a separate function for creating directory, or even make a new struct to define directories.

	5. Delete file: void deleteFile(char *, drive *);
	To delete file, I will make use of the memset() function. Before deleting a file, I will check if it exists and is not currently being opened. If one of the two conditions is not correct,
the function will do nothing. To delete directories, I will check if it is containing other files. A directory can only be deleted if it is empty.

	6. Get meta data:
	I plan to use this function to help with opening, reading, and writing files, by accessing the entries of the files themselves. This functions will be a helper function for the above functions.

	7. Read & Write file:
	These functions will probably take in parameters such as the file pointer, the drive pointer, and integer for the length of byte read/write, and the writing content (if writing), or the reading destination (if reading).
	Before reading & writing, I will have to check if the file exists, is not a directory, and is not currently being opened in another pointer.
	
	void readF(char * buf, filePointer * fptr, int len, drive * D);
	void writeF(filePointer * fptr, char * content, int len, drive * D);