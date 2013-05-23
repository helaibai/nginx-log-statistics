#include <stdio.h>

#include "strnchar.h"

char *strnchar(const char *str, char chr, int len)
{
	return NULL;
}
char *find_char_c(const char *str, char c)
{
	const char *char_ptr;
	const ulong *longword_ptr;
	ulong find_word;
	register ulong longword, himagic, lomagic;

	for(char_ptr = str;((ulong)char_ptr & (sizeof(ulong)-1)) != 0;++char_ptr)
	{
		if(char_ptr == c)
		{
			return  char_ptr;
		}
	}
	longword_ptr = (ulong *)char_ptr;
	
	himagic = 0x80808080L;
	lomagic = 0x01010101L;

	find_word = c |(c << 8);
	find_word |= (find_word << 16); 

	DBG_INFO("find_word[%0x]\n",find_word);

	while(1)
	{
		longword = *longword_ptr++;
		longword ^= find_word;

		if(((longword - lomagic) & himagic) != 0)
		{
			const char *cp = (const char *)(longword_ptr-1);
			if(cp[0] == c)
				return cp;
			if(cp[1] == c)
				return cp+1;
			if(cp[2] == c)
				return cp+2;
			if(cp[3] == c)
				return cp+3;
		}
	}
}