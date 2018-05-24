/**
 * @file sdcard_interface.h
 * @author Pradeepa Senanayake (pradeepa.kck@gmail.com)
 * @date 31 Jan 2016
 * @brief Function declarations for sdcard_interface.c
 *
 *
 * Version v1.0.0
 * Version v1.1.0	-	Added function SDCardIF_GetEventCount (21-05-2016)
 */

#ifndef _SDCARD_INTERFACE_H
#define _SDCARD_INTERFACE_H

#include "sdcard_err_codes.h"
#include <stdint.h>

#ifdef TEST
#define STATIC static
#else
#define STATIC
#endif

#ifndef TRUE
#define TRUE	((char)1)
#endif

#ifndef FALSE
#define FALSE	((char)0)
#endif

typedef struct _ITSI_LOG_EVENT{
	unsigned char length;
	unsigned char day;
	unsigned char month;
	unsigned char year;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned char id3;
	unsigned char id2;
	unsigned char id1;
	unsigned char event_no;
	unsigned char crc_msb;
	unsigned char crc_lsb;
} ITSI_LOG_EVENT;


#define DEFAULT_EVENT_LOG	("EVENT_Log.txt")
#define MAX_FILENAME_SIZE   ((size_t)255)

enum _READ_TYPE{
	FULL_READ = 0,
	LAST_100,
	N_FROM_BEGINING,
	N_FROM_LAST
};

typedef enum _READ_TYPE READ_TYPE;

int SDCardIF_Initialize(void);
int SDCardIF_SetAudioFileBuffer(char *p_buffer, uint32_t buf_size);
int SDCardIF_PlayAudioFile(const char *filename);
int SDCardIF_SetLogFile(const char* filename);
int SDCardIF_DeleteLogFile(const char* filename);
int SDCardIF_LogEvent(ITSI_LOG_EVENT *event);
int SDCardIF_ReadEventLog(const char* filename, ITSI_LOG_EVENT *event, READ_TYPE read_type, uint32_t *no_of_events, uint32_t offset);
int SDCardIF_GetEventCount(const char* filename, uint32_t *count);
int SDCardIF_GetCurrentLogFile(char *filename, uint32_t *filename_size);
int SDCardIF_ReadFirmwareFile(char *filename, uint32_t offset, char *buffer, uint32_t *buf_size, uint32_t *file_size);
int SDCardIF_CreateFirmwareFile(const char *filename);
int SDCardIF_DeleteFirmwareFile(const char *filename);
int SDCardIF_AppendFirmwareData(const char *filename, char* data, uint32_t data_size);
void SDCardIF_Reset(void);

#endif
