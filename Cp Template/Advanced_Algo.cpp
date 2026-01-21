// Berlekamp–Massey (BM) is used when you are given the first few terms of a sequence
// (usually modulo a prime like 1e9+7 or 998244353) and the sequence follows an
// unknown linear recurrence.

// It finds the minimal recurrence relation of the form:
// a[n] = c1*a[n-1] + c2*a[n-2] + ... + ck*a[n-k],
// where k is the smallest possible order.

// In ICPC or Codeforces problems, BM is applied when n is too large for DP
// (often up to 1e18), the sequence is implicit or pattern-based, or in polynomial
// and combinatorics optimizations.

// Once the recurrence is known, large terms can be computed efficiently using
// fast exponentiation or linear recurrence techniques.


class BerlekampMassey {
private:
    vector<long long> rec;     // recurrence coefficients
    vector<long long> init;    // initial terms
    int k;                     // order of recurrence

    long long modpow(long long a, long long e) const {
        long long r = 1;
        while (e) {
            if (e & 1) r = r * a % MOD;
            a = a * a % MOD;
            e >>= 1;
        }
        return r;
    }

    vector<long long> combine(const vector<long long>& a,
                              const vector<long long>& b) const {
        vector<long long> r(2 * k);
        for (int i = 0; i < k; i++)
            for (int j = 0; j < k; j++)
                r[i + j] = (r[i + j] + a[i] * b[j]) % MOD;

        for (int i = 2 * k - 1; i >= k; i--)
            for (int j = 1; j <= k; j++)
                r[i - j] = (r[i - j] + r[i] * rec[j - 1]) % MOD;

        r.resize(k);
        return r;
    }

public:
    // Build recurrence from first terms
    void build(const vector<long long>& sequence) {
        init = sequence;

        vector<long long> C(1, 1), B(1, 1);
        long long L = 0, m = 1, b = 1;

        for (int n = 0; n < (int)sequence.size(); n++) {
            long long d = 0;
            for (int i = 0; i <= L; i++)
                d = (d + C[i] * sequence[n - i]) % MOD;

            if (d == 0) {
                m++;
            } else {
                vector<long long> T = C;
                long long coef = d * modpow(b, MOD - 2) % MOD;

                while ((int)C.size() < (int)B.size() + m)
                    C.push_back(0);

                for (int i = 0; i < (int)B.size(); i++)
                    C[i + m] = (C[i + m] - coef * B[i] % MOD + MOD) % MOD;

                if (2 * L > n) {
                    m++;
                } else {
                    L = n + 1 - L;
                    B = T;
                    b = d;
                    m = 1;
                }
            }
        }

        C.erase(C.begin());
        for (long long &x : C) x = (MOD - x) % MOD;
        rec = C;
        k = rec.size();
    }

    // Compute n-th term (0-indexed)
    long long getNth(long long n) const {
        if (n < (long long)init.size()) return init[n];

        vector<long long> pol(k), e(k);
        pol[0] = 1;
        e[1] = 1;

        while (n) {
            if (n & 1) pol = combine(pol, e);
            e = combine(e, e);
            n >>= 1;
        }

        long long res = 0;
        for (int i = 0; i < k; i++)
            res = (res + pol[i] * init[i]) % MOD;

        return res;
    }

    // Print recurrence in readable form
    void printRec() const {
        cout << "Recurrence order: " << k << "\n";
        cout << "a_n = ";
        for (int i = 0; i < k; i++) {
            if (i > 0) cout << " + ";
            cout << rec[i] << " * a_{n-" << i+1 << "}";
        }
        cout << " (mod " << MOD << ")" << endl;
    }
};


// Matrix Exponentiation is used when a problem can be expressed as a fixed linear
// transition between states, and you need to compute the result after a very
// large number of steps.

// It is commonly applied to linear recurrences, DP transitions, graph walks,
// and counting problems where the state at step n depends linearly on previous
// states.

// Instead of simulating n steps, matrix exponentiation raises the transition
// matrix to the power n in O(k^3 log n), where k is the number of states.

class Matrix {
private:
    int n;                       // dimension (n x n)
    vector<vector<long long>> a;

public:
    // Constructor
    Matrix(int _n) {
        n = _n;
        a.assign(n, vector<long long>(n, 0));
    }

    // Accessor for elements
    long long valueAt(int i, int j) {
        return a[i][j];
    }
    
    void setAt(int i,int j,long long val)
    {
        a[i][j] = val;
    }

    // Identity matrix
    static Matrix identity(int n) {
        Matrix I(n);
        for (int i = 0; i < n; i++)
            I.a[i][i] = 1;
        return I;
    }

    // Matrix multiplication with modulo
    Matrix multiply(const Matrix &other, long long mod) const {
        Matrix res(n);
        for (int i = 0; i < n; i++) {
            for (int k = 0; k < n; k++) {
                if (a[i][k] == 0) continue; // optimization
                for (int j = 0; j < n; j++) {
                    res.a[i][j] = (res.a[i][j] + a[i][k] * other.a[k][j] % mod) % mod;
                }
            }
        }
        return res;
    }

    // Matrix exponentiation with modulo
    Matrix power(long long p, long long mod) const {
        if (p == 0) return identity(n);
        if (p == 1) return *this;

        Matrix half = this->power(p / 2, mod);
        Matrix result = half.multiply(half, mod);
        if (p % 2 == 1) result = result.multiply(*this, mod);
        return result;
    }

    // Print matrix (for debugging)
    void print() const {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) cout << a[i][j] << ' ';
            cout << '\n';
        }
    }
};