// Template for parsing an ELF file to print its symbol table. You are
// free to rename any variables that appear below as you see fit.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <elf.h>

int DEBUG = 0;                  // controls whether to print debug messages

int main(int argc, char *argv[]){
  if(argc < 2){
    printf("usage: %s [-d] <file>\n",argv[0]);
    return 0;
  }

  char *objfile_name = argv[1];

  // check for debug mode
  if(argc >=3){
    if(strcmp("-d",argv[1])==0){ // command line arg -d enables debug printing
      DEBUG = 1;
      objfile_name = argv[2];
    }
    else{
      printf("incorrect usage\n");
      return 1;
    }
  }

  // Open file descriptor and set up memory map for objfile_name


  int file = open(objfile_name,O_RDONLY); //opening file reading only

  struct stat stat_buf; // get stats on the open file such as size
  fstat(file, &stat_buf);
  int size=stat_buf.st_size; //size for mmap()'d file

  char *file_chars = //pointer to file contents
    mmap(NULL, size, PROT_READ, MAP_PRIVATE,file,0); //call mmap with given size and read only



  // CREATE A POINTER to the intial bytes of the file which are an ELF64_Ehdr struct
  Elf64_Ehdr *ehdr = (Elf64_Ehdr *) file_chars;
  //binary header struct is first thing in the file
  //check the first bytes to ensure correct file format below

  // CHECK e_ident field's bytes 0 to for for the sequence {0x7f,'E','L','F'}.
  // Exit the program with code 1 if the bytes do not match
  if(ehdr->e_ident[0]==0x7f
      && ehdr->e_ident[1]=='E'
      && ehdr->e_ident[2]=='L'
      && ehdr->e_ident[3]=='F'){
      }
  else{ //if not correct file format, unmap, close, and print message
    munmap(file_chars, size);
    close(file);
    printf("Magic bytes wrong, this is not an ELF file\n");
    return 1;
  }


  // PROVIDED: check for a 64-bit file
  if(ehdr->e_ident[EI_CLASS] != ELFCLASS64){
    printf("Not a 64-bit file ELF file\n");
    return 1;
  }

  // PROVIDED: check for x86-64 architecture
  if(ehdr->e_machine != EM_X86_64){
    printf("Not an x86-64 file\n");
    return 1;
  }

  // DETERMINE THE OFFSET of the Section Header Array (e_shoff), the
  // number of sections (e_shnum), and the index of the Section Header
  // String table (e_shstrndx). These fields are from the ELF File
  // Header.

  long offset=ehdr->e_shoff; //determins offset of section header array
  int sections=ehdr->e_shnum; //number of sections
  int aindex=ehdr->e_shstrndx; //index of section header string table

  // Set up a pointer to the array of section headers. Use the section
  // header string table index to find its byte position in the file
  // and set up a pointer to it.
  Elf64_Shdr *sec_hdrs = (void*)ehdr + offset; //pointer to array of section headerss
  char *sec_names = NULL; //null pointer to the section names, will fill this in later

  //ehdr + sec_hdrs[aindex].sh_offset;


  // Search the Section Header Array for the secion with name .symtab
  // (symbol table) and .strtab (string table).  Note their positions
  // in the file (sh_offset field).  Also note the size in bytes
  // (sh_size) and and the size of each entry (sh_entsize) for .symtab
  // so its number of entries can be computed.
  //initializing variables for symbol and string table offsets
  int symoffset;
  int stroffset;
//initializing variables for symbol table total size and size of each entry
  int symsize;
  int symentsize;
//initializing variables for string table total size and size of each entry
  int strsize;
  int strentsize;
//initializing variables for symbol and string table to be used as boolean values
  int foundsym=0;
  int foundstr=0;


  for(int i=0; i<sections; i++){ //for loop to iterate through the sections
    sec_names=sec_hdrs[i].sh_name+ sec_hdrs[aindex].sh_offset+(void *)ehdr; //determining the offset to find each seciton name
    if(strcmp(sec_names,".symtab")==0){ //if the .symtab section is found
        symoffset=sec_hdrs[i].sh_offset;  //setting the offset value
        symsize=sec_hdrs[i].sh_size;      //setting the size of the symbols
        symentsize=sec_hdrs[i].sh_entsize;  // setting element size of the symbols
        foundsym=1;  //symbol table found
    }
    else if(strcmp(sec_names,".strtab")==0){ //if the .strtab section is found
        stroffset=sec_hdrs[i].sh_offset;     //setting the offset of the string table
        strsize=sec_hdrs[i].sh_size;         //setting the size of the string table
        foundstr=1;  //string table found
    }

  }

  if(foundsym==0){ //if symbol table not found print message
    printf("Couldn't find symbol table\n");
    return 1;
  }

  if(foundstr==0){ //if string table not found print message
    printf("Couldn't find string table\n");
    return 1;
  }

  // PRINT byte information about where the symbol table was found and
  // its sizes. The number of entries in the symbol table can be
  // determined by dividing its total size in bytes by the size of
  // each entry.

  int entries = symsize/symentsize; //determining the size of the entries in the symbol table
  printf("Symbol Table\n");
  printf("- %ld bytes offset from start of file\n",symoffset);
  printf("- %ld bytes total size\n",symsize);
  printf("- %ld bytes per entry\n",symentsize);
  printf("- %ld entries\n",entries);


  // Set up pointers to the Symbol Table and associated String Table
  // using offsets found earlier.

  Elf64_Sym *symptr= (void*) ehdr + symoffset; //points to the symbol table to be used to find entries

  // Print column IDs for info on each symbol
  printf("[%3s]  %8s %4s %s\n",
         "idx","TYPE","SIZE","NAME");

  // Iterate over the symbol table entries
  for(int i=0; i<entries; i++){
    // Determine size of symbol and name. Use <NONE> name has zero
    // length.
    char *name=symptr[i].st_name+(void*)ehdr+stroffset;
    int symsize=symptr[i].st_size;
    // Determine type of symbol. See assignment specification for
    // fields, macros, and definitions related to this.
    unsigned char typec = ELF64_ST_TYPE(symptr[i].st_info); //given code

    if(strlen(name)==0){ //if there is no name
      name="<NONE>";
    }
    if(typec==STT_NOTYPE){
      printf("[%3d]: %8s %4lu %s\n",i,"NOTYPE",symsize,name);
    }
    if(typec==STT_OBJECT){
      printf("[%3d]: %8s %4lu %s\n",i,"OBJECT",symsize,name);
    }
    if(typec==STT_FUNC){
      printf("[%3d]: %8s %4lu %s\n",i,"FUNC",symsize,name);
    }
    if(typec==STT_FILE){
      printf("[%3d]: %8s %4lu %s\n",i,"FILE",symsize,name);
    }
    if(typec==STT_SECTION){
      printf("[%3d]: %8s %4lu %s\n",i,"SECTION",symsize,name);
    }
  }


  // Unmap file from memory and close associated file descriptor
  munmap(file_chars, size); //unmap
  close(file);              //close
  return 0;
}
