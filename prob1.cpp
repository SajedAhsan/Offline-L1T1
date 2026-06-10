// Tree diameter for weighted tree
#include <bits/stdc++.h>
using namespace std;

void dfs(int node, int parent, vector<vector<pair<int, int>>> &graph, vector<long long int> &dist)
{
    for (auto [x, y] : graph[node])
    {
        if (x == parent)
            continue;

        dist[x] = dist[node] + y*1ll;
        dfs(x, node, graph, dist);
    }
}
int main()
{
ios::sync_with_stdio(false);
cin.tie(nullptr);
    int t;
    cin >> t;
    for(int test = 1; test <= t; test++)
    {
        int n;
        cin >> n;
        vector<vector<pair<int, int>>> graph(n);
        vector<long long int> dist(n);
        for (int i = 1; i < n; i++)
        {
            int u, v, w;
            cin >> u >> v >> w;
            graph[u].push_back({v, w});
            graph[v].push_back({u, w});
        }
        dfs(0, -1, graph, dist);
        long long int mxNode = -1, mxDist = -1;
        for (int i = 0; i < n; i++)
        {
            if (dist[i] > mxDist)
            {
                mxDist = dist[i];
                mxNode = i;
            }
            dist[i] = 0;
        }
        dfs(mxNode, -1, graph, dist);
        mxNode = -1;
        mxDist = -1;
        for (int i = 0; i < n; i++)
        {
            if (dist[i] > mxDist)
            {
                mxDist = dist[i];
                mxNode = i;
            }
            dist[i] = 0;
        }

        cout << "Case " << test << ": " << mxDist << endl;
    }
}