const ld infinity = numeric_limits<ld>::infinity();

struct Point {
    ld x, y;
    Point() {}
    Point(ld x, ld y):
        x(x), y(y) {}
};

struct Line {
    ld k, b;
    Line() {}
    Line(ld k, ld b):
        k(k), b(b) {}
    ll valAt(ll x) const {
        return k * x + b;
    }
};

Point intersect(const Line& lhs, const Line& rhs) {
    if (lhs.k == rhs.k && lhs.b == rhs.b) return Point(infinity, infinity);
    if (lhs.k == rhs.k) return Point(infinity, infinity);
    ld x = (rhs.b - lhs.b) / (lhs.k - rhs.k);
    ld y = lhs.k * x + lhs.b;
    return Point(x, y);
}

class ConvexHull {
public:
    ConvexHull() {}
    void addLine(const Line& ln) {
        while (!point.empty() && intersect(line.back(), ln).x <= point.back().x) {
            point.pop_back();
            line.pop_back();
        }
        if (!line.empty()) point.push_back(intersect(line.back(), ln));
        line.push_back(ln);
    }
    ll getMin(ll x) {
        if (line.empty()) return LLINF;
        int idx = lower_bound(all(point), Point(x, -infinity),
            [](Point lhs, Point rhs){
                return lhs.x < rhs.x;
        }) - point.begin();
        return line[idx].valAt(x);
    }
    friend ConvexHull operator +(const ConvexHull& lhs, const ConvexHull& rhs) {
        ConvexHull ret;
        int lf = 0, rg = 0;
		for(int i = 0; i < lhs.line.size()+rhs.line.size(); ++i) {
            ld fst = (lf < lhs.line.size() ? lhs.line[lf].k : -infinity);
            ld snd = (rg < rhs.line.size() ? rhs.line[rg].k : -infinity);
            if (fst == snd) {
                if (lhs.line[lf].b < rhs.line[rg].b) {
                    ret.addLine(lhs.line[lf]);
                    ++lf;
                } else {
                    ret.addLine(rhs.line[rg]);
                    ++rg;
                }
            } else if (fst > snd) {
                ret.addLine(lhs.line[lf]);
                ++lf;
            } else {
                ret.addLine(rhs.line[rg]);
                ++rg;
            }
        }
        return ret;
    }
private:
    vector<Point> point;
    vector<Line> line;
};