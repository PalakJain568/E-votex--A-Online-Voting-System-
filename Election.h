#ifndef ELECTION_H
#define ELECTION_H

#include <vector>
#include "Voter.h"
#include "Candidate.h"
#include "SearchSort.h"
using namespace std;

class Election
{
private:
    vector<Voter> voters;
    vector<Candidate> candidates;

public:
    void addVoter(Voter voter);
    void addCandidate(Candidate candidate);

    bool loginVoter(string id, string password);

    bool castVote(string voterId, string candidateId);

    void showCandidates();
    void showResults();

    vector<Voter> getVoters();
    vector<Candidate> getCandidates();
};

#endif
