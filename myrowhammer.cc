#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

int main()
{
	int gbToAllocate=2;
	int numberLocations =gbToAllocate*2;
	int numberHammer = 5000000;
	
	//allocate memory
	size_t size=1024ULL*1024ULL*1024ULL*gbToAllocate;
	//printf("%zu\n",size);
	uint32_t* memory = (uint32_t*) malloc(size);
	size_t numberOfBlocks=size/sizeof(uint32_t);
	
	//initialise memory
	for (int i = 0; i < numberOfBlocks; i++){
		int r=rand() % 2;
		memory[i] = 0xFFFFFFFFU * r;
	}
	printf("Memory Allocation finished\n");
	int tries=0;
	
	while(1){
		for (int i = 0; i < numberLocations; i++)    {
			//find location to hammer
			size_t offset = rand() % numberOfBlocks;
			volatile uint32_t* hammerLocation =  offset+memory;
			printf("Hammering %d at offset %zu\n", tries, offset);
			tries++;
			//hammertime
			for (int j=0;j<numberHammer;j++){
				*hammerLocation;
				asm volatile("clflush (%0)" : : "r" (hammerLocation) : "memory");
			}
		}
		//check for Bitflip
		for (int i = 0; i < numberOfBlocks; i++){
			if (memory[i] != 0 && memory[i] != 0xFFFFFFFFU){
				printf("Found flip at offset %d.\n",i);
				return 1;
			}
		}
	}
}