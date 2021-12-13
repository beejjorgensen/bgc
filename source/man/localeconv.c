#include <stdio.h>
#include <locale.h>
#include <limits.h>  // for CHAR_MAX

void print_grouping(char *mg)
{
    int done = 0;

    while (!done) {
        if (*mg == CHAR_MAX)
            printf("CHAR_MAX ");
        else
            printf("%c ", *mg + '0');
        done = *mg == CHAR_MAX || *mg == 0;
        mg++;
    }
}

int main(void)
{
    setlocale(LC_ALL, "");

    struct lconv *lc = localeconv();

    printf("mon_decimal_point : %s\n", lc->mon_decimal_point);
    printf("mon_thousands_sep : %s\n", lc->mon_thousands_sep);
    printf("mon_grouping      : ");
    print_grouping(lc->mon_grouping);
    printf("\n");
    printf("positive_sign     : %s\n", lc->positive_sign);
    printf("negative_sign     : %s\n", lc->negative_sign);
    printf("currency_symbol   : %s\n", lc->currency_symbol);
    printf("frac_digits       : %c\n", lc->frac_digits);
    printf("p_cs_precedes     : %c\n", lc->p_cs_precedes);
    printf("n_cs_precedes     : %c\n", lc->n_cs_precedes);
    printf("p_sep_by_space    : %c\n", lc->p_sep_by_space);
    printf("n_sep_by_space    : %c\n", lc->n_sep_by_space);
    printf("p_sign_posn       : %c\n", lc->p_sign_posn);
    printf("p_sign_posn       : %c\n", lc->p_sign_posn);
    printf("int_curr_symbol   : %s\n", lc->int_curr_symbol);
    printf("int_frac_digits   : %c\n", lc->int_frac_digits);
    printf("int_p_cs_precedes : %c\n", lc->int_p_cs_precedes);
    printf("int_n_cs_precedes : %c\n", lc->int_n_cs_precedes);
    printf("int_p_sep_by_space: %c\n", lc->int_p_sep_by_space);
    printf("int_n_sep_by_space: %c\n", lc->int_n_sep_by_space);
    printf("int_p_sign_posn   : %c\n", lc->int_p_sign_posn);
    printf("int_n_sign_posn   : %c\n", lc->int_n_sign_posn);
}
