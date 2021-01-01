#ifndef CLEARABLE_QUEUE_H
#define CLEARABLE_QUEUE_H
#include <queue>

/**
 * An extension to the standard queue that requires the container
 * to also be clearable.
 *
 * This allows for the queue to be cleared in a more efficient manner.
 * @tparam T type of value to be held in the container
 * @tparam Container the internal container type to be used
 */
template <class T, class Container = std::deque<T>>
class clearable_queue: public std::queue<T, Container> {
public:
    void clear() noexcept {
        std::queue<T, Container>::c.clear();
    }
};
#endif
