#ifndef CLEARABLE_STACK_H
#define CLEARABLE_STACK_H
#include <stack>

/**
 * An extension to the standard stack that requires the container
 * to also be clearable.
 *
 * This allows for the stack to be cleared in a more efficient manner.
 * @tparam T type of value to be held in the container
 * @tparam Container the internal container type to be used
 */
template <class T, class Container = std::deque<T>>
class clearable_stack: public std::stack<T, Container> {
  public:
    void clear() noexcept {
        std::stack<T, Container>::c.clear();
    }
};
#endif
