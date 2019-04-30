namespace centriod {
    int sz[N];
    bool used[N];
    int dfs(int v, int p) {
        sz[v] = 1;
        for(int to : g[v]) {
            if (to == p || used[to]) continue;
            sz[v] += dfs(to, v);
        }
        return sz[v];
    }
    int find(int v) {
        bool run = true;
        int N = sz[v], p = -1;
        while (run) {
            run = false;
            for(int to : g[v]) {
                if(to == p || used[to]) continue;
                if (sz[to] > N/2) {
                    p = v;
                    v = to;
                    run = true;
                    break;
                }
            }
        }
        return v;
    }
    void solve(int v) {
        dfs(v, -1);
        v = find(v);
        // solve here
        used[v] = true;
        for(int to : g[v]) if (!used[to]) solve(to);
    }
}
