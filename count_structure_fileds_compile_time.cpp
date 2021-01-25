#include <utility>
#include <string>
#include <iostream>

namespace detail {
    template<std::size_t>
    struct Caster {
        template<typename U>
        operator U() const noexcept;
    };

    template<typename T, std::size_t... I>
    constexpr auto struct_size_impl(std::index_sequence<I...> args)
    {
        return args.size() - 1;
    }

    template<typename T, std::size_t Head, std::size_t... I>
    constexpr auto struct_size_impl(std::index_sequence<Head, I...> args) -> decltype(T{Caster<Head>{}, Caster<I>{}...}, std::size_t{})
    {
        return struct_size_impl<T>(std::make_index_sequence<args.size() + 1>());
    }
} // namespace detail

template<typename St>
constexpr size_t struct_size() {
    return detail::struct_size_impl<St>(std::make_index_sequence<1>{});
}

struct Foo {
    int a;
    double b;
    char c;
    const char* w;
    std::string ss;
};

int main() {
    constexpr size_t n = struct_size<Foo>();
    std::cout << "n: " << n << std::endl; // n = 5
}
