#include "types.h"
#include "decode.h"
#include "common.h"

Status do_decodeing(char* argv[],Decode* decInfo)
{
  Status ret1= read_then_valiate_args(argv,decInfo);
  if(ret1!=e_success)
{
    printf("!The validation part have some issues!.\n");
    return e_failure;
}
printf("Validation part is done going further....\n");
Status ret2= open_file(decInfo);
if(ret2!=e_success)
{
    printf("File is not opened");
    return e_failure;
}
printf("File is opened successfully going further....\n");
Status ret3= decode_magic_string(decInfo);
if(ret3!= e_success)
{
    printf("Magic string doesn't match not able to decode further.\n");
    return e_failure;
}
printf("Magic string decoded successfully.\n");
Status ret4=decode_extn_size(decInfo);
if(ret4!=e_success)
{
    printf("Extension size not decoded successfully.\n");
    return e_failure;
}
printf("Extension size decoded successfully.\n");
Status ret5=decode_extn(decInfo);
if(ret5!=e_success)
{
    printf("Extension not decoded successfully.\n");
    return e_failure;
}
//printf("opened--%s\n",decInfo->out_file_name);
/*decInfo->fptr_out_fname=fopen(decInfo->out_file_name,"w");
if(decInfo->fptr_out_fname==NULL)
{
    printf("Still the output file is not opened, Need to check the extention decoding part.\n");
}*/
printf("Extention portin decoded successfully.\n");
Status ret6=decode_file_size(decInfo);
if(ret6!=e_success)
{
    printf("File size is not decoded.\n");
    return e_failure;
}
printf("File size decoded successfully.\n");
Status ret7=decode_file_data(decInfo);
if (ret7!=e_success)
{
    printf("File data not decoded.\n");
    return e_failure;
}
printf("The message are decoded successfully and stored!.\n");
return e_success;
}


Status read_then_valiate_args(char* argv[],Decode* decInfo)
{
    if(strstr(argv[2],".bmp")!=NULL)
    {
        decInfo->inp_image_name=argv[2];
    }
    if(strcmp(decInfo->inp_image_name,argv[2])!=0)
    {
        return e_failure;
    }
    if(argv[3]!=NULL)
    {
       strcpy( decInfo->out_file_name ,argv[3]);
    }
    else
    {
      strcpy(decInfo->out_file_name,"Message");  
    }
    //printf("%s",decInfo->inp_image_name);
    //printf("%s",decInfo->out_file_name);
    /*if((strcmp(decInfo->out_file_name,argv[3]))!=0||(strcmp(decInfo->out_file_name,"Message"))!=0)
    {
        return e_failure;
    }
   printf("%s",decInfo->out_file_name);*/
    return e_success;


}
Status open_file(Decode *decInfo)
{
decInfo->fptr_inp_image=fopen(decInfo->inp_image_name,"r");
if(decInfo->fptr_inp_image==NULL)
{
    printf("ERROR: Not able to open the file");
    return e_failure;
}
return e_success;
}
Status decode_magic_string(Decode* decInfo)
{
    char ma_arr[100];
    __fpurge(stdin);
    printf("Please enter the Magic string: \n");
    scanf("%[^\n]",ma_arr);
  fseek(decInfo->fptr_inp_image,54,SEEK_SET);
  //printf("%ln",ftell(decInfo->fptr_inp_image)); 
  if(ftell(decInfo->fptr_inp_image)!=54)
  {
    printf("Decoding in file have some proplem");
    return e_failure;
  }
  if((strcmp(ma_arr,MAGIC_STRING))!=0)
  {
    printf("Enter Magic string is not matched.\n");
    return e_failure;
  }
  Status decmag=decode_image_to_data(ma_arr,strlen(ma_arr),decInfo);
  printf("%s\n",ma_arr);
  if((strcmp(ma_arr,MAGIC_STRING))!=0)
  {
    printf("Encoded Magic string is not matched.\n");
    return e_failure;
  }
  return e_success;
  
   
}
Status decode_extn_size(Decode* decInfo)
{
 decInfo->ext_size=decode_image_to_int(decInfo);
 if(decInfo->ext_size<=0)
 {
    printf("Decoding of the extension size is not decoded");
    return e_failure;
 }
 return e_success;
}
Status decode_extn(Decode* decInfo)
{
 char buffer_ext[100];
 Status r_dat= decode_image_to_data(decInfo->ext_name,decInfo->ext_size,decInfo);
 //printf("%s",decInfo->ext_name);
 if(r_dat!=e_success)
 {
    printf("Extention doesn't decoded properly.\n");
    return e_failure;
 }
 //strcpy(buffer_ext,decInfo->out_file_name);
 strcat(decInfo->out_file_name,decInfo->ext_name);
 printf("%s\n",decInfo->out_file_name);

 decInfo->fptr_out_fname=fopen(decInfo->out_file_name,"w");
if(decInfo->fptr_out_fname==NULL)
{
    printf("Still the output file is not opened, Need to check the extention decoding part.\n");
}
 printf("After open..%s\n",decInfo->out_file_name);
 return e_success;

}
Status decode_file_size(Decode* decInfo)
{
decInfo->extn_fsize=decode_image_to_int(decInfo);
//printf("%d",decInfo->extn_fsize);
if(decInfo->extn_fsize<0)
{
    printf("File size is not docoded properly");
    return e_failure;
}
return e_success;
}
Status decode_file_data(Decode* decInfo)
{
   char buff[decInfo->extn_fsize];
   Status r_ret=decode_image_to_data(buff,decInfo->extn_fsize,decInfo);
   printf("%s",buff);
   if(r_ret!=e_success)
   {
    printf("Decoding of file data is not done successfully");
    return e_failure;
   }
   fwrite(buff,decInfo->extn_fsize,1,decInfo->fptr_out_fname);
   fclose(decInfo->fptr_inp_image);
   fclose(decInfo->fptr_out_fname);
   return e_success;
}
Status decode_image_to_data(char* data,int n_times,Decode* decInfo)
{
    char var;
    char temp;
    //data[strlen(data)]='\0';
    for(int n=0;n<n_times;n++)  
    {
        var='\0';
        for(int i=0;i<8;i++)
        {
            fread(&temp,1,1,decInfo->fptr_inp_image);
            var=var|((temp&1)<<(7-i));
        }
        *data=var;
        data++;    
    }
    data--;
    //printf("-----------------%c\n",var);
    if(*data!=var)
    return e_failure;
    else
    return e_success;


}
uint decode_image_to_int(Decode* decInfo)
{
int var=0;
char temp;
for(int i=0;i<sizeof(int)*8;i++)
{
    fread(&temp,1,1,decInfo->fptr_inp_image);
    var=var|((temp&1)<<(31-i));
}
//printf("%d\n",var);
return var;
}
