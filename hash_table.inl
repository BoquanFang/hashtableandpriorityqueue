/**
 * log:
 * Do operator overloadings and noexcept
 * Noexcept can't say it is right
 * operator+???
 */


template <typename ValueType>
HashTable<ValueType>::HashTable(const HashTable& rhs) {
    table = new Pair<ValueType>[rhs.size];
    size = rhs.size;
    num_Elements = rhs.num_Elements;
    for (unsigned i = 0; i < rhs.size; i ++) {
        table[i].key = rhs.table[i].key;
        table[i].value = rhs.table[i].value;
        table[i].state = rhs.table[i].state;
        //table[i].order = rhs.table[i].order;
    }
}

template <typename ValueType>
HashTable<ValueType>&HashTable<ValueType>:: operator=(const HashTable& rhs) {
    delete[] table;
    table = new Pair<ValueType>[rhs.size];
    size = rhs.size;
    num_Elements = rhs.num_Elements;
    for (unsigned i = 0; i < rhs.size; i ++) {
        table[i].key = rhs.table[i].key;
        table[i].value = rhs.table[i].value;
        table[i].state = rhs.table[i].state;
        //table[i].order = rhs.table[i].order;
    }
    return *this;       
}

//Need to do those two noexpects
template <typename ValueType>
HashTable<ValueType>::HashTable(HashTable&& rhs) noexcept {
    //delete[] this->table;
    this -> table = rhs.table;
    this -> size = rhs.size;
    this -> num_Elements = rhs.num_Elements;
    rhs.table = NULL;
    rhs.size = 0;
    rhs.num_Elements = 0;
}

template <typename ValueType>
HashTable<ValueType>&HashTable<ValueType>:: operator=(HashTable&& rhs) noexcept {
    delete[] table;
    this -> table = rhs.table;
    this -> size = rhs.size;
    this -> num_Elements = rhs.num_Elements;
    rhs.table = NULL;
    rhs.size = 0;
    rhs.num_Elements = 0;
    return *this;    
}

template <typename ValueType>
unsigned HashTable<ValueType>::tableSize() const {
    return size;
}

template <typename ValueType>
unsigned HashTable<ValueType>::numElements() const {
    return num_Elements;
}

bool isPrime(unsigned n)  { 
    // Corner case 
    if (n <= 1) {
        return false; 
    }

  
    // Check from 2 to n-1 
    for (unsigned i = 2; i < n; i++) {
        if (n % i == 0) {
            return false; 
        }
    }

  
    return true; 
}

unsigned next_prime(unsigned table_size) {
    table_size ++;
    while (!isPrime(table_size)) {
        table_size ++;
    }
    return table_size;
}

template <typename ValueType>
void HashTable<ValueType>::rehash() {
    Pair<ValueType>* temp = this->table;
    unsigned temp_length = this->size;
    this->table = new Pair<ValueType>[next_prime(2 * this->size)];
    this->size = next_prime(2 * this->size);
    for (unsigned i = 0; i < temp_length; i ++) {
        if (temp[i].state == 1) {
            this->insert(temp[i].key, temp[i].value);
            this->num_Elements -= 1;
        }
    }
    delete[] temp;

}

template <typename ValueType>
bool HashTable<ValueType>::insert(unsigned key, const ValueType& value) {
    //std::cout << "The load factor is " << ((num_Elements + 1) /(double) this->size) << std::endl;
     
    unsigned location = key % this->size;
    unsigned new_location = location;
    unsigned i = 1;
    while (this -> table [new_location].state == 1) {
        if (table[new_location].key == key) {
            return false;
        }
        new_location = (location + i*i) % this->size;
        if (new_location  == location) {
            return false;
        }
        i++;
    }
    if (((this->num_Elements + 1) / (double)this->size) >= (0.5)) {
        this -> rehash();
    }
    location = key % this->size;
    new_location = location;
    if (this->table[location].state != 1) {
  
        this->table[location].state = 1;
        this->table[location].key = key;
        this->table[location].value = value;
    } else {
        unsigned i = 1;
        while (this -> table [new_location].state == 1) {
            if (table[new_location].key == key) {
                return false;
            }
            new_location = (location + i*i) % this->size;
            if (new_location  == location) {
                return false;
            }
            i++;
        }
        this->table[new_location].state = 1;
        this->table[new_location].key = key;
        this->table[new_location].value = value;        
    }
    this->num_Elements ++;
    //this->table[new_location].order = this-> num_Elements;
    return true;
}

template <typename ValueType>
ValueType* HashTable<ValueType>::get(unsigned key) {
    unsigned location = key % this->size;
    unsigned new_location = location;
    int i = 1;
    if (table[location].state == 0) {
        return nullptr;
    } else {
        while (this->table[new_location].key != key) {
            new_location = (location + i*i) % this->size;
            if (new_location  == location) {
                return nullptr;
            }
            i ++;
        }
        if (this->table[new_location].state != 1) {
            return nullptr;
        }
        return &this -> table[new_location].value;
    }
}

template <typename ValueType>
const ValueType* HashTable<ValueType>::get(unsigned key) const {
    unsigned location = key % this->size;
    unsigned new_location = location;
    int i = 1;
    if (this->table[location].state == 0) {
        return nullptr;
    } else {
        while (this->table[new_location].key != key) {
            new_location = (location + i*i) % this->size;
            if (new_location  == location) {
                return nullptr;
            }
        }
        if (this->table[new_location].state != 1) {
            return nullptr;
        }
        return &this -> table[new_location].value;
    }
}

template <typename ValueType>
bool HashTable<ValueType>::update(unsigned key, const ValueType& newValue) {
    ValueType* val = this->get(key);
    if (val == NULL) {
        return false;
    }
    *val = newValue;
    return true;
}

template <typename ValueType>
bool HashTable<ValueType>::remove(unsigned key) {
    unsigned location = key % this->size;
    unsigned new_location = location;
    int i = 1;
    if (this->table[location].state == 0) {
        return false;
    } else {
        while (this->table[new_location].key != key) {
            //std::cout << this->table[new_location].key << std::endl;
            new_location = (location + i*i) % this->size;
            if (new_location  == location) {
                return false;
            }
            i ++;
        }
        this -> table[new_location].state = 2;
        this -> num_Elements --;
        return true;
    }
}

template <typename ValueType>
unsigned HashTable<ValueType>::removeAllByValue(const ValueType& value) {
    unsigned counter = 0;
    for (unsigned i = 0; i < this->size; i ++) {
        if (this->table[i].value == value && this->table[i].state == 1) {
            this->table[i].state = 2;
            counter ++;
            this -> num_Elements --;
        }
    }
    return counter;
}

template <typename ValueType>
bool HashTable<ValueType>::operator==(const HashTable& rhs) const {
    if (this->num_Elements != rhs.num_Elements) {
        return false;
    }
    for (unsigned i = 0; i < this->size ; i ++) {
        bool found = false;
        if (this->table[i].state == 1) {
            unsigned this_key = this->table[i].key;
            ValueType this_value = this->table[i].value;
            for (unsigned j = 0; j < rhs.size; j ++) {
                if (rhs.table[j].state == 1 && rhs.table[j].key == this_key) {
                    found = true;
                    ValueType rhs_value = rhs.table[j].value;
                    if (this_value != rhs_value) {
                        return false;
                    }
                }
            }
            if (found == false) {
                return false;
            } else {
                found = false;
            }            
        }
    }
    return true;
}

template <typename ValueType>
bool HashTable<ValueType>::operator!=(const HashTable& rhs) const {
    return !(*this == rhs);
}

template <typename ValueType>
HashTable<ValueType> HashTable<ValueType>::operator+(const HashTable& rhs) const {
    HashTable<ValueType> temp (this->size);
    delete[] temp.table;
    temp.table = new Pair<ValueType> [this->size];
    temp.size = this->size;
    temp.num_Elements = this->num_Elements;
    for (unsigned i = 0; i < this->size; i ++) {
        temp.table[i].key = this->table[i].key;
        temp.table[i].value = this->table[i].value;
        temp.table[i].state = this->table[i].state;
    }
    for (unsigned i = 0; i < rhs.size; i++) {
        if (rhs.table[i].state == 1) {
            temp.insert(rhs.table[i].key, rhs.table[i].value);
        }
    }
    return temp;
}

