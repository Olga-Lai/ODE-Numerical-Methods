
#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

// ODE: y' = f(t, y) = ty+t^3
double f(double t, double y) {
    return t*y+t*t*t;
}

// y = 3e^(t^2/2)-t^2-2
double exact_solution(double t) {
    return 3*exp(t*t/2)-t*t-2;
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
    cout << "t\t\t\t Trapezoid\t Exact\t\t Error \n";

    cout << t << "\t " << y << "\t " << exact_solution(t) << "\t "
             << std::abs(y-exact_solution(t)) << "\n";

    for (int n = 0; n < N; n++) {
        // Step 1: Evaluate the derivative at the current point
        double k1 = f(t, y);

        // Step 2: Predict the next value using Euler's method
        double y_predict = y + h * k1;
        double t_next = t + h;

        // Step 3: Evaluate the derivative at the predicted endpoint
        double k2 = f(t_next, y_predict);

        // Step 4: Correct the value using the trapezoidal formula
        y = y + (h / 2.0) * (k1 + k2);
        t = t_next;

        double error = std::abs(y-exact_solution(t));
        cout << t << "\t " << y << "\t " << exact_solution(t) << "\t "
             << error << "\n";
    }

    cout << "\nFinal absolute error: "
         << abs(y - exact_solution(T)) << "\n";

    return 0;
}
