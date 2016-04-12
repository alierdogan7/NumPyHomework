#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "salloc.h"

int main(int argc, char *argv[])
{
	int ret;
	int size;
	void *x1, *x2, *x3;	// object pointers

	if (argc != 2) {
            printf("usage: app <size in KB>\n");
            exit(1);
	}
        else if ( (size = atoi(argv[1])) < 32 || size > 32768 )
        {
            printf("size can be between 32 KB and 32768 KB (32MB) \n");
            exit(1);
        }

        printf("screate with size: %d\n", size);
	//ret =  s_create (size * 1024); 
	ret =  s_create ( 1000 ); 
        if (ret == -1) {
		printf ("could not create segment\n"); 
		exit (1); 
	}

	/*x1 = s_alloc(26);
        char *cptr = (char *) x1;
        int i, j;
	for (j = 0, i = 97; j < 26; ++i, ++j)
		cptr[j] = i;
        
        x2 = s_alloc(4);
	for (j = 26, i = 97; j < 30; ++i, ++j)
		cptr[j] = i;
        
        for (i = 0; i < 30; i++)
            putchar(cptr[i]);*/
	//x2 = s_alloc(4500);
	//x3 = s_alloc(1300);

        x1 = s_alloc(400);
        x2 = s_alloc(372);
        x3 = s_alloc(193);
        x1 = s_alloc(193);
        
	//s_free(x1);
	//s_free(x2);
	//s_free(x3);


	return 0;
}