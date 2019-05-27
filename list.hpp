#ifndef __LIST_HPP__
#define __LIST_HPP__


#include <type_traits>
#include <tuple>
#include <cstddef>


namespace atsuko {


template <auto... xs>
struct List {};


namespace detail {


// index :: List a -> Int -> a
template <typename T, size_t i>
struct index;

template <auto x, auto... xs, size_t i>
struct index<List<x, xs...>, i> {
    static constexpr auto result = index<List<xs...>, i - 1>::result;
};

template <auto x, auto... xs>
struct index<List<x, xs...>, 0> {
    static constexpr auto result = x;
};


// cons :: a -> List a -> List a
template <auto x, typename T>
struct cons;

template <auto x, auto... ys>
struct cons<x, List<ys...>> {
    using result = List<x, ys...>;
};


// append :: List a -> List a -> List a
template <typename T, typename U>
struct append;

template <auto... xs, auto... ys>
struct append<List<xs...>, List<ys...>> {
    using result = List<xs..., ys...>;
};


// head :: List a -> a
template <typename T>
struct head;

template <auto x, auto... xs>
struct head<List<x, xs...>> {
    static constexpr auto result = x;
};


// last :: List a -> a
template <typename T>
struct last;

template <auto x, auto... xs>
struct last<List<x, xs...>> {
    static constexpr auto result = last<List<xs...>>::result;
};

template <auto x>
struct last<List<x>> {
    static constexpr auto result = x;
};


// init :: List a -> List a
template <typename T>
struct init;

template <auto x, auto... xs>
struct init<List<x, xs...>> {
    using result = typename cons<x, typename init<List<xs...>>::result>::result;
};

template <auto x>
struct init<List<x>> {
    using result = List<>;
};


// tail :: List a -> List a
template <typename T>
struct tail;

template <auto x, auto... xs>
struct tail<List<x, xs...>> {
    using result = List<xs...>;
};


// split :: List a -> (List a, List a)
template <typename T>
struct split;

template <auto x, auto y, auto... zs>
struct split<List<x, y, zs...>> {
    static constexpr auto tp = typename split<List<zs...>>::result{};
    using xs = std::decay_t<decltype(std::get<0>(tp))>;
    using ys = std::decay_t<decltype(std::get<1>(tp))>;
    using result = std::tuple<typename cons<x, xs>::result, typename cons<y, ys>::result>;
};

template <auto x>
struct split<List<x>> {
    using result = std::tuple<List<x>, List<>>;
};

template <>
struct split<List<>> {
    using result = std::tuple<List<>, List<>>;
};


// merge :: List a -> List a -> List a
template <typename T, typename U>
struct merge;

template <auto x, auto... xs, auto y, auto... ys>
struct merge<List<x, xs...>, List<y, ys...>> {
    using result = std::conditional_t<x <= y,
            typename cons<x, typename merge<List<xs...>, List<y, ys...>>::result>::result,
            typename cons<y, typename merge<List<x, xs...>, List<ys...>>::result>::result>;
};

template <typename T>
struct merge<T, List<>> {
    using result = T;
};

template <typename T>
struct merge<List<>, T> {
    using result = T;
};


// mergeSort :: List a -> List a
template <typename T>
struct mergeSort {
    static constexpr auto tp = typename split<T>::result{};
    using xs = std::decay_t<decltype(std::get<0>(tp))>;
    using ys = std::decay_t<decltype(std::get<1>(tp))>;
    using result = typename merge<typename mergeSort<xs>::result, typename mergeSort<ys>::result>::result;
};

template <auto x>
struct mergeSort<List<x>> {
    using result = List<x>;
};

template <>
struct mergeSort<List<>> {
    using result = List<>;
};


}   // namespace detail


template <typename T, size_t i>
constexpr auto index = detail::index<T, i>::result;

template <auto x, typename T>
using cons = typename detail::cons<x, T>::result;

template <typename T, typename U>
using append = typename detail::append<T, U>::result;

template <typename T>
constexpr auto head = detail::head<T>::result;

template <typename T>
constexpr auto last = detail::last<T>::result;

template <typename T>
using init = typename detail::init<T>::result;

template <typename T>
using tail = typename detail::tail<T>::result;

template <typename T>
using sort = typename detail::mergeSort<T>::result;


}   // namespace akko


#endif  // __LIST_HPP__
