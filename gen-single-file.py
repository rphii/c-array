
single = []

with open('src/vec.h') as header:
    single.append(header.read())

with open('src/vec.c') as source:
    single.append('#ifdef VECTOR_IMPLEMENTATION')
    single.append(''.join(source.readlines()[1:]))     # skip #include "vec.h"
    single.append('#endif')

single = '\n'.join(single)
with open('vector.h', 'w') as dest:
    dest.write(single)

