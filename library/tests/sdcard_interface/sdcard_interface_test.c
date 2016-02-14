
#include "unity_fixture.h"
#include "sdcard_interface.h"
#include "test_helper.h"
#include <string.h>

extern char sdcardif_initialized;

extern char *audio_buffer;
extern int audio_buffer_size;

extern char event_log_file[MAX_FILENAME_SIZE];

TEST_GROUP(sdcard_interface);

TEST_SETUP(sdcard_interface)
{
    
}

TEST_TEAR_DOWN(sdcard_interface)
{
	SDCardIF_Reset();
	remove("sdcard_interface/event_file_not_available.dat");
	remove("sdcard_interface/event_file1.txt");
	remove("sdcard_interface/event_file2.txt");
	remove("sdcard_interface/empty_firmware.bin");
	remove("sdcard_interface/data_append");
	remove(DEFAULT_EVENT_LOG);
		
}

TEST(sdcard_interface, InitFunctionSetsTheEventLogFileToDefault)
{	SDCardIF_Initialize();
	
	TEST_ASSERT_EQUAL_STRING(DEFAULT_EVENT_LOG, event_log_file);
}

TEST(sdcard_interface, SetAudioFileBufferErrorIfInitIsNotCalled)
{
	int ret;
	char buffer;
	char *bufferp = &buffer;

	ret = SDCardIF_SetAudioFileBuffer(bufferp, 1);

	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_NOT_INITIALIZED, ret);
}

TEST(sdcard_interface, SetAudioFileBufferReturnsErrorIfBufferPointerIsNull)
{
	int ret;

	SDCardIF_Initialize();
	ret = SDCardIF_SetAudioFileBuffer(NULL, 1);

	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM,ret);
}

TEST(sdcard_interface, SetAudioFileBufferReturnsErrorIfBufferSizeIsZero)
{
	int ret;
	char buffer;
	
	SDCardIF_Initialize();
	ret = SDCardIF_SetAudioFileBuffer(&buffer, 0);

	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_BUFFER_SIZE, ret);
}

TEST(sdcard_interface, SetAudioFileBufferReturnsErrorIfBufferSizeIsLessthanZero)
{
	int ret;
	char buffer;
	
	SDCardIF_Initialize();
	ret = SDCardIF_SetAudioFileBuffer(&buffer, -20);

	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_BUFFER_SIZE, ret);
}

TEST(sdcard_interface, SetAudioFileBufferReturnsSuccessIfParametersAreCorrectAndInitCalled)
{
	int ret;
	char buffer;

	SDCardIF_Initialize();
	ret = SDCardIF_SetAudioFileBuffer(&buffer, 1);
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
}

TEST(sdcard_interface, SetAudioFileBufferSetsInternalBufferPointer)
{
	int ret;
	char buffer[10];
	char expected[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

	SDCardIF_Initialize();
	ret = SDCardIF_SetAudioFileBuffer(buffer, 10);
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);

	ret = SDCardIF_PlayAudioFile("sdcard_interface/one_to_ten.wav");

	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);

	TEST_ASSERT_EQUAL_MEMORY(expected, buffer, 10);
}

TEST(sdcard_interface, SetAudioFileBufferSetsInternalBufferSize)
{
	int ret;
	char buffer[10];

	SDCardIF_Initialize();
	ret = SDCardIF_SetAudioFileBuffer(buffer, 10);
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);

	TEST_ASSERT_EQUAL(audio_buffer_size, sizeof(buffer));
}

TEST(sdcard_interface, PlayAudioFileErrorIfInitIsNotCalled)
{
	int ret;	

	ret = SDCardIF_PlayAudioFile("sdcard_interface/one_to_twenty.wav");

	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_NOT_INITIALIZED, ret);
}

TEST(sdcard_interface, PlayAudioFileErrorIfFileNameIsNULL)
{
	int ret;	
	char buffer[10];
	
	SDCardIF_Initialize();
	
	ret = SDCardIF_SetAudioFileBuffer(buffer, 10);
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
	
	ret = SDCardIF_PlayAudioFile(NULL);

	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM, ret);
}

TEST(sdcard_interface, PlayAudioFileErrorIfFileNotAvailable)
{
	int ret;	
	char buffer[10];
	
	SDCardIF_Initialize();
	
	ret = SDCardIF_SetAudioFileBuffer(buffer, 10);
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
	
	ret = SDCardIF_PlayAudioFile("sdcard_interface/file_not_there");

	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_FILE_NOT_AVAILABLE, ret);
}

TEST(sdcard_interface, PlayAudioFileWarnIfFileBiggerThanBuffer)
{
	int ret;	
	char buffer[10];
	
	SDCardIF_Initialize();
	
	ret = SDCardIF_SetAudioFileBuffer(buffer, 10);
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
	
	ret = SDCardIF_PlayAudioFile("sdcard_interface/one_to_twenty.wav");

	TEST_ASSERT_EQUAL(SDCARD_IF_WARN_BUFFER_SIZE_SMALL, ret);
}


TEST(sdcard_interface, PlayAudioFileCopyOnlyBufferSizeIfFileIsBiggerThanBuffer)
{
	int ret;	
	char buffer[10];
	char expected[10] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
	SDCardIF_Initialize();
	
	ret = SDCardIF_SetAudioFileBuffer(buffer, 10);
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
	
	ret = SDCardIF_PlayAudioFile("sdcard_interface/one_to_twenty.wav");

	TEST_ASSERT_EQUAL(SDCARD_IF_WARN_BUFFER_SIZE_SMALL, ret);
	
	TEST_ASSERT_EQUAL_MEMORY(expected, buffer, 10);
}

TEST(sdcard_interface, PlayAudioFileWarnIfFileSmallerThanBuffer)
{
	int ret;	
	char buffer[20];
	
	SDCardIF_Initialize();
	
	ret = SDCardIF_SetAudioFileBuffer(buffer, 20);
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
	
	ret = SDCardIF_PlayAudioFile("sdcard_interface/one_to_ten.wav");

	TEST_ASSERT_EQUAL(SDCARD_IF_WARN_BUFFER_SIZE_LARGE, ret);
}

TEST(sdcard_interface, PlayAudioFileCopyFullFileContentIfFileIfSmallerThanBuffer)
{
	int ret;	
	char buffer[20];
	char expected[20] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
	SDCardIF_Initialize();
		
	ret = SDCardIF_SetAudioFileBuffer(buffer, 20);
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
	
	ret = SDCardIF_PlayAudioFile("sdcard_interface/one_to_ten.wav");

	TEST_ASSERT_EQUAL(SDCARD_IF_WARN_BUFFER_SIZE_LARGE, ret);
	
	TEST_ASSERT_EQUAL_MEMORY(expected, buffer, 10);
}

TEST(sdcard_interface, PlayAudioFileErrorIfBufferIsNULL)
{
	int ret;	

	SDCardIF_Initialize();
	
	ret = SDCardIF_PlayAudioFile("sdcard_interface/one_to_ten.wav");

	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_AUDIO_BUFFER_NOT_SET, ret);
}

TEST(sdcard_interface, PlayAudioFileErrorIfBufferSizeIsZero)
{
	int ret;	

	SDCardIF_Initialize();
	
	ret = SDCardIF_PlayAudioFile("sdcard_interface/one_to_ten.wav");

	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_AUDIO_BUFFER_NOT_SET, ret);
}

TEST(sdcard_interface, PlayAudioFileCopiesDataCorrectly)
{
	int ret;	
	char buffer[8*1024];
	char expected[8*1024];
	int i;
	SDCardIF_Initialize();
		
	ret = SDCardIF_SetAudioFileBuffer(buffer, sizeof(buffer));
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
	
	ret = SDCardIF_PlayAudioFile("sdcard_interface/8k_file.wav");

	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
	
	for(i=0; i < 8*1024; i++){
		expected[i] = i%250;
	}
	
	TEST_ASSERT_EQUAL_MEMORY(expected, buffer, sizeof(buffer));
}

TEST(sdcard_interface, InitFunctionClearsBufferPointerAndSize)
{
	int ret;	
	char buffer[10];
	SDCardIF_Initialize();
		
	ret = SDCardIF_SetAudioFileBuffer(buffer, sizeof(buffer));
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
	
	SDCardIF_Initialize();

	TEST_ASSERT_NULL(audio_buffer);

	TEST_ASSERT_EQUAL(0, audio_buffer_size);
}


TEST(sdcard_interface, SetLogFileErrorIfFileNameIsNULL)
{
	int ret;
	
	SDCardIF_Initialize();
	
	ret = SDCardIF_SetLogFile(NULL);
	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM, ret);
}

TEST(sdcard_interface, SetLogFileErrorIfInitIsNotCalled)
{
	int ret;
	
	ret = SDCardIF_SetLogFile(NULL);
	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_NOT_INITIALIZED, ret);
}

TEST(sdcard_interface, SetLogFileReturnsSuccessIfFileIsAvailable)
{
	int ret;
	
	SDCardIF_Initialize();
	
	ret = SDCardIF_SetLogFile("sdcard_interface/event_file_available.dat");
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
	
	TEST_ASSERT_EQUAL(1, IsFileAvailable("sdcard_interface/event_file_available.dat"));
}


TEST(sdcard_interface, SetLogFileReturnsSuccessIfFileIsNotAvailableAndCreateFile)
{
	int ret;
	
	SDCardIF_Initialize();
	
	ret = SDCardIF_SetLogFile("sdcard_interface/event_file_not_available.dat");
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
	
	TEST_ASSERT_EQUAL(1, IsFileAvailable("sdcard_interface/event_file_not_available.dat"));
}


TEST(sdcard_interface, SetLogFileSetsTheLogFile)
{
	int ret;
	
	SDCardIF_Initialize();
	
	ret = SDCardIF_SetLogFile("sdcard_interface/event_file_not_available.dat");
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
	
	TEST_ASSERT_EQUAL_STRING("sdcard_interface/event_file_not_available.dat", event_log_file);
}

TEST(sdcard_interface, SetLogFileDoesNotChangeLogFileIfErrored)
{
	char buffer[MAX_FILENAME_SIZE];
	
	SDCardIF_Initialize();
	
	memcpy(buffer, event_log_file, sizeof(event_log_file));
	
	SDCardIF_SetLogFile(NULL);
	
	TEST_ASSERT_EQUAL_STRING(buffer, event_log_file);
}

TEST(sdcard_interface, SetLogFileErrorIfFilenameTooLarge)
{
	int ret;
	char filename[MAX_FILENAME_SIZE+10];
	int i;
	
	for(i = 0; i < MAX_FILENAME_SIZE+1 ; i++){
		filename[i] = 'a';		
	}
	
	SDCardIF_Initialize();
	
	ret = SDCardIF_SetLogFile(filename);
	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM, ret);
}

TEST(sdcard_interface, DeleteLogFileGeneratesErrorIfFilenameIsNULL)
{
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM , SDCardIF_DeleteLogFile(NULL));
}

TEST(sdcard_interface, DeleteLogFileGeneratesErrorIfFileNotAvailable)
{
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_FILE_NOT_AVAILABLE , SDCardIF_DeleteLogFile("sdcard_interface/event_file_not_available.dat"));
}

TEST(sdcard_interface, DeleteLogFileDeletesTheFileIfFileIsAvailable)
{
	int ret;
	
	SDCardIF_Initialize();
	
	ret = SDCardIF_SetLogFile("sdcard_interface/new_file_to_delete.dat");
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
	TEST_ASSERT_EQUAL(1, IsFileAvailable("sdcard_interface/new_file_to_delete.dat"));
	
	ret = SDCardIF_DeleteLogFile("sdcard_interface/new_file_to_delete.dat");
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
	TEST_ASSERT_EQUAL(0, IsFileAvailable("sdcard_interface/new_file_to_delete.dat"));
}

TEST(sdcard_interface, DeleteLogFileSetsTheDefaultLogFileIfItDeletesTheCurrentLogFile)
{
	int ret;
	
	SDCardIF_Initialize();
	
	ret = SDCardIF_SetLogFile("sdcard_interface/new_file_to_delete.dat");
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
	TEST_ASSERT_EQUAL(1, IsFileAvailable("sdcard_interface/new_file_to_delete.dat"));
	
	ret = SDCardIF_DeleteLogFile("sdcard_interface/new_file_to_delete.dat");
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
	TEST_ASSERT_EQUAL(0, IsFileAvailable("sdcard_interface/new_file_to_delete.dat"));
	
	TEST_ASSERT_EQUAL_STRING(DEFAULT_EVENT_LOG, event_log_file);
}

TEST(sdcard_interface, GetCurrentLogFileGeneratesErrorIfFilenameIsNULL)
{
	int size = 10;
	SDCardIF_Initialize();
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM , SDCardIF_GetCurrentLogFile(NULL, &size));
}

TEST(sdcard_interface, GetCurrentLogFileGeneratesErrorIfBufferSizeIsLessThanOrEqualsToZero)
{
	char buffer;
	int size = -5;
	SDCardIF_Initialize();
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_BUFFER_SIZE , SDCardIF_GetCurrentLogFile(&buffer, &size));
}

TEST(sdcard_interface, GetCurrentLogFileGeneratesErrorIfBufferSizeIsNotEnough)
{	
	char buffer;
	int size = sizeof(buffer);
	SDCardIF_Initialize();
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_BUFFER_SIZE , SDCardIF_GetCurrentLogFile(&buffer, &size));
}

TEST(sdcard_interface, GetCurrentLogFileGeneratesErrorIfModuleNotInitialized)
{	
	char buffer[MAX_FILENAME_SIZE];
	int size = sizeof(buffer);
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_NOT_INITIALIZED , SDCardIF_GetCurrentLogFile(buffer, &size));
}

TEST(sdcard_interface, ReadFirmwareFileGeneratesErrorIfFilenameIsNULL)
{
	char buffer;
	int file_size;
	int buf_size = sizeof(buffer);
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM , SDCardIF_ReadFirmwareFile(NULL, 0, &buffer, &buf_size, &file_size));
}

TEST(sdcard_interface, ReadFirmwareFileGeneratesErrorIfBufferIsNULL)
{
	char buffer;
	int file_size;
	int buf_size = sizeof(buffer);
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM , SDCardIF_ReadFirmwareFile("sdcard_interface/10k_firmware.bin", 0, NULL, &buf_size, &file_size));
}

TEST(sdcard_interface, ReadFirmwareFileGeneratesErrorIfBufferSizeIsNULL)
{
	char buffer;
	int file_size;
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM , SDCardIF_ReadFirmwareFile("sdcard_interface/10k_firmware.bin", 0, &buffer, NULL, &file_size));
}

TEST(sdcard_interface, ReadFirmwareFileGeneratesErrorIfBufferSizeLessThanOrEqualsToZero)
{
	char buffer;
	int file_size;
	int buf_size = -1;
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_BUFFER_SIZE , SDCardIF_ReadFirmwareFile("sdcard_interface/10k_firmware.bin", 0, &buffer, &buf_size, &file_size));
}

TEST(sdcard_interface, ReadFirmwareFileGeneratesErrorIfProvidedOffsetIsLessThanZero)
{
	char buffer;
	int file_size;
	int buf_size = sizeof(buffer);
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_BUFFER_OFFSET , SDCardIF_ReadFirmwareFile("sdcard_interface/10k_firmware.bin", -1, &buffer, &buf_size, &file_size));
}

TEST(sdcard_interface, ReadFirmwareFileGeneratesErrorIfProvidedOffsetIsMoreThanFilesize)
{
	char buffer[8*1024];
	int file_size;
	int buf_size = sizeof(buffer);
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_BUFFER_OFFSET , SDCardIF_ReadFirmwareFile("sdcard_interface/10k_firmware.bin", 11*1024, buffer, &buf_size, &file_size));
}

TEST(sdcard_interface, ReadFirmwareFileGeneratesErrorIfFilesizeParameterNULL)
{
	char buffer[8*1024];
	int buf_size = sizeof(buffer);
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM , SDCardIF_ReadFirmwareFile("sdcard_interface/10k_firmware.bin", 11*1024, buffer, &buf_size, NULL));
}

TEST(sdcard_interface, ReadFirmwareFileGeneratesCopiesFilesizeWhenBufferSizeIsInvalid)
{
	char buffer[8*1024];
	int buf_size = 0;
	int file_size;
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_BUFFER_SIZE , SDCardIF_ReadFirmwareFile("sdcard_interface/10k_firmware.bin", 0, buffer, &buf_size, &file_size));
	TEST_ASSERT_EQUAL(10*1024 , file_size);
}

TEST(sdcard_interface, ReadFirmwareFileGeneratesCopiesFilesizeWhenBufferOffsetIsMore)
{
	char buffer[8*1024];
	int file_size = 0;
	int buf_size = sizeof(buffer);
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_BUFFER_OFFSET , SDCardIF_ReadFirmwareFile("sdcard_interface/10k_firmware.bin", 11*1024, buffer, &buf_size, &file_size));
	TEST_ASSERT_EQUAL(10*1024 , file_size);
}

TEST(sdcard_interface, ReadFirmwareFileGeneratesCopiesFilesizeWhenBufferOffsetIsInvalid)
{
	char buffer[8*1024];
	int file_size = 0;
	int buf_size = sizeof(buffer);
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_BUFFER_OFFSET , SDCardIF_ReadFirmwareFile("sdcard_interface/10k_firmware.bin", -1, buffer, &buf_size, &file_size));
	TEST_ASSERT_EQUAL(10*1024 , file_size);
}

TEST(sdcard_interface, ReadFirmwareFileGeneratesCopiesFilesizeWhenSuccess)
{
	char buffer[20*1024];
	int file_size;
	int buf_size = sizeof(buffer);
	TEST_ASSERT_EQUAL(SDCARD_IF_WARN_BUFFER_SIZE_LARGE , SDCardIF_ReadFirmwareFile("sdcard_interface/10k_firmware.bin", 0, buffer, &buf_size, &file_size));
	TEST_ASSERT_EQUAL(10*1024 , file_size);
}

TEST(sdcard_interface, ReadFirmwareFileGeneratesCopiesCopiedAmountToBufSize)
{
	char buffer[20*1024];
	int file_size;
	int buf_size = sizeof(buffer);
	TEST_ASSERT_EQUAL(SDCARD_IF_WARN_BUFFER_SIZE_LARGE , SDCardIF_ReadFirmwareFile("sdcard_interface/10k_firmware.bin", 0, buffer, &buf_size, &file_size));
	TEST_ASSERT_EQUAL(10*1024 , buf_size);
}

TEST(sdcard_interface, ReadFirmwareFileGeneratesStartsDataCopyingAtTheOffset)
{
	char buffer[20];
	char expected[20] = {0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33};
	int file_size;
	int buf_size = sizeof(buffer);
	TEST_ASSERT_EQUAL(SDCARD_IF_WARN_BUFFER_SIZE_SMALL , SDCardIF_ReadFirmwareFile("sdcard_interface/10k_firmware.bin", 0x20, buffer, &buf_size, &file_size));
	TEST_ASSERT_EQUAL(20, buf_size);
	TEST_ASSERT_EQUAL(10*1024, file_size);
	TEST_ASSERT_EQUAL_MEMORY(expected, buffer, 20);
}

TEST(sdcard_interface, ReadFirmwareFileCopiesDataUntilBufferIsFullWhenFileIsBiggerAndGeneratesWarning)
{
	char buffer[20];
	char expected[20] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13};
	int file_size;
	int buf_size = sizeof(buffer);
	TEST_ASSERT_EQUAL(SDCARD_IF_WARN_BUFFER_SIZE_SMALL , SDCardIF_ReadFirmwareFile("sdcard_interface/10k_firmware.bin", 0x0, buffer, &buf_size, &file_size));
	TEST_ASSERT_EQUAL(20, buf_size);
	TEST_ASSERT_EQUAL(10*1024, file_size);
	TEST_ASSERT_EQUAL_MEMORY(expected, buffer, 20);
}

TEST(sdcard_interface, ReadFirmwareFileCopiesDataUntilBufferIsFullWhenFileIsSmallerAndGeneratesWarning)
{
	char buffer[20];	
	char expected[20];
	int file_size;
	int buf_size = sizeof(buffer);
	int i;
	TEST_ASSERT_EQUAL(SDCARD_IF_WARN_BUFFER_SIZE_LARGE , SDCardIF_ReadFirmwareFile("sdcard_interface/one_to_ten.wav", 0x0, buffer, &buf_size, &file_size));
	
	
	memset(expected, 0x00, 20);
	for(i = 0; i<10; i++){
		expected[i] = i;
	}
	
	TEST_ASSERT_EQUAL_MEMORY(expected, buffer, 20);
}



TEST(sdcard_interface, GetCurrentLogCopiesTheFilenameToTheProvidedBuffer)
{	
	char buffer[MAX_FILENAME_SIZE];
	int size = sizeof(buffer);
	SDCardIF_Initialize();
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS , SDCardIF_GetCurrentLogFile(buffer, &size));	
	TEST_ASSERT_EQUAL_STRING(DEFAULT_EVENT_LOG, buffer);
}

TEST(sdcard_interface, LogEventReturnsErrorIfInitNotCalled)
{		
	ITSI_LOG_EVENT event;
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_NOT_INITIALIZED , SDCardIF_LogEvent(&event));	
}

TEST(sdcard_interface, LogEventReturnsErrorIfEventsStructureIsNULL)
{		
	SDCardIF_Initialize();
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM , SDCardIF_LogEvent(NULL));	
}

TEST(sdcard_interface, LogEventOpenFileIfAvailableAndAppendsEvent)
{		
	ITSI_LOG_EVENT event;
	
	event.length = 0;
	event.day = 29;
	event.month = 1;
	event.year = 16;
	event.hour = 22;
	event.minute = 6;
	event.second = 35;
	event.id3 = 33;
	event.id2 = 22;
	event.id1 = 11;
	event.event_no = 1;
	event.crc_msb = 88;
	event.crc_lsb = 99;
	
	SDCardIF_Initialize();
	
	TEST_ASSERT_EQUAL(0, IsFileAvailable("sdcard_interface/event_file1.txt"));
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, SDCardIF_SetLogFile("sdcard_interface/event_file1.txt"));
	TEST_ASSERT_EQUAL(1, IsFileAvailable("sdcard_interface/event_file1.txt"));
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS , SDCardIF_LogEvent(&event));
	TEST_ASSERT_EQUAL(1,CompareFiles("sdcard_interface/event_file1.txt","sdcard_interface/event_file1_expected.txt"));
}

TEST(sdcard_interface, LogEventAppendsEventsInNewLines)
{		
	ITSI_LOG_EVENT event;
	
	event.length = 0;
	event.day = 29;
	event.month = 1;
	event.year = 16;
	event.hour = 22;
	event.minute = 6;
	event.second = 35;
	event.id3 = 33;
	event.id2 = 22;
	event.id1 = 11;
	event.event_no = 1;
	event.crc_msb = 88;
	event.crc_lsb = 99;
	
	SDCardIF_Initialize();
	
	TEST_ASSERT_EQUAL(0, IsFileAvailable("sdcard_interface/event_file2.txt"));
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, SDCardIF_SetLogFile("sdcard_interface/event_file2.txt"));
	TEST_ASSERT_EQUAL(1, IsFileAvailable("sdcard_interface/event_file2.txt"));
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS , SDCardIF_LogEvent(&event));
	
	event.length = 0;
	event.day = 29;
	event.month = 1;
	event.year = 16;
	event.hour = 23;
	event.minute = 17;
	event.second = 42;
	event.id3 = 11;
	event.id2 = 54;
	event.id1 = 11;
	event.event_no = 2;
	event.crc_msb = 38;
	event.crc_lsb = 79;
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS , SDCardIF_LogEvent(&event));
	
	TEST_ASSERT_EQUAL(1,CompareFiles("sdcard_interface/event_file2.txt","sdcard_interface/event_file2_expected.txt"));
}

TEST(sdcard_interface, ReadEventReturnsErrorIfEventsStructureIsNULL)
{		
	ITSI_LOG_EVENT event;
	char read_type = 0;
	int no_of_events = 1;
	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM , SDCardIF_ReadEventLog(NULL, &event, read_type, &no_of_events));
}

TEST(sdcard_interface, ReadEventReturnsErrorIfFileNotAvailable)
{		
	ITSI_LOG_EVENT event;
	char read_type = 0;
	int no_of_events = 1;
	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_FILE_NOT_AVAILABLE , SDCardIF_ReadEventLog("sdcard_interface/event_file_not_available.dat", &event, read_type, &no_of_events));
}

TEST(sdcard_interface, ReadEventReturnsErrorIfNoOfEventsParamIsNULL)
{		
	ITSI_LOG_EVENT event;
	char read_type = 0;	
	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM , SDCardIF_ReadEventLog("sdcard_interface/event_file_not_available.dat", &event, read_type, NULL));
}

TEST(sdcard_interface, ReadEventReturnsErrorIfEventBufferIsNULL)
{		
	char read_type = 0;	
	int no_of_events = 1;
	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM , SDCardIF_ReadEventLog("sdcard_interface/event_file_not_available.dat", NULL, read_type, &no_of_events));
}

TEST(sdcard_interface, ReadEventReturnsErrorIfReadTypeIsInvalid)
{		
	char read_type = -1;	
	int no_of_events = 40;
	ITSI_LOG_EVENT event[40];
	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM , SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
	
	read_type = 4;
	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM , SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
	
	read_type = 200;
	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM , SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
}

TEST(sdcard_interface, ReadEventReturnsWarningIfRequiredNoOfEventsByParameterAreNotAvailable)
{		
	char read_type = 0;	
	int no_of_events = 40;
	ITSI_LOG_EVENT event[40];
	
	TEST_ASSERT_EQUAL(SDCARD_IF_WARN_LESS_NO_Of_EVENTS_AVAIL, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
}

TEST(sdcard_interface, ReadEventReturnsErrorIfNoOfEventsIsZeroUnlessReadTypeLast100)
{		
	READ_TYPE read_type = N_FROM_BEGINING;	
	int no_of_events = 0;
	ITSI_LOG_EVENT event[100];
	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_EVENT_COUNT, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
	
	no_of_events = 0;
	read_type = N_FROM_LAST;
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_EVENT_COUNT, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
	
	no_of_events = 0;
	read_type = FULL_READ;
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_EVENT_COUNT, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
	
	no_of_events = 0;
	read_type = LAST_100;
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_1000events.txt", event, read_type, &no_of_events));
}

TEST(sdcard_interface, ReadEventReturnsErrorIfNoOfEventsIsZeroReturnsRequiredAmount)
{		
	READ_TYPE read_type = N_FROM_BEGINING;	
	int no_of_events = 0;
	ITSI_LOG_EVENT event[100];
	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_EVENT_COUNT, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
	TEST_ASSERT_EQUAL(10, no_of_events);
	
	no_of_events = 0;
	read_type = N_FROM_LAST;
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_EVENT_COUNT, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
	TEST_ASSERT_EQUAL(10, no_of_events);
	
	no_of_events = 0;
	read_type = FULL_READ;
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_EVENT_COUNT, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
	TEST_ASSERT_EQUAL(10, no_of_events);
	
	no_of_events = 0;
	read_type = LAST_100;
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_1000events.txt", event, read_type, &no_of_events));
	TEST_ASSERT_EQUAL(100, no_of_events);
}

TEST(sdcard_interface, ReadEventReturnsWarningIf100EventsNotAvailableWhenLast100Used)
{		
	READ_TYPE read_type = LAST_100;	
	int no_of_events = 0;
	ITSI_LOG_EVENT event[40];
	
	read_type = LAST_100;
	TEST_ASSERT_EQUAL(SDCARD_IF_WARN_LESS_NO_Of_EVENTS_AVAIL, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
}

TEST(sdcard_interface, ReadEventReturnsReadNumberOfEventsWhenLessEventsAreThereForLast100)
{		
	READ_TYPE read_type = LAST_100;	
	int no_of_events = 0;
	ITSI_LOG_EVENT event[40];
	
	read_type = LAST_100;
	TEST_ASSERT_EQUAL(SDCARD_IF_WARN_LESS_NO_Of_EVENTS_AVAIL, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
	TEST_ASSERT_EQUAL(10, no_of_events);
}

TEST(sdcard_interface, ReadEventReturnsNoOfEventsAs100IfEventsAreThereForLast100)
{		
	READ_TYPE read_type = LAST_100;	
	int no_of_events = 0;
	ITSI_LOG_EVENT event[100];
	
	read_type = LAST_100;
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_1000events.txt", event, read_type, &no_of_events));
	TEST_ASSERT_EQUAL(100, no_of_events);
}

TEST(sdcard_interface, ReadEventCopiesAvailableEventsIf100EventsAreNotThereForLast100)
{		
	READ_TYPE read_type = LAST_100;	
	int no_of_events = 0;
	ITSI_LOG_EVENT event[10];
	ITSI_LOG_EVENT expected_event[10];
	
	char val[] = {0,29,1,16,22,6,35,33,22,11,1,88,99, 
					0,29,1,16,23,6,35,33,22,11,2,88,99,
					0,30,1,16,24,6,35,33,22,11,3,88,99, 
					0,30,1,16,25,6,35,33,22,11,4,88,99,  
					0,31,1,16,26,6,35,33,22,11,5,88,99, 
					0,31,1,16,27,6,35,33,22,11,6,88,99, 
					0,01,2,16,28,6,35,33,22,11,7,88,99, 
					0,01,2,16,29,6,35,33,22,11,8,88,99, 
					0,02,2,16,20,6,35,33,22,11,9,88,99, 
					0,03,2,16,28,6,35,33,22,11,10,88,99};
					
	memcpy(expected_event, val, sizeof(expected_event));
	
	read_type = LAST_100;
	TEST_ASSERT_EQUAL(SDCARD_IF_WARN_LESS_NO_Of_EVENTS_AVAIL, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
	TEST_ASSERT_EQUAL(10, no_of_events);
	TEST_ASSERT_EQUAL_MEMORY(expected_event, event, sizeof(expected_event));
}

TEST(sdcard_interface, ReadEventCopies100EventsIf100EventsAreThereForLast100)
{					
	READ_TYPE read_type = LAST_100;	
	
	ITSI_LOG_EVENT expected_event[100];
	ITSI_LOG_EVENT event[100];
	int no_of_events = 0;
	
	char val[] = {0,29,1,16,22,6,35,33,22,11,1,88,99,
					0,29,1,16,23,6,35,33,22,11,2,88,99,
					0,30,1,16,24,6,35,33,22,11,3,88,99,
					0,30,1,16,25,6,35,33,22,11,4,88,99,
					0,31,1,16,26,6,35,33,22,11,5,88,99,
					0,31,1,16,27,6,35,33,22,11,6,88,99,
					0,01,2,16,28,6,35,33,22,11,7,88,99,
					0,01,2,16,29,6,35,33,22,11,8,88,99,
					0,02,2,16,20,6,35,33,22,11,9,88,99,
					0,03,2,16,28,6,35,33,22,11,10,88,99,
					0,29,1,16,22,6,35,33,22,11,1,88,99,
					0,29,1,16,23,6,35,33,22,11,2,88,99,
					0,30,1,16,24,6,35,33,22,11,3,88,99,
					0,30,1,16,25,6,35,33,22,11,4,88,99,
					0,31,1,16,26,6,35,33,22,11,5,88,99,
					0,31,1,16,27,6,35,33,22,11,6,88,99,
					0,01,2,16,28,6,35,33,22,11,7,88,99,
					0,01,2,16,29,6,35,33,22,11,8,88,99,
					0,02,2,16,20,6,35,33,22,11,9,88,99,
					0,03,2,16,28,6,35,33,22,11,10,88,99,
					0,29,1,16,22,6,35,33,22,11,1,88,99,
					0,29,1,16,23,6,35,33,22,11,2,88,99,
					0,30,1,16,24,6,35,33,22,11,3,88,99,
					0,30,1,16,25,6,35,33,22,11,4,88,99,
					0,31,1,16,26,6,35,33,22,11,5,88,99,
					0,31,1,16,27,6,35,33,22,11,6,88,99,
					0,01,2,16,28,6,35,33,22,11,7,88,99,
					0,01,2,16,29,6,35,33,22,11,8,88,99,
					0,02,2,16,20,6,35,33,22,11,9,88,99,
					0,03,2,16,28,6,35,33,22,11,10,88,99,
					0,29,1,16,22,6,35,33,22,11,1,88,99,
					0,29,1,16,23,6,35,33,22,11,2,88,99,
					0,30,1,16,24,6,35,33,22,11,3,88,99,
					0,30,1,16,25,6,35,33,22,11,4,88,99,
					0,31,1,16,26,6,35,33,22,11,5,88,99,
					0,31,1,16,27,6,35,33,22,11,6,88,99,
					0,01,2,16,28,6,35,33,22,11,7,88,99,
					0,01,2,16,29,6,35,33,22,11,8,88,99,
					0,02,2,16,20,6,35,33,22,11,9,88,99,
					0,03,2,16,28,6,35,33,22,11,10,88,99,
					0,29,1,16,22,6,35,33,22,11,1,88,99,
					0,29,1,16,23,6,35,33,22,11,2,88,99,
					0,30,1,16,24,6,35,33,22,11,3,88,99,
					0,30,1,16,25,6,35,33,22,11,4,88,99,
					0,31,1,16,26,6,35,33,22,11,5,88,99,
					0,31,1,16,27,6,35,33,22,11,6,88,99,
					0,01,2,16,28,6,35,33,22,11,7,88,99,
					0,01,2,16,29,6,35,33,22,11,8,88,99,
					0,02,2,16,20,6,35,33,22,11,9,88,99,
					0,03,2,16,28,6,35,33,22,11,10,88,99,
					0,29,1,16,22,6,35,33,22,11,1,88,99,
					0,29,1,16,23,6,35,33,22,11,2,88,99,
					0,30,1,16,24,6,35,33,22,11,3,88,99,
					0,30,1,16,25,6,35,33,22,11,4,88,99,
					0,31,1,16,26,6,35,33,22,11,5,88,99,
					0,31,1,16,27,6,35,33,22,11,6,88,99,
					0,01,2,16,28,6,35,33,22,11,7,88,99,
					0,01,2,16,29,6,35,33,22,11,8,88,99,
					0,02,2,16,20,6,35,33,22,11,9,88,99,
					0,03,2,16,28,6,35,33,22,11,10,88,99,
					0,29,1,16,22,6,35,33,22,11,1,88,99,
					0,29,1,16,23,6,35,33,22,11,2,88,99,
					0,30,1,16,24,6,35,33,22,11,3,88,99,
					0,30,1,16,25,6,35,33,22,11,4,88,99,
					0,31,1,16,26,6,35,33,22,11,5,88,99,
					0,31,1,16,27,6,35,33,22,11,6,88,99,
					0,01,2,16,28,6,35,33,22,11,7,88,99,
					0,01,2,16,29,6,35,33,22,11,8,88,99,
					0,02,2,16,20,6,35,33,22,11,9,88,99,
					0,03,2,16,28,6,35,33,22,11,10,88,99,
					0,29,1,16,22,6,35,33,22,11,1,88,99,
					0,29,1,16,23,6,35,33,22,11,2,88,99,
					0,30,1,16,24,6,35,33,22,11,3,88,99,
					0,30,1,16,25,6,35,33,22,11,4,88,99,
					0,31,1,16,26,6,35,33,22,11,5,88,99,
					0,31,1,16,27,6,35,33,22,11,6,88,99,
					0,01,2,16,28,6,35,33,22,11,7,88,99,
					0,01,2,16,29,6,35,33,22,11,8,88,99,
					0,02,2,16,20,6,35,33,22,11,9,88,99,
					0,03,2,16,28,6,35,33,22,11,10,88,99,
					0,29,1,16,22,6,35,33,22,11,1,88,99,
					0,29,1,16,23,6,35,33,22,11,2,88,99,
					0,30,1,16,24,6,35,33,22,11,3,88,99,
					0,30,1,16,25,6,35,33,22,11,4,88,99,
					0,31,1,16,26,6,35,33,22,11,5,88,99,
					0,31,1,16,27,6,35,33,22,11,6,88,99,
					0,01,2,16,28,6,35,33,22,11,7,88,99,
					0,01,2,16,29,6,35,33,22,11,8,88,99,
					0,02,2,16,20,6,35,33,22,11,9,88,99,
					0,03,2,16,28,6,35,33,22,11,10,88,99,
					0,29,1,16,22,6,35,33,22,11,1,88,99,
					0,29,1,16,23,6,35,33,22,11,2,88,99,
					0,30,1,16,24,6,35,33,22,11,3,88,99,
					0,30,1,16,25,6,35,33,22,11,4,88,99,
					0,31,1,16,26,6,35,33,22,11,5,88,99,
					0,31,1,16,27,6,35,33,22,11,6,88,99,
					0,01,2,16,28,6,35,33,22,11,7,88,99,
					0,01,2,16,29,6,35,33,22,11,8,88,99,
					0,02,2,16,20,6,35,33,22,11,9,88,99,
					0,03,2,16,28,6,35,33,22,11,10,88,99};
					
	memcpy(expected_event, val, sizeof(expected_event));
	read_type = LAST_100;
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_1000events.txt", event, read_type, &no_of_events));
	TEST_ASSERT_EQUAL(100, no_of_events);	
	TEST_ASSERT_EQUAL_MEMORY(expected_event, event, sizeof(expected_event));
}

TEST(sdcard_interface, ReadEventReturnsErrorIfFULL_READusedButBufferNotEnough)
{		
	READ_TYPE read_type = FULL_READ;		
	ITSI_LOG_EVENT event[5];
	int no_of_events = 5;
	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_EVENT_COUNT, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
}

TEST(sdcard_interface, ReadEventCopiesRequiredBufSizeIfFULL_READusedButBufferNotEnough)
{		
	READ_TYPE read_type = FULL_READ;		
	ITSI_LOG_EVENT event[5];
	int no_of_events = 5;
	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_EVENT_COUNT, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
	TEST_ASSERT_EQUAL(10, no_of_events);
}

TEST(sdcard_interface, ReadEventCopiesAllTheEventsAvailable)
{		
	READ_TYPE read_type = LAST_100;	
	int no_of_events = 20;
	ITSI_LOG_EVENT event[20];
	ITSI_LOG_EVENT expected_event[10];
	
	char val[] = {0,29,1,16,22,6,35,33,22,11,1,88,99, 
					0,29,1,16,23,6,35,33,22,11,2,88,99,
					0,30,1,16,24,6,35,33,22,11,3,88,99, 
					0,30,1,16,25,6,35,33,22,11,4,88,99,  
					0,31,1,16,26,6,35,33,22,11,5,88,99, 
					0,31,1,16,27,6,35,33,22,11,6,88,99, 
					0,01,2,16,28,6,35,33,22,11,7,88,99, 
					0,01,2,16,29,6,35,33,22,11,8,88,99, 
					0,02,2,16,20,6,35,33,22,11,9,88,99, 
					0,03,2,16,28,6,35,33,22,11,10,88,99};
					
	memcpy(expected_event, val, sizeof(expected_event));
	
	TEST_ASSERT_EQUAL(SDCARD_IF_WARN_LESS_NO_Of_EVENTS_AVAIL, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
	TEST_ASSERT_EQUAL(10, no_of_events);
	TEST_ASSERT_EQUAL_MEMORY(expected_event, event, sizeof(expected_event));
}

TEST(sdcard_interface, ReadEventCopiesAvailableEventsIfNisMoreForN_FROM_BEGINING)
{		
	READ_TYPE read_type = N_FROM_BEGINING;	
	int no_of_events = 20;
	ITSI_LOG_EVENT event[20];
	ITSI_LOG_EVENT expected_event[10];
	
	char val[] = {0,29,1,16,22,6,35,33,22,11,1,88,99, 
					0,29,1,16,23,6,35,33,22,11,2,88,99,
					0,30,1,16,24,6,35,33,22,11,3,88,99, 
					0,30,1,16,25,6,35,33,22,11,4,88,99,  
					0,31,1,16,26,6,35,33,22,11,5,88,99, 
					0,31,1,16,27,6,35,33,22,11,6,88,99, 
					0,01,2,16,28,6,35,33,22,11,7,88,99, 
					0,01,2,16,29,6,35,33,22,11,8,88,99, 
					0,02,2,16,20,6,35,33,22,11,9,88,99, 
					0,03,2,16,28,6,35,33,22,11,10,88,99};
					
	memcpy(expected_event, val, sizeof(expected_event));
		
	TEST_ASSERT_EQUAL(SDCARD_IF_WARN_LESS_NO_Of_EVENTS_AVAIL, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
	TEST_ASSERT_EQUAL(10, no_of_events);
	TEST_ASSERT_EQUAL_MEMORY(expected_event, event, sizeof(expected_event));
}

TEST(sdcard_interface, ReadEventCopiesNEventsIfNEventsAvailableForN_FROM_BEGINING)
{		
	READ_TYPE read_type = N_FROM_BEGINING;	
	int no_of_events = 5;
	ITSI_LOG_EVENT event[5];
	ITSI_LOG_EVENT expected_event[5];
	
	char val[] = {0,29,1,16,22,6,35,33,22,11,1,88,99, 
					0,29,1,16,23,6,35,33,22,11,2,88,99,
					0,30,1,16,24,6,35,33,22,11,3,88,99, 
					0,30,1,16,25,6,35,33,22,11,4,88,99,  
					0,31,1,16,26,6,35,33,22,11,5,88,99};
					
	memcpy(expected_event, val, sizeof(expected_event));
		
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
	TEST_ASSERT_EQUAL(5, no_of_events);
	TEST_ASSERT_EQUAL_MEMORY(expected_event, event, sizeof(expected_event));
}

TEST(sdcard_interface, ReadEventCopiesAvailableEventsIfNisMoreForN_FROM_LAST)
{		
	READ_TYPE read_type = N_FROM_LAST;	
	int no_of_events = 20;
	ITSI_LOG_EVENT event[20];
	ITSI_LOG_EVENT expected_event[10];
	
	char val[] = {0,29,1,16,22,6,35,33,22,11,1,88,99, 
					0,29,1,16,23,6,35,33,22,11,2,88,99,
					0,30,1,16,24,6,35,33,22,11,3,88,99, 
					0,30,1,16,25,6,35,33,22,11,4,88,99,  
					0,31,1,16,26,6,35,33,22,11,5,88,99, 
					0,31,1,16,27,6,35,33,22,11,6,88,99, 
					0,01,2,16,28,6,35,33,22,11,7,88,99, 
					0,01,2,16,29,6,35,33,22,11,8,88,99, 
					0,02,2,16,20,6,35,33,22,11,9,88,99, 
					0,03,2,16,28,6,35,33,22,11,10,88,99};
					
	memcpy(expected_event, val, sizeof(expected_event));
	
	read_type = FULL_READ;
	TEST_ASSERT_EQUAL(SDCARD_IF_WARN_LESS_NO_Of_EVENTS_AVAIL, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
	TEST_ASSERT_EQUAL(10, no_of_events);
	TEST_ASSERT_EQUAL_MEMORY(expected_event, event, sizeof(expected_event));
}

TEST(sdcard_interface, ReadEventCopiesNEventsIfNEventsAvailableForN_FROM_LAST)
{		
	READ_TYPE read_type = N_FROM_LAST;	
	int no_of_events = 5;
	ITSI_LOG_EVENT event[5];
	ITSI_LOG_EVENT expected_event[5];
	
	char val[] = {0,31,1,16,27,6,35,33,22,11,6,88,99, 
					0,01,2,16,28,6,35,33,22,11,7,88,99, 
					0,01,2,16,29,6,35,33,22,11,8,88,99, 
					0,02,2,16,20,6,35,33,22,11,9,88,99, 
					0,03,2,16,28,6,35,33,22,11,10,88,99};
					
	memcpy(expected_event, val, sizeof(expected_event));
		
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, SDCardIF_ReadEventLog("sdcard_interface/event_file_with_10events.txt", event, read_type, &no_of_events));
	TEST_ASSERT_EQUAL(5, no_of_events);
	TEST_ASSERT_EQUAL_MEMORY(expected_event, event, sizeof(expected_event));
}


IGNORE_TEST(sdcard_interface, GetCurrentLogFileCopiesTheLogFileNameToTheBuffer)
{
	int ret;
	
	SDCardIF_Initialize();
	
	ret = SDCardIF_SetLogFile("sdcard_interface/event_file_not_available.dat");
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, ret);
	
}

TEST(sdcard_interface, InitFunctionCreatesTheDefaultLogIfNotAlreadyThere)
{
	SDCardIF_Initialize();
	TEST_ASSERT_EQUAL(1, IsFileAvailable(DEFAULT_EVENT_LOG));
}

TEST(sdcard_interface, CreateFirmwareFile_errorIfFilenameIsNull)
{	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM, SDCardIF_CreateFirmwareFile(NULL));
}

TEST(sdcard_interface, CreateFirmwareFile_errorIfFileIsAlreadyThere)
{	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_FILE_ACCESS, SDCardIF_CreateFirmwareFile("sdcard_interface/10k_firmware.bin"));
}

TEST(sdcard_interface, CreateFirmwareFile_CreatesAnEmptyFile)
{	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, SDCardIF_CreateFirmwareFile("sdcard_interface/empty_firmware.bin"));
	TEST_ASSERT_EQUAL(1, IsFileAvailable("sdcard_interface/empty_firmware.bin"));
}

TEST(sdcard_interface, DeleteFirmwareFile_errorIfFilenameIsNull)
{	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM, SDCardIF_DeleteFirmwareFile(NULL));
}

TEST(sdcard_interface, DeleteFirmwareFile_successIfFileNotthere)
{	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, SDCardIF_DeleteFirmwareFile("sdcard_interface/not_a_file"));
}

TEST(sdcard_interface, DeleteFirmwareFile_DeletesTheFile)
{	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, SDCardIF_CreateFirmwareFile("sdcard_interface/empty_firmware.bin"));
	TEST_ASSERT_EQUAL(1, IsFileAvailable("sdcard_interface/empty_firmware.bin"));
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, SDCardIF_DeleteFirmwareFile("sdcard_interface/empty_firmware.bin"));
	TEST_ASSERT_EQUAL(0, IsFileAvailable("sdcard_interface/empty_firmware.bin"));
}

TEST(sdcard_interface, AppendFirmwareData_errorIfFilenameIsNull)
{	
	char filecontent[10];
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM, SDCardIF_AppendFirmwareData(NULL, filecontent, sizeof(filecontent)));
}

TEST(sdcard_interface, AppendFirmwareData_errorIfFileNotAvailable)
{	
	char filecontent[10];
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_FILE_NOT_AVAILABLE, SDCardIF_AppendFirmwareData("sdcard_interface/not_a_file", filecontent, sizeof(filecontent)));
}

TEST(sdcard_interface, AppendFirmwareData_errorIfDataBufferIsNull)
{	
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM, SDCardIF_AppendFirmwareData("sdcard_interface/not_a_file", NULL, 50));
}

TEST(sdcard_interface, AppendFirmwareData_errorIfDataLengthIsZero)
{	
	char filecontent[10];
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM, SDCardIF_AppendFirmwareData("sdcard_interface/not_a_file", filecontent, 0));
}

TEST(sdcard_interface, AppendFirmwareData_errorIfDataLengthIsNegative)
{	
	char filecontent[10];
	TEST_ASSERT_EQUAL(SDCARD_IF_ERR_INVALID_PARAM, SDCardIF_AppendFirmwareData("sdcard_interface/not_a_file", filecontent, -100));
}


TEST(sdcard_interface, AppendFirmwareData_AppendsDataToTheEndOfFile)
{	
	char filename[] = "sdcard_interface/data_append";	
	char data[60];		
	int data_size = sizeof(data);
	int i;
	
	for(i = 0; i < data_size; i++){
		data[i] = i;
	}
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, SDCardIF_CreateFirmwareFile(filename));
	TEST_ASSERT_EQUAL(1, IsFileAvailable(filename));
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, SDCardIF_AppendFirmwareData(filename, data, data_size));				
	TEST_ASSERT_EQUAL(1, CompareFiles("sdcard_interface/data_append", "sdcard_interface/data_append_test1"));
	
	for(i = 0; i < data_size; i++){
		data[i] = data_size-i;
	}
	
	TEST_ASSERT_EQUAL(SDCARD_IF_OP_SUCCESS, SDCardIF_AppendFirmwareData(filename, data, data_size));	
	TEST_ASSERT_EQUAL(1, CompareFiles(filename, "sdcard_interface/data_append_test2"));
}

