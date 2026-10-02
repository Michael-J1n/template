#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ld = long double;
struct ProbSim {
    mt19937_64 rng;
    ProbSim() {
        rng.seed(chrono::steady_clock::now().time_since_epoch().count());
    }
    ProbSim(unsigned long long seed) {
        rng.seed(seed);
    }
    int randint(int l, int r) {
        uniform_int_distribution<int> dist(l, r);
        return dist(rng);
    }
    ll randll(ll l, ll r) {
        uniform_int_distribution<ll> dist(l, r);
        return dist(rng);
    }
    double uniform(double l = 0.0, double r = 1.0) {
        uniform_real_distribution<double> dist(l, r);
        return dist(rng);
    }
    bool bernoulli(double p) {
        bernoulli_distribution dist(p);
        return dist(rng);
    }
    int binomial(int n, double p) {
        binomial_distribution<int> dist(n, p);
        return dist(rng);
    }
    int geometric(double p) {
        geometric_distribution<int> dist(p);
        return dist(rng);
    }
    int poisson(double lambda) {
        poisson_distribution<int> dist(lambda);
        return dist(rng);
    }
    double exponential(double lambda) {
        exponential_distribution<double> dist(lambda);
        return dist(rng);
    }
    double normal(double mean = 0.0, double stddev = 1.0) {
        normal_distribution<double> dist(mean, stddev);
        return dist(rng);
    }
    double gamma(double alpha, double beta) {
        gamma_distribution<double> dist(alpha, beta);
        return dist(rng);
    }
    double chi_square(double k) {
        chi_squared_distribution<double> dist(k);
        return dist(rng);
    }
    double cauchy(double a = 0.0, double b = 1.0) {
        cauchy_distribution<double> dist(a, b);
        return dist(rng);
    }
    int discrete(vector<double> w) {
        discrete_distribution<int> dist(w.begin(), w.end());
        return dist(rng);
    }
    template<class T>
    T choice(const vector<T>& a) {
        return a[randint(0, a.size() - 1)];
    }
    template<class T>
    void shuffle_vec(vector<T>& a) {
        shuffle(a.begin(), a.end(), rng);
    }
};