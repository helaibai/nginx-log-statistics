#include<stdio.h>

#define LABEL_SIZE 128
#define VALUE_SIZE 128

typedef struct opt{
	char label[LABEL_SIZE];
	char value[VALUE_SIZE];
	int valuelen;
}opt_t;

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
	int i = 0;
	for(i=0; i<n; i++)
	{
		printf("%s\t\n",opts[i].label);
	}
	printf("\n");
	return 0;
}
