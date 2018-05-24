#ifndef __UARTSTDIO_H__
#define __UARTSTDIO_H__

#include <stdarg.h>


#ifdef __cplusplus
extern "C"
{
#endif

extern int UARTgets( char *pcBuf, unsigned int ui32Len);
extern unsigned char UARTgetc(void);
extern int UARTwrite(const char *pcBuf, unsigned int ui32Len);
extern void UARTprintf(const char *pcString, ...);
extern void UARTvprintf(const char *pcString, va_list vaArgP);


#ifdef __cplusplus
}
#endif

#endif /* __UARTSTDIO_H__ */
