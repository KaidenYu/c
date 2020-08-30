#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include <sys/mman.h>

char var_8=0;
short var_16=0;
int var_32=0;

extern int other_func();

int target_func(int copy_distance)
{
#if defined(__clang__)
	*((char*)(&var_8)) = 8;
	*((char*)(&var_16)) = 7;
	//*((char*)(&var_32)) = (*(other_func)+copy_distance)();

#elif defined(__GNUC__)
	*((char*)(&var_8) + copy_distance) = 7;
	*((char*)(&var_16) + copy_distance) = 8;
	*((char*)(&var_32) + copy_distance) = (*(other_func+copy_distance))();
#endif

	return 123;
}

int other_func()
{
	return 255;
}


int main()
{
	int target_func_size = abs(target_func - other_func);
	void* space = aligned_alloc(4096,1000);

	printf("target_func=%x\n",target_func);
	printf("other_func=%x\n",other_func);
	printf("target_func_size=%d\n",target_func_size);
	printf("space=%x\n",space);

	if (mprotect(space, target_func_size, PROT_READ|PROT_EXEC|PROT_WRITE) == -1) {
		perror ("mprotect");
	}

	memcpy(space,target_func,abs(target_func_size));

	int copy_distance = (void*)target_func - space;
	printf("copy_distance=%d\n",copy_distance);

	int test = (*((int (*)())space))(copy_distance);
	printf("test=%d\n",test);

	printf("var_8 = %d\n",var_8);
	printf("var_16 = %d\n",var_16);
	printf("var_32 = %d\n",var_32);

	return 0;
}