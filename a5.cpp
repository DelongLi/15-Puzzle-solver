#include <iostream>
#include <climits>
#include <iterator>
#include <set>
#include <stack>
#include <algorithm> //random_shuffle
#include <vector>
#include <queue> //queue
#include <map> //map
#include <fstream> //file processing
#include <sstream> //formatted string processing
#include <cstdlib> //atof and atoi
#include <utility> // pair and make_pair
#include <ctime>
using namespace std;
typedef vector<int> vecint;


struct State {
    int board[4][4];
    int x, y; // location of blank
};

int myrandom(int);
vecint randPuzzle();
int findBlank(vecint);
int numInversion(vecint);
bool isSolvable(vecint);
void display(const State&);

// -----------------------------------------------------------------------
// load the 663 pattern database
// -----------------------------------------------------------------------

map<int,int> database_234; // 2, 3, 4
map<int,int> database_156; // 1, 5, 6, 9, 10, 13
map<int,int> database_78; // 7, 8, 11, 12, 14, 15

void load_pattern_database_663() {
    ifstream myfile1 ("database234.txt");
    if (myfile1.is_open())
    {
        string line;
        while(getline(myfile1,line)) {
            istringstream ss(line);
            int position, distance;
            string pos, dis;        
            getline(ss, pos, ' ');
            getline(ss, dis, ' ');
            position = atoi(pos.c_str());
            distance = atoi(dis.c_str());
            database_234[position] = distance;
        }
        myfile1.close();
    }
    else cout << "Unable to open file";

    ifstream myfile2 ("database15691013.txt");
    if (myfile2.is_open())
    {
        string line;
        while(getline(myfile2,line)) {
            istringstream ss(line);
            int position, distance;
            string pos, dis;        
            getline(ss, pos, ' ');
            getline(ss, dis, ' ');
            position = atoi(pos.c_str());
            distance = atoi(dis.c_str());
            database_156[position] = distance;
        }
        myfile2.close();
    }
    else cout << "Unable to open file";

    ifstream myfile3 ("database7811121415.txt");
    if (myfile3.is_open())
    {
        string line;
        while(getline(myfile3,line)) {
            istringstream ss(line);
            int position, distance;
            string pos, dis;        
            getline(ss, pos, ' ');
            getline(ss, dis, ' ');
            position = atoi(pos.c_str());
            distance = atoi(dis.c_str());
            database_78[position] = distance;
        }
        myfile3.close();
    }
    else cout << "Unable to open file";
}

// -----------------------------------------------------------------------

State goal, nongoal;
set<State> nongoals;
map<pair<int, int>, char> moves;

bool operator==(const State& lhs, const State& rhs) {
    if (!(lhs.x == rhs.x && lhs.y == rhs.y)) return false;
    for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) if (lhs.board[i][j] != rhs.board[i][j]) return false;
    return true;
}

bool operator<(const State& lhs, const State& rhs) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (lhs.board[i][j] < rhs.board[i][j]) return true;
            else if (lhs.board[i][j] > rhs.board[i][j]) return false;
        }
    }
    if (lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y)) return true;
    return false;
}

int find_num234(const State& state) {
    int num2 = -1, num3 = -1, num4 = -1;
    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++) {
            if(num2 != -1 && num2 != -1 && num3 != -1) return 10000*num2+100*num3+num4;
            if(state.board[i][j] == 2) num2 = 10*i+j;
            else if(state.board[i][j] == 3) num3 = 10*i+j;
            else if(state.board[i][j] == 4) num4 = 10*i+j;
        }
    }
}

int find_num15(const State& state) {
    int num1 = -1, num5 = -1, num6 = -1;
    int num9 = -1, num10 = -1, num13 = -1;
    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++) {
            if(num1 != -1 && num5 != -1 && num6 != -1 && num9 != -1 && num10 != -1 && num13 != -1) return 10000000000*num1+100000000*num5+1000000*num6+10000*num9+100*num10+num13;
            if(state.board[i][j] == 1) num1 = 10*i+j;
            else if(state.board[i][j] == 5) num5 = 10*i+j;
            else if(state.board[i][j] == 6) num6 = 10*i+j;
            else if(state.board[i][j] == 9) num9 = 10*i+j;
            else if(state.board[i][j] == 10) num10 = 10*i+j;
            else if(state.board[i][j] == 13) num13 = 10*i+j;
        }
    }
}

int find_num78(const State& state) {
    int num7 = -1, num8 = -1, num11 = -1;
    int num12 = -1, num14 = -1, num15 = -1;
    for(int i=0;i<4;i++) {
        for(int j=0;j<4;j++) {
            if(num7 != -1 && num8 != -1 && num11 != -1 && num12 != -1 && num14 != -1 && num15 != -1) return 10000000000*num7+100000000*num8+1000000*num11+10000*num12+100*num14+num15;
            if(state.board[i][j] == 7) num7 = 10*i+j;
            else if(state.board[i][j] == 8) num8 = 10*i+j;
            else if(state.board[i][j] == 11) num11 = 10*i+j;
            else if(state.board[i][j] == 12) num12 = 10*i+j;
            else if(state.board[i][j] == 14) num14 = 10*i+j;
            else if(state.board[i][j] == 15) num15 = 10*i+j;
        }
    }
}

// int h(State &s) {
//     int ret = 0;
//     for (int i = 0; i < 4; i++) {
//         for (int j = 0; j < 4; j++) {
//             if (s.x == i && s.y == j) continue;
//             int v = s.board[i][j];
//             int x = (v - 1)/4, y = (v - 1)%4;
//             ret += abs(x-i) + abs(y-j);
//         }    
//     }
//     return ret;
// }

int h(State& s) {
    return database_234[find_num234(s)] + database_156[find_num15(s)] + database_78[find_num78(s)];
}


bool solvable;
vector<char> prefix;
int exceed, costbound;
set<State> visited;
State node;

bool dfs(int depth) {
    int currcost = depth + h(node);
    if (currcost > costbound) {
        exceed = min(exceed, currcost);
        return false;
    }
    if (node == goal || nongoals.count(node) > 0) {
        solvable = (node == goal);
        return true;
    }

    visited.insert(node);

    for (map<pair<int, int>, char>::iterator it = moves.begin(); it != moves.end(); it++) {
        int dx = it->first.first, dy = it->first.second;
        if (0 <= node.x + dx && node.x + dx < 4 && 0 <= node.y + dy && node.y + dy < 4) {
            swap(node.board[node.x][node.y], node.board[node.x + dx][node.y + dy]);
            node.x += dx; node.y += dy;
            if (visited.count(node) == 0) {
                prefix.push_back(it->second);
                if (dfs(depth + 1)) return true;
                else prefix.pop_back();
            }
            node.x -= dx; node.y -= dy;
            swap(node.board[node.x][node.y],
            node.board[node.x + dx][node.y + dy]);
        }
    }
    visited.erase(node);
    return false;
}

void solve_idastar(State &start) {
    costbound = h(start);
    bool success;
    do {
        exceed = INT_MAX;
        prefix.clear();
        visited.clear();
        node = start;
        success = dfs(0);
        costbound = exceed;
        // cerr << "deepening to " << costbound << endl;
    } while (!success);
    if (solvable) {
        copy(prefix.begin(), prefix.end(), ostream_iterator<char>(cout, ""));
        cout << endl;
    }
    else cout << "This puzzle is not solvable." << endl;
}

int myrandom(int i) { return rand()%i; }

vecint randPuzzle() {
    srand(unsigned(time(0)));
    vecint state;
    for (int i=0; i<16; i++) state.push_back(i);
    random_shuffle(state.begin(),state.end(),myrandom);
    return state;
    while (true) {
        random_shuffle(state.begin(),state.end(),myrandom);
        if (isSolvable(state)) return state;
    }
}

bool isSolvable(vecint state) { // 15 puzzle
    int blank = findBlank(state);
    int line = 4 - blank/4; // line number from bottom
    /* if n is even, puzzle instance is solvable if 
     * 1. the blank is on an even row counting from the bottom and number of inversions is odd.
     * 2. the blank is on an odd row counting from the bottom and number of inversions is even.
     */ 
    if (line%2==0 && numInversion(state)%2==1) return true;
    else if (line%2==1 && numInversion(state)%2==0) return true;
    return false;
}

int numInversion(vecint state) {
    int size = state.size();
    int num = 0;
    for (int i=0; i<size-1; i++) {
        if (state[i] == 0) continue;
        for (int j=i+1; j<size; j++) {
            if (state[j] == 0) continue;
            if (state[i] > state[j]) num++;
        }
    }
    return num;
}

int findBlank(vecint state) {
    for(int i=0;i<state.size();i++) if(state[i] == 0) return i;
}

State rand_state() {
    vecint puzzle = randPuzzle();
    State state;
    for(int i=0;i<puzzle.size();i++) {
        int row = i/4, column = i%4;
        if(puzzle[i] == 0) {
            state.x = row;
            state.y = column;
        }
        state.board[row][column] = puzzle[i];
    }
    return state;
} 


// display the puzzle
void display(const State& s) {
    cout << "The state of " << 15 << "-puzzle: " << endl;
    cout << "--------------------------------------------" << endl;
    for (int i=0; i<4; i++) {
        for (int j=0;j<4;j++) {
            if (s.board[i][j] == 0) cout << " * ";
            else {
                if (s.board[i][j] < 10) cout << " ";
                cout << s.board[i][j] << " ";
            }
        }
        cout << endl;
    }
    cout << "--------------------------------------------" << endl;

}

int main() {
    const clock_t begin_time_load = clock();
    load_pattern_database_663();
    cout << "Running time(seconds) of loading pattern database 663: " << float( clock () - begin_time_load ) /  CLOCKS_PER_SEC << endl;


    moves[make_pair(0, -1)] = 'L'; moves[make_pair(0, +1)] = 'R';
    moves[make_pair(-1, 0)] = 'U'; moves[make_pair(+1, 0)] = 'D';

    const clock_t begin_time_rand_state = clock();
    State start = rand_state();
    display(start);
    cout << "Running time(seconds) of creating random state: " << float( clock () - begin_time_rand_state ) /  CLOCKS_PER_SEC << endl;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            goal.board[i][j] = nongoal.board[i][j] = 4*i + j + 1;
        }
    }

    goal.board[3][3] = nongoal.board[3][3] = 0;
    goal.x = nongoal.x = 3; goal.y = nongoal.y = 3;

    swap(nongoal.board[3][2], nongoal.board[3][1]);
    nongoals.insert(nongoal);
    swap(nongoal.board[3][2], nongoal.board[3][1]);

    swap(nongoal.board[0][0], nongoal.board[0][1]);
    nongoals.insert(nongoal);
    swap(nongoal.board[0][0], nongoal.board[0][1]);
    
	const clock_t begin_time_solve = clock();
    solve_idastar(start);
	cout << "Running time(seconds) of solving puzzle: " << float( clock () - begin_time_solve ) /  CLOCKS_PER_SEC << endl;


    return 0;
}