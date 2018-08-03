int sp[21][N];
int lg[N], pot[21];

void prepare() {
    rep(i, 0, n) pot[i] = (1<<i);
    int cur = 0;
    rep(i, 1, n+1) {
        if ((1<<cur) == i) cur++;
        lg[i] = cur-1;
    }
    rep(log, 0, 21) {
        rep(i, 0, n) {
            if (log == 0) sp[log][i] = a[i];
            else sp[log][i] = min(sp[log-1][i], sp[log-1][i+pot[log-1]]);
        }
    }
}

int getmin(int l, int r) {
    --l, --r;
    int log = lg[r-l+1];
    return min(sp[log][l], sp[log][r-pot[log]+1]);
}