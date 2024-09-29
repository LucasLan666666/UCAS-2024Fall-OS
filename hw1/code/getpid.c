#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <stdint.h>

// 使用 glibc 提供的库函数调用 getpid
long measure_glibc() {
    struct timespec start, end;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    getpid();
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    return (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
}

// 使用 syscall 函数直接调用系统调用 getpid
long measure_syscall() {
    struct timespec start, end;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    syscall(SYS_getpid);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    return (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
}

// 使用内联汇编调用 getpid
long measure_inline_asm() {
    struct timespec start, end;
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    asm volatile (
        "syscall"
        :
        : "a" (SYS_getpid)
        : "rcx", "r11", "memory"
    );
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
    return (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
}

int main() {
    long time_glibc      = measure_glibc();
    long time_syscall    = measure_syscall();
    long time_inline_asm = measure_inline_asm();

    printf("CPU time for call (glibc)     : %7ld ns\n", time_glibc);
    printf("CPU time for call (syscall)   : %7ld ns\n", time_syscall);
    printf("CPU time for call (inline asm): %7ld ns\n", time_inline_asm);

    return 0;
}
