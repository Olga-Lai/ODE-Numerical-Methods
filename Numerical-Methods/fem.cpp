
#include <cmath>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <vector>

using namespace std;

// Exact solution of u''(x) + u(x) = x
double exactSolution(double x) {
    return x - sin(x) / sin(1.0);
}

// Thomas algorithm
vector<double> thomasAlgorithm(
    const vector<double>& a,
    const vector<double>& b,
    const vector<double>& c,
    const vector<double>& d
) {
    int n = b.size();

    if (n == 0 || a.size() != n ||
        c.size() != n || d.size() != n) {
        throw invalid_argument("Invalid vector sizes.");
    }

    vector<double> ct(n, 0.0);
    vector<double> dt(n, 0.0);
    vector<double> x(n, 0.0);

    if (abs(b[0]) < 1e-12)
        throw runtime_error("Zero pivot.");

    ct[0] = c[0] / b[0];
    dt[0] = d[0] / b[0];

    for (int i = 1; i < n; i++) {
        double m = b[i] - a[i] * ct[i - 1];

        if (abs(m) < 1e-12)
            throw runtime_error("Zero pivot.");

        if (i < n - 1)
            ct[i] = c[i] / m;

        dt[i] = (d[i] - a[i] * dt[i - 1]) / m;
    }

    x[n - 1] = dt[n - 1];

    for (int i = n - 2; i >= 0; i--)
        x[i] = dt[i] - ct[i] * x[i + 1];

    return x;
}

int main() {

    // Number of elements
    int N = 10;

    double h = 1.0 / N;
    int n = N - 1;

    // Global tridiagonal system
    vector<double> a(n, 0.0);
    vector<double> b(n, 0.0);
    vector<double> c(n, 0.0);
    vector<double> d(n, 0.0);

    // Element assembly
    for (int e = 0; e < N; e++) {

        double xL = e * h;
        double xR = (e + 1) * h;

        //
        double ke[2][2] = {
            {1.0 / h - h / 3.0, -1.0 / h - h / 6.0},
            {-1.0 / h - h / 6.0, 1.0 / h - h / 3.0}
        };

        //
        double fe[2] = {
            -h * (2.0 * xL + xR) / 6.0,
            -h * (xL + 2.0 * xR) / 6.0
        };

        int nodes[2] = {e, e + 1};

        // Assemble global system
        for (int i = 0; i < 2; i++) {

            int gi = nodes[i];

            if (gi == 0 || gi == N)
                continue;

            int row = gi - 1;

            d[row] += fe[i];

            for (int j = 0; j < 2; j++) {

                int gj = nodes[j];

                if (gj == 0 || gj == N)
                    continue;

                int col = gj - 1;

                if (row == col)
                    b[row] += ke[i][j];
                else if (col == row - 1)
                    a[row] += ke[i][j];
                else if (col == row + 1)
                    c[row] += ke[i][j];
            }
        }
    }

    // Solve the linear system
    vector<double> u = thomasAlgorithm(a, b, c, d);

    // Compare numerical and exact solutions
    double maxError = 0.0;

    cout << fixed << setprecision(10);

    cout << "    x           Numerical       Exact"
         << "           Error\n";

    for (int i = 0; i <= N; i++) {

        double x = i * h;
        double numerical = 0.0;

        if (i > 0 && i < N)
            numerical = u[i - 1];

        double exact = exactSolution(x);
        double error = abs(numerical - exact);

        maxError = max(maxError, error);

        cout << setw(8) << x << "  "
             << setw(14) << numerical << "  "
             << setw(14) << exact << "  "
             << setw(14) << error << '\n';
    }

    cout << "\nMaximum nodal absolute error: "
         << maxError << '\n';

    return 0;
}
