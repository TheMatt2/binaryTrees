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
    void clear() {
        std::queue<T, Container>::c.clear();
    }
};