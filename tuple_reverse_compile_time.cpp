#include <tuple>
#include <vector>

template <typename T, typename Result = std::tuple<>>
struct Reverse {
    using type = Result;
};

template <typename First, typename... Rest, typename... ResultArgs>
struct Reverse<std::tuple<First, Rest...>, std::tuple<ResultArgs...>> {
    using type = typename Reverse<std::tuple<Rest...>,
                                  std::tuple<First, ResultArgs...>>::type;
};

template <typename T>
using reverse_t = typename Reverse<T>::type;

int main() {
    static_assert(std::is_same_v<reverse_t<std::tuple<int, bool, double>>,
                                 std::tuple<double, bool, int>>);
    static_assert(
        std::is_same_v<
            reverse_t<
                std::tuple<int, bool, double, bool, long, long, std::string>>,
            std::tuple<std::string, long, long, bool, double, bool, int>>);
    static_assert(
        std::is_same_v<
            reverse_t<std::tuple<int, bool, double, std::vector<int>, long>>,
            std::tuple<long, std::vector<int>, double, bool, int>>);
    static_assert(std::is_same_v<reverse_t<std::tuple<int>>, std::tuple<int>>);
    static_assert(std::is_same_v<reverse_t<std::tuple<>>, std::tuple<>>);
}