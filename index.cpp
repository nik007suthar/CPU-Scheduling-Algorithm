#include <bits/stdc++.h>
#include "parser.h"
using namespace std;

int Arrival_Time(pair<string,pair<int,int>> a){
    return a.second.first;
}

int Burst_Time(pair<string,pair<int,int>> a){
    return a.second.second;
}

string process_Name(pair<string,pair<int,int>> a){
    return a.first;
}

int Priority_Level(pair<string,pair<double,int>> a){
    return a.second.second;
}

bool sort_by_response_ratio(pair<string,pair<double,int>> a,pair<string,pair<double,int>> b){
    return a.second.first>b.second.first;
}

bool sort_by_Priority_Level(pair<int,pair<double,int>> a,pair<int,pair<double,int>> b){
    if(a.first==b.first)    return a.second.second>b.second.second;
    return  a.first>b.first;
}

void First_come_first_serve(){
    int start=Arrival_Time(processes[0]);
    for(int i=0;i<total_processes;i++){
        int Arrival=Arrival_Time(processes[i]);
        int Burst=Burst_Time(processes[i]);

        Completion_Time[i]=start+Burst;
        Turn_Around_Time[i]=Completion_Time[i]-Arrival;
        Norm_Turn[i]=Turn_Around_Time[i]*1.0/Burst;

        for(int j=start;j<Completion_Time[i];j++){
            Timeline[j][i]='1';
        }

        for(int j=Arrival;j<start;j++){
            Timeline[j][i]='0';
        }

        start=start+Burst;
    }
}

void Round_Robbin(int initial_quantum){
    queue<pair<int,int>>q;
    int j=0;
    if(Arrival_Time(processes[j])==0){
        q.push({j,Burst_Time(processes[j])});
        j++;
    }
    int current_quantum=initial_quantum;
    for(int i=0;i<total_time;i++){
        if(!q.empty()){
            int index=q.front().first;
            q.front().second=q.front().second-1;
            int remaining_Burst_time=q.front().second;
            int Arrival=Arrival_Time(processes[index]);
            int Burst=Burst_Time(processes[index]);

            current_quantum--;
            
            Timeline[i][index]='1';
            
            while(j<total_processes && Arrival_Time(processes[j])==i+1){
                q.push({j,Burst_Time(processes[j])});
                j++;
            }

            if(current_quantum==0 && remaining_Burst_time==0){
                Completion_Time[index]=i+1;
                Turn_Around_Time[index]=Completion_Time[index]-Arrival;
                Norm_Turn[index]=(Turn_Around_Time[index]*1.0)/Burst;
                current_quantum=initial_quantum;
                q.pop();
            }
            else if(current_quantum==0 && remaining_Burst_time!=0){
                q.pop();
                q.push({index,remaining_Burst_time});
                current_quantum=initial_quantum;
            }
            else if(current_quantum!=0 && remaining_Burst_time==0){
                Completion_Time[index]=i+1;
                Turn_Around_Time[index]=Completion_Time[index]-Arrival;
                Norm_Turn[index]=(Turn_Around_Time[index]*1.0)/Burst;
                q.pop();
                current_quantum=initial_quantum;
            }
        }
        while(j<total_processes && Arrival_Time(processes[j])==i+1){
            q.push({j,Arrival_Time(processes[j])});
            j++;
        }
    }
}

void Pre_emptive_Shortest_job_first(){
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
    int j=0;
    for(int i=0;i<total_time;i++){
        while(j<total_processes && Arrival_Time(processes[j])<=i){
            pq.push({Burst_Time(processes[j]),j});
            j++;
        }
        if(!pq.empty()){
            int index=pq.top().second;
            int Arrival=Arrival_Time(processes[index]);
            int Burst=Burst_Time(processes[index]);
            pq.pop();
            int x=Arrival;
            while(x<i){
                Timeline[x][index]='0';
                x++;
            }
            x=i;
            while(x<i+Burst){
                Timeline[x][index]='1';
                x++;
            }

            Completion_Time[index]=i+Burst;
            Turn_Around_Time[index]=Completion_Time[index]-Arrival;
            Norm_Turn[index]=Turn_Around_Time[index]*1.0/Burst;
            i=x-1;
        }
    }
}

void Non_pre_emptive_Shortest_job_first(){
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
    int j=0;
    for(int i=0;i<total_time;i++){
        while(j<total_processes && Arrival_Time(processes[j])==i){
            pq.push({Burst_Time(processes[j]),j});
            j++;
        }
        if(!pq.empty()){
            int remaining_Time=pq.top().first;
            int index=pq.top().second;
            pq.pop();
            int Arrival=Arrival_Time(processes[index]);
            int Burst=Burst_Time(processes[index]);
            Timeline[i][index]='1';

            if(remaining_Time==1){
                Completion_Time[index]=i+1;
                Turn_Around_Time[index]=Completion_Time[index]-Arrival;
                Norm_Turn[index]=Turn_Around_Time[index]*1.0/Burst;
            }

            else{
                pq.push({remaining_Time-1,index});
            }
        }
    }
}

void highest_response_ratio(){
    vector<pair<string,pair<double,int>>> running_process;
    int j=0;
    for(int i=0;i<total_time;i++){
        while(j<total_processes && Arrival_Time(processes[j])<=i){
            running_process.push_back({process_Name(processes[j]),{1.0,0}});
            j++;
        }

        for(auto & it:running_process){
            int index=process_index[it.first];
            int Wait=i-Arrival_Time(processes[index]);
            int Burst=Burst_Time(processes[index]);
            it.second.first=(Wait+Burst)*1.0/Burst;
        }

        sort(running_process.begin(),running_process.end(),sort_by_response_ratio);

        if(!running_process.empty()){
            int index=process_index[running_process[0].first];
            while(i<total_time && running_process[0].second.second!=Burst_Time(processes[index])){
                Timeline[i][index]='1';
                i++;
                running_process[0].second.second++;
            }
            i--;

            int Arrival=Arrival_Time(processes[index]);
            int Burst=Burst_Time(processes[index]);
            
            running_process.erase(running_process.begin());
            Completion_Time[index]=i+1;
            Turn_Around_Time[index]=Completion_Time[index]-Arrival;
            Norm_Turn[index]=Turn_Around_Time[index]*1.0/Burst;
        }
    }
}

void Feedback()
{
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq; //pair of priority level and process index
    unordered_map<int,int>remainingServiceTime; //map from process index to the remaining service i
    int j=0;
    if(Arrival_Time(processes[0])==0){
        pq.push({0,j});
        remainingServiceTime[j]=Burst_Time(processes[j]);
        j++;
    }
    for(int i=0;i<total_time;i++){
        if(!pq.empty()){
            int priority=pq.top().first;
            int index=pq.top().second;
            int arrivalTime=Arrival_Time(processes[index]);
            int serviceTime=Burst_Time(processes[index]);
            pq.pop();
            while(j<total_processes && Arrival_Time(processes[j])==i+1){
                    pq.push({0,j});
                    remainingServiceTime[j]=Burst_Time(processes[j]);
                    j++;
            }
            remainingServiceTime[index]--;
            Timeline[i][index]='*';
            if(remainingServiceTime[index]==0){
                Completion_Time[index]=i+1;
                Turn_Around_Time[index]=(Completion_Time[index]-arrivalTime);
                Norm_Turn[index]=(Turn_Around_Time[index]*1.0/serviceTime);
            }
            else{
                if(pq.size()>=1)
                pq.push({priority+1,index});
                else
                pq.push({priority,index});
            }
        }
        while(j<total_processes && Arrival_Time(processes[j])==i+1){
            pq.push({0,j});
            remainingServiceTime[j]=Burst_Time(processes[j]);
            j++;
        }
    }
}

void aging(int initial_quantum){
    vector<pair<int,pair<int,int>>>vec;
    int j=0;
    int index=-1;
    for(int i=0;i<total_time;i++){
        while(j<total_processes && Arrival_Time(processes[j])<=i){
            vec.push_back({Priority_Level(processes[j]),{j,0}});
            j++;
        }

        for(int k=0;k<vec.size();k++){
            if(vec[k].second.first==index){
                vec[k].second.second=0;
                vec[k].first=Priority_Level(processes[index]);
            }
            else{
                vec[k].first++;
                vec[k].second.second++;
            }
        }

        sort(vec.begin(),vec.end(),sort_by_Priority_Level);
        index=vec[0].second.first;
        int current_quantum=initial_quantum;
        while(current_quantum-- && i<total_time){
            Timeline[i][index]='1';
            i++;
        }
        i--;
    }
}

void print_process(){
    cout<<"Process       ";
    for(int i=0;i<total_processes;i++){
        cout<<"|   "<<process_Name(processes[i])<<"    ";
    }
    cout << "|\n";
}

void print_Algorithm_Name(int index){
    int algorithm_ID=algorithm[index].first-'0';
    if(algorithm_ID==2)
    cout<<Name_of_algorithm[algorithm_ID]<<algorithm[index].second<<endl;
    else
    cout<<Name_of_algorithm[algorithm_ID]<<endl;
}

void print_Arrival_Time(){
    cout<<"Arrival       ";
    for(int i=0;i<total_processes;i++){
        cout<<"|"<<setw(4)<<Arrival_Time(processes[i])<<"    ";
    }
    cout<<"|"<<endl;
}

void print_Burst_Time(){
    cout<<"Burst         ";
    for(int i=0;i<total_processes;i++){
        cout<<"|"<<setw(4)<<Burst_Time(processes[i])<<"    ";
    }
    cout<<"|"<<endl;
}

void print_Completion_Time(){
    cout<<"Completion    ";
    for(int i=0;i<total_processes;i++){
        cout<<"|"<<setw(4)<<Completion_Time[i]<<"    ";
    }
    cout<<"|"<<endl;
}

void print_Turnaround_Time(){
    cout<<"Turnaround    ";
    for(int i=0;i<total_processes;i++){
        cout<<"|"<<setw(4)<<Turn_Around_Time[i]<<"    ";
    }
    cout<<"|"<<endl;
}

void print_Normturn(){
    cout<<"NormTurn      ";
    for(int i=0;i<total_processes;i++){
        cout<<"|  "<<setw(4)<<fixed<<setprecision(2)<<Norm_Turn[i]<<"  ";
    }
    cout<<"|"<<endl;
}

void printStats(int index){
    cout<<endl;
    print_Algorithm_Name(index);
    print_process();
    print_Arrival_Time();
    print_Burst_Time();
    print_Completion_Time();
    print_Turnaround_Time();
    print_Normturn();
}

void print_Timeline(int index){
    cout<<endl;
    print_Algorithm_Name(index);
    cout<<"        ";
    for(int i=0;i<=total_time;i++){
        cout<<i%10<<"     ";
    }
    cout<<endl<<endl;
    for(int i=0;i<total_processes;i++){
        cout<<process_Name(processes[i])<<"       |  ";
        for(int j=0;j<total_time;j++){
            cout<<Timeline[j][i]<<"  |  ";
        }
        cout<<endl;
    }
}

void Run_Algorithm(char algorithm_ID,int quantum,string operation){
    if(algorithm_ID=='1'){
        First_come_first_serve();
    }
    if(algorithm_ID=='2'){
        Round_Robbin(quantum);
    }
    if(algorithm_ID=='3'){
        Pre_emptive_Shortest_job_first();
    }
    if(algorithm_ID=='4'){
        Non_pre_emptive_Shortest_job_first();
    }
    if(algorithm_ID=='5'){
        highest_response_ratio();
    }
    if(algorithm_ID=='6'){
        Feedback();
    }
    if(algorithm_ID=='7'){
        aging(quantum);
    }
}

int main(){
    parse();
    for(int i=0;i<algorithm.size();i++){

        char algorithm_ID=algorithm[i].first;
        int quantum=algorithm[i].second;
        
        Run_Algorithm(algorithm_ID,quantum,operation);
        
        if(operation=="trace")
        print_Timeline(i);
        
        if(operation=="stats")
        printStats(i);
        
        cout<<endl;
    }
    return 0;
}
