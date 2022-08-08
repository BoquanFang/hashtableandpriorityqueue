#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP

#include "hash_table.hpp"
int power(int x, int y);
/**
 * Implementation of a priority queue that supports the
 * extended API discussed during lecture. This priority
 * queue maps unsigned values to instances of ValueType.
 * You are required to use a hash table to efficiently
 * support the operations of the extended API.
 *
 * As with the hash table, any use of the term "element" here
 * refers to a key-value pair.
 *
 * As with the hash table (see the block comment right before the
 * definition of class HahsTable), any mention of "constant time" in
 * double quotes means "amortized constant time on average",
 * i.e. you should use hashing / the required hash table
 * to speed things up and not iterate through the entire underlying
 * binary heap array unnecessarily.
 *
 * The priority queue's underlying implementation is
 * required to be a binary min heap and the hash table
 * that you implement in the first part of this assignment.
 */
template <typename ValueType>
class PriorityQueue
{
public:
    /**
     * Creates a priority queue that can have at most @maxSize elements.
     *
     * Throws std::runtime_error if @maxSize is 0.
     */
    explicit PriorityQueue(unsigned maxSize) 
    : heap_table {next_prime(2*maxSize)}
     {
        if (maxSize == 0) {
            throw std::runtime_error("Run time error! Table size is 0!");
        }
        tree_table = new Pair<ValueType> [maxSize + 1];
        heap_elements = 0;
        heap_size = maxSize + 1;
    
    };

    ~PriorityQueue() {
        delete[] tree_table;
    };

    /**
     * Makes the underlying implementation details (including the max size) look
     * exactly the same as that of @rhs.
     */
    PriorityQueue(const PriorityQueue& rhs);
    PriorityQueue& operator=(const PriorityQueue& rhs);

    /**
     * Takes the underlying implementation details of @rhs
     * and gives them to "this" object.
     * After this, @rhs should be in a "moved from" state.
     */
    PriorityQueue(PriorityQueue&& rhs) noexcept;
    PriorityQueue& operator=(PriorityQueue&& rhs) noexcept;

    /**
     * Both of these must run in constant time.
     */
    unsigned numElements() const;
    unsigned maxSize() const;

    /**
     * Print the underlying heap level-by-level.
     * See prog_hw4.pdf for how exactly this should look.
     */
    friend std::ostream& operator<<(
        std::ostream& os,
        const PriorityQueue<ValueType>& pq)
    {
        // TODO: Implement this method.
        if (pq.heap_elements == 0) {
            return os;
        }
        int line = 1;
        int num_print = power(2, line - 1);
        int num_already = 0;
        bool s = false;
        for (unsigned i = 1; i <= pq.heap_elements; i ++) {
            if (pq.tree_table[i].state == 1) {
                if (num_already + 1 == num_print) {
                    os << "(" << pq.tree_table[i].key << "," << pq.tree_table[i].value << ")" <<  std::endl;
                    num_already = 0;
                    line ++;
                    num_print=power(2,line - 1);
                    s = true;
                } else {
                    os << "(" << pq.tree_table[i].key << "," << pq.tree_table[i].value << ")" << " " ;
                    num_already ++;
                    s= false;
                }
            }
        }
        if (!s) os << std::endl;
        return os;
    }

    void swap(unsigned child, unsigned parent);
    void proclate_up();
    void proclate_up_try(unsigned current_index);

    /**
     * Inserts a key-value pair mapping @key to @value into
     * the priority queue.
     *
     * Returns true if success.
     * In this case, must run in logarithmic time.
     *
     * Returns false if @key is already in the priority queue
     * or if max size would be exceeded.
     * (In either of these cases, the insertion is not performed.)
     * In this case, must run in "constant time".
     */
    bool insert(unsigned key, const ValueType& value);

    /**
     * Returns key of the smallest element in the priority queue
     * or null pointer if empty.
     *
     * This function must run in constant time.
     *
     * The pointer may be invalidated if the priority queue is modified.
     */
    const unsigned* getMinKey() const;

    /**
     * Returns value of the smallest element in the priority queue
     * or null pointer if empty.
     *
     * This function must run in constant time.
     *
     * The pointer may be invalidated if the priority queue is modified.
     */
    const ValueType* getMinValue() const;

    /**
     * Removes the root of the priority queue.
     *
     * This function must run in logarithmic time.
     *
     * Returns true if success.
     * Returns false if priority queue is empty, i.e. nothing to delete.
     */
    bool deleteMin();

    /**
     * Returns address of the value that @key is mapped to in the priority queue.
     *
     * These functions must run in "constant time".
     *
     * Returns null pointer if @key is not in the table.
     */
    ValueType* get(unsigned key);
    const ValueType* get(unsigned key) const;

    bool proclate_down(unsigned current_index);

    /**
     * Subtracts/adds @change from/to the key of
     * the element that has key @key.
     *
     * These functions must run in "constant time" + logarithmic time.
     * This means you must use the required hash table to find the
     * location of @key in the underlying binary heap array.
     *
     * Returns true if success.
     * Returns false if any of the following:
     * - @change is 0.
     * - @key not found.
     * - If the change would lead to a duplicate key.
     *
     * The function does not do anything about  overflow/underflow.
     * For example, an operation like decreaseKey(2, 10)
     * has an undefined effect.
     */
    bool decreaseKey(unsigned key, unsigned change);
    bool increaseKey(unsigned key, unsigned change);

    /**
     * Removes element that has key @key.
     *
     * These functions must run in "constant time" + logarithmic time.
     * This means you must use the required hash table to find the
     * location of @key in the underlying binary heap array.
     *
     * Returns true if success.
     * Returns false if @key not found.
     */
    bool remove(unsigned key);


private:
    // TODO: Your members here.
    HashTable<unsigned> heap_table;
    Pair<ValueType>* tree_table;
    unsigned heap_size; //max size
    unsigned heap_elements; // num of elements
};



#include "priority_queue.inl"
#endif  // PRIORITY_QUEUE_HPP
