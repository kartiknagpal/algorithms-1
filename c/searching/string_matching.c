#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int brute_force(const char *text, const char *pattern)
{
	int i, j;
	int len_text, len_pattern;
	len_text = strlen(text);
	len_pattern = strlen(pattern);
	
	int find_last = len_text - len_pattern + 1;
	for (i = 0 ; i < find_last; i++) {
		for (j = 0; j < len_pattern; j++) {
			if (text[i+j] != pattern[j])
				break;
		}
		if (j == len_pattern)
			return i;
	}
	return -1;
}

/* this variable only use kmp algorithm */
char *Table;
void make_table(const char *pattern, int len_pattern)
{
	int i, j;
	i = 0;
	j = 1;

	Table = (char*)malloc(sizeof(char) * (len_pattern + 1));
	Table[0] = -1;
	while (j < len_pattern) {
		if (pattern[i] == pattern[j]) {
			Table[j] = i;
			i++; j++;
		} else {
			Table[j] = i;
			i = 0; j++;
		}
	}
}
int kmp_matching(const char *text, const char *pattern)
{
	int len_text, len_pattern;
	int text_point, pattern_point;
	int find_last;
	int return_value = -1;
	text_point = pattern_point = 0;

	len_pattern = strlen(pattern);
	len_text = strlen(text);
	find_last = len_text - len_pattern + 1;

	make_table(pattern, len_pattern);
	
	while (text_point < find_last) {
		if (text[text_point + pattern_point] 
			== pattern[pattern_point]) {
			pattern_point++;
			if (pattern_point == len_pattern) {
				return_value = text_point;
				break;
			}
		} else {
			if (pattern_point == 0 || Table[pattern_point] == 0) {
				text_point++;
				pattern_point = 0;
			} else {
				text_point += pattern_point;
				pattern_point = Table[pattern_point];
			}	
		}
	
	}
	free(Table);
	Table=NULL;
	return return_value;
}


/*      
 * Copyright 2001-2010 Georges Menie (www.menie.org)
 * Copyright 2010 Salvatore Sanfilippo (adapted to Redis coding style)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of California, Berkeley nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* CRC16 implementation acording to CCITT standards.
 *
 * Note by @antirez: this is actually the XMODEM CRC 16 algorithm, using the
 * following parameters:
 *
 * Name                       : "XMODEM", also known as "ZMODEM", "CRC-16/ACORN"
 * Width                      : 16 bit
 * Poly                       : 1021 (That is actually x^16 + x^12 + x^5 + 1)
 * Initialization             : 0000
 * Reflect Input byte         : False
 * Reflect Output CRC         : False
 * Xor constant to output CRC : 0000
 * Output for "123456789"     : 31C3
 */

static const uint16_t crc16tab[256] = {
    0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
    0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
    0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
    0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
    0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
    0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
    0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
    0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
    0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
    0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
    0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
    0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
    0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
    0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
    0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
    0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
    0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
    0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
    0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
    0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
    0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
    0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
    0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
    0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
    0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
    0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
    0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
    0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
    0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
    0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
    0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
    0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};
  
uint16_t crc16(const char *buf, int len) {
    int counter;
    uint16_t crc = 0;
    for (counter = 0; counter < len; counter++)
            crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *buf++)&0x00FF];
    return crc;
}
int rabin_karp(const char *text, const char *pattern)
{
	unsigned short hash_text, hash_pattern;
	int len_text, len_pattern;
	int find_last;
	int i;

	len_text = strlen(text);
	len_pattern = strlen(pattern);
	find_last = len_text - len_pattern + 1;

	hash_text = crc16(text, len_pattern);
	hash_pattern = crc16(pattern, len_pattern);

	
	for (i = 0; i < find_last; i++) {
		if (hash_text == hash_pattern)
			return i;
		hash_text = crc16(text + i + 1, len_pattern);	
	}
	return -1;
}


#define MAX_CHAR 122 
int shift_or(const char *text, const char *pattern)
{
	int len_text;
	int len_pattern;
	uint32_t move_bit;
	uint32_t pattern_mask[MAX_CHAR + 1];
	int i;
	int idx_pattern;	
	int final_last;

	len_text = strlen(text);	
	len_pattern = strlen(pattern);

	if (*pattern == '\0') return -1;
	if (len_pattern > 31) return -1;

	//move_bit = ~0;
	move_bit = ~1;
	memset(pattern_mask, 0xFF, sizeof(uint32_t) * (MAX_CHAR + 1));	
	for (i = 0; i < len_pattern; i++) {
		pattern_mask[pattern[i]] &= ~(1UL << i);
	}
/* good */
	idx_pattern = 0;
	final_last = (1UL << len_pattern);
	for (i = 0; text[i] != '\0'; i++) {
		move_bit |= pattern_mask[text[i]];
		move_bit <<= 1;
		if (0 == (move_bit & final_last))
			return i - len_pattern + 1;
	}
/* good END*/

/* another way
	final_last = ~0;
	final_last <<= len_pattern;
	for (i = 0; text[i] != '\0'; i++) {
		if (text[i] == pattern[idx_pattern]) {
			move_bit &= pattern_mask[text[i]];
			idx_pattern++;
		} else {
			move_bit = ~0;
			idx_pattern = 0;
		}			
		if ((move_bit | final_last) == final_last)
			return i - len_pattern + 1;
	}
another way END*/
	return -1;
}

#define ASCII_RANGE 129
#define BUFFER_SIZE 1024
#define MAX(x, y) (x) > (y) ? (x):(y)

void make_badcase(char *text, int len_text, int *bad_case)
{
	int i;
	for (i = 0; i < ASCII_RANGE; i++)
		bad_case[i] = len_text;	
	for (i = 0; i < len_text-1; i++)
		bad_case[text[i]] = len_text - i - 1;
}

void make_suffixes(char *text, int len_text, int *suffixes)
{
	int i;
	int first_end;
	int second_end;
	int suffix_idx;
	int compare_size;
	suffixes[len_text - 1] = len_text;

	second_end = len_text - 1;
	for (compare_size = 0; compare_size < len_text; compare_size++) {
		first_end = compare_size; 
		suffix_idx = 0;
		for (i = 0; i <= compare_size; i++) {
			if (text[first_end - i] != text[second_end - i])
				break;
			suffix_idx += 1;
		}
		suffixes[compare_size] = suffix_idx;
	}
}
int pre_process(char *text, int len_text, int *bad_case, int *good_case, \
		int *suffixes)
{
	int i, j;
	make_badcase(text, len_text, bad_case);
	
	make_suffixes(text, len_text, suffixes);

	for (i = 0; i < len_text; i++)
		good_case[i] = len_text;
	
		
	for (i = len_text - 1; i >= 0; i--) {
		if (suffixes[i] == i + 1)
			good_case[i] = i;
	}

	for (i = 0; i < len_text -1; i++) {
		good_case[len_text-suffixes[i]-1] = len_text - i - 1;
	}


	return 0;
}

int boyer_moore(char *text, int len_text, char *pattern, int len_pattern)
{
	int *good_case, *suffixes;
	int bad_case[ASCII_RANGE];
	int i, j;
	int ret_value = -1;

	good_case = (int*)malloc(sizeof(int) * len_pattern);
	suffixes = (int*)malloc(sizeof(int) * len_pattern);

	pre_process(pattern, len_pattern, bad_case, good_case, suffixes);

	j = 0;
	while (j <= len_text - len_pattern) {
		for (i = len_pattern - 1; i >= 0 && pattern[i] == text[i + j]; i--) ;
		if (i < 0) {
			ret_value = j;
			break;
		}
		else {
			j += MAX(good_case[i],\
				 bad_case[text[j + i]] - (len_pattern - i) - 1 );
		}
			
	}
	
	free(good_case);	
	free(suffixes);	
	return ret_value;
}

int main(int argc, char **argv)
{
	/* ref. http://en.wikipedia.org/wiki/String_searching_algorithm#Na.C3.AFve_string_search */
	char *text = "Naive string search \
The simplest and least efficient way to see where one string occurs inside \
another is to check each place it could be, one by one, to see if it's there. \
So first we see if there's a copy of the needle in the first character of the \
haystack; if not, we look to see if there's a copy of the needle starting \
at the second character of the haystack; if not, we look starting at the third \
character, and so forth. In the normal case, we only have to look at one or two\
 characters for each wrong position to see that it is a wrong position, \
so in the average case, Kang this takes";

	
	char input[BUFFER_SIZE + 2];
	int idx_find;
	int len_pattern, len_text;

	len_text = strlen(text);
	while ((fgets(input, BUFFER_SIZE, stdin)) != NULL) {
		len_pattern = strlen(input);
		if (input[len_pattern - 1] == '\n') {
			input[len_pattern-1] = '\0';
			len_pattern -= 1;
		}

		idx_find = brute_force(text, input);	
		if (idx_find == -1)
			fprintf(stdout, "can't find %s in orginal Text\n", input);
		else
			fprintf(stdout, "brute_force find idx : %d\n", idx_find);
		idx_find = kmp_matching(text, input);	
		if (idx_find == -1)
			fprintf(stdout, "can't find %s in orginal Text\n", input);
		else
			fprintf(stdout, "kmp find idx : %d\n", idx_find);

		idx_find = rabin_karp(text, input);	
		if (idx_find == -1)
			fprintf(stdout, "can't find %s in orginal Text\n", input);
		else
			fprintf(stdout, "rabin_karp find idx : %d\n", idx_find);

		idx_find = shift_or(text, input);	
		if (idx_find == -1)
			fprintf(stdout, "can't find %s in orginal Text\n", input);
		else
			fprintf(stdout, "shift_or find idx : %d\n", idx_find);
		idx_find = boyer_moore(text, len_text, input, len_pattern);	
		if (idx_find == -1)
			fprintf(stdout, "can't find %s in orginal Text\n", input);
		else
			fprintf(stdout, "boyer_moore find idx : %d\n", idx_find);

	} 
	return 0;
}
