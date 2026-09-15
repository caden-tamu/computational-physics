#include <iostream>
#include <vector>
#include <cmath>
#include <complex>
#include <string>
#include <algorithm>
#include <fstream>

using namespace std;

// Constants
const double PI = 3.141592653589793;
const double L = 2.0;             // Length of string (m)
const double FT = 720.0;          // Tension (N)
const double mass = 0.025;        // Total mass (kg)
const double mu = mass / L;       // Linear density
const double v = sqrt(FT / mu);   // Wave speed
const double f1 = v / (2 * L);    // Fundamental harmonic
const int max_harmonic = 15;
const double f_target = f1 * max_harmonic;
const double nyquist_margin = 1.1; // Slight margin
const double dt = 1.0 / (2 * f_target * nyquist_margin); // Time step (s)
const int N = 2048;               // Number of time samples
const double T = N * dt;          // Total time
const double df = 1.0 / T;        // Frequency resolution
const double f_nyquist = 1.0 / (2 * dt); // Nyquist frequency

// Generate the time-domain signal (using 1st and 3rd harmonics)
vector<double> generate_signal() {
    vector<double> y(N);
    for (int n = 0; n < N; ++n) {
        double t = n * dt;
        y[n] = 0.6 * sin(2 * PI * f1 * t) + 0.3 * sin(2 * PI * 3 * f1 * t);
    }
    return y;
}

// Compute the Discrete Fourier Transform (DFT) manually
vector<complex<double>> compute_dft(const vector<double>& y) {
    vector<complex<double>> Y(N);
    for (int j = 0; j < N; ++j) {
        complex<double> sum = 0.0;
        for (int n = 0; n < N; ++n) {
            double angle = 2 * PI * j * n / N;
            sum += y[n] * exp(complex<double>(0, angle));
        }
        Y[j] = dt * sum;
    }
    return Y;
}

// Compute the inverse DFT to reconstruct the signal
vector<double> inverse_dft(const vector<complex<double>>& Y) {
    vector<double> y_rec(N);
    for (int n = 0; n < N; ++n) {
        complex<double> sum = 0.0;
        for (int j = 0; j < N; ++j) {
            double angle = -2 * PI * j * n / N;
            sum += Y[j] * exp(complex<double>(0, angle));
        }
        y_rec[n] = real(sum) / (N * dt);
    }
    return y_rec;
}

// Save two-column CSV file (time, signal)
void save_signal_to_file(const vector<double>& t, const vector<double>& y, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file " << filename << endl;
        return;
    }
    for (size_t i = 0; i < t.size(); ++i) {
        file << t[i] << "," << y[i] << "\n";
    }
    file.close();
}

// Save two-column CSV file (frequency, power)
void save_spectrum_to_file(const vector<double>& freqs, const vector<double>& power, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file " << filename << endl;
        return;
    }
    for (size_t i = 0; i < freqs.size(); ++i) {
        file << freqs[i] << "," << power[i] << "\n";
    }
    file.close();
}

// Save three-column CSV file (time, original signal, reconstructed signal)
void save_comparison_to_file(const vector<double>& t, const vector<double>& orig, const vector<double>& rec, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file " << filename << endl;
        return;
    }
    for (size_t i = 0; i < t.size(); ++i) {
        file << t[i] << "," << orig[i] << "," << rec[i] << "\n";
    }
    file.close();
}

// Print the power of the harmonics near expected positions
void print_harmonic_peaks(const vector<double>& power) {
    cout << "\nHarmonics:\n";
    for (int n = 1; n <= max_harmonic; ++n) {
        int j = round((f1 * n) / df);
        if (j < power.size())
            cout << "  Harmonic " << n << ": ~" << n * f1 << " Hz \t Power = " << power[j] << endl;
    }
}

int main() {
    cout << "Wave speed v = " << v << " m/s\n";
    cout << "Fundamental f1 = " << f1 << " Hz\n";
    cout << "Nyquist frequency = " << f_nyquist << " Hz\n";
    cout << "Time step dt = " << dt << " s\n";

    // Generate signal and time vector
    auto y = generate_signal();
    vector<double> t_vals(N);
    for (int n = 0; n < N; ++n)
        t_vals[n] = n * dt;
    
    // Save time-domain signal data to CSV
    save_signal_to_file(t_vals, y, "time_signal.csv");

    // Compute the DFT of the signal
    auto Y = compute_dft(y);
    
    // Compute the power spectrum for frequencies up to the Nyquist limit
    vector<double> power;
    vector<double> freqs;
    for (int j = 0; j <= N / 2; ++j) {
        double freq = j * df;
        double power_val = norm(Y[j]);
        freqs.push_back(freq);
        power.push_back(power_val);
    }
    // Save power spectrum data to CSV
    save_spectrum_to_file(freqs, power, "power_spectrum.csv");

    // Print harmonic peaks
    print_harmonic_peaks(power);

    // Compute the inverse DFT for signal reconstruction
    auto y_reconstructed = inverse_dft(Y);
    // Save comparison data (original vs. reconstructed) to CSV
    save_comparison_to_file(t_vals, y, y_reconstructed, "comparison.csv");

    cout << "\nData files saved: time_signal.csv, power_spectrum.csv, comparison.csv\n";
    return 0;
}
