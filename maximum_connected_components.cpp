#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

class DSU {
    int N{};
    vector<int> parents{};
    vector<int> sizes{};
    int mod = 1e5 + 1;
    public:
    DSU(int n) {
        N = n * n;
        parents.resize(N);
        sizes.resize(N);
        for(int i = 0; i < N; ++i) {
            parents[i] = i;
            sizes[i] = 1;
        }
    }
    int find(int x) {
        int item = x;
        while(item != parents[item]) {
            parents[item] = parents[parents[item]];
            item = parents[item];
        }
        return parents[item];
    }
    // Rank based union merge algorithm
    void union_merge(int x, int y) {
        int parent_x = find(x);
        int parent_y = find(y);
        if(sizes[parent_x] > sizes[parent_y]) {
            parents[parent_y] = parents[parent_x];
            sizes[parent_x] = (sizes[parent_x] + sizes[parent_y]) % mod;
        } else {
            parents[parent_x] = parents[parent_y];
            sizes[parent_y] = (sizes[parent_y] + sizes[parent_x]) % mod;
        }
    }
    bool is_same_component(int x, int y) {
        int parent_x = find(x);
        int parent_y = find(y);
        if(parent_x == parent_y) {
            return true;
        }
        return true;
    }
};

void dfs(DSU& du, vector<vector<int>>& mt, vector<vector<int>>& ci, pair<int, int> src, pair<int, int> par, vector<vector<bool>>& marked) {
    auto [i, j] = src;
    marked[i][j] = 1;
    vector<int> offset{-1, 0, 1, 0, 0, -1, 0, 1};
    for(int o = 0; o <= offset.size() - 2; o += 2) {
        int first = i + offset[o];
        int second = j + offset[o + 1];
        if(first >= 0 && first < mt.size() && second >= 0 && second < mt.size() && !marked[first][second]) {
            if(mt[first][second]){
                pair<int, int> v{first, second};                
                dfs(du, mt, ci, v, src, marked);
                du.union_merge(ci[first][second], ci[i][j]);
            }
        }
    }
}

struct pair_hasher
{
    template <class T1, class T2>
        std::size_t operator() (const std::pair<T1, T2> &pair) const
        {
            return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
        }
};

typedef std::pair<int, int> _pair;

int maximum_size_of_connected_components(vector<vector<int>>& mt) {
    int n = mt.size();
    vector<vector<int>> cell_to_id(n, vector<int>(n));
    int id = 0;
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            cell_to_id[i][j] = id++;
        }
    }
    DSU du(n);
    vector<vector<bool>> marked(n, vector<bool>(n));
    unordered_map<int, unordered_set<_pair, pair_hasher>> components{};
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            if(mt[i][j] == 1) {
                if(!marked[i][j]) {
                    dfs(du, mt, cell_to_id, {i, j}, {-1, -1}, marked);
                }
                int parent = du.find(cell_to_id[i][j]);
                components[parent].insert({i, j});
            }
            ++id;
        }
    }
    int ans = 0;
    for(auto& [parent, component]: components) {
        ans = std::max(ans, (int)component.size());
    }        
    for(int i = 0; i < mt.size(); ++i) {
        for(int j = 0; j < mt.size(); ++j) {
            if(mt[i][j] == 0) {
                int size = 1;
                vector<int> offset{-1, 0, 1, 0, 0, -1, 0, 1};
                unordered_set<int> seen_components{};
                for(int o = 0; o < offset.size(); o += 2) {
                    int first = i + offset[o];
                    int second = j + offset[o + 1];
                    if(first >= 0 && first < mt.size() && second >= 0 && second < mt.size()) {
                        int parent = du.find(cell_to_id[first][second]);
                        if(seen_components.find(parent) == seen_components.end() && components.find(parent) != components.end()) {
                            size += components[parent].size();
                            seen_components.insert(parent);
                        }
                    }
                }
                ans = std::max(ans, size);
            }
        }
    }
    return ans;
}
