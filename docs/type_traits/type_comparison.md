## Type Comparison Concepts

Defined in header `<rbox/type_traits/type_comparison.hpp>`.

```cpp
namespace rbox {

// ---- Same-As Check ----

template <class T, class U>
concept same_as = std::is_same_v<T, U>;

template <class T, class U>
concept same_as_without_cv = std::is_same_v<std::remove_cv_t<T>, std::remove_cv_t<U>>;

template <class T, class U>
concept same_as_without_cvref = std::is_same_v<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;

template <class T, class U>
concept not_same_as = !std::is_same_v<T, U>;

template <class T, class U>
concept not_same_as_without_cv = !std::is_same_v<std::remove_cv_t<T>, std::remove_cv_t<U>>;

template <class T, class U>
concept not_same_as_without_cvref = !std::is_same_v<std::remove_cvref_t<T>, std::remove_cvref_t<U>>;

template <class T, class... Args>
concept same_as_one_of = (std::is_same_v<T, Args> || ...);

template <class T, class... Args>
concept same_as_none_of = !same_as_one_of<T, Args...>;

template <class T, class... Args>
concept same_as_one_of_without_cv = (std::is_same_v<std::remove_cv_t<T>, std::remove_cv_t<Args>> || ...);

template <class T, class... Args>
concept same_as_none_of_without_cv = !same_as_one_of_without_cv<T, Args...>;

template <class T, class... Args>
concept same_as_one_of_without_cvref = (std::is_same_v<std::remove_cvref_t<T>, std::remove_cvref_t<Args>> || ...);

template <class T, class... Args>
concept same_as_none_of_without_cvref = !same_as_one_of_without_cvref<T, Args...>;

// ---- Conversion Check ----

template <class T, class... Args>
concept convertible_to_one_of = (std::is_convertible_v<T, Args> || ...);

template <class T, class... Args>
concept convertible_to_none_of = !convertible_to_one_of<T, Args...>;

template <class T, class... Args>
concept convertible_to_one_of_without_cv = (std::is_convertible_v<std::remove_cv_t<T>, std::remove_cv_t<Args>> || ...);

template <class T, class... Args>
concept convertible_to_none_of_without_cv = !convertible_to_one_of_without_cv<T, Args...>;

template <class T, class... Args>
concept convertible_to_one_of_without_cvref = (std::is_convertible_v<std::remove_cvref_t<T>, std::remove_cvref_t<Args>> || ...);

template <class T, class... Args>
concept convertible_to_none_of_without_cvref = !convertible_to_one_of_without_cvref<T, Args...>;

}  // namespace rbox
```

### Same-As Concepts

These concepts provide type identity checks with various levels of cv-qualifier stripping:

| Concept                                     | Behavior                                        |
| ------------------------------------------- | ----------------------------------------------- |
| `same_as<T, U>`                             | `T` and `U` are the same type (exact match)     |
| `same_as_without_cv<T, U>`                  | Same after removing top-level cv-qualifiers     |
| `same_as_without_cvref<T, U>`               | Same after removing cv-qualifiers and reference |
| `not_same_as<T, U>`                         | Negation of `same_as`                           |
| `not_same_as_without_cv<T, U>`              | Negation of `same_as_without_cv`                |
| `not_same_as_without_cvref<T, U>`           | Negation of `same_as_without_cvref`             |
| `same_as_one_of<T, Args...>`                | `T` is the same as at least one of `Args...`    |
| `same_as_none_of<T, Args...>`               | `T` is not the same as any of `Args...`         |
| `same_as_one_of_without_cv<T, Args...>`     | Same as one of, ignoring cv                     |
| `same_as_none_of_without_cv<T, Args...>`    | Same as none of, ignoring cv                    |
| `same_as_one_of_without_cvref<T, Args...>`  | Same as one of, ignoring cvref                  |
| `same_as_none_of_without_cvref<T, Args...>` | Same as none of, ignoring cvref                 |

### Conversion Concepts

These concepts provide convertibility checks with various levels of cv-qualifier stripping:

| Concept                                            | Behavior                                                 |
| -------------------------------------------------- | -------------------------------------------------------- |
| `convertible_to_one_of<T, Args...>`                | `T` is convertible to at least one of `Args...`          |
| `convertible_to_none_of<T, Args...>`               | `T` is not convertible to any of `Args...`               |
| `convertible_to_one_of_without_cv<T, Args...>`     | Convertible to one of, ignoring cv on `T` and targets    |
| `convertible_to_none_of_without_cv<T, Args...>`    | Not convertible to any, ignoring cv                      |
| `convertible_to_one_of_without_cvref<T, Args...>`  | Convertible to one of, ignoring cvref on `T` and targets |
| `convertible_to_none_of_without_cvref<T, Args...>` | Not convertible to any, ignoring cvref                   |
