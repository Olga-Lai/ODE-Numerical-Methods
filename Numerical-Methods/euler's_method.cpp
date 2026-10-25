
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <vector>

struct Point {
    double t;
    double y;
};

std::vector<Point> euler(
    const std::function<double(double, double)>& f,
    double t0,
    double y0,
    double h,
    int steps
) {
    std::vector<Point> result;
    result.reserve(steps + 1);

    double t = t0;
    double y = y0;

    result.push_back({t, y});

    for (int n = 0; n < steps; n++) {
        y = y + h * f(t, y);
        t = t0 + (n + 1) * h;

        result.push_back({t, y});
    }

    return result;
}

int main() {
    // y' = ty+t^3, y(0) = 1
    auto f = [](double t, double y) {
        return t*y+t*t*t;
    };

    double t0 = 0.0;
    double y0 = 1.0;
    double h = 0.1;
    int steps = 10;

    auto solution = euler(f, t0, y0, h, steps);

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "t\t\t\tEuler\t\tExact\t\tAbs.Error\n";

    for (const auto& p : solution) {
        double exact = 3*std::exp(p.t*p.t/2)-p.t*p.t-2;
        double error = std::abs(exact - p.y);

        std::cout << p.t << '\t'
                  << p.y << '\t'
                  << exact << '\t'
                  << error << '\n';
    }

    return 0;
}
