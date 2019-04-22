#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "minut.h"

bool cmp(const char *str_a, const char *str_b)
{
    return !strcmp(str_a, str_b);
}

char *to_lowercase(char * const str)
{
    for (char *i = str; *i; i++) {
        *i = tolower(*i);
    }

    return str;
}

int main(void)
{
    MINUT(true);

    return 0;
}

TEST(to_lowercase_all_lowercase)
{
    char test[] = "yes";
    ASSERT_EQ_CMP(cmp,
        "yes",
        to_lowercase(test));
}

TEST(to_lowercase_all_uppercase)
{
    char test[] = "YES";
    ASSERT_EQ_CMP(cmp,
        "yes",
        to_lowercase(test));
}

TEST(to_lowercase_mixed_case)
{
    char test[] = "YeSsSs";
    ASSERT_EQ_CMP(cmp,
        "yessss",
        to_lowercase(test));
}

MINUT_BEG
    RUN(to_lowercase_all_lowercase());
    RUN(to_lowercase_all_uppercase());
    RUN(to_lowercase_mixed_case());
MINUT_END
