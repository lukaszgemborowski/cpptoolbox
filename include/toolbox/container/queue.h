#ifndef _TOOLBOX_CONTAINER_QUEUE_H_
#define _TOOLBOX_CONTAINER_QUEUE_H_

#include <toolbox/std/utility.h>

namespace toolbox
{

// queue_node is not inner class of queue to allow
// specialization of allocator passed to the queue
template<class T>
struct queue_node {
    queue_node(const T& value)
        : value (value)
    {}

    T value;
    queue_node *previous;
};

template<class T, class A>
struct queue
{
private:
    template<class El, class... Tail>
    void push(El &&el, Tail&&... tail)
    {
        push_back(el);
        push(std::forward<Tail>(tail)...);
    }

    // end recursion
    void push() {}

public:
    template<class... Els>
    explicit queue(Els&&... els)
    {
        push(els...);
    }

    queue() = default;

    void push_back(const T &element)
    {
        auto &n = alloc_.allocate(element);

        if (back_) {
            back_->previous = &n;
        }
        if (!front_) {
            front_ = &n;
        }

        back_ = &n;
        back_->previous = front_;
    }

    T& front()
    {
        return front_->value;
    }

    void pop_front()
    {
        auto current_front = front_;

        if (front_ == back_) {
            front_ = nullptr;
            back_ = nullptr;
        } else {
            front_ = front_->previous;
        }

        alloc_.deallocate(*current_front);
    }

    void remove(const T& value)
    {
        queue_node<T> *c = back_;
        for (std::size_t i = 0; i < size(); ++i, c = c->previous) {
            if (c->previous->value != value)
                continue;

            auto to_delete = c->previous;
            c->previous = to_delete->previous;

            if (to_delete == front_)
                front_ = c;
            if (to_delete == back_)
                back_ = to_delete->previous;

            if (size() == 0) {
                front_ = nullptr;
                back_ = nullptr;
            }

            alloc_.deallocate(*to_delete);
            return;
        }
    }

    /** \brief Move element from the front to the back
     *
     * This function is more efficient than sequence of:
     *
     * auto n = front();
     * pop_front();
     * push_back(n);
     *
     * because it just moves pointers without actually
     * reallocating elements in the queue
    */
    void rotate()
    {
        // as previous pointer makes a cyclic reference (ie. previous of back is front)
        // we can just reassign front and back pointers
        back_ = front_;
        front_ = front_->previous;
    }

    constexpr auto size() const
    {
        return alloc_.allocated_count();
    }

private:
    A alloc_;
    queue_node<T> *front_ = nullptr;
    queue_node<T> *back_ = nullptr;
};

} // namespace toolbox

#endif