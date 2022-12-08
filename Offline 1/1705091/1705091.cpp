#include<bits/stdc++.h>

#define Q_limit 100
#define BUSY 1
#define IDLE 0
using namespace std;

int next_event_type, num_custs_delayed, num_delays_required, num_events, num_in_q, server_status;
float area_num_in_q, area_server_status, mean_interarrival, mean_service, sim_time, time_arrival[Q_limit + 1], time_last_event, time_next_event[3], total_of_delays;
ifstream infile;
ofstream outfile;

float expon(float mean)
{
    return -mean * logf(1.0 - (float)rand() / RAND_MAX);
}

void initialize(void)
{
    sim_time = 0.0;
    server_status = IDLE;
    num_in_q = 0;
    time_last_event = 0.0;
    num_custs_delayed = 0;
    total_of_delays = 0.0;
    area_num_in_q = 0.0;
    area_server_status = 0.0;
    time_next_event[1] = sim_time + expon(mean_interarrival);
    time_next_event[2] = 1.0e+30;
}

void timing()
{
    int i;
    float min_time_next_event = 1.0e+29;
    next_event_type = 0;

    
    for (i = 1; i <= num_events; ++i)
        if (time_next_event[i] < min_time_next_event) {
            min_time_next_event = time_next_event[i];
            next_event_type = i;
        }

    
    if (next_event_type == 0) {
        outfile << "\nEvent list empty at time" << sim_time;
        exit(1);
    }

    
    sim_time = min_time_next_event;
}

void update_time_avg_stats()
{
    float time_since_last_event;
    time_since_last_event = sim_time - time_last_event;
    time_last_event = sim_time;
    area_num_in_q += num_in_q * time_since_last_event;
    area_server_status += server_status * time_since_last_event;
}

void arrive(){
    float delay;
    time_next_event[1] = sim_time + expon(mean_interarrival);
    if(server_status == BUSY){
        num_in_q++;
        time_arrival[num_in_q] = sim_time;
    }
    else{
        delay = 0.0;
        total_of_delays += delay;
        num_custs_delayed++;
        server_status = BUSY;
        time_next_event[2] = sim_time + expon(mean_service);
    }
}

void depart(){
    int i;
    float delay;
    if(num_in_q == 0){
        server_status = IDLE;
        time_next_event[2] = 1.0e+30;
    }
    else{
        num_in_q--;
        delay = sim_time - time_arrival[1];
        total_of_delays += delay;
        num_custs_delayed++;
        time_next_event[2] = sim_time + expon(mean_service);
        for(i = 1; i <= num_in_q; i++){
            time_arrival[i] = time_arrival[i + 1];
        }
    }
}

void report(){
    outfile << fixed;
    outfile << setprecision(3);
    outfile << "Avergae delay in queue\t\t" << total_of_delays / num_custs_delayed << " minutes\n";
    outfile << "Average number in queue\t\t" << area_num_in_q / sim_time << endl;
    outfile << "Server utilization\t\t\t" << area_server_status / sim_time << endl;
    outfile << "Time simulation ended\t\t" << sim_time << " minutes\n";
}

int main(){
    
    infile.open("input.txt");
    outfile.open("output.txt");

    num_events = 2;

    infile >> mean_interarrival >> mean_service >> num_delays_required;

    outfile << fixed;
    outfile << setprecision(3);
    outfile << "Single-server queueing system\n\n";
    outfile << "Mean interarrival time\t\t" << mean_interarrival << " minutes\n\n";
    outfile << "Mean service time\t\t\t" << mean_service << " minutes\n\n";
    outfile << "Number of customers\t\t\t" << num_delays_required << "\n\n";

    
    initialize();

    while(num_custs_delayed < num_delays_required){
        timing();

        update_time_avg_stats();

        switch(next_event_type){
            case 1:
                arrive();
                break;
            case 2:
                depart();
                break;
        }
    }

    report();

    infile.close();
    outfile.close();

    return 0;
}