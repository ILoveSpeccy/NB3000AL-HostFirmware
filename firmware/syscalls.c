#include <stdio.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include "hardware.h"

void sendchar(const char value)
{
//   while(UART_STATUS);
//   UART_DATA = value;
}

caddr_t sbrk(int incr)
{
   extern char _end; /* Defined by the linker */
   static char *heap_end;
   char *prev_heap_end;
   if (heap_end == 0)
   {
      heap_end = &_end;
   }
   prev_heap_end = heap_end;
   heap_end += incr;
   return (caddr_t) prev_heap_end;
}

int isatty(int file)
{
   return 0;
}

int write(int fd, const void *_buf, size_t nbyte )
{
   const char *buf = _buf;
   int i;

   if (fd != 1)
   {
      errno = EBADF;
      return -1;
   }

   for (i = 0; i < nbyte; i++)
      sendchar(*buf++);

   return nbyte;
}

int fstat(int file, struct stat *st)
{
//   st->st_mode = S_IFCHR;
   return EBADF;
}

int lseek(int file, int ptr, int dir)
{
   return EBADF;
}

ssize_t read(int fd, void *buf, size_t count) {
	errno = EBADF;
	return -1;
}

int atexit(void (*function)(void))
{
   return -1;
}

pid_t getpid()
{
   return 0;
}

int kill(int pid, int sig)
{
   errno = ESRCH;
   return -1;
}

int open(const char *a, int b, ...)
{
   errno = EACCES;
   return -1;
}

int close(int fd)
{
   errno = EBADF;
   return -1;
}
