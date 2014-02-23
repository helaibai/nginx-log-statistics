#ifndef __STRNCHAR_H__
#define __STRNCHAR_H__

#define X86_64

#ifdef X86_64

#define HIMAGIC  0x8080808080808080L
#define	LOMAGIC  0x0101010101010101L

#else

#define HIMAGIC  0x80808080L
#define	LOMAGIC  0x01010101L

#endif

const char *strnchar(const char *str, char chr, int len);
const char *find_char_c(const char *str, char c);

#endif
