namespace lca {
    int up[N][21];
    int in[N], out[N];
    void dfs(int v, int p) {
        static int timer = 0;
        in[v] = ++timer;
        up[v][0] = p;
        rep(i, 1, 21) up[v][i] = up[up[v][i-1]][i-1];
        for(int to:g[v]) {
            if (to == p) continue;
            dfs(to, v);
        }
        out[v] = ++timer;
    }
    bool upper(int x, int y) {
        return in[x] < in[y] && out[y] < out[x];
    }
    int get(int x, int y) {
        if (x == y) return x;
        if (upper(x, y)) return x;
        if (upper(y, x)) return y;
        per(i, 0, 21) {
            if (!upper(up[x][i], y)) x = up[x][i];
        }
        return up[x][0];
    }
}
