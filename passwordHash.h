#ifndef PASSWORD_HASH_H
#define PASSWORD_HASH_H

#include <string>
#include <functional>

using namespace std;

class PasswordHash
{
public:
    static string hashPassword(string password)
    {
        hash<string> hasher;

        size_t hashValue = hasher(password);

        return to_string(hashValue);
    }
};

#endif