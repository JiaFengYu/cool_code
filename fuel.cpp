#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <array>
#include <algorithm>
using namespace std;

struct state {
    int fuel;
    int dollars;
    int time_passed;
    // change this to a manual array, only up to n_city anyways
    //vector<int> gas_added;
    array<int, 20> gas_added;

};

int main() {
    string holder;
    getline(cin, holder);
    istringstream iss (holder);
    string s;
    iss >> s;
    int num_cities = stoi(s);
    iss >> s;
    int budget = stoi(s);
    iss >> s;
    int capacity = stoi(s);

    vector<int> time_to_city (num_cities-1);
    vector<int> time_per_fuel (num_cities);
    vector<int> cost_per_fuel (num_cities);
    getline(cin, holder);
    istringstream iss1 (holder);
    for (int i = 0; i < num_cities-1; i++) {
        iss1 >> s;
        time_to_city[i] = stoi(s);
    }
    getline(cin, holder);
    istringstream iss2 (holder);
    for (int i = 0; i < num_cities; i++) {
        iss2 >> s;
        time_per_fuel[i] = stoi(s);
    }
    getline(cin, holder);
    istringstream iss3 (holder);
    for (int i = 0; i < num_cities; i++) {
        iss3 >> s;
        cost_per_fuel[i] = stoi(s);
    }
    if (time_to_city[0] > capacity) {
        // cant even reach city 2 LOL
        cout << "-1" << endl;
        return 0;
    }

    // change first layer to manual array
    vector<vector<state>> dp (num_cities);
    for (int i = 0; i < num_cities; i++) {
        vector<state> temp_vec;
        dp[i] = temp_vec;
    }
    
    struct state first_city;
    first_city.time_passed = time_to_city[0];
    array<int, 20> base;
    first_city.gas_added = base;
    first_city.gas_added[0] = 0;
    first_city.dollars = budget;
    first_city.fuel = capacity - time_to_city[0];
    dp[1].emplace_back(first_city);


    for (int i = 1; i < num_cities-1; i++) {
        for (auto state: dp[i]) {
            int distance = time_to_city[i];
            int min_fueling = distance-state.fuel;
            if (distance > state.fuel) {
                // must refuel 
                if (cost_per_fuel[i]*min_fueling > state.dollars) {
                    // cant go nowhere
                    // continue to the next state
                    continue;
                }
            } else {
                // dont need to fuel, check if we keep going
                struct state vroom;
                vroom.gas_added = state.gas_added;
                vroom.gas_added[i] = 0;
                vroom.fuel = state.fuel-distance;
                vroom.dollars = state.dollars;
                vroom.time_passed = state.time_passed + distance;
                dp[i+1].emplace_back(vroom);
            }
            // add all legit fuel combinations - CHOOSE TO FUEL
            // from min_fueling to max_capacity
            if (min_fueling <= 0) {
                min_fueling = 1;
            }
            for (int j = min_fueling; j <= capacity-state.fuel; j++) {
                if (j*cost_per_fuel[i] > state.dollars) {
                    // cant add this much fuel
                    continue;
                }
                struct state new_state;
                new_state.gas_added = state.gas_added;
                new_state.gas_added[i] = j;
                new_state.fuel = state.fuel + j - distance;
                new_state.dollars = state.dollars - j*cost_per_fuel[i];
                new_state.time_passed = state.time_passed + j*time_per_fuel[i] + distance;
                dp[i+1].emplace_back(new_state);
            }
        }
    }
    if (dp[num_cities-1].empty()) {
        cout << "-1" << endl;
        return 0;
    }
    int min_timepassed = dp[num_cities-1][0].time_passed;
    struct state optimal_route = dp[num_cities-1][0];
    for (auto f_state : dp[num_cities-1]) {
        if (f_state.time_passed < min_timepassed) {
            min_timepassed = f_state.time_passed;
            optimal_route = f_state;
        }
    }
    cout << optimal_route.time_passed << endl;
    for (int i = 0; i < num_cities-1; i++) {
        cout << optimal_route.gas_added[i] << " ";
    }
    cout << "0 \n"; // because we dont add gass at final city
    return 0;
}

