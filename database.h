#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include "Voter.h"
#include "candidate.h"

using namespace std;

class Database
{
public:
    static void saveVoters(vector<Voter> voters);
    static vector<Voter> loadVoters();

    static void saveCandidates(vector<Candidate> candidates);
    static vector<Candidate> loadCandidates();

    static void saveResults(vector<Candidate> candidates);
};

#endif