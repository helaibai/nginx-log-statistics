#include <stdio.h>

#include "debug.h"
#include "strnchar.h"
#include "types.h"

/*
* 32bits suport, don't suport 64bits
*/
const char *strnchar(const char *str, char chr, int len)
{
	return NULL;
}
const char *find_char_c(const char *str, char c)
{
	const char *char_ptr;
	const ulong *longword_ptr;
	ulong find_word;
	register ulong longword, himagic, lomagic;
	
	int i = 0;
	for(char_ptr = str;((ulong)char_ptr & (sizeof(ulong)-1)) != 0;++char_ptr)
	{
		if((*char_ptr) == c)
		{
			return  char_ptr;
		}
	}
	longword_ptr = (ulong *)char_ptr;
	
	himagic = HIMAGIC;
	lomagic = LOMAGIC;

	find_word = c |(c << 8);
	find_word |= (find_word << 16); 

#ifdef X86_64
	find_word |= (find_word << 32); 
#endif

	while(1)
	{
		longword = *longword_ptr++;
		longword ^= find_word;

		if(((longword - lomagic) & himagic) != 0)
		{
      
			const char *cp = (const char *)(longword_ptr-1);
			
      //need fixd
         
      //for(i=0; i<(sizeof(char *)); ++i)
			//{
			 //  if(cp[i] == c)
			 //  {
			 //  	return (cp + i);
			 //  }
			}
		}
	}
}
