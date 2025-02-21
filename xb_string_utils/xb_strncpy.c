char *xb_strncpy(const char *src, char *dest, int n)
{
    char *start = dest;

    for (int i = 0; i < n; i++)
    {
        if (*src)
        {
            *dest = *src;
            src++;
        }
        else
        {
            *dest = '\0';
        }

        dest++;
    }

    return start;
}