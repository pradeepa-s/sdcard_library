#include "unity_fixture.h"
#include "plat_ubuntu.h"
#include "test_helper.h"
#include <string.h>

/*
 * TESTS
 * 
 * CopyFileToBuffer generates error if file name is NULL - OK
 * CopyFileToBuffer generates error if file is not available - OK
 * CopyFileToBuffer generates error if buffer is NULL - OK 
 * CopyFileToBuffer generates error if buffer size is less than or equal to zero - OK 
 * CopyFileToBuffer generates error if buffer size pointer is NULL - OK 
 * CopyFileToBuffer generates error if file size pointer is NULL - OK
 * CopyFileToBuffer generates warning if buffer size is less than file size  - OK  
 * CopyFileToBuffer generates warning if buffer size is more than file size - OK 
 * CopyFileToBuffer copies the entire file content to the buffer when buffer size is same as file size.  - OK 
 * CopyFileToBuffer copies the buffer size amount of file content to the buffer when buffer size is less than file size. - OK 
 * CopyFileToBuffer copies the entire file content to the buffer when buffer size is more than file size and rest will be zero. - OK 
 * CopyFileToBuffer sets the filesize in the filesize variable it self - OK
 * CopyFileToBuffer sets the number of bytes copied in the buffer size variable it self - OK
 * CopyFileToBuffer closes the file after all the operations - OK
 * CopyFileToBuffer returns success if no errors happened - OK
 * CopyFileToBuffer generates error if offset is negative - OK
 * CopyFileToBuffer generates error if offset is more than filesize - OK
 * CopyFileToBuffer starts reading data from the offset - OK
 * CopyFileToBuffer copies the filesize if offset is invalid - OK
 * CopyFileToBuffer copies the filesize if offset is more - OK
 * CopyFileToBuffer copies the filesize if buffer size is invalid - OK
 * CopyFileToBuffer copies the filesize if buffer is invalid - OK
 * 
 * FileIF_IsFileAvailable generates error if file name is NULL - OK
 * FileIF_IsFileAvailable generates error if file is not available - OK
 * FileIF_IsFileAvailable is success if file is available - OK
 * FileIF_IsFileAvailable closes the file after checking for availability - OK
 * 
 * CreateFile generates error if file name is NULL - OK
 * CreateFile returns success if file is already there - OK
 * CreateFile creates a file with given name - OK
 * CreateFile closes the file after creating it  and return success - OK
 * 
 * DeleteFile generates error if file name is NULL - OK
 * DeleteFile generates error if file is not available - OK
 * DeleteFile removes the file and returns success is file is available - OK
 * DeleteFile generates error if file cannot be removed - IGNORED
 * 
 * GetFileSize generates error if filename is NULL - OK
 * GetFileSize generates error if file_size parameter is NULL - OK
 * GetFileSize generates error if file cannot be accessed - OK
 * GetFileSize reads file size and returns success if file is available - OK
 * 
 * FileIF_AppendString returns error if filename is NULL - OK
 * FileIF_AppendString returns error if string is NULL - OK
 * FileIF_AppendString returns error if file is not accessible - OK
 * FileIF_AppendString appends the string at the end of the file - OK
 * 
 * FileIF_GetNoOfLines returns error if filename is NULL - OK
 * FileIF_GetNoOfLines returns error if file is not available - OK
 * FileIF_GetNoOfLines returns error if no_of_lines parameter is NULL - OK
 * FileIF_GetNoOfLines returns success and copies the number of lines if parameters are correct - OK
 * FileIF_GetNoOfLines returns zero as no_of_lines of empty file - OK
 * 
 * FileIF_ReadLine returns error if filename is NULL - OK
 * FileIF_ReadLine returns error if line buffer is NULL - OK
 * FileIF_ReadLine returns error if buffer_size is NULL - OK
 * FileIF_ReadLine returns error if buffer_size is too small - OK
 * FileIF_ReadLine copies required buffer size to buffer_size parameter if buffer_size is too small - OK
 * FileIF_ReadLine copies copied data amount if successful - OK
 * FileIF_ReadLine returns error if file is not accessible - OK
 * FileIF_ReadLine returns error if line_no is negative - OK
 * FileIF_ReadLine returns error if line_no is zero - OK
 * FileIF_ReadLine returns error if line_no is more than total lines - OK
 * FileIF_ReadLine copies the string to the buffer if successful
 * 
 */
 
TEST_GROUP(file_interface);

TEST_SETUP(file_interface)
{
    
}

TEST_TEAR_DOWN(file_interface)
{
	remove("file_interface/new_file");
	remove("file_interface/appendstring_test_file.txt");
}

TEST(file_interface, CopyFileToBufferErrorIfFileNameIsNULL)
{	
	int buf_size = 10;;
	char buffer;
	int file_size;
	
	TEST_ASSERT_EQUAL(FILEIF_ERR_INVALID_PARAM, FileIF_CopyFileToBuffer(NULL, 0, &buffer, &buf_size, &file_size));
}


TEST(file_interface, CopyFileToBufferErrorIfBuffSizeIsNULL)
{		
	char buffer;
	int file_size;
	
	TEST_ASSERT_EQUAL(FILEIF_ERR_INVALID_PARAM, FileIF_CopyFileToBuffer("file_interface/file_20_bytes.bin", 0, &buffer, NULL, &file_size));
}

TEST(file_interface, CopyFileToBufferErrorIfOffsetIsNegative)
{		
	char buffer;
	int buf_size = 10;
	int file_size;
	
	TEST_ASSERT_EQUAL(FILEIF_ERR_FILE_OFFSET, FileIF_CopyFileToBuffer("file_interface/file_20_bytes.bin", -1, &buffer, &buf_size, &file_size));
}

TEST(file_interface, CopyFileToBufferErrorIfBufferIsNULL)
{		
	int buf_size = 10;
	int file_size;
	TEST_ASSERT_EQUAL(FILEIF_ERR_INVALID_PARAM, FileIF_CopyFileToBuffer("file_interface/file_20_bytes.bin", 0, NULL, &buf_size, &file_size));
}

TEST(file_interface, CopyFileToBufferErrorIfFilesizeIsNULL)
{		
	int buf_size = 10;	
	char buffer;
	TEST_ASSERT_EQUAL(FILEIF_ERR_INVALID_PARAM, FileIF_CopyFileToBuffer("file_interface/file_20_bytes.bin", 0, &buffer, &buf_size, NULL));
}

TEST(file_interface, CopyFileToBufferErrorIfBufferSizeIsZero)
{
	int buf_size = 0;
	char buffer;
	int file_size;
	
	TEST_ASSERT_EQUAL(FILEIF_ERR_BUFFER_SIZE, FileIF_CopyFileToBuffer("file_interface/file_20_bytes.bin", 0, &buffer, &buf_size, &file_size));
}

TEST(file_interface, CopyFileToBufferErrorIfBufferSizeIsLessThanZero)
{
	int buf_size = -20;
	char buffer;
	int file_size;
	
	TEST_ASSERT_EQUAL(FILEIF_ERR_BUFFER_SIZE, FileIF_CopyFileToBuffer("file_interface/file_20_bytes.bin", 0, &buffer, &buf_size, &file_size));
}

TEST(file_interface, CopyFileToBufferErrorIfFileNotAvailable)
{		
	char buffer;
	int buf_size = 1;
	int file_size;
	
	TEST_ASSERT_EQUAL(FILEIF_ERR_FILE_NOT_AVAILABLE, FileIF_CopyFileToBuffer("test", 0, &buffer, &buf_size, &file_size));
}


TEST(file_interface, CopyFileToBufferWarningIfBufferSizeMoreThanFileSize)
{		
	char buffer[50];
	int buf_size = sizeof(buffer);
	int file_size = 0;
	
	TEST_ASSERT_EQUAL(FILEIF_WARN_BUFFER_SIZE_LARGE, FileIF_CopyFileToBuffer("file_interface/file_20_bytes.bin", 0, buffer, &buf_size, &file_size));
}

TEST(file_interface, CopyFileToBufferStartsReadingDataFromTheOffset)
{		
	char buffer[50];
	char expect[10] = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13};
	int buf_size = sizeof(buffer);
	int file_size = 0;
	
	TEST_ASSERT_EQUAL(FILEIF_WARN_BUFFER_SIZE_SMALL, FileIF_CopyFileToBuffer("file_interface/file_100bytes_counter.bin", 10, buffer, &buf_size, &file_size));
	TEST_ASSERT_EQUAL_MEMORY(expect, buffer, 10);
}

TEST(file_interface, CopyFileToBufferCopiesFileSizeEvenIfOffsetIsInvalid)
{		
	char buffer[50];
	int buf_size = sizeof(buffer);
	int file_size = 0;
	
	TEST_ASSERT_EQUAL(FILEIF_ERR_FILE_OFFSET, FileIF_CopyFileToBuffer("file_interface/file_100bytes_counter.bin", -1, buffer, &buf_size, &file_size));
	TEST_ASSERT_EQUAL(100, file_size);
}

TEST(file_interface, CopyFileToBufferCopiesFileSizeEvenIfOffsetIsMore)
{		
	char buffer[50];
	int buf_size = sizeof(buffer);
	int file_size = 0;
	
	TEST_ASSERT_EQUAL(FILEIF_ERR_FILE_OFFSET, FileIF_CopyFileToBuffer("file_interface/file_100bytes_counter.bin", 500, buffer, &buf_size, &file_size));
	TEST_ASSERT_EQUAL(100, file_size);
}

TEST(file_interface, CopyFileToBufferCopiesFileSizeEvenIfBufferSizeIsInvalid)
{		
	char buffer[50];
	int buf_size = -1;
	int file_size = 0;
	
	TEST_ASSERT_EQUAL(FILEIF_ERR_BUFFER_SIZE, FileIF_CopyFileToBuffer("file_interface/file_100bytes_counter.bin", 10, buffer, &buf_size, &file_size));
	TEST_ASSERT_EQUAL(100, file_size);
}

TEST(file_interface, CopyFileToBufferCopiesFileSizeEvenIfBufferIsInvalid)
{		
	int buf_size = 5;
	int file_size = 0;
	
	TEST_ASSERT_EQUAL(FILEIF_ERR_INVALID_PARAM, FileIF_CopyFileToBuffer("file_interface/file_100bytes_counter.bin", 10, NULL, &buf_size, &file_size));
	TEST_ASSERT_EQUAL(100, file_size);
}

TEST(file_interface, CopyFileToBufferStoresFilesizeCorrectly)
{		
	char buffer[50];
	int buf_size = sizeof(buffer);
	int file_size;
	
	TEST_ASSERT_EQUAL(FILEIF_WARN_BUFFER_SIZE_LARGE, FileIF_CopyFileToBuffer("file_interface/file_20_bytes.bin", 0, buffer, &buf_size, &file_size));
	TEST_ASSERT_EQUAL(20, file_size);
}

TEST(file_interface, CopyFileToBufferWarningIfBufferSizeLessThanFileSize)
{		
	char buffer[10];
	int buf_size = sizeof(buffer);
	int file_size;
	
	TEST_ASSERT_EQUAL(FILEIF_WARN_BUFFER_SIZE_SMALL, FileIF_CopyFileToBuffer("file_interface/file_20_bytes.bin", 0, buffer, &buf_size, &file_size));
}

TEST(file_interface, CopyFileToBufferCopiesTheFileContentToBufferSameBufferSize)
{		
	char buffer[100];	
	char expected[100];
	int buf_size = sizeof(buffer);
	int i;
	int file_size;
	
	memset(buffer, 0xFF, sizeof(buffer));
	
	for( i = 0; i < sizeof(expected); i++){
		expected[i] = i;
	} 
	
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_CopyFileToBuffer("file_interface/file_100bytes_counter.bin", 0, buffer, &buf_size, &file_size));
	
	TEST_ASSERT_EQUAL_MEMORY(expected, buffer, sizeof(buffer));
}

TEST(file_interface, CopyFileToBufferCopiesTheFileContentToBufferWhenBufferSizeIsLess)
{		
	char buffer[50];	
	char expected[100];
	int buf_size = sizeof(buffer);
	int i;
	int file_size;
	
	memset(buffer, 0xFF, sizeof(buffer));
	
	for( i = 0; i < sizeof(buffer); i++){
		expected[i] = i;
	} 
	
	TEST_ASSERT_EQUAL(FILEIF_WARN_BUFFER_SIZE_SMALL, FileIF_CopyFileToBuffer("file_interface/file_100bytes_counter.bin", 0, buffer, &buf_size, &file_size));
	
	TEST_ASSERT_EQUAL_MEMORY(expected, buffer, sizeof(buffer));
}

TEST(file_interface, CopyFileToBufferCopiesTheFileContentToBufferWhenBufferSizeIsMore)
{		
	char buffer[200];	
	char expected[100];
	int buf_size = sizeof(buffer);
	int i;
	int file_size;
	
	memset(buffer, 0xFF, sizeof(buffer));
	
	for( i = 0; i < sizeof(expected); i++){
		expected[i] = i;
	} 
	
	TEST_ASSERT_EQUAL(FILEIF_WARN_BUFFER_SIZE_LARGE, FileIF_CopyFileToBuffer("file_interface/file_100bytes_counter.bin", 0, buffer, &buf_size, &file_size));
	
	TEST_ASSERT_EQUAL_MEMORY(expected, buffer, sizeof(expected));
}

TEST(file_interface, CopyFileToBufferWhenBufferSizeIsMoreMakesRestOfTheBufferZero)
{		
	char buffer[200];	
	char expected[200];
	int buf_size = sizeof(buffer);
	int i;
	int file_size;
	
	memset(buffer, 0xFF, sizeof(buffer));
	memset(expected, 0x00, sizeof(buffer));
	
	for( i = 0; i < 100; i++){
		expected[i] = i;
	} 
	
	TEST_ASSERT_EQUAL(FILEIF_WARN_BUFFER_SIZE_LARGE, FileIF_CopyFileToBuffer("file_interface/file_100bytes_counter.bin", 0, buffer, &buf_size, &file_size));
	
	TEST_ASSERT_EQUAL_MEMORY(expected, buffer, sizeof(expected));
}

TEST(file_interface, CopyFileToBufferSetsTheReadAmountInBufSizeVariable)
{		
	char buffer[50];	
	int buf_size = sizeof(buffer);
	int file_size;
	
	FileIF_CopyFileToBuffer("file_interface/file_100bytes_counter.bin", 0, buffer, &buf_size, &file_size);
	
	TEST_ASSERT_EQUAL(50, buf_size);
}

TEST(file_interface, CopyFileToBufferReturnsSuccessIfNoErrors)
{		
	int ret;
	char buffer[100];	
	int buf_size = sizeof(buffer);
	int file_size;
	
	ret = FileIF_CopyFileToBuffer("file_interface/file_100bytes_counter.bin", 0, buffer, &buf_size, &file_size);
	
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, ret);
}

TEST(file_interface, CopyFileToBufferGeneratesErrorIfOffsetIsMoreThenFileSize)
{		
	int ret;
	char buffer[100];	
	int buf_size = sizeof(buffer);
	int file_size;
	
	ret = FileIF_CopyFileToBuffer("file_interface/file_100bytes_counter.bin", 300, buffer, &buf_size, &file_size);
	
	TEST_ASSERT_EQUAL(FILEIF_ERR_FILE_OFFSET, ret);
}

TEST(file_interface, CopyFileToBufferClosesTheFileAtTheEnd)
{		
	int ret;
	char buffer[100];	
	int buf_size = sizeof(buffer);
	int file_size;
	int i;
	
	for(i=0; i < 10000; i++){
		ret = FileIF_CopyFileToBuffer("file_interface/file_100bytes_counter.bin", 0, buffer, &buf_size, &file_size);
		
		TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, ret);
	}
}

TEST(file_interface, IsFileAvailableErrorIfFilenameIsNULL)
{
	TEST_ASSERT_EQUAL(FILEIF_ERR_INVALID_PARAM, FileIF_IsFileAvailable(NULL));
}

TEST(file_interface, IsFileAvailableErrorIfFileNotAvailable)
{
	TEST_ASSERT_EQUAL(FILEIF_ERR_FILE_NOT_AVAILABLE, FileIF_IsFileAvailable("file_interface/not_a_file"));
}

TEST(file_interface, IsFileAvailableSuccessIfFileAvailable)
{
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_IsFileAvailable("file_interface/file_available"));
}

TEST(file_interface, IsFileAvailableClosesTheFileBeforeReturning)
{
	int i;
	for(i=0; i < 10000; i++){
		TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_IsFileAvailable("file_interface/file_available"));
		TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_IsFileAvailable("file_interface/file_available"));
	}
}

TEST(file_interface, CreateFileGeneratesErrorIfFilenameIsNULL)
{
	TEST_ASSERT_EQUAL(FILEIF_ERR_INVALID_PARAM, FileIF_CreateFile(NULL));
}

TEST(file_interface, CreateFileGeneratesSuccessIfFileAvailable)
{
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_CreateFile("file_interface/file_available"));
}

TEST(file_interface, CreateFileCreatesTheFileIfFileNotAvailable)
{
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_CreateFile("file_interface/new_file"));
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_IsFileAvailable("file_interface/new_file"));
}

TEST(file_interface, CreateFileClosesTheFileAtTheEnd)
{
	int i;
	for(i=0; i < 10000; i++){
		TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_CreateFile("file_interface/new_file"));
	}
}

TEST(file_interface, DeleteFileGeneratesAnErrorIfFileNameIsNULL)
{
	TEST_ASSERT_EQUAL(FILEIF_ERR_INVALID_PARAM, FileIF_DeleteFile(NULL));
}

TEST(file_interface, DeleteFileGeneratesAnErrorIfFileIsNotAvailable)
{
	TEST_ASSERT_EQUAL(FILEIF_ERR_FILE_NOT_AVAILABLE, FileIF_DeleteFile("file_interface/not_a_file"));
}

TEST(file_interface, DeleteFileDeletesFileIfFileIsAvailable)
{
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_CreateFile("file_interface/new_file"));
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_DeleteFile("file_interface/new_file"));
	TEST_ASSERT_EQUAL(FILEIF_ERR_FILE_NOT_AVAILABLE, FileIF_IsFileAvailable("file_interface/new_file"));
}

IGNORE_TEST(file_interface, DeleteFileGeneratesAnErrorIfFileCannotBeRemoved)
{
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_CreateFile("file_interface/new_file"));
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_DeleteFile("file_interface/new_file"));
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_IsFileAvailable("file_interface/new_file"));
}

TEST(file_interface, GetFileSizeGeneratesAnErrorIfFilenameNULL)
{
	int size;
	TEST_ASSERT_EQUAL(FILEIF_ERR_INVALID_PARAM, FileIF_GetFileSize(NULL, &size));
}

TEST(file_interface, GetFileSizeGeneratesAnErrorIfFilesizeNULL)
{
	TEST_ASSERT_EQUAL(FILEIF_ERR_INVALID_PARAM, FileIF_GetFileSize("test", NULL));
}

TEST(file_interface, GetFileSizeGeneratesAnErrorIfFileCannotBeAccessed)
{
	int size;
	TEST_ASSERT_EQUAL(FILEIF_ERR_FILE_NOT_AVAILABLE, FileIF_GetFileSize("file_interface/not_a_file", &size));
}

TEST(file_interface, GetFileSizeCopiesTheFileSizeIfSuccess)
{
	int size;
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_GetFileSize("file_interface/file_20_bytes.bin", &size));
	TEST_ASSERT_EQUAL(20, size);
}

TEST(file_interface, FileIF_AppendStringErrorIfFilenameIsNULL)
{
	char value[] = "Hello";
	TEST_ASSERT_EQUAL(FILEIF_ERR_INVALID_PARAM, FileIF_AppendString(NULL, value));
}

TEST(file_interface, FileIF_AppendStringErrorIfStringIsNULL)
{
	char filename[] = "Hello";
	TEST_ASSERT_EQUAL(FILEIF_ERR_INVALID_PARAM, FileIF_AppendString(filename, NULL));
}

TEST(file_interface, FileIF_AppendStringErrorIfFileNotAvailable)
{
	char filename[] = "file_interface/not_a_file";
	char value[] = "Hello";
	TEST_ASSERT_EQUAL(FILEIF_ERR_FILE_NOT_AVAILABLE, FileIF_AppendString(filename, value));
}

TEST(file_interface, FileIF_AppendStringAppendsStringToEndOfTheFile)
{
	char filename[] = "file_interface/appendstring_test_file.txt";
	char value1[] = "Hello1\n";
	char value2[] = "Hello2\n";
	
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_CreateFile(filename));	
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_AppendString(filename, value1));
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_AppendString(filename, value2));
	TEST_ASSERT_EQUAL(1, CompareFiles(filename, "file_interface/appendstring_test_file_expected.txt"));
}

TEST(file_interface, FileIF_GetNoOfLinesIfFilenameIsNULL)
{
	int no_of_lines;
	TEST_ASSERT_EQUAL(FILEIF_ERR_INVALID_PARAM, FileIF_GetNoOfLines(NULL, &no_of_lines));
}

TEST(file_interface, FileIF_GetNoOfLinesIfFileNotAvailable)
{
	int no_of_lines;
	char filename[] = "file_interface/not_a_file";
	TEST_ASSERT_EQUAL(FILEIF_ERR_FILE_NOT_AVAILABLE, FileIF_GetNoOfLines(filename, &no_of_lines));
}

TEST(file_interface, FileIF_GetNoOfLinesIfNoOfLinesIsNULL)
{	
	char filename[] = "file_interface/file_20_bytes.bin";
	TEST_ASSERT_EQUAL(FILEIF_ERR_INVALID_PARAM, FileIF_GetNoOfLines(filename, NULL));
}

TEST(file_interface, FileIF_GetNoOfLinesReturnsNoOfLinesInTheFile)
{	
	char filename[] = "file_interface/line_8_file.txt";
	int no_of_lines = 0;
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_GetNoOfLines(filename, &no_of_lines));
	TEST_ASSERT_EQUAL(8,no_of_lines);
}

TEST(file_interface, FileIF_GetNoOfLinesReturnsZeroAsNoOfLinesForEmptyFile)
{	
	char filename[] = "file_interface/empty_file.txt";
	int no_of_lines = 0;
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_GetNoOfLines(filename, &no_of_lines));
	TEST_ASSERT_EQUAL(0,no_of_lines);
}

TEST(file_interface, FileIF_ReadLineErrorIfFilenameIsNULL)
{	
	char string_val[100];
	int string_size = sizeof(string_val);
	
	TEST_ASSERT_EQUAL(FILEIF_ERR_INVALID_PARAM, FileIF_ReadLine(NULL, 1, string_val, &string_size));	
}

TEST(file_interface, FileIF_ReadLineErrorIfLineBufferIsNULL)
{	
	char filename[] = "file_interface/empty_file.txt";	
	int string_size = 100;
	
	TEST_ASSERT_EQUAL(FILEIF_ERR_INVALID_PARAM, FileIF_ReadLine(filename, 1, NULL, &string_size));	
}

TEST(file_interface, FileIF_ReadLineErrorIfBufferSizeIsNULL)
{	
	char filename[] = "file_interface/empty_file.txt";	
	char string_val[100];	
	
	TEST_ASSERT_EQUAL(FILEIF_ERR_INVALID_PARAM, FileIF_ReadLine(filename, 1, string_val, NULL));	
}

TEST(file_interface, FileIF_ReadLineErrorIfBufferSizeIsSmall)
{	
	char filename[] = "file_interface/file_with_30char_line.txt";	
	char string_val[10];	
	int string_size = sizeof(string_val);
		
	TEST_ASSERT_EQUAL(FILEIF_ERR_BUFFER_SIZE, FileIF_ReadLine(filename, 1, string_val, &string_size));	
}

TEST(file_interface, FileIF_ReadLineReturnsTheRequiredBufferSizeIfBufferSizeIsSmall)
{	
	char filename[] = "file_interface/file_with_30char_line.txt";	
	char string_val[10];	
	int string_size = sizeof(string_val);
		
	TEST_ASSERT_EQUAL(FILEIF_ERR_BUFFER_SIZE, FileIF_ReadLine(filename, 1, string_val, &string_size));	
	TEST_ASSERT_EQUAL(31,string_size);
}


TEST(file_interface, FileIF_ReadLineReturnsCopiedDataAmountIfFunctionIsSuccessful)
{	
	char filename[] = "file_interface/file_with_30char_line.txt";	
	char string_val[60];	
	int string_size = sizeof(string_val);
		
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_ReadLine(filename, 1, string_val, &string_size));	
	TEST_ASSERT_EQUAL(31,string_size);
}

TEST(file_interface, FileIF_ReadLineErrorIfFileNotAccessible)
{	
	char filename[] = "file_interface/not_a_file";	
	char string_val[60];	
	int string_size = sizeof(string_val);
		
	TEST_ASSERT_EQUAL(FILEIF_ERR_FILE_NOT_AVAILABLE, FileIF_ReadLine(filename, 1, string_val, &string_size));	
}

TEST(file_interface, FileIF_ReadLineErrorIfLineNumberIsNegetive)
{	
	char filename[] = "file_interface/file_with_30char_line.txt";	
	char string_val[60];	
	int string_size = sizeof(string_val);
		
	TEST_ASSERT_EQUAL(FILEIF_ERR_LINE_NO, FileIF_ReadLine(filename, -1, string_val, &string_size));	
}

TEST(file_interface, FileIF_ReadLineErrorIfLineNumberIsZero)
{	
	char filename[] = "file_interface/file_with_30char_line.txt";	
	char string_val[60];	
	int string_size = sizeof(string_val);
		
	TEST_ASSERT_EQUAL(FILEIF_ERR_LINE_NO, FileIF_ReadLine(filename, 0, string_val, &string_size));	
}

TEST(file_interface, FileIF_ReadLineErrorIfLineNumberIsMore)
{	
	char filename[] = "file_interface/file_with_30char_line.txt";	
	char string_val[60];	
	int string_size = sizeof(string_val);
		
	TEST_ASSERT_EQUAL(FILEIF_ERR_LINE_NO, FileIF_ReadLine(filename, 500, string_val, &string_size));	
}

TEST(file_interface, FileIF_ReadLineECopiesTheStringIfSuccessful)
{	
	char filename[] = "file_interface/file_with_30char_line.txt";	
	char string_val[60];	
	char expect[] = "1234567890asdfghjklzxcvbnmqwer\n";
	int string_size = sizeof(string_val);
		
	TEST_ASSERT_EQUAL(FILEIF_OP_SUCCESS, FileIF_ReadLine(filename, 1, string_val, &string_size));	
	TEST_ASSERT_EQUAL(31,string_size);
	TEST_ASSERT_EQUAL_MEMORY(expect, string_val, sizeof(expect));
}
