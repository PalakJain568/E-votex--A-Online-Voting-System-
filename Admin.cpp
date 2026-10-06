#include "../include/Admin.h"

Admin::Admin()
{
    username = "";
    password = "";
}

Admin::Admin(string username, string password)
{
    this->username = username;
    this->password = password;
}

bool Admin::login(string username, string password)
{
    return this->username == username &&
           this->password == password;
}

string Admin::getUsername()
{
    return username;
}
