
#include <cmath>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>

using namespace std;

//Exact solution of -u''(x) = x^2
double exactSolution(double x) {
    return (x-pow(x,4))/12;

    // Exact solution of -u''(x) + u(x) = 1
    //return 1.0 - cosh(x - 0.5) / cosh(0.5);
}

// Solve a tridiagonal system using the Thomas algorithm.
// a: lower diagonal
// b: main diagonal
// c: upper diagonal
// d: right-hand side
vector<double> thomasAlgorithm(
    const vector<double>& a,
    const vector<double>& b,
    const vector<double>& c,
    const vector<double>& d
) {
    int n = b.size();

    if (n == 0 || a.size() != n || c.size() != n || d.size() != n) {
        throw invalid_argument("Invalid vector sizes.");
    }

    vector<double> c_tilde(n, 0.0);
    vector<double> d_tilde(n, 0.0);
    vector<double> x(n, 0.0);

    // Forward elimination
    if (abs(b[0]) < 1e-12) {
        throw runtime_error("Zero pivot in Thomas algorithm.");
    }

    c_tilde[0] = c[0] / b[0];
    d_tilde[0] = d[0] / b[0];

    for (int i = 1; i < n; i++) {
        double m = b[i] - a[i] * c_tilde[i - 1];

        if (abs(m) < 1e-12) {
            throw runtime_error("Zero pivot in Thomas algorithm.");
        }

        if (i < n - 1) {
            c_tilde[i] = c[i] / m;
        }

        d_tilde[i] = (d[i] - a[i] * d_tilde[i - 1]) / m;
    }

    // Backward substitution
    x[n - 1] = d_tilde[n - 1];

    for (int i = n - 2; i >= 0; i--) {
        x[i] = d_tilde[i] - c_tilde[i] * x[i + 1];
    }

    return x;
}

int main() {
    // Number of subintervals
    int N = 10;

    double h = 1.0 / N;
    int n = N - 1;  // Number of interior points

    // Construct the tridiagonal system
    vector<double> a(n, -1.0 / (h * h));
    vector<double> b(n,  2.0 / (h * h));
    vector<double> c(n, -1.0 / (h * h));
    vector<double> d(n, 1.0);

    for (int i = 0; i < n; ++i) {
        double xi = (i + 1) * h;
        d[i] = xi * xi;
    }

    // Boundary values are zero, so no boundary
    // contributions need to be added to d.
    a[0] = 0.0;
    c[n - 1] = 0.0;

    vector<double> u = thomasAlgorithm(a, b, c, d);

    // Compare numerical and exact solutions
    double maxError = 0.0;

    cout << fixed << setprecision(8);
    cout << "    x        Numerical       Exact"
         << "          Error\n";

    cout << 0.0 << "    " << 0.0 << "    "
         << exactSolution(0.0) << "    " << 0.0 << '\n';

    for (int i = 1; i < N; ++i) {
        double x = i * h;
        double exact = exactSolution(x);
        double error = abs(u[i - 1] - exact);

        maxError = max(maxError, error);

        cout << setw(8) << x << "  "
             << setw(12) << u[i - 1] << "  "
             << setw(12) << exact << "  "
             << setw(12) << error << '\n';
    }

    cout << 1.0 << "    " << 0.0 << "    "
         << exactSolution(1.0) << "    " << 0.0 << '\n';

    cout << "\nMaximum absolute error: "
         << maxError << '\n';

    return 0;
}
