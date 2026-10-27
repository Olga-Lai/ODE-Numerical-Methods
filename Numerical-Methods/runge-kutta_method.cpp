
#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

// ODE: y' = f(t, y) = ty + t^3
double f(double t, double y) {
    return t * y + t * t * t;
}

// Exact solution: y = 3e^(t^2/2) - t^2 - 2
double exact_solution(double t) {
    return 3 * exp(t * t / 2) - t * t - 2;
}

int main() {
    double t0 = 0.0;
    double y0 = 1.0;
    double h = 0.1;
    double T = 1.0;

    int N = static_cast<int>((T - t0) / h);

    double t = t0;
    double y = y0;

    cout << fixed << setprecision(8);
    cout << "t\t\tRK4\t\tExact\t\tError\n";

    cout << t << "\t" << y << "\t"
         << exact_solution(t) << "\t"
         << abs(y - exact_solution(t)) << "\n";

    for (int n = 0; n < N; n++) {
        // Step 1: Evaluate the derivative at the current point
        double k1 = f(t, y);

        // Step 2: Evaluate the derivative at the first midpoint estimate
        double k2 = f(t + h / 2.0,
                      y + (h / 2.0) * k1);

        // Step 3: Evaluate the derivative at the second midpoint estimate
        double k3 = f(t + h / 2.0,
                      y + (h / 2.0) * k2);

        // Step 4: Evaluate the derivative at the predicted endpoint
        double k4 = f(t + h,
                      y + h * k3);

        // Step 5: Update the solution using the weighted average
        y = y + (h / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4);
        t = t + h;

        double error = abs(y - exact_solution(t));

        cout << t << "\t" << y << "\t"
             << exact_solution(t) << "\t"
             << error << "\n";
    }

    cout << "\nFinal absolute error: "
         << abs(y - exact_solution(T)) << "\n";

    return 0;
}
