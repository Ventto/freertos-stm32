/*
 * C
 *
 * Copyright 2012-2013 IS2T. All rights reserved.
 * For demonstration purpose only.
 * IS2T PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

/*
 * Implementation of the newlib C Library underlying subroutines.
 *
 * If you use the C library on a system that complies with the POSIX.1 standard
 * most of these subroutines are supplied with your operating system.
 *
 * If some of these subroutines are not provided with your system (if you are
 * developing software for a Ã¢â‚¬Å“bare boardÃ¢â‚¬ï¿½ system, without an OS) you will at
 * least need to provide do-nothing stubs (or subroutines with minimal
 * functionality) to allow your programs to link with the subroutines in libc.a.
 *
 * These minimal implementations are extracted from the newlib C Library
 * documentation.
 */

#include <sys/stat.h>
#include <stdio.h>

#include "stm32f4xx_rcc.h"

#include "FreeRTOS.h"
#include "task.h"

int __errno;

/**
 * Read from a file. Minimal implementation
 */
int _read(int file, char *ptr, int len) {
	return 0;
}

/**
 * Increase program data space. As malloc and related functions depend on this,
 * it is useful to have a working implementation. The following suffices for a
 * standalone system; it exploits the symbol _end automatically defined by the
 * GNU linker.
 */
caddr_t _sbrk(int incr) {
	extern char _end; // Defined by the linker
	static char *heap_end;
	char *prev_heap_end;
	if (heap_end == 0) {
		heap_end = &_end;
	}
	prev_heap_end = heap_end;
	heap_end += incr;
	return (caddr_t) prev_heap_end;
}

/**
 * Close a file.
 * Minimal implementation.
 */
int _close(int file) {
	return -1;
}


/**
 * Status of an open file. For consistency with other minimal implementations
 * in these examples, all files are regarded as character special devices. The
 * Ã¢â‚¬Ëœsys/stat.hÃ¢â‚¬â„¢ header file required is distributed in the Ã¢â‚¬ËœincludeÃ¢â‚¬â„¢ subdirectory
 * for this C library.
 */
int _fstat(int file, struct stat *st) {
	st->st_mode = S_IFCHR;
	return 0;
}

/**
 * Set position in a file. Minimal implementation.
 */
int _lseek(int file, int ptr, int dir) {
	return 0;
}

/**
 * Query whether output stream is a terminal.
 * Minimal implementation for consistency with the other
 * minimal implementations, which only support output to stdout.
 */
int _isatty(int file) {
	return 1;
}

/**
 * get the id of the process.
 * Minimal implementation.
 */
int _getpid (void)
{
	return -1; /*return -1*/
}
/**
 * kill process.
 * Minimal implementation.
 */
int _kill (int pid, int sig)
{
	return -1; /*return -1*/
}

/**
 * exit.
 * Minimal implementation.
 */
void _exit (int status)
{
     /*Nothing to do */
}

/* called by newlibC for printf  */
int fputc(int ch, FILE *f)
{
    if (ch == '\n') {
        int rc = fputc('\r', f);
        if (rc == EOF)
            return rc;
    }
    USART_SendData(USART1, (uint8_t) ch);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    return ch;
}

int _write(int file, char *ptr, int len)
{
    taskENTER_CRITICAL();
    int remaining = len;
    while(--remaining >= 0){
        fputc(*ptr, (FILE*)0);
        ++ptr;
    }
    taskEXIT_CRITICAL();
    return len;
}
