#include <utility> // std::pair, std::make_pair
#include <iostream>
#include <map>
#include <queue> // queue
#include <vector>
#include <fstream> //file processing
using namespace std;
typedef vector<int> vecint;


// operator<< overloading for vector<int>
ostream& operator<<(ostream& os, const vecint& dt)
{
    for(int i=0;i<dt.size()-1;i++) {
        if(dt[i] < 10) os << 0;
        os << dt[i];
    }
    return os;
}

// -----------------------------------------------------------------------
// create the 663 pattern database for 15 puzzle
// -----------------------------------------------------------------------

void move( map<vecint,pair<int,bool>>& database, queue<vecint>& myqueue, int position0, int new_position0, vecint parent) {
    int size = parent.size();
    vecint move(parent);
    move[size-1] = new_position0;
    int cost = 0;
    for(int i=0;i<size-1;i++) {
        if(parent[i] == new_position0) {
            cost = 1;
            move[i] = position0;
            break;
        }
    }
    if(database[move].first == 0) {
        if(!database[move].second) {
            myqueue.push(move);
            database[move] = make_pair(database[parent].first + cost, true);
        }
    }
    else {
        if(database[move] > database[parent]) {
            myqueue.push(move);
            database[move].first = database[parent].first + cost;                    
        }
    }
}

void bfs_pattern_database( map<vecint,pair<int,bool>>& database, vecint goal) {
    queue<vecint> myqueue;
    myqueue.push(goal);
    database[goal] = make_pair(0,true);
    while(!myqueue.empty()) {
        vecint parent = myqueue.front();
        myqueue.pop();

        int position0 = parent.back();
        int row = position0 / 10, column = position0 % 10;
        // move up
        if(row != 0) move(database, myqueue, position0, position0-10, parent);
        // move left
        if(column != 0) move(database, myqueue, position0, position0-1, parent);
        // move right
        if(column != 3) move(database, myqueue, position0, position0+1, parent);
        // move down
        if(row != 3) move(database, myqueue, position0, position0+10, parent);
    }
}

void create_pattern_database_663() {
    int partation1[] = { 01, 02, 03, 33 };
    vecint goal1(partation1, partation1 + sizeof(partation1) / sizeof(int) );
    int partation2[] = { 00, 10, 11, 20, 21, 30, 33 };
    vecint goal2(partation2, partation2 + sizeof(partation2) / sizeof(int) );
    int partation3[] = { 12, 13, 22, 23, 31, 32, 33 };
    vecint goal3(partation3, partation3 + sizeof(partation3) / sizeof(int) );

    map<vecint,pair<int,bool>> database1;
    map<vecint,pair<int,bool>> database2;
    map<vecint,pair<int,bool>> database3;

    cout << "Create pattern database for 2, 3, 4 ......" << endl;
    const clock_t begin_time_1 = clock();
    bfs_pattern_database(database1, goal1);
    cout << "Running time(seconds): " << float( clock () - begin_time_1 ) /  CLOCKS_PER_SEC << endl;
    cout << " ..... done ......" << endl;

    cout << "Create pattern database for 1, 5, 6, 9, 10, 13 ......" << endl;
    const clock_t begin_time_2 = clock();
    bfs_pattern_database(database2, goal2);
    cout << "Running time(seconds): " << float( clock () - begin_time_2 ) /  CLOCKS_PER_SEC << endl;
    cout << " ..... done ......" << endl;

    cout << "Create pattern database for 7, 8, 11, 12, 14, 15 ......" << endl;
    const clock_t begin_time_3 = clock();
    bfs_pattern_database(database3, goal3);
    cout << "Running time(seconds): " << float( clock () - begin_time_3 ) /  CLOCKS_PER_SEC << endl;
    cout << " ..... done ......" << endl;

    // store the pattern database in 3 files
    // 2, 3, 4    
    ofstream myfile;
    myfile.open ("database234.txt");
    int i = 1;
    for (auto it = database1.begin(); it != database1.end(); it++) {
        if(i%13 == 1) myfile << it->first <<" " << it->second.first << '\n'; 
        i++;
    }
    myfile.close();

    // 1, 5, 6, 9, 10, 13
    myfile.open ("database15691013.txt");
    i = 1;
    for (auto it = database2.begin(); it != database2.end(); ++it) {
        if(i%10 == 1) myfile << it->first <<" " << it->second.first << '\n'; 
        i++;
    }
    myfile.close();

    // 1, 5, 6, 9, 10, 13
    myfile.open ("database7811121415.txt");
    i = 1;
    for (auto it = database3.begin(); it != database3.end(); ++it) {
        if(i%10 == 1) myfile << it->first <<" " << it->second.first << '\n';
        i++;
    }
    myfile.close();
}

int main() {
    const clock_t begin_time_create = clock();
    create_pattern_database_663();
    cout << "Running time(seconds) of creating pattern database 663: " << float( clock () - begin_time_create ) /  CLOCKS_PER_SEC << endl;
}


