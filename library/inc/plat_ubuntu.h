/**
 * @file plat_ubuntu.h
 * @author Pradeepa Senanayake (pradeepa.kck@gmail.com)
 * @date 31 Jan 2016
 * @brief Contains return types and function implementation for plat_ubuntu.c
 *
 * This header file contains the function definitions of plat_ubuntu.c file.

 * If this file is not directly used by the user, make sure that proper
 * return code mapping is done to make development easier.
 *
 */

#ifndef _PLAT_UBUNTU_H
#define _PLAT_UBUNTU_H

#include "sdcard_err_codes.h"

#if 0

#define FILEIF_OP_SUCCESS				0
#define FILEIF_ERR_INVALID_PARAM		-1
#define FILEIF_ERR_FILE_NOT_AVAILABLE	-2
#define FILEIF_ERR_BUFFER_SIZE			-3
#define FILEIF_ERR_FILE_OFFSET			-4
#define FILEIF_ERR_LINE_NO				-5

#define FILEIF_WARN_BUFFER_SIZE_SMALL	-200
#define FILEIF_WARN_BUFFER_SIZE_LARGE	-201

#endif

#define FILEIF_OP_SUCCESS				SDCARD_IF_OP_SUCCESS
#define FILEIF_ERR_INVALID_PARAM		SDCARD_IF_ERR_INVALID_PARAM
#define FILEIF_ERR_FILE_NOT_AVAILABLE	SDCARD_IF_ERR_FILE_NOT_AVAILABLE
#define FILEIF_ERR_BUFFER_SIZE			SDCARD_IF_ERR_INVALID_BUFFER_SIZE
#define FILEIF_ERR_FILE_OFFSET			SDCARD_IF_ERR_BUFFER_OFFSET
#define FILEIF_ERR_LINE_NO				SDCARD_IF_ERR_EVENT_NOT_FOUND

#define FILEIF_WARN_BUFFER_SIZE_SMALL	SDCARD_IF_WARN_BUFFER_SIZE_SMALL
#define FILEIF_WARN_BUFFER_SIZE_LARGE	SDCARD_IF_WARN_BUFFER_SIZE_LARGE

//#define FILE FIL

int FileIF_CopyFileToBuffer(const char *filename, int offset, char *buffer, int *buf_size, int *file_size);
int FileIF_IsFileAvailable(const char *filename);
int FileIF_CreateFile(const char *filename);
int FileIF_DeleteFile(const char *filename);
int FileIF_GetFileSize(const char *filename, int *file_size);
int FileIF_AppendString(const char *filename, const char *string);
int FileIF_GetNoOfLines(const char *filename,int *no_of_lines);
int FileIF_ReadLine(const char *filename, int line_no, char *line_buffer, int *buf_size);

#endif
