/* ... */
/*
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 */

#ifndef _PWD_H_
#define _PWD_H_

#ifdef _WIN32
	#include <stdint.h>
	#include <stdlib.h>
	#include <string.h>
	#include <time.h>

	#define u_int8_t unsigned __int8
	#define u_int16_t unsigned __int16
	#define u_int32_t unsigned __int32
	#define u_int64_t unsigned __int64

	#define WRAP 

	#define	_PASSWORD_LEN		128	/* max length, not counting NUL */

	#define CLOCK_THREAD_CPUTIME_ID 0
	static int clock_gettime(int clock_id, struct timespec* spec)
	{
		clock_t tm = clock();
		spec->tv_sec = tm / CLOCKS_PER_SEC;
		spec->tv_nsec = tm * CLOCKS_PER_SEC;
		return 0;
	};

	static void explicit_bzero(void* buf, size_t len)
	{
		memset(buf, 0, len);
	}

	static void arc4random_buf(void* buf, size_t nbytes)
	{
		static unsigned int current_time = 0;
		if (current_time == 0)
		{
			current_time = time(NULL);
			srand(current_time);
		}

		for (size_t n = 0; n < nbytes; ++n)
			((char*)(buf))[n] = rand() % 256;
	}

	static int timingsafe_bcmp(const void* b1, const void* b2, size_t n)
	{
		const unsigned char* p1 = (unsigned char*)b1, * p2 = (unsigned char*)b2;
		int ret = 0;

		for (; n > 0; n--)
			ret |= *p1++ ^ *p2++;
		return (ret != 0);
	}

	char* bcrypt_gensalt(u_int8_t log_rounds);
	char* bcrypt(const char* pass, const char* salt);
#endif

#endif
