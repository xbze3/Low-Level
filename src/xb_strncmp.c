int xb_strncmp(const char *str1, const char *str2, int n)
{
    int count = 0;
    while (*str1 && *str2 && *str1 == *str2 && count < (n - 1))
    {
        str1++;
        str2++;
        count++;
    }

    if (count == n)
        return 0;

    return (unsigned char)*str1 - (unsigned char)*str2;
}