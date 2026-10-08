#include "../include/Voter.h"

Voter::Voter()
{
    id = "";
    name = "";
    password = "";
    hasVoted = false;
}

Voter::Voter(string id, string name, string password,
             bool hasVoted, bool passwordIsHashed)
{
    this->id = id;
    this->name = name;

    if (passwordIsHashed)
    {
        this->password = password;
    }
    else
    {
        this->password = PasswordHash::hashPassword(password);
    }

    this->hasVoted = hasVoted;
}

string Voter::getId()
{
    return id;
}

string Voter::getName()
{
    return name;
}

bool Voter::getHasVoted()
{
    return hasVoted;
}

bool Voter::checkPassword(string password)
{
    return this->password ==
           PasswordHash::hashPassword(password);
}

void Voter::setVoted()
{
    hasVoted = true;
}

string Voter::getData()
{
    return id + "|" + name + "|H:" + password + "|" +
           (hasVoted ? "1" : "0");
}
