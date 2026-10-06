#include "../include/Candidate.h"

Candidate::Candidate()
{
    id = "";
    name = "";
    party = "";
    voteCount = 0;
}

Candidate::Candidate(string id, string name, string party,
                     int voteCount)
{
    this->id = id;
    this->name = name;
    this->party = party;
    this->voteCount = voteCount;
}
string Candidate::getId()
{
    return id;
}

string Candidate::getName()
{
    return name;
}

string Candidate::getParty()
{
    return party;
}

int Candidate::getVoteCount()
{
    return voteCount;
}

void Candidate::addVote()
{
    voteCount++;
}
