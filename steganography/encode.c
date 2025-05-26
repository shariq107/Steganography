#include <stdio.h>
#include<string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
Status do_encoding(char* argv[],EncodeInfo *encInfo)
{
  Status ret=read_and_validate_encode_args(argv,encInfo); 
  if(ret!=e_success)
  {
    printf("ERROR : Validation part is not done, And file are not opended.\n\n");
    return e_failure;
  } 
  encInfo->size_secret_file=get_file_size(encInfo->fptr_secret);
  encInfo->image_capacity= get_image_size_for_bmp(encInfo->fptr_src_image);
  //printf("%d*",encInfo->size secret_file);
  //printf("%d*",encInfo->image_capacity);
  Status ret1=check_capacity(encInfo);
  if(ret1!=e_success)
  {
    printf("ERROR : Your encoding size is more tha the source image means not able to encode your data.\n\n");
    return e_failure;
  }
  printf("INFO : You source file have capable of encoding.\n\n");
  Status rett=copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image);
  if(rett!=e_success)
  {
    printf("ERROR : The header are doesn't copied.\n\n");
    return e_failure;
  }
  printf("INFO : The header are copied successfuly.\n\n");
  Status ret2=encode_magic_string(MAGIC_STRING,encInfo);
  if(ret2!=e_success)
  {
    printf("ERROR : Magic string is doesn't encoded.\n\n");
    return e_failure;
  }
  printf("INFO : Magic string encoded successfully.\n\n");
  Status ret3=encode_extn_file_size(encInfo);
  if(ret3!=e_success)
  {
    printf("ERROR : Extension file size is doesn't encoded.\n\n");
    return e_failure;
  }
  printf("INFO : Extension file size encoded successfully.\n\n");
  Status ret4= encode_secret_file_extn(encInfo->extn_secret_file,encInfo);
  if(ret4!=e_success)
  {
    printf("ERROR : Extension doesn't encoded.\n\n");
    return e_failure;
  }
  printf("INFO : The extension encoded successfully.\n\n");
  Status ret5=encode_secret_file_size(encInfo->size_secret_file,encInfo);
  if(ret5!=e_success)
  {
    printf("ERROR : The size of the secret file data doesn't encoded");
    return e_failure;
  }
  printf("INFO : The secret file size encoded successfully.\n\n");
  Status ret6=encode_secret_file_data(encInfo);
  if(ret6!=e_success)
  {
    printf("ERROR : The secret file data doesn't encoded.\n\n");
    return e_failure;
  }
  printf("INFO : The secrete file data encoded successfully\n\n");
  Status ret7=copy_remaining_img_data(encInfo->image_capacity,encInfo->fptr_src_image,encInfo->fptr_stego_image);
  if(ret7!=e_success)
  {
    printf("ERROR : The remaining data doesn't copied.\n\n");
    return e_failure;
  }
  printf("INFO : The remaining data are copied as it is successfully.\n\n");
  return e_success;




}
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image)
{
    char buffer_arr[8];
    while(*data!='\0')
    {
        //printf("--%c--\n",*data);
    fread(buffer_arr,8,1,fptr_src_image);
    for(int i=0;i<8;i++)
    {
    
       //printf("%d",buffer_arr[i]);
        buffer_arr[i]=(buffer_arr[i]&(~1));
        buffer_arr[i]=(buffer_arr[i]|((*data&(1<<(7-i)))>>(7-i)));
        //printf("#%d\n",buffer_arr[i]);
    }
    //printf("-----------------------------------------------\n");
    fwrite(buffer_arr,8,1,fptr_stego_image);
    data++;
    }
    //printf("%ld\n",ftell(fptr_stego_image));
    if(ftell(fptr_stego_image)>54)
    return e_success;
    else
    return e_failure;
    

}
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    //printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    //printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)

{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    // Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    // Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    // Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}
Status check_capacity(EncodeInfo* encInfo)
{
//printf("%d",54+strlen(MAGIC_STRING)*8+sizeof(int)*8+(strlen(".txt")*8)+sizeof(int)*8+encInfo->size_secret_file*8);

if(encInfo->image_capacity>(54+strlen(MAGIC_STRING)*8+sizeof(int)*8+(strlen(".txt")*8)+sizeof(int)*8+encInfo->size_secret_file*8))
return e_success;
else
return e_failure;

}
OperationType check_operation_type(char argv[])
{
    //printf("%d\n%d\n%d\n",e_encode,e_decode,e_unsupported);
    //char arr[3]={"-e"};
    //char arr1[3]={"-d"};
    if(strcmp(argv,"-e")==0)
    {
        return e_encode;
    }
    else if(strcmp(argv,"-d")==0)
    {
        return e_decode;
    }
    else
    {
        return e_unsupported;
    }

}
Status read_and_validate_encode_args(char* argv[],EncodeInfo* encInfo)
{
    if(strstr(argv[2],".bmp")!=NULL)
    {
        encInfo->src_image_fname=argv[2];
    }
    else{
        printf("Your file extention is not .bmp file\n");
        return e_failure;
    }
    if(strstr(argv[3],".txt")!=NULL)
    {
        encInfo->secret_fname=argv[3];
        strcpy(encInfo->extn_secret_file,".txt");
        encInfo->size_secret_file=strlen(".txt");
    }
    else{
        printf("Your file extention is not .txt file\n");
        return e_failure;
    }
    if(argv[4]!=NULL)
    {
        if(strstr(argv[4],".bmp")!=NULL)
        encInfo->stego_image_fname=argv[4];
        else {printf("You Entered optional file extention is not .bmp file\n");
        return e_failure;}
    }
    else{
        encInfo->stego_image_fname="new.bmp";
    }

    Status back=open_files(encInfo);
    return e_success;
    //print error msg
    //return efailure
    //return e_succes
}
uint get_file_size(FILE* fptr)
{
    fseek(fptr,0,SEEK_END);
    return ftell(fptr);
}
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
    char header[54];
    rewind(fptr_src_image);
    fread(header,54,1,fptr_src_image);
    fwrite(header,54,1,fptr_dest_image);
    if(ftell(fptr_dest_image)!=54)
        return e_failure;
        return e_success;
    
    /*rewind(fptr_dest_image);
    if(fseek(fptr_dest_image,1,SEEK_CUR)==0x42&&fseek(fptr_dest_image,1,SEEK_CUR)==0x4d);
    printf("hello");*/

}
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)

{
    int s_mstring=strlen(magic_string);
    Status sus=encode_data_to_image(magic_string,s_mstring*8,encInfo->fptr_src_image,encInfo->fptr_stego_image);
    if(sus==e_success)
    return e_success;
    else
    return e_failure;

}
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo)
{
    int s_estring=strlen(file_extn);
    Status sus=encode_data_to_image(file_extn,s_estring*8,encInfo->fptr_src_image,encInfo->fptr_stego_image);
    if(sus==e_success)
    return e_success;
    else
    return e_failure;
}
Status encode_extn_file_size(EncodeInfo *encInfo)
{
   Status ret= encode_size_to_bmp(strlen(encInfo->extn_secret_file),encInfo->fptr_src_image,encInfo->fptr_stego_image);
   if(ret!=e_success)
   {
    return e_failure;
   }
   return e_success;


}
Status encode_size_to_bmp(int data, FILE* fprt_fname,FILE* fptr_dest_name)
{
    char buffer_arr[32];
    fread(buffer_arr,32,1,fprt_fname);
    for(int i=0;i<32;i++)
    {
        //printf("%d#",buffer_arr[i]);
        buffer_arr[i]=(buffer_arr[i]&(~1));
        buffer_arr[i]=(buffer_arr[i]|((data&(1<<(31-i)))>>(31-i)));
        //printf("%d\n",buffer_arr[i]);

    }
    fwrite(buffer_arr,32,1,fptr_dest_name);
    //printf("%ld\n",ftell(fptr_dest_name));
    if(ftell(fptr_dest_name)<54)
    return e_failure;
    return e_success;

}
Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
    Status ret= encode_size_to_bmp(file_size,encInfo->fptr_src_image,encInfo->fptr_stego_image);
    if(ret!=e_success)
    return e_failure;
    return e_success;
    
}
Status encode_secret_file_data(EncodeInfo *encInfo)
{
char dataarr[encInfo->size_secret_file];
//dataarr[encInfo->size_secret_file]='\0';
rewind(encInfo->fptr_secret);
fread(dataarr,encInfo->size_secret_file,1,encInfo->fptr_secret);
//printf("%ld\n",encInfo->size_secret_file);
dataarr[encInfo->size_secret_file]='\0';
//printf("data ---> %s\n",dataarr);
Status ret=encode_data_to_image(dataarr,encInfo->size_secret_file*8,encInfo->fptr_src_image,encInfo->fptr_stego_image);
if(ret!=e_success)
return e_failure;
return e_success;

}
Status copy_remaining_img_data(int asize,FILE *fptr_src, FILE *fptr_dest)
{
    char ch;
while(ftell(fptr_dest)!=asize)
{
    fread(&ch, 1, 1, fptr_src);
    fwrite(&ch, 1, 1, fptr_dest);
}
return e_success;



  /*  char ch;
    while (fread(&ch, 1, 1, fptr_src)>0)
    {
        fwrite(&ch, 1, 1, fptr_dest);
    }
    return e_success;*/


}
