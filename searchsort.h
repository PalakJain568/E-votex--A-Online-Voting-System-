#ifndef SEARCH_SORT_H
#define SEARCH_SORT_H

#include <vector>
#include "Voter.h"
#include "Candidate.h"

using namespace std;

class SearchSort
{
public:
    static int binarySearchVoter(
        vector<Voter> voters,
        string voterId
    );

    static void mergeSort(
        vector<Candidate>& candidates,
        int left,
        int right
    );
};

#endif