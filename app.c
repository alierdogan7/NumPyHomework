#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "salloc.h"

int main(int argc, char *argv[])
{
	int ret;
	int size;
	//void *x1, *x2, *x3;	// object pointers

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
	ret =  s_create (size * 1024); 
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

        int i;
        void *blocks[10];
        for(i = 0; i < 20; ++i)
        {
            blocks[i] = s_alloc(1600 + i*i);
        }
        
        /*
        x1 = s_alloc(400);
        x2 = s_alloc(72);
        x3 = s_alloc(193);
        void *x4 = s_alloc(200);
        s_alloc(100);*/
        
	s_free(blocks[1]);
	s_free(blocks[3]);
	s_free(blocks[4]);
	s_free(blocks[11]);
	s_free(blocks[13]);
	s_free(blocks[15]);
        s_free(blocks[0]);
        s_print();
        
        /* //UNIT TEST OF IS_HOLE FCN.
         * for(i = 0; i < 16; i++)
            printf("is %lx a hole? %d\n", (long) blocks[i] - 4,is_hole(blocks[i] - 4));*/
        
        /*printf("hole %lx's prev. neighbor is %lx and next neighbor is %lx\n",
                (long) blocks[4] - 4, 
                find_neighbor_hole_from_down(blocks[4] - 4),
                find_neighbor_hole_from_up(blocks[4] - 4) );*/
        
        /*s_alloc(1630);
        s_alloc(1770);
        s_alloc(2000);
        s_alloc(13000);
        s_alloc(1000);
        s_alloc(500);
        
        s_print();*/
        /*printf("closest to x3: %lx, x1: %lx\n",
                find_closest_hole_from_back(x3),
                find_closest_hole_from_back(x1) );*/
	//s_free(x2);
	//s_free(x3);


        
}