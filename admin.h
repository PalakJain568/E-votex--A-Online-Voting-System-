#ifndef ADMIN_H
#define ADMIN_H

#include <string>
using namespace std;

class Admin
{
private:
    string username;
    string password;

public:
    Admin();

    Admin(string username, string password);

    bool login(string username, string password);

    string getUsername();
};

#endif