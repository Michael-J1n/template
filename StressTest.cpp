#include<bits/stdc++.h>
using namespace std;
#define ep emplace
#define eb emplace_back
#define endl '\n'
#define all(x) (x).begin(),(x).end()
#define rall(x) (x).rbegin(),(x).rend()
#define srt(x) sort((x).begin(),(x).end())
#define rsrt(x) sort((x).begin(),(x).end(),greater<>())
#define rep1(n) for(int _=0;_<(n);++_)
#define rep2(i,n) for(int i=0;i<(n);++i)
#define rep3(i,l,r) for(int i=(l);i<(r);++i)
#define rrep1(n) for(int _=(n);_--;)
#define rrep2(i,n) for(int i=(n);i--;)
#define rrep3(i,l,r) for(int i=(r);i-->(l);)
#define pick(_1,_2,_3,f,...) f
#define rep(...) pick(__VA_ARGS__,rep3,rep2,rep1)(__VA_ARGS__)
#define rrep(...) pick(__VA_ARGS__,rrep3,rrep2,rrep1)(__VA_ARGS__)
static constexpr int INF = 0x3f3f3f3f;
static constexpr long long LINF = 4e18;
static constexpr int LOG = 31;
static constexpr long long LLOG = 63;
static constexpr int mod = 1e9 + 7;
static constexpr int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};  // {'U','R','D','L'}
using i128=__int128_t;using ll=long long;using pii=pair<int,int>;using pll=pair<ll,ll>;using pli=pair<ll,int>;using vi=vector<int>;using vll=vector<ll>;using vii=vector<pii>;using vvi=vector<vi>;using tii=tuple<int,int,int>;

/* ------------------- Random Generator ------------------- */
using ull=unsigned long long;
const ull SEED=chrono::steady_clock::now().time_since_epoch().count();
mt19937_64 rng(SEED);
ll rnd(ll l,ll r){return uniform_int_distribution<ll>(l,r)(rng);}

/* ------------------- Stress Config ------------------- */
const int N_MIN=1,N_MAX=10;
const int A_MIN=-10,A_MAX=10;
const int T=100000;

struct Test{
    int n;
    vi a;
};

Test gen(){
    Test t;
    t.n=rnd(N_MIN,N_MAX);
    t.a.resize(t.n);
    rep(i,t.n)t.a[i]=rnd(A_MIN,A_MAX);
    return t;
}

void print(const Test&t){
    cout<<t.n<<endl;
    rep(i,t.n)cout<<t.a[i]<<" \n"[i==t.n-1];
}

/* --------------------- Solution ------------------------ */
ll brute(const Test&t){
    int n=t.n;
    vi a=t.a;
    return 0;
}

ll solve(const Test&t){
    int n=t.n;
    vi a=t.a;
    return 0;
}

/* -------------------------------------------------------- */
int main(){
    ios::sync_with_stdio(false);cin.tie(nullptr);
    rep(tc,1,T+1){
        Test t=gen();
        ll x=solve(t),y=brute(t);
        if(x!=y){
            cout<<"WA on test "<<tc<<endl;
            cout<<"Seed: "<<SEED<<endl;
            cout<<"Input:"<<endl;
            print(t);
            cout<<"Solve: "<<x<<endl;
            cout<<"Brute: "<<y<<endl;
            return 0;
        }
        if(tc%10000==0)cerr<<"Passed "<<tc<<endl;
    }
    cout<<"All tests passed! Seed: "<<SEED<<endl;
}
