/* Stand-in for libStormGLOFT.so (arm32 only in the original APK; libDungeonHunter2.so does not link it, the Java class just loads it).
 * First-boot debugging aid: on load it installs a guest-side crash handler that logs pc, lr, sp and a stack-scan backtrace of
 * addresses inside libDungeonHunter2.so (as offsets, symbolize with tools/trackb/boot/symbolize.sh), because the Android
 * ndk_translation tombstone has no guest registers for a jump to a non-executable address. */
#define _GNU_SOURCE
#include <android/log.h>
#include <link.h>
#include <signal.h>
#include <stdint.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <ucontext.h>
#include <unistd.h>

static uintptr_t g_base, g_end;

static int phdr_cb(struct dl_phdr_info* info, size_t sz, void* data) {
    (void)sz; (void)data;
    if (info->dlpi_name && strstr(info->dlpi_name, "libDungeonHunter2.so")) {
        g_base = info->dlpi_addr;
        for (int i = 0; i < info->dlpi_phnum; i++)
            if (info->dlpi_phdr[i].p_type == PT_LOAD) {
                uintptr_t e = info->dlpi_addr + info->dlpi_phdr[i].p_vaddr + info->dlpi_phdr[i].p_memsz;
                if (e > g_end) g_end = e;
            }
    }
    return 0;
}

#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "DH2CRASH", __VA_ARGS__)

static void handler(int sig, siginfo_t* si, void* ctx) {
    static volatile int busy;
    signal(sig, SIG_DFL);
    if (busy++) return;
    ucontext_t* uc = (ucontext_t*)ctx;
    uintptr_t pc = uc->uc_mcontext.pc, lr = uc->uc_mcontext.regs[30], sp = uc->uc_mcontext.sp, fp = uc->uc_mcontext.regs[29];
    dl_iterate_phdr(phdr_cb, 0);
    LOGE("signal %d code %d addr %p  base %p", sig, si->si_code, si->si_addr, (void*)g_base);
    LOGE("pc %p (off 0x%lx)  lr %p (off 0x%lx)  sp %p  fp %p", (void*)pc, (unsigned long)(pc - g_base), (void*)lr,
         (unsigned long)(lr - g_base), (void*)sp, (void*)fp);
    for (int i = 0; i < 31; i += 4)
        LOGE("x%-2d %016lx x%-2d %016lx x%-2d %016lx x%-2d %016lx", i, (unsigned long)uc->uc_mcontext.regs[i], i + 1,
             (unsigned long)uc->uc_mcontext.regs[i + 1], i + 2, (unsigned long)uc->uc_mcontext.regs[i + 2], i + 3,
             (unsigned long)(i + 3 < 31 ? uc->uc_mcontext.regs[i + 3] : 0));
    int n = 0;
    for (uintptr_t* p = (uintptr_t*)sp; n < 40 && (uintptr_t)p < sp + 16384; p++) {
        uintptr_t v = *p;
        if (v >= g_base && v < g_end) { LOGE("stack %p: off 0x%lx", (void*)p, (unsigned long)(v - g_base)); n++; }
    }
}

/* The game prints diagnostics with printf (stdout is /dev/null in an Android app): forward stdout and stderr to logcat. */
static void* pump(void* arg) {
    int fd = (int)(intptr_t)arg;
    char buf[2048];
    size_t n = 0;
    for (;;) {
        char c;
        ssize_t r = read(fd, &c, 1);
        if (r <= 0) break;
        if (c == '\n' || n == sizeof buf - 1) { buf[n] = 0; __android_log_write(ANDROID_LOG_INFO, "DH2STDOUT", buf); n = 0; }
        else buf[n++] = c;
    }
    return 0;
}

__attribute__((constructor)) static void install(void) {
    int fds[2];
    if (pipe(fds) == 0) {
        dup2(fds[1], 1);
        dup2(fds[1], 2);
        setvbuf(stdout, 0, _IONBF, 0);
        setvbuf(stderr, 0, _IONBF, 0);
        pthread_t t;
        pthread_create(&t, 0, pump, (void*)(intptr_t)fds[0]);
        printf("stdout pump up\n");
    }
    struct sigaction sa;
    memset(&sa, 0, sizeof sa);
    sa.sa_sigaction = handler;
    sa.sa_flags = SA_SIGINFO | SA_NODEFER;
    LOGE("crash handler installed");
    int sigs[] = {SIGSEGV, SIGBUS, SIGILL, SIGFPE, SIGABRT, SIGTRAP};
    for (unsigned i = 0; i < sizeof sigs / sizeof *sigs; i++) { int r = sigaction(sigs[i], &sa, 0); LOGE("sigaction %d -> %d", sigs[i], r); }
}
