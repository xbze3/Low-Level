int xb_strlen(const char *str)
{
    int count = 0;

    while (*str)
    {
        count++;
        str++;
    }

    return count;
}