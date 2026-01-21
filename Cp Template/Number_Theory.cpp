// LCM and GCD
#include <bits/stdc++.h>
using namespace std;

// GCD using Euclidean algorithm
long long gcd(long long a, long long b) {
    while (b) {
        a %= b;
        swap(a, b);
    }
    return a;
}

// LCM using GCD
long long lcm(long long a, long long b) {
    return a / gcd(a, b) * b;
}

// Extended Euclidian 
// Returns gcd(a,b) and finds x, y such that a*x + b*y = gcd(a,b)
long long extGCD(long long a, long long b, long long &x, long long &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    long long x1, y1;
    long long g = extGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}
// Big Mod
// Modular exponentiation (a^b % mod)
long long mod_pow(long long a, long long b) {
    long long res = 1;
    a %= MOD;
    while (b > 0) {
        if (b & 1) res = res * a % MOD;
        a = a * a % MOD;
        b >>= 1;
    }
    return res;
}
long long binpow(long long a, long long b, long long mod) {
    if (b == 0) return 1 % mod;
    long long half = binpow(a, b / 2, mod);
    long long res = (half * half) % mod;
    if (b % 2) res = (res * a) % mod;
    return res;
}

// Modular inverse using Fermat's little theorem (MOD must be prime)
long long mod_inv(long long a) {
    return mod_pow(a, MOD-2);
}
// sieve 
vector<int> sieve(int n) {
    vector<int> primes;
    vector<bool> is_prime(n + 1, true);

    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i <= n; i++) {
        if (is_prime[i]) {
            for (int j = i * i; j <= n; j += i)
                is_prime[j] = false;
        }
    }

    for (int i = 2; i <= n; i++) {
        if (is_prime[i])
            primes.push_back(i);
    }

    return primes;
}
// factorization 
const int N = 1000000; // limit
int spf[N + 1]; // spf[x] = smallest prime factor of x

void sieve_spf() {
    for (int i = 1; i <= N; i++) spf[i] = i;

    for (int i = 2; i * i <= N; i++) {
        if (spf[i] == i) { // i is prime
            for (int j = i * i; j <= N; j += i)
                if (spf[j] == j) spf[j] = i;
        }
    }
}
vector<int> getFactors(int x) {
    vector<int> factors;
    while (x != 1) {
        factors.push_back(spf[x]);
        x /= spf[x];
    }
    return factors;
}

// ===============================
// Purpose:
// This code computes two number-theoretic functions efficiently:
// 1. Euler's Totient function (phi): phi[n] = number of integers <= n that are coprime with n.
// 2. Function F[n] = sum_{d|n} (d * phi(n/d)), which is often used in problems
//    related to gcd sums, divisor sums, and multiplicative functions.
// 
// How it works:
// - calc_phi: Uses a sieve-like method to compute phi for all numbers up to n.
// - calc_F: Uses the divisor summation formula to compute F for all numbers up to n.
// ===============================

// Phi
void calc_phi(int n, vector<long long>& phi) {
    phi.resize(n + 1);
    for (int i = 1; i <= n; i++) phi[i] = i;

    for (int i = 2; i <= n; i++) {
        if (phi[i] == i) { // i is prime
            for (int j = i; j <= n; j += i) {
                phi[j] -= phi[j] / i;
            }
        }
    }
}
// F
void calc_F(int n, const vector<long long>& phi, vector<long long>& F) {
    F.assign(n + 1, 0);

    for (int i = 1; i <= n; i++) {
        for (int j = i; j <= n; j += i) {
            F[j] += 1LL * i * phi[j / i];
        }
    }
}

// nCr
const int MAXN = 1e6;
long long fact[MAXN], invfact[MAXN];
const long long MOD = 1e9+7;

void init() {
    fact[0] = 1;
    for (int i = 1; i < MAXN; i++)
        fact[i] = fact[i-1] * i % MOD;

    invfact[MAXN-1] = modpow(fact[MAXN-1], MOD-2, MOD);
    for (int i = MAXN-2; i >= 0; i--)
        invfact[i] = invfact[i+1] * (i+1) % MOD;
}

long long nCr(int n, int r) {
    if (r < 0 || r > n) return 0;
    return fact[n] * invfact[r] % MOD * invfact[n-r] % MOD;
}

//chinese Reminder Theorem
// Solve x ≡ a[i] (mod m[i]) for all i
long long CRT(const vector<long long>& a, const vector<long long>& m) {
    long long M = 1;
    for (auto x : m) M *= x;
    long long x = 0;
    for (int i=0;i<a.size();i++) {
        long long Mi = M / m[i];
        long long inv;
        extGCD(Mi, m[i], inv, inv);
        inv = (inv % m[i] + m[i]) % m[i];
        x = (x + a[i] * Mi % M * inv % M) % M;
    }
    return (x + M) % M;
}

// fast io
ios::sync_with_stdio(false);
cin.tie(nullptr);