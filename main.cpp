#include <functional>
#include <vector>
#include <iostream>
#include <limits>
#include <cmath>
#include <iomanip>

template <typename T>
using compare_t = std::function<bool (T, T, T)>;

template <typename T>
T calculate(T start, T decrement, std::size_t count)
{
    for (std::size_t i = 0; i < count; ++i)
    {
        start -= decrement;
    }

    return start;
}


template <typename T>
void testEquality(std::vector<compare_t<T>> compareFunctions, T tolerance)
{
    std::size_t total = 10000;
    std::vector<std::size_t> counts(compareFunctions.size(), 0);

    std::cout << "| Value1 | Value2 | abs(Value1 - Value2) |" ;
    for (std::size_t compareIndexFunction = 0; compareIndexFunction < compareFunctions.size(); compareIndexFunction++)
    {
        std::cout << " comparison function: " << compareIndexFunction << " |";
    }
    std::cout << std::endl;

    std::cout << "|:---:|:---:|:---:|" ;
    for (std::size_t compareIndexFunction = 0; compareIndexFunction < compareFunctions.size(); compareIndexFunction++)
    {
        std::cout << ":---:|";
    }
    std::cout << std::endl;

    for (std::size_t i = 0; i < total; ++i)
    {
        T expected = (i / 10.0);
        // auto actual = calculate(0.0, 0.01, i);
        T actual = calculate(9.0 + expected, 0.2, 45);
        std::cout << "| " << std::fixed << std::setprecision(20) << expected << " | " << actual << " | " << std::fabs(actual-expected) << " |";

        for (std::size_t compareIndexFunction = 0; compareIndexFunction < compareFunctions.size(); compareIndexFunction++)
        {
            bool areEqual = compareFunctions[compareIndexFunction](actual, expected, tolerance);
            counts[compareIndexFunction] = areEqual ? counts[compareIndexFunction] +1 : counts[compareIndexFunction];
            std::cout << areEqual << " |";
        }

        std::cout << std::endl;
    }

    std::cout << "Overview:" << std::endl;
    std::cout << "| Comparison Function | correct | total |" << std::endl;
    std::cout << "|:---:|:--:|:---:|" << std::endl;

    for (std::size_t compareIndexFunction = 0; compareIndexFunction < compareFunctions.size(); compareIndexFunction++)
    {
        std::cout <<  "| " << compareIndexFunction << " | " << counts[compareIndexFunction] << " | " << total << " |" << std::endl;
    }

}

template <typename T>
bool comparison1(T f1, T f2, T tolerance)
{
    return f1 == f2;
}

template <typename T>
bool comparison2(T f1, T f2, T tolerance)
{
    return std::abs(f1 - f2) <= tolerance;
}

template <typename T>
bool comparison3(T f1, T f2, T tolerance)
{
    return std::abs(f1 - f2) <= tolerance * std::max(std::abs(f1), std::abs(f2));
}

template <typename T>
bool comparison4(T f1, T f2, T tolerance)
{
    std::size_t ulp = 1;

    return std::fabs(f1 - f2) <= tolerance * std::fabs(f1 + f2) * ulp
        || std::fabs(f1 - f2) < std::numeric_limits<T>::min();
}

int main()
{

    constexpr double tolerance = 1e-10;

    testEquality<double>({comparison1<double>, comparison2<double>, comparison3<double>, comparison4<double>},
                        tolerance);

    std::cout << "tolerance: "  << std::fixed << std::setprecision(20) << tolerance << std::endl;


    return EXIT_SUCCESS;
}
