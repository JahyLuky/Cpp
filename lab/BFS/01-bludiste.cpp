#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <deque>
#include <queue>
#include <list>

struct Position {
    int row, col;

    Position() = default;

    Position(int r, int c) : row(r), col(c) {}

# define OP(op) \
    friend bool operator op (Position a, Position b) { \
      return std::tie(a.row, a.col) op std::tie(b.row, b.col); \
    }

    OP(==)

    OP(!=)

    OP(<)

    OP(>)

    OP(<=)

    OP(>=)

# undef OP
};

using Graph = std::map<Position, std::vector<Position>>;

struct Map {
    enum Tile {
        EMPTY, WALL
    };

    Graph to_graph() const {
        std::map<Position, std::vector<Position>> result;
        int num_rows = (int) map.size();
        int num_cols = (int) map[0].size();

        for (int i = 1; i < num_rows - 1; ++i) {
            for (int j = 0; j < num_cols - 1; ++j) {
                if (map[i][j] == WALL)
                    continue;

                std::vector<Position> neighbors;
                if (map[i + 1][j] != WALL)
                    neighbors.push_back({i + 1, j});
                if (map[i][j + 1] != WALL)
                    neighbors.push_back({i, j + 1});
                if (map[i - 1][j] != WALL)
                    neighbors.push_back({i - 1, j});
                if (map[i][j - 1] != WALL)
                    neighbors.push_back({i, j - 1});
                result[{i, j}] = neighbors;
            }
        }
        return result;
    }

    std::vector<std::vector<Tile>> map;
    Position start, exit;
    std::vector<Position> food, water, teleports;

    Map() = default;

    explicit Map(const std::string &file) {
        std::ifstream inp(file);
        std::string line;
        while (std::getline(inp, line)) {
            std::vector<Tile> cur;
            for (auto t: line)
                switch (t) {
                    case '#':
                        cur.push_back(WALL);
                        break;
                    case 'f':
                        food.emplace_back(map.size(), cur.size());
                        cur.push_back(EMPTY);
                        break;
                    case 'w':
                        water.emplace_back(map.size(), cur.size());
                        cur.push_back(EMPTY);
                        break;
                    case 'X':
                        teleports.emplace_back(map.size(), cur.size());
                        cur.push_back(EMPTY);
                        break;
                    case 'Y':
                        start = Position(map.size(), cur.size());
                        cur.push_back(EMPTY);
                        break;
                    case 'E':
                        exit = Position(map.size(), cur.size());
                        cur.push_back(EMPTY);
                        break;
                    case ' ':
                        cur.push_back(EMPTY);
                        break;
                    default:
                        abort(); // FIXME :-)
                }
            map.push_back(cur);
        }
    }

    void print(std::ostream &out, std::list<Position> path_ = {}) const {
        std::set<std::pair<unsigned, unsigned>> path;
        for (auto [x, y]: path_)
            path.insert({x, y});

        for (size_t i = 0; i < map.size(); i++) {
            const auto &row = map[i];
            for (size_t j = 0; j < row.size(); j++) {
                Position p(i, j);
                char c = row[j] == EMPTY ? ' ' : '#';
                if (p == start) c = 'Y';
                if (p == exit) c = 'E';
                if (path.count({i, j})) c = '.';
                std::cout << c;
            }
            std::cout << '\n';
        }
    }
};

std::list<Position> bfs(const Graph &G, Position start, Position exit) {
    std::set<Position> visited;
    std::queue<Position> Q;
    std::map<Position, Position> P;
    Q.push(start);

    while (!Q.empty()) {
        Position v = Q.front();
        Q.pop();

        for (auto &w: G.at(v)) {
            if (visited.count(w) == 0) {
                visited.insert(w);
                Q.push(w);
                P[w] = v;
            }
        }
    }
    if (visited.count(exit) == 0) {
        return {}; // empty list
    }
    std::list<Position> result;
    Position p = exit;
    while (P.count(p) == 1) {
        result.push_front(p);
        p = P[p];
    }
    return result;
};

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Zadejte jméno souboru!" << std::endl;
        return 1;
    }

    Map m(argv[1]);
    m.print(std::cout);

    auto result = bfs(m.to_graph(), m.start, m.exit);
    if (!result.empty()) {
        m.print(std::cout, result);
        std::cout << "Found" << std::endl;
    } else {
        std::cout << "Not found." << std::endl;
    }
    return 1;
}