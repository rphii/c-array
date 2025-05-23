# c-array

Very simple C array / array [single file](array.h) implementation, works for any type.

## Functions

```c
    array_grow(array, capacity) /* only reserve memory */
    array_resize(array, length) /* only grow and set length */
    array_copy(array)           /* copies array */
    array_push(array, item)     /* push one item */
    array_pop(array, item)      /* pop one item */
    array_at(array, index)      /* alternative to array[index] */
    array_it(array, index)      /* alternative to &array[index] */
    array_len(array)            /* query length */
    array_cap(array)            /* query capacity */
    array_clear(array)          /* set length to zero, keep memory */
    array_free(array)           /* free memory */
```

## Error Handling

There is no error handling, BUT a sophisticated amount of errors can happen:

- Different kinds of memory allocation / reallocation
- Out of bounds access

If any error occurs, it is one that should never even happen in the first
place, hence I just `exit(1)`. For that, see (Debuggability)[#Debuggability].

## Debuggability

All but the query- and freeing functions handle the `NDEBUG` preprocessor
definition.

If it is not defined, and an error occurs (out of bounds, allocation) a message
will be printed describing the error and from where it originated, in the form
of `file:line-nb:function() array error: message`.

If it is defined, the error from where it originated is left out in any case.
Additionally, the out of bounds checking (present in e.g. `array_at` and
`array_it`) will be disabled.

## Example

As seen in [main.c](examples/main.c):

```c
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
}
```

Compile with `gcc main.c` to get the full debug-experience, as described above.

Compile with `gcc main.c -DNDEBUG` to generate more performant code.


## Single-Header Generation

I've kept the actual [source](src/array.c) and [header](src/array.h) code in the subdirectory `src`.

To create the [single-header file](array.h), use the [python script](gen-single-file.py).

## See also

I took inspiration from the following:

- <https://github.com/Mashpoe/c-vector>

