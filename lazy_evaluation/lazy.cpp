#include <iostream>
#include <chrono>
#include <future>

unsigned long long factorial(const std::size_t n) 
    { return (n <= 1) ? 1 : n * factorial(n - 1); }

int main() 
{
    std::size_t number{};

    std::cout << "N: "; 
    std::cin >> number;

    std::shared_future<unsigned long long> future_result{ std::async(std::launch::deferred, factorial, number) };

    auto start{ std::chrono::high_resolution_clock::now() };
    unsigned long long result1{ future_result.get() };
    auto end{ std::chrono::high_resolution_clock::now() };
    std::chrono::duration<double> elapsed{ end - start };
    std::cout << "First time getting future: " 
              << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() << " microseconds\n";

    start = std::chrono::high_resolution_clock::now();
    unsigned long long result2{ future_result.get() };
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "Second time getting future: " 
              << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count() << " microseconds\n";

    return 0;
}
