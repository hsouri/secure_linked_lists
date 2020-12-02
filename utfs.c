#include "utfs.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
File* searchfile(File* headpointer,char *file_name);													//////////declarating the functions//////////
char derotator(int c, int m,char a,int key);															//////////declarating the functions//////////
int pushnewfile(File* headpointer,char *file_name);														//////////declarating the functions//////////
void popfile(File *ourfile,File *headpointer);															//////////declarating the functions//////////
void pushfullsect(Sector *lastsect,Sector*newsect);														//////////declarating the functions//////////
void pushholesect(Sector **headpointer,Sector**newsect);												//////////declarating the functions//////////
char enrotater(int c, int m,char a,int key);															//////////declarating the functions//////////
int pushnewsect(Sector* headpointer,int i,unsigned char *base,Storage *strg,unsigned int sector_size);	//////////declarating the functions//////////
int init_storage( Storage *strg, unsigned int sector_size, unsigned int disk_size){
	unsigned int i=0;
	int j;
	unsigned char *base=NULL;																			//initialization
	Sector* secbase=(Sector*)malloc(sizeof(Sector));													////////////////
	File* filebase=(File*)malloc(sizeof(File));																////////	
	if(sector_size==0)																				
		return -1;
	if(secbase==NULL){                                                                                  //testing the bugs
		free(filebase);
		return -1;
	}
	if(filebase==NULL){                                                                                 //testing the bugs
		free(secbase);
		return -1;
	}
	if(strg==NULL)                                                                                      //testing the bugs
		return -1;
	filebase->next=NULL;
	secbase->next_sector=NULL;
	strg->secbase=secbase;
	strg->filebase=filebase;
	strg->adrress_of_first=(unsigned char*)malloc(disk_size * sizeof(unsigned char));
	if(strg->adrress_of_first==NULL){                                                                   //testing the malloc
		free(filebase);
		free(secbase);
		return -1;
	}
	strg->disk_size=disk_size;
	strg->sector_size=sector_size;
	base=strg->adrress_of_first;
	for(i=0;i<(disk_size/sector_size);i++){
		j=pushnewsect((strg->secbase),i,base,strg,sector_size);                                  //loop for pushing new sectors
		if(j==-1){
			free(strg->adrress_of_first);
			free(filebase);
			free(secbase);
			return -1;
		}
	}
	return 0;
}
int pushnewsect(Sector* headpointer,int i,unsigned char *base,Storage *strg,unsigned int sector_size){
	Sector* prevnode=headpointer;
	Sector* nextnode=(Sector*)malloc(sizeof(Sector));
	if(nextnode==NULL)																	//testing the error
		return -1;
	if(i==0)
	strg->first_free_sector=nextnode;
	while((prevnode->next_sector)!=NULL)												//using a loop for pushing a new sector
	prevnode=prevnode->next_sector;														//
	prevnode->next_sector=nextnode;														//
	nextnode->next_sector=NULL;															//
	nextnode->adrress_sector=(base+i*sector_size);										//
	nextnode->sectnum=i;																//
	return 0;
}
int store_file(Storage* strg, char *file_name,unsigned char key){
	FILE *fileopenptr;  //defionating file pointer
	Sector * prevsect=(strg->first_free_sector),*temp,*temp2;
	int c,j=0,con=1,errorchecker,number=0,numsect=0;
	int a=pushnewfile((strg->filebase),file_name),counter=0;
	unsigned int i,totalsizei,m;
	unsigned char *ptr;
	if(key==NULL)									//testing the error
		return -1;
	if(strg==NULL)									//testing the error
		return -1;
	fileopenptr=fopen(file_name,"rb");				// open inFileName
	if(!fileopenptr)								//checking wronf file
		return -2;
	fseek(fileopenptr,0,SEEK_END);
	totalsizei=ftell(fileopenptr);				  // calculating total sizeof file
	fseek(fileopenptr,0,SEEK_SET);
	((strg->filebase))->next->sizeoffile=totalsizei;
	number=avail_space(strg);						// using avail_space for space
	if(totalsizei%(strg->sector_size)==0)
		numsect=totalsizei/(strg->sector_size);
	else
		numsect=totalsizei/(strg->sector_size)+1;
	if(number<numsect)									//cheking if we have enough free space
		return -4;
		if (a==-1)
		return -5;
	if(a==-2)
		return -3;
	c=key%2;           //testing the key if it is even or odd
	for(i=0;i<=(totalsizei/(strg->sector_size));i++){
		if(strg->first_free_sector){
		m=(strg->first_free_sector->sectnum)%2;			/* tesing if number of sector is odd or even */
		ptr=strg->first_free_sector->adrress_sector;  
		}
		if(i!=(totalsizei/(strg->sector_size))){
			errorchecker=fread(ptr,sizeof(unsigned char),(strg->sector_size),fileopenptr);  // getting the sector from file
			if(errorchecker==0)                                                    //checking fread function
				return -2;
			for(j=0;j<(strg->sector_size);j++){
				*(ptr+j)=enrotater(c,m,*(ptr+j),key);			//using incryption rotater function
			}
			counter++;
		}
		else{
			con=totalsizei%strg->sector_size;            //encrypting rest of the file
			if(con!=0){                           // check kardane halate marzi
			errorchecker=fread(ptr,sizeof(unsigned char),con,fileopenptr);		// getting the sector from file
			if(errorchecker==0)                                                 //checking fread function
				return -2;
			for(j=0;j<con;j++){
				*(ptr+j)=enrotater(c,m,*(ptr+j),key);			//using encryption rotater function
			}
			counter++;
			}
		}
		if(con){
		temp=strg->first_free_sector;                                        //poping the sector from free sectors link list
		strg->first_free_sector=strg->first_free_sector->next_sector;
		temp2=strg->filebase->next->fist_sec;
		if(temp2==NULL){                                                     //testing the error
			temp->next_sector=NULL;
		strg->filebase->next->fist_sec=temp;
		}
		else{
			while(temp2->next_sector!=NULL)									
			temp2=temp2->next_sector;
		pushfullsect(temp2,temp);										//poshing fullsect into ourfile
		}
		}
	}
	fclose(fileopenptr);  //close the file
	return counter;
}
char enrotater(int c, int m,char a,int key){
	int test,num1,num2;
	unsigned char out;
	test=(c+m)%2;       // testing odd or even
	out=key^a;          // xor the numbers
	if(test==1){                                    //right rotating
		num1=(out%2);								//
		out=out>>1;									//
		num2=num1<<7;								//
		if(num1==1)								//testing the first digit
			out=num2|out;
		else
			out=out&127;
	}
	else{											//left rotating
		num1=out>>7;								//
		num1=num1&1;								//
		out=out<<1;									//
		num2=num1%2;								//
		if(num2==0)								//testing the last digit
			out=out&254;
		else
			out=out|1;
	}
	return out;
}
void pushfullsect(Sector *lastsect,Sector*newsect){
	lastsect->next_sector=newsect;                      //poshing fullsector into ourfile
	newsect->next_sector=NULL;
}
int pushnewfile(File* headpointer,char *file_name){
	int i=0;
	File*newfile=(File*)malloc(sizeof(File)),*prevfile;
	newfile->next=NULL;
	newfile->fist_sec=NULL;
	if(newfile==NULL)											//testing the error
		return -2;
		prevfile=headpointer;
		strcpy(newfile->file_name,file_name);					//copy the filename into File struct
		while(prevfile->next!=NULL){
			prevfile=prevfile->next;
			for(i=0;i<256;i++){                                                         //
				if(prevfile->file_name[i]!=NULL){                                       //check the repeate files
					if(prevfile->file_name[i]!=newfile->file_name[i]){                  //
						break;															//check the repeate files
					}
					continue;
				}
				else if(prevfile->file_name[i]==newfile->file_name[i]){                 // free newfile if it is reapitive
					free(newfile);
					return -1;
				}
			}
		}
		newfile->next=headpointer->next;                                                //pop the file into link list
		headpointer->next=newfile;
		return 0;
}
int retrieve_file(Storage* strg, char *file_name,unsigned char key){
	FILE * fileoutptr;
	File * ourfile;
	Sector * oursect;
	int i,c,m,j=0,con;
	unsigned char *ptr;
	fileoutptr=fopen(file_name,"wb");           // open outFileName
	if(key==NULL)								//check the repeate files
		return -1;
	if(fileoutptr==NULL)						//check the repeate files
		return -2;
	if(strg==NULL)								//check the repeate files
		return -1;
	ourfile=searchfile((strg->filebase),file_name);
	if(ourfile==NULL)
		return -4;
	c=key%2;           //testing the key if it is even or odd
	oursect=ourfile->fist_sec;
	for(i=0;i<=(ourfile->sizeoffile)/(strg->sector_size);i++){
		ptr=(unsigned char*)malloc((strg->sector_size)*sizeof(unsigned char));   // memory allocation for each sector
		if(ptr==NULL)    // checking the malloc functio
			return -3;
		if(oursect)
		m=(oursect->sectnum)%2;			/* tesing if number of sector is odd or even */
		if(i!=((ourfile->sizeoffile)/(strg->sector_size))){
			for(j=0;j<strg->sector_size;j++){
				*(ptr+j)=*(oursect->adrress_sector+j);
				*(ptr+j)=derotator(c,m,*(ptr+j),key);				//using decryption rotater function
				}			 
			fwrite(ptr,strg->sector_size,sizeof(unsigned char),fileoutptr);			// writing new character to outFile			
		}
		else{
			con=ourfile->sizeoffile%strg->sector_size;            //decrypting rest of the file
			if(con!=0){                           // check kardane halate marzi
			for(j=0;j<con;j++){
				*(ptr+j)=*(oursect->adrress_sector+j);
				*(ptr+j)=derotator(c,m,*(ptr+j),key);				//using decryption rotater function
			}
				fwrite(ptr,con,sizeof(unsigned char),fileoutptr);			// writing new character to outFile	
			}
		}
		free(ptr);                           //releaze the memory allocation
		if(oursect)
		oursect=oursect->next_sector;
	}
	fclose(fileoutptr);                      //close the file
	return(ourfile->sizeoffile);
}
File* searchfile(File* headpointer,char *file_name){
	File * prevfile,*newfile;
	char arr[256];
	int i;
	prevfile=headpointer;
	strcpy(arr,file_name);
	while(prevfile->next!=NULL){                                  //a loop for checking whole files
		for(i=0;i<256;i++){
				if(prevfile->next->file_name[i]!=NULL){           // search for ourfile
					if(prevfile->next->file_name[i]!=arr[i])
						break;
				}
				else if(prevfile->next->file_name[i]!=arr[i])		// checking the NULL
					break;
				else{
				newfile=prevfile->next;                             //next file
				return newfile;
				}
		}
		prevfile=prevfile->next;									// updating the file
	}
	return NULL;
}
char derotator(int c, int m,char a,int key){
	int test,num1,num2;
	unsigned char out;
	test=(c+m)%2;			// testing odd or even
	out=a;
	if(test==0){					//right rotating
		num1=(out%2);				//
		out=out>>1;					//
		num2=num1<<7;				//
		if(num1==1)								//testing the first digit
			out=num2|out;
		else
			out=out&127;
	}
	else{							//left rotating
		num1=out>>7;				//
		num1=num1&1;				//
		out=out<<1;					//
		num2=num1%2;				//
		if(num2==0)								//testing the first digit
			out=out&254;
		else
			out=out|1;
	}
	out=out^key;			// xor the numbers
	return out;
}
int print_file_sequence(Storage *strg, char *file_name){
	File * ourfile;
	Sector * oursect;
	ourfile=searchfile((strg->filebase),file_name);           //finding the file with using searchfile function
	if(ourfile==NULL)											//testing errors
		return -2;
	if(strg==NULL)												//testing errors
		return -1;
	oursect=ourfile->fist_sec;
	while(oursect!=NULL){
		printf("%d ,",oursect->sectnum);						// printing the sectors in console
		oursect=oursect->next_sector;
	}
	return 0;
}
int delete_file(Storage *strg, char *file_name){
	File * ourfile;
	Sector * oursect,*temp;
	int counter=0;
	ourfile=searchfile((strg->filebase),file_name);					//finding the file with using searchfile function
	if(ourfile==NULL)												//testing errors
		return -2;
	if(strg==NULL)													//testing errors
		return -1;
	if(ourfile->fist_sec!=NULL){
	do{
	oursect=ourfile->fist_sec;											//														//
	temp=oursect->next_sector;											//														//
	pushholesect(&(strg->first_free_sector),&(oursect));				//	pushing all sectors of our file into free sectors	//
	ourfile->fist_sec=temp;												//														//
	counter++;															//														//
	}while(temp!=NULL);
	}
	popfile(ourfile,(strg->filebase));                                  // POP OUR FILE FROM FILEBASE//
	return counter;
}
void pushholesect(Sector **headpointer,Sector**newsect){
	(*newsect)->next_sector=(*headpointer);									////// pushing hole sects into free sectors /////
	*headpointer=*newsect;
}
void popfile(File *ourfile,File *headpointer){
	File *prevfile;
	prevfile=headpointer;
	while(prevfile->next!=ourfile)					// seach for ourfile pointer//
		prevfile=prevfile->next;
	prevfile->next=ourfile->next;					// pop it ////////////////////
	free(ourfile);
}
int avail_space(Storage *strg){
	int counter=0;
	Sector * oursect;
	oursect=strg->first_free_sector;
	if(strg==NULL)										//testing errors//
	return -1;
	while(oursect!=NULL){                               // counter plus one for each loop//
		counter++;
		oursect=oursect->next_sector;					// updating oursect//
	}
	counter=counter*(strg->sector_size);                //calculating avalable space
	return counter;
}
int list_dir(Storage *strg, char *path){
	File * prevsect; 
	if(strg==NULL)								//testing errors	
		return -1;
	path=NULL;
	prevsect=strg->filebase->next;
	while(prevsect!=NULL){
		printf("\n%s\n",prevsect->file_name);		//printng all files in file base
		prevsect=prevsect->next;					// updating our prevsect
	}
	return 0;
}	
int dump_storage( Storage *strg, char* dname){
	FILE * fileoutptr;
	if(strg==NULL)										//testing errors
		return -1;
	fileoutptr=fopen(dname,"wb");
	if(fileoutptr==NULL)								//testing errors
		return -1;
	fwrite(strg->adrress_of_first,strg->disk_size,sizeof(unsigned char),fileoutptr);		// type whole storage into file that given from user
	fclose(fileoutptr);																		// close the file
	return 0;
}
void free_storage(Storage *strg){
	File* prevfile=(strg->filebase->next),*tempfile=NULL;
	Sector *prevsect,*tempsect;
	while(prevfile!=NULL){                       // free each file from file base
		tempfile=prevfile->next;
		prevsect=prevfile->fist_sec;			// updating the prevsect
		while(prevsect!=NULL){
			tempsect=prevsect->next_sector;			// free each sector from each file
			free(prevsect);
			prevsect=tempsect;					// updating the prevsect
		}
		free(prevfile);
		prevfile=tempfile;						// updating prevfile
	}
	prevsect=strg->first_free_sector;
	while(prevsect!=NULL){
		tempsect=prevsect->next_sector;				// free each free sector from first free sector link list
		free(prevsect);
		prevsect=tempsect;
	}
	free(strg->secbase);								// free headpointers
	free(strg->filebase);								// free headpointers
	free(strg->adrress_of_first);						// free headpointers
}
