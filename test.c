#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

int flagAC(uint8_t a, uint8_t b)
{
	uint16_t value = a + b;
	return (value > 0xff);
}

void assert_equals(int a, int b)
{
	if (a == b)
		printf("Success\n");
	else
	{
		printf("ERROR");
		exit(EXIT_FAILURE);
	}
}

int main()
{
	
	
	return 0;
}