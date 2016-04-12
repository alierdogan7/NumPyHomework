#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


/* Your Library Global Variables */
void *segmentptr; // points to the start of memory segment to manage
int segmentsize;  // size of segment to manage
//You can define more variables if you wish

void *curr_hole; // to link the holes with each other
void *head_hole;

void set_params_of_curr_hole( long next_hole_add, int hole_size);
int get_size_of( void *hole);
void *get_next_hole_of( void *hole);
void set_next_hole_of( void *hole, long next_hole_add);
void set_size_of_hole( void *hole, int size);
void set_size_of_allocated_block( void *block, int size);
int get_size_of_allocated_block( void *block);
void *find_closest_hole_from_back(void *deallocated_block);

int s_create (int size)
{
	int i; 
	void *endptr; 
	char *cptr;
	void *vp; 
        
	segmentptr = sbrk(0);     // end of data segment
        segmentsize = size;
        
        
        //printf("sizeof=> void pointer: %d, int: %d\n", sizeof(curr_hole), sizeof(hole_size));
       // printf("size of long: %d, long int: %d, int: %d\n", sizeof(long), sizeof(long int), sizeof(int));
       //printf( "curr hole: %lx, next_hole: %lx, size:%d\n", curr_hole, next_hole, size);
	//printf("sizeof void*: %d, long: %d\n", sizeof(void*), sizeof(long)); //the result was 8 and 8
        
	vp = sbrk(size); // extend data segment by indicated amount       
	if (vp == ((void *)-1)) {
		printf ("segment creation failed\n"); 
		return (-1); 
	}
	
         /*
       gpointer p;
       int i;
       p = (void*) (long) 42;
       i = (int) (long) p;
         */
        
        curr_hole = segmentptr;
        head_hole = segmentptr;
        set_params_of_curr_hole( (long)curr_hole, segmentsize); //inital hole points to itself as the next hole
        
	endptr = sbrk(0); 
	
	printf("segmentstart=%lx, segmentend=%lx, segmentsize=%lu bytes\n",
	       (unsigned long)segmentptr,
	       (unsigned long)endptr, (unsigned long)(endptr - segmentptr));
	
	//test the segment                                                  
	printf("---starting testing segment\n");
	/*cptr = (char *)segmentptr;
	for (i = 0; i < size; ++i)
		cptr[i] = 0;*/
        
        long nextadd = (long) get_next_hole_of(curr_hole);// long nextadd = *((long*)curr_hole);
        int sz = get_size_of_hole(curr_hole); //int sz = *((int*) (curr_hole + 8));
        printf( "next_hole address: %lu - size: %d\n", nextadd, sz);
	printf("---segment test ended - success\n");
	
	return (0); 
}

void *s_alloc(int req_size)
{
	// minimum alloc 64 B and maximum 64 KB
        // hole size, hole start, next hole address
        
    	printf("s_alloc called with size: %d\n", req_size);

        // round the requested size to avoid unusable memory fragments
        int unit = 12 + sizeof(long); // define the unit size
        req_size = (req_size < 64) ? 64 : req_size; 
        req_size = (req_size > 65536) ? 65536 : req_size; 
        req_size = ((req_size / unit) + 2) * unit; 
        //req_size = req_size + 4; // additional 4-bytes just for holding size of block
        printf("new req_size: %d\n", req_size);
        
        if( curr_hole == NULL && head_hole == NULL )
        {
            printf("We're out of space (totally!). Please try again later..\n");
            return NULL;
        }
        
        //printf( "next_hole address: %lu - size: %d\n", next_hole_add, hole_size);
        void *search_starting_hole = curr_hole;
        void *prev_hole = NULL;
        do {
            int curr_hole_size = get_size_of_hole(curr_hole);

            // if we find a hole greater than requested space
            // just truncate the hole and return the requested amount of it
            if( curr_hole_size > req_size )
            {
                void *allocated = curr_hole;
                
                // if that was the first and only hole
                if ( curr_hole == get_next_hole_of(curr_hole)
                        && curr_hole == head_hole )
                {
                    curr_hole = curr_hole + req_size;
                    
                    //update our new smaller hole's attributes
                    //make the next hole field point to itself again
                    set_params_of_curr_hole( (long) curr_hole, 
                                             curr_hole_size - req_size);
                    
                    head_hole = curr_hole;
                }
                // if it was one of the numerous holes
                // set the next_hole ptr. of previous hole accordingly
                else
                {
                    // and if it was the last hole
                    int is_last_hole = 
                        ( curr_hole == get_next_hole_of(curr_hole) ) ? 1 : 0;

                    curr_hole = curr_hole + req_size;
                    
                    //if last --> make the next hole field point to itself again
                    // else point to the next hole of old hole
                    long next_add = (is_last_hole) ? 
                                        (long) curr_hole
                                        : (long) get_next_hole_of(curr_hole);
                    
                    //update our new smaller hole's attributes
                    set_params_of_curr_hole( next_add, curr_hole_size - req_size);
                    set_next_hole_of(prev_hole, (long) curr_hole);
                }
                
                printf("I'm allocating from address %lx, new hole add: %lx and new hole size: %d\n", 
                        (long)allocated, (long) curr_hole, get_size_of_hole(curr_hole));
                
                //We're doing something a little bit different.
                //We're setting the size of allocated block into its bytes btw 8,11
                //And returning an address starting from right after the size field
                set_size_of_allocated_block(allocated, req_size);
                allocated = allocated + 4;
                return allocated;
                //curr_hole = ((void*) (next_hole_add + req_size));
            }
            //if the requested memory exactly fits in our hole
            else if ( curr_hole_size == req_size )
            {
                void *allocated = curr_hole;
                
                //and if that hole is the first and only hole remaining
                if ( curr_hole == get_next_hole_of(curr_hole)
                        && curr_hole == head_hole )
                {
                    // we're out of holes as of now
                    curr_hole = NULL;
                    head_hole = NULL;
                }
                // and if that hole is one of the numerous holes
                else
                {
                    //and this hole is one of the numerous holes and
                    // it was the hole at the most bottom (head)
                    if(curr_hole == head_hole)
                    {
                        // the next hole is the new head hole
                        curr_hole = get_next_hole_of(curr_hole);
                        head_hole = curr_hole;
                    }
                    // and that hole was in somewhere between the first and last holes
                    else
                    {
                        prev_hole = curr_hole;
                        curr_hole = get_next_hole_of(curr_hole);
                        set_next_hole_of(prev_hole, (long) curr_hole);
                    }
                }
                
                if( curr_hole == NULL && head_hole == NULL)
                    printf("I'm allocating from address %lx, no new hole, memory full.", (long)allocated );
                else
                    printf("I'm allocating from address %lx, new hole add: %lx and new hole size: %d\n", 
                        (long)allocated, (long) curr_hole, get_size_of_hole(curr_hole));
    
                set_size_of_allocated_block(allocated, req_size);
                allocated = allocated + 4;
                return allocated;
            }
            // if this hole is smaller than the requested space
            else
            {
                // and that was the first and only hole
                if ( curr_hole == get_next_hole_of(curr_hole)
                        && curr_hole == head_hole )
                {
                    //so.. there's no hope :(
                    printf("We're out of space. Try again later..\n");
                    return NULL;
                }
                // if there isn't a next hole anymore
                // just return to the head hole to search over the remaining holes
                else if ( curr_hole == get_next_hole_of(curr_hole) )
                {
                    curr_hole = head_hole;
                }
                // just pass to the next hole to keep searching
                else
                {
                    prev_hole = curr_hole;
                    curr_hole = get_next_hole_of(curr_hole);
                }
            }
        } while ( curr_hole != search_starting_hole );
        
}

void s_free(void *objectptr)
{

	printf("s_free called with objectptr:%lx\n", (long) objectptr);

        printf("Deallocating block %lx , block size: %d\n",
                (long) (objectptr - 4), get_size_of_allocated_block(objectptr));
        
        //if there has been no hole until now
        if ( head_hole == NULL && curr_hole == NULL )
        {
            // make this block the head hole
            head_hole = curr_hole = objectptr - 4;
            
            // new hole points to itself
            set_params_of_curr_hole( (long) curr_hole, 
                                    get_size_of_allocated_block( objectptr));
        }
        //if there have been holes 
        else
        {
            void *closest_hole = NULL;
            // and every hole is above the space to be freed
            if( (closest_hole = find_closest_hole_from_back( objectptr)) == NULL )
            {
                // adjust our new hole, it becomes the new head hole
                // new hole points to the old head hole
                set_size_of_hole(objectptr - 4, get_size_of_allocated_block(objectptr));
                set_next_hole_of(objectptr - 4, (long) head_hole );
                head_hole = objectptr - 4;
            }
            else
            {
                // and every hole is below the space to be freed
                if( closest_hole == get_next_hole_of(closest_hole) )
                {
                    // set our deallocated space as the next hole of the closest hole
                    set_next_hole_of(closest_hole, (long) objectptr - 4);
            
                    // adjust our new hole
                    set_size_of_hole(objectptr - 4, get_size_of_allocated_block(objectptr));
                    set_next_hole_of(objectptr - 4, (long) (objectptr - 4) );
                }
                // and the space to be freed is between holes
                else
                {
                    void *next = get_next_hole_of(closest_hole);
                    
                    // set our deallocated space as the next hole of the closest hole
                    set_next_hole_of(closest_hole, (long) objectptr - 4);
            
                    // adjust our new hole
                    set_size_of_hole(objectptr - 4, get_size_of_allocated_block(objectptr));
                    set_next_hole_of(objectptr - 4, (long) next );
                }
            }
        }
	return;
}

void s_print(void)
{
	printf("s_print called\n");
        
        //starting from the head hole, print out the entire hole list
        void *tmp_hole = head_hole;

        do {
            if( tmp_hole != NULL )
            {
                printf("Hole address: %lx, size: %d Bytes, next hole address: %lx\n", 
                    (long) tmp_hole, get_size_of_hole(tmp_hole), 
                    (long) get_next_hole_of(tmp_hole));
                tmp_hole = get_next_hole_of(tmp_hole);
            }
        } while ( tmp_hole - get_next_hole_of(tmp_hole) != 0);

        if( tmp_hole != NULL )
        {
            printf("Hole address: %lx, size: %d Bytes, next hole address: %lx\n", 
                (long) tmp_hole, get_size_of_hole(tmp_hole), 
                (long) get_next_hole_of(tmp_hole));
        }
        
	return;
}


void set_params_of_curr_hole( long next_hole_add, int hole_size)
{
    set_next_hole_of( curr_hole, next_hole_add);
    set_size_of_hole(curr_hole, hole_size);
}

void set_next_hole_of( void *hole, long next_hole_add)
{
    *((long*)hole) = next_hole_add; //put next hole ptr to first 8 bytes of hole
}

void set_size_of_hole( void *hole, int size)
{
    *((int*) (hole + 8)) = size; // put the size info to the bytes between 8 & 11

}

void set_size_of_allocated_block( void *block, int size)
{
    *( (int*) block) = size; // put the block size into block's first 4 bytes
}

int get_size_of_allocated_block( void *block)
{
    //the actual block contains a 4-byte size value in 4-bytes back
    int sz = * ( (int*) (block - 4)); 
    return sz;
}

int get_size_of_hole( void *hole)
{
    int sz = *((int*) (hole + 8));
    return sz;
}

void *get_next_hole_of( void *hole)
{
     long nextadd = *((long*)hole);
     return (void*) nextadd;
}

void *find_closest_hole_from_back(void *deallocated_block)
{
    printf("Closest hole search function called\n");
    
    void *tmp_hole = NULL;
    for( tmp_hole = head_hole; 
            tmp_hole != NULL
            && deallocated_block - get_next_hole_of(tmp_hole) > 0
            && tmp_hole - get_next_hole_of(tmp_hole) != 0;
            tmp_hole = get_next_hole_of(tmp_hole) );    
    
    // if the resulting hole is behind our block, return it
    if( deallocated_block - tmp_hole > 0 )
    {
        /*printf("Closest Hole address: %lx, size: %d Bytes, next hole address: %lx\n", 
                    (long) tmp_hole, get_size_of_hole(tmp_hole), 
                    (long) get_next_hole_of(tmp_hole));*/

        return tmp_hole;    
    }
    // otherwise it means our block didn't have any hole behind it
    else 
        return NULL;
    
}