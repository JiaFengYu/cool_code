#include <iostream>
#include <vector>
#include <utility> 
#include <random>
#include <algorithm>
#include <chrono>
using namespace std;

vector<int> get_val(vector<vector<int>>& board, int i, int j) {
    vector<int> range = {1,2,3,4,5,6,7,8,9};
    vector<int> temp;
    for (auto item: board[i]) {
        if (item != 0) {
            temp.emplace_back(item);
        }
    }
    //remove the horizontal ones
    for (auto num: temp){
        auto it = find(range.begin(), range.end(), num);
        if (it != range.end()) {
            range.erase(it);
        }
    }
    temp = {};
    for (auto row: board) {
        if (row[j] != 0) {
            temp.emplace_back(row[j]);
        }
    }
    //remove the vertical ones;
    for (auto num: temp){
        auto it = find(range.begin(), range.end(), num);
        if (it != range.end()) {
            range.erase(it);
        }
    }

    int quadrant_h = ((i/3)*3);
    int quadrant_v = ((j/3)*3);
    vector<int> incr = {0,1,2};
    //removes 00, 01, 10, 11, 02, 20, 22 (relative coordinates)
    for (auto inc:incr) {
        if (board[quadrant_h+inc][quadrant_v] != 0) {
            auto it = find(range.begin(), range.end(), board[quadrant_h+inc][quadrant_v]);
            if (it != range.end()) {
                range.erase(it);
            }
        }
        if (board[quadrant_h][quadrant_v+inc] != 0) {
            auto it = find(range.begin(), range.end(), board[quadrant_h][quadrant_v+inc]);
            if (it != range.end()) {
                range.erase(it);
            }
        }

        if (board[quadrant_h+inc][quadrant_v+inc] != 0) {
            auto it = find(range.begin(), range.end(), board[quadrant_h+inc][quadrant_v+inc]);
            if (it != range.end()) {
                range.erase(it);
            }
        }
    }

    //removes 12, 21 (the missing coordinates)
    //note that ij will never be added/removed because ij == 0 by definition
    if (board[quadrant_h+1][quadrant_v+2] != 0) {
        auto it = find(range.begin(), range.end(), board[quadrant_h+1][quadrant_v+2]);
        if (it != range.end()) {
            range.erase(it);
        }
    }

    if (board[quadrant_h+2][quadrant_v+1] != 0) {
        auto it = find(range.begin(), range.end(), board[quadrant_h+2][quadrant_v+1]);
        if (it != range.end()) {
            range.erase(it);
        }
    }

    return range;
}

pair<vector<vector<int>>,int> B(int seed, vector<vector<int>> input) {
    srand(seed);
    vector<vector<int>> rval;
    copy(input.begin(), input.end(), back_inserter(rval));
    vector<pair<int,int>> positions;
    for (int i = 0; i < rval.size(); i++) {
        for (int j = 0; j < rval[i].size(); j++) {
            if ( rval[i][j] == 0) {
                //cout << i << " " << j << endl;            
                positions.emplace_back(make_pair(i,j));
            }
        }
    }
    shuffle(positions.begin(), positions.end(), default_random_engine(seed));
    int nodes_counter = 0;
    vector<pair<pair<int,int>, vector<int>>> stack;
    int random_index = 0;    
    int nums_random_index = -1;
    int initial_positions = positions.size();
    while(random_index < positions.size()) {
        //get possible numbers at that location
        vector<int> nums = get_val(rval, positions[random_index].first, positions[random_index].second);
        // IF NO POSSIBLE MOVES AT THIS SCENARIO, BACKTRACK
        if (nums.empty()) {
            int last_move_i = stack.back().first.first;
            int last_move_j = stack.back().first.second;
            if (stack.back().second.empty()) {
                //no more legal moves, backtrack to last node
                // reset to 0
                rval[last_move_i][last_move_j] = 0;
                stack.pop_back();
                // add stack top position back 
                random_index--;
                //previous node now since we popped
                nums = stack.back().second;
                //keep backtracing until we find something with choices
                while (nums.empty()) {
                    last_move_i = stack.back().first.first;
                    last_move_j = stack.back().first.second;
                    rval[last_move_i][last_move_j] = 0;
                    stack.pop_back();
                    random_index--;
                    //previous node now since we popped
                    nums = stack.back().second;
                }
                random_index--;
                // have a valid choice now 
                int backtrack_random_idx = rand()%(nums.size());
                rval[positions[random_index].first][positions[random_index].second] = nums[backtrack_random_idx];
                nums.erase(nums.begin() + backtrack_random_idx);
                stack.back().second = nums;
                random_index++;
                continue;
            } else {
                // update last value and try again
                bool new_num_works = false;
                while (!stack.back().second.empty()) {
                    // pick another legal move from the remaining MOVES
                    nums_random_index = rand()%(stack.back().second.size());
                    rval[last_move_i][last_move_j] = stack.back().second[nums_random_index];
                    //remove this possibility as we have tried it
                    stack.back().second.erase(stack.back().second.begin()+nums_random_index);
                    vector<int> new_nums = get_val(rval, positions[random_index].first, positions[random_index].second);
                    if (!new_nums.empty()) {
                        //new number works, push it and go next
                        new_num_works = true;
                        int new_num_random_idx = rand()%(new_nums.size());
                        rval[positions[random_index].first][positions[random_index].second] = new_nums[new_num_random_idx];
                        new_nums.erase(new_nums.begin() + new_num_random_idx);
                        stack.emplace_back(make_pair(positions[random_index], new_nums));
                        random_index++;
                        nodes_counter++;
                        break; 
                    }
                }
                if (!new_num_works) {
                    // still havent found a legit num 
                    //no more legal moves, backtrack to last node
                    // reset to 0
                    rval[last_move_i][last_move_j] = 0;
                    stack.pop_back();
                    random_index--;
                    //previous node now since we popped
                    nums = stack.back().second;
                    while (nums.empty()) {
                        last_move_i = stack.back().first.first;
                        last_move_j = stack.back().first.second;
                        rval[last_move_i][last_move_j] = 0;
                        stack.pop_back();
                        random_index--;
                        //previous node now since we popped
                        nums = stack.back().second;
                    }
                    // have a valid choice now 
                    random_index--;
                    int backtrack_random_idx = rand()%(nums.size());
                    rval[positions[random_index].first][positions[random_index].second] = nums[backtrack_random_idx];
                    nums.erase(nums.begin() + backtrack_random_idx);
                    stack.back().second = nums;
                    random_index++;
                    continue;
                }
            }
        } else {
            // keep going 
            nums_random_index = rand()%(nums.size());
            //pick a random, legal number at that location and set it in our solution
            rval[positions[random_index].first][positions[random_index].second] = nums[nums_random_index];
            //remove that number as we tried it
            nums.erase(nums.begin() + nums_random_index);
            //remove the element as we have taken it already
            stack.emplace_back(make_pair(positions[random_index], nums));
            //positions.erase(positions.begin() + random_index);
            random_index++;
            nodes_counter++;
        }
    }
    return make_pair(rval, nodes_counter);
}

// fn to check if this num at this position is legal
bool legal(vector<vector<int>>& grid, int row, int col, int num) {
    // Check if the number already exists in the row or column
    for (int i = 0; i < 9; i++) {
        if (grid[row][i] == num || grid[i][col] == num) {
            return false;
        }
    }

    // Check if the number already exists in the current 3x3 sub-grid
    int square_i = (row/3)*3;
    int square_j = (col/3)*3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[square_i+i][square_j+j] == num) {
                return false;
            }
        }
    }

    return true;
}

// check if we full or nah
bool not_full(vector<vector<int>>& grid, int& row, int& col) {
    for (row = 0; row < 9; row++) {
        for (col = 0; col < 9; col++) {
            if (grid[row][col] == 0) {
                return true;
            }
        }
    }
    return false;
}

// recursive fn to solve the sudoku with backtracking and fc
// different implementation because itterative B is too slow
bool B_fc(vector<vector<int>>& grid, int& nodes_expanded, int seed) {
    srand(seed);
    int row, col;
    //check if we have filled every spot, if yes gg
    if (!not_full(grid, row, col)) {
        return true; // CQFD
    }
    // get all possible values with forward checking
    vector<int> range = get_val(grid, row, col);
    shuffle(range.begin(), range.end(), default_random_engine(seed));
    //enumerate all possible values until we cant no more
    for (auto num : range) {
        if (legal(grid, row, col, num)) {
            grid[row][col] = num;
            //for (int i = 0; i < 9; i++) {
            //    for (int j = 0; j < 9; j++) {
            //        cout << grid[i][j] << " ";
            //    }
            //    cout << endl;
            //}
            //cout << nodes_expanded << endl;
            nodes_expanded++;
            if (B_fc(grid, nodes_expanded, seed)) {
                return true; //gg
            } else {
            // backtrack
            grid[row][col] = 0;
            }
        }
    }
    // no solution with this permutation
    return false; 
}

bool B_fc_h(vector<vector<int>>& grid, int& nodes_expanded) {
    int row, col;

    // Find an unassigned location
    if (!not_full(grid, row, col)) {
        return true; // Sudoku solved successfully
    }

    // Find the most constrained variable
    int minCandidates = 9 + 1;
    int mostConstrainedRow, mostConstrainedCol;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (grid[i][j] == 0) {
                int candidates = 0;
                for (int num = 1; num <= 9; num++) {
                    if (legal(grid, i, j, num)){ 
                        candidates++;
                    }
                }
                if (candidates < minCandidates) {
                    minCandidates = candidates;
                    mostConstrainedRow = i;
                    mostConstrainedCol = j;
                }
            }
        }
    }

    //enumerate all possible values until we cant no more
    for (int num = 1; num <= 9; num++) {
        if (legal(grid, mostConstrainedRow, mostConstrainedCol, num)){ 
            grid[mostConstrainedRow][mostConstrainedCol] = num;
            //for (int i = 0; i < 9; i++) {
            //    for (int j = 0; j < 9; j++) {
            //        cout << grid[i][j] << " ";
            //    }
            //    cout << endl;
            //}
            //cout << nodes_expanded << endl;
            nodes_expanded++;
            if (B_fc_h(grid, nodes_expanded)) {
                return true; //gg
            } else {
                // backtrack
            grid[mostConstrainedRow][mostConstrainedCol] = 0;
            }
        }
    }
    // no solution with this permutation
    return false; 
}


int main() {
    vector<vector<int>> easy = {
        {0,5,8,0,6,2,1,0,0},
        {0,0,2,7,0,0,4,0,0},
        {0,6,7,9,0,1,2,5,0},
        {0,8,6,3,4,0,0,0,0},
        {0,0,0,0,0,0,0,0,0},
        {0,0,0,0,7,6,8,9,0},
        {0,2,9,6,0,8,7,4,0},
        {0,0,3,0,0,4,9,0,0},
        {0,0,5,2,9,0,3,8,0},
    };
    vector<vector<int>> medium = {
        {8,3,0,6,0,0,0,0,7},
        {0,0,7,0,2,0,0,5,0},
        {0,2,1,0,0,9,0,8,0},
        {6,0,0,0,8,0,0,0,9},
        {0,0,0,4,6,5,0,0,0},
        {3,0,0,0,9,0,0,0,2},
        {0,8,0,2,0,0,3,9,0},
        {0,5,0,0,4,0,2,0,0},
        {2,0,0,0,0,8,0,1,6},
    };
    vector<vector<int>> hard = {
        {1,0,0,0,3,0,0,0,0},
        {0,6,2,0,0,0,0,0,0},
        {0,0,0,7,0,2,8,0,4},
        {0,7,0,1,4,0,0,0,2},
        {0,4,0,0,0,0,0,9,0},
        {8,0,0,0,5,6,0,7,0},
        {6,0,9,8,0,7,0,0,0},
        {0,0,0,0,0,0,2,1,0},
        {0,0,0,0,6,0,0,0,9},
    };
    vector<vector<int>> evil = {
        {0,1,0,0,0,0,0,0,6},
        {9,0,0,2,0,0,0,0,0},
        {7,3,2,0,4,0,0,1,0},
        {0,4,8,3,0,0,0,0,2},
        {0,0,0,0,0,0,0,0,0},
        {3,0,0,0,0,4,6,7,0},
        {0,9,0,0,3,0,5,6,8},
        {0,0,0,0,0,2,0,0,1},
        {6,0,0,0,0,0,0,3,0},
    };

    vector<vector<int>> sol;
    copy(easy.begin(), easy.end(), back_inserter(sol));
    //copy(medium.begin(), medium.end(), back_inserter(sol));
    //copy(hard.begin(), hard.end(), back_inserter(sol));
    //copy(evil.begin(), evil.end(), back_inserter(sol));
    int run_counter = 1;
    int nodes_expanded;
    vector<double> time;
    vector<int> nodes;
    while (run_counter < 51) {
        cout << "Seed value: " << run_counter*17 << endl;
        auto start = std::chrono::steady_clock::now();
        //pair<vector<vector<int>>,int> solution = B(run_counter, easy);
        //pair<vector<vector<int>>,int> solution = B(run_counter, medium);
        //pair<vector<vector<int>>,int> solution = B(run_counter, hard);
        //pair<vector<vector<int>>,int> solution = B(run_counter, evil);

        //B_fc(sol, nodes_expanded, run_counter*17);
        B_fc_h(sol, nodes_expanded);
        auto end = std::chrono::steady_clock::now();

        //UNCOMMENT IF THIS IS FOR B
        //for (auto i: solution.first) {
        //    for (auto j: i) {
        //        cout << j << " ";
        //    }
        //    cout << endl;
        //}
        // UNCOMMENT IF THIS IS FOR B+FC OR BFC+H
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                cout << sol[i][j] << " ";
            }
            cout << endl;
        }
        chrono::duration<double> diff = end - start;
        time.emplace_back(diff.count());
        // UNCOMMENT IF THIS IS FOR B
        //nodes.emplace_back(solution.second);
        // UNCOMMENT IF THIS IS FOR B+FC OR BFC+H
        nodes.emplace_back(nodes_expanded);
        
        cout << "Duration [seconds]: " << diff.count() << endl;

        // UNCOMMENT IF THIS IS FOR B
        //cout << "Nodes expanded: " << solution.second << endl;

        // UNCOMMENT IF THIS IS FOR B+FC OR BFC+H
        cout << "Nodes expanded: " << nodes_expanded << endl;
        nodes_expanded = 0;
        // reset the values for next run (ONLY FOR RECURSIVE FN)
        sol.clear();
        copy(easy.begin(), easy.end(), back_inserter(sol));
        //copy(medium.begin(), medium.end(), back_inserter(sol));
        //copy(hard.begin(), hard.end(), back_inserter(sol));
        //copy(evil.begin(), evil.end(), back_inserter(sol));
        run_counter++;
    }
    cout << "Runtimes (s): ";
    for (auto i: time) {
        cout << i << ","; 
    }
    cout << endl << "Nodes expanded: ";
    for (auto i: nodes) {
        cout << i << ",";
    }
    cout << endl;
}
