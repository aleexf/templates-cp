namespace bridge {
    bool used[N];
    int in[N], fup[N];
    
    void dfs(int v, int p) {
        static int timer = 0;
        used[v] = true;
        in[v] = fup[v] = ++timer;
        for(int to:g[v]) {
            if (to == p) continue;
            if (used[to]) umin(fup[v], in[to]);
            else {
                dfs(to, v);
                umin(fup[v],fup[to]);
                if (fup[to] > in[v]) ITS_BRIDGE(v, to);
            }
        }
    }
    void find() {
        rep(i, 0, n) if(!used[i]) dfs(i,-1);
    }
}
