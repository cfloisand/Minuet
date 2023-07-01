/*	fs_lib.h - Flyingsand Library
 *  v. 0.1
 */

#pragma once
#pragma warning (disable: 4068) // Disable unknown pragmas

#if defined(__APPLE__) && (__MACH__)
#	define FS_PLATFORM_OSX 1
#	define FS_ENV_64BIT 1
#elif defined(_WIN32) || defined(_WIN64)
#	define FS_PLATFORM_WINDOWS 1
#	if defined (_WIN64)
#		define FS_ENV_64BIT 1
#	else
#		define FS_ENV_32BIT 1
#	endif
#endif

#if (defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64))
#   define FS_ARCH_INTEL 1
#elif (defined(__arm__) || defined(__aarch64__))
#   define FS_ARCH_ARM 1
#else
#error "Unsupported architecture."
#endif

#ifdef __clang__
#	define FS_COMPILER_CLANG 1
#elif defined(_MSC_VER)
#	define FS_COMPILER_MSVC 1
#endif


// ==================================================================================
//                      Types
// ==================================================================================

#include <float.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string>
typedef int8_t fsi8;
typedef int16_t fsi16;
typedef int32_t fsi32;
typedef int64_t fsi64;
typedef uint8_t fsu8;
typedef uint16_t fsu16;
typedef uint32_t fsu32;
typedef uint64_t fsu64;

typedef float fsr32;
typedef double fsr64;

#define fsEnumFlags(T, ...) enum struct T : int { __VA_ARGS__ };	\
inline T operator&(T a, T b) { return (T)((int)a & (int)b); }	\
inline T operator|(T a, T b) { return (T)((int)a | (int)b); }	\
inline T operator^(T a, T b) { return (T)((int)a ^ (int)b); }	\
inline T operator~(T a) { return (T)(~(int)a); }				\
inline T& operator&=(T& a, T b) { return (a = a & b); }			\
inline T& operator|=(T& a, T b) { return (a = a | b); }			\
inline T& operator^=(T& a, T b) { return (a = a ^ b); }			\
inline bool fsIsSet(T flags, T mask)	\
{										\
    return ((int)(flags & mask) != 0);	\
}


// ==================================================================================
//                      Constants
// ==================================================================================

#define fsPi32	3.14159265358f
#define fsPi64	3.1415926535897932
#define fsTau32 6.28318530717f
#define fsTau64 6.2831853071795864

#define fsDeg2Rad 0.0174532925199432958
#define fsRad2Deg 57.2957795130823209

enum fsComparisonResult {
    fsEqualTo     = 0,
    fsLessThan    = 1,
    fsGreaterThan = -1
};


// ==================================================================================
//                      Utility macros
// ==================================================================================

#if defined(DEBUG) || defined(_DEBUG) || defined(FS_DEBUG)
#   define FS_DEBUG 1
#	if FS_PLATFORM_OSX
#		define fsAssert(expr) if (!(expr)) { __builtin_trap(); }
#	elif FS_PLATFORM_WINDOWS
#		define fsAssert(expr) if (!(expr)) { *(int *)0 = 0; }
#	endif
#else
#   define fsAssert(expr) if (expr) {}
#endif

#define _FS_STRINGIFY(s) #s

#define _FS_CONSOLE_FORMAT(stream, logger, fmt, ...)                            \
    do {                                                                        \
        if (stream != NULL) {                                                   \
            const size_t kBufferSize = 512;                                     \
            char buf[kBufferSize];                                              \
            char *file = fs_get_last_path_component(__FILE__);                  \
            snprintf(buf, kBufferSize, fmt, ##__VA_ARGS__);                     \
            fprintf(stream, "[" _FS_STRINGIFY(logger) "](%s:%d): %s",           \
                    file, __LINE__, buf);                                       \
            fflush(stream);                                                     \
            free(file);                                                         \
        }                                                                       \
    } while (0)

#define fsLog(fmt, ...) _FS_CONSOLE_FORMAT(stdout, FSLOG, fmt, ##__VA_ARGS__)

#define fsError(fmt, ...) _FS_CONSOLE_FORMAT(stderr, FSERROR, fmt, ##__VA_ARGS__)

#if FS_DEBUG
#   define fsDebugLog(fmt, ...) _FS_CONSOLE_FORMAT(stdout, FSDEBUGLOG, fmt, ##__VA_ARGS__)
#else
#   define fsDebugLog(...)
#endif

#define fsArrayCount(x) (sizeof(x)/sizeof((x)[0]))

#define fsMin(a, b) ((a) < (b) ? (a) : (b))
#define fsMax(a, b) ((a) > (b) ? (a) : (b))
#define fsClamp(x, xmin, xmax) ((x) < (xmin) ? (xmin) : (x) > (xmax) ? (xmax) : (x))
#define fsClampMin(x, xmin) ((x) < (xmin) ? (xmin) : (x))
#define fsClampMax(x, xmax) ((x) > (xmax) ? (xmax) : (x))

#define fsSign(x) ((x) < 0 ? -1 : 1)

#define fsIsPowerOf2(x) (!((x) & ((x)-1)) && (x))
#define fsIsDenormal(x) (((*(unsigned int *) & (x)) & 0x7f800000) == 0)
#define fsIsAlmostDenormal(x) (((*(unsigned int *) & (x)) & 0x7f800000) == 0x08000000)

#define fsdB2Lin(x) pow(10.0, ((x)/20.0))
#define fsLin2dB(x) ((x) > 0.0 ? (20.0 * log10((x))) : -60.0)

#define fsSwap(a, b) do { decltype(a) t = (a); a = (b); b = t; } while (0)

#define fsKilobytes(value) ((value)*1024LL)
#define fsMegabytes(value) (fsKilobytes((value))*1024LL)
#define fsGigabytes(value) (fsMegabytes((value))*1024LL)
#define fsTerabytes(value) (fsGigabytes((value))*1024LL)

#define fsIsAligned(ptr, alignment) (!((uintptr_t)ptr & (alignment - 1)))

template <typename T>
void fs_swap_pointers(T **a, T **b) {
    T *temp = *a;
    *a = *b;
    *b = temp;
}


// ==================================================================================
//                      Memory
// ==================================================================================

/*! @brief Copies \c size bytes pointed to by \c src into \c dst. Memory pointed to by the pointers cannot overlap. */
void fs_memcpy(const void *src, void *dst, size_t size);
/*! @brief Clears the memory with \c size bytes to 0. */
void fs_memclear(void *ptr, size_t size);
/*! @brief Compares \c size bytes in the two memory blocks pointed at by \c ptr1 and \c ptr2 
    and returns true if they are equal, and false otherwise. */
bool fs_memcmp_equal(const void *ptr1, const void *ptr2, size_t size);


// ==================================================================================
//                      Bitwise Operations & I/O
// ==================================================================================

enum struct fsByteOrder {
    littleEndian,
    bigEndian
};

template <fsByteOrder T>
struct fsBitStream {
    fsu8 *stream;
    fsu64 bitBuffer = 0;
    fsi32 bitCount = 0;
    
    
    fsBitStream(fsu8 *ptr)
        : stream(ptr)
    {}
    
    fsu64 get_bits(fsi32 bits);
    fsu8 get_bit();
    fsu8 get_byte();
    fsu8 peek_byte();
    void refill(fsi32 bits);
    void flush_byte();
    void advance_bytes(fsu32 bytes);
    void reset(fsu8 *ptr);
    
    template<typename U>
    U get(fsi32 bits = sizeof(U) * 8);
};

template <fsByteOrder T>
fsu8 fsBitStream<T>::get_bit() {
    fsu8 val = (fsu8)get_bits(1);
    return val;
}

template <fsByteOrder T>
fsu8 fsBitStream<T>::get_byte() {
    return get<fsu8>();
}

template <fsByteOrder T>
void fsBitStream<T>::reset(fsu8 *ptr) {
    stream = ptr;
    bitBuffer = 0;
    bitCount = 0;
}

template <fsByteOrder T>
template<typename U>
U fsBitStream<T>::get(fsi32 bits) {
    fsAssert(bits > 0 && (fsu64)bits <= sizeof(U) * 8);
    U val = (U)get_bits(bits);
    return val;
}

// NOTE(christian): Specializations for signed integer types that convert to two's complement by sign extension.
template<> template<> fsi16 fsBitStream<fsByteOrder::bigEndian>::get(fsi32 bits);
template<> template<> fsi16 fsBitStream<fsByteOrder::littleEndian>::get(fsi32 bits);
template<> template<> fsi32 fsBitStream<fsByteOrder::bigEndian>::get(fsi32 bits);
template<> template<> fsi32 fsBitStream<fsByteOrder::littleEndian>::get(fsi32 bits);


template <typename T>
T fs_byte_swap(T val);

/*! @brief Counts number of leading zeroes in the given byte. */
fsu32 fs_clz(fsu8 byte);
/*! @brief Counts number of leading zeroes in the given 32-bit value. */
fsu32 fs_clz(fsu32 val);
/*! @brief Counts number of leading zeroes in the given 64-bit value. */
fsu32 fs_clz(fsu64 val);
/*! @brief Returns the position of the first set bit in the given byte. */
fsu32 fs_ffs(fsu8 byte);
/*! @brief Returns the position of the first set bit in the given value. */
fsu32 fs_ffs(fsu32 val);


// ==================================================================================
//                      Math
// ==================================================================================

#define fsEpsilon32 FLT_EPSILON
#define fsEpsilon64 DBL_EPSILON

/*! @brief Compares the two floating-point numbers for equality using both absolute and relative tolerance with the given epsilon value. */
bool fs_are_equal(fsr32 a, fsr32 b, fsr32 epsilon = fsEpsilon32);
/*! @brief Compares the two floating-point numbers for equality using both absolute and relative tolerance with the given epsilon value. */
bool fs_are_equal(fsr64 a, fsr64 b, fsr64 epsilon = fsEpsilon64);
/*! @brief Compares the two floating-point numbers for equality using absolute tolerance with the given epsilon value. */
bool fs_are_equal_absolute(fsr32 a, fsr32 b, fsr32 epsilon = fsEpsilon32);
/*! @brief Compares the two floating-point numbers for equality using relative tolerance with the given epsilon value. */
bool fs_are_equal_relative(fsr32 a, fsr32 b, fsr32 epsilon = fsEpsilon32);
/*! @brief Compares the two floating-point numbers for equality using absolute tolerance with the given epsilon value. */
bool fs_are_equal_absolute(fsr64 a, fsr64 b, fsr64 epsilon = fsEpsilon64);
/*! @brief Compares the two floating-point numbers for equality using relative tolerance with the given epsilon value. */
bool fs_are_equal_relative(fsr64 a, fsr64 b, fsr64 epsilon = fsEpsilon64);

/*! @brief Returns a 32-bit floating point random number between 0 and 1 using xorshift. */
fsr32 fs_random01();
/*! @brief Returns a 32-bit unsigned pseudo-random integer given a seed. */
fsu32 fs_random(fsu32 seed);
/*! @brief Returns a 32-bit unsigned random integer using xorshift. */
fsu32 fs_random_xorshift();
/*! @brief Returns a 32-bit unsigned random integer using Wang Hash.*/
fsu32 fs_random_wang_hash(fsu32 seed);

/*! @brief Linear interpolation between \c from and \c to given interpolation value \c t. */
fsr32 fs_lerp(fsr32 from, fsr32 t, fsr32 to);

/*! @brief Maps \c val that lies within a range \c fromLow to \c fromHigh to the range \c toLow - \c toHigh. */
fsr32 fs_map_to_range(fsr32 val, fsr32 fromLow, fsr32 fromHigh, fsr32 toLow, fsr32 toHigh);
/*! @brief Maps \c val that is in the range [0..1] to the range \c toLow - \c toHight. */
fsr32 fs_map01_to_range(fsr32 val, fsr32 toLow, fsr32 toHigh);
/*! @brief Maps \c val that lies within a range \c fromLow to \c fromHigh to the range [0..1]. */
fsr32 fs_map_to_range01(fsr32 val, fsr32 fromLow, fsr32 fromHigh);

/*! @brief Rounds 32-bit floating point value towards zero. */
fsi32 fs_round_to_zero(fsr32 val);
/*! @brief Rounds 64-bit floating point value towards zero. */
fsi64 fs_round_to_zero(fsr64 val);
/*! @brief Rounds 32-bit floating point value away from zero. */
fsi32 fs_round_from_zero(fsr32 val);
/*! @brief Rounds 64-bit floating point value away from zero. */
fsi64 fs_round_from_zero(fsr64 val);

/*! @brief Rounds the integer up to the next power of 2. */
template <typename T>
T fs_next_powerof2(T val) {
    static_assert(std::is_integral<T>::value, "Type must be integral.");
    return (val == 1 ? 1 : ((T)1 << ((T)(sizeof(T) * 8) - fs_clz(val - 1))));
}

template <typename T>
T fs_next_multiple(T val, T multiple) {
    static_assert(std::is_integral<T>::value, "Type must be integral.");
    if (multiple == 0) {
        return val;
    }
    
    T remainder = val % multiple;
    if (remainder == 0) {
        return val;
    }
    
    return (val + multiple - remainder);
}

/*! @brief Rotate 16-bit value left by @c bits. */
fsu16 fs_rotl16(fsu16 val, fsu8 bits);
/*! @brief Rotate 32-bit value left by @c bits. */
fsu32 fs_rotl32(fsu32 val, fsu8 bits);
/*! @brief Rotate 64-bit value left by @c bits. */
fsu64 fs_rotl64(fsu64 val, fsu8 bits);

/*! @brief Rotate 16-bit value right by @c bits. */
fsu16 fs_rotr16(fsu16 val, fsu8 bits);
/*! @brief Rotate 32-bit value right by @c bits. */
fsu32 fs_rotr32(fsu32 val, fsu8 bits);
/*! @brief Rotate 64-bit value right by @c bits. */
fsu64 fs_rotr64(fsu64 val, fsu8 bits);


// ==================================================================================
//                      File IO
// ==================================================================================

/*! @brief Returns the size of the file in bytes. */
size_t fs_get_file_size(const char *fileName);

/*! @brief Reads the data from a binary file into fileData, and fills in the size of the data in bytes into fileSize. 
    Returns true on success. */
bool fs_read_file(const char *fileName, void **fileData, size_t *fileSize);
/*! @brief Reads the data from a binary file into fileData, and fills in the size of the data in bytes into fileSize. 
    Memory allocated to hold the file's data is done by the provided allocator. Returns true on success. */
bool fs_read_file(const char *fileName, void **fileData, size_t *fileSize, void* (*allocator)(size_t,void*), void *userData);
/*! @brief Reads the data from a binary file into a pre-allocated buffer that has a size of \c bufferSize. 
	The method reads up to \c bufferSize number of bytes or the size of the file, whichever is smaller. */
bool fs_read_file(const char *fileName, void *buffer, size_t bufferSize);

char* fs_read_line(FILE *file);

char* fs_read_line(FILE *file, char *buffer, size_t maxLength);


// ==================================================================================
//                      C Strings
// ==================================================================================

/*! @brief Returns the length of the null-terminated string (does not count the null terminator). */
size_t fs_strlen(const char *str);

/*! @brief Returns true if the two null-terminated strings are equal. Up to \c length number of characters will be compared. */
bool fs_are_equal(const char *str0, const char *str1, size_t length);

/*! @brief Returns true if \c c is a whitespace character. */
bool fs_is_whitespace(const char c);
/*! @brief Returns true if\c c is a line-ending character. */
bool fs_is_line_ending(const char c);

/*! @brief Splits the given string by the character \c delimeter and returns the components. The number of components returned is written to \c count . */
char** fs_split(const char *str, char delimeter, size_t *count);

/*! @brief Returns the last path component in the given path as a null-terminated string.
    @note The caller is responsible for freeing the returned string. */
char* fs_get_last_path_component(const char *path);

/*! @brief Strips off the last path component from the given path and returns the new null-terminated path.
    @note The caller is responsible for freeing the returned path. */
char* fs_trim_last_path_component(const char *path);

/*! @brief Appends the given null-terminated path component to \c path and returns the new null-terminated path.
    @note The caller is responsible for freeing the returned path. */
char* fs_append_path_component(const char *path, const char *component);

/*! @brief Returns the file extension for the given file, or null if it doesn't have one. */
char* fs_get_file_extension(const char *file);

/*! @brief Converts the given null-terminated string to a signed integer. */
fsi64 fs_to_integer(const char *string);
/*! @brief Converts the given integer (in base 10) to a null-terminated string.
    @note The caller is responsible for freeing the returned string. */
char* fs_to_string(fsi64 num);

/*! @brief Encodes the given data to Base64 representation.
    @note The caller is responsible for freeing the returned string. */
char* fs_base64_encode(fsu8 *data, size_t dataSize);
/*! @brief Encodes the given data to URL-safe Base64 representation.
    @note The caller is responsible for freeing the returned string. */
char* fs_base64_urlencode(fsu8 *data, size_t dataSize);
/*! @brief Encodes the given null-terminated string to Base64 representation.
    @note The caller is responsible for freeing the returned string. */
char* fs_base64_encode(const char *string);
/*! @brief Encodes the given null-terminated string to URL-safe Base64 representation.
    @note The caller is responsible for freeing the returned string. */
char* fs_base64_urlencode(const char *string);

/*! @brief Decodes the given Base64 string.
    @note The caller is responsible for freeing the returned data. */
fsu8* fs_base64_decode(const char *string);


// ==================================================================================
//                      Comparison
// ==================================================================================

template <typename T>
struct fsEqual
{
    bool operator() (const T& a, const T& b) const {
        return a == b;
    }
};

template <typename T>
struct fsCompare
{
    fsComparisonResult operator() (const T& a, const T& b) const {
        return (a < b ? fsLessThan : (a > b ? fsGreaterThan : fsEqualTo));
    }
};


// ==================================================================================
//                      Hashing
// ==================================================================================

template <typename T>
struct fsHash;

template<>
struct fsHash<std::string>
{
    size_t operator() (const std::string& key) const {
        // DJB string hash.
        size_t hash = 5381;
        for (size_t i = 0; i < key.length(); ++i) {
            hash = ((hash << 5) + hash) + key[i];
        }
        return hash;
    }
};

template<>
struct fsHash<char>
{
    size_t operator() (const char& key) const {
        return (size_t)key;
    }
};

template<>
struct fsHash<int32_t>
{
    size_t operator() (const int32_t& key) const {
        // Knuth's multiplicative method.
        size_t hash = (key * 2654435761) % 4294967296;
        return hash;
    }
};

template<>
struct fsHash<uint32_t>
{
    size_t operator() (const uint32_t& key) const {
        fsHash<int32_t> hash;
        return hash((int32_t)key);
    }
};

template<>
struct fsHash<int64_t>
{
    size_t operator() (const int64_t& key) const {
        // Thomas Wang's 64 bit mix function
        size_t hash = (size_t)(key + ~(key << 32));
        hash ^= (hash >> 22);
        hash += ~(hash << 13);
        hash ^= (hash >> 8);
        hash += (hash << 3);
        hash ^= (hash >> 15);
        hash += ~(hash << 27);
        hash ^= (hash >> 31);
        return hash;
    }
};

template<>
struct fsHash<uint64_t>
{
    size_t operator() (const uint64_t& key) const {
        fsHash<int64_t> hash;
        return hash((int64_t)key);
    }
};

template<>
struct fsHash<char const* const&>
{
    size_t operator() (char const* const& key) const {
        fsHash<std::string> hash;
        return hash(std::string(key));
    }
};

template<>
struct fsHash<long>
{
    size_t operator() (long &key) const {
        fsHash<int64_t> hash;
        return hash((int64_t)key);
    }
};


// ==================================================================================
//                      Timing
// ==================================================================================

struct fsTimingToken;

/*! @brief Starts a timing session and returns a token representing the session. */
fsTimingToken* fs_timing_start();
/*! @brief Stops the timing session represented by the given token, and returns the elapsed time in milliseconds.
    @note The token is no longer valid after this call is made. */
fsr64 fs_timing_stop(fsTimingToken *token);
