#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <complex>
#include <algorithm>

using namespace std;

//-------------------------------------------
// 1) Physical and numerical parameters
//-------------------------------------------
const double PI = 3.141592653589793;
const double L = 2.0;            // String length (m)
const double FT = 720.0;         // Tension (N)
const double mass = 0.025;       // Total mass (kg)
const double mu = mass / L;      // Linear density (kg/m)
const double c = sqrt(FT / mu);  // Wave speed (m/s)

// Courant number (must be <= 1 for stability)
double R = 1.0;

// Grid resolution
int Nx = 300;          // Number of spatial intervals
double dx = L / Nx;    // Spatial step

// We pick Nt to be large enough to capture multiple cycles
int Nt = 4000;         // Number of time steps
double dt = dx / (R * c); // Time step (s)

//-------------------------------------------
// 2) Initial condition: Centered Gaussian
//-------------------------------------------
const double Y0   = 0.25;       // Peak amplitude
const double x_bar = L / 2.0;   // Center the pluck at L/2
const double fwhm = 0.2;        // Full width at half max
const double sigma = fwhm / (2 * sqrt(2 * log(2)));

//-------------------------------------------
// 3) Wave equation solver (finite difference)
//-------------------------------------------
void initialize_gaussian(vector<double>& y0) {
    for (int i = 0; i <= Nx; ++i) {
        double x = i * dx;
        y0[i] = Y0 * exp( - (x - x_bar)*(x - x_bar) / (2 * sigma * sigma) );
    }
}

void simulate_wave(vector<vector<double>>& y) {
    // Initialize
    initialize_gaussian(y[0]);
    y[1] = y[0];  // zero initial velocity

    // Main update loop
    for (int n = 1; n < Nt - 1; ++n) {
        for (int i = 1; i < Nx; ++i) {
            y[n + 1][i] = 2.0 * (1.0 - R*R) * y[n][i]
                         - y[n - 1][i]
                         + R*R * (y[n][i + 1] + y[n][i - 1]);
        }
        // Fixed ends
        y[n + 1][0] = 0.0;
        y[n + 1][Nx] = 0.0;
    }
}

//-------------------------------------------
// 4) Write data to files for plotting
//-------------------------------------------
void save_snapshots_csv(const vector<vector<double>>& y, int step_interval) {
    // We will record the wave every "step_interval" time steps.
    // CSV columns: x, then displacement at each snapshot.
    ofstream file("snapshots.csv");
    if (!file.is_open()) {
        cerr << "Cannot open snapshots.csv\n";
        return;
    }

    // Times at which we record snapshots
    vector<int> snapshots;
    for (int n = 0; n < Nt; n += step_interval) {
        snapshots.push_back(n);
    }

    // Write header
    file << "x";
    for (auto n : snapshots) {
        file << ", t=" << (n * dt);
    }
    file << "\n";

    // Write rows for each spatial index
    for (int i = 0; i <= Nx; ++i) {
        double x_val = i * dx;
        file << x_val;
        for (auto n : snapshots) {
            file << ", " << y[n][i];
        }
        file << "\n";
    }
    file.close();
    cout << "Wrote snapshots to snapshots.csv\n";
}

// 3D data: entire wave in JSON (for surface plotting)
void save_3d_data_json(const vector<vector<double>>& y) {
    ofstream file("data.json");
    if (!file.is_open()) {
        cerr << "Cannot open data.json\n";
        return;
    }

    file << "{\n";
    // X array (spatial positions)
    file << " \"X\": [";
    for (int i = 0; i <= Nx; ++i) {
        if (i > 0) file << ", ";
        file << i * dx;
    }
    file << "],\n";

    // Y array (time steps)
    file << " \"Y\": [";
    for (int n = 0; n < Nt; ++n) {
        if (n > 0) file << ", ";
        file << n * dt;
    }
    file << "],\n";

    // Z array (2D array of displacements)
    file << " \"Z\": [\n";
    for (int n = 0; n < Nt; ++n) {
        file << "  [";
        for (int i = 0; i <= Nx; ++i) {
            if (i > 0) file << ", ";
            file << y[n][i];
        }
        file << "]";
        if (n < Nt - 1) file << ",\n";
        else file << "\n";
    }
    file << " ]\n";
    file << "}\n";

    file.close();
    cout << "Wrote 3D data to data.json\n";
}

//-------------------------------------------
// 5) Time signal at the midpoint & DFT
//-------------------------------------------

// Return the displacement at the midpoint vs. time
vector<double> get_time_signal(const vector<vector<double>>& y) {
    // Midpoint index
    int x_s = Nx / 2;  // Nx is 300, so x_s=150 => x=1.0 m for L=2

    vector<double> signal(Nt);
    for (int n = 0; n < Nt; ++n) {
        signal[n] = y[n][x_s];
    }
    return signal;
}

// Save time signal to CSV
void save_time_signal_csv(const vector<double>& signal) {
    ofstream file("time_signal.csv");
    if (!file.is_open()) {
        cerr << "Cannot open time_signal.csv\n";
        return;
    }
    file << "time,signal\n";
    for (int n = 0; n < Nt; ++n) {
        double t = n * dt;
        file << t << "," << signal[n] << "\n";
    }
    file.close();
    cout << "Wrote time signal to time_signal.csv\n";
}

// Manual DFT
vector<complex<double>> compute_dft(const vector<double>& y) {
    vector<complex<double>> Y(Nt, complex<double>(0.0, 0.0));
    for (int k = 0; k < Nt; ++k) {
        for (int n = 0; n < Nt; ++n) {
            double angle = 2.0 * PI * k * n / Nt;
            Y[k] += y[n] * exp(complex<double>(0.0, angle));
        }
        // Scale by dt so that Y has correct amplitude dimension
        Y[k] *= dt;
    }
    return Y;
}

// Inverse DFT
vector<double> inverse_dft(const vector<complex<double>>& Y) {
    vector<double> y_rec(Nt, 0.0);
    for (int n = 0; n < Nt; ++n) {
        complex<double> sum(0.0, 0.0);
        for (int k = 0; k < Nt; ++k) {
            double angle = -2.0 * PI * k * n / Nt;
            sum += Y[k] * exp(complex<double>(0.0, angle));
        }
        // 1/(Nt*dt) from the forward scaling
        y_rec[n] = real(sum) / (Nt * dt);
    }
    return y_rec;
}

// Save power spectrum
void save_power_spectrum_csv(const vector<complex<double>>& Y) {
    // Frequency resolution
    double df = 1.0 / (Nt * dt);
    ofstream file("power_spectrum.csv");
    if (!file.is_open()) {
        cerr << "Cannot open power_spectrum.csv\n";
        return;
    }
    file << "frequency,power\n";
    for (int k = 0; k <= Nt / 2; ++k) {
        double freq = k * df;
        double p = norm(Y[k]); // power is magnitude squared
        file << freq << "," << p << "\n";
    }
    file.close();
    cout << "Wrote power spectrum to power_spectrum.csv\n";
}

// Save original vs. reconstructed
void save_reconstruction_csv(const vector<double>& y_orig, const vector<double>& y_rec) {
    ofstream file("reconstruction.csv");
    if (!file.is_open()) {
        cerr << "Cannot open reconstruction.csv\n";
        return;
    }
    file << "time,original,reconstructed\n";
    for (int n = 0; n < Nt; ++n) {
        double t = n * dt;
        file << t << "," << y_orig[n] << "," << y_rec[n] << "\n";
    }
    file.close();
    cout << "Wrote reconstruction to reconstruction.csv\n";
}

//-------------------------------------------
// 6) Main
//-------------------------------------------
int main() {
    // Print some info
    cout << "String length L = " << L << " m\n";
    cout << "Wave speed c = "   << c << " m/s\n";
    cout << "Fundamental frequency f1 = c/(2L) = " << c/(2*L) << " Hz\n";
    cout << "Nx = " << Nx << ", dx = " << dx << " m\n";
    cout << "Nt = " << Nt << ", dt = " << dt << " s\n";
    cout << "Total simulation time T = " << (Nt * dt) << " s\n";
    cout << "Courant number R = " << R << "\n\n";

    // Prepare 2D array for wave data: y[n][i]
    vector<vector<double>> y(Nt, vector<double>(Nx + 1, 0.0));

    // Run the finite-difference wave simulation
    simulate_wave(y);

    // Save snapshots for visualization
    save_snapshots_csv(y, /*step_interval=*/400);

    // Save full 3D data
    save_3d_data_json(y);

    // Extract time signal at midpoint
    auto signal = get_time_signal(y);
    save_time_signal_csv(signal);

    // Compute DFT and power spectrum
    auto Y = compute_dft(signal);
    save_power_spectrum_csv(Y);

    // Reconstruct signal from DFT
    auto signal_rec = inverse_dft(Y);
    save_reconstruction_csv(signal, signal_rec);

    cout << "\nAll data files have been saved.\n";
    cout << "You can now plot them with your Python script.\n";
    return 0;
}
