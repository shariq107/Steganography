#ifndef DECODE_H
#define DECODE_H

#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>
#include "types.h"

#define MAX_EXTN 10

typedef struct Decode_prop
{
/*stego image file*/
char* inp_image_name;
FILE* fptr_inp_image;



/*output file data*/
char out_file_name[30];
FILE* fptr_out_fname;
char ext_name[MAX_EXTN];
int ext_size;
int extn_fsize;

/*End of structure*/

}Decode;

/*Function decleration part*/
/*Read and validate*/
Status read_then_valiate_args(char* argv[],Decode* decInfo);

/*do decoding function from here call every function*/
Status do_decodeing(char* argv[],Decode* decInfo);

/*open the encoded file*/
Status open_file(Decode* decInfo);

/*Magic string decoding*/
Status decode_magic_string(Decode *decInfo);

/*Entension size decoding*/
Status decode_extn_size(Decode* decInfo);

/*Extension decoding*/
Status decode_extn(Decode* decInfo);

/*file size decoding*/
Status decode_file_size(Decode* decInfo);

/*file data decoding*/
Status decode_file_data(Decode* decInfo);

/*generic decoding function for character*/
Status decode_image_to_data(char* data,int n_times,Decode* decInfo);

/*generic decoding function for integer*/
Status decode_image_to_int(Decode* decInfo);





#endif

