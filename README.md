# Computational Physics & Quantitative Modeling

This repository contains a collection of computational modeling and data analysis projects built during my undergraduate studies at Texas A&M University. The projects demonstrate the application of **C++ and Python** (`pandas`, `numpy`, `scipy`, `matplotlib`) to simulate complex systems, process large datasets, perform numerical integration, and execute stochastic modeling techniques relevant to quantitative risk analysis and data science.

---

## Table of Contents
* [Quantitative & Actuarial Relevance](#-quantitative--actuarial-relevance)
* [Tech Stack & Tools](#-tech-stack--tools)
* [Homework 1 - Numerical Methods & Population Dynamics](#homework-1---numerical-methods--population-dynamics)
* [Homework 2 - Projectile Motion & Multivariate Simulation](#homework-2---projectile-motion--multivariate-simulation)
* [Homework 3 - Bifurcation & Non-Linear System Analysis](#homework-3---bifurcation--non-linear-system-analysis)
* [Homework 4 - Orbital Mechanics & Relativistic Precession](#homework-4---orbital-mechanics--relativistic-precession)
* [Homework 5 - Vector Field Analysis & Numerical Integration](#homework-5---vector-field-analysis--numerical-integration)
* [Homework 6 - Wave Motion & Spectral Analysis](#homework-6---wave-motion--spectral-analysis)
* [Homework 7 - Particle Diffusion & Monte Carlo Simulation](#homework-7---particle-diffusion--monte-carlo-simulation)

---

## 💡 Quantitative & Actuarial Relevance

While these projects originate from coursework in computational physics, the core mathematical and computational methods directly map to quantitative risk modeling, financial mathematics, and data analytics:

* **Stochastic Processes & Monte Carlo Simulations:** Modeling random walks and particle diffusion mirrors stochastic loss estimation, claims frequency modeling, and ruin theory.
* **Deterministic Modeling & Differential Equations:** Applying Euler's method and numerical integration reflects deterministic decrement models, population projections, and compound interest dynamics.
* **Data Pipelines & Statistical Analysis:** Generating large-scale simulation outputs in C++ and performing data wrangling, curve fitting, and visualization using Python (`pandas`, `numpy`, `scipy`, `matplotlib`).
* **Sensitivity & Parameter Analysis:** Evaluating how small perturbations in inputs affect long-term system stability and equilibrium states.

---

## 🛠️ Tech Stack & Tools

* **Languages:** C++ (C++11/17), Python 3
* **Python Libraries:** Pandas, NumPy, SciPy, Matplotlib, Seaborn
* **Quantitative Methods:** Monte Carlo Simulation, Ordinary Differential Equations (ODEs), Euler's Method, Linear Regression & Curve Fitting, Discrete Fourier Transform (DFT), Trapezoidal Rule Integration

---

## Homework 1 - Numerical Methods & Population Dynamics

This module implements a C++ and Python pipeline to model deterministic population dynamics using Euler's method for numerical integration alongside exact analytical solutions. The study examines first-order differential equations governed by:

$$\frac{dN}{dt} = R(t) = rN$$

and evaluates a density-dependent mortality model featuring a non-linear decay term:

$$- \beta N^2$$

**Key Analysis & Workflow:**
* **C++ Pipeline:** C++ scripts (`Hw1.cpp`, `Hw1D.cpp`) execute the numerical solvers and export structured simulation outputs to CSV files (`population_growth_real1.0.csv`, `population_growth_beta_euler.csv`).
* **Data Processing:** The Jupyter Notebook (`PlottingHw1.ipynb`) ingests the datasets to calculate numerical error metrics ($\Delta N$ and $\Delta R$) and evaluate convergence against analytical baselines over varying step sizes.
* **Equilibrium Analysis:** Modeled a system transition at $t = 100$ where new death dynamics drive the population from exponential growth into a stable steady-state equilibrium at $N^* = \frac{r}{\beta} = 20\text{ million}$.

<img width="869" height="540" alt="image" src="https://github.com/user-attachments/assets/2fe5aca9-59c3-4667-8e86-b25eb13ad795" />


---

## Homework 2 - Projectile Motion & Multivariate Simulation

This project models two-dimensional kinematic trajectories subject to quadratic drag, gravity, and environmental variables (headwinds and heavy precipitation). The system is governed by the vector differential equation:

$$F = m \frac{d^2 r}{d t^2} = -mg\hat{j} - C_d v \mathbf{v} + F_{\text{env}}$$

**Key Analysis & Workflow:**
* **Multivariate Simulation:** C++ programs (`Hw2.cpp`, `Hw2Wind.cpp`, `Hw2Rain.cpp`) compute trajectory coordinates and velocity vectors across launch angles ($15^\circ$ to $45^\circ$) under various environmental constraints.
* **Data Analysis & Optimization:** Python (`pandas`, `matplotlib`) was used in `Graphing_Hw_2.ipynb` to clean and analyze the simulation outputs, identifying optimal parameters for maximum range and terminal impact velocity.
* **Key Results:** Baseline aerodynamic drag reduced the optimal launch angle from $45^\circ$ to $\theta \approx 37^\circ$ (peak height $\approx 102.86\text{ m}$). Factoring in a $25\text{ mph}$ ($11.18\text{ m/s}$) headwind and heavy rainfall further shifted the optimal launch angle downward to $33.5^\circ$ and $36.5^\circ$, respectively.

<img width="948" height="643" alt="image" src="https://github.com/user-attachments/assets/d36b9e13-6baa-4139-bbb9-ba5a4000bac8" />
<img width="883" height="524" alt="image" src="https://github.com/user-attachments/assets/96b4ba51-aa4c-4479-858a-a6db112fbd61" />



---

## Homework 3 - Bifurcation & Non-Linear System Analysis

This project investigates non-linear dynamics, chaos theory, and system stability through the numerical analysis of the logistic map:

$$x_{n+1} = r x_n (1 - x_n)$$

**Key Analysis & Workflow:**
* **Long-Term Iteration:** C++ scripts (`Hw3.cpp`, `Hw3B.cpp`, `Hw3D.cpp`) execute iterative simulations to model system behavior across varying values of the control parameter $r$, logging steady-state points and orbital trajectories into structured text files.
* **Visualization & Sensitivity Analysis:** Python (`Graphing3.ipynb`) processes the trajectory data to generate time-series plots and a full bifurcation diagram ($x_\infty$ vs. $r$), illustrating period-doubling cascades, Feigenbaum pitchfork bifurcations, and chaotic regimes.
* **Model Insights:** Demonstrates extreme sensitivity to initial conditions and parameter variations, a core concept in risk modeling and non-linear forecasting.

<img width="862" height="528" alt="image" src="https://github.com/user-attachments/assets/e13a6b98-8dfb-438a-8f0f-cda2ed585b36" />


---

## Homework 4 - Orbital Mechanics & Relativistic Precession

This project models orbital mechanics and quantifies the relativistic precession of Mercury's perihelion. The modified gravitational force is defined by:

$$F_g = \frac{G M m}{r^2} \left(1 + \frac{\alpha}{r^2}\right)$$

where the $\frac{\alpha}{r^2}$ term introduces General Relativity corrections that drive orbital precession.

**Key Analysis & Workflow:**
* **Numerical Simulation:** C++ scripts (`Hw1A.cpp` through `Hw1D.cpp`) solve the differential equations of motion, exporting time-series position and velocity vectors $(x, y, v_x, v_y)$ across varying $\alpha$ parameter values.
* **Regression & Extrapolation:** The Jupyter Notebook (`GraphingHw4.ipynb`) utilizes `numpy`, `matplotlib`, and `scipy` to track perihelion angle changes ($\theta_p$) over time and applies linear regression to determine the precession rate $\dot{\theta}_p$.
* **Quantitative Validation:** Extrapolating the regression models down to the physical value of $\alpha = 1.1 \times 10^{-8}\text{ AU}^2$ yielded a precession rate of $43.99''/\text{century}$, matching the accepted observational rate of $\sim 43''/\text{century}$.

<img width="1067" height="351" alt="image" src="https://github.com/user-attachments/assets/58f8f1c6-a537-451d-a947-45d0bab64ab0" />


---

## Homework 5 - Vector Field Analysis & Numerical Integration

This project evaluates 3D vector fields and magnetic field distributions produced by a circular current loop, utilizing the Biot–Savart law:

$$\mathbf{B}(\mathbf{r}) = \frac{\mu_0 I}{4\pi} \int \frac{d\mathbf{\ell} \times (\mathbf{r} - \mathbf{r}')}{|\mathbf{r} - \mathbf{r}'|^3}$$

**Key Analysis & Workflow:**
* **Numerical Integration:** C++ programs (`Hw5aSoln.cpp` through `Hw5dSoln.cpp`) apply the trapezoidal rule to evaluate the line integral across a 3D spatial grid, exporting component values $(B_x, B_y, B_z)$ to structured CSV files.
* **Error Analysis & Vector Mapping:** Python (`Hw5Graphs.ipynb`) evaluates the numerical accuracy against the analytical axial solution:
  $$B_z(z) = \frac{\mu_0 I R^2}{2(R^2 + z^2)^{3/2}}$$
* **Data Visualization:** Generated 1D profile plots and 2D vector field quiver diagrams in the $x\text{-}z$ plane to map field intensity and direction.

<img width="737" height="539" alt="image" src="https://github.com/user-attachments/assets/ed8c2454-8dd2-40e4-9789-fdc7eb87afb3" />


---

## Homework 6 - Wave Motion & Spectral Analysis

This project simulates 1D wave propagation and performs frequency domain analysis on a vibrating string under tension, governed by the wave equation:

$$\frac{\partial^2 y}{\partial t^2} = c^2 \frac{\partial^2 y}{\partial x^2} \quad \text{where} \quad c = \sqrt{\frac{F_T}{\mu}}$$

**Key Analysis & Workflow:**
* **Spatiotemporal Simulation:** C++ scripts (`hw6.cpp`, `hw6_3.cpp`) model wave packet propagation and standing wave states, outputting time-series displacement data to CSV files.
* **Signal Processing:** Python notebooks (`HomeworkGraphingC.ipynb`, `Graphing6C.ipynb`) apply the Discrete Fourier Transform (DFT) via `numpy.fft` to extract the frequency spectrum from time-domain signals.
* **Spectral Analysis:** Verified the fundamental harmonic $f_1 = \frac{c}{2L}$ and higher-order harmonics $f_n = n \cdot f_1$, evaluating energy distribution across different boundary conditions.

<img width="802" height="491" alt="image" src="https://github.com/user-attachments/assets/4855f1a2-f1f8-4a15-b25b-f94a70be5c89" />


---

## Homework 7 - Particle Diffusion & Monte Carlo Simulation

This project models 2D particle diffusion and the thermodynamic progression toward equilibrium using a stochastic Monte Carlo random walk simulation on $N \times N$ spatial grids ($100 \times 100$ and $200 \times 200$).

**Key Analysis & Workflow:**
* **Stochastic Simulation:** C++ (`Hw7DSoln.cpp`) tracks individual particle trajectories over time, recording spatial grid densities and entropy time-series data to CSV files.
* **Exponential Decay Fitting:** Python (`Hw7Graph.ipynb`) measures the system's approach to maximum entropy $S_\infty$ by analyzing the deviation parameter $R(t) = |S(t) - S_\infty|$.
* **Parameter Estimation:** By fitting a linear regression model to $\ln R(t) = -\frac{t}{\tau} + C$, the analysis extracted the characteristic relaxation time $\tau$, demonstrating how grid scale governs equilibration rates in stochastic systems.

<img width="1065" height="695" alt="image" src="https://github.com/user-attachments/assets/5d324629-0488-4cba-9e15-1c4ce27c72a2" />

