/**
 * @file plat_arm.c
 * @author Pradeepa Senanayake (pradeepa.kck@gmail.com)
 * @date 31 Jan 2016
 * @brief File contains arm compiler specific file access implementation.
 *
 * This file contains generic functions to do file manipulation using
 * FAT file system version R0.11a.
 *
 * The file can be easyly ported to any compiler to include compiler
 * specific functions.
 *
 * Ex: Currently this file uses f_open, f_read type functions which
 * are implemented in ff.h and ff.c files. If the file needs to be
 * ported to different architecture/file system, the developer
 * only needs to modify those. Link to Fat Filesystem is available
 * below.
 *
 * @see http://elm-chan.org/fsw/ff/00index_e.html
 */

#include <stdio.h>
#include <string.h>
#include "plat_arm.h"
#include "ff.h"

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
 * of data that can be accomadated by the buffer.
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
 * @warning Warning.
 */

int FileIF_CopyFileToBuffer(const char *filename, int offset, char *buffer, int *buf_size, int *file_size)
{
	int ret = FILEIF_OP_SUCCESS;
	int f_size = 0;
	int amount_to_read = 0;	
	UINT f_read_size = 0;
	FILE f;
	
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
		
		ret = f_open(&f, filename, FA_READ);
		
		if(ret == FR_OK){
			
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
				
				if(FR_OK == f_lseek(&f, offset)){
					if(FR_OK == f_read(&f, buffer, amount_to_read, &f_read_size)){
						*buf_size = f_read_size;
					}
					else{
						ret = FILEIF_ERR_FILE_NOT_AVAILABLE;
					}
				}
				else{
					ret = FILEIF_ERR_FILE_NOT_AVAILABLE;
				}
				
				f_close(&f);
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
 */
int FileIF_IsFileAvailable(const char *filename)
{
	int ret;
	FILE f;
	
	if(NULL == filename){
		ret = FILEIF_ERR_INVALID_PARAM;
	}
	else{
		
		ret = f_open(&f,filename,FA_READ);
		
		if(ret == FR_OK){
			ret = FILEIF_OP_SUCCESS;	
			f_close(&f);
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
 * @note If the file is already available function will return success.
 */

int FileIF_CreateFile(const char *filename)
{
	int ret;
	FILE f;
	
	ret = FileIF_IsFileAvailable(filename);
	
	if(FILEIF_ERR_FILE_NOT_AVAILABLE == ret){
		ret = f_open(&f,filename, FA_WRITE | FA_OPEN_ALWAYS);
		
		if(ret == FR_OK){
			ret = f_lseek(&f,f_size(&f));

			if(ret == FR_OK){
				ret = FILEIF_OP_SUCCESS;
				f_close(&f);
			}
		}

		if(ret != FR_OK){
			ret = FILEIF_ERR_FILE_NOT_AVAILABLE;
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
 * @note None
 * 
 * @warning None
 */
int FileIF_DeleteFile(const char *filename)
{		
	int ret = FileIF_IsFileAvailable(filename);
	
	if(FILEIF_OP_SUCCESS == ret){								
		if(f_unlink(filename) != FR_OK){
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
 * @note None
 * 
 * @warning None
 */
int FileIF_GetFileSize(const char *filename, int *file_size)
{
	int ret;
	FILE f;
	
	if(NULL == filename || NULL == file_size){
		ret = FILEIF_ERR_INVALID_PARAM;
	}
	else{

		ret = f_open(&f,filename, FA_READ);
			
		if(ret == FR_OK){
			/* Get the file size */
			*file_size = f_size(&f);
			
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
 * @note None
 * 
 * @warning None
 */
int FileIF_AppendString(const char *filename, const char *string)
{
	int ret = FILEIF_OP_SUCCESS;
	FILE f;
	
	if((NULL == filename) || (NULL == string)){
		ret = FILEIF_ERR_INVALID_PARAM;
	}
	else{
		ret = FileIF_IsFileAvailable(filename);
		
		if(FILEIF_OP_SUCCESS == ret){
			ret = f_open(&f, filename, FA_WRITE);

			if(FR_OK == ret){
				ret = f_lseek(&f, f_size(&f));

				if(FR_OK == ret){
					f_puts(string,&f);

					if(FR_OK == ret){
						f_close(&f);
						ret = FILEIF_OP_SUCCESS;
					}
				}
			}

			if(ret != FR_OK){
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
 * @note None
 * 
 * @warning None
 */
int FileIF_GetNoOfLines(const char *filename,int *no_of_lines)
{
	int ret = FileIF_IsFileAvailable(filename);
	FILE f;
	char ch[2];
	int line_count = 0;
	
	if(FILEIF_OP_SUCCESS == ret){
		if(NULL == no_of_lines){
			ret = FILEIF_ERR_INVALID_PARAM;
		}
		else{
			ret = f_open(&f,filename, FA_READ);
			if(ret == FR_OK){
				while(!f_eof(&f)){

					f_gets(ch,2,&f);

					if(ch[0] == '\n'){
						line_count++;
					}
				}
				
				f_close(&f);
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
 * @note 	If FILEIF_ERR_BUFFER_SIZE is returned then the @buf_size 
 * 			will contain the required data amount
 * 
 * @warning None
 */

int FileIF_ReadLine(const char *filename, int line_no, char *line_buffer, int *buf_size)
{
	int ret = FILEIF_OP_SUCCESS;
	FILE f;
	int line_index = 0;
	int line_length = 0;
	char line_found = 0;
	char temp_line[256];
	
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
		
		ret = f_open(&f,filename,FA_READ);
		
		if(ret == FR_OK){
			/* Search for the line */
			while(NULL != f_gets(temp_line, sizeof(temp_line), &f)){
				
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
			
			f_close(&f);
		}
		else{
			ret = FILEIF_ERR_FILE_NOT_AVAILABLE;
		}
	}
	
	return ret;
}
