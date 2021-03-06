/**
 * @file plat_arm.h
 * @author Pradeepa Senanayake (pradeepa.kck@gmail.com)
 * @date 31 Jan 2016
 * @brief Contains return types and function implementation for plat_arm.c
 *
 * This header file contains the function definitions of plat_arm.c file.

 * If this file is not directly used by the user, make sure that proper
 * return code mapping is done to make development easier.
 *
 *
 * Version v1.0.0
 */

#ifndef _PLAT_UBUNTU_H
#define _PLAT_UBUNTU_H

#include "sdcard_err_codes.h"
#include <stdint.h>

#if 0

#define FILEIF_OP_SUCCESS				0
#define FILEIF_ERR_INVALID_PARAM		-1
#define FILEIF_ERR_FILE_NOT_AVAILABLE	-2
#define FILEIF_ERR_BUFFER_SIZE			-3
#define FILEIF_ERR_FILE_OFFSET			-4
#define FILEIF_ERR_LINE_NO				-5
#define FILEIF_ERR_FILE_ACCESS			-6
#define FILEIF_ERR_UNINIT				-7

#define FILEIF_WARN_BUFFER_SIZE_SMALL	-200
#define FILEIF_WARN_BUFFER_SIZE_LARGE	-201
#define FILEIF_WARN_EIGHTY_PERCENT		-202

#define FILEIF_WARN_FF_DISK_ERROR			-301
#define FILEIF_WARN_FF_INTERNAL_ERROR		-302
#define FILEIF_WARN_FF_NOT_READY			-303
#define FILEIF_WARN_FF_NO_FILE				-304
#define FILEIF_WARN_FF_NO_PATH				-305
#define FILEIF_WARN_FF_INVALID_NAME			-306
#define FILEIF_WARN_FF_DENIED				-307
#define FILEIF_WARN_FF_EXIST				-308
#define FILEIF_WARN_FF_INVALID_OBJECT		-309
#define FILEIF_WARN_FF_WRITE_PROTECTED		-310
#define FILEIF_WARN_FF_INVALID_DRIVE		-311
#define FILEIF_WARN_FF_NOT_ENABLED			-312
#define FILEIF_WARN_FF_NO_FILESYSTEM		-313
#define FILEIF_WARN_FF_MKFS_ABORTED			-314
#define FILEIF_WARN_FF_TIMEOUT				-315
#define FILEIF_WARN_FF_LOCKED				-316
#define FILEIF_WARN_FF_NOT_ENOUGH_CORE		-317
#define FILEIF_WARN_FF_TOO_MANY_OPEN_FILES	-318
#define FILEIF_WARN_FF_INVALID_PARAM		-319
#define FILEIF_WARN_FF_UNKNOWN				-399


#endif

#define FILEIF_OP_SUCCESS				(SDCARD_IF_OP_SUCCESS)
#define FILEIF_ERR_INVALID_PARAM		(SDCARD_IF_ERR_INVALID_PARAM)
#define FILEIF_ERR_FILE_NOT_AVAILABLE	(SDCARD_IF_ERR_FILE_NOT_AVAILABLE)
#define FILEIF_ERR_BUFFER_SIZE			(SDCARD_IF_ERR_INVALID_BUFFER_SIZE)
#define FILEIF_ERR_FILE_OFFSET			(SDCARD_IF_ERR_BUFFER_OFFSET)
#define FILEIF_ERR_LINE_NO				(SDCARD_IF_ERR_EVENT_NOT_FOUND)
#define FILEIF_ERR_FILE_ACCESS			(SDCARD_IF_ERR_FILE_ACCESS)
#define FILEIF_ERR_UNINIT				(SDCARD_IF_ERR_NOT_INITIALIZED)

#define FILEIF_WARN_BUFFER_SIZE_SMALL	(SDCARD_IF_WARN_BUFFER_SIZE_SMALL)
#define FILEIF_WARN_BUFFER_SIZE_LARGE	(SDCARD_IF_WARN_BUFFER_SIZE_LARGE)
#define FILEIF_WARN_EIGHTY_PERCENT		(SDCARD_IF_WARN_CAPACITY_EIGHTY_PERCENT)

#define FILEIF_WARN_FF_DISK_ERROR			(SDCARD_IF_WARN_FF_DISK_ERROR)
#define FILEIF_WARN_FF_INTERNAL_ERROR		(SDCARD_IF_WARN_FF_INTERNAL_ERROR)
#define FILEIF_WARN_FF_NOT_READY			(SDCARD_IF_WARN_FF_NOT_READY)
#define FILEIF_WARN_FF_NO_FILE				(SDCARD_IF_WARN_FF_NO_FILE)
#define FILEIF_WARN_FF_NO_PATH				(SDCARD_IF_WARN_FF_NO_PATH)
#define FILEIF_WARN_FF_INVALID_NAME			(SDCARD_IF_WARN_FF_INVALID_NAME)
#define FILEIF_WARN_FF_DENIED				(SDCARD_IF_WARN_FF_DENIED)
#define FILEIF_WARN_FF_EXIST				(SDCARD_IF_WARN_FF_EXIST)
#define FILEIF_WARN_FF_INVALID_OBJECT		(SDCARD_IF_WARN_FF_INVALID_OBJECT)
#define FILEIF_WARN_FF_WRITE_PROTECTED		(SDCARD_IF_WARN_FF_WRITE_PROTECTED)
#define FILEIF_WARN_FF_INVALID_DRIVE		(SDCARD_IF_WARN_FF_INVALID_DRIVE)
#define FILEIF_WARN_FF_NOT_ENABLED			(SDCARD_IF_WARN_FF_NOT_ENABLED)
#define FILEIF_WARN_FF_NO_FILESYSTEM		(SDCARD_IF_WARN_FF_NO_FILESYSTEM)
#define FILEIF_WARN_FF_MKFS_ABORTED			(SDCARD_IF_WARN_FF_MKFS_ABORTED)
#define FILEIF_WARN_FF_TIMEOUT				(SDCARD_IF_WARN_FF_TIMEOUT)
#define FILEIF_WARN_FF_LOCKED				(SDCARD_IF_WARN_FF_LOCKED)
#define FILEIF_WARN_FF_NOT_ENOUGH_CORE		(SDCARD_IF_WARN_FF_NOT_ENOUGH_CORE)
#define FILEIF_WARN_FF_TOO_MANY_OPEN_FILES	(SDCARD_IF_WARN_FF_TOO_MANY_OPEN_FILES)
#define FILEIF_WARN_FF_INVALID_PARAM		(SDCARD_IF_WARN_FF_INVALID_PARAM)
#define FILEIF_WARN_FF_UNKNOWN				(SDCARD_IF_WARN_FF_UNKNOWN)

int32_t FileIF_Initialize(void);
int32_t FileIF_CopyFileToBuffer(const char *filename, uint32_t offset, char *buffer, uint32_t *buf_size, uint32_t *file_size);
int32_t FileIF_IsFileAvailable(const char *filename);
int32_t FileIF_CreateFile(const char *filename);
int32_t FileIF_DeleteFile(const char *filename);
int32_t FileIF_GetFileSize(const char *filename, uint32_t *file_size);
int32_t FileIF_AppendString(const char *filename, const char *string);
int32_t FileIF_GetNoOfLines(const char *filename,uint32_t *no_of_lines);
int32_t FileIF_ReadLine(const char *filename, uint32_t line_no, char *line_buffer, uint32_t *buf_size);
int32_t FileIF_CopyBufferToFile(const char *filename, char *buffer, uint32_t buf_size);

void FileIF_Uninit(void);
#endif
