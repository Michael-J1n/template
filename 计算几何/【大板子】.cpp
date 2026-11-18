#include <bits/stdc++.h>
using namespace std;

/**
 * 【计算几何综合模板】
 * 使用 long double，默认误差 EPS=1e-12L
 * 所有角度参数和返回值均为弧度
 * Line 内部表示为 p+v*t，Line(a,b) 表示经过 a、b 的直线
 * Segment(a,b) 表示闭线段 [a,b]
 * Circle(o,r) 表示圆心为 o、半径为 r 的圆
 * Triangle(a,b,c) 表示三点构成的三角形
 * 
 * 【一：基础方法】
 * sgn(x)：按 EPS 判断 x 的符号
 * equal(a,b)：判断两个浮点数是否相等
 * sqr(x)：返回 x^2
 * safe_sqrt(x)：将负误差截断为 0 后开方
 * difference_of_products(a,b,c,d)：补偿计算 a*b-c*d，减少两个大乘积相减的精度损失
 * deg_to_rad(deg)：角度转弧度
 * rad_to_deg(rad)：弧度转角度
 * normalize_angle(rad)：将角度归一化到 [0,2PI)
 * 
 * 【二：点与向量】
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
 * collinear(a,b,c)：按最长边对应的高是否 <= EPS 判断三点近似共线，与顶点顺序无关
 * project_length(a,b,p)：返回向量 ap 在 ab 方向上的有向投影长度
 * closest_pair_distance2(p)：返回点集 p 的最近点对距离平方，重复点返回 0，O(nlogn)
 * 少于两点时，closest_pair_distance2 的浮点版本返回 infinity，整数版本返回 -1
 * 
 * 【三：直线与线段】
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
 * polar_less(a,b)：按以原点为中心、正 x 轴为起点的逆时针极角比较
 * PolarCmp(o)：建立以 o 为中心的极角比较器
 * polar_sort(p,o)：将点集 p 按以 o 为中心的极角排序，同一射线按距离升序
 * 
 * 【四：圆形】
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
 * circle_overlap_area(a,b)：返回两圆公共部分的面积，支持相离、相切、包含、重合与零半径圆
 * minimum_enclosing_circle(p,seed)：返回点集 p 的最小覆盖圆，随机增量，期望 O(n)，最坏 O(n^3)
 * minimum_enclosing_circle 的 seed 可省略，默认用时钟种子打乱输入；空点集返回半径为 -1 的圆
 * 
 * 【五：三角形】
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
 * 【六：多边形】
 * convex_hull(p)：返回点集 p 的凸包，逆时针、不重复首尾点，边上共线点只保留两端
 * upper_hull(p)：返回点集的上凸包，从最左点到最右点
 * lower_hull(p)：返回点集的下凸包，从最左点到最右点
 * upper_right_hull(p)：返回点集的右上凸链，从最右的最高点到最右点
 * upper_left_hull(p)：返回点集的左上凸链，从最左点到最左的最高点
 * lower_right_hull(p)：返回点集的右下凸链，从最右的最低点到最右点
 * lower_left_hull(p)：返回点集的左下凸链，从最左点到最左的最低点
 * 上下/四方向凸链返回 vector；枚举链的边时不连接末点与首点。完整凸包则需要连接首尾。
 * Polygon(p)：由点集 p 建立凸多边形，内部自动取凸包并按逆时针保存
 * polygon_valid(poly)：判断 poly 是否为非退化凸多边形
 * polygon_perimeter(poly)：返回凸多边形周长
 * polygon_area_signed(poly)：返回凸多边形有向面积，Polygon 非退化时为正
 * polygon_area(poly)：返回凸多边形面积
 * polygon_centroid(poly,p)：求凸多边形的均匀面重心，成功返回 true，零面积返回 false
 * point_polygon_relation(p,poly)：p 在凸多边形外返回 -1，边界返回 0，内部返回 1，O(logn)
 * segment_in_polygon(s,poly)：判断闭线段 s 是否完全位于凸多边形内，含边界，O(logn)
 * segment_polygon_intersection(s,poly,p1,p2)：求线段与凸多边形的交集，返回 0 空集，1 单点 p1，2 线段 [p1,p2]
 * segment_polygon_intersect(s,poly)：判断线段与凸多边形是否有公共点
 * line_polygon_intersection(l,poly,p1,p2)：求直线与凸多边形的交集，返回 0 空集，1 单点 p1，2 线段 [p1,p2]
 * line_polygon_intersect(l,poly)：判断直线与凸多边形是否有公共点
 * convex_cut(poly,l)：返回凸多边形在有向直线 l 左闭半平面内的部分，l 必须非退化，O(n)
 * convex_polygon_intersection(a,b)：返回两个凸多边形的交集，支持空集、单点、线段退化，O(m(n+m))
 * convex_diameter2(poly)：返回凸多边形直径的平方，空集与单点返回 0，旋转卡壳 O(n)
 * convex_min_width(poly)：返回凸多边形的最小宽度，少于三个点返回 0，旋转卡壳 O(n)
 * polygon_perimeter(p)：参数为有序顶点 vector 时，返回一般简单多边形的周长，O(n)
 * polygon_area_signed(p)：参数为有序顶点 vector 时，返回一般简单多边形的有向面积，O(n)
 * polygon_area(p)：参数为有序顶点 vector 时，返回一般简单多边形的面积，O(n)
 * polygon_centroid(p,o)：参数为有序顶点 vector 时，求一般简单多边形的均匀面重心，零有向面积返回 false
 * point_simple_polygon_relation(x,p)：绕数法判断 x 与一般简单多边形的位置关系，外部 -1，边界 0，内部 1，O(n)
 * 一般多边形的 vector 按边界顺序保存顶点，支持凹多边形；Polygon 构造函数仍会自动取凸包
 * 
 * 【附：整点重载】
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
 * 整数版本的 closest_pair_distance2/convex_diameter2 返回 ll 精确距离平方
 * polygon_area2_signed(p)/polygon_area2(p)：参数为有序整数顶点 vector 时，返回一般多边形的两倍面积
 * polygon_boundary_lattice_points(poly)：返回整数凸多边形的边界格点数，空集 0，单点 1，线段 gcd+1
 * polygon_interior_lattice_points(poly)：使用 Pick 定理返回整数凸多边形内部格点数，退化时返回 0
 * convex_cut/convex_polygon_intersection 的整数重载先转为浮点，返回 Polygon，不提供精确拓扑保证
 * minimum_enclosing_circle 的整数点集重载返回 Circle；polygon_centroid 的整数重载返回 Point 重心
 * 
 * 【附：注意】
 * 整数层要求原始输入坐标和半径绝对值不超过 1e9
 * 在此前提下，单次整数点积、叉积、距离平方及圆关系中的整数中间结果均处于 ll 范围内
 * polygon_area2_signed/polygon_area2 还要求累计两倍面积处于 ll 范围内；模板始终不使用更宽整数类型
 * cmp_product 通过连分数比较乘积，线圆关系不会计算 cross^2 或 r^2*|v|^2
 * 不对用户自行进行的反复 IPoint 加减或任意整数数乘提供额外溢出保证
 * cmp_fraction/cmp_product、整数线圆与线段圆关系最坏 O(log V)；convex_hull/upper_hull/lower_hull 为 O(nlogn)
 * point_polygon_relation/segment_in_polygon 为 O(logn)，浮点线/线段-凸多边形求交 O(n)
 * 整数线/线段-凸多边形求交精确比较有理参数，最坏 O(nlogV)，额外空间 O(1)
 * 凸包构造需要 O(n) 额外空间；浮点线/线段-凸多边形求交只维护两端点，额外空间 O(1)
 * 最近点对使用 O(n) 额外空间；最小覆盖圆打乱输入需要 O(n) 空间；格点统计最坏 O(nlogV)
 * Point::operator== 按欧氏距离 <= EPS；排序比较器不使用 EPS，以满足严格弱序
 * 调用圆相关接口前应保证半径非负
 * 坐标量级极大或极小时需要按题目范围调整 EPS
 * 只做整数方向、共线和相交判断时，优先使用整数叉积模板
 * 浮点 orientation/on_line/on_segment 的 EPS 是距离误差，parallel/perpendicular 是归一化后的角度量误差
 * 浮点凸包的栈转向、凸多边形扇区二分采用不加 EPS 的叉积符号，避免误差沿拓扑操作累积
 * 浮点计算仍有舍入误差，不是精确几何内核；不要开启 -ffast-math（差积补偿依赖标准浮点语义）
 * Polygon/IPolygon 的 .p 必须保持逆时针严格凸包、不重复首尾、无共线中间点；自行修改后需重新取凸包
 * 两点退化多边形的周长为线段长度的两倍；退化直线按单点处理，有向距离约定为 0
 * common_tangents 保存切点而非直线，内/外切时两个切点可能重合，应使用圆心连线的垂线还原切线
 * 整数求交数量由精确判定决定，Point 坐标仍为近似值，极短交段可能舍入到同一个浮点点
 * 整数输入范围限制也适用于传入谓词的坐标；用户自行构造超范围的 IPoint/ILine 不在溢出保证内
 */
namespace Geometry { using ll = long long; using db = long double; const db EPS = 1e-12L; const db PI = acosl(-1.0L);
    int sgn(db x) { return (x > EPS) - (x < -EPS); }
    bool equal(db a, db b) { return sgn(a - b) == 0; }
    db sqr(db x) { return x * x; }
    db safe_sqrt(db x) { return sqrtl(max<db>(0, x)); }
    db difference_of_products(db a, db b, db c, db d) { db cd = c * d; return fmal(a, b, -cd) + fmal(-c, d, cd); }
    db deg_to_rad(db deg) { return deg * PI / 180; }
    db rad_to_deg(db rad) { return rad * 180 / PI; }
    db normalize_angle(db rad) { rad = fmodl(rad, 2 * PI); if (rad < 0) rad += 2 * PI; return rad; }
    struct Point { db x, y; Point() : x(0), y(0) {} Point(db _x, db _y) : x(_x), y(_y) {} Point operator+() const { return *this; } Point operator-() const { return Point(-x, -y); } Point operator+(const Point& o) const { return Point(x + o.x, y + o.y); } Point operator-(const Point& o) const { return Point(x - o.x, y - o.y); } Point operator*(db k) const { return Point(x * k, y * k); } Point operator/(db k) const { return Point(x / k, y / k); } Point& operator+=(const Point& o) { x += o.x, y += o.y; return *this; } Point& operator-=(const Point& o) { x -= o.x, y -= o.y; return *this; } Point& operator*=(db k) { x *= k, y *= k; return *this; } Point& operator/=(db k) { x /= k, y /= k; return *this; } bool operator==(const Point& o) const { db dx = x - o.x, dy = y - o.y; return dx * dx + dy * dy <= EPS * EPS; } bool operator!=(const Point& o) const { return !(*this == o); } bool operator<(const Point& o) const { return x != o.x ? x < o.x : y < o.y; } };
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
    bool zero_vector(const Point& a) { return norm2(a) <= EPS * EPS; }
    Point midpoint(const Point& a, const Point& b) { return (a + b) / 2; }
    Point unit(const Point& a) { db len = norm(a); return sgn(len) == 0 ? Point() : a / len; }
    Point rotate_90(const Point& a) { return Point(-a.y, a.x); }
    Point rotate(const Point& a, db rad) { db c = cosl(rad), s = sinl(rad); return Point(a.x * c - a.y * s, a.x * s + a.y * c); }
    db polar_angle(const Point& a) { return atan2l(a.y, a.x); }
    db vector_angle(const Point& a, const Point& b) { return zero_vector(a) || zero_vector(b) ? 0 : atan2l(fabsl(cross(a, b)), dot(a, b)); }
    db oriented_angle(const Point& a, const Point& b) { return zero_vector(a) || zero_vector(b) ? 0 : atan2l(cross(a, b), dot(a, b)); }
    db ccw_angle(const Point& a, const Point& b) { return normalize_angle(oriented_angle(a, b)); }
    bool parallel(const Point& a, const Point& b) { return sgn(cross(unit(a), unit(b))) == 0; }
    bool perpendicular(const Point& a, const Point& b) { return sgn(dot(unit(a), unit(b))) == 0; }
    int orientation(const Point& a, const Point& b, const Point& c) { Point v = b - a; return zero_vector(v) ? 0 : sgn(cross(v, c - a) / norm(v)); }
    bool collinear(const Point& a, const Point& b, const Point& c) { db base = max({dis(a, b), dis(a, c), dis(b, c)}); return base <= EPS || fabsl(cross(a, b, c)) <= EPS * base; }
    db project_length(const Point& a, const Point& b, const Point& p) { Point v = b - a; db len = norm(v); return sgn(len) == 0 ? 0 : dot(p - a, v) / len; }
    struct Line { Point p, v; Line() {} Line(const Point& a, const Point& b) : p(a), v(b - a) {} static Line from_point_vector(const Point& p, const Point& v) { Line l; l.p = p, l.v = v; return l; } Point at(db t) const { return p + v * t; } };
    struct Segment { Point a, b; Segment() {} Segment(const Point& _a, const Point& _b) : a(_a), b(_b) {} };
    bool line_valid(const Line& l) { return !zero_vector(l.v); }
    bool segment_valid(const Segment& s) { return s.a != s.b; }
    bool line_from_abc(db a, db b, db c, Line& l) { db scale = max(fabsl(a), fabsl(b)); if (scale == 0) return false; Point p = fabsl(a) > fabsl(b) ? Point(-c / a, 0) : Point(0, -c / b); l = Line::from_point_vector(p, unit(Point(-b / scale, a / scale))); return true; }
    Point line_normal(const Line& l) { return unit(rotate_90(l.v)); }
    Line parallel_line(const Line& l, const Point& p) { return Line::from_point_vector(p, l.v); }
    Line perpendicular_line(const Line& l, const Point& p) { return Line::from_point_vector(p, rotate_90(l.v)); }
    Line perpendicular_bisector(const Point& a, const Point& b) { return Line::from_point_vector(midpoint(a, b), rotate_90(b - a)); }
    bool on_line(const Point& p, const Line& l) { return line_valid(l) ? sgn(cross(l.v, p - l.p) / norm(l.v)) == 0 : p == l.p; }
    bool on_segment(const Point& p, const Segment& s) { if (!segment_valid(s)) return p == s.a; return orientation(s.a, s.b, p) == 0 && p.x >= min(s.a.x, s.b.x) - EPS && p.x <= max(s.a.x, s.b.x) + EPS && p.y >= min(s.a.y, s.b.y) - EPS && p.y <= max(s.a.y, s.b.y) + EPS; }
    bool on_ray(const Point& p, const Point& o, const Point& v) { if (zero_vector(v)) return p == o; db len = norm(v); return sgn(cross(v, p - o) / len) == 0 && sgn(dot(v, p - o) / len) >= 0; }
    int side_of_line(const Point& p, const Line& l) { return line_valid(l) ? sgn(cross(l.v, p - l.p) / norm(l.v)) : 0; }
    db distance_point_line_signed(const Point& p, const Line& l) { return line_valid(l) ? cross(l.v, p - l.p) / norm(l.v) : 0; }
    db distance_point_line(const Point& p, const Line& l) { return line_valid(l) ? fabsl(distance_point_line_signed(p, l)) : dis(p, l.p); }
    Point project_point_line(const Point& p, const Line& l) { return line_valid(l) ? l.p + l.v * (dot(p - l.p, l.v) / norm2(l.v)) : l.p; }
    Point reflect_point_line(const Point& p, const Line& l) { return project_point_line(p, l) * 2 - p; }
    Point closest_point_segment(const Point& p, const Segment& s) { Point v = s.b - s.a; if (zero_vector(v)) return s.a; db t = dot(p - s.a, v) / norm2(v); t = max<db>(0, min<db>(1, t)); return s.a + v * t; }
    db distance_point_segment(const Point& p, const Segment& s) { return dis(p, closest_point_segment(p, s)); }
    bool line_parallel(const Line& a, const Line& b) { return parallel(a.v, b.v); }
    bool line_coincident(const Line& a, const Line& b) { return line_valid(a) && line_valid(b) && line_parallel(a, b) && on_line(b.p, a); }
    int line_intersection(const Line& a, const Line& b, Point& p) { bool va = line_valid(a), vb = line_valid(b); if (!va && !vb) { if (a.p != b.p) return 0; p = a.p; return 1; } if (!va) { if (!on_line(a.p, b)) return 0; p = a.p; return 1; } if (!vb) { if (!on_line(b.p, a)) return 0; p = b.p; return 1; } Point u = unit(a.v), v = unit(b.v); db d = cross(u, v); if (sgn(d) == 0) return on_line(b.p, a) ? 2 : 0; p = a.p + u * (cross(b.p - a.p, v) / d); return 1; }
    bool segment_strict_intersect(const Segment& a, const Segment& b) { int c1 = orientation(a.a, a.b, b.a), c2 = orientation(a.a, a.b, b.b); int c3 = orientation(b.a, b.b, a.a), c4 = orientation(b.a, b.b, a.b); return c1 * c2 < 0 && c3 * c4 < 0; }
    int segment_intersection(const Segment& a, const Segment& b, Point& p1, Point& p2) { Point q[4]; int cnt = 0; auto add = [&](const Point& p) -> void { for (int i = 0; i < cnt; ++i) { if (q[i] == p) return; } q[cnt++] = p; }; if (on_segment(a.a, b)) add(a.a); if (on_segment(a.b, b)) add(a.b); if (on_segment(b.a, a)) add(b.a); if (on_segment(b.b, a)) add(b.b); if (cnt) { p1 = p2 = q[0]; for (int i = 1; i < cnt; ++i) p1 = min(p1, q[i]), p2 = max(p2, q[i]); return p1 == p2 ? 1 : 2; } if (!segment_strict_intersect(a, b)) return 0; Point u = a.b - a.a, v = b.b - b.a; p1 = a.a + u * (cross(b.a - a.a, v) / cross(u, v)); p2 = p1; return 1; }
    bool segment_intersect(const Segment& a, const Segment& b) { return segment_strict_intersect(a, b) || on_segment(a.a, b) || on_segment(a.b, b) || on_segment(b.a, a) || on_segment(b.b, a); }
    int line_segment_intersection(const Line& l, const Segment& s, Point& p1, Point& p2) { if (!line_valid(l)) { if (!on_segment(l.p, s)) return 0; p1 = p2 = l.p; return 1; } db len = norm(l.v); db x = cross(l.v, s.a - l.p) / len, y = cross(l.v, s.b - l.p) / len; int a = sgn(x), b = sgn(y); if (a == 0 && b == 0) { p1 = s.a, p2 = s.b; return p1 == p2 ? 1 : 2; } if (a != 0 && b != 0 && a == b) return 0; if (a == 0) p1 = s.a; else if (b == 0) p1 = s.b; else p1 = s.a + (s.b - s.a) * (x / (x - y)); p2 = p1; return 1; }
    db distance_line_line(const Line& a, const Line& b) { if (!line_valid(a) && !line_valid(b)) return dis(a.p, b.p); if (!line_valid(a)) return distance_point_line(a.p, b); if (!line_valid(b)) return distance_point_line(b.p, a); Point p; return line_intersection(a, b, p) != 0 ? 0 : distance_point_line(b.p, a); }
    db distance_line_segment(const Line& l, const Segment& s) { if (!line_valid(l)) return distance_point_segment(l.p, s); Point p1, p2; return line_segment_intersection(l, s, p1, p2) != 0 ? 0 : min(distance_point_line(s.a, l), distance_point_line(s.b, l)); }
    db distance_segment_segment(const Segment& a, const Segment& b) { if (segment_intersect(a, b)) return 0; db res = min(distance_point_segment(a.a, b), distance_point_segment(a.b, b)); return min(res, min(distance_point_segment(b.a, a), distance_point_segment(b.b, a))); }
    db line_angle(const Line& a, const Line& b) { db res = vector_angle(a.v, b.v); return min(res, PI - res); }
    db line_oriented_angle(const Line& a, const Line& b) { return oriented_angle(a.v, b.v); }
    int polar_half(const Point& p) { if (p.y > 0) return 0; if (p.y == 0 && p.x >= 0) return 0; return 1; }
    bool polar_less(const Point& a, const Point& b) { int ha = polar_half(a), hb = polar_half(b); if (ha != hb) return ha < hb; db c = cross(a, b); return c != 0 ? c > 0 : norm2(a) < norm2(b); }
    struct PolarCmp { Point o; PolarCmp(const Point& _o = Point()) : o(_o) {} bool operator()(const Point& a, const Point& b) const { return polar_less(a - o, b - o); } };
    void polar_sort(vector<Point>& p, const Point& o = Point()) { sort(p.begin(), p.end(), PolarCmp(o)); }
    struct Circle { Point o; db r; Circle() : r(0) {} Circle(const Point& _o, db _r) : o(_o), r(_r) {} static Circle from_diameter(const Point& a, const Point& b) { return Circle(midpoint(a, b), dis(a, b) / 2); } };
    bool circle_valid(const Circle& c) { return c.r >= 0; }
    db circle_area(const Circle& c) { return PI * c.r * c.r; }
    db circle_circumference(const Circle& c) { return 2 * PI * c.r; }
    db arc_length(const Circle& c, db rad) { return c.r * rad; }
    db sector_area(const Circle& c, db rad) { return c.r * c.r * rad / 2; }
    db chord_length(const Circle& c, db rad) { return 2 * c.r * sinl(rad / 2); }
    int point_circle_relation(const Point& p, const Circle& c) { return sgn(dis(p, c.o) - c.r); }
    int line_circle_relation(const Line& l, const Circle& c) { if (!line_valid(l)) return point_circle_relation(l.p, c) == 0; int t = sgn(distance_point_line(c.o, l) - c.r); return t > 0 ? 0 : t == 0 ? 1 : 2; }
    int circle_circle_relation(const Circle& a, const Circle& b) { db d = dis(a.o, b.o); if (a.o == b.o && equal(a.r, b.r)) return 5; if (sgn(d - a.r - b.r) > 0) return 0; if (sgn(d - a.r - b.r) == 0) return 1; if (sgn(d - fabsl(a.r - b.r)) > 0) return 2; if (sgn(d - fabsl(a.r - b.r)) == 0) return 3; return 4; }
    int circle_centers(const Point& a, const Point& b, db r, Point& o1, Point& o2) { if (r < 0) return 0; if (a == b) { if (r != 0) return -1; o1 = o2 = a; return 1; } db d = dis(a, b); int rel = sgn(d - 2 * r); if (rel > 0) return 0; Point m = midpoint(a, b); if (rel == 0) { o1 = o2 = m; return 1; } db h = safe_sqrt((r - d / 2) * (r + d / 2)); Point v = unit(rotate_90(b - a)) * h; o1 = m - v, o2 = m + v; return 2; }
    int line_circle_intersection(const Line& l, const Circle& c, Point& p1, Point& p2) { if (!line_valid(l)) { if (point_circle_relation(l.p, c) != 0) return 0; p1 = p2 = l.p; return 1; } int cnt = line_circle_relation(l, c); if (cnt == 0) return 0; Point h = project_point_line(c.o, l); if (cnt == 1) { p1 = p2 = h; return 1; } db distance = distance_point_line(c.o, l); db h2 = (c.r - distance) * (c.r + distance); Point d = unit(l.v) * safe_sqrt(h2); p1 = h - d, p2 = h + d; return 2; }
    int segment_circle_intersection(const Segment& s, const Circle& c, Point& p1, Point& p2) { if (!segment_valid(s)) { if (point_circle_relation(s.a, c) != 0) return 0; p1 = p2 = s.a; return 1; } int cnt = line_circle_relation(Line(s.a, s.b), c); if (cnt == 0) return 0; Point v = s.b - s.a, w = s.a - c.o; db a = norm2(v), b = dot(v, w), constant = norm2(w) - c.r * c.r; db t1 = -b / a, t2 = t1; if (cnt == 2) { db delta = safe_sqrt(difference_of_products(b, b, a, constant)); db q = -b - copysignl(delta, b); if (q != 0) t1 = q / a, t2 = constant / q; if (t1 > t2) swap(t1, t2); }   if (point_circle_relation(s.a, c) == 0) { if (fabsl(t1) <= fabsl(t2)) t1 = 0; else t2 = 0; } if (point_circle_relation(s.b, c) == 0) { if (fabsl(t1 - 1) <= fabsl(t2 - 1)) t1 = 1; else t2 = 1; } if (t1 > t2) swap(t1, t2); db tolerance = EPS / norm(v); int res = 0; auto at = [&](db t) { return s.a + v * max<db>(0, min<db>(1, t)); }; if (t1 >= -tolerance && t1 <= 1 + tolerance) p1 = at(t1), ++res; if (cnt == 2 && t2 >= -tolerance && t2 <= 1 + tolerance && (res == 0 || at(t2) != p1)) { if (res == 0) p1 = at(t2); else p2 = at(t2); ++res; } if (res == 1) p2 = p1; return res; }
    int circle_circle_intersection(const Circle& a, const Circle& b, Point& p1, Point& p2) { int rel = circle_circle_relation(a, b); if (rel == 5) { if (a.r != 0) return -1; p1 = p2 = a.o; return 1; } if (rel == 0 || rel == 4) return 0; db d2 = dis2(a.o, b.o); db d = safe_sqrt(d2); if (d == 0) return 0; db x = (d2 + (a.r - b.r) * (a.r + b.r)) / (2 * d); db sum = a.r + b.r, dif = fabsl(a.r - b.r); db h2 = ((sum - d) * (sum + d)) * ((d - dif) * (d + dif)) / (4 * d2); Point u = (b.o - a.o) / d; Point h = a.o + u * x; if (rel == 1 || rel == 3) { p1 = p2 = h; return 1; } Point v = rotate_90(u) * safe_sqrt(h2); p1 = h - v, p2 = h + v; return 2; }
    bool radical_axis(const Circle& a, const Circle& b, Line& l) { Point v = b.o - a.o; if (zero_vector(v)) return false; Point p = a.o + v * ((norm2(v) + (a.r - b.r) * (a.r + b.r)) / (2 * norm2(v))); l = Line::from_point_vector(p, rotate_90(v)); return true; }
    bool circle_from_three_points(const Point& a, const Point& b, const Point& c, Circle& res) { Point u = b - a, v = c - a; db cr = cross(u, v); if (collinear(a, b, c)) return false; Point delta(difference_of_products(norm2(u), v.y, norm2(v), u.y) / (2 * cr), difference_of_products(norm2(v), u.x, norm2(u), v.x) / (2 * cr)); res = Circle(a + delta, norm(delta)); return true; }
    int tangent_points(const Point& p, const Circle& c, Point& p1, Point& p2) { Point v = p - c.o; db d2 = norm2(v), r2 = c.r * c.r; int t = point_circle_relation(p, c); if (t < 0) return 0; if (c.r == 0) { p1 = p2 = c.o; return p == c.o ? -1 : 1; } if (t == 0) { p1 = p2 = p; return 1; } Point h = c.o + v * (r2 / d2); db len = norm(v); Point d = rotate_90(v) * (c.r * safe_sqrt((len - c.r) * (len + c.r)) / d2); p1 = h - d, p2 = h + d; return 2; }
    int common_tangents(const Circle& a, const Circle& b, vector<pair<Point, Point>>& res) { res.clear(); if (a.r == 0 || b.r == 0) { if (a.r == 0 && b.r == 0) { if (a.o == b.o) return -1; res.emplace_back(a.o, b.o); return 1; } Point p1, p2; int cnt = a.r == 0 ? tangent_points(a.o, b, p1, p2) : tangent_points(b.o, a, p1, p2); if (cnt >= 1) res.push_back(a.r == 0 ? make_pair(a.o, p1) : make_pair(p1, b.o)); if (cnt == 2) res.push_back(a.r == 0 ? make_pair(a.o, p2) : make_pair(p2, b.o)); return cnt; } Point d = b.o - a.o; db d2 = norm2(d); if (a.o == b.o && equal(a.r, b.r)) return -1; if (a.o == b.o) return 0; int cnt = 0; db len = norm(d); for (int type : {-1, 1}) { db r = a.r - type * b.r; int rel = sgn(len - fabsl(r)); if (rel < 0) continue; db h = rel == 0 ? 0 : safe_sqrt((len - fabsl(r)) * (len + fabsl(r))); for (int side : {-1, 1}) { if (rel == 0 && side == 1) continue; Point v = (d * r + rotate_90(d) * (h * side)) / d2; res.emplace_back(a.o + v * a.r, b.o + v * (type * b.r)); ++cnt; } } return cnt; }
    struct Triangle { Point a, b, c; Triangle() {} Triangle(const Point& _a, const Point& _b, const Point& _c) : a(_a), b(_b), c(_c) {} };
    db triangle_area_signed(const Point& a, const Point& b, const Point& c) { return cross(a, b, c) / 2; }
    db triangle_area(const Point& a, const Point& b, const Point& c) { return fabsl(triangle_area_signed(a, b, c)); }
    db triangle_area(db a, db b, db c) { if (a < 0 || b < 0 || c < 0) return 0; if (a < b) swap(a, b); if (a < c) swap(a, c); if (b < c) swap(b, c); if (c < a - b) return 0; return safe_sqrt((a + (b + c)) * (c - (a - b)) * (c + (a - b)) * (a + (b - c))) / 4; }
    db triangle_area_signed(const Triangle& t) { return cross(t.a, t.b, t.c) / 2; }
    db triangle_area(const Triangle& t) { return fabsl(triangle_area_signed(t)); }
    db triangle_perimeter(const Triangle& t) { return dis(t.a, t.b) + dis(t.b, t.c) + dis(t.c, t.a); }
    bool triangle_valid(const Triangle& t) { return !collinear(t.a, t.b, t.c); }
    db triangle_angle(const Point& a, const Point& b, const Point& c) { return vector_angle(a - b, c - b); }
    Point triangle_centroid(const Triangle& t) { return (t.a + t.b + t.c) / 3; }
    bool triangle_circumcircle(const Triangle& t, Circle& c) { return circle_from_three_points(t.a, t.b, t.c, c); }
    bool triangle_circumcenter(const Triangle& t, Point& p) { Circle c; if (!triangle_circumcircle(t, c)) return false; p = c.o; return true; }
    db triangle_circumradius(const Triangle& t) { return !triangle_valid(t) ? 0 : dis(t.a, t.b) * dis(t.b, t.c) * dis(t.c, t.a) / (4 * triangle_area(t)); }
    bool triangle_incircle(const Triangle& t, Circle& c) { db x = dis(t.b, t.c), y = dis(t.c, t.a), z = dis(t.a, t.b); db sum = x + y + z; if (sgn(sum) == 0 || !triangle_valid(t)) return false; Point o = (t.a * x + t.b * y + t.c * z) / sum; c = Circle(o, 2 * triangle_area(t) / sum); return true; }
    bool triangle_incenter(const Triangle& t, Point& p) { Circle c; if (!triangle_incircle(t, c)) return false; p = c.o; return true; }
    db triangle_inradius(const Triangle& t) { db perimeter = triangle_perimeter(t); return sgn(perimeter) == 0 ? 0 : 2 * triangle_area(t) / perimeter; }
    bool triangle_orthocenter(const Triangle& t, Point& p) { Point o; if (!triangle_circumcenter(t, o)) return false; p = t.a + t.b + t.c - o * 2; return true; }
    Line triangle_angle_bisector(const Point& a, const Point& b, const Point& c) { Point v = unit(a - b) + unit(c - b); if (zero_vector(v)) v = rotate_90(a - b); return Line::from_point_vector(b, v); }
    int point_triangle_relation(const Point& p, const Triangle& t) { if (!triangle_valid(t)) return on_segment(p, Segment(t.a, t.b)) || on_segment(p, Segment(t.b, t.c)) || on_segment(p, Segment(t.c, t.a)) ? 0 : -1; int a = orientation(t.a, t.b, p), b = orientation(t.b, t.c, p), c = orientation(t.c, t.a, p); if (a == 0 && on_segment(p, Segment(t.a, t.b))) return 0; if (b == 0 && on_segment(p, Segment(t.b, t.c))) return 0; if (c == 0 && on_segment(p, Segment(t.c, t.a))) return 0; bool pos = a > 0 || b > 0 || c > 0; bool neg = a < 0 || b < 0 || c < 0; return pos && neg ? -1 : 1; }
    vector<Point> convex_hull(vector<Point> p) { sort(p.begin(), p.end()); vector<Point> q; for (auto& x : p) if (q.empty() || x != q.back()) q.push_back(x); if (q.size() <= 1) return q; vector<Point> h(q.size() * 2); int top = 0; for (auto& x : q) { while (top >= 2 && cross(h[top - 2], h[top - 1], x) <= 0) --top; h[top++] = x; } int t = top + 1, n = q.size(); for (int i = n - 2; i >= 0; --i) { while (top >= t && cross(h[top - 2], h[top - 1], q[i]) <= 0) --top; h[top++] = q[i]; } h.resize(top - 1); return h; }
    vector<Point> upper_hull(vector<Point> p) { sort(p.begin(), p.end(), [](const Point& a, const Point& b) { return a.x != b.x ? a.x < b.x : a.y > b.y; }); vector<Point> q, h; for (auto& x : p) if (q.empty() || x.x != q.back().x) q.push_back(x); for (auto& x : q) { while (h.size() >= 2 && cross(h[h.size() - 2], h.back(), x) >= 0) h.pop_back(); h.push_back(x); } return h; }
    vector<Point> lower_hull(vector<Point> p) { sort(p.begin(), p.end()); vector<Point> q, h; for (auto& x : p) if (q.empty() || x.x != q.back().x) q.push_back(x); for (auto& x : q) { while (h.size() >= 2 && cross(h[h.size() - 2], h.back(), x) <= 0) h.pop_back(); h.push_back(x); } return h; }
    struct Polygon { vector<Point> p; Polygon() {} Polygon(const vector<Point>& _p) : p(convex_hull(_p)) {} int size() const { return p.size(); } bool empty() const { return p.empty(); } const Point& operator[](int i) const { return p[i]; } Point& operator[](int i) { return p[i]; } };
    bool polygon_valid(const Polygon& poly) { return poly.size() >= 3; }
    db polygon_perimeter(const Polygon& poly) { int n = poly.size(); if (n <= 1) return 0; db res = 0; for (int i = 0; i < n; ++i) res += dis(poly[i], poly[(i + 1) % n]); return res; }
    db polygon_area_signed(const Polygon& poly) { int n = poly.size(); if (n < 3) return 0; db res = 0; for (int i = 1; i + 1 < n; ++i) res += cross(poly[0], poly[i], poly[i + 1]); return res / 2; }
    db polygon_area(const Polygon& poly) { return fabsl(polygon_area_signed(poly)); }
    int point_polygon_relation(const Point& x, const Polygon& poly) { int n = poly.size(); if (n == 0) return -1; if (n == 1) return x == poly[0] ? 0 : -1; if (n == 2) return on_segment(x, Segment(poly[0], poly[1])) ? 0 : -1; if (on_segment(x, Segment(poly[0], poly[1])) || on_segment(x, Segment(poly[0], poly[n - 1]))) return 0; db a = cross(poly[0], poly[1], x), b = cross(poly[0], poly[n - 1], x); if (a <= 0 || b >= 0) return -1; int l = 1, r = n - 1; while (r - l > 1) { int mid = (l + r) >> 1; if (cross(poly[0], poly[mid], x) >= 0) l = mid; else r = mid; } int c = orientation(poly[l], poly[r], x); if (c < 0) return -1; return c == 0 ? 0 : 1; }
    bool segment_in_polygon(const Segment& s, const Polygon& poly) { return point_polygon_relation(s.a, poly) >= 0 && point_polygon_relation(s.b, poly) >= 0; }
    namespace detail { struct PointExtrema { Point origin, direction, first, last; db low = 0, high = 0; bool seen = false; PointExtrema(const Point& p, const Point& v) : origin(p), direction(v) {} void add(const Point& p) { db key = dot(p - origin, direction); if (!seen || key < low) low = key, first = p; if (!seen || key > high) high = key, last = p; seen = true; } int result(Point& p1, Point& p2) const { if (!seen) return 0; p1 = first, p2 = last; return p1 == p2 ? 1 : 2; } }; }
    int segment_polygon_intersection(const Segment& s, const Polygon& poly, Point& p1, Point& p2) { if (!segment_valid(s)) { if (point_polygon_relation(s.a, poly) < 0) return 0; p1 = p2 = s.a; return 1; } detail::PointExtrema result(s.a, s.b - s.a); if (point_polygon_relation(s.a, poly) >= 0) result.add(s.a); if (point_polygon_relation(s.b, poly) >= 0) result.add(s.b); for (int i = 0, n = poly.size(); i < n; ++i) { Point x, y; int cnt = segment_intersection(s, Segment(poly[i], poly[(i + 1) % n]), x, y); if (cnt >= 1) result.add(x); if (cnt == 2) result.add(y); } return result.result(p1, p2); }
    bool segment_polygon_intersect(const Segment& s, const Polygon& poly) { if (point_polygon_relation(s.a, poly) >= 0 || point_polygon_relation(s.b, poly) >= 0) return true; for (int i = 0, n = poly.size(); i < n; ++i) if (segment_intersect(s, Segment(poly[i], poly[(i + 1) % n]))) return true; return false; }
    int cmp_fraction(ll a, ll b, ll c, ll d) { int rev = 1; while (true) { ll x = a / b, y = c / d; if (x != y) return rev * (x < y ? -1 : 1); a %= b, c %= d; if (a == 0 || c == 0) return rev * ((a > 0) - (c > 0)); swap(a, b), swap(c, d); rev = -rev; } }
    int cmp_product(ll a, ll b, ll c, ll d) { bool x = a == 0 || b == 0, y = c == 0 || d == 0; if (x || y) return (!x) - (!y); return cmp_fraction(a, c, d, b); }
    struct IPoint { ll x, y; IPoint() : x(0), y(0) {} IPoint(ll _x, ll _y) : x(_x), y(_y) {} IPoint operator+() const { return *this; } IPoint operator-() const { return IPoint(-x, -y); } IPoint operator+(const IPoint& o) const { return IPoint(x + o.x, y + o.y); } IPoint operator-(const IPoint& o) const { return IPoint(x - o.x, y - o.y); } IPoint& operator+=(const IPoint& o) { x += o.x, y += o.y; return *this; } IPoint& operator-=(const IPoint& o) { x -= o.x, y -= o.y; return *this; } bool operator==(const IPoint& o) const { return x == o.x && y == o.y; } bool operator!=(const IPoint& o) const { return !(*this == o); } bool operator<(const IPoint& o) const { return x != o.x ? x < o.x : y < o.y; } };
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
    struct ILine { IPoint p, v; ILine() {} ILine(const IPoint& a, const IPoint& b) : p(a), v(b - a) {} static ILine from_point_vector(const IPoint& p, const IPoint& v) { ILine l; l.p = p, l.v = v; return l; } };
    struct ISegment { IPoint a, b; ISegment() {} ISegment(const IPoint& _a, const IPoint& _b) : a(_a), b(_b) {} };
    Line to_line(const ILine& l) { return Line::from_point_vector(to_point(l.p), to_point(l.v)); }
    Segment to_segment(const ISegment& s) { return Segment(to_point(s.a), to_point(s.b)); }
    bool line_valid(const ILine& l) { return !zero_vector(l.v); }
    bool segment_valid(const ISegment& s) { return s.a != s.b; }
    template<class A, class B, class C, enable_if_t<is_integral_v<A> && is_integral_v<B> && is_integral_v<C>, int> = 0>
    bool line_from_abc(A a, B b, C c, Line& l) { return line_from_abc((db)a, (db)b, (db)c, l); }
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
    int line_intersection(const ILine& a, const ILine& b, Point& p) { bool va = line_valid(a), vb = line_valid(b); if (!va && !vb) { if (a.p != b.p) return 0; p = to_point(a.p); return 1; } if (!va) { if (!on_line(a.p, b)) return 0; p = to_point(a.p); return 1; } if (!vb) { if (!on_line(b.p, a)) return 0; p = to_point(b.p); return 1; } ll d = cross(a.v, b.v); if (d == 0) return cross(a.v, b.p - a.p) == 0 ? 2 : 0; ll ca = cross(a.p, a.v), cb = cross(b.p, b.v); p = Point(difference_of_products(cb, a.v.x, ca, b.v.x) / d, difference_of_products(cb, a.v.y, ca, b.v.y) / d); return 1; }
    bool segment_strict_intersect(const ISegment& a, const ISegment& b) { int c1 = orientation(a.a, a.b, b.a), c2 = orientation(a.a, a.b, b.b); int c3 = orientation(b.a, b.b, a.a), c4 = orientation(b.a, b.b, a.b); return c1 * c2 < 0 && c3 * c4 < 0; }
    bool segment_intersect(const ISegment& a, const ISegment& b) { if (segment_strict_intersect(a, b)) return true; return on_segment(a.a, b) || on_segment(a.b, b) || on_segment(b.a, a) || on_segment(b.b, a); }
    int segment_intersection(const ISegment& a, const ISegment& b, Point& p1, Point& p2) { IPoint q[4]; int cnt = 0; auto add = [&](const IPoint& p) -> void { for (int i = 0; i < cnt; ++i) if (q[i] == p) return; q[cnt++] = p; }; if (on_segment(a.a, b)) add(a.a); if (on_segment(a.b, b)) add(a.b); if (on_segment(b.a, a)) add(b.a); if (on_segment(b.b, a)) add(b.b); if (cnt) { IPoint x = q[0], y = q[0]; for (int i = 1; i < cnt; ++i) x = min(x, q[i]), y = max(y, q[i]); p1 = to_point(x), p2 = to_point(y); return x == y ? 1 : 2; } if (!segment_strict_intersect(a, b)) return 0; line_intersection(ILine(a.a, a.b), ILine(b.a, b.b), p1); p2 = p1; return 1; }
    int line_segment_intersection(const ILine& l, const ISegment& s, Point& p1, Point& p2) { if (!line_valid(l)) { if (!on_segment(l.p, s)) return 0; p1 = p2 = to_point(l.p); return 1; } ll x = cross(l.v, s.a - l.p), y = cross(l.v, s.b - l.p); if (x == 0 && y == 0) { p1 = to_point(s.a), p2 = to_point(s.b); return s.a == s.b ? 1 : 2; } if ((x > 0 && y > 0) || (x < 0 && y < 0)) return 0; if (x == 0) p1 = to_point(s.a); else if (y == 0) p1 = to_point(s.b); else line_intersection(l, ILine(s.a, s.b), p1); p2 = p1; return 1; }
    db distance_line_line(const ILine& a, const ILine& b) { if (!line_valid(a) && !line_valid(b)) return dis(a.p, b.p); if (!line_valid(a)) return distance_point_line(a.p, b); if (!line_valid(b)) return distance_point_line(b.p, a); Point p; return line_intersection(a, b, p) != 0 ? 0 : distance_point_line(b.p, a); }
    db distance_line_segment(const ILine& l, const ISegment& s) { if (!line_valid(l)) return distance_point_segment(l.p, s); Point p1, p2; return line_segment_intersection(l, s, p1, p2) != 0 ? 0 : min(distance_point_line(s.a, l), distance_point_line(s.b, l)); }
    db distance_segment_segment(const ISegment& a, const ISegment& b) { if (segment_intersect(a, b)) return 0; db res = min(distance_point_segment(a.a, b), distance_point_segment(a.b, b)); return min(res, min(distance_point_segment(b.a, a), distance_point_segment(b.b, a))); }
    db line_angle(const ILine& a, const ILine& b) { db res = vector_angle(a.v, b.v); return min(res, PI - res); }
    db line_oriented_angle(const ILine& a, const ILine& b) { return oriented_angle(a.v, b.v); }
    int polar_half(const IPoint& p) { if (p.y != 0) return p.y < 0; return p.x < 0; }
    bool polar_less(const IPoint& a, const IPoint& b) { int ha = polar_half(a), hb = polar_half(b); if (ha != hb) return ha < hb; ll c = cross(a, b); return c != 0 ? c > 0 : norm2(a) < norm2(b); }
    struct IPolarCmp { IPoint o; IPolarCmp(const IPoint& _o = IPoint()) : o(_o) {} bool operator()(const IPoint& a, const IPoint& b) const { return polar_less(a - o, b - o); } };
    void polar_sort(vector<IPoint>& p, const IPoint& o = IPoint()) { sort(p.begin(), p.end(), IPolarCmp(o)); }
    struct ICircle { IPoint o; ll r; ICircle() : r(0) {} ICircle(const IPoint& _o, ll _r) : o(_o), r(_r) {} };
    Circle to_circle(const ICircle& c) { return Circle(to_point(c.o), c.r); }
    bool circle_valid(const ICircle& c) { return c.r >= 0; }
    db circle_area(const ICircle& c) { return PI * (db)c.r * c.r; }
    db circle_circumference(const ICircle& c) { return 2 * PI * c.r; }
    db arc_length(const ICircle& c, db rad) { return c.r * rad; }
    db sector_area(const ICircle& c, db rad) { return (db)c.r * c.r * rad / 2; }
    db chord_length(const ICircle& c, db rad) { return 2 * c.r * sinl(rad / 2); }
    int point_circle_relation(const IPoint& p, const ICircle& c) { ll v = dis2(p, c.o) - c.r * c.r; return (v > 0) - (v < 0); }
    int line_circle_relation(const ILine& l, const ICircle& c) { if (!line_valid(l)) return point_circle_relation(l.p, c) == 0; ll x = llabs(cross(l.v, c.o - l.p)), a = norm2(l.v), r2 = c.r * c.r; int t = cmp_product(x, x, r2, a); return t > 0 ? 0 : t == 0 ? 1 : 2; }
    int circle_circle_relation(const ICircle& a, const ICircle& b) { ll d2 = dis2(a.o, b.o); if (d2 == 0 && a.r == b.r) return 5; ll sum = a.r + b.r, dif = llabs(a.r - b.r); if (d2 > sum * sum) return 0; if (d2 == sum * sum) return 1; if (d2 > dif * dif) return 2; if (d2 == dif * dif) return 3; return 4; }
    int circle_centers(const IPoint& a, const IPoint& b, ll r, Point& o1, Point& o2) { if (r < 0) return 0; if (a == b) { if (r != 0) return -1; o1 = o2 = to_point(a); return 1; } ll d2 = dis2(a, b), rr = 4 * r * r; if (d2 > rr) return 0; Point m = midpoint(a, b); db h = safe_sqrt((db)r * r - (db)d2 / 4); Point v = unit(rotate_90(b - a)) * h; o1 = m - v, o2 = m + v; return d2 == rr ? 1 : 2; }
    int line_circle_intersection(const ILine& l, const ICircle& c, Point& p1, Point& p2) { if (!line_valid(l)) { if (point_circle_relation(l.p, c) != 0) return 0; p1 = p2 = to_point(l.p); return 1; } int cnt = line_circle_relation(l, c); if (cnt == 0) return 0; IPoint u = c.o - l.p; ll a = norm2(l.v), b = dot(u, l.v), x = cross(l.v, u); Point h = to_point(l.p) + to_point(l.v) * ((db)b / a); if (cnt == 1) { p1 = p2 = h; return 1; } db h2 = difference_of_products(c.r * c.r, a, x, x) / a; Point d = unit(l.v) * safe_sqrt(h2); p1 = h - d, p2 = h + d; return 2; }
    int segment_circle_relation(const ISegment& s, const ICircle& c) { IPoint v = s.b - s.a; ll a = norm2(v); if (a == 0) return point_circle_relation(s.a, c) == 0; ll r2 = c.r * c.r, x = dis2(s.a, c.o) - r2, y = dis2(s.b, c.o) - r2; int sx = (x > 0) - (x < 0), sy = (y > 0) - (y < 0); if (sx == 0 && sy == 0) return 2; ll d = dot(s.a - c.o, v); if (sx == 0) { if (d >= 0) return 1; return -d <= a / 2 ? 2 : 1; } if (sy == 0) return x > 0 && x < a ? 2 : 1; if (sx != sy) return 1; if (sx < 0 || d >= 0 || -d >= a) return 0; int t = cmp_product(-d, -d, a, x); return t < 0 ? 0 : t == 0 ? 1 : 2; }
    int segment_circle_intersection(const ISegment& s, const ICircle& c, Point& p1, Point& p2) { int cnt = segment_circle_relation(s, c); if (cnt == 0) return 0; if (!segment_valid(s)) { p1 = p2 = to_point(s.a); return 1; } int x = point_circle_relation(s.a, c), y = point_circle_relation(s.b, c); if (x == 0 && y == 0) { p1 = to_point(s.a), p2 = to_point(s.b); return 2; } Point q1, q2; line_circle_intersection(ILine(s.a, s.b), c, q1, q2); if (x == 0) { p1 = to_point(s.a), p2 = cnt == 1 ? p1 : q2; return cnt; } if (y == 0) { p1 = cnt == 1 ? to_point(s.b) : q1, p2 = to_point(s.b); return cnt; } if (cnt == 2) { p1 = q1, p2 = q2; return 2; } p1 = p2 = x < 0 ? q2 : q1; return 1; }
    int circle_circle_intersection(const ICircle& a, const ICircle& b, Point& p1, Point& p2) { int rel = circle_circle_relation(a, b); if (rel == 5) { if (a.r != 0) return -1; p1 = p2 = to_point(a.o); return 1; } if (rel == 0 || rel == 4) return 0; db d2 = dis2(a.o, b.o), d = safe_sqrt(d2); ll numerator = dis2(a.o, b.o) + (a.r - b.r) * (a.r + b.r); db x = (db)numerator / (2 * d); Point u = (to_point(b.o) - to_point(a.o)) / d; Point h = to_point(a.o) + u * x; if (rel == 1 || rel == 3) { p1 = p2 = h; return 1; } ll sum = a.r + b.r, dif = llabs(a.r - b.r), dd = dis2(a.o, b.o); db h2 = (db)(sum * sum - dd) * (dd - dif * dif) / (4 * d2); Point v = rotate_90(u) * safe_sqrt(h2); p1 = h - v, p2 = h + v; return 2; }
    bool radical_axis(const ICircle& a, const ICircle& b, Line& l) { IPoint d = b.o - a.o; ll d2 = norm2(d); if (d2 == 0) return false; ll x = d2 + a.r * a.r - b.r * b.r; Point p = to_point(a.o) + to_point(d) * ((db)x / (2 * (db)d2)); l = Line::from_point_vector(p, to_point(rotate_90(d))); return true; }
    bool circle_from_three_points(const IPoint& a, const IPoint& b, const IPoint& c, Circle& res) { IPoint u = b - a, v = c - a; ll cr = cross(u, v); if (cr == 0) return false; ll u2 = norm2(u), v2 = norm2(v); db d = 2 * (db)cr; Point o = to_point(a) + Point(difference_of_products(u2, v.y, v2, u.y) / d, difference_of_products(v2, u.x, u2, v.x) / d); res = Circle(o, dis(o, to_point(a))); return true; }
    int tangent_points(const IPoint& p, const ICircle& c, Point& p1, Point& p2) { int rel = point_circle_relation(p, c); if (rel < 0) return 0; if (c.r == 0) { p1 = p2 = to_point(c.o); return p == c.o ? -1 : 1; } if (rel == 0) { p1 = p2 = to_point(p); return 1; } IPoint iv = p - c.o; db d2 = norm2(iv), r2 = (db)c.r * c.r; Point v = to_point(iv), h = to_point(c.o) + v * (r2 / d2); Point d = rotate_90(v) * (c.r * safe_sqrt(d2 - r2) / d2); p1 = h - d, p2 = h + d; return 2; }
    int common_tangents(const ICircle& a, const ICircle& b, vector<pair<Point, Point>>& res) { res.clear(); if (a.r == 0 || b.r == 0) { if (a.r == 0 && b.r == 0) { if (a.o == b.o) return -1; res.emplace_back(to_point(a.o), to_point(b.o)); return 1; } Point p1, p2; int cnt = a.r == 0 ? tangent_points(a.o, b, p1, p2) : tangent_points(b.o, a, p1, p2); if (cnt >= 1) res.push_back(a.r == 0 ? make_pair(to_point(a.o), p1) : make_pair(p1, to_point(b.o))); if (cnt == 2) res.push_back(a.r == 0 ? make_pair(to_point(a.o), p2) : make_pair(p2, to_point(b.o))); return cnt; } IPoint id = b.o - a.o; ll d2 = norm2(id); if (d2 == 0 && a.r == b.r) return -1; if (d2 == 0) return 0; Point d = to_point(id); int cnt = 0; for (int type : {-1, 1}) { ll r = a.r - type * b.r, h2 = d2 - r * r; if (h2 < 0) continue; db h = safe_sqrt(h2); for (int side : {-1, 1}) { if (h2 == 0 && side == 1) continue; Point v = (d * r + rotate_90(d) * (h * side)) / d2; res.emplace_back(to_point(a.o) + v * a.r, to_point(b.o) + v * (type * b.r)); ++cnt; } } return cnt; }
    struct ITriangle { IPoint a, b, c; ITriangle() {} ITriangle(const IPoint& _a, const IPoint& _b, const IPoint& _c) : a(_a), b(_b), c(_c) {} };
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
    bool triangle_circumcircle(const ITriangle& t, Circle& c) { return circle_from_three_points(t.a, t.b, t.c, c); }
    bool triangle_circumcenter(const ITriangle& t, Point& p) { Circle c; if (!triangle_circumcircle(t, c)) return false; p = c.o; return true; }
    db triangle_circumradius(const ITriangle& t) { return triangle_circumradius(to_triangle(t)); }
    bool triangle_incircle(const ITriangle& t, Circle& c) { return triangle_valid(t) && triangle_incircle(to_triangle(t), c); }
    bool triangle_incenter(const ITriangle& t, Point& p) { Circle c; if (!triangle_incircle(t, c)) return false; p = c.o; return true; }
    db triangle_inradius(const ITriangle& t) { return triangle_inradius(to_triangle(t)); }
    bool triangle_orthocenter(const ITriangle& t, Point& p) { Point o; if (!triangle_circumcenter(t, o)) return false; p = to_point(t.a) + to_point(t.b - t.a) + to_point(t.c - t.a) - (o - to_point(t.a)) * 2; return true; }
    Line triangle_angle_bisector(const IPoint& a, const IPoint& b, const IPoint& c) { return triangle_angle_bisector(to_point(a), to_point(b), to_point(c)); }
    int point_triangle_relation(const IPoint& p, const ITriangle& t) { if (!triangle_valid(t)) return on_segment(p, ISegment(t.a, t.b)) || on_segment(p, ISegment(t.b, t.c)) || on_segment(p, ISegment(t.c, t.a)) ? 0 : -1; int a = orientation(t.a, t.b, p), b = orientation(t.b, t.c, p), c = orientation(t.c, t.a, p); if (a == 0 && on_segment(p, ISegment(t.a, t.b))) return 0; if (b == 0 && on_segment(p, ISegment(t.b, t.c))) return 0; if (c == 0 && on_segment(p, ISegment(t.c, t.a))) return 0; bool pos = a > 0 || b > 0 || c > 0; bool neg = a < 0 || b < 0 || c < 0; return pos && neg ? -1 : 1; }
    vector<IPoint> convex_hull(vector<IPoint> p) { sort(p.begin(), p.end()); p.erase(unique(p.begin(), p.end()), p.end()); if (p.size() <= 1) return p; vector<IPoint> h(p.size() * 2); int top = 0; for (auto& x : p) { while (top >= 2 && orientation(h[top - 2], h[top - 1], x) <= 0) --top; h[top++] = x; } int t = top + 1, n = p.size(); for (int i = n - 2; i >= 0; --i) { while (top >= t && orientation(h[top - 2], h[top - 1], p[i]) <= 0) --top; h[top++] = p[i]; } h.resize(top - 1); return h; }
    vector<IPoint> upper_hull(vector<IPoint> p) { sort(p.begin(), p.end(), [](const IPoint& a, const IPoint& b) { return a.x != b.x ? a.x < b.x : a.y > b.y; }); vector<IPoint> q, h; for (auto& x : p) if (q.empty() || x.x != q.back().x) q.push_back(x); for (auto& x : q) { while (h.size() >= 2 && orientation(h[h.size() - 2], h.back(), x) >= 0) h.pop_back(); h.push_back(x); } return h; }
    vector<IPoint> lower_hull(vector<IPoint> p) { sort(p.begin(), p.end()); vector<IPoint> q, h; for (auto& x : p) if (q.empty() || x.x != q.back().x) q.push_back(x); for (auto& x : q) { while (h.size() >= 2 && orientation(h[h.size() - 2], h.back(), x) <= 0) h.pop_back(); h.push_back(x); } return h; }
    vector<IPoint> upper_right_hull(vector<IPoint> p) { auto h = upper_hull(p); int id = 0; for (int i = 1; i < h.size(); ++i) if (h[i].y >= h[id].y) id = i; h.erase(h.begin(), h.begin() + id); return h; }
    vector<IPoint> upper_left_hull(vector<IPoint> p) { auto h = upper_hull(p); if (h.empty()) return h; int id = 0; for (int i = 1; i < h.size(); ++i) if (h[i].y > h[id].y) id = i; h.resize(id + 1); return h; }
    vector<IPoint> lower_right_hull(vector<IPoint> p) { auto h = lower_hull(p); int id = 0; for (int i = 1; i < h.size(); ++i) if (h[i].y <= h[id].y) id = i; h.erase(h.begin(), h.begin() + id); return h; }
    vector<IPoint> lower_left_hull(vector<IPoint> p) { auto h = lower_hull(p); if (h.empty()) return h; int id = 0; for (int i = 1; i < h.size(); ++i) if (h[i].y < h[id].y) id = i; h.resize(id + 1); return h; }
    struct IPolygon { vector<IPoint> p; IPolygon() {} IPolygon(const vector<IPoint>& _p) : p(convex_hull(_p)) {} int size() const { return p.size(); } bool empty() const { return p.empty(); } const IPoint& operator[](int i) const { return p[i]; } IPoint& operator[](int i) { return p[i]; } };
    Polygon to_polygon(const IPolygon& poly) { Polygon res; for (auto& x : poly.p) res.p.push_back(to_point(x)); return res; }
    bool polygon_valid(const IPolygon& poly) { return poly.size() >= 3; }
    db polygon_perimeter(const IPolygon& poly) { int n = poly.size(); if (n <= 1) return 0; db res = 0; for (int i = 0; i < n; ++i) res += dis(poly[i], poly[(i + 1) % n]); return res; }
    ll polygon_area2_signed(const IPolygon& poly) { int n = poly.size(); if (n < 3) return 0; ll res = 0; for (int i = 1; i + 1 < n; ++i) res += cross(poly[0], poly[i], poly[i + 1]); return res; }
    ll polygon_area2(const IPolygon& poly) { return llabs(polygon_area2_signed(poly)); }
    db polygon_area_signed(const IPolygon& poly) { return (db)polygon_area2_signed(poly) / 2; }
    db polygon_area(const IPolygon& poly) { return (db)polygon_area2(poly) / 2; }
    int point_polygon_relation(const IPoint& x, const IPolygon& poly) { int n = poly.size(); if (n == 0) return -1; if (n == 1) return x == poly[0] ? 0 : -1; if (n == 2) return on_segment(x, ISegment(poly[0], poly[1])) ? 0 : -1; int a = orientation(poly[0], poly[1], x), b = orientation(poly[0], poly[n - 1], x); if (a < 0 || b > 0) return -1; if (a == 0) return on_segment(x, ISegment(poly[0], poly[1])) ? 0 : -1; if (b == 0) return on_segment(x, ISegment(poly[0], poly[n - 1])) ? 0 : -1; int l = 1, r = n - 1; while (r - l > 1) { int mid = (l + r) >> 1; if (orientation(poly[0], poly[mid], x) >= 0) l = mid; else r = mid; } int c = orientation(poly[l], poly[r], x); if (c < 0) return -1; return c == 0 ? 0 : 1; }
    bool segment_in_polygon(const ISegment& s, const IPolygon& poly) { return point_polygon_relation(s.a, poly) >= 0 && point_polygon_relation(s.b, poly) >= 0; }
    namespace detail { struct IClipFraction { ll num, den; }; int compare_clip_fraction(const IClipFraction& a, const IClipFraction& b) { int sa = (a.num > 0) - (a.num < 0); int sb = (b.num > 0) - (b.num < 0); if (sa != sb) return (sa > sb) - (sa < sb); if (sa == 0) return 0; return sa * cmp_fraction(llabs(a.num), a.den, llabs(b.num), b.den); } int clip_integer_convex(const IPoint& p, const IPoint& v, const IPolygon& poly, bool segment, Point& p1, Point& p2) { bool has_lower = segment, has_upper = segment; IClipFraction lower{0, 1}, upper{1, 1}; for (int i = 0; i < poly.size(); ++i) { IPoint edge = poly[(i + 1) % poly.size()] - poly[i]; ll base = cross(edge, p - poly[i]), slope = cross(edge, v); if (slope == 0) { if (base < 0) return 0; continue; } if (slope > 0) { IClipFraction bound{-base, slope}; if (!has_lower || compare_clip_fraction(lower, bound) < 0) lower = bound; has_lower = true; } else { IClipFraction bound{base, -slope}; if (!has_upper || compare_clip_fraction(upper, bound) > 0) upper = bound; has_upper = true; } if (has_lower && has_upper && compare_clip_fraction(lower, upper) > 0) return 0; }   if (!has_lower || !has_upper) return 0; int cmp = compare_clip_fraction(lower, upper); if (cmp > 0) return 0; Point a = to_point(p), d = to_point(v); p1 = a + d * ((db)lower.num / lower.den); p2 = cmp == 0 ? p1 : a + d * ((db)upper.num / upper.den); return cmp == 0 ? 1 : 2; } }
    int segment_polygon_intersection(const ISegment& s, const IPolygon& poly, Point& p1, Point& p2) { int n = poly.size(); if (n == 0) return 0; if (n == 1) { if (!on_segment(poly[0], s)) return 0; p1 = p2 = to_point(poly[0]); return 1; } if (n == 2) { int count = segment_intersection(s, ISegment(poly[0], poly[1]), p1, p2); if (count == 2 && s.b < s.a) swap(p1, p2); return count; } if (!segment_valid(s)) { if (point_polygon_relation(s.a, poly) < 0) return 0; p1 = p2 = to_point(s.a); return 1; } return detail::clip_integer_convex(s.a, s.b - s.a, poly, true, p1, p2); }
    int line_polygon_intersection(const ILine& l, const IPolygon& poly, Point& p1, Point& p2) { int n = poly.size(); if (n == 0) return 0; if (!line_valid(l)) { if (point_polygon_relation(l.p, poly) < 0) return 0; p1 = p2 = to_point(l.p); return 1; } if (n == 1) { if (!on_line(poly[0], l)) return 0; p1 = p2 = to_point(poly[0]); return 1; } if (n == 2) { int count = line_segment_intersection(l, ISegment(poly[0], poly[1]), p1, p2); if (count == 2 && dot(poly[1] - poly[0], l.v) < 0) swap(p1, p2); return count; } return detail::clip_integer_convex(l.p, l.v, poly, false, p1, p2); }
    bool segment_polygon_intersect(const ISegment& s, const IPolygon& poly) { if (point_polygon_relation(s.a, poly) >= 0 || point_polygon_relation(s.b, poly) >= 0) return true; for (int i = 0, n = poly.size(); i < n; ++i) if (segment_intersect(s, ISegment(poly[i], poly[(i + 1) % n]))) return true; return false; }
    int line_polygon_intersection(const Line& l, const Polygon& poly, Point& p1, Point& p2) { if (!line_valid(l)) { if (point_polygon_relation(l.p, poly) < 0) return 0; p1 = p2 = l.p; return 1; } detail::PointExtrema result(l.p, l.v); int n = poly.size(); for (int i = 0; i < n; ++i) { Point x, y; int t = line_segment_intersection(l, Segment(poly[i], poly[(i + 1) % n]), x, y); if (t >= 1) result.add(x); if (t == 2) result.add(y); } return result.result(p1, p2); }
    bool line_polygon_intersect(const Line& l, const Polygon& poly) { Point p1, p2; return line_polygon_intersection(l, poly, p1, p2) != 0; }
    bool line_polygon_intersect(const ILine& l, const IPolygon& poly) { Point p1, p2; return line_polygon_intersection(l, poly, p1, p2) != 0; }
    db polygon_area_signed(const vector<Point>& p) { db area = 0; for (int i = 1; i + 1 < (int)p.size(); ++i) area += cross(p[0], p[i], p[i + 1]); return area / 2; }
    ll polygon_area2_signed(const vector<IPoint>& p) { ll area = 0; for (int i = 1; i + 1 < (int)p.size(); ++i) area += cross(p[0], p[i], p[i + 1]); return area; }
    ll polygon_area2(const vector<IPoint>& p) { return llabs(polygon_area2_signed(p)); }
    db polygon_area_signed(const vector<IPoint>& p) { return (db)polygon_area2_signed(p) / 2; }
    db polygon_area(const vector<Point>& p) { return fabsl(polygon_area_signed(p)); }
    db polygon_area(const vector<IPoint>& p) { return (db)polygon_area2(p) / 2; }
    template<class P> db polygon_perimeter(const vector<P>& p) { db ans = 0; for (int i = 0, n = (int)p.size(); i < n; ++i) ans += dis(p[i], p[(i + 1) % n]); return ans; }
    namespace detail { Point floating_point(const Point& p) { return p; } Point floating_point(const IPoint& p) { return to_point(p); } template<class P> bool on_edge(const P& p, const P& a, const P& b) { if constexpr (is_same_v<P, IPoint>) return on_segment(p, ISegment(a, b)); else return on_segment(p, Segment(a, b)); } }
    template<class P> int point_simple_polygon_relation(const P& x, const vector<P>& p) { int winding = 0, n = p.size(); for (int i = 0; i < n; ++i) { const P& a = p[i]; const P& b = p[(i + 1) % n]; if (detail::on_edge(x, a, b)) return 0; int side = orientation(a, b, x); if (a.y <= x.y && b.y > x.y && side > 0) ++winding; if (a.y > x.y && b.y <= x.y && side < 0) --winding; } return winding == 0 ? -1 : 1; }
    template<class P> bool polygon_centroid(const vector<P>& p, Point& center) { if (p.size() < 3) return false; db area2 = 0; Point moment; for (int i = 1; i + 1 < (int)p.size(); ++i) { db cr = cross(p[0], p[i], p[i + 1]); area2 += cr; moment += (detail::floating_point(p[i] - p[0]) + detail::floating_point(p[i + 1] - p[0])) * cr; } if (area2 == 0) return false; center = detail::floating_point(p[0]) + moment / (3 * area2); return true; }
    bool polygon_centroid(const Polygon& p, Point& center) { return polygon_centroid(p.p, center); }
    bool polygon_centroid(const IPolygon& p, Point& center) { return polygon_centroid(p.p, center); }
    Polygon convex_cut(const Polygon& poly, const Line& l) { Polygon result; if (!line_valid(l)) return result; db len = norm(l.v); auto add = [&](const Point& p) { if (result.empty() || result.p.back() != p) result.p.push_back(p); }; for (int i = 0, n = poly.size(); i < n; ++i) { Point a = poly[i], b = poly[(i + 1) % n]; db x = cross(l.v, a - l.p) / len, y = cross(l.v, b - l.p) / len; int sx = sgn(x), sy = sgn(y); if (sx >= 0) add(a); if (sx * sy < 0) add(a + (b - a) * (x / (x - y))); } if (result.size() > 1 && result.p.front() == result.p.back()) result.p.pop_back();   if (result.size() >= 3 && polygon_area(result) == 0) result = Polygon(result.p); return result; }
    Polygon convex_cut(const IPolygon& poly, const ILine& l) { return convex_cut(to_polygon(poly), to_line(l)); }
    Polygon convex_polygon_intersection(Polygon a, const Polygon& b) { if (a.empty() || b.empty()) return Polygon(); if (b.size() == 1) { Polygon res; if (point_polygon_relation(b[0], a) >= 0) res.p.push_back(b[0]); return res; } if (b.size() == 2) { Point p, q; Polygon res; int cnt = segment_polygon_intersection(Segment(b[0], b[1]), a, p, q); if (cnt) res.p.push_back(p); if (cnt == 2) res.p.push_back(q); return res; } for (int i = 0; i < b.size() && !a.empty(); ++i) a = convex_cut(a, Line(b[i], b[(i + 1) % b.size()])); return a; }
    Polygon convex_polygon_intersection(const IPolygon& a, const IPolygon& b) { return convex_polygon_intersection(to_polygon(a), to_polygon(b)); }
    namespace detail { template<class Poly> auto diameter2(const Poly& poly) { using D = decltype(dis2(poly[0], poly[0])); int n = poly.size(), j = 1; D ans = 0; if (n <= 1) return ans; if (n == 2) return dis2(poly[0], poly[1]); for (int i = 0; i < n; ++i) { auto edge = poly[(i + 1) % n] - poly[i]; while (cross(edge, poly[(j + 1) % n] - poly[i]) > cross(edge, poly[j] - poly[i])) j = (j + 1) % n; ans = max({ans, dis2(poly[i], poly[j]), dis2(poly[(i + 1) % n], poly[j])}); } return ans; } template<class Poly> db min_width(const Poly& poly) { int n = poly.size(), j = 1; if (n <= 2) return 0; db ans = numeric_limits<db>::infinity(); for (int i = 0; i < n; ++i) { auto edge = poly[(i + 1) % n] - poly[i]; while (cross(edge, poly[(j + 1) % n] - poly[i]) > cross(edge, poly[j] - poly[i])) j = (j + 1) % n; ans = min(ans, (db)cross(edge, poly[j] - poly[i]) / norm(edge)); } return ans; } template<class P> auto closest_pair2(vector<P> p) { using D = decltype(dis2(p[0], p[0])); D inf; if constexpr (is_same_v<D, db>) inf = numeric_limits<D>::infinity(); else inf = numeric_limits<D>::max(); sort(p.begin(), p.end()); vector<P> tmp(p.size()); auto by_y = [](const P& a, const P& b) { return a.y != b.y ? a.y < b.y : a.x < b.x; }; auto solve = [&](auto&& self, int l, int r) -> D { if (r - l <= 3) { D best = inf; for (int i = l; i < r; ++i) for (int j = i + 1; j < r; ++j) best = min(best, dis2(p[i], p[j])); sort(p.begin() + l, p.begin() + r, by_y); return best; } int mid = (l + r) / 2; auto x = p[mid].x; D best = min(self(self, l, mid), self(self, mid, r)); merge(p.begin() + l, p.begin() + mid, p.begin() + mid, p.begin() + r, tmp.begin() + l, by_y); copy(tmp.begin() + l, tmp.begin() + r, p.begin() + l); int cnt = 0; for (int i = l; i < r; ++i) { auto dx = p[i].x - x; if (dx * dx >= best) continue; for (int j = cnt - 1; j >= 0; --j) { auto dy = p[i].y - tmp[j].y; if (dy * dy >= best) break; best = min(best, dis2(p[i], tmp[j])); } tmp[cnt++] = p[i]; } return best; }; return solve(solve, 0, (int)p.size()); } }
    db convex_diameter2(const Polygon& poly) { return detail::diameter2(poly); }
    ll convex_diameter2(const IPolygon& poly) { return detail::diameter2(poly); }
    db convex_min_width(const Polygon& poly) { return detail::min_width(poly); }
    db convex_min_width(const IPolygon& poly) { return detail::min_width(poly); }
    db closest_pair_distance2(vector<Point> p) { return detail::closest_pair2(move(p)); }
    ll closest_pair_distance2(vector<IPoint> p) { return p.size() < 2 ? -1 : detail::closest_pair2(move(p)); }
    Circle minimum_enclosing_circle(vector<Point> p, uint64_t seed = chrono::steady_clock::now().time_since_epoch().count()) { mt19937_64 rng(seed); shuffle(p.begin(), p.end(), rng); Circle c(Point(), -1); auto outside = [&](const Point& x) { return c.r < 0 || dis(x, c.o) > c.r + EPS; }; for (int i = 0; i < (int)p.size(); ++i) if (outside(p[i])) { c = Circle(p[i], 0); for (int j = 0; j < i; ++j) if (outside(p[j])) { c = Circle::from_diameter(p[i], p[j]); for (int k = 0; k < j; ++k) if (outside(p[k])) { if (!circle_from_three_points(p[i], p[j], p[k], c)) { Point a = p[i], b = p[j]; if (dis2(p[i], p[k]) > dis2(a, b)) a = p[i], b = p[k]; if (dis2(p[j], p[k]) > dis2(a, b)) a = p[j], b = p[k]; c = Circle::from_diameter(a, b); } } } } return c; }
    Circle minimum_enclosing_circle(const vector<IPoint>& p, uint64_t seed = chrono::steady_clock::now().time_since_epoch().count()) { vector<Point> q; q.reserve(p.size()); for (auto& x : p) q.push_back(to_point(x)); return minimum_enclosing_circle(move(q), seed); }
    namespace detail { db angle_minus_sin(db x) { if (fabsl(x) >= 1e-3L) return x - sinl(x); db x2 = x * x; return x * x2 * (1.0L / 6 - x2 / 120 + x2 * x2 / 5040); } db lens_area(db r1, db r2, db x1, db x2, db h) { return (r1 * r1 * angle_minus_sin(2 * atan2l(h, x1)) + r2 * r2 * angle_minus_sin(2 * atan2l(h, x2))) / 2; } }
    db circle_overlap_area(const Circle& a, const Circle& b) { db d = dis(a.o, b.o), sum = a.r + b.r, dif = fabsl(a.r - b.r); if (d >= sum) return 0; if (d <= dif) return PI * sqr(min(a.r, b.r)); db x = (d * d + (a.r - b.r) * sum) / (2 * d); db h = safe_sqrt((sum - d) * (sum + d) * (d - dif) * (d + dif)) / (2 * d); return detail::lens_area(a.r, b.r, x, d - x, h); }
    db circle_overlap_area(const ICircle& a, const ICircle& b) { int rel = circle_circle_relation(a, b); if (rel <= 1) return 0; if (rel >= 3) return PI * sqr((db)min(a.r, b.r)); ll dd = dis2(a.o, b.o), sum = a.r + b.r, dif = llabs(a.r - b.r); db d = safe_sqrt(dd); db x = (db)(dd + (a.r - b.r) * sum) / (2 * d); db h = safe_sqrt((db)(sum * sum - dd) * (dd - dif * dif)) / (2 * d); return detail::lens_area(a.r, b.r, x, d - x, h); }
    ll polygon_boundary_lattice_points(const IPolygon& poly) { int n = poly.size(); if (n == 0) return 0; if (n == 1) return 1; if (n == 2) return gcd(llabs(poly[1].x - poly[0].x), llabs(poly[1].y - poly[0].y)) + 1; ll ans = 0; for (int i = 0; i < n; ++i) { IPoint d = poly[(i + 1) % n] - poly[i]; ans += gcd(llabs(d.x), llabs(d.y)); } return ans; }
    ll polygon_interior_lattice_points(const IPolygon& poly) { if (poly.size() < 3) return 0; return (polygon_area2(poly) - polygon_boundary_lattice_points(poly) + 2) / 2; }
    vector<Point> upper_right_hull(vector<Point> p) { auto h = upper_hull(move(p)); int id = 0; for (int i = 1; i < (int)h.size(); ++i) if (h[i].y >= h[id].y) id = i; h.erase(h.begin(), h.begin() + id); return h; }
    vector<Point> upper_left_hull(vector<Point> p) { auto h = upper_hull(move(p)); if (h.empty()) return h; int id = 0; for (int i = 1; i < (int)h.size(); ++i) if (h[i].y > h[id].y) id = i; h.resize(id + 1); return h; }
    vector<Point> lower_right_hull(vector<Point> p) { auto h = lower_hull(move(p)); int id = 0; for (int i = 1; i < (int)h.size(); ++i) if (h[i].y <= h[id].y) id = i; h.erase(h.begin(), h.begin() + id); return h; }
    vector<Point> lower_left_hull(vector<Point> p) { auto h = lower_hull(move(p)); if (h.empty()) return h; int id = 0; for (int i = 1; i < (int)h.size(); ++i) if (h[i].y < h[id].y) id = i; h.resize(id + 1); return h; }
} using namespace Geometry;
