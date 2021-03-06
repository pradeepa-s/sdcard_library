/**
 * @file sdcard_interface.c
 * @author Pradeepa Senanayake (pradeepa.kck@gmail.com)
 * @date 31 Jan 2016
 * @brief SDCard API function implementation
 *
 * This is a specific implementation which exposes more
 * higher level abstraction of low level sd card file
 * manipulations.
 *
 * Always use @@SDCardIF_Initialize function to initialize
 * the library before use.
 *
 * All the function comments has indepth details of each
 * function operations.
 *
 * Version v1.0.0
 * Version v1.1.0	-	Added function SDCardIF_GetEventCount (21-05-2016)
 */

#include "sdcard_interface.h"
#include "plat_arm.h"
#include <stdlib.h>
#include <string.h>

STATIC char sdcardif_initialized = FALSE;

STATIC char *audio_buffer = (char*)0;
STATIC uint32_t audio_buffer_size = 0;

STATIC char event_log_file[MAX_FILENAME_SIZE];

static char IsNotInitialized(void);
static void DecodeEvents(ITSI_LOG_EVENT *event, char *line_buffer);
static void create_event_entry(ITSI_LOG_EVENT event, char *out_string, size_t out_string_size);
static unsigned char string_to_val(char str[]);

/**
 * @brief Function initializes the sdcard API
 *
 * @return None
 * @note Make sure to call this function before calling any other API functions. 
 * 
 * @return	SDCARD_IF_OP_SUCCESS				-	Success
 * @return 	SDCARD_IF_ERR_NOT_INITIALIZED		-	Init function has not been called
 * @return 	SDCARD_IF_ERR_INVALID_PARAM			-	Invalid input parameter 
 * @return 	SDCARD_IF_ERR_FILE_NOT_AVAILABLE	-	Failed to create  new file 
 * @return	Refer to sdcard_err_codes.h
 */

int32_t SDCardIF_Initialize(void)
{
	int32_t ret = SDCARD_IF_OP_SUCCESS;
		
	audio_buffer = (char*)0;
	audio_buffer_size = 0U;
	
	ret = FileIF_Initialize();
	
	if(SDCARD_IF_OP_SUCCESS == ret){
		sdcardif_initialized = TRUE;
		
		ret = SDCardIF_SetLogFile(DEFAULT_EVENT_LOG);
	}

	if(SDCARD_IF_OP_SUCCESS != ret){
		sdcardif_initialized = FALSE;
	}
	
	return ret;
}


/**
 * @brief Function to set the buffer used by the audio file.
 * 
 * @param buffer[out] 	Pointer to an allocated buffer
 * @param buf_size[in] 	Size of the allocated buffer
 * @return 	SDCARD_IF_OP_SUCCESS			Operation success
 * 			SDCARD_IF_ERR_NOT_INITIALIZED	Init function not called
 * 			SDCARD_IF_ERR_INVALID_PARAM		Invalid parameters
 */

int32_t SDCardIF_SetAudioFileBuffer(char *p_buffer, uint32_t buf_size)
{
	int32_t ret = SDCARD_IF_OP_SUCCESS;
	
	if(IsNotInitialized()){
		ret = SDCARD_IF_ERR_NOT_INITIALIZED;
	}
	else if((NULL == p_buffer)){
		ret = SDCARD_IF_ERR_INVALID_PARAM;
	}
	else if(buf_size <= 0U){
		ret = SDCARD_IF_ERR_INVALID_BUFFER_SIZE;
	}
	else{
		audio_buffer = p_buffer;
		audio_buffer_size = buf_size;
	}
	
	return ret;
}

/**
 * @brief Function to read audio file from SDCARD to the audio file buffer
 * 
 * @param filename[in] 	Filename of the audio file
 * @return 	SDCARD_IF_OP_SUCCESS			Operation success 
 * @return	Refer to sdcard_err_codes.h
 */
int32_t SDCardIF_PlayAudioFile(const char *filename)
{
	int32_t ret = SDCARD_IF_OP_SUCCESS;
	uint32_t amount_read = audio_buffer_size;
	uint32_t file_size = 0U;
	
	if(IsNotInitialized()){
		ret = SDCARD_IF_ERR_NOT_INITIALIZED;
	}
	else if(((char*)0 == audio_buffer) || (0U == audio_buffer_size)){
		ret = SDCARD_IF_ERR_AUDIO_BUFFER_NOT_SET;
	}
	else{
		ret = FileIF_CopyFileToBuffer(filename, 0U, audio_buffer, &amount_read, &file_size);
	}
	
	return ret;
}

/**
 * @brief Function to set the current log file
 * 
 * If file is not available, an empty file will be created
 * 
 * @param filename[in] 	Filename of the log file
 * 
 * @return 	SDCARD_IF_OP_SUCCESS				Operation success 
 * @return 	SDCARD_IF_ERR_NOT_INITIALIZED		Init function has not been called
 * @return 	SDCARD_IF_ERR_INVALID_PARAM			Invalid input parameter 
 * @return 	SDCARD_IF_ERR_FILE_NOT_AVAILABLE	Failed to create  new file 
 * @return	Refer to sdcard_err_codes.h
 * 
 * @note None
 * @warning Maximum filename size is MAX_FILENAME_SIZE
 */

int32_t SDCardIF_SetLogFile(const char* filename)
{
	int32_t ret = SDCARD_IF_OP_SUCCESS;
	size_t filename_size = 0;
	
	if(IsNotInitialized()){
		ret = SDCARD_IF_ERR_NOT_INITIALIZED;
	}	
	else if(NULL == filename){
		ret = SDCARD_IF_ERR_INVALID_PARAM;
	}
	else if(strlen(filename) > MAX_FILENAME_SIZE){
		ret = SDCARD_IF_ERR_INVALID_PARAM;
	}
	else{
		ret = FileIF_IsFileAvailable(filename);
		
		if(FILEIF_ERR_FILE_NOT_AVAILABLE == ret){
			ret = FileIF_CreateFile(filename);
		}
		
		filename_size = strlen(event_log_file);

		memset(event_log_file, 0x00, filename_size);
		memcpy(event_log_file, filename, strlen(filename));
	}
	
	return ret;
}

/**
 * @brief Function to set delete the current log file
 * 
 * If the currently selected log file is deleted, the selected log file
 * will be changed back to the default value DEFAULT_EVENT_LOG.
 * 
 * @param filename[in] 	Filename of the log file
 * 
 * @return 	SDCARD_IF_OP_SUCCESS				Operation success
 * @return 	SDCARD_IF_ERR_INVALID_PARAM			Invalid input parameter 
 * @return 	SDCARD_IF_ERR_FILE_NOT_AVAILABLE	Failed to delete the file 
 * @return	Refer to sdcard_err_codes.h
 * 
 * @note No need to initialize the API to use this.
 * @warning Maximum filename size is MAX_FILENAME_SIZE
 */
int32_t SDCardIF_DeleteLogFile(const char* filename)
{
	int32_t ret = 0;
	
	if(NULL == filename){
		ret = SDCARD_IF_ERR_INVALID_PARAM;
	}
	else if(FILEIF_ERR_FILE_NOT_AVAILABLE == FileIF_IsFileAvailable(filename)){
		ret = SDCARD_IF_ERR_FILE_NOT_AVAILABLE;
	}
	else{
		ret = FileIF_DeleteFile(filename);
		
		/* MISRA-C:2004 12.4/R can be ignored because strcmp(filename, event_log_file)
		 * have no side effects.
		 */
		if((FILEIF_OP_SUCCESS == ret) && (0 == strcmp(filename, event_log_file))){
			memset(event_log_file, 0x00, sizeof(event_log_file));
			memcpy(event_log_file, DEFAULT_EVENT_LOG, strlen(DEFAULT_EVENT_LOG));
		}
	}
	
	return ret;
}

/**
 * @brief Function to get the current log file name
 * 
 * There can be multiple log files in the system. This API can be used to
 * check what is the current log file in use. 
 * 
 * @param filename[out] 			Buffer to copy the filename
 * @param filename_size[in] 	    Buffer size
 * 
 * @return 	SDCARD_IF_OP_SUCCESS				Operation success
 * @return 	SDCARD_IF_ERR_NOT_INITIALIZED		Init function was not called
 * @return 	SDCARD_IF_ERR_INVALID_PARAM			Invalid input parameter/buffer size  
 * @return	Refer to sdcard_err_codes.h
 * 
 * @see SDCardIF_SetLogFile
 * 
 * @note 	Init function must be called before calling this function.
 * 			It is advisable to allocate MAX_FILENAME_SIZE for buf_size to
 * 			store the filename.
 * 
 * @warning None
 */
int32_t SDCardIF_GetCurrentLogFile(char *filename, uint32_t *filename_size)
{
	int32_t ret;
	
	if(IsNotInitialized()){
		ret = SDCARD_IF_ERR_NOT_INITIALIZED;
	}
	else if((NULL == filename) || (((uint32_t*)0) == filename_size)){
		ret = SDCARD_IF_ERR_INVALID_PARAM;
	}
	else if(*filename_size < strlen(event_log_file)) {
		ret = SDCARD_IF_ERR_INVALID_BUFFER_SIZE;
	}
	else{
		memset(filename, 0x00, *filename_size);

		/* (MISRA-C:2004 12.2/R) can be ignored because the order of execution
		 * does not change the value of event_log_file
		 */
		*filename_size = strlen(event_log_file);
		memcpy(filename, event_log_file, *filename_size);
		ret = SDCARD_IF_OP_SUCCESS;
	}
	
	return ret;
}


/**
 * @brief Function to read a firmware file to a buffer
 * 
 * Firmware file needs to be read from the SD CARD to a buffer provided
 * by the firmware developer. This function is designed to facilitate 
 * that task.
 * 
 * Function can use @offset parameter to apply an offset to the start 
 * location of the firmware. 
 * 
 * @param filename[in] 				Firmware file name
 * @param offset[in] 				Offset to start reading
 * @param buffer[out] 				User allocated buffer
 * @param buf_size[inout] 			Buffer size as input, amount copied as output
 * @param file_size[out] 			Size of the filename
 * 
 * @return 	SDCARD_IF_OP_SUCCESS				Operation success
 * @return 	SDCARD_IF_ERR_INVALID_BUFFER_SIZE	Buffer size is zero or negetive 
 * @return 	SDCARD_IF_ERR_FILE_NOT_AVAILABLE	File is not accessible
 * @return 	SDCARD_IF_ERR_INVALID_PARAM			Invalid input parameter
 * @return 	SDCARD_IF_ERR_BUFFER_OFFSET			Offset is negetive or more than filesize
 * @return 	SDCARD_IF_WARN_BUFFER_SIZE_SMALL	Warning buffer size is smaller than filesize
 * @return 	SDCARD_IF_WARN_BUFFER_SIZE_LARGE	Warning buffer size is larger than filesize
 * @return	Refer to sdcard_err_codes.h
 * 
 * 
 * @note 	If a offset is provided data copying will start after applying
 * 			the specified amount of offset to the data in the firmware 
 * 			file
 * 
 * @note	If the @filename and @file_size parameters are valid and the file
 * 			is available. The @file_size will contain the size of the file
 * 			irrespective of the validity of other files.
 * 
 * @warning Do not treat warnings as errors.
 */
int32_t SDCardIF_ReadFirmwareFile(const char *filename, uint32_t offset, char *buffer, uint32_t *buf_size, uint32_t *file_size)
{
	int32_t ret;
	
	/* Read file size */
	ret = FileIF_GetFileSize(filename, file_size);
	
	if((NULL == buffer) || (NULL == buf_size)){
		ret = SDCARD_IF_ERR_INVALID_PARAM;
	}
	else if(*buf_size <= 0U){
		ret = SDCARD_IF_ERR_INVALID_BUFFER_SIZE;		
	}
	else if(offset < 0U){
		ret = SDCARD_IF_ERR_BUFFER_OFFSET;
	}
	else{
		ret = FileIF_CopyFileToBuffer(filename, offset, buffer, buf_size, file_size);
	}
	
	return ret;
}

/**
 * @brief Function to log an event to the file
 * 
 * Function will take the @event and log it in a new line in the 
 * log file set by @SDCardIF_SetLogFile API. 
 * 
 * @param event[in]		Pointer to the event structure whcih holds the event
 * 
 * @return 	SDCARD_IF_OP_SUCCESS				Operation success
 * @return 	SDCARD_IF_ERR_NOT_INITIALIZED		Init function has not been called
 * @return 	SDCARD_IF_ERR_FILE_NOT_AVAILABLE	File is not accessible
 * @return 	SDCARD_IF_ERR_INVALID_PARAM			Invalid input parameter
 * @return	Refer to sdcard_err_codes.h
 * 
 * @see 	SDCardIF_SetLogFile
 * @see 	SDCardIF_Initialize
 * 
 * @note 	@SDCardIF_Initialize function must be called prior to 
 * 			using this API.
 */
int32_t SDCardIF_LogEvent(const ITSI_LOG_EVENT *event)
{
	int32_t ret = SDCARD_IF_OP_SUCCESS;
	char value[100] = {0};
	
	if(IsNotInitialized()){
		ret = SDCARD_IF_ERR_NOT_INITIALIZED;
	}
	else if(NULL == event){
		ret = SDCARD_IF_ERR_INVALID_PARAM;
	}
	else{
		memset(value, 0x00, sizeof(value));
		
		create_event_entry(*event, value, sizeof(value));
							
		ret = FileIF_AppendString(event_log_file,value);
						
	}
	
	return ret;
}

/**
 * @brief Function to get number of events in the log file
 * 
 * @param filename[in] 		Filename 
 * @param count[out] 		No of events in the file
 * 
 * @return 	SDCARD_IF_OP_SUCCESS				Operation success 
 * @return 	SDCARD_IF_ERR_INVALID_PARAM			Function parameters are invalid
 * @return 	SDCARD_IF_ERR_FILE_NOT_AVAILABLE	File cannot be found or cannot be accessed
 * 
 * 
 * @warning None
 */
int32_t SDCardIF_GetEventCount(const char* filename, uint32_t *count)
{
	int32_t ret = SDCARD_IF_OP_SUCCESS;
	
	if(count == NULL){
		ret = SDCARD_IF_ERR_INVALID_PARAM;
	}
	else{
		ret = FileIF_IsFileAvailable(filename);
	}
	
	if(SDCARD_IF_OP_SUCCESS == ret){
		ret = FileIF_GetNoOfLines(filename, count);
	}
	
	return ret;
}


/**
 * @brief Function to read event log
 * 
 * Function will read the specified event log file and provide the
 * available events to the user. 
 * 
 * @param filename[in]			Filename of the log file
 * @param event[out]			Buffer to store the events
 * @param read_type[in]			Can be LAST_100, N_FROM_BEGINING, N_FROM_LAST or FULL_READ
 * @param no_of_events[inout]	No of event that can be stored in the event buffer
 * @param offset[in]			Offset used for N_FROM_LAST and N_FROM_BEGINING
 * 
 * @return 	SDCARD_IF_OP_SUCCESS					Operation success
 * @return 	SDCARD_IF_ERR_INVALID_PARAM				Invalid input parameters 
 * @return 	SDCARD_IF_ERR_FILE_NOT_AVAILABLE		File is not accessible 
 * @return 	SDCARD_IF_ERR_EVENT_COUNT				Provided @no_of_events parameter is invalid
 * @return 	SDCARD_IF_ERR_EVENT_NOT_FOUND			Error in fetching the event 
 * @return 	SDCARD_IF_ERR_BUFFER_OFFSET				Offset used for reading is invalid
 * @return 	SDCARD_IF_WARN_LESS_NO_Of_EVENTS_AVAIL	Required no of events is more than available
 * @return	Refer to sdcard_err_codes.h
 * 
 * @see 	SDCardIF_LogEvent
 * 
 * @note 	When the SDCARD_IF_ERR_EVENT_COUNT is returned, refer to the 
 * 			@no_of_events parameter to find out the available number of events.
 * 
 * @note	If required amount of events are not available, whatever 
 * 			available data will be copied to the buffer, subsequently
 * 			the return value will be SDCARD_IF_WARN_LESS_NO_Of_EVENTS_AVAIL.
 * 
 * @note	Upon successful completion of the function the @no_of_events
 * 			parameter will contain the copied number of events.
 * 
 */

int32_t SDCardIF_ReadEventLog(const char* filename, ITSI_LOG_EVENT event[], READ_TYPE read_type, uint32_t *no_of_events, uint32_t offset)
{
	int32_t ret = SDCARD_IF_OP_SUCCESS;
	uint32_t event_count = 0;
	uint32_t event_count_total = 0;
	uint32_t expected_count = 0;
	uint32_t read_count = 0;
	uint32_t start_line = 1;
	int32_t i = 0;
	char line_buffer[64];
	uint32_t buffer_size = sizeof(line_buffer);
	
	/* Validate parameters */
	if( (NULL == filename) || (NULL == no_of_events) || (NULL == event) || ((read_type < FULL_READ) || (read_type > N_FROM_LAST)) )
	{
		ret = SDCARD_IF_ERR_INVALID_PARAM;
	}
	else
	{
		ret = FileIF_IsFileAvailable(filename);		
	}
		
	if(FILEIF_OP_SUCCESS == ret){
		
		/* Get the available events in the file */
		ret = FileIF_GetNoOfLines(filename, &event_count);										
				
		if(FILEIF_OP_SUCCESS == ret){
				
			event_count_total = event_count;
			
			/* Handle the offset parameter */		
			if((read_type == N_FROM_LAST) || (read_type == N_FROM_BEGINING)){
				
				/* For N_FROM_BEGINING and N_FROM_LAST verify bounds of offset */
				if((offset > event_count) || (offset < 0U)){
					ret = SDCARD_IF_ERR_BUFFER_OFFSET;
				}			
				else{
					/* Because of offset event_count will decrease by that amount */
					event_count -= offset;
				}						
			}						
		}
		
		if(FILEIF_OP_SUCCESS == ret){	
		
			/* If buffer is not enough, return error */
			if(		((read_type == FULL_READ) && (event_count > *no_of_events)) || \
					((read_type != LAST_100)  && (*no_of_events <= 0U))	){
						
				*no_of_events = event_count;
				ret = SDCARD_IF_ERR_EVENT_COUNT;
			}
		}
		
		if(FILEIF_OP_SUCCESS == ret){				
			
			/* If LAST_100 is the read type we are expecting 100 events */
			if(read_type == LAST_100){
				expected_count = 100U;
			}
			else{
				expected_count = *no_of_events;
			}						
			
			/* Check whether required number of events are available */
			if(event_count < expected_count){				
				ret = SDCARD_IF_WARN_LESS_NO_Of_EVENTS_AVAIL;
				read_count 	 = event_count;			
			}
			else{
				read_count = expected_count;						
			}
			
			/* Find the starting event */
			if(read_type == LAST_100){
			    /*
			     * MISRA-C:2004 12.1/A can be ignored because the arithmatic operation has no effect from the operator precedence
			     */
				start_line = event_count_total - read_count + 1U;
			}
			else if(read_type == N_FROM_LAST){
			    /*
			     * MISRA-C:2004 12.1/A can be ignored because the arithmatic operation has no effect from the operator precedence
			     */
				start_line = event_count_total - read_count + 1U - offset;
			}
			else if(read_type == N_FROM_BEGINING){
				start_line = 1U + offset;
			}
			else{
			    /* FULL_READ */
			}
				
			/* Copy events to the structure */		
			for(i = 0; i < (int32_t)read_count; i++){
					
				buffer_size = sizeof(line_buffer);
				memset(line_buffer, 0x00, buffer_size);
								
				ret = FileIF_ReadLine(filename, (uint32_t)i+start_line, line_buffer, &buffer_size);

				if(ret == SDCARD_IF_OP_SUCCESS){

					DecodeEvents(&event[i], line_buffer);																				
				}
			}
			
			*no_of_events = read_count;
								
		}		
	}
	
	return ret;
}


/**
 * @brief Function to create an empty firmware file
 * 
 * This function creates an empty file provided that a file
 * with the same name does not exist in the provided location.
 * 
 * @param filename[in]			Filename of the firmware file
 * 
 * @return 	SDCARD_IF_OP_SUCCESS					Operation success
 * @return 	SDCARD_IF_ERR_INVALID_PARAM				Invalid input parameters 
 * @return 	SDCARD_IF_ERR_FILE_ACCESS				File creation failed
 * @return 	SDCARD_IF_ERR_FILE_NOT_AVAILABLE		File creation failed
 * @return	Refer to sdcard_err_codes.h
 * 
 * @note 	This function can work without initilizing the library.
 * 			ie: without calling @SDCardIF_Initialize
 */

int32_t SDCardIF_CreateFirmwareFile(const char *filename)
{
	int32_t ret = SDCARD_IF_OP_SUCCESS;
	
	if(NULL == filename){
		ret = SDCARD_IF_ERR_INVALID_PARAM;
	}
	else if(SDCARD_IF_OP_SUCCESS == FileIF_IsFileAvailable(filename)){
		ret = SDCARD_IF_ERR_FILE_ACCESS;
	}
	else{
		ret = FileIF_CreateFile(filename);
	}
	
	return ret;
}

/**
 * @brief Function to delete a firmware file
 * 
 * This function delets an empty file. 
 * If the file is not there, the function will return SUCCESS.
 * 
 * @param filename[in]			Filename of the firmware file
 * 
 * @return 	SDCARD_IF_OP_SUCCESS					Operation success
 * @return 	SDCARD_IF_ERR_INVALID_PARAM				Invalid input parameters  
 * @return 	SDCARD_IF_ERR_FILE_NOT_AVAILABLE		File deletion failed
 * @return	Refer to sdcard_err_codes.h
 * 
 * @note 	This function can work without initilizing the library.
 * 			ie: without calling @SDCardIF_Initialize
 */

int32_t SDCardIF_DeleteFirmwareFile(const char *filename)
{
	int32_t ret = SDCARD_IF_OP_SUCCESS;
	
	if(NULL == filename){
		ret = SDCARD_IF_ERR_INVALID_PARAM;
	}	
	else{
		ret = FileIF_DeleteFile(filename);
		
		if(SDCARD_IF_ERR_FILE_NOT_AVAILABLE == ret){
			ret = SDCARD_IF_OP_SUCCESS;
		}
	}
	
	return ret;
}

/**
 * @brief Append the firmware data to a file
 * 
 * Append provided data to the firmware file
 * 
 * @param filename[in]			Filename of the firmware file
 * @param data[in]				Firmware data buffer
 * @param data_size[in]			Size of the data buffer
 * 
 * @return 	SDCARD_IF_OP_SUCCESS					Operation success
 * @return 	SDCARD_IF_ERR_INVALID_PARAM				Invalid input parameters  
 * @return	Refer to sdcard_err_codes.h
 * 
 */
 
int32_t SDCardIF_AppendFirmwareData(const char *filename, char* data, uint32_t data_size)
{		
	int32_t ret = SDCARD_IF_OP_SUCCESS;
	
	if(	(NULL == filename)	||
		(NULL == data)	||
		(data_size <= 0U)){
			ret = SDCARD_IF_ERR_INVALID_PARAM;
	}
	
	if(SDCARD_IF_OP_SUCCESS == ret){
		ret = FileIF_IsFileAvailable(filename);
	}
	
	if(SDCARD_IF_OP_SUCCESS == ret){
		ret = FileIF_CopyBufferToFile(filename, data, data_size);
	}	
	
	return ret;
}


/**
 * @brief Function to reset all the static and global buffers
 * 
 */
void SDCardIF_Reset(void)
{
	sdcardif_initialized = FALSE;
	audio_buffer = (char*)NULL;
	audio_buffer_size = 0U;
	memset(event_log_file, 0x00, sizeof(event_log_file));
}

static char IsNotInitialized(void)
{
    char ret = (sdcardif_initialized == FALSE);
	return ret;
}

static
void DecodeEvents(ITSI_LOG_EVENT *event, char *line_buffer)
{
    const char delim[2] = ",";
    char *tokens = (char*)0;
    int32_t val = 0;
    unsigned char index = 0U;

    if(line_buffer != (char*)0){

        tokens = strtok(line_buffer, delim);

        while(tokens != (char*)0){
            val = (int32_t)string_to_val(tokens);

            if(index == 0U){
                event->length = (unsigned char)val;
            }else if(index == 1U){
                event->day = (unsigned char)val;
            }else if(index == 2U){
                event->month = (unsigned char)val;
            }else if(index == 3U){
                event->year = (unsigned char)val;
            }else if(index == 4U){
                event->hour = (unsigned char)val;
            }else if(index == 5U){
                event->minute = (unsigned char)val;
            }else if(index == 6U){
                event->second = (unsigned char)val;
            }else if(index == 7U){
                event->id3 = (unsigned char)val;
            }else if(index == 8U){
                event->id2 = (unsigned char)val;
            }else if(index == 9U){
                event->id1 = (unsigned char)val;
            }else if(index == 10U){
                event->event_no = (unsigned char)val;
            }else if(index == 11U){
                event->crc_msb = (unsigned char)val;
            }else if(index == 12U){
                event->crc_lsb = (unsigned char)val;
            }else{
            }

            index = index + 1U;

            tokens = strtok((char*)0, delim);
        }
    }
}

static
unsigned char string_to_val(char str[])
{
    /* Maximum value is 255 */
    size_t str_size = strlen(str);
    const unsigned char zero_ascii = (unsigned char)'0';
    const unsigned char nine_ascii = (unsigned char)'9';
    unsigned char ones_pos = 0U;
    unsigned char tenth_pos = 0U;
    unsigned char hundredth_pos = 0U;
    unsigned char final_val = 0U;
    unsigned char char_val = 0U;
    unsigned int i = 0U;

    if(str != (char*)0){

        for(i = 0U; i < str_size; i++){

            char_val = (unsigned char)str[i];

            if((char_val < zero_ascii) || (char_val > nine_ascii)){
                str_size = i;
                break;
            }
        }

        if(str_size == 1U){

            ones_pos = (unsigned char)str[0] - zero_ascii;
        }
        else if(str_size == 2U){

            ones_pos = (unsigned char)str[1] - zero_ascii;
            tenth_pos = (unsigned char)str[0] - zero_ascii;
        }
        else if(str_size == 3U){

            ones_pos = (unsigned char)str[2] - zero_ascii;
            tenth_pos = (unsigned char)str[1] - zero_ascii;
            hundredth_pos = (unsigned char)str[0] - zero_ascii;
        }
        else{
        }
    }

    final_val = ones_pos + (tenth_pos * 10U) + (hundredth_pos * 100U);

    return final_val;
}

static
void create_event_entry(ITSI_LOG_EVENT event, char *out_string, size_t out_string_size)

{
    unsigned char output_string[100] = {0};
    unsigned char *p_string = (unsigned char*)0;
    unsigned char temp_val = 0U;
    char temp_string[20] = {0};
    uint32_t string_len = 0U;
    uint32_t string_pos = 0U;

    if((out_string != (char*)0) && (out_string_size > 0U)){

        /* Length */

        /* MISRA-C:2004 17.6/R can be ignored because p_string and output_string have same scope */
        p_string = &output_string[string_pos];

        temp_val = (unsigned char)event.length;
        ltoa((int32_t)temp_val, temp_string);
        string_len = strlen(temp_string);

        memcpy(p_string, temp_string, string_len);

        string_pos = string_pos + string_len;

        output_string[string_pos] = (unsigned char)',';
        string_pos = string_pos + 1U;

        /* Day */

        /* MISRA-C:2004 17.6/R can be ignored because p_string and output_string have same scope */
        p_string = &output_string[string_pos];

        memset(temp_string, 0x00, (size_t)20);
        temp_val = (unsigned char)event.day;
        ltoa((int32_t)temp_val, temp_string);
        string_len = strlen(temp_string);

        memcpy(p_string, temp_string, string_len);

        string_pos = string_pos + string_len;

        output_string[string_pos] = (unsigned char)',';
        string_pos = string_pos + 1U;

        /* Month */

        /* MISRA-C:2004 17.6/R can be ignored because p_string and output_string have same scope */
        p_string = &output_string[string_pos];

        memset(temp_string, 0x00, (size_t)20);
        temp_val = (unsigned char)event.month;
        ltoa((int32_t)temp_val, temp_string);
        string_len = strlen(temp_string);

        memcpy(p_string, temp_string, string_len);

        string_pos = string_pos + string_len;

        output_string[string_pos] = (unsigned char)',';
        string_pos = string_pos + 1U;

        /* Year */

        /* MISRA-C:2004 17.6/R can be ignored because p_string and output_string have same scope */
        p_string = &output_string[string_pos];

        memset(temp_string, 0x00, (size_t)20);
        temp_val = (unsigned char)event.year;
        ltoa((int32_t)temp_val, temp_string);
        string_len = strlen(temp_string);

        memcpy(p_string, temp_string, string_len);

        string_pos = string_pos + string_len;

        output_string[string_pos] = (unsigned char)',';
        string_pos = string_pos + 1U;

        /* Hour */

        /* MISRA-C:2004 17.6/R can be ignored because p_string and output_string have same scope */
        p_string = &output_string[string_pos];

        memset(temp_string, 0x00, (size_t)20);
        temp_val = (unsigned char)event.hour;
        ltoa((int32_t)temp_val, temp_string);
        string_len = strlen(temp_string);

        memcpy(p_string, temp_string, string_len);

        string_pos = string_pos + string_len;

        output_string[string_pos] = (unsigned char)',';
        string_pos = string_pos + 1U;

        /* Minute */

        /* MISRA-C:2004 17.6/R can be ignored because p_string and output_string have same scope */
        p_string = &output_string[string_pos];

        memset(temp_string, 0x00, (size_t)20);
        temp_val = (unsigned char)event.minute;
        ltoa((int32_t)temp_val, temp_string);
        string_len = strlen(temp_string);

        memcpy(p_string, temp_string, string_len);

        string_pos = string_pos + string_len;

        output_string[string_pos] = (unsigned char)',';
        string_pos = string_pos + 1U;

        /* Second */

        /* MISRA-C:2004 17.6/R can be ignored because p_string and output_string have same scope */
        p_string = &output_string[string_pos];

        memset(temp_string, 0x00, (size_t)20);
        temp_val = (unsigned char)event.second;
        ltoa((int32_t)temp_val, temp_string);
        string_len = strlen(temp_string);

        memcpy(p_string, temp_string, string_len);

        string_pos = string_pos + string_len;

        output_string[string_pos] = (unsigned char)',';
        string_pos = string_pos + 1U;

        /* id3 */

        /* MISRA-C:2004 17.6/R can be ignored because p_string and output_string have same scope */
        p_string = &output_string[string_pos];

        memset(temp_string, 0x00, (size_t)20);
        temp_val = (unsigned char)event.id3;
        ltoa((int32_t)temp_val, temp_string);
        string_len = strlen(temp_string);

        memcpy(p_string, temp_string, string_len);

        string_pos = string_pos + string_len;

        output_string[string_pos] = (unsigned char)',';
        string_pos = string_pos + 1U;

        /* id2 */

        /* MISRA-C:2004 17.6/R can be ignored because p_string and output_string have same scope */
        p_string = &output_string[string_pos];

        memset(temp_string, 0x00, (size_t)20);
        temp_val = (unsigned char)event.id2;
        ltoa((int32_t)temp_val, temp_string);
        string_len = strlen(temp_string);

        memcpy(p_string, temp_string, string_len);

        string_pos = string_pos + string_len;

        output_string[string_pos] = (unsigned char)',';
        string_pos = string_pos + 1U;

        /* id1 */

        /* MISRA-C:2004 17.6/R can be ignored because p_string and output_string have same scope */
        p_string = &output_string[string_pos];

        memset(temp_string, 0x00, (size_t)20);
        temp_val = (unsigned char)event.id1;
        ltoa((int32_t)temp_val, temp_string);
        string_len = strlen(temp_string);

        memcpy(p_string, temp_string, string_len);

        string_pos = string_pos + string_len;

        output_string[string_pos] = (unsigned char)',';
        string_pos = string_pos + 1U;

        /* event_no */

        /* MISRA-C:2004 17.6/R can be ignored because p_string and output_string have same scope */
        p_string = &output_string[string_pos];

        memset(temp_string, 0x00, (size_t)20);
        temp_val = (unsigned char)event.event_no;
        ltoa((int32_t)temp_val, temp_string);
        string_len = strlen(temp_string);

        memcpy(p_string, temp_string, string_len);

        string_pos = string_pos + string_len;

        output_string[string_pos] = (unsigned char)',';
        string_pos = string_pos + 1U;

        /* crc_msb */

        /* MISRA-C:2004 17.6/R can be ignored because p_string and output_string have same scope */
        p_string = &output_string[string_pos];

        memset(temp_string, 0x00, (size_t)20);
        temp_val = (unsigned char)event.crc_msb;
        ltoa((int32_t)temp_val, temp_string);
        string_len = strlen(temp_string);

        memcpy(p_string, temp_string, string_len);

        string_pos = string_pos + string_len;

        output_string[string_pos] = (unsigned char)',';
        string_pos = string_pos + 1U;

        /* crc_lsb */

        /* MISRA-C:2004 17.6/R can be ignored because p_string and output_string have same scope */
        p_string = &output_string[string_pos];

        memset(temp_string, 0x00, (size_t)20);
        temp_val = (unsigned char)event.crc_lsb;
        ltoa((int32_t)temp_val, temp_string);
        string_len = strlen(temp_string);

        memcpy(p_string, temp_string, string_len);

        string_pos = string_pos + string_len;

        /* new line */
        output_string[string_pos] = (unsigned char)'\n';
        string_pos = string_pos + 1U;

        memcpy(out_string, output_string, string_pos);
    }

}
