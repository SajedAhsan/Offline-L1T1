// C. Circular RMQ

#include <bits/stdc++.h>
using namespace std;
const int N = 2e5 + 5;

long long tree[4 * N];
long long lazy[4 * N];
int arr[N];

void build(int node, int l, int r)
{
    if (l == r)
    {
        tree[node] = arr[l];
        return;
    }

    int mid = (l + r) / 2;

    build(2 * node + 1, l, mid);
    build(2 * node + 2, mid + 1, r);

    tree[node] = min(tree[2 * node + 1],tree[2 * node + 2]);
}

void push(int node, int l, int r)
{
    if (lazy[node] == 0)
        return;

    tree[node] += lazy[node];

    if (l != r)
    {
        lazy[2 * node + 1] += lazy[node];
        lazy[2 * node + 2] += lazy[node];
    }

    lazy[node] = 0;
}

void update(int node, int l, int r, int ql, int qr, long long val)
{
    push(node, l, r);

    if (r < ql || qr < l)
        return;

    if (ql <= l && r <= qr)
    {
        lazy[node] += val;
        push(node, l, r);
        return;
    }

    int mid = (l + r) / 2;

    update(2 * node + 1, l, mid, ql, qr, val);
    update(2 * node + 2, mid + 1, r, ql, qr, val);

    tree[node] = min(tree[2 * node + 1],tree[2 * node + 2]);
}

long long query(int node, int l, int r, int ql, int qr)
{
    push(node, l, r);

    if (r < ql || qr < l)
        return INT_MAX;

    if (ql <= l && r <= qr)
        return tree[node];

    int mid = (l + r) / 2;

    return min(query(2 * node + 1, l, mid, ql, qr),query(2 * node + 2, mid + 1, r, ql, qr));
}
int main()
{
    int n;
    cin >> n;
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    build(0,0,n-1);
    int q;
    cin >> q;
    cin.ignore();
    while (q--)
    {
        string line;
        getline(cin, line);

        stringstream ss(line);

        int x, y, z;

        ss >> x >> y;

        if (ss >> z)
        {
            if(x > y)
            {
                update(0,0,n-1,x,n-1,z);
                update(0,0,n-1,0,y,z);
            }
            else
                update(0,0,n-1,x,y,z);
        }
        else
        {
            if(x > y)
                cout << min(query(0,0,n-1,x,n-1),query(0,0,n-1,0,y)) << endl;
            else
                cout << query(0,0,n-1,x,y) << endl;
        }
    }
}