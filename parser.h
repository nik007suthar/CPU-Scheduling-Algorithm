#include <bits/stdc++.h>
using namespace std;


string operation;           //      trace or ststs
int total_time;             //      total time
int total_processes;          //      total number of processes
vector<pair<char,int>>algorithm;
vector<pair<string,pair<int,int>>>processes;
map<string,int>process_index;

//      These four vectors store results
vector<int>Completion_Time;
vector<int>Turn_Around_Time;
vector<float>Norm_Turn;
vector<vector<char>>Timeline;

vector<string> Name_of_algorithm={"",
                                  "First come first serve",
                                  "Round Robbin having time quantum ",
                                  "Pre-emptive Shortest job first",
                                  "Non-Pre-emptive Shortest job first",
                                  "highest response ratio",
                                  "Feedback",
                                  "Ageing"};

void parse_algorithms(string data)
{
    stringstream stream(data);
    while (stream.good())
    {
        string temp;
        getline(stream,temp,',');
        stringstream s(temp);
        getline(s,temp,'-');
        char algorithm_ID=temp[0];
        getline(s,temp,'-');
        int quantum=temp.size()>=1?stoi(temp):-1;
        algorithm.push_back({algorithm_ID,quantum});
    }
}

void parse_processes()
{
    string process_chunk,process_name;
    int Arrival,Burst;
    for(int i=0;i<total_processes;i++){
        cin>>process_chunk;

        stringstream stream(process_chunk);
        string temp;
        getline(stream,temp,',');
        process_name=temp;
        getline(stream,temp,',');
        Arrival=stoi(temp);
        getline(stream,temp,',');
        Burst=stoi(temp);

        processes.push_back({process_name,{Arrival,Burst}});
        process_index[process_name]=i;
    }
}

void parse(){
    string data;
    cin>>operation;
    cin>>data;
    cin>>total_time;
    cin>>total_processes;
    parse_algorithms(data);
    parse_processes();
    Completion_Time.resize(total_processes);
    Turn_Around_Time.resize(total_processes);
    Norm_Turn.resize(total_processes);
    Timeline.resize(total_time);
    for(int i=0;i<total_time;i++){
        for(int j=0;j<total_processes;j++){
            Timeline[i].push_back(' ');
        }
    }
}