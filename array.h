#ifndef ARRAY_SIMPLE_H

#include <stddef.h>

/* debug optimization {{{ */

#if defined(NDEBUG)
#define ARRAY_DEBUG_INFO
#define ARRAY_DEBUG_ARG
#define ARRAY_DEBUG_ARGS
#define ARRAY_DEBUG_DEF
#define ARRAY_DEBUG_DEFS
#define ARRAY_DEBUG_FMT
#else
#define ARRAY_DEBUG_INFO  , __FILE__, __LINE__, __func__
#define ARRAY_DEBUG_ARG   file, line, func
#define ARRAY_DEBUG_ARGS  , ARRAY_DEBUG_ARG
#define ARRAY_DEBUG_DEF   const char *file, const int line, const char *func
#define ARRAY_DEBUG_DEFS  , ARRAY_DEBUG_DEF
#define ARRAY_DEBUG_FMT   "%s:%u:%s() "
#endif

/*}}}*/

/* functions for use {{{ */

#define array_grow(array, capacity)     _array_grow(&array ARRAY_DEBUG_INFO, sizeof(*array), capacity)
#define array_resize(array, length)     _array_resize(&array ARRAY_DEBUG_INFO, sizeof(*array), length)
#define array_copy(array)               _array_copy(array ARRAY_DEBUG_INFO, sizeof(*array))
#define array_push(array, item)         (*(typeof(array))_array_push(&array ARRAY_DEBUG_INFO, sizeof(*array)) = item)
#define array_pop(array, item)          *(typeof(array))_array_pop(array ARRAY_DEBUG_INFO, sizeof(*array))
#define array_at(array, index)          *(typeof(array))_array_addr(array ARRAY_DEBUG_INFO, sizeof(*array), index)
#define array_it(array, index)          (typeof(array))_array_addr(array ARRAY_DEBUG_INFO, sizeof(*array), index)
#define array_len(array)                _array_len(array)
#define array_cap(array)                _array_cap(array)
#define array_clear(array)              _array_clear(array)
#define array_free(array)               _array_free(&array)

/*}}}*/

/* internal functions {{{ */

void _array_grow(void *array ARRAY_DEBUG_DEFS, size_t size, size_t capacity);
void _array_resize(void *array ARRAY_DEBUG_DEFS, size_t size, size_t length);
void *_array_copy(void *array ARRAY_DEBUG_DEFS, size_t size);
void *_array_push(void *array ARRAY_DEBUG_DEFS, size_t size);
void *_array_pop(void *array ARRAY_DEBUG_DEFS, size_t size);
void *_array_addr(const void *array ARRAY_DEBUG_DEFS, size_t size, size_t index);
size_t _array_len(const void *array);
size_t _array_cap(const void *array);
void _array_clear(void *array);
void _array_free(void *array);

/*}}}*/

#define ARRAY_SIMPLE_H
#endif


#ifdef ARRAY_IMPLEMENTATION
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include <stdio.h>

static inline void *array_init(ARRAY_DEBUG_DEF);
static inline void *_array_grow2(void *array ARRAY_DEBUG_DEFS, size_t size, size_t capacity);

#define array_assert_arg(arg)     assert(arg && "null pointer argument!");
#define array_base(array)   ((array) - offsetof(Array, data))

#define array_error(msg, ...)     do { \
        fprintf(stderr, "\n" ARRAY_DEBUG_FMT "array error: " msg "\n" ARRAY_DEBUG_ARGS, ##__VA_ARGS__); \
        exit(1); \
    } while(0)

typedef struct Array {
    size_t length;
    size_t capacity;
    void *data;
} Array;

static inline void *array_init(ARRAY_DEBUG_DEF) {
    Array *v = malloc(sizeof(Array));
    if(!v) {
        array_error("failed creating array");
    }
    memset(v, 0, sizeof(*v));
    return &v->data;
}

static inline void *_array_grow2(void *array ARRAY_DEBUG_DEFS, size_t size, size_t capacity) {
    if(!array) {
        array = array_init(ARRAY_DEBUG_ARG);
    }
    Array *v = array_base(array);
    if(capacity <= v->capacity) return array;
    if(capacity * 2 < capacity) {
        array_error("invalid allocation size: %zu", capacity);
    }
    size_t require = 2;
    while(require < capacity) require *= 2;
    size_t bytes = sizeof(Array) + size * require;
    if((bytes - sizeof(Array)) / size != require) {
        array_error("array member of %zu bytes can't allocate %zu elements", size, require);
    }
    void *temp = realloc(v, bytes);
    if(!temp) {
        array_error("failed allocation of: %zu elements (%zu bytes)", require, bytes);
    }
    v = temp;
    memset((void *)&v->data + size * v->capacity, 0, size * (require - v->capacity));
    v->capacity = require;
    return &v->data;
}

void _array_grow(void *array ARRAY_DEBUG_DEFS, size_t size, size_t capacity) {
    array_assert_arg(array);
    void **p = array;
    *p = _array_grow2(*p ARRAY_DEBUG_ARGS, size, capacity);
}

void _array_resize(void *array ARRAY_DEBUG_DEFS, size_t size, size_t length) {
    array_assert_arg(array);
    void **p = array;
    *p = _array_grow2(*p ARRAY_DEBUG_ARGS, size, length);
    Array *v = array_base(*p);
    v->length = length;
}

void *_array_copy(void *array ARRAY_DEBUG_DEFS, size_t size) {
    if(!array) return 0;
    void *v = array_init(ARRAY_DEBUG_ARG);
    size_t len = array_len(array);
    _array_grow(&v ARRAY_DEBUG_ARGS, size, len);
    memcpy(v, array, size * len);
    return v;
}

void *_array_addr(const void *array ARRAY_DEBUG_DEFS, size_t size, size_t index) {
    array_assert_arg(array);
#if !defined(NDEBUG)
    Array *v = (void *)array_base(array);
    if(!(index < v->length)) {
        array_error("index %zu is out of bounds %zu", index, v->length);
    }
#endif
    return (void *)array + size * index;
}

void *_array_push(void *array ARRAY_DEBUG_DEFS, size_t size) {
    void **p = array; Array *v = *p ? array_base(*p) : 0;
    *p = _array_grow2(*p ARRAY_DEBUG_ARGS, size, v ? v->length + 1 : 1);
    v = array_base(*p);
    size_t index = v->length++;
    return (void *)&v->data + size * index;
}

void *_array_pop(void *array ARRAY_DEBUG_DEFS, size_t size) {
    array_assert_arg(array);
    Array *v = array_base(array);
#if !defined(NDEBUG)
    if(!v->length) {
        array_error("no elements left to pop");
    }
#endif
    size_t index = --v->length;
    return array + size * index;
}

void _array_free(void *array) {
    array_assert_arg(array);
    void **p = array;
    if(!*p) return;
    Array *v = array_base(*p);
    free(v);
    *p = 0;
}

size_t _array_len(const void *array) {
    if(!array) return 0;
    Array *v = (Array *)array_base(array);
    return v->length;
}

size_t _array_cap(const void *array) {
    if(!array) return 0;
    Array *v = (Array *)array_base(array);
    return v->capacity;
}

void _array_clear(void *array) {
    if(!array) return;
    Array *v = array_base(array);
    v->length = 0;
}

#endif