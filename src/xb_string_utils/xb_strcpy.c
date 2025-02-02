char *xb_strcpy(const char *src, char *dest)
{
    char *dest_start = dest;

    while (*src)
    {
        *dest = *src;
        dest++;
        src++;
    }

    *dest = '\0';

    return dest_start;
}