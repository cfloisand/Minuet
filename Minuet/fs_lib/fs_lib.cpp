/*	fs_lib.cpp - Flyingsand Library
 *  v. 0.1
 */

#include "fs_lib.h"
#include <chrono>
#include <cmath>
#include <climits>
#include <string>


#pragma mark - Memory
// ===================================================================================================

void 
fs_memcpy(const void *src, void *dst, size_t size) {
    fsAssert(src && dst);
    fsAssert(src != dst);
    fsAssert((intptr_t)((fsu8 *)src + size) < (intptr_t)dst || (intptr_t)((fsu8 *)dst + size) < (intptr_t)src); // NOTE(christian): Overlap test.
    memcpy(dst, src, size);
}

void 
fs_memclear(void *ptr, size_t size) {
    fsAssert(ptr);
    memset(ptr, 0, size);
}

bool 
fs_memcmp_equal(const void *ptr1, const void *ptr2, size_t size) {
    fsAssert(ptr1 && ptr2);
    fsAssert(ptr1 != ptr2);
    return (memcmp(ptr1, ptr2, size) == 0);
}


#pragma mark - Math
// ===================================================================================================

bool
fs_are_equal(fsr32 a, fsr32 b, fsr32 epsilon) {
    return (fabsf(a - b) <= (epsilon * fsMax(1.f, fsMax(fabsf(a), fabsf(b)))));
}

bool
fs_are_equal(fsr64 a, fsr64 b, fsr64 epsilon) {
    return (fabs(a - b) <= (epsilon * fsMax(1.0, fsMax(fabs(a), fabs(b)))));
}

bool
fs_are_equal_absolute(fsr32 a, fsr32 b, fsr32 epsilon) {
    return (fabsf(a - b) <= epsilon);
}

bool
fs_are_equal_relative(fsr32 a, fsr32 b, fsr32 epsilon) {
    return (fabsf(a - b) <= (epsilon * fsMax(fabsf(a), fabsf(b))));
}

bool
fs_are_equal_absolute(fsr64 a, fsr64 b, fsr64 epsilon) {
    return (fabs(a - b) <= epsilon);
}

bool
fs_are_equal_relative(fsr64 a, fsr64 b, fsr64 epsilon) {
    return (fabs(a - b) <= (epsilon * fsMax(fabs(a), fabs(b))));
}

fsr32 
fs_random01() {
    return ((fsr32)fs_random_xorshift() / (fsr32)UINT32_MAX);
}

fsu32 
fs_random(fsu32 seed) {
    seed = (seed << 13) ^ seed;
    return ((seed * (seed * seed * 15731 + 789221) + 1376312589) & 0x7fffffff);
}

fsu32
fs_random_xorshift() {
    static uint32_t state = rand();
    uint32_t x = state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    state = x;
    
    return x;
}

fsu32
fs_random_wang_hash(fsu32 seed) {
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed;
}

fsr32 
fs_lerp(fsr32 from, fsr32 t, fsr32 to) {
    return (1.f - t) * from + t * to;
}

fsr32 
fs_map_to_range(fsr32 val, fsr32 fromLow, fsr32 fromHigh, fsr32 toLow, fsr32 toHigh) {
    fsr32 result = (val - fromLow) / (fromHigh - fromLow) * (toHigh - toLow) + toLow;
    return result;
}

fsr32 
fs_map01_to_range(fsr32 val, fsr32 toLow, fsr32 toHigh) {
    fsr32 result = val * (toHigh - toLow) + toLow;
    return result;
}

fsr32
fs_map_to_range01(fsr32 val, fsr32 fromLow, fsr32 fromHigh) {
    fsr32 result = (val - fromLow) / (fromHigh - fromLow);
    return result;
}

fsi32
fs_round_to_zero(fsr32 val) {
    return (fsi32)ceilf(fabsf(val) - 0.5f) * fsSign(val);
}

fsi64
fs_round_to_zero(fsr64 val) {
    return (fsi64)ceil(fabs(val) - 0.5) * fsSign(val);
}

fsi32
fs_round_from_zero(fsr32 val) {
    return (fsi32)floorf(fabsf(val) + 0.5f) * fsSign(val);
}

fsi64
fs_round_from_zero(fsr64 val) {
    return (fsi64)floor(fabs(val) + 0.5) * fsSign(val);
}

fsu32
fs_next_power_of_2(fsu32 val) {
    return (val == 1 ? 1 : (1 << (32 - fs_clz(val - 1))));
}

fsu64
fs_next_power_of_2(fsu64 val) {
    return (val == 1 ? 1 : (1UL << (64UL - fs_clz(val - 1))));
}

fsu16
fs_rotl16(fsu16 val, fsu8 bits) {
    fsAssert(bits < 16);
    if (!bits) return val;
    fsu16 result = (fsu16)((val << bits) | (val >> (16 - bits)));
    return result;
}

fsu32
fs_rotl32(fsu32 val, fsu8 bits) {
    fsAssert(bits < 32);
    if (!bits) return val;
    fsu32 result = (val << bits) | (val >> (32 - bits));
    return result;
}

fsu64
fs_rotl64(fsu64 val, fsu8 bits) {
    fsAssert(bits < 64);
    if (!bits) return val;
    fsu64 result = (val << bits) | (val >> (64 - bits));
    return result;
}

fsu16
fs_rotr16(fsu16 val, fsu8 bits) {
    fsAssert(bits < 16);
    if (!bits) return val;
    fsu16 result = (fsu16)((val >> bits) | (val << (16 - bits)));
    return result;
}

fsu32
fs_rotr32(fsu32 val, fsu8 bits) {
    fsAssert(bits < 32);
    if (!bits) return val;
    fsu32 result = (val >> bits) | (val << (32 - bits));
    return result;
}

fsu64
fs_rotr64(fsu64 val, fsu8 bits) {
    fsAssert(bits < 64);
    if (!bits) return val;
    fsu64 result = (val >> bits) | (val << (64 - bits));
    return result;
}


#pragma mark - Bitwise Operations & I/O
// ===================================================================================================

template<> void
fsBitStream<fsByteOrder::bigEndian>::refill(fsi32 bits) {
    fsAssert(bits >= 0 && bits <= 57);
    while (bitCount < bits) {
        bitBuffer <<= 8;
        bitBuffer |= *stream++;
        bitCount += 8;
    }
}

template<> void
fsBitStream<fsByteOrder::littleEndian>::refill(fsi32 bits) {
    fsAssert(bits >= 0 && bits <= 57);
    while (bitCount < bits) {
        fsu64 byte = *stream++;
        bitBuffer |= (byte << bitCount);
        bitCount += 8;
    }
}

template<> fsu64
fsBitStream<fsByteOrder::bigEndian>::get_bits(fsi32 bits) {
    refill(bits);
    bitCount -= bits;
    fsAssert(bitCount >= 0);
    fsu64 value = bitBuffer >> bitCount;
    value &= (1ULL << bits) - 1;
    
    return value;
}

template<> fsu64
fsBitStream<fsByteOrder::littleEndian>::get_bits(fsi32 bits) {
    refill(bits);
    fsAssert(bitCount >= 0);
    fsu64 value = bitBuffer;
    value &= (1ULL << bits) - 1;
    bitCount -= bits;
    bitBuffer >>= bits;
    
    return value;
}

template<> fsu8
fsBitStream<fsByteOrder::bigEndian>::peek_byte() {
    refill(8);
    fsAssert(bitCount >= 8);
    fsu8 byte = (fsu8)(bitBuffer >> (bitCount - 8)) & 0xFF;
    return byte;
}

template<> fsu8
fsBitStream<fsByteOrder::littleEndian>::peek_byte() {
    refill(8);
    fsAssert(bitCount >= 8);
    fsu8 byte = (fsu8)(bitBuffer & 0xFF);
    return byte;
}

template<> void
fsBitStream<fsByteOrder::bigEndian>::advance_bytes(fsu32 bytes) {
    while (bytes-- > 0) {
        bitCount -= 8;
        if (bitCount < 0) {
            refill(8);
        }
    }
}

template<> void
fsBitStream<fsByteOrder::littleEndian>::advance_bytes(fsu32 bytes) {
    while (bytes-- > 0) {
        bitCount -= 8;
        bitBuffer >>= 8;
        if (bitCount < 0) {
            refill(8);
        }
    }
}

template<> void
fsBitStream<fsByteOrder::bigEndian>::flush_byte() {
    bitCount -= (bitCount % 8);
}

template<> void
fsBitStream<fsByteOrder::littleEndian>::flush_byte() {
    fsu8 bits = bitCount % 8;
    bitCount -= bits;
    bitBuffer >>= bits;
}

// NOTE(christian): These template specializations convert to two's complement signed integer by sign extending if needed.
// TODO(christian): Verify portability of right shift on signed type (compiler dependent?).

template <typename T>
T fs_sign_extend(T val, fsi32 bits) {
    static_assert(std::is_integral<T>::value, "Sign extension allowed on integral types only.");
    fsAssert((size_t)bits <= sizeof(T) * 8);
    size_t shift = (sizeof(T) * 8) - bits;
    val = (T)(val << shift);
    val = val >> shift;
    return val;
}

template<> template<> fsi16
fsBitStream<fsByteOrder::bigEndian>::get(fsi32 bits) {
    fsi16 val = (fsi16)get_bits(bits);
    return fs_sign_extend(val, bits);
}

template<> template<> fsi16
fsBitStream<fsByteOrder::littleEndian>::get(fsi32 bits) {
    fsi16 val = (fsi16)get_bits(bits);
    return fs_sign_extend(val, bits);
}

template<> template<> fsi32
fsBitStream<fsByteOrder::bigEndian>::get(fsi32 bits) {
    fsi32 val = (fsi32)get_bits(bits);
    return fs_sign_extend(val, bits);
}

template<> template<> fsi32
fsBitStream<fsByteOrder::littleEndian>::get(fsi32 bits) {
    fsi32 val = (fsi32)get_bits(bits);
    return fs_sign_extend(val, bits);
}


template<>
fsu16 fs_byte_swap(fsu16 val) {
    fsu16 result = (fsu16)((val & 0xFF00) >> 8) | (fsu16)((val & 0xFF) << 8);
    return result;
}

template<>
fsu32 fs_byte_swap(fsu32 val) {
    fsu32 result = (val >> 24) | (val << 24) | ((val & 0xFF0000) >> 8) | ((val & 0xFF00) << 8);
    return result;
}

template<>
fsu64 fs_byte_swap(fsu64 val) {
    fsu64 result = (val >> 56) | (val << 56) | ((val & 0xFF000000000000) >> 40) | ((val & 0xFF00) << 40) | ((val & 0xFF0000000000) >> 24) | ((val & 0xFF0000) << 24) | ((val & 0xFF00000000) >> 8) | ((val & 0xFF000000) << 8);
    return result;
}

template<>
fsi16 fs_byte_swap(fsi16 val) {
    return (fsi16)fs_byte_swap((fsu16)val);
}

fsu32
fs_clz(fsu8 byte) {
    fsu32 lzs = 0;
    fsu8 mask = 0x80;
    while (mask > 0x0) {
        if (mask & byte) break;
        mask >>= 1;
        lzs++;
    }
    
    return lzs;
}

fsu32
fs_clz(fsu32 val) {
    if (val == 0) return 32;
#if FS_PLATFORM_OSX
    return __builtin_clz(val);
#elif FS_PLATFORM_WINDOWS
	return __lzcnt(val);
#endif
}

fsu32
fs_clz(fsu64 val) {
    if (val == 0) return 64;
#if FS_PLATFORM_OSX
    return __builtin_clzl(val);
#elif FS_PLATFORM_WINDOWS
#if FS_ENV_64BIT
    return __lzcnt64(val);
#else
	// TODO(christian): Find an intrinsic that works in a 32-bit environment, or something equivalent/more optimal.
	fsu32 lzs = 0;
	fsu64 mask = 0x8000000000000000;
	while (mask > 0x0) {
		if (mask & val) break;
		mask >>= 1;
		lzs++;
	}

	return lzs;
#endif
#endif
}

fsu32
fs_ffs(fsu8 byte) {
    if (byte == 0) return 0;
    
    fsu32 ffs = 1;
    fsu8 mask = 0x1;
    while (mask < 0x80) {
        if (mask & byte) break;
        mask <<= 1;
        ffs++;
    }
    
    return ffs;
}

fsu32
fs_ffs(fsu32 val) {
#if FS_PLATFORM_OSX
    return __builtin_ffs(val);
#elif FS_PLATFORM_WINDOWS
	unsigned long index;
	// NOTE(christian): Advance 1 past the position since intrinsic counts the position of first bit as 0, not 1.
	return _BitScanForward(&index, val) != 0 ? index + 1 : 0;
#endif
}


#pragma mark - File I/O
// ===================================================================================================

size_t
fs_get_file_size(const char *fileName) {
	size_t fileSize = 0;
	FILE *f = fopen(fileName, "rb");
	if (f) {
		if (fseek(f, 0, SEEK_END) == 0) {
			fileSize = ftell(f);
		}

		fclose(f);
	}

	return fileSize;
}

bool
fs_read_file(const char *fileName, void **fileData, size_t *fileSize) {
    return fs_read_file(fileName, fileData, fileSize, [](size_t size, void*){ return malloc(size); }, nullptr);
}

bool
fs_read_file(const char *fileName, void **fileData, size_t *fileSize, void* (*alloc)(size_t,void*), void *userData) {
    FILE *f = fopen(fileName, "rb");
    if (f) {
        if (fseek(f, 0, SEEK_END) == 0) {
            long size = ftell(f);
            if (size != -1) {
				if (fileSize) *fileSize = size;
                rewind(f);
                
				fsAssert(fileData);
				if (fileData) {
					*fileData = alloc(size, userData);
					size_t readSize = fread(*fileData, size, 1, f);
					if ((readSize == 1) && (ferror(f) == 0)) {
						fclose(f);
						return true;
					}
				}
            }
        }
        
        fclose(f);
    }
    
    return false;
}

bool
fs_read_file(const char *fileName, void *buffer, size_t bufferSize) {
	FILE *f = fopen(fileName, "rb");
	if (f) {
		if (fseek(f, 0, SEEK_END) == 0) {
			long fileSize = ftell(f);
			if (fileSize != -1) {
				size_t bytesToRead = fsMin((size_t)fileSize, bufferSize);
				rewind(f);

				fsAssert(buffer);
				if (buffer) {
					size_t readSize = fread(buffer, bytesToRead, 1, f);
					if ((readSize == 1) && (ferror(f) == 0)) {
						fclose(f);
						return true;
					}
				}
			}
		}

		fclose(f);
	}

	return false;
}

char*
fs_read_line(FILE *file) {
    size_t lineLength = 0;
    
    int c = fgetc(file);
    while ((c != '\n') && (c != EOF)) {
        lineLength++;
        c = fgetc(file);
    }
    
    char *lineBuffer = nullptr;
    if (lineLength > 0) {
        lineBuffer = (char *)malloc(sizeof(char) * (lineLength + 1));
        fseek(file, -((long)lineLength + 1), SEEK_CUR);
        fread(lineBuffer, sizeof(char), lineLength, file);
        lineBuffer[lineLength] = '\0';
        fgetc(file); // Consume newline character
    } else {
        // Rewind by the one char that was read before entering the while loop.
        fseek(file, -1, SEEK_CUR);
    }
    
    return lineBuffer;
}

char*
fs_read_line(FILE *file, char *buffer, size_t maxLength) {
    size_t lineLength = 0;
    
    char *at = buffer;
    int c = fgetc(file);
    while ((c != '\n') && (c != EOF) && (lineLength < maxLength)) {
        lineLength++;
        *at++ = (char)c;
        c = fgetc(file);
    }
    
    if ((lineLength > 0) && (lineLength <= maxLength)) {
        *at = '\0';
    } else {
        buffer = nullptr;
    }
    
    return buffer;
}

#pragma mark - Strings
// ===================================================================================================

size_t
fs_strlen(const char *str) {
    size_t length = 0;
    const char *at = str;
    while (at && *at != '\0') {
        at++;
        length++;
    }
    return length;
}

bool
fs_are_equal(const char *str0, const char *str1, size_t length) {
	bool equal = false;
	if (str0 && str1) {
		const char *at = str1;
		for (size_t i = 0; i < length; ++i, ++at) {
			if ((*at == '\0') || (str0[i] != *at)) {
				return false;
			}
		}
		equal = (*at == '\0');
	} else {
		equal = (!str0 && !str1 && (length == 0));
	}

	return equal;
}

bool
fs_is_whitespace(const char c) {
	return ((c == ' ') || (c == '\t') || (c == '\v') || (c == '\f') || fs_is_line_ending(c));
}

bool
fs_is_line_ending(const char c) {
	return ((c == '\n') || (c == '\r'));
}

char**
fs_split(const char *str, char delimeter, size_t *count) {
    fsAssert(count);
    char **result = nullptr;
    
    if (str) {
        const char *start = str;
        const char *at = str;
        *count = 0;
        
        auto addToken = [&](const char *from, const char *to) {
            while (fs_is_whitespace(*from)) from++;
            fsAssert((intptr_t)to >= (intptr_t)from);
            
            result = (char **)realloc(result, sizeof(char*) * (*count + 1));
            size_t length = to - from;
            char *token = (char *)malloc(sizeof(char) * (length + 1));
            strncpy(token, from, length);
            token[length] = '\0';
            
            result[*count] = token;
            *count += 1;
        };
        
        while (*at != '\0') {
            if (*at == delimeter) {
                addToken(start, at);
                start = at + 1;
            }
            
            at++;
        }
        
        if (result) {
            addToken(start, at);
        }
    }
    
    return result;
}

char*
fs_get_last_path_component(const char *path) {
    char pathDelim;
#if FS_PLATFORM_OSX
    pathDelim = '/';
#elif FS_PLATFORM_WINDOWS
    pathDelim = '\\';
#else
#error "Unsupported platform."
#endif
    
    size_t pathLength = fs_strlen(path);
    const char *at = path + pathLength;
    while ((*at != pathDelim) && (at != path)) {
        at--;
    }
    
    if (*at == pathDelim) {
        at++;
    }
    
    // NOTE(christian): + terminating null char.
    size_t length = (size_t)((path + pathLength) - at) + 1;
    char *lastComponent = (char *)calloc(1, length);
    fs_memcpy(at, lastComponent, length);
    lastComponent[length-1] = '\0';
    
    return lastComponent;
}

char*
fs_trim_last_path_component(const char *path) {
    char pathDelim;
#if FS_PLATFORM_OSX
    pathDelim = '/';
#elif FS_PLATFORM_WINDOWS
    pathDelim = '\\';
#else
#error "Unsupported platform."
#endif
    
    size_t pathLength = fs_strlen(path);
    const char *at = path + pathLength;
    while ((*at != pathDelim) && (at != path)) {
        at--;
    }
    
    // NOTE(christian): + terminating null char.
    size_t trimmedLength = (size_t)(at - path) + 1;
    char *trimmedPath = (char *)calloc(1, trimmedLength);
    fs_memcpy(path, trimmedPath, trimmedLength);
    trimmedPath[trimmedLength-1] = '\0';
    
    return trimmedPath;
}

char*
fs_append_path_component(const char *path, const char *component) {
    char pathDelim;
#if FS_PLATFORM_OSX
    pathDelim = '/';
#elif FS_PLATFORM_WINDOWS
    pathDelim = '\\';
#else
#error "Unsupported platform."
#endif
    
    char *newPath = nullptr;
    if (path && component) {
        size_t pathLength = fs_strlen(path);
        size_t componentLength = fs_strlen(component);
        
        if (pathLength > 0 && componentLength > 0) {
            // NOTE(christian): String lengths + terminating null character.
            size_t newLength = pathLength + componentLength + 1;
            bool needsDivider = false;
            if (path[pathLength] != pathDelim) {
                newLength++;
                needsDivider = true;
            }
            
            newPath = (char *)calloc(1, newLength);
            fs_memcpy(path, newPath, pathLength);
            char *at = newPath + pathLength;
            if (needsDivider) {
                *at++ = pathDelim;
            }
            
            fs_memcpy(component, at, componentLength);
            newPath[newLength-1] = '\0';
        }
    }
    
    return newPath;
}

char*
fs_get_file_extension(const char *file) {
    char pathDelim;
#if FS_PLATFORM_OSX
    pathDelim = '/';
#elif FS_PLATFORM_WINDOWS
    pathDelim = '\\';
#else
#error "Unsupported platform."
#endif
    
    size_t length = fs_strlen(file);
    const char *end = file + length;
    const char *at = end;
    while (at != file && *at != '.' && *at != pathDelim) {
        at--;
    }
    
    char *ext = nullptr;
    if (*at == '.') {
        at++; // Skip over '.'
        size_t extLength = (intptr_t)end - (intptr_t)at + 1;
        ext = (char *)malloc(sizeof(char) * extLength);
        strncpy(ext, at, extLength);
        ext[extLength-1] = '\0';
    }
    
    return ext;
}

fsi64
fs_to_integer(const char *string) {
    fsi64 n = 0;
    bool isNeg = false;
    
    const char *at = string;
    if (string[0] == '-') {
        at++;
        isNeg = true;
    }
    
    while (*at != '\0') {
        n = n * 10 + (*at - '0');
        at++;
    }
    
    if (isNeg) {
        n = -n;
    }
    
    return n;
}

char*
fs_to_string(fsi64 num) {
    char buf[21]; // NOTE(christian): Max number of digits for a 64-bit integer (20) + possible sign character (-).
    bool isNeg = false;
    
    if (num == 0) {
        char *str = (char *)malloc(sizeof(char) * 2);
        str[0] = '0';
        str[1] = '\0';
        return str;
    }
    
    size_t length = 0;
    if (num < 0) {
        isNeg = true;
        num = -num;
        length++;
    }
    
    size_t idx = 0;
    while (num > 0) {
        char digit = num % 10;
        buf[idx++] = '0' + digit;
        num /= 10;
        length++;
    }
    
    if (isNeg) {
        buf[idx] = '-';
    }
    
    char *str = (char *)malloc(sizeof(char) * (length + 1));
    
    for (idx = 0; idx < length; ++idx) {
        str[idx] = buf[length-idx-1];
    }
    str[idx] = '\0';
    
    return str;
}

static const char kBase64Alphabet[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'
};
static const char kBase64URLAlphabet[] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_'
};
static const char kBase64PaddingChar = '=';

// NOTE(christian): Based on the Base64 Encoding standard specified in the document https://datatracker.ietf.org/doc/html/rfc4648#ref-5

static char*
base64_encode(fsu8 *data, size_t dataSize, const char *alphabet) {
    size_t length = (size_t)(ceilf(dataSize/3.f) * 4);
    char *str = (char *)malloc(sizeof(char) * (length + 1));
    
    if (str != nullptr) {
        fsBitStream<fsByteOrder::bigEndian> stream(data);
        size_t bitCount = dataSize * 8;
        while (bitCount >= 6) {
            fsu64 index = stream.get_bits(6);
            char ch = alphabet[index];
            *str++ = ch;
            bitCount -= 6;
        }
        
        if (bitCount > 0) {
            fsu64 index = stream.get_bits((fsi32)bitCount);
            index <<= (6 - bitCount);
            *str++ = alphabet[index];
        }
        
        size_t paddingChars = length - (size_t)ceilf(dataSize * 8 / 6.f);
        for (size_t i = 0; i < paddingChars; ++i) {
            *str++ = kBase64PaddingChar;
        }
        
        *str = '\0';
        str -= length;
    }
    
    return str;
}

static char*
base64_encode(const char *string, const char *alphabet) {
    size_t length = fs_strlen(string);
    fsu8 *data = (fsu8 *)malloc(sizeof(fsu8) * length);
    if (data != nullptr) {
        for (size_t i = 0; i < length; ++i) {
            data[i] = string[i];
        }
        char *base64 = base64_encode(data, length, alphabet);
        free(data);
        
        return base64;
    }
    return nullptr;
}

char*
fs_base64_encode(fsu8 *data, size_t dataSize) {
    return base64_encode(data, dataSize, kBase64Alphabet);
}

char*
fs_base64_urlencode(fsu8 *data, size_t dataSize) {
    return base64_encode(data, dataSize, kBase64URLAlphabet);
}

char*
fs_base64_encode(const char *string) {
    return base64_encode(string, kBase64Alphabet);
}

char*
fs_base64_urlencode(const char *string) {
    return base64_encode(string, kBase64URLAlphabet);
}

fsu8*
fs_base64_decode(const char *string) {
    size_t length = fs_strlen(string);
    size_t dataSize = (size_t)floorf(length/4.f * 3);
    
    if (length > 0) {
        const char *at = string + (length - 1);
        if (*at-- == kBase64PaddingChar) {
            dataSize--;
        }
        if (*at-- == kBase64PaddingChar) {
            dataSize--;
        }
        if (*at == kBase64PaddingChar) {
            // NOTE(christian): Invalid Base64 string.
            return nullptr;
        }
    }
    
    fsu8 *data = (fsu8 *)malloc(sizeof(fsu8) * dataSize);
    if (data != nullptr) {
        fsu8 *at = data;
        fsu8 *end = data + (dataSize - 1);
        fsu32 sextet = 0;
        
        for (size_t i = 0; i < length; ++i) {
            char ch = string[i];
            fsu8 index;
            if (ch >= 'A' && ch <= 'Z') {
                index = ch - 65;
            } else if (ch >= 'a' && ch <= 'z') {
                index = ch - 65 - 6;
            } else if (ch >= '0' && ch <= '9') {
                index = ch + 4;
            } else if (ch == '+' || ch == '-') {
                index = 62;
            } else if (ch == '/' || ch == '_') {
                index = 63;
            } else {
                // NOTE(christian): Invalid Base64 string.
                free(data);
                return nullptr;
            }
            
            if (sextet == 0) {
                *at = (fsu8)(index << 2);
                sextet++;
            } else if (sextet == 1) {
                *at++ |= ((index >> 4) & 0xFF);
                if (at > end) {
                    break;
                }
                *at = (fsu8)(index << 4);
                sextet++;
            } else if (sextet == 2) {
                *at++ |= ((index >> 2) & 0xFFFF);
                if (at > end) {
                    break;
                }
                *at = (fsu8)(index << 6);
                sextet++;
            } else if (sextet == 3) {
                *at++ |= (index & 0x3F);
                sextet = 0;
            }
        }
        
        return data;
    }
    return nullptr;
}


#pragma mark - Timing
// ===================================================================================================

typedef std::chrono::steady_clock::time_point fsTimePoint;
struct fsTimingToken {
    fsTimePoint start;
};

fsTimingToken*
fs_timing_start() {
    auto now = std::chrono::steady_clock::now();
    fsTimingToken *token = new fsTimingToken;
    token->start = now;
    return token;
}

fsr64
fs_timing_stop(fsTimingToken *token) {
    fsTimePoint end = std::chrono::steady_clock::now();
    std::chrono::duration<fsr64, std::milli> diff = end - token->start;
    delete token;
    return diff.count();
}
