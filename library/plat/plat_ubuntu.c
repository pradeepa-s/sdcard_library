/**
 * @file plat_ubuntu.c
 * @author Pradeepa Senanayake (pradeepa.kck@gmail.com)
 * @date 31 Jan 2016
 * @brief File contains arm compiler specific file access implementation.
 *
 * This file contains generic functions to do file manipulation using
 * standard file access interface.
 *
 * The file can be easily ported to any compiler to include compiler
 * specific functions.
 *
 * Version v1.0.0
 */

#include <stdio.h>
#include <string.h>
#include "plat_ubuntu.h"

enum {INITIALIZED = 1, NOT_INITIALIZED = 0};

static char initialized = NOT_INITIALIZED;


static int CheckInitialization(void);


/**
 * @brief Function to initialize the file system access layer
 * 
 * @return 	FILEIF_OP_SUCCESS				Operation success 
 */
int FileIF_Initialize(void)
{
	initialized = INITIALIZED;
	return FILEIF_OP_SUCCESS;
}

/**
 * @brief Function to open a file and copy the content to a buffer
 * 
 * Function will attempt to open the file with the given filename and copy
 * the content of the file to the buffer.
 * 
 * If the buffer size is more than file size, file content will be copied and 
 * the rest of the buffer will have 0x00 value.
 * 
 * If the buffer size is less than the file size, this will only copy the amount 
 * of data that can be accommodated by the buffer.
 * 
 * If an @offset is specified the data copying will start after applying the
 * offset to the file buffer.
 * 
 * If the function succeed the @buf_size parameter will contain the amount
 * of data copied.
 * 
 * If @filename and @file_size are valid, the @file_size parameter will 
 * contain the size of the pointed file when function returns.
 * 
 * @param filename[in] 		Filename to use
 * @param offset[in] 		Offset to apply to file data when start reading
 * @param buffer[out] 		Buffer pointer to copy data
 * @param buf_size[inout] 	Size of the buffer. Upon proper execution of the function the copied data amount will be saved in this variable pointer.
 * @param file_size[out]	Pointer to an integer which to get the pointed file size
 * 
 * @return 	FILEIF_OP_SUCCESS				Operation success 
 * @return 	FILEIF_ERR_INVALID_PARAM		Function parameters are invalid
 * @return 	FILEIF_ERR_BUFFER_SIZE			Invalid buffer size (zero or negative)
 * @return 	FILEIF_ERR_FILE_OFFSET			Offset is negative or more than file size
 * @return 	FILEIF_ERR_FILE_NOT_AVAILABLE	File cannot be accessed
 * @return 	FILEIF_WARN_BUFFER_SIZE_SMALL	Provided buffer is small (warning)
 * @return 	FILEIF_WARN_BUFFER_SIZE_LARGE	Provided buffer is large (warning)
 * 
 * @note Do not treat warnings as errors
 *
 * @note	If called in uninitialized state, the function internally will
 * 			initialize the library and continue.
 *
 * @warning Warning.
 */

int FileIF_CopyFileToBuffer(const char *filename, int offset, char *buffer, int *buf_size, int *file_size)
{
	int ret = FILEIF_OP_SUCCESS;
	int f_size = 0;
	int amount_to_read = 0;	
	int f_read_size = 0;
	FILE *f = NULL;
	
	/* Check whether initialization is done */
	ret = CheckInitialization();

	if(FILEIF_OP_SUCCESS != ret){
		return ret;
	}
	
	ret = FileIF_GetFileSize(filename, file_size);
	
	if(FILEIF_OP_SUCCESS != ret){
		return ret;
	}
	else if((NULL == buf_size) || (NULL == buffer)){
		ret = FILEIF_ERR_INVALID_PARAM;
	}
	else if(*buf_size <= 0) {
		ret = FILEIF_ERR_BUFFER_SIZE;
	}
	else if(offset < 0){
		ret = FILEIF_ERR_FILE_OFFSET;
	}
	else{
			
		FileIF_GetFileSize(filename, &f_size);
		*file_size = f_size;
		
		f = fopen(filename, "r");
		
		if(f){
			
			/* Set the buffer content to 0x00 */
			memset(buffer, 0x00, *buf_size);
			
			amount_to_read = *buf_size;
			
			/* Check the offset */
			if(offset > f_size){
				ret = FILEIF_ERR_FILE_OFFSET;
			}
			else{
										
				/* Check the buf size */
				if(f_size < *buf_size){
					ret = FILEIF_WARN_BUFFER_SIZE_LARGE;	
					amount_to_read = f_size;			
				}
				else if(f_size > *buf_size){
					ret = FILEIF_WARN_BUFFER_SIZE_SMALL;
				}
				
				fseek(f, offset, SEEK_SET);
				f_read_size = fread(buffer, 1, amount_to_read, f);	
				
				*buf_size = f_read_size;	
				fclose(f);				
			}
		}
		else{
			ret = FILEIF_ERR_FILE_NOT_AVAILABLE;
		}
	}
	
	return ret;
}

/**
 * @brief Function to check whether a file is present in the system
 * 
 * 
 * @param filename 	Filename 
 * 
 * @return 	FILEIF_OP_SUCCESS				Operation success 
 * @return 	FILEIF_ERR_INVALID_PARAM		Function parameters are invalid
 * @return 	FILEIF_ERR_FILE_NOT_AVAILABLE	File cannot be accessed
 * 
 * @note	If called in uninitialized state, the function internally will
 * 			initialize the library and continue.
 *
 *
 */
int FileIF_IsFileAvailable(const char *filename)
{
	int ret;
	FILE *f = NULL;
	
	/* Check whether initialization is done */
	ret = CheckInitialization();
	
	if(FILEIF_OP_SUCCESS != ret){
		return ret;
	}

	if(NULL == filename){
		ret = FILEIF_ERR_INVALID_PARAM;
	}
	else{
		
		f = fopen(filename, "r");
		
		if(f){
			ret = FILEIF_OP_SUCCESS;	
			fclose(f);		
		}
		else{
			ret = FILEIF_ERR_FILE_NOT_AVAILABLE;
		}
	}
	
	return ret;
}

/**
 * @brief Function to create a new file in the system
 * 
 * Function will attempt to create a new file. If file is already available,
 * function will return success.
 * 
 * @param filename 	Filename 
 * 
 * @return 	FILEIF_OP_SUCCESS				Operation success 
 * @return 	FILEIF_ERR_INVALID_PARAM		Function parameters are invalid
 * @return 	FILEIF_ERR_FILE_NOT_AVAILABLE	File cannot be accessed/created
 * 
 * @note	If called in uninitialized state, the function internally will
 * 			initialize the library and continue.
 
 * @note If the file is already available function will return success.
 */

int FileIF_CreateFile(const char *filename)
{
	int ret;
	FILE *f = NULL;

	/* Check whether initialization is done */
	ret = CheckInitialization();

	if(FILEIF_OP_SUCCESS != ret){
		return ret;
	}

	ret = FileIF_IsFileAvailable(filename);
	
	if(FILEIF_ERR_FILE_NOT_AVAILABLE == ret){
		f = fopen(filename, "w");
		
		if(f){
			ret = FILEIF_OP_SUCCESS;
			fclose(f);
		}
	}
	
	return ret;
}

/**
 * @brief Function to delete a file from the system
 * 
 * @param filename 	Filename 
 * 
 * @return 	FILEIF_OP_SUCCESS				Operation success 
 * @return 	FILEIF_ERR_INVALID_PARAM		Function parameters are invalid
 * @return 	FILEIF_ERR_FILE_NOT_AVAILABLE	File cannot be found or cannot be deleted
 * 
 * @note	If called in uninitialized state, the function internally will
 * 			initialize the library and continue.
 *
 * 
 * @warning None
 */
int FileIF_DeleteFile(const char *filename)
{		
	int ret;

	/* Check whether initialization is done */
	ret = CheckInitialization();

	if(FILEIF_OP_SUCCESS != ret){
		return ret;
	}

	ret = FileIF_IsFileAvailable(filename);

	if(FILEIF_OP_SUCCESS == ret){								
		if(remove(filename) != 0){
			ret = FILEIF_ERR_FILE_NOT_AVAILABLE;
		}
	}
	
	return ret;
}

/**
 * @brief Function to delete a file from the system
 * 
 * @param filename[in] 		Filename 
 * @param file_size[out] 	Integer pointer to store the filesize 
 * 
 * @return 	FILEIF_OP_SUCCESS				Operation success 
 * @return 	FILEIF_ERR_INVALID_PARAM		Function parameters are invalid
 * @return 	FILEIF_ERR_FILE_NOT_AVAILABLE	File cannot be found or cannot be accessed
 * 
 * @note	If called in uninitialized state, the function internally will
 * 			initialize the library and continue.
 *
 * 
 * @warning None
 */
int FileIF_GetFileSize(const char *filename, int *file_size)
{
	int ret;
	FILE *f;

	/* Check whether initialization is done */
	ret = CheckInitialization();

	if(FILEIF_OP_SUCCESS != ret){
		return ret;
	}

	if(NULL == filename || NULL == file_size){
		ret = FILEIF_ERR_INVALID_PARAM;
	}
	else{

		f = fopen(filename, "r");
			
		if(f){
			/* Get the file size */
			fseek(f, 0, SEEK_END);
			*file_size = ftell(f);
			rewind(f);
			
			fclose(f);
			
			ret = FILEIF_OP_SUCCESS;
		}
		else{
			ret = FILEIF_ERR_FILE_NOT_AVAILABLE;
		}
			
	}
	
	return ret;
}

/**
 * @brief Function to append a string to the end of a file
 * 
 * @param filename[in] 		Filename 
 * @param string[in] 		Null terminated string to append
 * 
 * @return 	FILEIF_OP_SUCCESS				Operation success 
 * @return 	FILEIF_ERR_INVALID_PARAM		Function parameters are invalid
 * @return 	FILEIF_ERR_FILE_NOT_AVAILABLE	File cannot be found or cannot be accessed
 * 
 * @note	If called in uninitialized state, the function internally will
 * 			initialize the library and continue.
 *
 * 
 * @warning None
 */
int FileIF_AppendString(const char *filename, const char *string)
{
	int ret = FILEIF_OP_SUCCESS;
	FILE *f = NULL;

	/* Check whether initialization is done */
	ret = CheckInitialization();

	if(FILEIF_OP_SUCCESS != ret){
		return ret;
	}

	if((NULL == filename) || (NULL == string)){
		ret = FILEIF_ERR_INVALID_PARAM;
	}
	else{
		ret = FileIF_IsFileAvailable(filename);
		
		if(FILEIF_OP_SUCCESS == ret){
			f = fopen(filename, "a+");
			fseek(f,0,SEEK_END);
			
			if(f){
				fputs(string,f);
				fclose(f);
			}
			else{
				ret = FILEIF_ERR_FILE_NOT_AVAILABLE;
			}
		}			
	}
	
	return ret;
}

/**
 * @brief Function to get number of lines in a file
 * 
 * This function can be used to get number of lines available in a
 * text file.
 * 
 * @param filename[in] 		Filename 
 * @param no_of_lines[out] 	No of lines in the file
 * 
 * @return 	FILEIF_OP_SUCCESS				Operation success 
 * @return 	FILEIF_ERR_INVALID_PARAM		Function parameters are invalid
 * @return 	FILEIF_ERR_FILE_NOT_AVAILABLE	File cannot be found or cannot be accessed
 * 
 * @note	If called in uninitialized state, the function internally will
 * 			initialize the library and continue.
 *
 * 
 * @warning None
 */
int FileIF_GetNoOfLines(const char *filename,int *no_of_lines)
{
	int ret;
	FILE *f = NULL;
	char ch;
	int line_count = 0;

	/* Check whether initialization is done */
	ret = CheckInitialization();

	if(FILEIF_OP_SUCCESS != ret){
		return ret;
	}

	ret = FileIF_IsFileAvailable(filename);

	if(FILEIF_OP_SUCCESS == ret){
		if(NULL == no_of_lines){
			ret = FILEIF_ERR_INVALID_PARAM;
		}
		else{
			f = fopen(filename, "r");
			if(f){
				while(!feof(f)){
					ch = fgetc(f);
					if(ch == '\n'){
						line_count++;
					}
				}
				
				fclose(f);
				*no_of_lines = line_count;
			}
			else{
				ret = FILEIF_ERR_FILE_NOT_AVAILABLE;
			}
		}		
	}
	
	return ret;
}

/**
 * @brief Function to read a line of text from a file
 * 
 * This function can be used to read a line of text from a file.
 * 
 * @param filename[in] 		Filename 
 * @param line_no[in] 		Line number to read
 * @param line_buffer[out] 	Buffer to store data
 * @param buf_size[inout] 	Buffer length
 * 
 * @return 	FILEIF_OP_SUCCESS				Operation success 
 * @return 	FILEIF_ERR_INVALID_PARAM		Function parameters are invalid
 * @return 	FILEIF_ERR_FILE_NOT_AVAILABLE	File cannot be found or cannot be accessed
 * @return 	FILEIF_ERR_BUFFER_SIZE			Buffer size is invalid
 * @return 	FILEIF_ERR_LINE_NO				Line number is invalid
 *
 * @note	If called in uninitialized state, the function internally will
 * 			initialize the library and continue.
 * @note 	If FILEIF_ERR_BUFFER_SIZE is returned then the @buf_size 
 * 			will contain the required data amount
 * 
 * @warning None
 */

int FileIF_ReadLine(const char *filename, int line_no, char *line_buffer, int *buf_size)
{
	int ret = FILEIF_OP_SUCCESS;
	FILE *f = NULL;	
	int line_index = 0;
	int line_length = 0;
	char line_found = 0;
	char temp_line[256];

	/* Check whether initialization is done */
	ret = CheckInitialization();

	if(FILEIF_OP_SUCCESS != ret){
		return ret;
	}

	/* Parameter validation */
	if((NULL == filename) || (NULL == line_buffer) || (NULL == buf_size)){
		ret = FILEIF_ERR_INVALID_PARAM;
	}
	else if(line_no <= 0){
		ret = FILEIF_ERR_LINE_NO;
	}
	else{
		ret = FileIF_IsFileAvailable(filename);
	}
	
	if(FILEIF_OP_SUCCESS == ret){
		
		f = fopen(filename,"r");
		
		if(f){
			/* Search for the line */
			while(NULL != fgets(temp_line, sizeof(temp_line), f)){
				
				line_index++;				
				if(line_no == line_index){					
					line_found = 1;
					break;
				}				
			}
			
			if(line_found){
				/* Verify line size */
				memset(line_buffer, 0x00, *buf_size);
				
				line_length = strlen(temp_line);
				
				if(line_length <= *buf_size){
					/* Copy line content */					
					memcpy(line_buffer, temp_line, line_length);
				}
				else{					
					ret = FILEIF_ERR_BUFFER_SIZE;
				}
				
				/* Copy line size */
				*buf_size = line_length;
			}
			else{
				ret = FILEIF_ERR_LINE_NO;
			}			
			
			fclose(f);
		}
		else{
			ret = FILEIF_ERR_FILE_NOT_AVAILABLE;
		}
	}
	
	return ret;
}

/**
 * @brief Function to copy buffer content to file
 * 
 * This function appends buffer content to a file 
 * 
 * @param filename[in] 		Filename 
 * @param buffer[in] 		Data to append
 * @param buf_size[in] 		Data amount
 * 
 * @return 	FILEIF_OP_SUCCESS				Operation success 
 * @return 	FILEIF_ERR_INVALID_PARAM		Function parameters are invalid
 * @return 	FILEIF_ERR_FILE_NOT_AVAILABLE	File cannot be found
 * @return 	FILEIF_ERR_FILE_ACCESS			File cannot be accessed
 * 
 * @note	If called in uninitialized state, the function internally will
 * 			initialize the library and continue.
 *
 * @warning None
 */

int FileIF_CopyBufferToFile(const char *filename, char *buffer, int buf_size)
{
	int ret = FILEIF_OP_SUCCESS;
	
	FILE *f;
	int copied_amount = 0;

	/* Check whether initialization is done */
	ret = CheckInitialization();

	if(FILEIF_OP_SUCCESS != ret){
		return ret;
	}

	if(	(NULL == filename)	||
		(NULL == buffer)	||
		(buf_size <= 0)){
		ret = FILEIF_ERR_INVALID_PARAM;
	}
	else{
		ret = FileIF_IsFileAvailable(filename);
	}
	
	if(FILEIF_OP_SUCCESS == ret){
		f = fopen(filename, "a+");
		
		if(f){
			copied_amount = fwrite(buffer, sizeof(char), buf_size, f);
			
			if(copied_amount != buf_size){
				ret = FILEIF_ERR_FILE_ACCESS;
			}
			
			fclose(f);
		}
		else{
			ret = FILEIF_ERR_FILE_NOT_AVAILABLE;
		}
	}
	
	return ret;
}

/**
 * @brief	Function to reset the states to original
 *
 * This function is a debug function which can make the FileIf to
 * move to uninitialized state.
 *
 * @param 	None
 *
 * @return 	None
 *
 */
void FileIF_Uninit(void)
{
	initialized = NOT_INITIALIZED;
}

/************************** Static functions **************************/

int CheckInitialization(void)
{
	int ret = FILEIF_OP_SUCCESS;

	if(NOT_INITIALIZED == initialized){
		ret = FileIF_Initialize();

		if(FILEIF_OP_SUCCESS != ret){
			return ret;
		}
	}

	return ret;
}
