# computational-physics
This repository showcases the work I completed during my time in undergrad at Texas A\&M University. These biweekly projects outline my skills in Python and C++ to handle complex data and arrive at simplified assumptions based on the data.

## Homework 1 - Numerical Methods & Euler Method

In this assignment, I was tasked to model population growth that is proportional to its current size. Euler's method was employed to numerically solve the population-growth differential equation. It was then compared with the analytical solution to $\frac{dN}{dt} = rN$, providing the solution $N(t) = N_{0} e^{rt}$. I then investigated how adjusting the timesteps and percentage deviation between the analytical and numerical solutions. The population growth and bifurcation was plotted in PlottingHw1.jpynb

## Homework 2 - Projectile Motion

#### Overview

This project models projectile motion under various environmental conditions using C++ simulations for data generation and Python (pandas, matplotlib) for visualization and analysis. The goal of the assignment is to analyze how drag, wind, and precipitation affect a projectile's trajectory, maximum height, range, and impact speed across different launch angles.

#### Output data 

files containing trajectory $(x, y)$ coordinate points and impact speed arrays for each scenario.Key Scenarios & Features1. Standard Projectile MotionEvaluates trajectories at angles: 15°, 25°, 35°, and 45°. Identifies the optimal launch angle for maximum range (37°, reaching a maximum height of ~102.86 m).2. Projectile Motion with HeadwindModels aerodynamic drag under a 25 mph (11.18 m/s) headwind.Determines the adjusted optimal launch angle (33.5°).3. Projectile Motion in Heavy RainSimulates the drag/impedance effect caused by heavy rain.Evaluates impact speeds and optimal launch angle (36.5°).Dependencies & RequirementsC++ Compiler (g++ or similar with C++11 support)Python 3.xPython Libraries:pandasmatplotlibglob


## Homework 3 - Bifurcation of Logistic Map

In this assignment, I was tasked with determining the transition to chaos through period doubling of the logistic map $x_n = \mu x_n(1-x_n)$. The goal was to determine the critical value of $\mu$ where the system begins to behave chaotically. Ie used two complementary methods in Python to evaluate the Lyapunov exponent for different values of $\mu$:Analytical Derivative Method (Direct Formula):The theoretical Lyapunov exponent is calculated using the average sum of the natural log of the absolute derivative of the map at each step: 

$\lambda = \lim_{N \to \infty} \frac{1}{N} \sum_{i=0}^{N-1} \ln \vert{}\mu(1 - 2x_i)\vert{}$ 

$\mu = 2.1$: $\lambda \approx -2.3026$ (Rapidly settles to a single stable fixed point)

$\mu = 3.3$: $\lambda \approx -0.6189$ (Settles into a stable 2-cycle)

$\mu = 3.6$: $\lambda \approx 0.1821$ (Positive value confirms chaotic behavior)

Numerical Perturbation & Slope Fitting Method: We tracked two trajectories starting at $x_0$ and $x_0 + \epsilon$ (where $\epsilon = 10^{-5}$).We logged the log-distance $\ln\vert{}\Delta x_n\vert{}$ over several iterations.By fitting a line to the initial growth phase of $\ln\vert{}\Delta x_n\vert{}$ versus the iteration number $n$, the slope of that line provides an empirical estimate of $\lambda$.
