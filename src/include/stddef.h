#ifndef STDDEF_H
#define STDDEF_H

/* size_t: Bellek boyutlarını temsil eder */
typedef unsigned long size_t;

/* ptrdiff_t: Pointer farklarını temsil eder */
typedef long ptrdiff_t;

/* NULL: Null pointer */
#define NULL ((void *)0)

/* offsetof: Yapıdaki bir üyenin offset'ini hesaplamak için */
#define offsetof(type, member) ((size_t)&((type *)0)->member)

#endif /* STDDEF_H */
