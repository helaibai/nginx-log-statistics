#include<stdio.h>

#define LABEL_SIZE 128
#define VALUE_SIZE 128

typedef struct opt{
	char label[LABEL_SIZE];
	char value[VALUE_SIZE];
	int  valuelen;
}opt_t;

int getconfig(const char *file, opt_t *opt_p, int opt_n)
{
	int i = 0;
	for(i=0; i<opt_n; i++)
	{
		printf("%s\t\n",opt_p[i].label);
	}
	return 0;
}



int main(int argc, char **argv)
{
	opt_t opts[5]={
		{"hello",	"",0},
		{"welcome",	"",0},
		{"to",		"",0},
		{"home",	"",0},
		{"shift",	"",0}
	};
	int n = sizeof(opts)/sizeof(opt_t);
	getconfig(NULL, opts, n);
	return 0;
}
