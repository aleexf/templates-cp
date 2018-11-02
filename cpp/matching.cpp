namespace matching {
    int n, k;
    int mt[N];
    bool in[N];
    bool used[N];
    vector<int> g[N];
    bool dfs(int v) {
        if (used[v]) return false;
        used[v] = true;
        for(int to:g[v]) {
            if (mt[to] == -1) {
                mt[to] = v;
                in[v] = true;
                return true;
            }
        }
        for(int to:g[v]) {
            if (dfs(mt[to], mrk)) {
                mt[to] = v;
                in[v] = true;
                return true;
            }
        }
        return false;
    }
    int run() {
        fill(mt, mt+k, -1);
        bool run = true;
        while (run) {
            run = false;
            fill(used, used+n, false);
            rep(i, 0, n) {
                if (in[i] == false && dfs(i)) {
                    run = true;
                }
            }
        }
        int result = 0;
        rep(i, 0, k) {
            result += (mt[i] != -1);
        }
        return result;
    }
    inline void init(int _n, int _k) {
        n = _n, k = _k;
    }
    inline void addEdge(int x, int y) {
        g[x].push_back(y);
    }
}
