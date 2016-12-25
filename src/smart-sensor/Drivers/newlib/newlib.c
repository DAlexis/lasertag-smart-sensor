#include <errno.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/unistd.h>
#include <sys/time.h>

#include "stm32f0xx_hal.h"
#include "usart.h"

#undef errno
extern int errno;

char *__env[1] = { 0 };
char **environ = __env;

int _write(int file, char *ptr, int len);

void _exit(int status)
{
	UNUSED(status);
    while (1);
}

int _close(int file)
{
	UNUSED(file);
    return -1;
}

int _execve(char *name, char **argv, char **env)
{
	UNUSED(name); UNUSED(argv); UNUSED(env);
    errno = ENOMEM;
    return -1;
}

int _fork()
{
    errno = EAGAIN;
    return -1;
}

int _fstat(int file, struct stat *st)
{
	UNUSED(file);
    st->st_mode = S_IFCHR;
    return 0;
}

int _getpid()
{
    return 1;
}
/*
int _gettimeofday(struct timeval *tv, struct timezone *tz)
{
    tv->tv_sec = virtualTimer / 1000;
    tv->tv_usec = (virtualTimer % 1000) * 1000;
    return 0;
}*/

int _isatty(int file)
{
    switch (file)
    {
    case STDOUT_FILENO:
    case STDERR_FILENO:
    case STDIN_FILENO:
        return 1;
    default:
        //errno = ENOTTY;
        errno = EBADF;
        return 0;
    }
}

int _kill(int pid, int sig)
{
	UNUSED(pid); UNUSED(sig);
    errno = EINVAL;
    return (-1);
}

int _link(char *old, char *new)
{
	UNUSED(old); UNUSED(new);
    errno = EMLINK;
    return -1;
}

int _lseek(int file, int ptr, int dir)
{
	UNUSED(file); UNUSED(ptr); UNUSED(dir);
    return 0;
}

caddr_t _sbrk(int incr)
{
    /*
    extern char _ebss;
    static char *heap_end= &_ebss;
    char *prev_heap_end;

    prev_heap_end = heap_end;

    char * stack = (char*) __get_MSP();
    if (heap_end + incr > stack)
    {
        _write(STDERR_FILENO, "Heap and stack collision\n", 25);
        errno = ENOMEM;
        return (caddr_t) - 1;
        //abort ();
    }

    heap_end += incr;
    return (caddr_t) prev_heap_end;
*/
	extern char __bss_end__;
	extern char _estack;
	char* heapLimit = &_estack - 0x200;

    //extern char isMemoryCorrupted;

    static char* current_heap_end;
    char* current_block_address;

    if (current_heap_end == 0)
    {
      current_heap_end = &__bss_end__;
    }

    current_block_address = current_heap_end;

    // Need to align heap to word boundary, else will get
    // hard faults on Cortex-M0. So we assume that heap starts on
    // word boundary, hence make sure we always add a multiple of
    // 4 to it.
    incr = (incr + 3) & (~3); // align value to 4
    if (current_heap_end + incr > heapLimit)
    {
      // Some of the libstdc++-v3 tests rely upon detecting
      // out of memory errors, so do not abort here.
    #if 0
      extern void abort (void);

      _write (1, "_sbrk: Heap and stack collision\n", 32);

      abort ();
    #else
      // Heap has overflowed
      //isMemoryCorrupted = 1;

      errno = ENOMEM;
      return (caddr_t) - 1;
    #endif
    }

    current_heap_end += incr;

    return (caddr_t) current_block_address;
}

int _read(int file, char *ptr, int len)
{
	UNUSED(ptr); UNUSED(len);
    switch (file)
    {
    case STDIN_FILENO:
        HAL_UART_Receive(&huart1, (uint8_t *)ptr, 1, HAL_MAX_DELAY);
    default:
        errno = EBADF;
        return -1;
    }
}

int _stat(const char *filepath, struct stat *st)
{
	UNUSED(filepath);
    st->st_mode = S_IFCHR;
    return 0;
}

clock_t _times(struct tms *buf)
{
	UNUSED(buf);
    return -1;
}

int _unlink(char *name)
{
	UNUSED(name);
    errno = ENOENT;
    return -1;
}

int _wait(int *status)
{
	UNUSED(status);
    errno = ECHILD;
    return -1;
}

int _write(int file, char *ptr, int len)
{
    switch (file)
    {
    case STDOUT_FILENO: // stdout
        HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, HAL_MAX_DELAY);
        break;
    case STDERR_FILENO: // stderr
        HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, HAL_MAX_DELAY);
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return len;
}
