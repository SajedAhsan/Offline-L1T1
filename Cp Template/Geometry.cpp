#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double PI = acos(-1.0);

struct Point {
    double x, y;
    Point() {}
    Point(double x, double y) : x(x), y(y) {}

    Point operator + (const Point &p) const { return Point(x + p.x, y + p.y); }
    Point operator - (const Point &p) const { return Point(x - p.x, y - p.y); }
    Point operator * (double c) const { return Point(x * c, y * c); }
    Point operator / (double c) const { return Point(x / c, y / c); }

    bool operator == (const Point &p) const { return fabs(x - p.x) < EPS && fabs(y - p.y) < EPS; }
};

// Dot product: used for angles, projections
double dot(const Point &a, const Point &b) {
    return a.x * b.x + a.y * b.y;
}

// Cross product: used for area, orientation, intersection
double cross(const Point &a, const Point &b) {
    return a.x * b.y - a.y * b.x;
}

// Distance between points
double dist(const Point &a, const Point &b) {
    return hypot(a.x - b.x, a.y - b.y);
}

// Orientation: 0 = collinear, 1 = clockwise, 2 = counterclockwise
int orientation(const Point &a, const Point &b, const Point &c) {
    double val = cross(b - a, c - a);
    if(fabs(val) < EPS) return 0;
    return (val > 0) ? 2 : 1;
}

// Line segment intersection
bool onSegment(Point p, Point q, Point r) {
    // check if q lies on pr
    return q.x <= max(p.x, r.x)+EPS && q.x >= min(p.x, r.x)-EPS &&
           q.y <= max(p.y, r.y)+EPS && q.y >= min(p.y, r.y)-EPS;
}

bool segmentsIntersect(Point p1, Point q1, Point p2, Point q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if(o1 != o2 && o3 != o4) return true;
    if(o1 == 0 && onSegment(p1, p2, q1)) return true;
    if(o2 == 0 && onSegment(p1, q2, q1)) return true;
    if(o3 == 0 && onSegment(p2, p1, q2)) return true;
    if(o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false;
}

// Polygon area (shoelace formula)
double polygonArea(vector<Point> &poly) {
    double area = 0;
    int n = poly.size();
    for(int i=0; i<n; i++) {
        area += cross(poly[i], poly[(i+1)%n]);
    }
    return fabs(area)/2.0;
}

// Convex Hull (Andrew's monotone chain, O(n log n))
vector<Point> convexHull(vector<Point> P) {
    int n = P.size();
    if(n <= 1) return P;
    sort(P.begin(), P.end(), [](Point a, Point b) {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    });
    vector<Point> L, U;
    for(int i=0; i<n; i++) {
        while(L.size() >= 2 && cross(L.back() - L[L.size()-2], P[i] - L[L.size()-2]) <= EPS) L.pop_back();
        L.push_back(P[i]);
    }
    for(int i=n-1; i>=0; i--) {
        while(U.size() >= 2 && cross(U.back() - U[U.size()-2], P[i] - U[U.size()-2]) <= EPS) U.pop_back();
        U.push_back(P[i]);
    }
    L.pop_back(); U.pop_back();
    L.insert(L.end(), U.begin(), U.end());
    return L;
}

// Point in polygon: ray casting method
bool pointInPolygon(Point p, vector<Point> &poly) {
    int cnt = 0;
    int n = poly.size();
    for(int i=0; i<n; i++) {
        Point a = poly[i], b = poly[(i+1)%n];
        if(fabs(cross(b-a, p-a)) < EPS && dot(p-a, p-b) <= 0) return true; // on edge
        if(a.y > b.y) swap(a,b);
        if(p.y > a.y && p.y <= b.y && cross(b-a, p-a) > 0) cnt++;
    }
    return cnt & 1;
}

// Circle-line intersection
vector<Point> circleLineIntersection(Point a, Point b, Point c, double r) {
    // Line a->b, Circle center c, radius r
    Point d = b - a;
    Point f = a - c;
    double A = dot(d,d);
    double B = 2*dot(f,d);
    double C = dot(f,f) - r*r;
    double disc = B*B - 4*A*C;
    vector<Point> res;
    if(disc < -EPS) return res; // no intersection
    disc = max(disc,0.0);
    double t1 = (-B + sqrt(disc))/(2*A);
    double t2 = (-B - sqrt(disc))/(2*A);
    res.push_back(a + d*t1);
    if(disc > EPS) res.push_back(a + d*t2);
    return res;
}

int main() {
    // Example usage
    Point p1(0,0), p2(1,1);
    cout << "Distance: " << dist(p1,p2) << endl;

    vector<Point> poly = {{0,0},{4,0},{4,3},{0,3}};
    cout << "Polygon Area: " << polygonArea(poly) << endl;

    vector<Point> hull = convexHull(poly);
    cout << "Convex Hull Points: ";
    for(auto &p: hull) cout << "(" << p.x << "," << p.y << ") ";
    cout << endl;

    return 0;
}
