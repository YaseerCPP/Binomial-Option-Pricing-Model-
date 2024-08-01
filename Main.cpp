#include <iostream>
#include <vector>
#include <cmath>

// Function to calculate the binomial call option price
double binomialOptionPrice(
    double S, // Current stock price
    double K, // Strike price
    double T, // Time to expiration (in years)
    double r, // Risk-free interest rate
    double sigma, // Volatility of the stock
    int n, // Number of time steps
    bool isCall // True for call option, false for put option
) {
    double dt = T / n; // Time step
    double u = exp(sigma * sqrt(dt)); // Up factor
    double d = 1 / u; // Down factor
    double p = (exp(r * dt) - d) / (u - d); // Risk-neutral probability

    std::vector<double> optionPrices(n + 1);

    // Calculate option values at maturity
    for (int i = 0; i <= n; ++i) {
        double ST = S * pow(u, n - i) * pow(d, i);
        if (isCall) {
            optionPrices[i] = std::max(ST - K, 0.0);
        } else {
            optionPrices[i] = std::max(K - ST, 0.0);
        }
    }

    // Backward induction to calculate option price at the present time
    for (int j = n - 1; j >= 0; --j) {
        for (int i = 0; i <= j; ++i) {
            optionPrices[i] = (p * optionPrices[i] + (1 - p) * optionPrices[i + 1]) / exp(r * dt);
        }
    }

    return optionPrices[0];
}

int main() {
    double S = 100.0; // Current stock price
    double K = 100.0; // Strike price
    double T = 1.0; // Time to expiration in years
    double r = 0.05; // Risk-free interest rate
    double sigma = 0.2; // Volatility
    int n = 100; // Number of time steps

    double call_price = binomialOptionPrice(S, K, T, r, sigma, n, true);
    double put_price = binomialOptionPrice(S, K, T, r, sigma, n, false);

    std::cout << "Call Price: " << call_price << std::endl;
    std::cout << "Put Price:  " << put_price << std::endl;

    return 0;
}
