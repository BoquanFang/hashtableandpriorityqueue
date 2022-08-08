/**
 * log:
 * no except need to delete??
 */

template <typename ValueType>
PriorityQueue<ValueType>::PriorityQueue(const PriorityQueue& rhs) : PriorityQueue(rhs.heap_size)  {
    this->heap_table = rhs.heap_table;
    delete[] tree_table;
    this->tree_table = new Pair<ValueType>[rhs.heap_size];
    this->heap_elements = rhs.heap_elements;
    this->heap_size = rhs.heap_size;
    for (unsigned i = 0; i <= this->heap_elements; i ++) {
        this->tree_table[i].key = rhs.tree_table[i].key;
        this->tree_table[i].value = rhs.tree_table[i].value;
        this->tree_table[i].state = rhs.tree_table[i].state;
    }
}

template <typename ValueType>
PriorityQueue<ValueType>& PriorityQueue<ValueType>::operator=(const PriorityQueue& rhs) {
    this->heap_table = rhs.heap_table;
    delete[] tree_table;
    this->tree_table = new Pair<ValueType>[rhs.heap_size];
    this->heap_elements = rhs.heap_elements;
    this->heap_size = rhs.heap_size;
    for (unsigned i = 0; i <= this->heap_elements; i ++) {
        this->tree_table[i].key = rhs.tree_table[i].key;
        this->tree_table[i].value = rhs.tree_table[i].value;
        this->tree_table[i].state = rhs.tree_table[i].state;
    }
    return *this;       
}

template <typename ValueType>
PriorityQueue<ValueType>::PriorityQueue(PriorityQueue&& rhs) noexcept: PriorityQueue(rhs.heap_size) {
    this->heap_table = std::move(rhs.heap_table);
    this->heap_elements = rhs.heap_elements;
    this->heap_size = rhs.heap_size;
    delete[] tree_table;
    this->tree_table = rhs.tree_table;
    rhs.tree_table = NULL;
    rhs.heap_size = 0;
    rhs.heap_elements = 0;
    //rhs.~PriorityQueue();

}

template <typename ValueType>
PriorityQueue<ValueType>& PriorityQueue<ValueType>::operator=(PriorityQueue&& rhs) noexcept {
    this->heap_table = std::move(rhs.heap_table);
    this->heap_elements = rhs.heap_elements;
    this->heap_size = rhs.heap_size;
    delete[] tree_table;
    this->tree_table = rhs.tree_table;
    rhs.tree_table = NULL;
    rhs.heap_size = 0;
    rhs.heap_elements = 0;
    return *this;
}

template <typename ValueType>
unsigned PriorityQueue<ValueType>::numElements() const {
    return this->heap_elements;
}

template <typename ValueType>
unsigned PriorityQueue<ValueType>::maxSize() const {
    unsigned temp = this->heap_size - 1;
    return temp;
}


int power(int x, int y) {
    // Initialize answer
    int res = 1;
    // Check till the number becomes zero
    while (y) { 
        // If y is odd, multiply x with result
        if (y % 2 == 1)
            res = (res * x);             
        // y = y/2
        y = y >> 1;         
        // Change x to x^2
        x = (x * x);
    }
    return res;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::insert(unsigned key, const ValueType& value) {
    if (this->heap_elements == this->heap_size - 1) {
        return false;
    }
    bool is_inserted = this->heap_table.insert(key, this->heap_elements + 1);
    if (is_inserted == false) {
        return is_inserted;
    }
    this->tree_table[heap_elements + 1].key = key;
    this->tree_table[heap_elements + 1].value = value;
    this->tree_table[heap_elements + 1].state = 1;
    heap_elements ++;
    this-> proclate_up_try(heap_elements);
    return true;

}

template <typename ValueType>
void PriorityQueue<ValueType>::swap(unsigned child, unsigned parent) {
    unsigned temp_child_key = this->tree_table[child].key;
    ValueType temp_child_value = this->tree_table[child].value;
    this->heap_table.update(tree_table[child].key, parent);    
    this->tree_table[child].key = this->tree_table[parent].key;
    this->tree_table[child].value = this->tree_table[parent].value;
    //*(this->heap_table.get(tree_table[child].key)) = parent;
    this->heap_table.update(tree_table[parent].key, child);
    this->tree_table[parent].key = temp_child_key;
    this->tree_table[parent].value = temp_child_value;
    //*(this->heap_table.get(tree_table[parent].key)) = child;


}

template <typename ValueType>
void PriorityQueue<ValueType>::proclate_up() {
    unsigned child_index = this->heap_elements;
    unsigned parent_index = this->heap_elements/2;
    while (this->tree_table[parent_index].key > this->tree_table[child_index].key) {
        this->swap(child_index, parent_index);
        child_index = parent_index;
        parent_index = child_index/2;
    }
}

template <typename ValueType>
void PriorityQueue<ValueType>::proclate_up_try(unsigned current_index) {
    unsigned child_index = current_index;
    unsigned parent_index = current_index/2;
    while (this->tree_table[parent_index].key > this->tree_table[child_index].key) {
        this->swap(child_index, parent_index);
        child_index = parent_index;
        parent_index = child_index/2;
    }    
}

template <typename ValueType>
bool PriorityQueue<ValueType>::proclate_down(unsigned current_index) {
    unsigned left_child_index = 2 * current_index;
    unsigned right_child_index = 2 * current_index + 1;
    unsigned final_index = 0;
    if (left_child_index > heap_elements && right_child_index > heap_elements) {
        return true;
    } else if (left_child_index <= heap_elements && right_child_index > heap_elements) {
        final_index = left_child_index;
    } else {
        if (this->tree_table[left_child_index].key > this->tree_table[right_child_index].key) {
            final_index = right_child_index;
        } else {
            final_index = left_child_index;
        }
    }
    if (this->tree_table[final_index].state != 1) {
        return true;
    }
    while (this->tree_table[current_index].key > this->tree_table[final_index].key) {
        this->swap (current_index, final_index);
        current_index = final_index;
        left_child_index = 2 * current_index;
        right_child_index = 2 * current_index + 1;
        if (left_child_index > heap_size - 1 && right_child_index > heap_size - 1) {
            return true;
        } else if (left_child_index <= heap_elements && right_child_index > heap_elements) {
            final_index = left_child_index;
        } else {
            if (this->tree_table[left_child_index].key > this->tree_table[right_child_index].key) {
                final_index = right_child_index;
            } else {
                final_index = left_child_index;
            }
        }
        if (this->tree_table[final_index].state != 1) {
            return true;
        }
    }    
    return true;
}

template <typename ValueType>
const unsigned* PriorityQueue<ValueType>::getMinKey() const {
    if (this->heap_elements == 0) {
        return nullptr;
    }
    return &this->tree_table[1].key;
}

template <typename ValueType>
const ValueType* PriorityQueue<ValueType>::getMinValue() const {
    if (this->heap_elements == 0) {
        return nullptr;
    }
    return &this->tree_table[1].value;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::deleteMin() {
    if (this->heap_elements == 0) {
        return false;
    }
    this->heap_table.remove(tree_table[1].key);
    this->swap(1, heap_elements);
    this->tree_table[heap_elements].state = 2;
    this->heap_elements --;
    //figure out which way we wanna go!
    unsigned current_index = 1;
    return proclate_down(current_index);
}

template <typename ValueType>
ValueType* PriorityQueue<ValueType>::get(unsigned key) {
    if (this->heap_table.get(key) == nullptr) {
        return nullptr;
    } 
    return &((tree_table[*(this->heap_table.get(key))]).value);
}

template <typename ValueType>
const ValueType* PriorityQueue<ValueType>::get(unsigned key) const {
    if (this->heap_table.get(key) == nullptr) {
        return nullptr;
    } 
    return &((tree_table[*(this->heap_table.get(key))]).value);
}

template <typename ValueType>
bool PriorityQueue<ValueType>::decreaseKey(unsigned key, unsigned change) {
    if (change == 0) {
        return false;
    }
    unsigned* temp = this->heap_table.get(key);
    if (temp == nullptr) {
        return false;
    }
    temp = this->heap_table.get(key - change);
    if (temp != nullptr) {
        return false;
    }

    unsigned current_index = *(heap_table.get(key));
    this->heap_table.remove(key);
    this->heap_table.insert(key - change, current_index);
    tree_table[current_index].key = key - change;
    proclate_up_try(current_index);
    return true;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::increaseKey(unsigned key, unsigned change) {
    if (change == 0) {
        return false;
    }
    unsigned* temp = this->heap_table.get(key);
    if (temp == nullptr) {
        return false;
    }
    temp = this->heap_table.get(key + change);
    if (temp != nullptr) {
        return false;
    }
    unsigned current_index = *(heap_table.get(key));
    this->heap_table.remove(key);
    this->heap_table.insert(key + change, current_index);
    tree_table[current_index].key = key + change;
    proclate_down(current_index);
    return true;
}

template <typename ValueType>
bool PriorityQueue<ValueType>::remove(unsigned key) {
    unsigned* temp = this->heap_table.get(key);
    if (temp == nullptr) {
        return false;
    }
    unsigned key_index = *(this->heap_table.get(key));
    this->swap(key_index, this->heap_elements);
    this->tree_table[this->heap_elements].state = 2;
    this->heap_table.remove(key);
    this->heap_elements --;
    return proclate_down(key_index);
}