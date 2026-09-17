# computational-physics
This repository showcases the work I completed during my time in undergrad at Texas A\&M University. These biweekly projects outline my skills in Python and C++ to handle complex data and arrive at simplified assumptions based on the data.

## Homework 1 - Numerical Methods & Euler Method

This repository contains a C++ and Python-based computational study modeling population dynamics over time using Euler’s method for numerical integration alongside analytical solutions. The assignment explores the rate of population growth, defined by the differential equation $\frac{dN}{dt} = R(t) = rN$, as well as a modified system incorporating a time-dependent, density-dependent mortality factor represented by the term $-\beta N^2$. Through C++ simulation scripts like Hw1.cpp and Hw1D.cpp, numerical models are generated and saved to CSV datasets—including population_growth_real1.0.csv and population_growth_beta_euler.csv—to analyze how numerical approximations diverge from exact mathematical models over time.The Python notebook PlottingHw1.ipynb is used to load, visualize, and evaluate these results by plotting $N(t)$ alongside the growth rate $R(t)$, as well as calculating numerical error metrics ($\Delta N$ and $\Delta R$) across time steps. Additionally, the project analyzes a scenario where new, $N^2$-dependent death dynamics are introduced at $t = 100$, demonstrating how the population transitions from exponential growth toward a stable steady-state equilibrium value, $N^* = \frac{r}{\beta} = 20\text{ million}$.
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
