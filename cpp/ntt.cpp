const int mod = 998244353;

inline int mul(int a, int b) {
    return 1ll * a * b % mod;
}

inline int binpow(int x, int y) {
    int ret = 1;
    while (y > 0) {
        if (y % 2 == 1) ret = mul(ret, x);
        x = mul(x, x);
        y /= 2;
    }
    return ret;
}

inline int inv(int a) {
    return binpow(a, mod-2);
}

namespace ntt {
    int base = 1;
    vector<int> roots = {0, 1};
    vector<int> rev = {0, 1};
    int max_base = -1;
    int root = -1;

    void init() {
        int tmp = mod - 1;
        max_base = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            max_base++;
        }
        root = 2;
        while (true) {
            if (binpow(root, 1 << max_base) == 1) {
                if (binpow(root, 1 << (max_base - 1)) != 1) {
                    break;
                }
            }
            root++;
        }
    }

    void ensure_base(int nbase) {
        if (max_base == -1) {
            init();
        }
        if (nbase <= base) {
            return;
        }
        assert(nbase <= max_base);
        rev.resize(1 << nbase);
        for (int i = 0; i < (1 << nbase); i++) {
            rev[i] = (rev[i >> 1] >> 1) + ((i & 1) << (nbase - 1));
        }
        roots.resize(1 << nbase);
        while (base < nbase) {
            int z = binpow(root, 1 << (max_base - 1 - base));
            for (int i = 1 << (base - 1); i < (1 << base); i++) {
                roots[i << 1] = roots[i];
                roots[(i << 1) + 1] = mul(roots[i], z);
            }
            base++;
        }
    }

    void fft(vector<int>& a) {
        int n = (int) a.size();
        assert((n & (n - 1)) == 0);
        int zeros = __builtin_ctz(n);
        ensure_base(zeros);
        int shift = base - zeros;
        for (int i = 0; i < n; i++) {
            if (i < (rev[i] >> shift)) {
                swap(a[i], a[rev[i] >> shift]);
            }
        }
        for (int k = 1; k < n; k <<= 1) {
            for (int i = 0; i < n; i += 2 * k) {
                for (int j = 0; j < k; j++) {
                    int x = a[i + j];
                    int y = mul(a[i + j + k], roots[j + k]);
                    a[i + j] = x + y - mod;
                    if (a[i + j] < 0) a[i + j] += mod;
                    a[i + j + k] = x - y + mod;
                    if (a[i + j + k] >= mod) a[i + j + k] -= mod;
                }
            }
        }
    }

    vector<int> multiply(vector<int> a, vector<int> b, int eq = 0) {
        int need = (int)(a.size() + b.size() - 1);
        int nbase = 0;
        while ((1 << nbase) < need) nbase++;
        ensure_base(nbase);
        int sz = 1 << nbase;
        a.resize(sz);
        b.resize(sz);
        fft(a);
        if (eq) b = a;
        else fft(b);
        int inv_sz = inv(sz);
        for (int i = 0; i < sz; i++) {
            a[i] = mul(mul(a[i], b[i]), inv_sz);
        }
        reverse(a.begin() + 1, a.end());
        fft(a);
        a.resize(need);
        while (!a.empty() && a.back() == 0) {
            a.pop_back();
        }
        return a;
    }

    vector<int> square(vector<int> a) {
        return multiply(a, a, 1);
    }
};