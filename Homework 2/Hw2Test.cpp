#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

using std::string;

class Projectile {
public:
    double height = 60;
    double v0 = 50;
    double airDensity = 1.29;
    double dragCoeff = 0.5;
    double rockMassDensity = 2400;
    double rockDiameter = 0.25; // 25 cm ball
    double g = 9.8;
    double dt = 0.005;

    double trajectory(double theta0, string file1, bool saveCSV) {
        double thetaRadians = theta0 * (M_PI / 180);
        double radius = rockDiameter / 2;
        double area = M_PI * pow(radius, 2);
        double mass = (4.0 / 3.0) * M_PI * pow(radius, 3) * rockMassDensity;

        double Vx = v0 * cos(thetaRadians);
        double Vy = v0 * sin(thetaRadians);

        double x = 0.0, y = height, t = 0.0;
        double x_prev = x, y_prev = y;

        while (y >= 0) {
            double v = sqrt(Vx * Vx + Vy * Vy);
            double F_drag = 0.5 * dragCoeff * v * v * area * airDensity;

            double accelX = -F_drag * (Vx / v) / mass;
            double accelY = -g - (F_drag * (Vy / v) / mass);

            Vx += accelX * dt;
            Vy += accelY * dt;
            x += Vx * dt;
            y += Vy * dt;

            x_prev = x;
            y_prev = y;
        }

        return x;
    }
};

int main() {
    Projectile obj;

    double maxRange = 0.0;
    double bestAngle = 0.0;

    for (double angle = 5.0; angle <= 85.0; angle += 0.5) {
        double range = obj.trajectory(angle, "temp.csv", false);
        if (range > maxRange) {
            maxRange = range;
            bestAngle = angle;
        }
    }

    double targetCenter = maxRange - 10.0;
    double targetLeft = targetCenter - 1.0 - 0.125; // 0.125m to account for ball size
    double targetRight = targetCenter + 1.0 + 0.125; // Full projectile must fit

    std::cout << "Maximum range: " << maxRange << " meters at " << bestAngle << " degrees\n";
    std::cout << "Target positioned at: " << targetCenter << " meters (from " << targetLeft << "m to " << targetRight << "m)\n";

    double theta_lower = -1, theta_upper = -1;
    double prev_angle = 5.0, prev_range = obj.trajectory(prev_angle, "temp.csv", false);

    for (double angle = 5.0; angle <= bestAngle; angle += 0.1) {
        double range = obj.trajectory(angle, "temp.csv", false);

        if (range >= targetLeft && prev_range < targetLeft) {
            if (range != prev_range) {
                theta_lower = prev_angle + ((targetLeft - prev_range) / (range - prev_range)) * (angle - prev_angle);
            }
        }
        if (range >= targetRight && prev_range < targetRight) {
            if (range != prev_range) {
                theta_upper = prev_angle + ((targetRight - prev_range) / (range - prev_range)) * (angle - prev_angle);
            }
            break;
        }

        prev_angle = angle;
        prev_range = range;
    }

    if (theta_lower > 0 && theta_upper > 0) {
        double angle_accuracy = theta_upper - theta_lower;
        std::cout << "Launch angle must be between " << theta_lower << "° and " << theta_upper << "°\n";
        std::cout << "Required angle accuracy: " << angle_accuracy << " degrees\n";
    } else {
        std::cout << "❌ No valid angles found within the target range.\n";
    }

    return 0;
}
