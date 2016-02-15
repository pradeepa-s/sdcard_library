/**
 * @file sdcard_err_codes.h
 * @author Pradeepa Senanayake (pradeepa.kck@gmail.com)
 * @date 31 Jan 2016
 * @brief File contains error codes for sdcard_interface API.
 *
 * Include this file in any place to refer to these definitions.
 */

#ifndef _SDCARD_ERR_CODES_H
#define _SDCARD_ERR_CODES_H

#define SDCARD_IF_OP_SUCCESS				0
#define SDCARD_IF_ERR_NOT_INITIALIZED		-1
#define SDCARD_IF_ERR_INVALID_PARAM			-2
#define SDCARD_IF_ERR_FILE_NOT_AVAILABLE	-3
#define SDCARD_IF_ERR_AUDIO_BUFFER_NOT_SET	-4
#define SDCARD_IF_ERR_INVALID_BUFFER_SIZE	-5
#define SDCARD_IF_ERR_BUFFER_OFFSET			-6
#define SDCARD_IF_ERR_EVENT_COUNT			-7
#define SDCARD_IF_ERR_EVENT_NOT_FOUND		-8


#define SDCARD_IF_WARN_BUFFER_SIZE_SMALL		-200
#define SDCARD_IF_WARN_BUFFER_SIZE_LARGE		-201
#define SDCARD_IF_WARN_LESS_NO_Of_EVENTS_AVAIL	-202
#endif
