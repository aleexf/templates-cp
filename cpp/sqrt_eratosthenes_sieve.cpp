typedef unsigned int uint;

const uint BLOCK = (uint)1e4;     // OPTIMAL IN [1e4, 1e5]
const uint SQRTMAX = (uint)2e4+1; // SQRT OF MAXN

int prime[SQRTMAX];
bool block[BLOCK+8];
bool isprime[SQRTMAX];

int iPrime = 0;

void run_sieve() {
    fill(isprime, isprime+SQRTMAX, true);
    isprime[0] = isprime[1] = false;
    for(int i = 2; i * i <= n; ++i) {
        if (!isprime[i]) continue;
        prime[iPrime++] = i;
        if (i * i > n) continue;
        for(int j = i * i; j <= n; j += i) {
            isprime[j] = false;
        }
    }

    for(int bId = 0; ; ++bId) {
        int tl = bId * BLOCK, tr = min<int>(n+1, tl + BLOCK);
        fill(block, block+BLOCK, true);
        for(int i = 0; i < iPrime; ++i) {
            for(int bad = max(2, (tl + prime[i] - 1) / prime[i]) * prime[i]; 
                bad < tr; bad += prime[i]) {

                block[bad - tl] = false;
            }
        }
        if (bId == 0) {
            block[0] = block[1] = false;
        }
        for(int i = tl; i < tr; ++i) {
            if (block[i - tl]) {
                // i is prime
            }
        }
        if (tr >= n) break;
    }
}
// long long!!
