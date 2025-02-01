#ifndef STRING_UTILS_H
#define STRING_UTILS_H

int xb_strlen(const char *str);
char *xb_strcat(char *dest, const char *src);
char *xb_strchr(char ch, const char *src);
char *xb_strcpy(char *dest, const char *src);
int xb_strcmp(const char *str1, const char *str2);
char *xb_strncpy(const char *src, char *dest, int n);
int xb_strncmp(const char *str1, const char *str2, int n);
char *xb_strrchr(const char *src, const char ch);

#endif
