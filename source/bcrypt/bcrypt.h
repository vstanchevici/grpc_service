/* ... */
/*
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 */

#ifndef _BCRYPT_H_
#define _BCRYPT_H_

namespace bcrypt
{
	char* GenerateHash(const char* password, unsigned int rounds);

	bool ValidatePassword(const char* password, const char* hash);
}

/*
// for online validation -> https://bcrypt.online/

std::string password = "top_secret";

std::string hash = bcrypt::GenerateHash(password.c_str());

std::cout << "Hash      : " << hash << std::endl;
std::cout << "Hash Size : " << hash.size() << std::endl;
std::cout << "Validate Passowrd  top_secret: " << bcrypt::ValidatePassword("top_secret", hash.c_str()) << std::endl;
std::cout << "Validate Passowrd  top_ssdsdt: " << bcrypt::ValidatePassword("top_ssdsdt", hash.c_str()) << std::endl;
*/
#endif
