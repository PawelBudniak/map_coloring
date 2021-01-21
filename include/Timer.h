#ifndef KORKI_TIMER_H
#define KORKI_TIMER_H

#include <chrono>
#include <utility>
#include <iostream>

namespace timer {

    using seconds = std::chrono::seconds;
    using milliseconds = std::chrono::milliseconds;
    using microseconds = std::chrono::microseconds;
    using nanoseconds = std::chrono::nanoseconds;

    template<typename Resolution>
    class Timer {
        using clock = std::chrono::steady_clock;

    public:
        [[nodiscard]] double getRatio() const { return double(Resolution::period::num) / Resolution::period::den; }

        //  Time regular functions, lambdas and functors
        template<typename Callable, typename... Args>
        auto time(Callable f, Args &&... args) -> std::tuple<double, int, std::vector<int>>
        {
            auto start = clock::now();

            auto [colorsUsed, colors] = f(std::forward<Args>(args)...);

            Resolution duration = std::chrono::duration_cast<Resolution>(clock::now() - start);

            return {duration.count(), colorsUsed, colors};
        }

//        //  Time function templates
//        template<typename... Args>
//        double time(void (*f)(Args...), Args &&... args) {
//            auto start = clock::now();
//
//            f(std::forward<Args>(args)...);
//
//            Resolution duration = std::chrono::duration_cast<Resolution>(clock::now() - start);
//
//            return duration.count();
//        }
//
//        //  Time function templates with return type
//        template<typename R, typename... Args>
//        double time(R (*f)(Args...), Args &&... args) {
//            auto start = clock::now();
//
//            f(std::forward<Args>(args)...);
//
//            Resolution duration = std::chrono::duration_cast<Resolution>(clock::now() - start);
//
//            return duration.count();
//        }

//        //  Time member functions
//        template<typename T, typename... Args>
//        double time(void (T::*f)(Args...), Args &&... args) {
//            auto start = clock::now();
//
//            T temp;
//            (temp.*f)(std::forward<Args>(args)...);
//
//            Resolution duration = std::chrono::duration_cast<Resolution>(clock::now() - start);
//
//            return duration.count();
//        }
//
//        //  Time member function with return type
//        template<typename T, typename R, typename... Args>
//        double time(R (T::*f)(Args...), Args &&... args) {
//            auto start = clock::now();
//
//            T temp;
//            (temp.*f)(std::forward<Args>(args)...);
//
//            Resolution duration = std::chrono::duration_cast<Resolution>(clock::now() - start);
//
//            return duration.count();
//        }
//
//        //  Time member functions without default constructor
//        template<typename T, typename... Args>
//        double time(void (T::*f)(Args...), T obj, Args &&... args) {
//            auto start = clock::now();
//
//            (obj.*f)(std::forward<Args>(args)...);
//
//            Resolution duration = std::chrono::duration_cast<Resolution>(clock::now() - start);
//
//            return duration.count();
//        }
    };

}

#endif //KORKI_TIMER_H
