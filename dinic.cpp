namespace flow {
    struct edge {
        ll to, cap, flow;
        edge() {}
        edge(int to, ll cap):
            to(to), cap(cap) {flow = 0;}
    };
    ll n, s, t, d[N], ptr[N];
    vector<edge> e;
    vector<int> g[N];
    void addEdge(int v, int u, ll cap) {
        g[v].push_back(e.size());
        g[u].push_back(e.size()+1);
        e.push_back(edge(u, cap));
        e.push_back(edge(v, 0));
    }
    bool bfs() {
        queue<int> q;
        fill(d, d+n, INF);
        d[s] = 0;
        q.push(s);
        while (!q.empty() && d[t] == INF) {
            int v = q.front();
            q.pop();
            for(int id:g[v]) {
                int to = e[id].to;
                //cout << v << ' ' << to << ' '  << d[v] << ' ' << d[to] << endl;
                if (e[id].cap > e[id].flow && umin(d[to], d[v]+1)) {
                    q.push(to);
                }
            }
        }
        return d[t] != INF;
    }
    ll dfs(int v, ll flow) {
        if (flow == 0 || v == t) return flow;
        for(; ptr[v] < g[v].size(); ++ptr[v]) {
            int id = g[v][ptr[v]], to = e[id].to;
            if (d[to] != d[v]+1) continue;
            ll push = dfs(to, min(flow, e[id].cap-e[id].flow));
            if (push != 0) {
                e[id].flow += push;
                e[id^1].flow -= push;
                return push;
            }
        }
        return 0;
    }
    ll dinic() {
        ll flow = 0;
        while (true) {
            if (!bfs()) break;
            fill(ptr, ptr+n, 0);
            ll push = 0;
            do {
                push = dfs(s, INF);
                flow += push;
            } while(push != 0);
        }
        return flow;
    }
}