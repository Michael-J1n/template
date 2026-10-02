#include <bits/stdc++.h>
using namespace std;

/**
 * 计算几何基本元素
 *
 * 使用 long double，默认误差 EPS=1e-12L
 * 所有角度参数和返回值均为弧度
 * Line 内部表示为 p+v*t，Line(a,b) 表示经过 a、b 的直线
 * Segment(a,b) 表示闭线段 [a,b]
 * Circle(o,r) 表示圆心为 o、半径为 r 的圆
 * Triangle(a,b,c) 表示三点构成的三角形
 *
 * sgn(x)：按 EPS 判断 x 的符号
 * equal(a,b)：判断两个浮点数是否相等
 * sqr(x)：返回 x^2
 * safe_sqrt(x)：将负误差截断为 0 后开方
 * deg_to_rad(deg)：角度转弧度
 * rad_to_deg(rad)：弧度转角度
 * normalize_angle(rad)：将角度归一化到 [0,2PI)
 *
 * Point(x,y)：建立点或向量 (x,y)
 * dot(a,b)：返回向量 a、b 的点积
 * dot(a,b,c)：返回向量 ab、ac 的点积
 * cross(a,b)：返回向量 a、b 的叉积
 * cross(a,b,c)：返回向量 ab、ac 的叉积
 * norm2(a)：返回向量 a 的模长平方
 * norm(a)：返回向量 a 的模长
 * dis2(a,b)：返回点 a、b 的距离平方
 * dis(a,b)：返回点 a、b 的距离
 * zero_vector(a)：判断 a 是否为零向量
 * midpoint(a,b)：返回 a、b 的中点
 * unit(a)：返回 a 的单位向量，零向量返回零向量
 * rotate_90(a)：将 a 逆时针旋转 90 度
 * rotate(a,rad)：将 a 逆时针旋转 rad
 * polar_angle(a)：返回 a 的极角，范围 (-PI,PI]
 * vector_angle(a,b)：返回 a、b 的无向夹角，范围 [0,PI]
 * oriented_angle(a,b)：返回从 a 到 b 的有向夹角，范围 (-PI,PI]
 * ccw_angle(a,b)：返回从 a 逆时针转到 b 的夹角，范围 [0,2PI)
 * parallel(a,b)：判断向量 a、b 是否平行
 * perpendicular(a,b)：判断向量 a、b 是否垂直
 * orientation(a,b,c)：c 在有向直线 ab 左侧返回 1，右侧返回 -1，共线返回 0
 * collinear(a,b,c)：判断 a、b、c 是否共线
 * project_length(a,b,p)：返回向量 ap 在 ab 方向上的有向投影长度
 *
 * Line(a,b)：建立经过点 a、b 的直线
 * Line::from_point_vector(p,v)：建立经过 p、方向向量为 v 的直线
 * l.at(t)：返回直线 l 上的点 l.p+l.v*t
 * Segment(a,b)：建立闭线段 [a,b]
 * line_valid(l)：判断 l 是否为非退化直线
 * segment_valid(s)：判断 s 是否为非退化线段
 * line_from_abc(a,b,c,l)：由 ax+by+c=0 建立直线，成功返回 true
 * line_normal(l)：返回 l 的逆时针单位法向量
 * parallel_line(l,p)：返回经过 p 且平行于 l 的直线
 * perpendicular_line(l,p)：返回经过 p 且垂直于 l 的直线
 * perpendicular_bisector(a,b)：返回线段 ab 的垂直平分线
 * on_line(p,l)：判断 p 是否在 l 上
 * on_segment(p,s)：判断 p 是否在线段 s 上
 * on_ray(p,o,v)：判断 p 是否在以 o 为起点、v 为方向的射线上
 * side_of_line(p,l)：p 在 l 左侧返回 1，右侧返回 -1，线上返回 0
 * distance_point_line_signed(p,l)：返回 p 到有向直线 l 的有向距离
 * distance_point_line(p,l)：返回 p 到直线 l 的距离
 * project_point_line(p,l)：返回 p 在 l 上的投影点
 * reflect_point_line(p,l)：返回 p 关于 l 的对称点
 * closest_point_segment(p,s)：返回线段 s 上距离 p 最近的点
 * distance_point_segment(p,s)：返回 p 到线段 s 的距离
 * line_parallel(a,b)：判断直线 a、b 是否平行
 * line_coincident(a,b)：判断直线 a、b 是否重合
 * line_intersection(a,b,p)：求两直线交点，返回 0 表示无交点，1 表示唯一交点，2 表示重合
 * segment_strict_intersect(a,b)：判断两线段是否在各自内部严格相交
 * segment_intersect(a,b)：判断两闭线段是否相交
 * segment_intersection(a,b,p1,p2)：返回 0 表示不相交，1 表示交于 p1，2 表示重合部分为 [p1,p2]
 * line_segment_intersection(l,s,p1,p2)：返回 0 表示不相交，1 表示交于 p1，2 表示整条线段在直线上
 * distance_line_line(a,b)：返回两直线距离
 * distance_line_segment(l,s)：返回直线与线段距离
 * distance_segment_segment(a,b)：返回两线段距离
 * line_angle(a,b)：返回两直线的无向锐夹角，范围 [0,PI/2]
 * line_oriented_angle(a,b)：返回从直线 a 的方向到直线 b 的方向的有向夹角
 *
 * polar_less(a,b)：按以原点为中心、正 x 轴为起点的逆时针极角比较
 * PolarCmp(o)：建立以 o 为中心的极角比较器
 * polar_sort(p,o)：将点集 p 按以 o 为中心的极角排序，同一射线按距离升序
 *
 * Circle(o,r)：建立圆心为 o、半径为 r 的圆
 * Circle::from_diameter(a,b)：建立以 ab 为直径的圆
 * circle_valid(c)：判断 c 的半径是否合法
 * circle_area(c)：返回圆面积
 * circle_circumference(c)：返回圆周长
 * arc_length(c,rad)：返回圆心角为 rad 的弧长
 * sector_area(c,rad)：返回圆心角为 rad 的扇形面积
 * chord_length(c,rad)：返回圆心角为 rad 的弦长
 * point_circle_relation(p,c)：p 在圆内返回 -1，圆上返回 0，圆外返回 1
 * line_circle_relation(l,c)：返回直线与圆的交点数量 0/1/2
 * circle_circle_relation(a,b)：返回 0 外离，1 外切，2 相交，3 内切，4 内含，5 重合
 * circle_centers(a,b,r,o1,o2)：求经过 a、b 且半径为 r 的圆心，返回 -1 表示无限多个，否则返回圆心数量 0/1/2
 * line_circle_intersection(l,c,p1,p2)：求直线与圆的交点，返回交点数量 0/1/2
 * segment_circle_intersection(s,c,p1,p2)：求线段与圆周的交点，返回交点数量 0/1/2
 * circle_circle_intersection(a,b,p1,p2)：求两圆交点，返回 -1 表示两非退化圆重合，否则返回交点数量 0/1/2
 * radical_axis(a,b,l)：求两圆根轴，存在唯一根轴返回 true
 * circle_from_three_points(a,b,c,res)：求经过三点的圆，三点不共线返回 true
 * tangent_points(p,c,p1,p2)：求从 p 向圆 c 作切线的切点，返回 -1 表示点圆重合且有无限条切线，否则返回切点数量 0/1/2
 * common_tangents(a,b,res)：求两圆公切线，每条保存两个切点，返回 -1 表示两圆重合，否则返回切线数量 0..4
 *
 * Triangle(a,b,c)：建立三角形 abc
 * triangle_area_signed(a,b,c)：返回三点构成三角形的有向面积
 * triangle_area(a,b,c)：三参数为点时返回三点构成三角形的面积
 * triangle_area(a,b,c)：三参数为边长时使用海伦公式求面积
 * triangle_area_signed(t)：返回三角形 t 的有向面积
 * triangle_area(t)：返回三角形 t 的面积
 * triangle_perimeter(t)：返回三角形 t 的周长
 * triangle_valid(t)：判断 t 是否非退化
 * triangle_angle(a,b,c)：返回顶点 b 的内角
 * triangle_centroid(t)：返回重心
 * triangle_circumcircle(t,c)：求外接圆，成功返回 true
 * triangle_circumcenter(t,p)：求外心，成功返回 true
 * triangle_circumradius(t)：返回外接圆半径
 * triangle_incircle(t,c)：求内切圆，成功返回 true
 * triangle_incenter(t,p)：求内心，成功返回 true
 * triangle_inradius(t)：返回内切圆半径
 * triangle_orthocenter(t,p)：求垂心，成功返回 true
 * triangle_angle_bisector(a,b,c)：返回顶点 b 的内角平分线
 * point_triangle_relation(p,t)：p 在三角形外返回 -1，边界返回 0，内部返回 1
 *
 * convex_hull(p)：返回点集 p 的凸包，逆时针、不重复首尾点，边上共线点只保留两端
 * upper_hull(p)：返回点集的上凸包，从最左点到最右点
 * lower_hull(p)：返回点集的下凸包，从最左点到最右点
 * 类似的，还有右上右下左上左下凸包
 * 【注意】！：返回的凸包是一个多边形对象，包含一条假边，在使用时如果不希望干扰答案，请依次枚举 ch 中的相邻边并忽略 ch[sz-1],ch[0] 组成的最后一条假边
 * Polygon(p)：由点集 p 建立凸多边形，内部自动取凸包并按逆时针保存
 * polygon_valid(poly)：判断 poly 是否为非退化凸多边形
 * polygon_perimeter(poly)：返回凸多边形周长
 * polygon_area_signed(poly)：返回凸多边形有向面积，Polygon 非退化时为正
 * polygon_area(poly)：返回凸多边形面积
 * point_polygon_relation(p,poly)：p 在凸多边形外返回 -1，边界返回 0，内部返回 1，O(logn)
 * segment_in_polygon(s,poly)：判断闭线段 s 是否完全位于凸多边形内，含边界，O(logn)
 * segment_polygon_intersection(s,poly,p1,p2)：求线段与凸多边形的交集，返回 0 空集，1 单点 p1，2 线段 [p1,p2]
 * segment_polygon_intersect(s,poly)：判断线段与凸多边形是否有公共点
 * line_polygon_intersection(l,poly,p1,p2)：求直线与凸多边形的交集，返回 0 空集，1 单点 p1，2 线段 [p1,p2]
 * line_polygon_intersect(l,poly)：判断直线与凸多边形是否有公共点
 *
 * IPoint(x,y)：建立整数点或整数向量 (x,y)
 * ILine(a,b)：建立经过两个整数点 a、b 的整数直线
 * ISegment(a,b)：建立整数闭线段 [a,b]
 * ICircle(o,r)：建立整数圆心、整数半径的圆
 * ITriangle(a,b,c)：建立三个整数顶点构成的三角形
 * IPolygon(p)：由整数点集 p 建立整数凸多边形，内部自动取凸包并按逆时针保存
 * to_point/to_line/to_segment/to_circle/to_triangle/to_polygon：转换为对应浮点元素
 * 整数版本的 dot/cross/norm2/dis2/orientation/collinear 使用 ll 精确计算
 * 整数版本的 on_line/on_segment/on_ray、线段相交、极角排序使用 ll 精确判断
 * 整数版本的 point_circle_relation/circle_circle_relation 使用 ll 精确判断
 * triangle_area2_signed/triangle_area2 返回三角形面积的两倍，使用 ll 精确计算
 * 整数版本的 convex_hull/upper_hull/lower_hull、point_polygon_relation、segment_in_polygon 使用 ll 精确判断
 * polygon_area2_signed/polygon_area2 返回整数凸多边形面积的两倍，使用 ll 精确计算
 * 整数凸多边形的交点坐标仍返回 Point；to_polygon 将 IPolygon 转为 Polygon
 * 距离、投影、交点坐标、圆心、切点、角度等非整数结果仍返回 db/Point/Line/Circle
 * cmp_fraction(a,b,c,d)：精确比较非负分数 a/b 与 c/d，要求 b,d>0，不进行交叉乘法
 * cmp_product(a,b,c,d)：精确比较非负乘积 a*b 与 c*d，不实际计算两个乘积
 * 整数版本的 line_circle_relation/segment_circle_relation 使用 ll 精确判断
 * 整数版本的线圆、线段圆交点数量严格精确，交点坐标返回 Point
 *
 * 整数层要求原始输入坐标和半径绝对值不超过 1e9
 * 在此前提下，单次整数点积、叉积、距离平方及圆关系中的整数中间结果均处于 ll 范围内
 * polygon_area2_signed/polygon_area2 还要求累计两倍面积处于 ll 范围内；模板始终不使用更宽整数类型
 * cmp_product 通过连分数比较乘积，线圆关系不会计算 cross^2 或 r^2*|v|^2
 * 不对用户自行进行的反复 IPoint 加减或任意整数数乘提供额外溢出保证
 * cmp_fraction/cmp_product、整数线圆与线段圆关系最坏 O(log V)；convex_hull/upper_hull/lower_hull 为 O(nlogn)
 * point_polygon_relation/segment_in_polygon 为 O(logn)，segment_polygon_intersection 为 O(n)，polar_sort 为 O(nlogn)，其余基础操作均为 O(1)
 * 凸包构造与线段-多边形求交需要 O(n) 额外空间，common_tangents 的输出最多为 4 条切线
 * Point::operator== 使用 EPS，Point::operator< 和极角比较使用精确比较
 * 调用圆相关接口前应保证半径非负
 * 坐标量级极大或极小时需要按题目范围调整 EPS
 * 只做整数方向、共线和相交判断时，优先使用整数叉积模板
 */
namespace Geometry {
    using ll = long long;
    using db = long double;
    const db EPS = 1e-12L;
    const db PI = acosl(-1.0L);
    int sgn(db x) { return (x > EPS) - (x < -EPS); }
    bool equal(db a, db b) { return sgn(a - b) == 0; }
    db sqr(db x) { return x * x; }
    db safe_sqrt(db x) { return sqrtl(max<db>(0, x)); }
    db deg_to_rad(db deg) { return deg * PI / 180; }
    db rad_to_deg(db rad) { return rad * 180 / PI; }
    db normalize_angle(db rad) { rad = fmodl(rad, 2 * PI); if (rad < 0) rad += 2 * PI; return rad; }
    struct Point {
        db x, y;
        Point() : x(0), y(0) {}
        Point(db _x, db _y) : x(_x), y(_y) {}
        Point operator+() const { return *this; }
        Point operator-() const { return Point(-x, -y); }
        Point operator+(const Point& o) const { return Point(x + o.x, y + o.y); }
        Point operator-(const Point& o) const { return Point(x - o.x, y - o.y); }
        Point operator*(db k) const { return Point(x * k, y * k); }
        Point operator/(db k) const { return Point(x / k, y / k); }
        Point& operator+=(const Point& o) { x += o.x, y += o.y; return *this; }
        Point& operator-=(const Point& o) { x -= o.x, y -= o.y; return *this; }
        Point& operator*=(db k) { x *= k, y *= k; return *this; }
        Point& operator/=(db k) { x /= k, y /= k; return *this; }
        bool operator==(const Point& o) const { return equal(x, o.x) && equal(y, o.y); }
        bool operator!=(const Point& o) const { return !(*this == o); }
        bool operator<(const Point& o) const { return x != o.x ? x < o.x : y < o.y; }
    };
    Point operator*(db k, const Point& p) { return p * k; }
    istream& operator>>(istream& is, Point& p) { return is >> p.x >> p.y; }
    ostream& operator<<(ostream& os, const Point& p) { return os << p.x << ' ' << p.y; }
    db dot(const Point& a, const Point& b) { return a.x * b.x + a.y * b.y; }
    db dot(const Point& a, const Point& b, const Point& c) { return dot(b - a, c - a); }
    db cross(const Point& a, const Point& b) { return a.x * b.y - a.y * b.x; }
    db cross(const Point& a, const Point& b, const Point& c) { return cross(b - a, c - a); }
    db norm2(const Point& a) { return dot(a, a); }
    db norm(const Point& a) { return hypotl(a.x, a.y); }
    db dis2(const Point& a, const Point& b) { return norm2(a - b); }
    db dis(const Point& a, const Point& b) { return hypotl(a.x - b.x, a.y - b.y); }
    bool zero_vector(const Point& a) { return sgn(norm2(a)) == 0; }
    Point midpoint(const Point& a, const Point& b) { return (a + b) / 2; }
    Point unit(const Point& a) { db len = norm(a); return sgn(len) == 0 ? Point() : a / len; }
    Point rotate_90(const Point& a) { return Point(-a.y, a.x); }
    Point rotate(const Point& a, db rad) { db c = cosl(rad), s = sinl(rad); return Point(a.x * c - a.y * s, a.x * s + a.y * c); }
    db polar_angle(const Point& a) { return atan2l(a.y, a.x); }
    db vector_angle(const Point& a, const Point& b) { return zero_vector(a) || zero_vector(b) ? 0 : atan2l(fabsl(cross(a, b)), dot(a, b)); }
    db oriented_angle(const Point& a, const Point& b) { return zero_vector(a) || zero_vector(b) ? 0 : atan2l(cross(a, b), dot(a, b)); }
    db ccw_angle(const Point& a, const Point& b) { return normalize_angle(oriented_angle(a, b)); }
    bool parallel(const Point& a, const Point& b) { return sgn(cross(a, b)) == 0; }
    bool perpendicular(const Point& a, const Point& b) { return sgn(dot(a, b)) == 0; }
    int orientation(const Point& a, const Point& b, const Point& c) { return sgn(cross(a, b, c)); }
    bool collinear(const Point& a, const Point& b, const Point& c) { return orientation(a, b, c) == 0; }
    db project_length(const Point& a, const Point& b, const Point& p) { Point v = b - a; db len = norm(v); return sgn(len) == 0 ? 0 : dot(p - a, v) / len; }
    struct Line {
        Point p, v;
        Line() {}
        Line(const Point& a, const Point& b) : p(a), v(b - a) {}
        static Line from_point_vector(const Point& p, const Point& v) { Line l; l.p = p, l.v = v; return l; }
        Point at(db t) const { return p + v * t; }
    };
    struct Segment {
        Point a, b;
        Segment() {}
        Segment(const Point& _a, const Point& _b) : a(_a), b(_b) {}
    };
    bool line_valid(const Line& l) { return !zero_vector(l.v); }
    bool segment_valid(const Segment& s) { return s.a != s.b; }
    bool line_from_abc(db a, db b, db c, Line& l) { if (sgn(a) == 0 && sgn(b) == 0) return false; Point p = fabsl(a) > fabsl(b) ? Point(-c / a, 0) : Point(0, -c / b); l = Line::from_point_vector(p, Point(-b, a)); return true; }
    Point line_normal(const Line& l) { return unit(rotate_90(l.v)); }
    Line parallel_line(const Line& l, const Point& p) { return Line::from_point_vector(p, l.v); }
    Line perpendicular_line(const Line& l, const Point& p) { return Line::from_point_vector(p, rotate_90(l.v)); }
    Line perpendicular_bisector(const Point& a, const Point& b) { return Line::from_point_vector(midpoint(a, b), rotate_90(b - a)); }
    bool on_line(const Point& p, const Line& l) { return line_valid(l) ? sgn(cross(l.v, p - l.p)) == 0 : p == l.p; }
    bool on_segment(const Point& p, const Segment& s) { return orientation(s.a, s.b, p) == 0 && sgn(dot(p - s.a, p - s.b)) <= 0; }
    bool on_ray(const Point& p, const Point& o, const Point& v) { return zero_vector(v) ? p == o : sgn(cross(v, p - o)) == 0 && sgn(dot(v, p - o)) >= 0; }
    int side_of_line(const Point& p, const Line& l) { return line_valid(l) ? sgn(cross(l.v, p - l.p)) : 0; }
    db distance_point_line_signed(const Point& p, const Line& l) { return line_valid(l) ? cross(l.v, p - l.p) / norm(l.v) : 0; }
    db distance_point_line(const Point& p, const Line& l) { return line_valid(l) ? fabsl(distance_point_line_signed(p, l)) : dis(p, l.p); }
    Point project_point_line(const Point& p, const Line& l) { return line_valid(l) ? l.p + l.v * (dot(p - l.p, l.v) / norm2(l.v)) : l.p; }
    Point reflect_point_line(const Point& p, const Line& l) { return project_point_line(p, l) * 2 - p; }
    Point closest_point_segment(const Point& p, const Segment& s) { Point v = s.b - s.a; db len2 = norm2(v); if (sgn(len2) == 0) return s.a; db t = dot(p - s.a, v) / len2; t = max<db>(0, min<db>(1, t)); return s.a + v * t; }
    db distance_point_segment(const Point& p, const Segment& s) { return dis(p, closest_point_segment(p, s)); }
    bool line_parallel(const Line& a, const Line& b) { return parallel(a.v, b.v); }
    bool line_coincident(const Line& a, const Line& b) { return line_valid(a) && line_valid(b) && line_parallel(a, b) && on_line(b.p, a); }
    int line_intersection(const Line& a, const Line& b, Point& p) {
        bool va = line_valid(a), vb = line_valid(b);
        if (!va && !vb) { if (a.p != b.p) return 0; p = a.p; return 1; }
        if (!va) { if (!on_line(a.p, b)) return 0; p = a.p; return 1; }
        if (!vb) { if (!on_line(b.p, a)) return 0; p = b.p; return 1; }
        db d = cross(a.v, b.v);
        if (sgn(d) == 0) return on_line(b.p, a) ? 2 : 0;
        p = a.at(cross(b.p - a.p, b.v) / d);
        return 1;
    }
    bool segment_strict_intersect(const Segment& a, const Segment& b) { int c1 = orientation(a.a, a.b, b.a), c2 = orientation(a.a, a.b, b.b); int c3 = orientation(b.a, b.b, a.a), c4 = orientation(b.a, b.b, a.b); return c1 * c2 < 0 && c3 * c4 < 0; }
    int segment_intersection(const Segment& a, const Segment& b, Point& p1, Point& p2) {
        Point q[4];
        int cnt = 0;
        auto add = [&](const Point& p) -> void {
            for (int i = 0; i < cnt; ++i) {
                if (q[i] == p) return;
            }
            q[cnt++] = p;
        };
        if (on_segment(a.a, b)) add(a.a);
        if (on_segment(a.b, b)) add(a.b);
        if (on_segment(b.a, a)) add(b.a);
        if (on_segment(b.b, a)) add(b.b);
        if (cnt) { p1 = p2 = q[0]; for (int i = 1; i < cnt; ++i) p1 = min(p1, q[i]), p2 = max(p2, q[i]); return p1 == p2 ? 1 : 2; }
        if (!segment_strict_intersect(a, b)) return 0;
        line_intersection(Line(a.a, a.b), Line(b.a, b.b), p1);
        p2 = p1;
        return 1;
    }
    bool segment_intersect(const Segment& a, const Segment& b) { Point p1, p2; return segment_intersection(a, b, p1, p2) != 0; }
    int line_segment_intersection(const Line& l, const Segment& s, Point& p1, Point& p2) {
        if (!line_valid(l)) { if (!on_segment(l.p, s)) return 0; p1 = p2 = l.p; return 1; }
        int a = sgn(cross(l.v, s.a - l.p)), b = sgn(cross(l.v, s.b - l.p));
        if (a == 0 && b == 0) { p1 = s.a, p2 = s.b; return p1 == p2 ? 1 : 2; }
        if (a != 0 && b != 0 && a == b) return 0;
        if (a == 0) p1 = s.a;
        else if (b == 0) p1 = s.b;
        else line_intersection(l, Line(s.a, s.b), p1);
        p2 = p1;
        return 1;
    }
    db distance_line_line(const Line& a, const Line& b) { if (!line_valid(a) && !line_valid(b)) return dis(a.p, b.p); if (!line_valid(a)) return distance_point_line(a.p, b); if (!line_valid(b)) return distance_point_line(b.p, a); Point p; return line_intersection(a, b, p) != 0 ? 0 : distance_point_line(b.p, a); }
    db distance_line_segment(const Line& l, const Segment& s) { if (!line_valid(l)) return distance_point_segment(l.p, s); Point p1, p2; return line_segment_intersection(l, s, p1, p2) != 0 ? 0 : min(distance_point_line(s.a, l), distance_point_line(s.b, l)); }
    db distance_segment_segment(const Segment& a, const Segment& b) { if (segment_intersect(a, b)) return 0; db res = min(distance_point_segment(a.a, b), distance_point_segment(a.b, b)); return min(res, min(distance_point_segment(b.a, a), distance_point_segment(b.b, a))); }
    db line_angle(const Line& a, const Line& b) { db res = vector_angle(a.v, b.v); return min(res, PI - res); }
    db line_oriented_angle(const Line& a, const Line& b) { return oriented_angle(a.v, b.v); }
    int polar_half(const Point& p) { if (p.y > 0) return 0; if (p.y == 0 && p.x >= 0) return 0; return 1; }
    bool polar_less(const Point& a, const Point& b) { int ha = polar_half(a), hb = polar_half(b); if (ha != hb) return ha < hb; db c = cross(a, b); return c != 0 ? c > 0 : norm2(a) < norm2(b); }
    struct PolarCmp {
        Point o;
        PolarCmp(const Point& _o = Point()) : o(_o) {}
        bool operator()(const Point& a, const Point& b) const { return polar_less(a - o, b - o); }
    };
    void polar_sort(vector<Point>& p, const Point& o = Point()) { ranges::sort(p, PolarCmp(o)); }
    struct Circle {
        Point o;
        db r;
        Circle() : r(0) {}
        Circle(const Point& _o, db _r) : o(_o), r(_r) {}
        static Circle from_diameter(const Point& a, const Point& b) { return Circle(midpoint(a, b), dis(a, b) / 2); }
    };
    bool circle_valid(const Circle& c) { return sgn(c.r) >= 0; }
    db circle_area(const Circle& c) { return PI * c.r * c.r; }
    db circle_circumference(const Circle& c) { return 2 * PI * c.r; }
    db arc_length(const Circle& c, db rad) { return c.r * rad; }
    db sector_area(const Circle& c, db rad) { return c.r * c.r * rad / 2; }
    db chord_length(const Circle& c, db rad) { return 2 * c.r * sinl(rad / 2); }
    int point_circle_relation(const Point& p, const Circle& c) { return sgn(dis2(p, c.o) - c.r * c.r); }
    int line_circle_relation(const Line& l, const Circle& c) { if (!line_valid(l)) return point_circle_relation(l.p, c) == 0; int t = sgn(distance_point_line(c.o, l) - c.r); return t > 0 ? 0 : t == 0 ? 1 : 2; }
    int circle_circle_relation(const Circle& a, const Circle& b) { db d = dis(a.o, b.o); if (a.o == b.o && equal(a.r, b.r)) return 5; if (sgn(d - a.r - b.r) > 0) return 0; if (sgn(d - a.r - b.r) == 0) return 1; if (sgn(d - fabsl(a.r - b.r)) > 0) return 2; if (sgn(d - fabsl(a.r - b.r)) == 0) return 3; return 4; }
    int circle_centers(const Point& a, const Point& b, db r, Point& o1, Point& o2) {
        if (a == b) { if (sgn(r) != 0) return -1; o1 = o2 = a; return 1; }
        db d = dis(a, b);
        if (sgn(r) < 0 || sgn(d - 2 * r) > 0) return 0;
        Point m = midpoint(a, b);
        db h = safe_sqrt(r * r - d * d / 4);
        Point v = unit(rotate_90(b - a)) * h;
        o1 = m - v, o2 = m + v;
        return sgn(h) == 0 ? 1 : 2;
    }
    int line_circle_intersection(const Line& l, const Circle& c, Point& p1, Point& p2) {
        if (!line_valid(l)) { if (point_circle_relation(l.p, c) != 0) return 0; p1 = p2 = l.p; return 1; }
        Point h = project_point_line(c.o, l);
        db h2 = c.r * c.r - dis2(h, c.o);
        int t = sgn(h2);
        if (t < 0) return 0;
        if (t == 0) { p1 = p2 = h; return 1; }
        Point d = unit(l.v) * safe_sqrt(h2);
        p1 = h - d, p2 = h + d;
        return 2;
    }
    int segment_circle_intersection(const Segment& s, const Circle& c, Point& p1, Point& p2) {
        Point q1, q2;
        int cnt = line_circle_intersection(Line(s.a, s.b), c, q1, q2);
        if (cnt == 0) return 0;
        int res = 0;
        if (on_segment(q1, s)) p1 = q1, ++res;
        if (cnt == 2 && on_segment(q2, s) && (res == 0 || q2 != p1)) { if (res == 0) p1 = q2; else p2 = q2; ++res; }
        if (res == 1) p2 = p1;
        return res;
    }
    int circle_circle_intersection(const Circle& a, const Circle& b, Point& p1, Point& p2) {
        db d2 = dis2(a.o, b.o);
        if (a.o == b.o) { if (!equal(a.r, b.r)) return 0; if (sgn(a.r) != 0) return -1; p1 = p2 = a.o; return 1; }
        db d = safe_sqrt(d2);
        if (sgn(d - a.r - b.r) > 0 || sgn(d - fabsl(a.r - b.r)) < 0) return 0;
        db x = (d2 + a.r * a.r - b.r * b.r) / (2 * d);
        db h2 = a.r * a.r - x * x;
        Point u = (b.o - a.o) / d;
        Point h = a.o + u * x;
        if (sgn(h2) <= 0) { p1 = p2 = h; return 1; }
        Point v = rotate_90(u) * safe_sqrt(h2);
        p1 = h - v, p2 = h + v;
        return 2;
    }
    bool radical_axis(const Circle& a, const Circle& b, Line& l) { Point v = b.o - a.o; if (zero_vector(v)) return false; db rhs = norm2(b.o) - b.r * b.r - norm2(a.o) + a.r * a.r; return line_from_abc(v.x, v.y, -rhs / 2, l); }
    bool circle_from_three_points(const Point& a, const Point& b, const Point& c, Circle& res) { db d = 2 * cross(a, b, c); if (sgn(d) == 0) return false; db a2 = norm2(a), b2 = norm2(b), c2 = norm2(c); Point o((a2 * (b.y - c.y) + b2 * (c.y - a.y) + c2 * (a.y - b.y)) / d, (a2 * (c.x - b.x) + b2 * (a.x - c.x) + c2 * (b.x - a.x)) / d); res = Circle(o, dis(o, a)); return true; }
    int tangent_points(const Point& p, const Circle& c, Point& p1, Point& p2) {
        Point v = p - c.o;
        db d2 = norm2(v), r2 = c.r * c.r;
        int t = sgn(d2 - r2);
        if (t < 0) return 0;
        if (sgn(c.r) == 0) { p1 = p2 = c.o; return p == c.o ? -1 : 1; }
        if (t == 0) { p1 = p2 = p; return 1; }
        Point h = c.o + v * (r2 / d2);
        Point d = rotate_90(v) * (c.r * safe_sqrt(d2 - r2) / d2);
        p1 = h - d, p2 = h + d;
        return 2;
    }
    int common_tangents(const Circle& a, const Circle& b, vector<pair<Point, Point>>& res) {
        res.clear();
        Point d = b.o - a.o;
        db d2 = norm2(d);
        if (a.o == b.o && equal(a.r, b.r)) return -1;
        if (a.o == b.o) return 0;
        int cnt = 0;
        for (int type : {-1, 1}) {
            db r = a.r - type * b.r;
            db h2 = d2 - r * r;
            if (sgn(h2) < 0) continue;
            db h = safe_sqrt(h2);
            for (int side : {-1, 1}) { if (sgn(h) == 0 && side == 1) continue; Point v = (d * r + rotate_90(d) * (h * side)) / d2; res.emplace_back(a.o + v * a.r, b.o + v * (type * b.r)); ++cnt; }
        }
        return cnt;
    }
    struct Triangle {
        Point a, b, c;
        Triangle() {}
        Triangle(const Point& _a, const Point& _b, const Point& _c) : a(_a), b(_b), c(_c) {}
    };
    db triangle_area_signed(const Point& a, const Point& b, const Point& c) { return cross(a, b, c) / 2; }
    db triangle_area(const Point& a, const Point& b, const Point& c) { return fabsl(triangle_area_signed(a, b, c)); }
    db triangle_area(db a, db b, db c) { db s = (a + b + c) / 2; return safe_sqrt(s * (s - a) * (s - b) * (s - c)); }
    db triangle_area_signed(const Triangle& t) { return cross(t.a, t.b, t.c) / 2; }
    db triangle_area(const Triangle& t) { return fabsl(triangle_area_signed(t)); }
    db triangle_perimeter(const Triangle& t) { return dis(t.a, t.b) + dis(t.b, t.c) + dis(t.c, t.a); }
    bool triangle_valid(const Triangle& t) { return orientation(t.a, t.b, t.c) != 0; }
    db triangle_angle(const Point& a, const Point& b, const Point& c) { return vector_angle(a - b, c - b); }
    Point triangle_centroid(const Triangle& t) { return (t.a + t.b + t.c) / 3; }
    bool triangle_circumcircle(const Triangle& t, Circle& c) { return circle_from_three_points(t.a, t.b, t.c, c); }
    bool triangle_circumcenter(const Triangle& t, Point& p) { Circle c; if (!triangle_circumcircle(t, c)) return false; p = c.o; return true; }
    db triangle_circumradius(const Triangle& t) { db area = triangle_area(t); return sgn(area) == 0 ? 0 : dis(t.a, t.b) * dis(t.b, t.c) * dis(t.c, t.a) / (4 * area); }
    bool triangle_incircle(const Triangle& t, Circle& c) { db x = dis(t.b, t.c), y = dis(t.c, t.a), z = dis(t.a, t.b); db sum = x + y + z; if (sgn(sum) == 0 || !triangle_valid(t)) return false; Point o = (t.a * x + t.b * y + t.c * z) / sum; c = Circle(o, 2 * triangle_area(t) / sum); return true; }
    bool triangle_incenter(const Triangle& t, Point& p) { Circle c; if (!triangle_incircle(t, c)) return false; p = c.o; return true; }
    db triangle_inradius(const Triangle& t) { db perimeter = triangle_perimeter(t); return sgn(perimeter) == 0 ? 0 : 2 * triangle_area(t) / perimeter; }
    bool triangle_orthocenter(const Triangle& t, Point& p) { Point o; if (!triangle_circumcenter(t, o)) return false; p = t.a + t.b + t.c - o * 2; return true; }
    Line triangle_angle_bisector(const Point& a, const Point& b, const Point& c) { Point v = unit(a - b) + unit(c - b); if (zero_vector(v)) v = rotate_90(a - b); return Line::from_point_vector(b, v); }
    int point_triangle_relation(const Point& p, const Triangle& t) { if (!triangle_valid(t)) return on_segment(p, Segment(t.a, t.b)) || on_segment(p, Segment(t.b, t.c)) || on_segment(p, Segment(t.c, t.a)) ? 0 : -1; int a = orientation(t.a, t.b, p), b = orientation(t.b, t.c, p), c = orientation(t.c, t.a, p); if (a == 0 && on_segment(p, Segment(t.a, t.b))) return 0; if (b == 0 && on_segment(p, Segment(t.b, t.c))) return 0; if (c == 0 && on_segment(p, Segment(t.c, t.a))) return 0; bool pos = a > 0 || b > 0 || c > 0; bool neg = a < 0 || b < 0 || c < 0; return pos && neg ? -1 : 1; }

    vector<Point> convex_hull(vector<Point> p) {
        sort(p.begin(), p.end());
        vector<Point> q;
        for (auto& x : p) if (q.empty() || x != q.back()) q.push_back(x);
        if (q.size() <= 1) return q;
        vector<Point> h(q.size() * 2);
        int top = 0;
        for (auto& x : q) { while (top >= 2 && orientation(h[top - 2], h[top - 1], x) <= 0) --top; h[top++] = x; }
        int t = top + 1, n = q.size();
        for (int i = n - 2; i >= 0; --i) { while (top >= t && orientation(h[top - 2], h[top - 1], q[i]) <= 0) --top; h[top++] = q[i]; }
        h.resize(top - 1);
        return h;
    }
    vector<Point> upper_hull(vector<Point> p) {
        sort(p.begin(), p.end(), [](const Point& a, const Point& b) { return a.x != b.x ? a.x < b.x : a.y > b.y; });
        vector<Point> q, h;
        for (auto& x : p) if (q.empty() || x.x != q.back().x) q.push_back(x);
        for (auto& x : q) { while (h.size() >= 2 && orientation(h[h.size() - 2], h.back(), x) >= 0) h.pop_back(); h.push_back(x); }
        return h;
    }
    vector<Point> lower_hull(vector<Point> p) {
        sort(p.begin(), p.end());
        vector<Point> q, h;
        for (auto& x : p) if (q.empty() || x.x != q.back().x) q.push_back(x);
        for (auto& x : q) { while (h.size() >= 2 && orientation(h[h.size() - 2], h.back(), x) <= 0) h.pop_back(); h.push_back(x); }
        return h;
    }
    struct Polygon {
        vector<Point> p;
        Polygon() {}
        Polygon(const vector<Point>& _p) : p(convex_hull(_p)) {}
        int size() const { return p.size(); }
        bool empty() const { return p.empty(); }
        const Point& operator[](int i) const { return p[i]; }
        Point& operator[](int i) { return p[i]; }
    };
    bool polygon_valid(const Polygon& poly) { return poly.size() >= 3; }
    db polygon_perimeter(const Polygon& poly) { int n = poly.size(); if (n <= 1) return 0; db res = 0; for (int i = 0; i < n; ++i) res += dis(poly[i], poly[(i + 1) % n]); return res; }
    db polygon_area_signed(const Polygon& poly) { int n = poly.size(); if (n < 3) return 0; db res = 0; for (int i = 1; i + 1 < n; ++i) res += cross(poly[0], poly[i], poly[i + 1]); return res / 2; }
    db polygon_area(const Polygon& poly) { return fabsl(polygon_area_signed(poly)); }
    int point_polygon_relation(const Point& x, const Polygon& poly) {
        int n = poly.size();
        if (n == 0) return -1;
        if (n == 1) return x == poly[0] ? 0 : -1;
        if (n == 2) return on_segment(x, Segment(poly[0], poly[1])) ? 0 : -1;
        int a = orientation(poly[0], poly[1], x), b = orientation(poly[0], poly[n - 1], x);
        if (a < 0 || b > 0) return -1;
        if (a == 0) return on_segment(x, Segment(poly[0], poly[1])) ? 0 : -1;
        if (b == 0) return on_segment(x, Segment(poly[0], poly[n - 1])) ? 0 : -1;
        int l = 1, r = n - 1;
        while (r - l > 1) { int mid = (l + r) >> 1; if (orientation(poly[0], poly[mid], x) >= 0) l = mid; else r = mid; }
        int c = orientation(poly[l], poly[r], x);
        if (c < 0) return -1;
        return c == 0 ? 0 : 1;
    }
    bool segment_in_polygon(const Segment& s, const Polygon& poly) { return point_polygon_relation(s.a, poly) >= 0 && point_polygon_relation(s.b, poly) >= 0; }
    int segment_polygon_intersection(const Segment& s, const Polygon& poly, Point& p1, Point& p2) {
        vector<Point> q;
        auto add = [&](const Point& p) -> void { for (auto& x : q) if (x == p) return; q.push_back(p); };
        if (point_polygon_relation(s.a, poly) >= 0) add(s.a);
        if (point_polygon_relation(s.b, poly) >= 0) add(s.b);
        int n = poly.size();
        for (int i = 0; i < n; ++i) { Point x, y; int t = segment_intersection(s, Segment(poly[i], poly[(i + 1) % n]), x, y); if (t >= 1) add(x); if (t == 2) add(y); }
        if (q.empty()) return 0;
        Point v = s.b - s.a;
        if (zero_vector(v)) { p1 = p2 = q[0]; return 1; }
        int mn = 0, mx = 0;
        int m = q.size();
        for (int i = 1; i < m; ++i) { if (dot(q[i] - s.a, v) < dot(q[mn] - s.a, v)) mn = i; if (dot(q[i] - s.a, v) > dot(q[mx] - s.a, v)) mx = i; }
        p1 = q[mn], p2 = q[mx];
        return p1 == p2 ? 1 : 2;
    }
    bool segment_polygon_intersect(const Segment& s, const Polygon& poly) { Point p1, p2; return segment_polygon_intersection(s, poly, p1, p2) != 0; }

    int cmp_fraction(ll a, ll b, ll c, ll d) {
        int rev = 1;
        while (true) {
            ll x = a / b, y = c / d;
            if (x != y) return rev * (x < y ? -1 : 1);
            a %= b, c %= d;
            if (a == 0 || c == 0) return rev * ((a > 0) - (c > 0));
            swap(a, b), swap(c, d);
            rev = -rev;
        }
    }
    int cmp_product(ll a, ll b, ll c, ll d) { bool x = a == 0 || b == 0, y = c == 0 || d == 0; if (x || y) return (!x) - (!y); return cmp_fraction(a, c, d, b); }

    struct IPoint {
        ll x, y;
        IPoint() : x(0), y(0) {}
        IPoint(ll _x, ll _y) : x(_x), y(_y) {}
        IPoint operator+() const { return *this; }
        IPoint operator-() const { return IPoint(-x, -y); }
        IPoint operator+(const IPoint& o) const { return IPoint(x + o.x, y + o.y); }
        IPoint operator-(const IPoint& o) const { return IPoint(x - o.x, y - o.y); }
        IPoint& operator+=(const IPoint& o) { x += o.x, y += o.y; return *this; }
        IPoint& operator-=(const IPoint& o) { x -= o.x, y -= o.y; return *this; }
        bool operator==(const IPoint& o) const { return x == o.x && y == o.y; }
        bool operator!=(const IPoint& o) const { return !(*this == o); }
        bool operator<(const IPoint& o) const { return x != o.x ? x < o.x : y < o.y; }
    };
    istream& operator>>(istream& is, IPoint& p) { return is >> p.x >> p.y; }
    ostream& operator<<(ostream& os, const IPoint& p) { return os << p.x << ' ' << p.y; }
    ll dot(const IPoint& a, const IPoint& b) { return a.x * b.x + a.y * b.y; }
    ll dot(const IPoint& a, const IPoint& b, const IPoint& c) { return dot(b - a, c - a); }
    ll cross(const IPoint& a, const IPoint& b) { return a.x * b.y - a.y * b.x; }
    ll cross(const IPoint& a, const IPoint& b, const IPoint& c) { return cross(b - a, c - a); }
    ll norm2(const IPoint& a) { return dot(a, a); }
    ll dis2(const IPoint& a, const IPoint& b) { return norm2(a - b); }
    db norm(const IPoint& a) { return hypotl(a.x, a.y); }
    db dis(const IPoint& a, const IPoint& b) { return hypotl(a.x - b.x, a.y - b.y); }
    bool zero_vector(const IPoint& a) { return a.x == 0 && a.y == 0; }
    IPoint rotate_90(const IPoint& a) { return IPoint(-a.y, a.x); }
    bool parallel(const IPoint& a, const IPoint& b) { return cross(a, b) == 0; }
    bool perpendicular(const IPoint& a, const IPoint& b) { return dot(a, b) == 0; }
    int orientation(const IPoint& a, const IPoint& b, const IPoint& c) { ll v = cross(a, b, c); return (v > 0) - (v < 0); }
    bool collinear(const IPoint& a, const IPoint& b, const IPoint& c) { return cross(a, b, c) == 0; }
    Point to_point(const IPoint& p) { return Point(p.x, p.y); }
    Point midpoint(const IPoint& a, const IPoint& b) { return (to_point(a) + to_point(b)) / 2; }
    Point unit(const IPoint& a) { db len = norm(a); return len == 0 ? Point() : to_point(a) / len; }
    Point rotate(const IPoint& a, db rad) { return rotate(to_point(a), rad); }
    db polar_angle(const IPoint& a) { return atan2l(a.y, a.x); }
    db vector_angle(const IPoint& a, const IPoint& b) { return zero_vector(a) || zero_vector(b) ? 0 : atan2l(fabsl((db)cross(a, b)), (db)dot(a, b)); }
    db oriented_angle(const IPoint& a, const IPoint& b) { return zero_vector(a) || zero_vector(b) ? 0 : atan2l((db)cross(a, b), (db)dot(a, b)); }
    db ccw_angle(const IPoint& a, const IPoint& b) { return normalize_angle(oriented_angle(a, b)); }
    db project_length(const IPoint& a, const IPoint& b, const IPoint& p) { IPoint v = b - a; ll len2 = norm2(v); return len2 == 0 ? 0 : (db)dot(p - a, v) / safe_sqrt(len2); }
    struct ILine {
        IPoint p, v;
        ILine() {}
        ILine(const IPoint& a, const IPoint& b) : p(a), v(b - a) {}
        static ILine from_point_vector(const IPoint& p, const IPoint& v) { ILine l; l.p = p, l.v = v; return l; }
    };
    struct ISegment {
        IPoint a, b;
        ISegment() {}
        ISegment(const IPoint& _a, const IPoint& _b) : a(_a), b(_b) {}
    };
    Line to_line(const ILine& l) { return Line::from_point_vector(to_point(l.p), to_point(l.v)); }
    Segment to_segment(const ISegment& s) { return Segment(to_point(s.a), to_point(s.b)); }
    bool line_valid(const ILine& l) { return !zero_vector(l.v); }
    bool segment_valid(const ISegment& s) { return s.a != s.b; }
    bool line_from_abc(ll a, ll b, ll c, Line& l) { if (a == 0 && b == 0) return false; Point p = llabs(a) > llabs(b) ? Point(-(db)c / a, 0) : Point(0, -(db)c / b); l = Line::from_point_vector(p, Point(-b, a)); return true; }
    Point line_normal(const ILine& l) { return unit(rotate_90(l.v)); }
    ILine parallel_line(const ILine& l, const IPoint& p) { return ILine::from_point_vector(p, l.v); }
    ILine perpendicular_line(const ILine& l, const IPoint& p) { return ILine::from_point_vector(p, rotate_90(l.v)); }
    Line perpendicular_bisector(const IPoint& a, const IPoint& b) { return Line::from_point_vector(midpoint(a, b), to_point(rotate_90(b - a))); }
    bool on_line(const IPoint& p, const ILine& l) { return line_valid(l) ? cross(l.v, p - l.p) == 0 : p == l.p; }
    bool on_segment(const IPoint& p, const ISegment& s) { return cross(s.a, s.b, p) == 0 && dot(p - s.a, p - s.b) <= 0; }
    bool on_ray(const IPoint& p, const IPoint& o, const IPoint& v) { return zero_vector(v) ? p == o : cross(v, p - o) == 0 && dot(v, p - o) >= 0; }
    int side_of_line(const IPoint& p, const ILine& l) { if (!line_valid(l)) return 0; ll v = cross(l.v, p - l.p); return (v > 0) - (v < 0); }
    db distance_point_line_signed(const IPoint& p, const ILine& l) { return line_valid(l) ? (db)cross(l.v, p - l.p) / norm(l.v) : 0; }
    db distance_point_line(const IPoint& p, const ILine& l) { return line_valid(l) ? fabsl(distance_point_line_signed(p, l)) : dis(p, l.p); }
    Point project_point_line(const IPoint& p, const ILine& l) { if (!line_valid(l)) return to_point(l.p); db t = (db)dot(p - l.p, l.v) / norm2(l.v); return to_point(l.p) + to_point(l.v) * t; }
    Point reflect_point_line(const IPoint& p, const ILine& l) { return project_point_line(p, l) * 2 - to_point(p); }
    Point closest_point_segment(const IPoint& p, const ISegment& s) { IPoint v = s.b - s.a; ll len2 = norm2(v); if (len2 == 0) return to_point(s.a); db t = (db)dot(p - s.a, v) / len2; t = max<db>(0, min<db>(1, t)); return to_point(s.a) + to_point(v) * t; }
    db distance_point_segment(const IPoint& p, const ISegment& s) { return dis(to_point(p), closest_point_segment(p, s)); }
    bool line_parallel(const ILine& a, const ILine& b) { return cross(a.v, b.v) == 0; }
    bool line_coincident(const ILine& a, const ILine& b) { return line_valid(a) && line_valid(b) && line_parallel(a, b) && cross(a.v, b.p - a.p) == 0; }
    int line_intersection(const ILine& a, const ILine& b, Point& p) {
        bool va = line_valid(a), vb = line_valid(b);
        if (!va && !vb) { if (a.p != b.p) return 0; p = to_point(a.p); return 1; }
        if (!va) { if (!on_line(a.p, b)) return 0; p = to_point(a.p); return 1; }
        if (!vb) { if (!on_line(b.p, a)) return 0; p = to_point(b.p); return 1; }
        ll d = cross(a.v, b.v);
        if (d == 0) return cross(a.v, b.p - a.p) == 0 ? 2 : 0;
        db t = (db)cross(b.p - a.p, b.v) / d;
        p = to_point(a.p) + to_point(a.v) * t;
        return 1;
    }
    bool segment_strict_intersect(const ISegment& a, const ISegment& b) { int c1 = orientation(a.a, a.b, b.a), c2 = orientation(a.a, a.b, b.b); int c3 = orientation(b.a, b.b, a.a), c4 = orientation(b.a, b.b, a.b); return c1 * c2 < 0 && c3 * c4 < 0; }
    bool segment_intersect(const ISegment& a, const ISegment& b) { if (segment_strict_intersect(a, b)) return true; return on_segment(a.a, b) || on_segment(a.b, b) || on_segment(b.a, a) || on_segment(b.b, a); }
    int segment_intersection(const ISegment& a, const ISegment& b, Point& p1, Point& p2) {
        IPoint q[4];
        int cnt = 0;
        auto add = [&](const IPoint& p) -> void { for (int i = 0; i < cnt; ++i) if (q[i] == p) return; q[cnt++] = p; };
        if (on_segment(a.a, b)) add(a.a);
        if (on_segment(a.b, b)) add(a.b);
        if (on_segment(b.a, a)) add(b.a);
        if (on_segment(b.b, a)) add(b.b);
        if (cnt) { IPoint x = q[0], y = q[0]; for (int i = 1; i < cnt; ++i) x = min(x, q[i]), y = max(y, q[i]); p1 = to_point(x), p2 = to_point(y); return x == y ? 1 : 2; }
        if (!segment_strict_intersect(a, b)) return 0;
        line_intersection(ILine(a.a, a.b), ILine(b.a, b.b), p1);
        p2 = p1;
        return 1;
    }
    int line_segment_intersection(const ILine& l, const ISegment& s, Point& p1, Point& p2) {
        if (!line_valid(l)) { if (!on_segment(l.p, s)) return 0; p1 = p2 = to_point(l.p); return 1; }
        ll x = cross(l.v, s.a - l.p), y = cross(l.v, s.b - l.p);
        if (x == 0 && y == 0) { p1 = to_point(s.a), p2 = to_point(s.b); return s.a == s.b ? 1 : 2; }
        if ((x > 0 && y > 0) || (x < 0 && y < 0)) return 0;
        if (x == 0) p1 = to_point(s.a);
        else if (y == 0) p1 = to_point(s.b);
        else line_intersection(l, ILine(s.a, s.b), p1);
        p2 = p1;
        return 1;
    }
    db distance_line_line(const ILine& a, const ILine& b) { if (!line_valid(a) && !line_valid(b)) return dis(a.p, b.p); if (!line_valid(a)) return distance_point_line(a.p, b); if (!line_valid(b)) return distance_point_line(b.p, a); Point p; return line_intersection(a, b, p) != 0 ? 0 : distance_point_line(b.p, a); }
    db distance_line_segment(const ILine& l, const ISegment& s) { if (!line_valid(l)) return distance_point_segment(l.p, s); Point p1, p2; return line_segment_intersection(l, s, p1, p2) != 0 ? 0 : min(distance_point_line(s.a, l), distance_point_line(s.b, l)); }
    db distance_segment_segment(const ISegment& a, const ISegment& b) { if (segment_intersect(a, b)) return 0; db res = min(distance_point_segment(a.a, b), distance_point_segment(a.b, b)); return min(res, min(distance_point_segment(b.a, a), distance_point_segment(b.b, a))); }
    db line_angle(const ILine& a, const ILine& b) { db res = vector_angle(a.v, b.v); return min(res, PI - res); }
    db line_oriented_angle(const ILine& a, const ILine& b) { return oriented_angle(a.v, b.v); }
    int polar_half(const IPoint& p) { if (p.y != 0) return p.y < 0; return p.x < 0; }
    bool polar_less(const IPoint& a, const IPoint& b) { int ha = polar_half(a), hb = polar_half(b); if (ha != hb) return ha < hb; ll c = cross(a, b); return c != 0 ? c > 0 : norm2(a) < norm2(b); }
    struct IPolarCmp {
        IPoint o;
        IPolarCmp(const IPoint& _o = IPoint()) : o(_o) {}
        bool operator()(const IPoint& a, const IPoint& b) const { return polar_less(a - o, b - o); }
    };
    void polar_sort(vector<IPoint>& p, const IPoint& o = IPoint()) { ranges::sort(p, IPolarCmp(o)); }
    struct ICircle {
        IPoint o;
        ll r;
        ICircle() : r(0) {}
        ICircle(const IPoint& _o, ll _r) : o(_o), r(_r) {}
    };
    Circle to_circle(const ICircle& c) { return Circle(to_point(c.o), c.r); }
    bool circle_valid(const ICircle& c) { return c.r >= 0; }
    db circle_area(const ICircle& c) { return PI * (db)c.r * c.r; }
    db circle_circumference(const ICircle& c) { return 2 * PI * c.r; }
    db arc_length(const ICircle& c, db rad) { return c.r * rad; }
    db sector_area(const ICircle& c, db rad) { return (db)c.r * c.r * rad / 2; }
    db chord_length(const ICircle& c, db rad) { return 2 * c.r * sinl(rad / 2); }
    int point_circle_relation(const IPoint& p, const ICircle& c) { ll v = dis2(p, c.o) - c.r * c.r; return (v > 0) - (v < 0); }
    int line_circle_relation(const ILine& l, const ICircle& c) {
        if (!line_valid(l)) return point_circle_relation(l.p, c) == 0;
        ll x = llabs(cross(l.v, c.o - l.p)), a = norm2(l.v), r2 = c.r * c.r;
        int t = cmp_product(x, x, r2, a);
        return t > 0 ? 0 : t == 0 ? 1 : 2;
    }
    int circle_circle_relation(const ICircle& a, const ICircle& b) { ll d2 = dis2(a.o, b.o); if (d2 == 0 && a.r == b.r) return 5; ll sum = a.r + b.r, dif = llabs(a.r - b.r); if (d2 > sum * sum) return 0; if (d2 == sum * sum) return 1; if (d2 > dif * dif) return 2; if (d2 == dif * dif) return 3; return 4; }
    int circle_centers(const IPoint& a, const IPoint& b, ll r, Point& o1, Point& o2) {
        if (a == b) { if (r != 0) return -1; o1 = o2 = to_point(a); return 1; }
        if (r < 0) return 0;
        ll d2 = dis2(a, b), rr = 4 * r * r;
        if (d2 > rr) return 0;
        Point m = midpoint(a, b);
        db h = safe_sqrt((db)r * r - (db)d2 / 4);
        Point v = unit(rotate_90(b - a)) * h;
        o1 = m - v, o2 = m + v;
        return d2 == rr ? 1 : 2;
    }
    int line_circle_intersection(const ILine& l, const ICircle& c, Point& p1, Point& p2) {
        if (!line_valid(l)) { if (point_circle_relation(l.p, c) != 0) return 0; p1 = p2 = to_point(l.p); return 1; }
        int cnt = line_circle_relation(l, c);
        if (cnt == 0) return 0;
        IPoint u = c.o - l.p;
        ll a = norm2(l.v), b = dot(u, l.v), x = cross(l.v, u);
        Point h = to_point(l.p) + to_point(l.v) * ((db)b / a);
        if (cnt == 1) { p1 = p2 = h; return 1; }
        db h2 = (db)c.r * c.r - (db)x * x / a;
        Point d = unit(l.v) * safe_sqrt(h2);
        p1 = h - d, p2 = h + d;
        return 2;
    }
    int segment_circle_relation(const ISegment& s, const ICircle& c) {
        IPoint v = s.b - s.a;
        ll a = norm2(v);
        if (a == 0) return point_circle_relation(s.a, c) == 0;
        ll r2 = c.r * c.r, x = dis2(s.a, c.o) - r2, y = dis2(s.b, c.o) - r2;
        int sx = (x > 0) - (x < 0), sy = (y > 0) - (y < 0);
        if (sx == 0 && sy == 0) return 2;
        ll d = dot(s.a - c.o, v);
        if (sx == 0) { if (d >= 0) return 1; return -d <= a / 2 ? 2 : 1; }
        if (sy == 0) return x > 0 && x < a ? 2 : 1;
        if (sx != sy) return 1;
        if (sx < 0 || d >= 0 || -d >= a) return 0;
        int t = cmp_product(-d, -d, a, x);
        return t < 0 ? 0 : t == 0 ? 1 : 2;
    }
    int segment_circle_intersection(const ISegment& s, const ICircle& c, Point& p1, Point& p2) {
        int cnt = segment_circle_relation(s, c);
        if (cnt == 0) return 0;
        if (!segment_valid(s)) { p1 = p2 = to_point(s.a); return 1; }
        int x = point_circle_relation(s.a, c), y = point_circle_relation(s.b, c);
        if (x == 0 && y == 0) { p1 = to_point(s.a), p2 = to_point(s.b); return 2; }
        Point q1, q2;
        line_circle_intersection(ILine(s.a, s.b), c, q1, q2);
        if (x == 0) { p1 = to_point(s.a), p2 = cnt == 1 ? p1 : q2; return cnt; }
        if (y == 0) { p1 = cnt == 1 ? to_point(s.b) : q1, p2 = to_point(s.b); return cnt; }
        if (cnt == 2) { p1 = q1, p2 = q2; return 2; }
        p1 = p2 = x < 0 ? q2 : q1;
        return 1;
    }
    int circle_circle_intersection(const ICircle& a, const ICircle& b, Point& p1, Point& p2) {
        int rel = circle_circle_relation(a, b);
        if (rel == 5) { if (a.r != 0) return -1; p1 = p2 = to_point(a.o); return 1; }
        if (rel == 0 || rel == 4) return 0;
        db d2 = dis2(a.o, b.o), d = safe_sqrt(d2);
        db x = (d2 + (db)a.r * a.r - (db)b.r * b.r) / (2 * d);
        Point u = (to_point(b.o) - to_point(a.o)) / d;
        Point h = to_point(a.o) + u * x;
        if (rel == 1 || rel == 3) { p1 = p2 = h; return 1; }
        Point v = rotate_90(u) * safe_sqrt((db)a.r * a.r - x * x);
        p1 = h - v, p2 = h + v;
        return 2;
    }
    bool radical_axis(const ICircle& a, const ICircle& b, Line& l) {
        IPoint d = b.o - a.o;
        ll d2 = norm2(d);
        if (d2 == 0) return false;
        ll x = d2 + a.r * a.r - b.r * b.r;
        Point p = to_point(a.o) + to_point(d) * ((db)x / (2 * (db)d2));
        l = Line::from_point_vector(p, to_point(rotate_90(d)));
        return true;
    }
    bool circle_from_three_points(const IPoint& a, const IPoint& b, const IPoint& c, Circle& res) {
        IPoint u = b - a, v = c - a;
        ll cr = cross(u, v);
        if (cr == 0) return false;
        ll u2 = norm2(u), v2 = norm2(v);
        db d = 2 * (db)cr;
        Point o = to_point(a) + Point(((db)u2 * v.y - (db)v2 * u.y) / d, ((db)v2 * u.x - (db)u2 * v.x) / d);
        res = Circle(o, dis(o, to_point(a)));
        return true;
    }
    int tangent_points(const IPoint& p, const ICircle& c, Point& p1, Point& p2) {
        int rel = point_circle_relation(p, c);
        if (rel < 0) return 0;
        if (c.r == 0) { p1 = p2 = to_point(c.o); return p == c.o ? -1 : 1; }
        if (rel == 0) { p1 = p2 = to_point(p); return 1; }
        IPoint iv = p - c.o;
        db d2 = norm2(iv), r2 = (db)c.r * c.r;
        Point v = to_point(iv), h = to_point(c.o) + v * (r2 / d2);
        Point d = rotate_90(v) * (c.r * safe_sqrt(d2 - r2) / d2);
        p1 = h - d, p2 = h + d;
        return 2;
    }
    int common_tangents(const ICircle& a, const ICircle& b, vector<pair<Point, Point>>& res) {
        res.clear();
        IPoint id = b.o - a.o;
        ll d2 = norm2(id);
        if (d2 == 0 && a.r == b.r) return -1;
        if (d2 == 0) return 0;
        Point d = to_point(id);
        int cnt = 0;
        for (int type : {-1, 1}) {
            ll r = a.r - type * b.r, h2 = d2 - r * r;
            if (h2 < 0) continue;
            db h = safe_sqrt(h2);
            for (int side : {-1, 1}) { if (h2 == 0 && side == 1) continue; Point v = (d * r + rotate_90(d) * (h * side)) / d2; res.emplace_back(to_point(a.o) + v * a.r, to_point(b.o) + v * (type * b.r)); ++cnt; }
        }
        return cnt;
    }
    struct ITriangle {
        IPoint a, b, c;
        ITriangle() {}
        ITriangle(const IPoint& _a, const IPoint& _b, const IPoint& _c) : a(_a), b(_b), c(_c) {}
    };
    Triangle to_triangle(const ITriangle& t) { return Triangle(to_point(t.a), to_point(t.b), to_point(t.c)); }
    ll triangle_area2_signed(const IPoint& a, const IPoint& b, const IPoint& c) { return cross(a, b, c); }
    ll triangle_area2(const IPoint& a, const IPoint& b, const IPoint& c) { return llabs(triangle_area2_signed(a, b, c)); }
    ll triangle_area2_signed(const ITriangle& t) { return cross(t.a, t.b, t.c); }
    ll triangle_area2(const ITriangle& t) { return llabs(triangle_area2_signed(t)); }
    db triangle_area_signed(const IPoint& a, const IPoint& b, const IPoint& c) { return (db)triangle_area2_signed(a, b, c) / 2; }
    db triangle_area(const IPoint& a, const IPoint& b, const IPoint& c) { return (db)triangle_area2(a, b, c) / 2; }
    db triangle_area_signed(const ITriangle& t) { return (db)triangle_area2_signed(t) / 2; }
    db triangle_area(const ITriangle& t) { return (db)triangle_area2(t) / 2; }
    db triangle_perimeter(const ITriangle& t) { return dis(t.a, t.b) + dis(t.b, t.c) + dis(t.c, t.a); }
    bool triangle_valid(const ITriangle& t) { return cross(t.a, t.b, t.c) != 0; }
    db triangle_angle(const IPoint& a, const IPoint& b, const IPoint& c) { return vector_angle(a - b, c - b); }
    Point triangle_centroid(const ITriangle& t) { return (to_point(t.a) + to_point(t.b) + to_point(t.c)) / 3; }
    bool triangle_circumcircle(const ITriangle& t, Circle& c) { return triangle_valid(t) && triangle_circumcircle(to_triangle(t), c); }
    bool triangle_circumcenter(const ITriangle& t, Point& p) { Circle c; if (!triangle_circumcircle(t, c)) return false; p = c.o; return true; }
    db triangle_circumradius(const ITriangle& t) { return triangle_circumradius(to_triangle(t)); }
    bool triangle_incircle(const ITriangle& t, Circle& c) { return triangle_valid(t) && triangle_incircle(to_triangle(t), c); }
    bool triangle_incenter(const ITriangle& t, Point& p) { Circle c; if (!triangle_incircle(t, c)) return false; p = c.o; return true; }
    db triangle_inradius(const ITriangle& t) { return triangle_inradius(to_triangle(t)); }
    bool triangle_orthocenter(const ITriangle& t, Point& p) { return triangle_valid(t) && triangle_orthocenter(to_triangle(t), p); }
    Line triangle_angle_bisector(const IPoint& a, const IPoint& b, const IPoint& c) { return triangle_angle_bisector(to_point(a), to_point(b), to_point(c)); }
    int point_triangle_relation(const IPoint& p, const ITriangle& t) { if (!triangle_valid(t)) return on_segment(p, ISegment(t.a, t.b)) || on_segment(p, ISegment(t.b, t.c)) || on_segment(p, ISegment(t.c, t.a)) ? 0 : -1; int a = orientation(t.a, t.b, p), b = orientation(t.b, t.c, p), c = orientation(t.c, t.a, p); if (a == 0 && on_segment(p, ISegment(t.a, t.b))) return 0; if (b == 0 && on_segment(p, ISegment(t.b, t.c))) return 0; if (c == 0 && on_segment(p, ISegment(t.c, t.a))) return 0; bool pos = a > 0 || b > 0 || c > 0; bool neg = a < 0 || b < 0 || c < 0; return pos && neg ? -1 : 1; }

    vector<IPoint> convex_hull(vector<IPoint> p) {
        sort(p.begin(), p.end());
        p.erase(unique(p.begin(), p.end()), p.end());
        if (p.size() <= 1) return p;
        vector<IPoint> h(p.size() * 2);
        int top = 0;
        for (auto& x : p) { while (top >= 2 && orientation(h[top - 2], h[top - 1], x) <= 0) --top; h[top++] = x; }
        int t = top + 1, n = p.size();
        for (int i = n - 2; i >= 0; --i) { while (top >= t && orientation(h[top - 2], h[top - 1], p[i]) <= 0) --top; h[top++] = p[i]; }
        h.resize(top - 1);
        return h;
    }
    vector<IPoint> upper_hull(vector<IPoint> p) {
        sort(p.begin(), p.end(), [](const IPoint& a, const IPoint& b) { return a.x != b.x ? a.x < b.x : a.y > b.y; });
        vector<IPoint> q, h;
        for (auto& x : p) if (q.empty() || x.x != q.back().x) q.push_back(x);
        for (auto& x : q) { while (h.size() >= 2 && orientation(h[h.size() - 2], h.back(), x) >= 0) h.pop_back(); h.push_back(x); }
        return h;
    }
    vector<IPoint> lower_hull(vector<IPoint> p) {
        sort(p.begin(), p.end());
        vector<IPoint> q, h;
        for (auto& x : p) if (q.empty() || x.x != q.back().x) q.push_back(x);
        for (auto& x : q) { while (h.size() >= 2 && orientation(h[h.size() - 2], h.back(), x) <= 0) h.pop_back(); h.push_back(x); }
        return h;
    }
    vector<IPoint> upper_right_hull(vector<IPoint> p) {
        auto h = upper_hull(p);
        int id = 0; for (int i = 1; i < h.size(); ++i) if (h[i].y >= h[id].y) id = i;
        h.erase(h.begin(), h.begin() + id); return h;
    }
    vector<IPoint> upper_left_hull(vector<IPoint> p) {
        auto h = upper_hull(p);
        int id = 0; for (int i = 1; i < h.size(); ++i) if (h[i].y > h[id].y) id = i;
        h.resize(id + 1); return h;
    }
    vector<IPoint> lower_right_hull(vector<IPoint> p) {
        auto h = lower_hull(p);
        int id = 0; for (int i = 1; i < h.size(); ++i) if (h[i].y <= h[id].y) id = i;
        h.erase(h.begin(), h.begin() + id); return h;
    }
    vector<IPoint> lower_left_hull(vector<IPoint> p) {
        auto h = lower_hull(p);
        int id = 0; for (int i = 1; i < h.size(); ++i) if (h[i].y < h[id].y) id = i;
        h.resize(id + 1); return h;
    }
    struct IPolygon {
        vector<IPoint> p;
        IPolygon() {}
        IPolygon(const vector<IPoint>& _p) : p(convex_hull(_p)) {}
        int size() const { return p.size(); }
        bool empty() const { return p.empty(); }
        const IPoint& operator[](int i) const { return p[i]; }
        IPoint& operator[](int i) { return p[i]; }
    };
    Polygon to_polygon(const IPolygon& poly) { Polygon res; for (auto& x : poly.p) res.p.push_back(to_point(x)); return res; }
    bool polygon_valid(const IPolygon& poly) { return poly.size() >= 3; }
    db polygon_perimeter(const IPolygon& poly) { int n = poly.size(); if (n <= 1) return 0; db res = 0; for (int i = 0; i < n; ++i) res += dis(poly[i], poly[(i + 1) % n]); return res; }
    ll polygon_area2_signed(const IPolygon& poly) { int n = poly.size(); if (n < 3) return 0; ll res = 0; for (int i = 1; i + 1 < n; ++i) res += cross(poly[0], poly[i], poly[i + 1]); return res; }
    ll polygon_area2(const IPolygon& poly) { return llabs(polygon_area2_signed(poly)); }
    db polygon_area_signed(const IPolygon& poly) { return (db)polygon_area2_signed(poly) / 2; }
    db polygon_area(const IPolygon& poly) { return (db)polygon_area2(poly) / 2; }
    int point_polygon_relation(const IPoint& x, const IPolygon& poly) {
        int n = poly.size();
        if (n == 0) return -1;
        if (n == 1) return x == poly[0] ? 0 : -1;
        if (n == 2) return on_segment(x, ISegment(poly[0], poly[1])) ? 0 : -1;
        int a = orientation(poly[0], poly[1], x), b = orientation(poly[0], poly[n - 1], x);
        if (a < 0 || b > 0) return -1;
        if (a == 0) return on_segment(x, ISegment(poly[0], poly[1])) ? 0 : -1;
        if (b == 0) return on_segment(x, ISegment(poly[0], poly[n - 1])) ? 0 : -1;
        int l = 1, r = n - 1;
        while (r - l > 1) { int mid = (l + r) >> 1; if (orientation(poly[0], poly[mid], x) >= 0) l = mid; else r = mid; }
        int c = orientation(poly[l], poly[r], x);
        if (c < 0) return -1;
        return c == 0 ? 0 : 1;
    }
    bool segment_in_polygon(const ISegment& s, const IPolygon& poly) { return point_polygon_relation(s.a, poly) >= 0 && point_polygon_relation(s.b, poly) >= 0; }
    int segment_polygon_intersection(const ISegment& s, const IPolygon& poly, Point& p1, Point& p2) {
        vector<Point> q;
        auto add = [&](const Point& p) -> void { for (auto& x : q) if (x == p) return; q.push_back(p); };
        if (point_polygon_relation(s.a, poly) >= 0) add(to_point(s.a));
        if (point_polygon_relation(s.b, poly) >= 0) add(to_point(s.b));
        int n = poly.size();
        for (int i = 0; i < n; ++i) { Point x, y; int t = segment_intersection(s, ISegment(poly[i], poly[(i + 1) % n]), x, y); if (t >= 1) add(x); if (t == 2) add(y); }
        if (q.empty()) return 0;
        IPoint iv = s.b - s.a;
        if (zero_vector(iv)) { p1 = p2 = q[0]; return 1; }
        Point a = to_point(s.a), v = to_point(iv);
        int mn = 0, mx = 0;
        int m = q.size();
        for (int i = 1; i < m; ++i) { if (dot(q[i] - a, v) < dot(q[mn] - a, v)) mn = i; if (dot(q[i] - a, v) > dot(q[mx] - a, v)) mx = i; }
        p1 = q[mn], p2 = q[mx];
        return p1 == p2 ? 1 : 2;
    }
    bool segment_polygon_intersect(const ISegment& s, const IPolygon& poly) { Point p1, p2; return segment_polygon_intersection(s, poly, p1, p2) != 0; }
    int line_polygon_intersection(const Line& l, const Polygon& poly, Point& p1, Point& p2) {
        if (!line_valid(l)) {
            if (point_polygon_relation(l.p, poly) < 0) return 0;
            p1 = p2 = l.p;
            return 1;
        }
        vector<Point> q;
        auto add = [&](const Point& p) -> void {
            for (auto& x : q) if (x == p) return;
            q.push_back(p);
        };
        int n = poly.size();
        for (int i = 0; i < n; ++i) {
            Point x, y;
            int t = line_segment_intersection(l, Segment(poly[i], poly[(i + 1) % n]), x, y);
            if (t >= 1) add(x);
            if (t == 2) add(y);
        }
        if (q.empty()) return 0;
        int mn = 0, mx = 0;
        for (int i = 1; i < q.size(); ++i) {
            if (dot(q[i] - l.p, l.v) < dot(q[mn] - l.p, l.v)) mn = i;
            if (dot(q[i] - l.p, l.v) > dot(q[mx] - l.p, l.v)) mx = i;
        }
        p1 = q[mn], p2 = q[mx];
        return p1 == p2 ? 1 : 2;
    }
    bool line_polygon_intersect(const Line& l, const Polygon& poly) { Point p1, p2; return line_polygon_intersection(l, poly, p1, p2) != 0; }
    int line_polygon_intersection(const ILine& l, const IPolygon& poly, Point& p1, Point& p2) {
        if (!line_valid(l)) {
            if (point_polygon_relation(l.p, poly) < 0) return 0;
            p1 = p2 = to_point(l.p);
            return 1;
        }
        vector<Point> q;
        auto add = [&](const Point& p) -> void {
            for (auto& x : q) if (x == p) return;
            q.push_back(p);
        };
        int n = poly.size();
        for (int i = 0; i < n; ++i) {
            Point x, y;
            int t = line_segment_intersection(l, ISegment(poly[i], poly[(i + 1) % n]), x, y);
            if (t >= 1) add(x);
            if (t == 2) add(y);
        }
        if (q.empty()) return 0;
        Point a = to_point(l.p), v = to_point(l.v);
        int mn = 0, mx = 0;
        for (int i = 1; i < q.size(); ++i) {
            if (dot(q[i] - a, v) < dot(q[mn] - a, v)) mn = i;
            if (dot(q[i] - a, v) > dot(q[mx] - a, v)) mx = i;
        }
        p1 = q[mn], p2 = q[mx];
        return p1 == p2 ? 1 : 2;
    }
    bool line_polygon_intersect(const ILine& l, const IPolygon& poly) { Point p1, p2; return line_polygon_intersection(l, poly, p1, p2) != 0; }
} using namespace Geometry;