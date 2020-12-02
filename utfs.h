#ifndef __UTFS__
#define __UTFS__

//Uncomment following macro if you paln on using Tree-Structured directory
//#define _LEVEL_3

typedef struct sector Sector;
typedef struct file File;
typedef struct storage Storage;

struct sector {
	int sectnum;
	struct sector * next_sector;
	unsigned char * adrress_sector;
};
struct file{
	char file_name[256];
	Sector * fist_sec;
	unsigned int sizeoffile;
	struct file * next;
};
struct storage {
	unsigned int disk_size;
	unsigned int sector_size;
	unsigned char * adrress_of_first;
	Sector * first_free_sector;
	Sector* secbase;
	File *filebase;
};


//Intializes strg, given other arguments. disk_size is not necessarily a submultiple of sector_size.
int init_storage( Storage *strg, unsigned int sector_size, unsigned int disk_size);

//Frees memory allocated from heap for strg usage to prevent memory leak.
void free_storage( Storage *strg);

//file operations
//Stores file [file_name] from disk into strg using key for encryption
int store_file( Storage *strg, char* file_name, unsigned char key);

//Retrives file [file_name] from strg into disk using key for decryption.
int retrieve_file( Storage *strg, char* file_name, unsigned char key);

//Prints sequence of sector numbers assigned to file [file_name]
int print_file_sequence( Storage *strg, char* file_name);

//Deletes file from storage
int delete_file( Storage *strg, char* file_name);

//Returns free space of strg in bytes
int avail_space( Storage* strg );

//List files and sub-folders of the specified path in strg. path is relative to working directory of strg
int list_dir( Storage* strg, char* path);

//Dumps allocated memory space for file storage in strg into disk. Output file is dname.
//Note that this function writes raw bytes in output file, no decryption needed.
int dump_storage( Storage *strg, char* dname);

#if defined _LEVEL_3

//Changes working directory of strg. path is relevant to current working directory.
int change_dir( Storage *strg, char* path);

//Creates new directory [dir_name] in working directory of strg.
int make_dir( Storage *strg, char* dir_name);

#endif

#endif
