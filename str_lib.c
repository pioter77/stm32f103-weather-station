#include "str_lib.h"

void str_empty(char str[])//clearing the string after message handling finished
{
	int i=0;
		while(str[i] != '\0')
	{
		str[i]='\0';
		i++;
	}
}
//get the len of the string
unsigned short str_len(char str[])
{
	unsigned short len =0;
	while(str[len]!='\0')
		len++;
	return len-1;
}

//find matching string inside a string
_Bool str_find(char str_target[],char str_given[])
{
	volatile int tmp=0;
	_Bool match=0;
	unsigned short cnt=0;
	unsigned short cnt2=0;
	while(str_target[0]!=str_given[cnt] && str_given[cnt]!='\0')	
		cnt++;
	if(str_given[cnt]=='\0')	return match;	//came to an end not found even first letter
	while(str_target[cnt2]==str_given[cnt] && str_target[cnt2]!='\0')
	{
		cnt2++;
		cnt++;
	}
	cnt2--;
	tmp=str_len(str_target);
	if(cnt2==str_len(str_target))
		match=1;		//if all letters found form target string
	return match;
}

 //find exact string match , check if the string are identical
_Bool str_findL(char str_target[],char str_given[])
{
	
	if((str_len(str_target)==str_len(str_given)) && str_find(str_target,str_given))
	{
		return 1;
	}
	return 0;
}

//concatenate strings
void str_concat(char str_end[],char str_beg_res[])
{
	//lenghtens the first string by adding second arg to the first one
	unsigned short i=0;
	unsigned short j=0;
	while(str_beg_res[i++]!='\0');
	i--;
	while(str_end[j]!='\0')
	{
		str_beg_res[i]=str_end[j];
		j++;
		i++;
	}
	str_beg_res[i]='\0';
}
//nwm czy bedzie z ujemnymi dzialaac
void int2str(int num, char str_res[])
{
	char lstr[12];
 short cnt=0;
	int div=10;
	 short j=0;
	
	while (num>=div)
	{
		lstr[cnt]=(char)(num%div)+0x30;	//od konca cyfry lecimy od najmnijeszej
		num/=10;
		cnt++;
	}
	lstr[cnt]=(char)num+0x30;	//ostania cyfra (najwieksza pozycja)
	for(j=cnt;j>=0;j--)	str_res[cnt-j]=lstr[j];	//od lkonca naszego stringa wynikowefgo zapisujemy do jego pocztku
	str_res[cnt+1]='\0';
}

//fcn char2int conversion

int char2int(char str[])
{
	int result = 0;
	int stop = 0;
	int Strlen = str_len(str);
	int j = 0;
	char check[1];
	
	for(j=0; j<= Strlen; j++)
	{
		check[0] = str[j];
		if(str_find(check, "0123456789") == 0)
		{
			stop = 1;
			break;
		}
	}
	if(stop == 0)
	{
		for(j=0; j<= Strlen;j++)
		{
			result = result *10 + (str[j] -0x30);
		}
	}
	
	return result;
}