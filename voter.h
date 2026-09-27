#ifndef VOTER_H
#define VOTER_H

#include <string>
#include "PasswordHash.h"

using namespace std;

class Voter
{
private:
    string id;
    string name;
    string password;
    bool hasVoted;

public:
    Voter();

    Voter(string id, string name, string password,
          bool hasVoted = false,bool passwordIsHashed=false);

    string getId();
    string getName();
    bool getHasVoted();

    bool checkPassword(string password);

    void setVoted();

    string getData();
};

#endif