#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"
#include "common.h"

int main(int argc,char* cmline[])
{
    Status s_encode,s_decode;
    /*decleare the variable for Structure to acces the structure variable.*/
    EncodeInfo encInfo;
    Decode dencInfo;
    /*checking the command line argument that user tells do encode or do decode.*/
    OperationType ret= check_operation_type(cmline[1]);
    //printf("%d\n",ret);
    if(ret==e_encode)
    {
printf("Now you are going to do the encoding.\n");
        s_encode=do_encoding(cmline,&encInfo);
        if(s_encode!=e_success )
        {
            printf("You have some issue in your encoding part.\n");
            return e_failure;
        }
        else
        printf("Done your encoding part sucessfully!.\n");
        
    }
    if(ret==e_decode)
    {
        s_decode= do_decodeing(cmline,&dencInfo);
        if(s_decode!=e_success)
        {
            printf("Decoding part is not done properly have to check.\n");
            return e_failure;
        }
        else
        printf("Done your Decoding part sucessfully!.\n");
    }

    
    if(ret==e_unsupported)
    {
            printf("Enter commend line is wrong enter""-e""or""-d""\n");
            return e_failure;
    }
   
    
    
    return e_success;

    
}



