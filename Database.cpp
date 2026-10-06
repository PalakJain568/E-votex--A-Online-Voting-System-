#include "../include/Database.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

void Database::saveVoters(vector<Voter> voters)
{
    ofstream file("data/voters.txt");

    if (!file)
    {
        cout << "Error opening voters file." << endl;
        return;
    }

    for (int i = 0; i < voters.size(); i++)
    {
        file << voters[i].getData() << endl;
    }

    file.close();
}

vector<Voter> Database::loadVoters()
{
    vector<Voter> voters;

    ifstream file("data/voters.txt");

    if (!file)
    {
        return voters;
    }

    string line;

    while (getline(file, line))
    {
        stringstream ss(line);

        string id;
        string name;
        string password;
        string voted;

        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, password, '|');
        getline(ss, voted, '|');

        bool hasVoted = (voted == "1");

        bool passwordIsHashed = false;

        if (password.length() >= 2 &&
            password.substr(0, 2) == "H:")
        {
            password = password.substr(2);
            passwordIsHashed = true;
        }

        Voter voter(
            id,
            name,
            password,
            hasVoted,
            passwordIsHashed
        );

        voters.push_back(voter);
    }

    file.close();

    return voters;
}

void Database::saveCandidates(vector<Candidate> candidates)
{
    ofstream file("data/candidates.txt");

    if (!file)
    {
        cout << "Error opening candidates file." << endl;
        return;
    }

    for (int i = 0; i < candidates.size(); i++)
    {
        file << candidates[i].getId() << "|"
             << candidates[i].getName() << "|"
             << candidates[i].getParty() << "|"
             << candidates[i].getVoteCount()
             << endl;
    }

    file.close();
}

vector<Candidate> Database::loadCandidates()
{
    vector<Candidate> candidates;

    ifstream file("data/candidates.txt");

    if (!file)
    {
        return candidates;
    }

    string line;

    while (getline(file, line))
    {
        stringstream ss(line);

        string id;
        string name;
        string party;
        string votes;

        getline(ss, id, '|');
        getline(ss, name, '|');
        getline(ss, party, '|');
        getline(ss, votes, '|');

        int voteCount = stoi(votes);

        Candidate candidate(
            id,
            name,
            party,
            voteCount
        );

        candidates.push_back(candidate);
    }

    file.close();

    return candidates;
}

void Database::saveResults(vector<Candidate> candidates)
{
    ofstream file("data/results.txt");

    if (!file)
    {
        cout << "Error opening results file." << endl;
        return;
    }

    for (int i = 0; i < candidates.size(); i++)
    {
        file << candidates[i].getId() << "|"
             << candidates[i].getName() << "|"
             << candidates[i].getParty() << "|"
             << candidates[i].getVoteCount()
             << endl;
    }

    file.close();
}
