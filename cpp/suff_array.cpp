namespace SuffixArray {
    const int ALPHABET = 127;
    int letter(int x) { return x; }
 
    int n;
    int g;
    vector<int> p;
    vector<int> c;
    vector<int> lcp;
    vector<vector<int>> all;
 
    void build(const string& s) {
        n = s.length();
        p.resize(n);
        c.resize(n);
        lcp.resize(n-1);
        vector<int> cnt(max(n, ALPHABET));
        for(char ch : s) ++cnt[ch];
        rep(i, 1, ALPHABET) cnt[i] += cnt[i-1];
        rep(i, 0, n) p[--cnt[s[i]]] = i;
        g = 0;
        rep(i, 1, n) {
            if (s[p[i-1]] != s[p[i]]) ++g;
            c[p[i]] = g;
        }
        all.push_back(c);
        vector<int> tp(n), tc(n);
        for(int size = 1; size < n+n; size <<= 1) {
            cnt.assign(g+1, 0);
            rep(i, 0, n) {
                tp[i] = (p[i] - size + n + n) % n;
                ++cnt[c[tp[i]]];
            }
            rep(i, 1, g+1) cnt[i] += cnt[i-1];
            per(i, 0, n) p[--cnt[c[tp[i]]]] = tp[i];
            g = 0;
            tc[p[0]] = 0;
            rep(i, 1, n) {
                int m1 = (p[i] + size) % n;
                int m2 = (p[i-1] + size) % n;
                if (c[m1] != c[m2] || c[p[i]] != c[p[i-1]]) {
                    ++g;
                }
                tc[p[i]] = g;
            }
            c.swap(tc);
            all.push_back(c);
        }
        vector<pair<int,int>> pr;
        rep(i, 0, n) pr.emplace_back(c[i], i);
        sort(all(pr));
        rep(i, 0, n-1) {
            int x = pr[i].snd;
            int y = pr[i + 1].snd;
            int& res = lcp[i];
            per(j, 0, all.size()) {
                if (all[j][x] == all[j][y]) {
                    res += (1 << j);
                    x = (x + (1<<j)) % n;
                    y = (y + (1<<j)) % n;
                }
            }
            umin(res, n);
        }
    }
};


 