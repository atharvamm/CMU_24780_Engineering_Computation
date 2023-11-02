#include <iostream>
#include "trinagulate.h"

bool isEar(const Point& a, const Point& b, const Point& c, const std::vector<Point>& polygon) {
    // Check if triangle ABC is an ear by verifying if no other point is inside it.
    for (const Point& p : polygon) {
        if ((p.x != a.x || p.y != a.y) && (p.x != b.x || p.y != b.y) && (p.x != c.x || p.y != c.y)) {
            double det = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
            if (det == 0) continue;

            double alpha = ((c.y - a.y) * (p.x - a.x) - (c.x - a.x) * (p.y - a.y)) / det;
            double beta = ((a.y - b.y) * (p.x - a.x) - (a.x - b.x) * (p.y - a.y)) / det;

            if (alpha >= 0 && beta >= 0 && (alpha + beta) <= 1)
                return false;
        }
    }
    return true;
}

std::vector<std::vector<Point>> triangulateConcavePolygon(const std::vector<Point>& polygon) {
    std::vector<std::vector<Point>> triangles;

    std::vector<Point> remainingPoints = polygon;

    while (remainingPoints.size() > 3) {
        int n = remainingPoints.size();
        for (int i = 0; i < n; i++) {
            int prevIdx = (i - 1 + n) % n;
            int currIdx = i;
            int nextIdx = (i + 1) % n;

            const Point& prev = remainingPoints[prevIdx];
            const Point& curr = remainingPoints[currIdx];
            const Point& next = remainingPoints[nextIdx];

            if (isEar(prev, curr, next, remainingPoints)) {
                triangles.push_back({prev, curr, next});
                remainingPoints.erase(remainingPoints.begin() + currIdx);
                break;
            }
        }
    }

    triangles.push_back(remainingPoints); // The remaining three points form the last triangle.

    return triangles;
}
