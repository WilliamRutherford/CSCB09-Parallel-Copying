#include <stdio.h>
#include <stdlib.h>


char* hash(FILE *f) {
	long block_size = 8;
	int index = 0;
	char *hash_val = malloc(block_size * sizeof(char));
	int characterXOR;
	while ((characterXOR = fgetc(f)) != EOF)
	{
		hash_val[index%block_size] = hash_val[index%block_size] ^ (char)characterXOR;
		index += 1;
	}
	return hash_val;
}


int check_hash(const char *hash1, const char *hash2, long block_size) {
	for (int i = 0; i < block_size; i ++){
		if (hash1[i] != hash2[i]){
			return i;
		}
	}
    return block_size;
}
