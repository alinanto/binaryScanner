#include <stdio.h>
#include <stdlib.h>
#define BYTES_PER_LINE 16

unsigned long int findSize(FILE *file);
void *loadFile(FILE *file,unsigned long int fileSize);
void binToHexString(char binary, char *hexString);

int main(int argc,char **argv)
{
    if(argc < 2 || argc >3)
    {
        printf("This program accepts 2 arguments.\n");
        printf("binScanner.exe <path1> [<path2>]\n");
        printf("<path1> : Path to input Binary file\n");
        printf("<path2> : (Optional) Path to output text file.\n");
        printf("          if file at <path2> does not exist, it will be created.\n");
        printf("          if file at <path2> exists, it will be replaced.\n");
        return 0;
    }
    FILE *file;
    void *fileBuffer;
    unsigned long int i=0;
    char hexString[3];
    unsigned long int fileSize=0;

    //open file,find file size and load file to dynamic array then close file
    file=fopen(argv[1],"rb");
    if(file == NULL)
    {
        printf("Failed to open file %s!\n",argv[1]);
        return 1;
    }
    fileSize=findSize(file);
    fileBuffer=loadFile(file,fileSize);
    if(fileBuffer == NULL)
    {
        printf("Failed to load file %s!\n",argv[1]);
        return 1;
    }
    fclose(file);

    if(argc == 3)
    {
        file=fopen(argv[2],"w");
        if(file == NULL)
        {
        printf("Failed to create file %s!\n",argv[2]);
        return 1;
        }
        fprintf(file,"Size of input file %s is %d bytes.\n",argv[1],fileSize);
    }

    for(i=0;i<=(BYTES_PER_LINE+2)*4;i++)
    {
        printf("-");
        if(argc == 3)   fprintf(file,"-");
    }
    //scanning through file
    for(i=0;i<fileSize;i++)
    {
        if(!(i%BYTES_PER_LINE))
        {
            unsigned char highWord;
            unsigned char lowWord;
            char highString[3];
            char lowString[3];
            lowWord = i&0x00FF;
            highWord = (i&0xFF00)>>8;
            binToHexString(highWord,highString);
            binToHexString(lowWord,lowString);
            printf("\n %s%sH : ",highString,lowString);
            if(argc == 3)   fprintf(file,"\n %s%sH : ",highString,lowString);
        }
        binToHexString(*((char *)fileBuffer+i),hexString);
        printf("%sH ",hexString);
        if(argc == 3)   fprintf(file,"%sH ",hexString);
    }
    printf("\n");
    if(argc == 3)   fprintf(file,"\n");
    for(i=0;i<=(BYTES_PER_LINE+2)*4;i++)
    {
        printf("-");
        if(argc == 3)   fprintf(file,"-");
    }
    printf("\nFile ends here.\n");
    if(argc == 3)   fprintf(file,"\nFile ends here.\n");
    fclose(file);
    free(fileBuffer);
    return 0;
}

unsigned long int findSize(FILE *file)
{
    if(file == NULL)
        return 0;
    unsigned long int  fileSize;
    //finding size of file
    fseek(file,0,SEEK_END);
    fileSize = ftell(file);
    rewind(file);
    printf("Size of file = %d bytes.\n",fileSize);
    return fileSize;
}

void *loadFile(FILE *file,unsigned long int fileSize)
{
    //dynamic allocation of fileBuffer
    void *fileBuffer;
    fileBuffer = malloc(fileSize);
    if (!fileBuffer)
	{
        printf("Memory allocation failed!\n");
        return NULL;
    }
    //reading file and store it in an dynamic string
    long unsigned int readSize;
    readSize = fread(fileBuffer,1,fileSize,file);
    if (readSize != fileSize)
	{
        printf("Unable to read complete file!\n");
        printf("Read only %d bytes...\n",readSize);
        return NULL;
	}
	printf("Reading success.\n");
    return fileBuffer;
}

void binToHexString(char binary, char *hexString)
{
    char hexLookUp[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    unsigned char lowerNibble=0;
    unsigned char upperNibble=0;
    lowerNibble=binary&0x0F;
    upperNibble=(binary&0xF0)>>4;
    hexString[0]=hexLookUp[upperNibble];
    hexString[1]=hexLookUp[lowerNibble];
    hexString[2]='\0';
}
