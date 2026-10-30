#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

struct CandidateRecord {
    string id, name, party;
    int votes = 0;
};

int main() {
    const string votersPath = "data/voters.txt";
    const string candidatesPath = "data/candidates.txt";
    const string resultsPath = "data/results.txt";
    const string receiptsPath = "data/receipts.txt";

    cout << "========================================\n";
    cout << " EVOTEX READ-ONLY DATA RECONCILIATION\n";
    cout << "========================================\n";
    cout << "This tool only reads files. It does NOT modify any records.\n\n";

    // Read voters
    ifstream votersFile(votersPath);
    if (!votersFile.is_open()) {
        cerr << "ERROR: Cannot open " << votersPath << '\n';
        return 1;
    }

    int voterRecords = 0;
    int votedVoters = 0;
    int malformedVoters = 0;
    unordered_set<string> voterIds;
    string line;

    while (getline(votersFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name, password, voted;

        if (!getline(ss, id, '|') ||
            !getline(ss, name, '|') ||
            !getline(ss, password, '|') ||
            !getline(ss, voted, '|') ||
            (voted != "0" && voted != "1")) {
            malformedVoters++;
            continue;
        }

        voterRecords++;
        if (!voterIds.insert(id).second) {
            cout << "WARNING: Duplicate voter ID found: " << id << '\n';
        }
        if (voted == "1") votedVoters++;
    }

    // Read candidates
    ifstream candidatesFile(candidatesPath);
    if (!candidatesFile.is_open()) {
        cerr << "ERROR: Cannot open " << candidatesPath << '\n';
        return 1;
    }

    vector<CandidateRecord> candidates;
    unordered_map<string, int> candidateVotesById;
    int malformedCandidates = 0;
    int totalCandidateVotes = 0;

    while (getline(candidatesFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name, party, votes;

        if (!getline(ss, id, '|') ||
            !getline(ss, name, '|') ||
            !getline(ss, party, '|') ||
            !getline(ss, votes, '|')) {
            malformedCandidates++;
            continue;
        }

        try {
            size_t used = 0;
            int count = stoi(votes, &used);
            if (used != votes.size() || count < 0) {
                malformedCandidates++;
                continue;
            }

            candidates.push_back({id, name, party, count});
            if (candidateVotesById.count(id)) {
                cout << "WARNING: Duplicate candidate ID found: " << id << '\n';
            }
            candidateVotesById[id] = count;
            totalCandidateVotes += count;
        } catch (...) {
            malformedCandidates++;
        }
    }

    // Read results snapshot and compare by candidate ID
    ifstream resultsFile(resultsPath);
    bool resultsAvailable = resultsFile.is_open();
    int resultsRecords = 0;
    int malformedResults = 0;
    int totalResultVotes = 0;
    unordered_map<string, int> resultVotesById;

    if (resultsAvailable) {
        while (getline(resultsFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string id, name, party, votes;

            if (!getline(ss, id, '|') ||
                !getline(ss, name, '|') ||
                !getline(ss, party, '|') ||
                !getline(ss, votes, '|')) {
                malformedResults++;
                continue;
            }

            try {
                size_t used = 0;
                int count = stoi(votes, &used);
                if (used != votes.size() || count < 0) {
                    malformedResults++;
                    continue;
                }
                resultsRecords++;
                resultVotesById[id] = count;
                totalResultVotes += count;
            } catch (...) {
                malformedResults++;
            }
        }
    }

    // Count receipt lines; a receipt count is a clue, not proof of valid votes.
    ifstream receiptsFile(receiptsPath);
    bool receiptsAvailable = receiptsFile.is_open();
    int receiptRecords = 0;
    int malformedReceipts = 0;
    unordered_set<string> receiptIds;

    if (receiptsAvailable) {
        while (getline(receiptsFile, line)) {
            if (line.empty()) continue;
            size_t separator = line.find('|');
            if (separator == string::npos || separator == 0) {
                malformedReceipts++;
                continue;
            }
            string receiptId = line.substr(0, separator);
            receiptRecords++;
            if (!receiptIds.insert(receiptId).second) {
                cout << "WARNING: Duplicate receipt ID found: " << receiptId << '\n';
            }
        }
    }

    cout << "\n--- SUMMARY ---\n";
    cout << "Voter records parsed:       " << voterRecords << '\n';
    cout << "Voters marked as voted:     " << votedVoters << '\n';
    cout << "Candidate records parsed:   " << candidates.size() << '\n';
    cout << "Total candidate votes:      " << totalCandidateVotes << '\n';

    if (resultsAvailable) {
        cout << "Results records parsed:     " << resultsRecords << '\n';
        cout << "Total votes in results.txt: " << totalResultVotes << '\n';
    } else {
        cout << "results.txt:                NOT FOUND / UNREADABLE\n";
    }

    if (receiptsAvailable) {
        cout << "Receipt records parsed:     " << receiptRecords << '\n';
    } else {
        cout << "receipts.txt:               NOT FOUND / UNREADABLE\n";
    }

    cout << "\n--- CHECKS ---\n";
    bool anyWarning = false;

    if (votedVoters != totalCandidateVotes) {
        cout << "MISMATCH: voted-voter count (" << votedVoters
             << ") differs from candidate vote total ("
             << totalCandidateVotes << ").\n";
        anyWarning = true;
    } else {
        cout << "OK: voted-voter count matches candidate vote total.\n";
    }

    if (resultsAvailable) {
        if (totalResultVotes != totalCandidateVotes) {
            cout << "MISMATCH: results.txt total (" << totalResultVotes
                 << ") differs from candidates.txt total ("
                 << totalCandidateVotes << ").\n";
            anyWarning = true;
        } else {
            cout << "OK: results.txt total matches candidates.txt.\n";
        }

        for (const auto& candidate : candidates) {
            auto it = resultVotesById.find(candidate.id);
            if (it == resultVotesById.end()) {
                cout << "MISMATCH: Candidate " << candidate.id
                     << " is missing from results.txt.\n";
                anyWarning = true;
            } else if (it->second != candidate.votes) {
                cout << "MISMATCH: Candidate " << candidate.id
                     << " has " << candidate.votes
                     << " in candidates.txt but " << it->second
                     << " in results.txt.\n";
                anyWarning = true;
            }
        }

        for (const auto& result : resultVotesById) {
            if (!candidateVotesById.count(result.first)) {
                cout << "MISMATCH: Unknown candidate " << result.first
                     << " appears in results.txt.\n";
                anyWarning = true;
            }
        }
    }

    if (receiptsAvailable && receiptRecords != totalCandidateVotes) {
        cout << "NOTE: Receipt count (" << receiptRecords
             << ") differs from candidate vote total (" << totalCandidateVotes
             << "). This is a discrepancy to investigate, not proof of which record is wrong.\n";
        anyWarning = true;
    }

    if (malformedVoters || malformedCandidates || malformedResults || malformedReceipts) {
        cout << "WARNING: Malformed lines found — voters: " << malformedVoters
             << ", candidates: " << malformedCandidates
             << ", results: " << malformedResults
             << ", receipts: " << malformedReceipts << ".\n";
        anyWarning = true;
    }

    cout << "\nFINAL STATUS: "
         << (anyWarning ? "REVIEW REQUIRED" : "NO COUNT MISMATCH DETECTED")
         << '\n';
    cout << "No files were changed by this tool.\n";
    cout << "========================================\n";

    return anyWarning ? 2 : 0;
}
