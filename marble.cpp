#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include <vector>
#include <array>
#include <map>
using namespace std;

// get all the possible moves for one board state
// you want to backtrace iwth a parent vector as well
// and use bfs to find all states, if a state is the final state, 
// exit the while loop 
// there are at most 16 neighborhood states 
vector<array<int, 4>> get_hood(array<int, 4> state, int n) {
    vector<array<int, 4 >> rval;
    // first can go right
    if (state[0]+1 < state[1]) {
        array<int, 4> first_right = {state[0]+1, state[1], state[2], state[3]};
        rval.emplace_back(first_right);
    } 
    // first can go left
    if (state[0] != 1) {
        array<int, 4> first_left = {state[0]-1, state[1], state[2], state[3]};
        rval.emplace_back(first_left);
    } 
    // first can reflect across second
    // DO I NEED TO CHECK IF IT IS EQUAL TO POSITION OF MARBLE 2?
    if ((2*state[1]-state[0] < state[2])&&(2*state[1]-state[0] > state[1])) {
        array<int, 4> first_reflect = {state[1], 2*state[1]-state[0], state[2], state[3]};
        rval.emplace_back(first_reflect);
    }
    
    //second can go right 
    if (state[1]+1 < state[2]) {
        array<int, 4> second_right = {state[0], state[1]+1, state[2], state[3]};
        rval.emplace_back(second_right);
    }
    //second can go left
    if (state[1]-1 > state[0]) {
        array<int, 4> second_left = {state[0], state[1]-1, state[2], state[3]};
        rval.emplace_back(second_left);
    }
    //second can reflect across 1
    // DO I NEED TO CHECK IF 2P-Q < STATE[0]???
    if ((2*state[0] - state[1] >= 1) && (2*state[0]-state[1] <  state[0])) {
        array<int, 4> second_r_left = {2*state[0]-state[1], state[0], state[2], state[3]};
        rval.emplace_back(second_r_left);
    }
    // second can reflect across 3
    //
    if ((2*state[2]-state[1] < state[3]) && (2*state[2]-state[1] > state[2])) {
        array<int, 4> second_r_right = {state[0], state[2], 2*state[2]-state[1], state[3]};
        rval.emplace_back(second_r_right);
    }

    //third can go right 
    if (state[2]+1 < state[3]) {
        array<int, 4> third_right = {state[0], state[1], state[2]+1, state[3]};
        rval.emplace_back(third_right);
    }
    //third can go left
    if (state[2]-1 > state[1]) {
        array<int, 4> third_left = {state[0], state[1], state[2]-1, state[3]};
        rval.emplace_back(third_left);
    }
    //third can reflect across 2
    if ((2*state[1] - state[2] > state[0]) && (2*state[1]-state[2] < state[1])) {
        array<int, 4> third_r_left = {state[0], 2*state[1]-state[2], state[1], state[3]};
        rval.emplace_back(third_r_left);
    }
    // third can reflect across 4
    //
    if ((2*state[3]-state[2] <= n) && (2*state[3]-state[2] > state[3])) {
        array<int, 4> third_r_right = {state[0], state[1], state[3], 2*state[3]-state[2]};
        rval.emplace_back(third_r_right);
    }

    //fourth can go left
    if (state[3]-1 > state[2]) {
        array<int, 4> fourth_left = {state[0], state[1], state[2], state[3]-1};
        rval.emplace_back(fourth_left);
    }
    // fourth can go right
    if (state[3]+1 <= n) {
        array<int, 4> fourth_right = {state[0], state[1], state[2], state[3]+1};
        rval.emplace_back(fourth_right);
    }
    //fourth can reflect accross 3
    if ((2*state[2]-state[3] > state[1]) && (2*state[2]-state[3] < state[2])) {
        array<int, 4> fourth_r_left = {state[0], state[1], 2*state[2]-state[3], state[2]};
        rval.emplace_back(fourth_r_left);
    }

    return rval;

}

int main() {
    string holder;
    getline(cin, holder);
    int n = stoi(holder);
    array<int, 4> start;
    array<int, 4> end;
    // get starting state
    getline(cin, holder);
    istringstream iss (holder);
    for (int i = 0; i < 4; i++){
        string val;
        iss >> val;
        start[i] = stoi(val);
    }
    getline(cin, holder);
    istringstream iss1 (holder);
    for (int i = 0; i < 4; i++) {
        string val;
        iss1 >> val;
        end[i] = stoi(val);
    }
    //    for (int i = 0; i < 4; i++) {
    //        cout << start[i] << " "; 
    //    }
    //    cout << n << endl;
    queue<array<int, 4>> neighbors;
    map<array<int, 4>, bool> visited;
    vector<array<int, 4>> rval;
    map<array<int, 4>, array<int, 4>> parent;

    neighbors.push(start);
    while (!neighbors.empty()) {
        array<int, 4> node = neighbors.front();
        neighbors.pop();
        rval = get_hood(node, n);
        for (auto i: rval){
            // if already visited, then its not it 
            if (!visited.count(i)){
                visited.insert({i, true});
                neighbors.push(i);

                // check if this current iteration is the
                // the target state 
                if (i == end) {
                    // return here
                    // find a way to trace back its path
                    // cout << "FOUND IT" << endl;
                    // for (int j = 0; j < 4; j++){
                    //     cout << i[j] << " ";
                    // }
                    // cout << endl;
                    int path_len = 0;
                    vector<array<int, 4>> path;
                    path.insert(path.begin(), i);
                    path.insert(path.begin(), node);
                    array<int, 4> curr_dad = node;
                    while (curr_dad != start) {
                        auto yeye = parent.find(curr_dad)->second;
                        // maybe check if its null?
                        path.insert(path.begin(), yeye);
                        curr_dad = yeye;
                    }
                    cout << path.size()-1 << endl;
                    for (auto line:path) {
                        for (int i = 0; i < 3; i++) {
                            cout << line[i] << " ";
                        }

                        cout << line[3] << endl;
                    }
                    return 0;
                }
                parent.insert({i, node});
            }
            //for (int j = 0; j < 4; j++){
            //    cout << i[j] << " ";
            //}
            //cout << endl;
        }
    }
    return 0;
}

