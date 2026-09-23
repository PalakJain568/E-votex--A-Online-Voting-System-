#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <string>
using namespace std;

class Candidate
{
private:
    string id;
    string name;
    string party;
    int voteCount;

public:
    Candidate();

   Candidate(string id, string name, string party,
          int voteCount = 0);

    string getId();
    string getName();
    string getParty();
    int getVoteCount();

    void addVote();
};

#endif