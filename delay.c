#include "delay.h"
#include "common.h"

void delay_ms(uint k)
{
	uint i,j;
	for(i=k;i>0;i--)
		for(j=500;j>0;j--);//110
}

void delay(uint z)
{
	uint x, y;
	for (x = z; x > 0; x--)
		for(y = 110; y > 0; y--);
}
