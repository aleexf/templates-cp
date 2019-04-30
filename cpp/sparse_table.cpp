int lg[N];
int st[21][N];

void build() {
    int cur = 0;
    rep(i, 2, n+1) {
        if ((i & (i - 1)) == 0) ++cur;
        lg[i] = cur;
    }
    rep(k, 0, 21) rep(i, 0, n) {
        if (k == 0) st[k][i] = a[i];
        else st[k][i] = min(st[k-1][i], st[k-1][i+(1<<(k-1))]);
    }
}

int getmin(int l, int r) {
    int w = lg[r - l + 1];
    return min(st[w][l], st[w][r - (1 << w) + 1]);
}
