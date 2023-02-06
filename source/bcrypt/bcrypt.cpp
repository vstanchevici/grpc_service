/* ... */
/*
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 */

#include "bcrypt.h"

namespace bcrypt
{	
	extern "C"
	{
		#include "pwd.h"
	}

	char* GenerateHash(const char* password, unsigned int rounds = 12)
	{
		char* salt = bcrypt_gensalt(rounds);
		return bcrypt(password, salt);
	}

	bool ValidatePassword(const char* password, const char* hash)
	{
		char* new_hash = bcrypt(password, hash);
		return !strcmp(hash, new_hash);
	}
}
