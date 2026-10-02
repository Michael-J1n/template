#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;
#ifndef ONLINE_JUDGE
#define local(x) x
template<class T,class U>ostream&operator<<(ostream&os,const pair<T,U>&p){return os<<"("<<p.first<<", "<<p.second<<")";}template<class T>ostream&operator<<(ostream&os,const vector<T>&v){os<<"[";for(int i=0;i<v.size();++i){if(i)os<<", ";os<<v[i];}return os<<"]";}template<class T,size_t N>ostream&operator<<(ostream&os,const array<T,N>&a){os<<"[";for(int i=0;i<N;++i){if(i)os<<", ";os<<a[i];}return os<<"]";}template<class T>ostream&operator<<(ostream&os,const set<T>&s){os<<"{";int f=0;for(auto&x:s){if(f)os<<", ";f=1;os<<x;}return os<<"}";}template<class T>ostream&operator<<(ostream&os,const multiset<T>&s){os<<"{";int f=0;for(auto&x:s){if(f)os<<", ";f=1;os<<x;}return os<<"}";}template<class T,class U>ostream&operator<<(ostream&os,const map<T,U>&mp){os<<"{";int f=0;for(auto&x:mp){if(f)os<<", ";f=1;os<<x;}return os<<"}";}
void debug(){cerr<<endl;}template<class T,class... Args>void debug(T x,Args...args){cerr<<x;if(sizeof...(args))cerr<<", ";debug(args...);}
#define dbg(...) cerr<<"Line("<<__LINE__<<") ["<<#__VA_ARGS__<<"] = ",debug(__VA_ARGS__)
#else
#define local(x)
#define dbg(...)
#endif
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
template<class T,class U>using ump=unordered_map<T,U>;template<class T>using minpq=priority_queue<T,vector<T>,greater<T>>;template<class T>using maxpq=priority_queue<T>;
inline ll lowbit(ll x){return x&-x;}inline ll norm(ll x,int mmod){return(x%mmod+mmod)%mmod;}inline ll fdiv(ll a,ll b){ll q=a/b,r=a%b;if(r&&((r>0)!=(b>0)))--q;return q;}inline ll cdiv(ll a,ll b){ll q=a/b,r=a%b;if(r&&((r>0)==(b>0)))++q;return q;}inline ll qpow(ll a,ll b,ll p){ll res=1;a%=p;while(b){if(b&1)res=(i128)res*a%p;a=(i128)a*a%p,b>>=1;}return res;}inline ll qpow(ll a,ll b){if(a==1||b<=0)return 1;return qpow(a,b,mod);}inline void yes(bool ok=true){cout<<(ok?"YES":"NO")<<endl;}inline void no(){cout<<"NO"<<endl;}
template<class T>istream&operator>>(istream&is,vector<T>&v){for(auto& x:v)is>>x;return is;}template<class T>vector<T>build_pre(const vector<T>& a){int n=a.size();vector<T>pre(n);for(int i=0;i<n;++i)pre[i]=(i?pre[i-1]:0)+a[i];return pre;}vvi build_g(int n,int m,bool directed=false){vvi g(n);while(m--){int u,v;cin>>u>>v;--u,--v;g[u].eb(v);if(!directed)g[v].eb(u);}return g;}vector<vii>build_wg(int n,int m,bool directed=false){vector<vii>g(n);while(m--){int u,v,w;cin>>u>>v>>w;--u,--v;g[u].eb(v,w);if(!directed)g[v].eb(u,w);}return g;}
inline void print_i128(i128 x){if(x==0){cout<<'0';return;}if(x<0){cout<<'-';x=-x;}string s;while(x){s.push_back('0'+x%10);x/=10;}reverse(s.begin(),s.end());cout<<s;}inline i128 read_i128(){string s;cin>>s;i128 x=0,sign=1;int i=0;if(s[0]=='-'){sign=-1;i++;}for(;i<s.size();++i)x=x*10+(s[i]-'0');return x*sign;}
struct FastIO{static constexpr int S=1<<20;char ibuf[S],obuf[S];int ipos=0,ilen=0,opos=0;inline static constexpr char digit[]="0001020304050607080910111213141516171819""2021222324252627282930313233343536373839""4041424344454647484950515253545556575859""6061626364656667686970717273747576777879""8081828384858687888990919293949596979899";~FastIO(){flush();}inline char gc(){if(ipos==ilen)ilen=::read(0,ibuf,S),ipos=0;return ibuf[ipos++];}template<class T>inline void read(T& x){char c=gc();while(c!='-'&&(c<'0'||c>'9'))c=gc();bool neg=false;if(c=='-')neg=true,c=gc();using U=make_unsigned_t<T>;U y=0;while(c>='0'&&c<='9')y=y*10+c-'0',c=gc();if constexpr(is_signed_v<T>)x=neg?static_cast<T>(U(0)-y):static_cast<T>(y);else x=static_cast<T>(y);}inline void read(char& c){c=gc();while(c<=' ')c=gc();}inline void read(string& s){char c=gc();while(c<=' ')c=gc();s.clear();while(c>' ')s.push_back(c),c=gc();}template<class T,class...U>inline void read(T& x,U&...y){read(x);(read(y),...);}inline void flush(){if(!opos)return;::write(1,obuf,opos);opos=0;}inline void pc(char c){if(opos==S)flush();obuf[opos++]=c;}template<class T>inline void write(T x,char end='\n'){if(opos>S-64)flush();using U=make_unsigned_t<T>;U y;if constexpr(is_signed_v<T>){if(x<0)obuf[opos++]='-',y=U(0)-U(x);else y=U(x);}else y=x;char s[32];int p=32;while(y>=100){int r=y%100;y/=100;s[--p]=digit[r*2+1],s[--p]=digit[r*2];}if(y<10)s[--p]='0'+y;else s[--p]=digit[y*2+1],s[--p]=digit[y*2];int len=32-p;memcpy(obuf+opos,s+p,len);opos+=len;if(end)obuf[opos++]=end;}inline void write(char c,char end='\n'){pc(c);if(end)pc(end);}inline void write(const string& s,char end='\n'){for(char c:s)pc(c);if(end)pc(end);}}io;template<class...T>inline void read(T&...x){io.read(x...);}template<class T>inline void write(T x,char end='\n'){io.write(x,end);}inline void write(const string& s,char end='\n'){io.write(s,end);}template<class T>inline void read(vector<T>& v){for(auto& x:v)read(x);}

const int MX = 2e5 + 5;
const int MX_LOG = 18;
void init() {
    return;
}
struct Node {
    int v;
    bool operator<(const Node&node)const{
        return v<node.v;
    }
};

/* -------------------------------------------------------- */
void solve() {
    
}
/* -------------------------------------------------------- */

int main() {
    // srand(time(NULL));
    local(clock_t st=clock();)
    // ios::sync_with_stdio(false);cin.tie(nullptr);cout.tie(nullptr);
    // init();
    int tes;
    read(tes);
    while(tes--)solve();
    local(io.flush();cerr<<"Time: "<<1000.0*(clock()-st)/CLOCKS_PER_SEC<<" ms"<<endl;)
    return 0;
}