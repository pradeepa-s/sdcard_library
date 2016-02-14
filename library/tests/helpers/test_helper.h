#ifndef _SDCARD_INTERFACE_TEST_HELPER_H
#define _SDCARD_INTERFACE_TEST_HELPER_H

char IsFileAvailable(const char *filename);
void DeleteFile(const char *filename);
char CompareFiles(const char *file1, const char *file2);

#endif
