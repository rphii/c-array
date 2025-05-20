#define VECTOR_IMPLEMENTATION
#include "../vector.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {

    int *numbers = 0; /* initialize to 0! */
    printf("push numbers\n");
    for(size_t i = 0; i < 10; ++i) {
        vec_push(numbers, rand());
    }
    printf("length %zu / pointer %p\n", vec_len(numbers), numbers);

    vec_resize(numbers, 5);
    printf("/* resize back to %zu elements, capacity is still %zu */\n", vec_len(numbers), vec_cap(numbers));

    printf("/* access by [] */\n");
    for(size_t i = 0; i < vec_len(numbers); ++i) {
        printf("%zu: %u\n", i, numbers[i]);
    }

    printf("/* access by _at */\n");
    for(size_t i = 0; i < vec_len(numbers); ++i) {
        printf("%zu: %u\n", i, vec_at(numbers, i));
    }

    printf("/* access by pop */\n");
    for(size_t i = 0; vec_len(numbers); ++i) {
        printf("%zu: %u\n", i, vec_pop(numbers, i));
    }
    printf("length %zu\n", vec_len(numbers));

    vec_free(numbers); /* free once done */
    printf("freed vector, verify null: %p\n", numbers);

    return 0;
}

