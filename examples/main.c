#define ARRAY_IMPLEMENTATION
#include "../array.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {

    int *origin = 0; /* initialize to 0! */
    printf("push numbers\n");
    for(size_t i = 0; i < 10; ++i) {
        array_push(origin, rand());
    }
    int *numbers = array_copy(origin);
    printf("length %zu / pointer %p\n", array_len(numbers), numbers);

    array_resize(numbers, 5);
    printf("/* resize back to %zu elements, capacity is still %zu */\n", array_len(numbers), array_cap(numbers));

    printf("/* access by [] */\n");
    for(size_t i = 0; i < array_len(numbers); ++i) {
        printf("%zu: %u\n", i, numbers[i]);
    }

    printf("/* access by _at */\n");
    for(size_t i = 0; i < array_len(numbers); ++i) {
        printf("%zu: %u\n", i, array_at(numbers, i));
    }

    printf("/* access by _it */\n");
    for(size_t i = 0; i < array_len(numbers); ++i) {
        printf("%zu: %p->%u\n", i, array_it(numbers, i), *array_it(numbers, i));
    }

    printf("/* access by pop */\n");
    for(size_t i = 0; array_len(numbers); ++i) {
        printf("%zu: %u\n", i, array_pop(numbers, i));
    }
    printf("length %zu\n", array_len(numbers));

    array_free(numbers); /* free once done */
    array_free(origin);
    printf("freed array, verify null: %p\n", numbers);

    return 0;
}

