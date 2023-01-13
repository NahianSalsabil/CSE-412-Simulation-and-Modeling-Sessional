//write code for inventory system here

#include<bits/stdc++.h>
#include "lcgrand.h"
using namespace std;

int amount, bigs, initial_inv_level, inv_level, next_event_type, num_events, num_months, num_values_demand, smalls;
float area_holding, area_shortage, holding_cost, incremental_cost, maxlag, mean_interdemand, minlag, prob_distrib_demand[26], setup_cost,
      shortage_cost, sim_time, time_last_event, time_next_event[5], total_ordering_cost;

ifstream infile;
ofstream outfile;

void initialize(void);
void timing(void);
void order_arrival(void);
void demand(void);
void evaluate(void);
void report(void);
void update_time_avg_stats(void);
float expon(float mean);
float uniform(float min, float max);
int random_integer(float prob_distrib []);

float expon(float mean)
{
    return -mean * log(lcgrand(1));
}


void initialize(void){
    sim_time = 0.0;
    inv_level = initial_inv_level;
    time_last_event = 0.0;
    total_ordering_cost = 0.0;
    area_holding = 0.0;
    area_shortage = 0.0;
    time_next_event[1] = 1.0e+30;
    time_next_event[2] = sim_time + expon(mean_interdemand);
    time_next_event[3] = num_months;
    time_next_event[4] = 0.0;
}

void timing(void)  /* Timing function. */
{
    int   i;
    float min_time_next_event = 1.0e+29;

    next_event_type = 0;

    /* Determine the event type of the next event to occur. */

    for (i = 1; i <= num_events; ++i)
        if (time_next_event[i] < min_time_next_event) {
            min_time_next_event = time_next_event[i];
            next_event_type     = i;
        }

    /* Check to see whether the event list is empty. */

    if (next_event_type == 0) {

        /* The event list is empty, so stop the simulation */

        outfile << "\nEvent list empty at time " << sim_time;
        exit(1);
    }

    /* The event list is not empty, so advance the simulation clock. */

    sim_time = min_time_next_event;
}

int random_integer(float prob_distrib []){
    // float u, sum;
    // int i;
    // u = rand() / (RAND_MAX + 1.0);
    // sum = 0.0;
    // i = 0;
    // while(sum < u){
    //     i++;
    //     sum += prob_distrib[i];
    // }
    // return i;
    int   i;
    float u;

    u = lcgrand(1);
    for (i = 1; u >= prob_distrib[i]; ++i)
        ;
    return i;
}

float uniform(float min, float max){
    return min + lcgrand(1) * (max - min);
}

void order_arrival(void){
    inv_level += amount;
    time_next_event[1] = 1.0e+30;
}

void demand(void){
    inv_level -= random_integer(prob_distrib_demand);
    time_next_event[2] = sim_time + expon(mean_interdemand);
}

void evaluate(void){
    if (inv_level < smalls){
        amount = bigs - inv_level;
        total_ordering_cost += setup_cost + amount * incremental_cost;
        time_next_event[1] = sim_time + uniform(minlag, maxlag);
    }
    time_next_event[4] = sim_time + 1.0;
}

void update_time_avg_stats(void){
    float time_since_last_event;
    time_since_last_event = sim_time - time_last_event;
    time_last_event = sim_time;
    if(inv_level < 0){
        area_shortage -= time_since_last_event * inv_level;
    }
    else if(inv_level > 0){
        area_holding += time_since_last_event * inv_level;
    }
}

void report(void){
    float avg_holding_cost, avg_shortage_cost, avg_ordering_cost;
    avg_ordering_cost = total_ordering_cost / num_months;
    avg_holding_cost = holding_cost * area_holding / num_months;
    avg_shortage_cost = shortage_cost * area_shortage / num_months;
    cout << "order: " << total_ordering_cost << " holding: " << area_holding << " shortage: " << area_shortage << "\n";
    outfile << "\n\n(" << smalls << "," << bigs << ")\t" << avg_ordering_cost + avg_holding_cost + avg_shortage_cost << "\t" << avg_ordering_cost << "\t" << avg_holding_cost << "\t" << avg_shortage_cost << "\n";
}

int main(){
    int i, num_policies;
    infile.open("input.txt");
    outfile.open("output.txt");

    num_events = 4;

    // read input parameters
    infile >> initial_inv_level >> num_months >> num_policies >> num_values_demand >> mean_interdemand >> setup_cost >> incremental_cost >> holding_cost >> shortage_cost >> minlag >> maxlag;

    // read probability distribution
    for(i = 0; i < num_values_demand; i++){
        infile >> prob_distrib_demand[i];
    }

    // write report heading and input parameters
    outfile << "Single-Product Inventory System\n\n";
    outfile << "Initial inventory level \t" << initial_inv_level << " itemms\n\n";
    outfile << "Number of demand sizes \t" << num_values_demand << "\n\n";
    outfile << "Distribution function of demand sizes \t";
    for(i = 0; i < num_values_demand; i++){
        outfile << prob_distrib_demand[i] << " ";
    }
    outfile << "\n\n";
    outfile << "Mean interdemand time \t" << mean_interdemand << " months\n\n";
    outfile << "Delivery lag range \t" << minlag << " to " << maxlag << " months\n\n";
    outfile << "length of the simulation \t" << num_months << " months\n\n";
    outfile << "K = " << setup_cost << " i = " << incremental_cost << " h = " << holding_cost << " pi = " << shortage_cost << "\n\n";
    outfile << "Number of policies \t" << num_policies << "\n\n";
    outfile << "       \t Average \t\t Average \t\t Average \t\t Average\n";
    outfile << "Policy \t total Cost \t ordering cost \t holding cost \t shortage cost\n";

    // simulate the system with each policy
    for (i = 1; i <= num_policies; ++i) {
        infile >> smalls >> bigs;
        initialize();

        do {
            timing();

            update_time_avg_stats();

            switch (next_event_type) {
                case 1:
                    order_arrival();
                    break;
                case 2:
                    demand();
                    break;
                case 4:
                    evaluate();
                    break;
                case 3:
                    report();
                    break;
            }

        } while (next_event_type != 3);
    }
    infile.close();
    outfile.close();
}