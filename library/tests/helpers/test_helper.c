#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "test_helper.h"
#include <unistd.h>

char IsFileAvailable(const char *filename)
{
	FILE *f = NULL;
	char state = 0;
	
	f = fopen(filename, "r");
	
	if(f){
		fclose(f);
	}
	
	state = (f != NULL);
	
	return state;
}


char CompareFiles(const char *file1, const char *file2)
{
	FILE *f1 = NULL;
	FILE *f2 = NULL;
	int size1, size2;
	char state = 0;
	char *content1 = NULL;
	char *content2 = NULL;
	
	usleep(10000);
	
	f1 = fopen(file1, "r");
	f2 = fopen(file2, "r");
	
	if(f1 && f2){
		
		fseek(f1, 0, SEEK_END);
		size1 = ftell(f1);
		rewind(f1);
		
		fseek(f2, 0, SEEK_END);
		size2 = ftell(f2);
		rewind(f2);
		
		if(size1 == size2){
			content1 = (char*)malloc(size1);
			content2 = (char*)malloc(size2);
			
			if(content1 && content2){
				fread(content1, 1, size1, f1);
				fread(content2, 1, size2, f2);
				
				if(0 == memcmp(content1, content2, size1)){
					state = 1;
				}
				
				free(content1);
				free(content2);
			}
			else{
				printf("content mismatch\n");
			}
				
		}
		else{
			printf("size mismatch: %d, %d\n",size1, size2);
		}
		
		fclose(f1);
		fclose(f2);
	}
	else{
		printf("access error\n");
	}
	
	return state;
}
