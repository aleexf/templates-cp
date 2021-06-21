#include <bits/stdc++.h>
using namespace std;

namespace utils {

vector<int> Unite(const vector<int>& a, const vector<int>& b) {
  vector<int> ret(a.size() + b.size());
  copy(a.begin(), a.end(), ret.begin());
  copy(b.begin(), b.end(), ret.begin() + a.size());
  return ret;
}

bool IsSubset(const vector<int>& what, const vector<int>& where) {
  if (what.size() <= 10) {
    for (int x : what) {
      if (find(where.begin(), where.end(), x) == where.end()) {
        return false;
      }
    }
    return true;
  } else {
    set<int> sWhere(where.begin(), where.end());
    for (int x : what) {
      if (!sWhere.count(x)) {
        return false;
      }
    }
    return true;
  }
}

int FindIndexOf(const vector<int>& container, int x) {
  int id =
      lower_bound(container.begin(), container.end(), x) - container.begin();
  return (id != (int)container.size() && container[id] == x ? id : -1);
}

}  // namespace utils

const int N = (int)2e2 + 7;

int n, m;
bool used[N];
vector<int> g[N];

namespace branch_and_bound {

#define All(x) (x).begin(), (x).end()
#define Remove(vec, x) vec.erase(find(All(vec), (x)))

struct VertexCover {
  VertexCover() { picked_nodes = uncovered_edges = 0; }
  VertexCover(int edges) : uncovered_edges(edges) { picked_nodes = 0; }

  int picked_nodes;
  int uncovered_edges;
};

bool picked[N];
vector<pair<int, int>> events;

inline void RemoveEdge(int x, int y) {
  Remove(g[x], y);
  Remove(g[y], x);
  events.emplace_back(x, y);
}

inline void AddEdge(int x, int y) {
  g[x].push_back(y);
  g[y].push_back(x);
}

void RollBack(int ptr) {
  assert(ptr <= events.size());
  while (ptr < events.size()) {
    AddEdge(events.back().first, events.back().second);
    events.pop_back();
  }
}

void RemoveVertex(int v) {
  while (!g[v].empty()) {
    RemoveEdge(v, g[v].back());
  }
}

void PickVertex(int v, VertexCover& vc) {
  picked[v] = true;
  ++vc.picked_nodes;
  vc.uncovered_edges -= g[v].size();
  RemoveVertex(v);
}

void Reduce(vector<int>& comp, VertexCover& vc) {
  vector<int> to_remove;
  for (int v : comp) {
    if (picked[v]) {
      to_remove.push_back(v);
    }
  }
  for (int v : to_remove) {
    Remove(comp, v);
    PickVertex(v, vc);
  }
  to_remove.clear();
  bool run;
  do {
    //! one-degree
    run = false;
    for (int v : comp) {
      if (g[v].size() != 1) continue;
      run = true;
      int to = g[v][0];
      if (!picked[v]) {
        PickVertex(to, vc);
      }
      to_remove.push_back(v);
      to_remove.push_back(to);
    }
    for (int v : to_remove) {
      Remove(comp, v);
    }
    to_remove.clear();
    //! dominance
    if (run) continue;
    for (int v : comp) {
      auto full_v = utils::Unite(g[v], {v});
      for (int u : g[v]) {
        auto full_u = utils::Unite(g[u], {u});
        if (utils::IsSubset(full_u, full_v)) {
          run = true;
          PickVertex(v, vc);
          to_remove.push_back(v);
          break;
        }
      }
      if (!to_remove.empty()) break;
    }
    for (int v : to_remove) {
      Remove(comp, v);
    }
    to_remove.clear();
  } while (run);
}

void Dfs(int v, set<int>& visited, vector<int>& comp, int& edges) {
  visited.insert(v);
  comp.push_back(v);
  edges += g[v].size();
  for (int to : g[v]) {
    if (!visited.count(to)) {
      Dfs(to, visited, comp, edges);
    }
  }
}

int Solve(vector<int> comp, const vector<int>& order, int ptr, int best,
          VertexCover vc) {
  if (vc.uncovered_edges == 0) {
    return vc.picked_nodes;
  }

  int rollback_ptr = events.size();
  vector<int> old = comp;
  Reduce(comp, vc);

  if (vc.picked_nodes >= best) {
    return best;
  }

  if (vc.uncovered_edges == 0) {
    RollBack(rollback_ptr);
    return vc.picked_nodes;
  }

  sort(All(comp));
  while (ptr < order.size()) {
    auto it = lower_bound(All(comp), order[ptr]);
    if (it == comp.end() || *it != order[ptr]) {
      ++ptr;
      continue;
    }
    break;
  }
  assert(ptr < order.size());
  int v = order[ptr];
  int ret = best;
  //! pick this one
  vector<pair<vector<int>, int>> comps;
  {
    int rollback_ptr = events.size();
    PickVertex(v, vc);
    int res = vc.picked_nodes;

    set<int> visited = {v};
    for (int u : comp) {
      if (visited.count(u)) continue;
      comps.push_back(make_pair(vector<int>(), 0));
      Dfs(u, visited, comps.back().first, comps.back().second);
      comps.back().second /= 2;
    }
    sort(All(comps),
         [&](const pair<vector<int>, int>& a, const pair<vector<int>, int>& b) {
           return a.first.size() < b.first.size();
         });
    for (const auto& cmp : comps) {
      res +=
          Solve(cmp.first, order, ptr + 1, best - res, VertexCover(cmp.second));
    }
    ret = min(ret, res);
    RollBack(rollback_ptr);
    --vc.picked_nodes;
    for (int v : comp) picked[v] = false;
  }
  best = min(best, ret);
  //! dont pick this one
  {
    int res = vc.picked_nodes;
    for (int to : g[v]) picked[to] = true;
    RemoveVertex(v);
    for (const auto& cmp : comps) {
      res +=
          Solve(cmp.first, order, ptr + 1, best - res, VertexCover(cmp.second));
    }
    ret = min(ret, res);
  }
  for (int v : old) {
    picked[v] = false;
  }
  RollBack(rollback_ptr);
  return ret;
}

}  // namespace branch_and_bound

void Dfs(int v, vector<int>& comp, int& edges) {
  used[v] = true;
  comp.push_back(v);
  edges += g[v].size();
  for (int to : g[v]) {
    if (!used[to]) {
      Dfs(to, comp, edges);
    }
  }
}

int Solve() {
  // Read graph from input
  // Assume verticies are 0-indexed
  int n, m;
  cin >> n >> m;
  for (int i = 0; i < m; ++i) {
    int x, y;
    cin >> x >> y;
    g[x].push_back(y);
    g[y].push_back(x);
  }

  vector<int> order(n);
  iota(order.begin(), order.end(), 0);

  sort(order.rbegin(), order.rend(),
       [&](int x, int y) { return g[x].size() < g[y].size(); });

  // Split graph into components and solve the problem
  // for each component independently
  int res = 0;
  for (int v = 0; v < n; ++v) {
    if (used[v]) {
      continue;
    }

    int edges = 0;
    vector<int> comp;
    Dfs(v, comp, edges);
    edges /= 2;
    res += branch_and_bound::Solve(comp, order, 0, comp.size(),
                                   branch_and_bound::VertexCover(edges));
  }
  return res;
}

int32_t main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  Solve();

  return 0;
}
