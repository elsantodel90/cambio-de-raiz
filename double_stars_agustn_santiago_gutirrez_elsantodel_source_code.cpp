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
#define ALL(c) begin(c), end(c)
#define esta(x,c) ((c).find(x) != (c).end())

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







// ********* Esta es la parte especifica de este problema *****************

struct Data
{
    vector<int> h; // Alturas de mayor a menor
};

Data combine(const Data &a, const Data &b)
{
    Data ret;
    int i=0, j = 0;
    while (i < SIZE(a.h) || j < SIZE(b.h))
    {
        if (i == SIZE(a.h))
            ret.h.push_back(b.h[j++]);
        else if (j == SIZE(b.h))
            ret.h.push_back(a.h[i++]);
        else if (a.h[i] > b.h[j])
            ret.h.push_back(a.h[i++]);
        else
            ret.h.push_back(b.h[j++]);
    }
    assert(SIZE(ret.h) == SIZE(a.h) + SIZE(b.h));
    return ret;
}

Data extend(const Data &a)
{
    Data ret;
    if (a.h.empty())
        ret.h.push_back(1);
    else
        ret.h.push_back(a.h[0]+1);
    return ret;
}

Data leaf()
{
    return Data();
}

struct Edge
{
    int child;
};

// ********* Esto seria la tecnica de cambio de raiz general, independiente del problema *****************

const int MAXN = 1100000;

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
        extendedSubtree[e.child] = extend(subtree[e.child]);
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
        extendedParentSubtree[EDGE.child] = extend(parentSubtree[EDGE.child]);
        dfsParentSubtree(EDGE.child, node);
    }
}



















int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    int TT; cin >> TT;
	forn(tt,TT)
	{
        int N; cin >> N;
        forn(i,N)
            g[i].clear();
        forn(i,N-1)
        {
            int p;
            cin >> p;
            p--;
            g[p].push_back({i+1});
            g[i+1].push_back({p});
        }
        
        dfsSubtree(0, -1);
        dfsParentSubtree(0, -1);
        
        tint ret = 0;
        forsn(node,1, N)
        {
            const Data &d1 = parentSubtree[node];
            const Data &d2 = subtree[node];
            forn(i, min(SIZE(d1.h), SIZE(d2.h)))
                ret += min(d1.h[i], d2.h[i]);
        }
        cout << "Case #" << tt+1 << ": " << ret << "\n";
	}
    
    return 0;
}
