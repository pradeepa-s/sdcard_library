/**
 * @file sdcard_err_codes.h
 * @author Pradeepa Senanayake (pradeepa.kck@gmail.com)
 * @date 31 Jan 2016
 * @brief File contains error codes for sdcard_interface API.
 *
 * Include this file in any place to refer to these definitions.
 * 
 * Version v1.0.0
 */

#ifndef _SDCARD_ERR_CODES_H
#define _SDCARD_ERR_CODES_H

#define SDCARD_IF_OP_SUCCESS				((int32_t)0)
#define SDCARD_IF_ERR_NOT_INITIALIZED		((int32_t)-1)
#define SDCARD_IF_ERR_INVALID_PARAM			((int32_t)-2)
#define SDCARD_IF_ERR_FILE_NOT_AVAILABLE	((int32_t)-3)
#define SDCARD_IF_ERR_AUDIO_BUFFER_NOT_SET	((int32_t)-4)
#define SDCARD_IF_ERR_INVALID_BUFFER_SIZE	((int32_t)-5)
#define SDCARD_IF_ERR_BUFFER_OFFSET			((int32_t)-6)
#define SDCARD_IF_ERR_EVENT_COUNT			((int32_t)-7)
#define SDCARD_IF_ERR_EVENT_NOT_FOUND		((int32_t)-8)
#define SDCARD_IF_ERR_FILE_ACCESS			((int32_t)-9)


#define SDCARD_IF_WARN_BUFFER_SIZE_SMALL		((int32_t)-200)
#define SDCARD_IF_WARN_BUFFER_SIZE_LARGE		((int32_t)-201)
#define SDCARD_IF_WARN_LESS_NO_Of_EVENTS_AVAIL	((int32_t)-202)
#define SDCARD_IF_WARN_CAPACITY_EIGHTY_PERCENT	((int32_t)-203)

#define SDCARD_IF_WARN_FF_DISK_ERROR			((int32_t)-301)
#define SDCARD_IF_WARN_FF_INTERNAL_ERROR		((int32_t)-302)
#define SDCARD_IF_WARN_FF_NOT_READY				((int32_t)-303)
#define SDCARD_IF_WARN_FF_NO_FILE				((int32_t)-304)
#define SDCARD_IF_WARN_FF_NO_PATH				((int32_t)-305)
#define SDCARD_IF_WARN_FF_INVALID_NAME			((int32_t)-306)
#define SDCARD_IF_WARN_FF_DENIED				((int32_t)-307)
#define SDCARD_IF_WARN_FF_EXIST					((int32_t)-308)
#define SDCARD_IF_WARN_FF_INVALID_OBJECT		((int32_t)-309)
#define SDCARD_IF_WARN_FF_WRITE_PROTECTED		((int32_t)-310)
#define SDCARD_IF_WARN_FF_INVALID_DRIVE			((int32_t)-311)
#define SDCARD_IF_WARN_FF_NOT_ENABLED			((int32_t)-312)
#define SDCARD_IF_WARN_FF_NO_FILESYSTEM			((int32_t)-313)
#define SDCARD_IF_WARN_FF_MKFS_ABORTED			((int32_t)-314)
#define SDCARD_IF_WARN_FF_TIMEOUT				((int32_t)-315)
#define SDCARD_IF_WARN_FF_LOCKED				((int32_t)-316)
#define SDCARD_IF_WARN_FF_NOT_ENOUGH_CORE		((int32_t)-317)
#define SDCARD_IF_WARN_FF_TOO_MANY_OPEN_FILES	((int32_t)-318)
#define SDCARD_IF_WARN_FF_INVALID_PARAM			((int32_t)-319)
#define SDCARD_IF_WARN_FF_UNKNOWN				((int32_t)-399)
#endif
