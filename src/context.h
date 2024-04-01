#ifndef CONTEXT_H
#define CONTEXT_H

// determine compiler
#if defined(_MSC_VER)
#define COMPILER_MSVC 1
#elif defined(__clang__)
#define COMPILER_CLANG 1
#elif defined(__GNUC__)
#define COMPILER_GCC 1
#else
#error unsupported compiler (use msvc, clang, or gcc)
#endif

// determine operating system
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)
#define OS_WINDOWS 1
#elif defined(__linux__)
#define OS_LINUX 1
#elif defined(__APPLE__) && defined(__MACH__)
#define OS_MAC 1
#else
#error unsupported os (use windowx, linux, or macOS)
#endif

// determine cpu architecture
#if defined(__amd64__) || defined(_amd64) || defined(__x86_64) || defined(__x86_64)                \
    || defined(_M_AMD64) || defined(_M_X64)
#define ARCH_X64 1
#elif defined(i386) || defined(__i386) || defined(__i386__) || defined(_M_I86) || defined(_IX86)   \
    || defined(__X86__)
#define ARCH_X32 1
#elif defined(__arm__) || || defined(__thumb__) || defined(_M_ARM) || defined(_M_ARMT)
#define ARCH_ARM 1
#elif
#error unsupported machine architecture (use x64, x86_64, or arm)
#endif

// zero fill missing context macros
#if !defined(COMPILER_MSVC)
#define COMPILER_MSVC 0
#endif
#if !defined(COMPILER_GCC)
#define COMPILER_GCC 0
#endif
#if !defined(COMPILER_CLANG)
#define COMPILER_CLANG 0
#endif

#if !defined(OS_WINDOWS)
#define OS_WINDOWS 0
#endif

#if !defined(OS_LINUX)
#define OS_LINUX 0
#endif

#if !defined(OS_MAC)
#define OS_MAC 0
#endif

#endif /* ifndef CONTEXT_H */
