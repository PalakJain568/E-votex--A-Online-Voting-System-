#include <iostream>

#include "../third_party/httplib.h"
#include Voter.h"
#include "../include/Candidate.h"
#include "../include/Election.h"
#include "../include/Database.h"
#include "../include/SearchSort.h"

using namespace std;
using namespace httplib;

int main()
{
    // Load data
    vector<Voter> voters = Database::loadVoters();
    vector<Candidate> candidates = Database::loadCandidates();

    Election election;

    for (int i = 0; i < voters.size(); i++)
    {
        election.addVoter(voters[i]);
    }

    for (int i = 0; i < candidates.size(); i++)
    {
        election.addCandidate(candidates[i]);
    }

    Server server;

    // CORS
    server.set_pre_routing_handler(
        [](const Request& req, Response& res)
        {
            res.set_header(
                "Access-Control-Allow-Origin",
                "*"
            );

            res.set_header(
                "Access-Control-Allow-Methods",
                "GET, POST, OPTIONS"
            );

            res.set_header(
                "Access-Control-Allow-Headers",
                "Content-Type"
            );

            if (req.method == "OPTIONS")
            {
                res.status = 200;
                return Server::HandlerResponse::Handled;
            }

            return Server::HandlerResponse::Unhandled;
        });


    // HOME
    server.Get("/", [](const Request& req, Response& res)
    {
        res.set_content(
            "EVoteX API is working!",
            "text/plain"
        );
    });


    // LOGIN
    server.Post("/login",
        [&election](const Request& req, Response& res)
        {
            if (!req.has_param("voterId") ||
                !req.has_param("password"))
            {
                res.status = 400;

                res.set_content(
                    "Voter ID and password are required.",
                    "text/plain"
                );

                return;
            }

            string voterId =
                req.get_param_value("voterId");

            string password =
                req.get_param_value("password");

            bool loginSuccessful =
                election.loginVoter(
                    voterId,
                    password
                );

            if (loginSuccessful)
            {
                res.set_content(
                    "Login successful",
                    "text/plain"
                );
            }
            else
            {
                res.status = 401;

                res.set_content(
                    "Invalid Voter ID or password",
                    "text/plain"
                );
            }
        });


    // CANDIDATES
    server.Get("/candidates",
        [&election](const Request& req, Response& res)
        {
            vector<Candidate> candidates =
                election.getCandidates();

            string json = "[";

            for (int i = 0; i < candidates.size(); i++)
            {
                json += "{";

                json += "\"id\":\"";
                json += candidates[i].getId();

                json += "\",\"name\":\"";
                json += candidates[i].getName();

                json += "\",\"party\":\"";
                json += candidates[i].getParty();

                json += "\"}";

                if (i < candidates.size() - 1)
                {
                    json += ",";
                }
            }

            json += "]";

            res.set_content(
                json,
                "application/json"
            );
        });


    // CAST VOTE
    server.Post("/vote",
        [&election](const Request& req, Response& res)
        {
            if (!req.has_param("voterId") ||
                !req.has_param("candidateId"))
            {
                res.status = 400;

                res.set_content(
                    "Voter ID and Candidate ID are required.",
                    "text/plain"
                );

                return;
            }

            string voterId =
                req.get_param_value("voterId");

            string candidateId =
                req.get_param_value("candidateId");

            bool voteSuccessful =
                election.castVote(
                    voterId,
                    candidateId
                );

            if (voteSuccessful)
            {
                Database::saveVoters(
                    election.getVoters()
                );

                Database::saveCandidates(
                    election.getCandidates()
                );

                Database::saveResults(
                    election.getCandidates()
                );

                res.set_content(
                    "Vote cast successfully!",
                    "text/plain"
                );
            }
            else
            {
                res.status = 400;

                res.set_content(
                    "Vote could not be cast.",
                    "text/plain"
                );
            }
        });


    // RESULTS
    server.Get("/results",
        [&election](const Request& req, Response& res)
        {
            vector<Candidate> candidates =
                election.getCandidates();

            // Sort candidates by vote count
            if (candidates.size() > 0)
            {
                SearchSort::mergeSort(
                    candidates,
                    0,
                    candidates.size() - 1
                );
            }

            string json = "[";

            for (int i = 0; i < candidates.size(); i++)
            {
                json += "{";

                json += "\"id\":\"";
                json += candidates[i].getId();

                json += "\",\"name\":\"";
                json += candidates[i].getName();

                json += "\",\"party\":\"";
                json += candidates[i].getParty();

                json += "\",\"votes\":";
                json += to_string(
                    candidates[i].getVoteCount()
                );

                json += "}";

                if (i < candidates.size() - 1)
                {
                    json += ",";
                }
            }

            json += "]";

            res.set_content(
                json,
                "application/json"
            );
        });


    cout << "===== EVOTEX SERVER =====" << endl;

    cout << "Voters loaded: "
         << voters.size()
         << endl;

    cout << "Candidates loaded: "
         << candidates.size()
         << endl;

    cout << "Server running at:" << endl;

    cout << "http://localhost:8080"
         << endl;


    server.listen(
        "localhost",
        8080
    );

    return 0;
}