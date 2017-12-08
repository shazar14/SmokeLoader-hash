#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>

typedef unsigned char   uint8;
#define _BYTE  uint8
#define BYTEn(x, n)   (*((_BYTE*)&(x)+n))
#define BYTE1(x)   BYTEn(x,  1)         // byte 1 (counting from 0)
#define LOBYTE(x)   (*((_BYTE*)&(x)))   // low byte


DWORD result = 0;

#define __ROL__(x, y) _rotl(x, y)
inline unsigned int __ROL4__(unsigned int value, int count) { return __ROL__((unsigned int)value, count); }


int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf("Usage: %s file", argv[0]);
		exit(-1);
	}
	char input[255];
	FILE *fp_in, *fp_out;

	fopen_s(&fp_out, "output.txt", "w");
	fopen_s(&fp_in, argv[1], "r");
	if (fp_in == NULL || fp_out == NULL)
	{
		printf("Failed to open file");
		exit(-2);
	}

	while (1)
	{
		result = 0;
		if (fgets(input, 255, fp_in) == NULL)
			break;

		char *pos;
		if ((pos = strchr(input, '\n')) != NULL)
			*pos = '\0';

		for (int i = 0; i < strlen(input); i++)
		{	
			BYTE1(result) ^= input[i] & 0xDF;
			result = __ROL4__(result, 8);
			LOBYTE(result) = BYTE1(result) ^ result;

			BYTE1(result) ^= 0 & 0xDF;
			result = __ROL4__(result, 8);
			LOBYTE(result) = BYTE1(result) ^ result;
		}

		char *output = NULL;

		output = (char *)malloc(5000 * sizeof(char));
		if (output == NULL)
		{
			printf("Failed to allocate space");
			exit(-3);
		}

		sprintf(output, "%s\t0x%X\n", input, result);
		fputs(output, fp_out);

		output = NULL;
		free(output);
		input[0] = '\0';
	}
	
	fclose(fp_out);
}