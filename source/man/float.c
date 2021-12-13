#include <stdio.h>
#include <float.h>

int main(void)
{
    printf("FLT_RADIX: %d\n", FLT_RADIX);
    printf("FLT_ROUNDS: %d\n", FLT_ROUNDS);
    printf("FLT_EVAL_METHOD: %d\n", FLT_EVAL_METHOD);
    printf("DECIMAL_DIG: %d\n\n", DECIMAL_DIG);

    printf("FLT_HAS_SUBNORM: %d\n", FLT_HAS_SUBNORM);
    printf("FLT_MANT_DIG: %d\n", FLT_MANT_DIG);
    printf("FLT_DECIMAL_DIG: %d\n", FLT_DECIMAL_DIG);
    printf("FLT_DIG: %d\n", FLT_DIG);
    printf("FLT_MIN_EXP: %d\n", FLT_MIN_EXP);
    printf("FLT_MIN_10_EXP: %d\n", FLT_MIN_10_EXP);
    printf("FLT_MAX_EXP: %d\n", FLT_MAX_EXP);
    printf("FLT_MAX_10_EXP: %d\n", FLT_MAX_10_EXP);
    printf("FLT_MIN: %.*e\n", FLT_DECIMAL_DIG, FLT_MIN);
    printf("FLT_MAX: %.*e\n", FLT_DECIMAL_DIG, FLT_MAX);
    printf("FLT_EPSILON: %.*e\n", FLT_DECIMAL_DIG, FLT_EPSILON);
    printf("FLT_TRUE_MIN: %.*e\n\n", FLT_DECIMAL_DIG, FLT_TRUE_MIN);

    printf("DBL_HAS_SUBNORM: %d\n", DBL_HAS_SUBNORM);
    printf("DBL_MANT_DIG: %d\n", DBL_MANT_DIG);
    printf("DBL_DECIMAL_DIG: %d\n", DBL_DECIMAL_DIG);
    printf("DBL_DIG: %d\n", DBL_DIG);
    printf("DBL_MIN_EXP: %d\n", DBL_MIN_EXP);
    printf("DBL_MIN_10_EXP: %d\n", DBL_MIN_10_EXP);
    printf("DBL_MAX_EXP: %d\n", DBL_MAX_EXP);
    printf("DBL_MAX_10_EXP: %d\n", DBL_MAX_10_EXP);
    printf("DBL_MIN: %.*e\n", DBL_DECIMAL_DIG, DBL_MIN);
    printf("DBL_MAX: %.*e\n", DBL_DECIMAL_DIG, DBL_MAX);
    printf("DBL_EPSILON: %.*e\n", DBL_DECIMAL_DIG, DBL_EPSILON);
    printf("DBL_TRUE_MIN: %.*e\n\n", DBL_DECIMAL_DIG, DBL_TRUE_MIN);

    printf("LDBL_HAS_SUBNORM: %d\n", LDBL_HAS_SUBNORM);
    printf("LDBL_MANT_DIG: %d\n", LDBL_MANT_DIG);
    printf("LDBL_DECIMAL_DIG: %d\n", LDBL_DECIMAL_DIG);
    printf("LDBL_DIG: %d\n", LDBL_DIG);
    printf("LDBL_MIN_EXP: %d\n", LDBL_MIN_EXP);
    printf("LDBL_MIN_10_EXP: %d\n", LDBL_MIN_10_EXP);
    printf("LDBL_MAX_EXP: %d\n", LDBL_MAX_EXP);
    printf("LDBL_MAX_10_EXP: %d\n", LDBL_MAX_10_EXP);
    printf("LDBL_MIN: %.*Le\n", LDBL_DECIMAL_DIG, LDBL_MIN);
    printf("LDBL_MAX: %.*Le\n", LDBL_DECIMAL_DIG, LDBL_MAX);
    printf("LDBL_EPSILON: %.*Le\n", LDBL_DECIMAL_DIG, LDBL_EPSILON);
    printf("LDBL_TRUE_MIN: %.*Le\n\n", LDBL_DECIMAL_DIG, LDBL_TRUE_MIN);
    
    printf("sizeof(float): %zu\n", sizeof(float));
    printf("sizeof(double): %zu\n", sizeof(double));
    printf("sizeof(long double): %zu\n", sizeof(long double));
}
