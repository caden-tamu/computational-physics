#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>

int main() {
    double mu0I = 3e-6;   // μ0 * I
    double d = 0.1;       // diameter
    double R = d / 2.0;   // radius

    int N_phi = 2000;
    int N_x   = 200;

    double x_min = -0.1;
    double x_max =  0.1;
    double dx = (x_max - x_min) / (N_x - 1);

    double phi_start = 0.0;
    double phi_end   = 2.0 * M_PI;
    double dphi = (phi_end - phi_start) / (N_phi - 1);

    // We'll store Bx, By, Bz vs x in a CSV
    std::ofstream outfile("BxByBz_vs_x_skip_band.csv");
    outfile << "x,Bx,By,Bz\n";

    // Skip a band of ±(band) around x = ±R
    double band = 0.001;  // skip 0.05 ± 0.001 => [0.049, 0.051]

    for (int i = 0; i < N_x; i++) {
        double x = x_min + i * dx;

        // If x is within ±band of ±R, skip it
        if (std::fabs(std::fabs(x) - R) < band) {
            continue;  // do not compute or output
        }

        double Bx_sum = 0.0;
        double By_sum = 0.0;
        double Bz_sum = 0.0;

        for (int j = 0; j < N_phi; j++) {
            double phi = phi_start + j * dphi;

            // r'(φ) = (R cosφ, R sinφ, 0)
            // dr'   = (-R sinφ, R cosφ, 0)
            double drx = -R * std::sin(phi);
            double dry =  R * std::cos(phi);
            double drz =  0.0;

            // Field point in the plane => (x, 0, 0)
            double rx = x - R * std::cos(phi);
            double ry =    - R * std::sin(phi);
            double rz = 0.0;

            double cross_x = dry * rz - drz * ry;
            double cross_y = drz * rx - drx * rz;
            double cross_z = drx * ry - dry * rx;

            double r_mag = std::sqrt(rx*rx + ry*ry + rz*rz);
            double denom = r_mag*r_mag*r_mag;

            double dBx = (mu0I / (4.0 * M_PI)) * (cross_x / denom);
            double dBy = (mu0I / (4.0 * M_PI)) * (cross_y / denom);
            double dBz = (mu0I / (4.0 * M_PI)) * (cross_z / denom);

            // Trapezoid weighting
            if (j == 0 || j == N_phi - 1) {
                Bx_sum += dBx;
                By_sum += dBy;
                Bz_sum += dBz;
            } else {
                Bx_sum += 2.0 * dBx;
                By_sum += 2.0 * dBy;
                Bz_sum += 2.0 * dBz;
            }
        }

        Bx_sum *= (dphi / 2.0);
        By_sum *= (dphi / 2.0);
        Bz_sum *= (dphi / 2.0);

        outfile << std::fixed << std::setprecision(8)
                << x << "," << Bx_sum << "," << By_sum << "," << Bz_sum << "\n";
    }

    outfile.close();
    std::cout << "Data written to BxByBz_vs_x_skip_band.csv\n";
    return 0;
}
