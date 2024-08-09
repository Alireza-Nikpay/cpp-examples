#include <iostream>
#include <future>
#include <map>

unsigned long long factorial(const std::size_t n) 
{ 
    if (n == 0 || n == 1) return 1;

    static std::map<int, unsigned long long> factorials;
    if (factorials.find(n) != factorials.cend()) 
    { 
      std::cout << "Founded, no calculation for: " << n << '\n';
      return factorials[n];
    }

    std::cout << "Not founded, caculation for: " << n << '\n';
    factorials[n] = n * factorial(n - 1);
    return factorials[n];
}
int main() 
{
    std::cout << "First time\n";
    unsigned long long result1{ factorial(10) };

    std::cout << "\n--------------------------------\n";
    
    std::cout << "Second time\n";
    unsigned long long result2{ factorial(5) };

    return 0;
}
