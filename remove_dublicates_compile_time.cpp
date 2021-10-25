#include <iostream>
#include <utility>

template <int... I>
struct MyVector;

// Option 1
template <typename Input, typename Output = MyVector<>>
struct MakeUnique {
    using type = Output;
};

template <int First, int... Tail, int... Result>
struct MakeUnique<MyVector<First, First, Tail...>, MyVector<Result...>> {
    using type = typename MakeUnique<MyVector<First, Tail...>,
                                     MyVector<Result...>>::type;
};

template <int First, int... Tail, int... Result>
struct MakeUnique<MyVector<First, Tail...>, MyVector<Result...>> {
    using type = typename MakeUnique<MyVector<Tail...>,
                                     MyVector<Result..., First>>::type;
};

// Option 2
template <int Elem, typename Vec>
struct PushFront;

template <int Elem, int... VecElems>
struct PushFront<Elem, MyVector<VecElems...>> {
    using type = MyVector<Elem, VecElems...>;
};

template <typename Vec>
struct MakeUnique2 {
    using type = Vec;
};

template <int First, int Second, int... Tail>
struct MakeUnique2<MyVector<First, Second, Tail...>>
    : public std::conditional_t<
          First == Second, MakeUnique2<MyVector<Second, Tail...>>,
          PushFront<First,
                    typename MakeUnique2<MyVector<Second, Tail...>>::type>> {};

int main() {
    // Option 1
    using ans_1_1 = typename MakeUnique<MyVector<1, 2, 3, 4>>::type;
    static_assert(std::is_same_v<ans_1_1, MyVector<1, 2, 3, 4>>);

    using ans_1_2 = typename MakeUnique<MyVector<1, 1, 1>>::type;
    static_assert(std::is_same_v<ans_1_2, MyVector<1>>);

    using ans_1_3 = typename MakeUnique<MyVector<1, 2, 3, 4, 4, 5, 6>>::type;
    static_assert(std::is_same_v<ans_1_3, MyVector<1, 2, 3, 4, 5, 6>>);

    using ans_1_4 = typename MakeUnique<MyVector<1>>::type;
    static_assert(std::is_same_v<ans_1_4, MyVector<1>>);

    using ans_1_5 = typename MakeUnique<MyVector<>>::type;
    static_assert(std::is_same_v<ans_1_5, MyVector<>>);

    // Option 2
    using ans_2_1 = typename MakeUnique2<MyVector<1, 2, 3, 4>>::type;
    static_assert(std::is_same_v<ans_2_1, MyVector<1, 2, 3, 4>>);

    using ans_2_2 = typename MakeUnique2<MyVector<1, 1, 1>>::type;
    static_assert(std::is_same_v<ans_2_2, MyVector<1>>);

    using ans_2_3 = typename MakeUnique2<MyVector<1, 2, 3, 4, 4, 5, 6>>::type;
    static_assert(std::is_same_v<ans_2_3, MyVector<1, 2, 3, 4, 5, 6>>);

    using ans_2_4 = typename MakeUnique2<MyVector<1>>::type;
    static_assert(std::is_same_v<ans_2_4, MyVector<1>>);

    using ans_2_5 = typename MakeUnique2<MyVector<>>::type;
    static_assert(std::is_same_v<ans_2_5, MyVector<>>);
}