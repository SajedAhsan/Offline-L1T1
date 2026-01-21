//DFS
vector<int> g[200005];
bool vis[200005];

void dfs(int u) {
    vis[u] = true;
    for (int v : g[u]) {
        if (!vis[v]) dfs(v);
    }
}
//BFS
queue<int> q;
vector<int> dist(n+1, -1);

dist[src] = 0;
q.push(src);

while (!q.empty()) {
    int u = q.front(); q.pop();
    for (int v : g[u]) {
        if (dist[v] == -1) {
            dist[v] = dist[u] + 1;
            q.push(v);
        }
    }
}
// connected components
int comp = 0;
for (int i = 1; i <= n; i++) {
    if (!vis[i]) {
        dfs(i);
        comp++;
    }
}

// 0-1 BFS
deque<int> dq;
vector<int> dist(n+1, 1e9);

dist[src] = 0;
dq.push_front(src);

while (!dq.empty()) {
    int u = dq.front(); dq.pop_front();
    for (auto [v, w] : g[u]) {
        if (dist[v] > dist[u] + w) {
            dist[v] = dist[u] + w;
            if (w == 0) dq.push_front(v);
            else dq.push_back(v);
        }
    }
}

// dijkstra 
// Weighted graph
// No negative edges
priority_queue<pair<long long,int>,
               vector<pair<long long,int>>,
               greater<pair<long long,int>>> pq;

vector<long long> dist(n+1, 1e18);
dist[src] = 0;
pq.push({0, src});

while (!pq.empty()) {
    auto [d, u] = pq.top(); pq.pop();
    if (d > dist[u]) continue;
    for (auto [v, w] : g[u]) {
        if (dist[v] > d + w) {
            dist[v] = d + w;
            pq.push({dist[v], v});
        }
    }
}

// Bellman Ford
// Negative edges
// Detect negative cycle
vector<long long> dist(n+1, 1e18);
dist[src] = 0;

for (int i = 1; i <= n-1; i++) {
    for (auto [u, v, w] : edges) {
        if (dist[u] + w < dist[v])
            dist[v] = dist[u] + w;
    }
}

// ---Tree Basics----
// Parent, depth
// Subtree size
// Rooted tree logic

int parent[N], depth[N], sub[N];

void dfs(int u, int p) {
    parent[u] = p;
    sub[u] = 1;
    for (int v : g[u]) {
        if (v == p) continue;
        depth[v] = depth[u] + 1;
        dfs(v, u);
        sub[u] += sub[v];
    }
}
// LCA binary Lifting
// Distance queries
// Path queries
// Tree problems (MOST IMPORTANT)
int up[N][20], depth[N];

void dfs(int u, int p) {
    up[u][0] = p;
    for (int i = 1; i < 20; i++)
        up[u][i] = up[up[u][i-1]][i-1];

    for (int v : g[u]) {
        if (v == p) continue;
        depth[v] = depth[u] + 1;
        dfs(v, u);
    }
}

int lca(int a, int b) {
    if (depth[a] < depth[b]) swap(a, b);

    for (int i = 19; i >= 0; i--)
        if (depth[a] - (1<<i) >= depth[b])
            a = up[a][i];

    if (a == b) return a;

    for (int i = 19; i >= 0; i--) {
        if (up[a][i] != up[b][i]) {
            a = up[a][i];
            b = up[b][i];
        }
    }
    return up[a][0];
}
// ⃣Tree DP
// Count paths
// Maximize/minimize values
// Subtree decisions
long long dp[N];

void dfs(int u, int p) {
    dp[u] = 1;
    for (int v : g[u]) {
        if (v == p) continue;
        dfs(v, u);
        dp[u] += dp[v];
    }
}
// DSU (Union-Find)
// Why used
// Connectivity
// MST
// Offline queries
int parent[N], sz[N];

int find(int x) {
    return parent[x] == x ? x : parent[x] = find(parent[x]);
}

void unite(int a, int b) {
    a = find(a); b = find(b);
    if (a != b) {
        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
    }
}
// MST(Kruskal)
// Minimum cost connection
sort(edges.begin(), edges.end());
long long cost = 0;

for (auto [w, u, v] : edges) {
    if (find(u) != find(v)) {
        unite(u, v);
        cost += w;
    }
}
// Topological Sort
// Dependency problems
// DAG DP
queue<int> q;
vector<int> indeg(n+1);

for (int i = 1; i <= n; i++)
    if (indeg[i] == 0) q.push(i);

while (!q.empty()) {
    int u = q.front(); q.pop();
    for (int v : g[u]) {
        if (--indeg[v] == 0)
            q.push(v);
    }
}
// Euler Path (Undirected)Hierholzer Algorithm
vector<int> g[200005];
map<pair<int,int>, int> used;
vector<int> path;

void dfs(int u) {
    for (int v : g[u]) {
        if (used[{u,v}]) continue;
        used[{u,v}] = used[{v,u}] = 1;
        dfs(v);
    }
    path.push_back(u);
}

// How to use
int start = 1;
for (int i = 1; i <= n; i++)
    if (g[i].size() % 2 == 1)
        start = i;

dfs(start);
reverse(path.begin(), path.end());


//  path.size() == edges + 1  Euler Path exists


// Euler Path (Directed)
vector<int> g[N];
int in[N], out[N];
vector<int> path;

void dfs(int u) {
    while (!g[u].empty()) {
        int v = g[u].back();
        g[u].pop_back();
        dfs(v);
    }
    path.push_back(u);
}
// Start node logic
int start = 1;
for (int i = 1; i <= n; i++) {
    if (out[i] == in[i] + 1)
        start = i;
}
// Hamiltonian Path (DP + Bitmask)
bool dp[1<<20][20];
int parent[1<<20][20];

dp[1][0] = true;

for (int mask = 1; mask < (1<<n); mask++) {
    for (int u = 0; u < n; u++) {
        if (!dp[mask][u]) continue;
        for (int v : g[u]) {
            if (!(mask & (1<<v))) {
                dp[mask | (1<<v)][v] = true;
                parent[mask | (1<<v)][v] = u;
            }
        }
    }
}

// Check existence
int full = (1<<n) - 1;
for (int i = 0; i < n; i++)
    if (dp[full][i])
        cout << "Hamiltonian Path exists\n";