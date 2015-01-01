#ifndef TEST_H
#define TEST_H

#define ALT_CI_SHIFT_L(a,b)         ((a) << (b))
#define ALT_CI_SHIFT_R(a,b)         ((a) >> (b))
#define ALT_CI_ASHIFT_L(a,b)        ((a) << (b))
#define ALT_CI_ASHIFT_R(a,b)        ((a) >> (b))
#define ALT_CI_BUTTERFLY_1(a, b)    (((a) - (b)) >> 1)
#define ALT_CI_BUTTERFLY_2(a, b)    (((a) + (b)) >> 1)

#endif // TEST_H
