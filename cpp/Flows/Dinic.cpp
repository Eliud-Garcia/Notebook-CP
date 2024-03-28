#include <bits/stdc++.h>
using namespace std;

#define ln '\n'
#define all(x) x.begin(), x.end()
#define forn(i, n) for(int i = 0; i < n; i++)
#define forab(i, a, b) for (int i = a; i < b; i++)
#define pb push_back
#define sz(x) int(x.size())

typedef long long ll;
typedef vector<int> vi;
typedef vector<bool> vb;
typedef vector<ll> vll;
typedef vector<pair<int, int>> vii;

// Corte minimo: vertices con dist[v]>=0 (del lado de src) VS.  dist[v]==-1 (del lado del dst)
// Para el caso de la red de Bipartite Matching (Sean V1 y V2 los conjuntos mas proximos a src y dst respectivamente):
// Reconstruir matching: para todo v1 en V1 ver las aristas a vertices de V2 con it->f>0, es arista del Matching
// Min Vertex Cover: vertices de V1 con dist[v]==-1 + vertices de V2 con dist[v]>0
// Max Independent Set: tomar los vertices NO tomados por el Min Vertex Cover
// Max Clique: construir la red de G complemento (debe ser bipartito!) y encontrar un Max Independet Set
// Min Edge Cover: tomar las aristas del matching + para todo vertices no cubierto hasta el momento, tomar cualquier arista de el
// Complexity O(V^2*E)

const ll inf = 1e18;
struct edge {
  int to, rev; ll cap, f{0};
  edge(int to, int rev, ll cap): to(to), rev(rev), cap(cap){}
};

struct Dinic{
  int n, s, t; ll max_flow = 0;
  vector<vector<edge>> g;
  vi q, dis, work;
  Dinic(int n): n(n), s(n-2), t(n-1), g(n), q(n){}
  Dinic(int n, int s, int t): n(n), s(s), t(t), g(n), q(n){}
  void addEdge(int s, int t, ll cap){
    g[s].pb(edge(t, sz(g[t]), cap));
    g[t].pb(edge(s, sz(g[s])-1, 0));
  }
  
  bool bfs(){
    dis.assign(n, -1), dis[s] = 0;
    int qt = 0;
    q[qt++] = s;
    forn(qh, qt){
      int u = q[qh];
      for(auto& [v, _, cap, f]: g[u])
        if(dis[v] < 0 && f < cap) dis[v] = dis[u] + 1, q[qt++] = v;
    }
    return dis[t] >= 0;
  }
  ll dfs(int u, ll cur){
    if(u == t) return cur;
    for(int& i = work[u]; i < sz(g[u]); ++i){
      auto& [v, rev, cap, f] = g[u][i];
      if(cap <= f) continue;
      if(dis[v] == dis[u] + 1){
        ll df = dfs(v, min(cur, cap - f));
        if(df > 0){
          f += df, g[v][rev].f -= df;
          return df;
        }
      }
    }
    return 0;
  }
  ll maxFlow(){
    ll cur_flow = 0;
    while(bfs()){
      work.assign(n, 0);
      while(ll delta = dfs(s, inf)) cur_flow += delta;
    }
    max_flow += cur_flow;
    // todos los nodos con dis[u]!=-1 vs los que tienen dis[v]==-1 forman el min-cut, (u,v)
    return max_flow; 
  }
  vii min_cut(){
    maxFlow();
    vii cut;
    forn(u, n){
      if(dis[u] == -1) continue;
      for(auto& e: g[u]) if(dis[e.to] == -1) cut.pb({u, e.to});
    }
    sort(all(cut)),  cut.resize(unique(all(cut)) - cut.begin());
    return cut;
  }
};
 
int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    string s; 
    int n;
    cin >> s >> n;
    vector<set<char>> cubes(n);
    forn(i, n){
        forn(j, 6){
            char c; cin >> c;
            cubes[i].insert(c);
        }
    }
    int nodes = sz(s) + n + 2; //cantidad nodos
    Dinic dd(nodes);
    // S -> s[i]
    forn(i, sz(s)) dd.addEdge(dd.s, i, 1);
    //cubes[i] -> T
    forn(i, n) dd.addEdge(sz(s) + i, dd.t, 1);
    forn(i, sz(s)){
        forn(j, n){
            if(cubes[j].count(s[i])){
                dd.addEdge(i, sz(s) + j, 1);
            }
        }
    }
    ll flow = dd.maxFlow();
    cout << (flow == sz(s) ? "YES" : "NO") <<ln;
    return 0;
}