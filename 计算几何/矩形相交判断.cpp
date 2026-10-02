#include <bits/stdc++.h>
using namespace std;

/** 左下角坐标 (x1,y1)，右下角坐标 (x2,y2) */
struct Rectangle {
	int x1, y1;
	int x2, y2;
};
bool intersect(Rectangle a, Rectangle b) {
	return max(a.x1, b.x1) <= min(a.x2, b.x2)
	&& max(a.y1, b.y1) <= min(a.y2, b.y2);
}