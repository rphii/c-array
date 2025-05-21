# c-vector-simple

Very simple C vector [single file](vector.h) implementation, works for any type.

## Functions

```c
    vec_grow(vec, capacity) /* only reserve memory */
    vec_resize(vec, length) /* only grow and set length */
    vec_push(vec, item)     /* push one item */
    vec_pop(vec, item)      /* pop one item */
    vec_at(vec, index)      /* alternative to vec[index] */
    vec_it(vec, index)      /* alternative to &vec[index] */
    vec_len(vec)            /* query length */
    vec_cap(vec)            /* query capacity */
    vec_clear(vec)          /* set length to zero, keep memory */
    vec_free(vec)           /* free memory */
```

## Example

As seen in [main.c](examples/main.c):

```c
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
```

## Single-Header Generation

I've kept the actual [source](src/vec.c) and [header](src/vec.h) code in the subdirectory `src`.

To create the [single-header file](vector.h), use the [python script](gen-single-file.py).

## See also

I took inspiration from the following:

- <https://github.com/Mashpoe/c-vector>

