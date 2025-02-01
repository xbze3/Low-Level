char *xb_strcat(char *dest, const char *src)
{

    char *dest_start = dest;

    while (*dest)
    {
        dest++;
    }

    while (*src)
    {
        *dest = *src;
        dest++;
        src++;
    }

    *dest = '\0';

    return dest_start;
}