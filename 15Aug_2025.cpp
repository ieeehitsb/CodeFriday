#include <bits/stdc++.h>
using namespace std;

struct Node {
    int f, g;
    pair<int, int> pos;
    vector<pair<int, int>> path;
    bool operator>(const Node &other) const {
        return f > other.f; // min-heap behavior
    }
};

int H(pair<int, int> a, pair<int, int> b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

vector<pair<int, int>> astar(const vector<vector<int>> &grid,
                             pair<int, int> start,
                             pair<int, int> goal) {
    priority_queue<Node, vector<Node>, greater<Node>> open_set;
    set<pair<int, int>> visited;

    open_set.push({H(start, goal), 0, start, {}});

    while (!open_set.empty()) {
        Node current = open_set.top();
        open_set.pop();

        if (current.pos == goal) {
            current.path.push_back(current.pos);
            return current.path;
        }

        if (visited.count(current.pos)) continue;
        visited.insert(current.pos);

        int dr[4] = {1, -1, 0, 0};
        int dc[4] = {0, 0, 1, -1};

        for (int i = 0; i < 4; i++) {
            int nr = current.pos.first + dr[i];
            int nc = current.pos.second + dc[i];
            if (nr >= 0 && nr < (int)grid.size() &&
                nc >= 0 && nc < (int)grid[0].size() &&
                grid[nr][nc] == 0) {
                auto new_path = current.path;
                new_path.push_back(current.pos);
                open_set.push({current.g + 1 + H({nr, nc}, goal),
                               current.g + 1,
                               {nr, nc},
                               new_path});
            }
        }
    }
    return {}; // no path found
}

int main() {
    vector<vector<int>> grid = {
        {0, 0, 0, 0},
        {1, 1, 0, 1},
        {0, 0, 0, 0},
        {0, 1, 1, 0}
    };
    pair<int, int> start = {0, 0}, goal = {3, 3};

    auto path = astar(grid, start, goal);
    if (!path.empty()) {
        cout << "Path found:\n";
        for (auto &p : path) {
            cout << "(" << p.first << "," << p.second << ") ";
        }
        cout << "\n";
    } else {
        cout << "No path found.\n";
    }
}