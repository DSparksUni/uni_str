#ifndef UNI_STR_H_INCLUDED_
#define UNI_STR_H_INCLUDED_
#define UNI_STR_IMPLEMENTATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define nodiscard [[nodiscard]]

#ifndef UNI_SMALL_BUFFER_SIZE
#define UNI_SMALL_BUFFER_SIZE 24
#endif  //UNI_SMALL_BUFFER_SIZE

#ifndef UNI_BLOCK_SIZE
#define UNI_BLOCK_SIZE 32
#endif  //UNI_BLOCK_SIZE

#define UNI_STRING_SPARE_CAPACITY(s) \
    s.small[UNI_SMALL_BUFFER_SIZE - 1] & 0x1f //Bottom five bits

#define UNI_STRING_IS_SMALL(s) \
    ((char*)&s)[UNI_SMALL_BUFFER_SIZE - 1] & 0x80 //Top bit

typedef struct uni_string_t {
#ifdef UNI_NO_SSO
    char* buffer;
    size_t size;
    size_t capacity;
} uni_string;
#else
    union {
        //SSO String
        char small[UNI_SMALL_BUFFER_SIZE];

        //Normal String 
        struct {
            char* buffer;
            size_t size;
            size_t capacity;
        };
    };
} uni_string;
#endif //UNI_NO_SOO

nodiscard uni_string uni_string_new();

nodiscard size_t uni_string_size(uni_string);
nodiscard size_t uni_string_capacity(uni_string);

void uni_string_index(uni_string, size_t);
void uni_string_append(uni_string, const char*);
nodiscard char* uni_string_find(uni_string, const char*);
nodiscard uni_string uni_string_substr(uni_string, size_t, size_t);

void uni_string_destroy(uni_string);

#endif  //UNI_STR_H_INCLUDED_

#ifdef UNI_STR_IMPLEMENTATION
nodiscard uni_string uni_string_new() {
    uni_string str;
    memset(str.small, 0, UNI_SMALL_BUFFER_SIZE);

    //Last byte of small string is spare capacity, which
    //both size and capacity can be deduced from
    str.small[UNI_SMALL_BUFFER_SIZE - 1] = 23;

    //Set the last bit of the string as a flag
    //1: SS0 or 0: Normal
    str.small[UNI_SMALL_BUFFER_SIZE - 1] |= 0x80;

    return str;
}

nodiscard size_t uni_string_size(uni_string str) {
    if(UNI_STRING_IS_SMALL(str))
        return UNI_SMALL_BUFFER_SIZE - 
               UNI_STRING_SPARE_CAPACITY(str);
    else return str.size;
}

nodiscard size_t uni_string_capacity(uni_string str) {
    if(UNI_STRING_IS_SMALL(str))
        return UNI_SMALL_BUFFER_SIZE - 1;
    else return str.capacity;
}

void uni_string_destroy(uni_string str) {
    if(!UNI_STRING_IS_SMALL(str)) free(str.buffer);
}

#endif  //UNI_STR_IMPLEMENTATION