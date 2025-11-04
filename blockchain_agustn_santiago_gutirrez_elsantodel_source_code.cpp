// Solucion al problema de Hacker Cup 2021, Round 1, Problem C
// Link: https://www.facebook.com/codingcompetitions/hacker-cup/2021/round-1/problems/C

#include <iostream>
#include <algorithm>
#include <cassert>
#include <vector>
#include <stack>
#include <set>
#include <map>
#include <queue>
#include <string>
#include <cstring>

using namespace std;

#define forn(i,n) for(int i=0;i<int(n);i++)
#define forsn(i,s,n) for(int i=int(s);i<int(n);i++)
#define dforn(i,n) for(int i=int(n)-1;i>=0;i--)
#define dforsn(i,s,n) for(int i=int(n)-1;i>=int(s);i--)

#define DBG(x) do {cerr << #x << " = " << (x) << endl;} while (false)
#define DBGY(x) do {cerr << #x << " = " << (x) << " , ";} while (false)
#define DBG2(x,y) do {DBGY(x); DBG(y); } while (false)
#define DBG3(x,y,z) do {DBGY(x); DBGY(y); DBG(z); } while (false)
#define DBG4(x,y,z,w) do {DBGY(x); DBGY(y); DBGY(z); DBG(w); } while (false)
#define RAYA do {cerr << " ================ " << endl; } while (false)

#define SIZE(c) int((c).size())
#define esta(x,c) ((c).find(x) != (c).end())
#define all(c) (c).begin(), (c).end()

typedef long long tint;
typedef pair<int,int> pint;
typedef pair<tint,tint> ptint;

template <typename T>
ostream & operator <<(ostream &os, const vector<T>& v) {
    os << "[";
    forn(i,v.size())
    {
        if (i > 0) os << " ";
        os << v[i];
    }
    return os << "]";
}

template <typename T>
istream & operator >>(istream &is, vector<T>& v) {
    int n;
    is >> n;
    v.resize(n);
    forn(i,n)
        is >> v[i];
    return is;
}

// ********* Esta es la parte especifica de este problema *****************

const int MAX_CAP = 20;

struct Data
{
    tint cant[MAX_CAP+1]; // Cantidad de caminos a la raiz, por cada capacidad
    tint S; // Suma sobre todos los pares
};

Data combine(const Data &a, const Data &b)
{
    Data ret;
    ret.S = a.S + b.S;
    forn(i,MAX_CAP+1)
    forn(j,MAX_CAP+1)
        ret.S += a.cant[i] * b.cant[j] * min(i,j);
    forn(i,MAX_CAP+1)
        ret.cant[i] = a.cant[i] + b.cant[i];
    return ret;
}

Data extend(const Data &a, int value)
{
    assert(0 <= value);
    assert(value <= MAX_CAP);
    Data ret;
    forn(i,value)
        ret.cant[i] = a.cant[i];
    forsn(i,value, MAX_CAP+1)
        ret.cant[i] = 0;
    forsn(i,value, MAX_CAP+1)
        ret.cant[value] += a.cant[i];
    ret.cant[value]++;
    ret.S = a.S;
    forn(i, MAX_CAP+1)
        ret.S += ret.cant[i] * i;
    return ret;
}

Data leaf()
{
    Data ret;
    ret.S = 0;
    forn(i,MAX_CAP+1)
        ret.cant[i] = 0;
    return ret;
}

struct Edge
{
    int child;
    int value;
};

// ********* Esto seria la tecnica de cambio de raiz general, independiente del problema *****************

const int MAXN = 1000000;

vector<Edge> g[MAXN];
Data subtree[MAXN]; // El clasico subarbol con raiz en el nodo
Data extendedSubtree[MAXN]; // El clasico + la arista al padre
Data parentSubtree[MAXN]; // El "complemento" en aristas del extended
Data extendedParentSubtree[MAXN]; // El "complemento" en aristas del subtree

void dfsSubtree(int node, int parent)
{
    subtree[node] = leaf();
    for (Edge e : g[node])
    if (e.child != parent)
    {
        dfsSubtree(e.child, node);
        extendedSubtree[e.child] = extend(subtree[e.child], e.value);
        subtree[node] = combine(extendedSubtree[e.child], subtree[node]);
    }
}

void dfsParentSubtree(int node, int parent)
{
    const int D = SIZE(g[node]);
    vector<Data> prefixes(D+1);
    vector<Data> suffixes(D+1);
    
    #define EDGE g[node][i]
    #define ELEM (EDGE.child == parent ? extendedParentSubtree[node] : extendedSubtree[EDGE.child])
    
    prefixes[0] = leaf();
    forn(i, D)
        prefixes[i+1] = combine(ELEM, prefixes[i]);
    
    suffixes[D] = leaf();
    dforn(i, D)
        suffixes[i] = combine(ELEM, suffixes[i+1]);
    
    // En este punto, prefixes[D] == suffixes[0] == el valor con este nodo como raiz
    
    forn(i, D)
    if (EDGE.child != parent)
    {
        parentSubtree[EDGE.child] = combine(prefixes[i], suffixes[i+1]);
        extendedParentSubtree[EDGE.child] = extend(parentSubtree[EDGE.child], EDGE.value);
        dfsParentSubtree(EDGE.child, node);
    }
}

const tint MOD = 1000000007;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int TT; cin >> TT;
	forn(tt,TT)
	{
        int N; cin >> N;
        assert(2 <= N);
        assert(N < MAXN);
        forn(i,N)
            g[i].clear();
        forn(i,N-1)
        {
            int a,b,value;
            cin >> a >> b >> value;
            a--; b--;
            assert(0 <= value);
            assert(value <= MAX_CAP);
            g[a].push_back({b, value});
            g[b].push_back({a, value});
        }
        dfsSubtree(0, -1);
        dfsParentSubtree(0, -1);
        tint ret = 1;
        forsn(i, 1, N)
        {
            ret *= (parentSubtree[i].S + subtree[i].S) % MOD;
            ret %= MOD;
        }
        cout << "Case #" << tt+1 << ": " << ret << "\n";
	}
    
    return 0;
}
