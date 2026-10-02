#include <bits/stdc++.h>
using namespace std;

// 利用单调栈求出柱状图包含的最大矩形面积，复杂度 O(n)
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        heights.push_back(-1);
        stack<int> st; st.push(-1);
        int n = heights.size(), res = 0;
        for (int r = 0; r < n; ++r) {
            int h = heights[r];
            while (st.size() > 1 && heights[st.top()] >= h) {
                int i = st.top();
                st.pop();
                int l = st.top();
                res = max(res, heights[i] * (r - l - 1));
            }
            st.push(r);
        }
        return res;
    }
};