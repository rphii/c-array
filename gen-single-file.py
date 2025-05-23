
single = []

with open('src/array.h') as header:
    single.append(header.read())

with open('src/array.c') as source:
    single.append('#ifdef ARRAY_IMPLEMENTATION')
    single.append(''.join(source.readlines()[1:]))     # skip #include "vec.h"
    single.append('#endif')

single = '\n'.join(single)
with open('array.h', 'w') as dest:
    dest.write(single)

