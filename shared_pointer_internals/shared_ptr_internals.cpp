#include <iostream>
#include <memory>
#include <cstddef> // for std::nullptr_t

template<typename T>
class shared_pointer {
private:
    T* ptr;           // Actual pointer to the object
    unsigned* count;  // Reference count

public:
    // Default constructor
    explicit shared_pointer(T* p = nullptr) : ptr(p), count(new unsigned(1)) {
        if (p == nullptr) {
            *count = 0;
        }
    }

    // Copy constructor
    shared_pointer(const shared_pointer<T>& sp) : ptr(sp.ptr), count(sp.count) {
        if (ptr) {
            (*count)++;
        }
    }

    // Assignment operator
    shared_pointer<T>& operator=(const shared_pointer<T>& sp) {
        if (this != &sp) {  // Self-assignment check
            // Decrement the old reference count
            // if reference becomes zero delete the old data
            if (ptr && --(*count) == 0) {
                delete ptr;
                delete count;
            }

            // Copy the data from sp
            ptr = sp.ptr;
            count = sp.count;
            if (ptr) {
                (*count)++;
            }
        }
        return *this;
    }

    // Destructor
    ~shared_pointer() {
        if (ptr && --(*count) == 0) {
            delete ptr;
            delete count;
        }
    }

    // Overload dereference operator
    T& operator*() const {
        return *ptr;
    }

    // Overload arrow operator
    T* operator->() const {
        return ptr;
    }

    // Get the current reference count
    unsigned use_count() const {
        return *count;
    }

    // Check if the pointer is unique (use count is 1)
    bool unique() const {
        return *count == 1;
    }

    // Reset the shared_pointer
    void reset(T* p = nullptr) {
        if (ptr && --(*count) == 0) {
            delete ptr;
            delete count;
        }
        ptr = p;
        count = new unsigned(p ? 1 : 0);
    }

    // Check if there is an associated managed object
    bool operator==(std::nullptr_t) const {
        return ptr == nullptr;
    }
};

// Helper function to create a shared_pointer
template<typename T, typename... Args>
shared_pointer<T> make_shared_pointer(Args&&... args) {
    return shared_pointer<T>(new T(std::forward<Args>(args)...));
}


void test_std_shared_ptr() {
    std::shared_ptr<int> sp1(new int(10)); // Reference count is 1
    {
        std::shared_ptr<int> sp2 = sp1; // Reference count is 2
        std::cout << "SP2 - Value: " << *sp2 << ", Ref count: " << sp2.use_count() << std::endl;
    } // sp2 goes out of scope, Reference count is 1

    std::cout << "SP1 - Value: " << *sp1 << ", Ref count: " << sp1.use_count() << std::endl;
} // sp1 goes out of scope, Reference count is 0, object is deleted

void test_custom_shared_ptr() {
    // Create shared pointer using explicit constructor
    shared_pointer<int> custom_sp1(new int(10));

    // Create shared pointer using the helper function
    shared_pointer<int> custom_sp2 = make_shared_pointer<int>(100);

    std::cout << "custom_sp1 value: " << *custom_sp1 << ", ref count: " << custom_sp1.use_count() << std::endl;
    std::cout << "custom_sp2 value: " << *custom_sp2 << ", ref count: " << custom_sp2.use_count() << std::endl;

    {
        // invokes copy constructor 
        shared_pointer<int> custom_sp3 = custom_sp1;
        std::cout << std::endl;
        std::cout << "copy constructor 1st way: (custom_sp3 = custom_sp1), custom_sp3 value: " << *custom_sp3 << std::endl;
        std::cout << "custom_sp1 ref count: " << custom_sp1.use_count() << std::endl;
        std::cout << "custom_sp2 ref count: " << custom_sp2.use_count() << std::endl;
        std::cout << "custom_sp3 ref count: " << custom_sp3.use_count() << std::endl;


        //invoke the copy constructor
        shared_pointer<int> custom_sp4(custom_sp3);
        std::cout << std::endl;
        std::cout << "copy constructor 2nd way: (custom_sp4(custom_sp3)), custom_sp4 value: " << *custom_sp4 << std::endl;
        std::cout << "custom_sp1 ref count: " << custom_sp1.use_count() << std::endl;
        std::cout << "custom_sp2 ref count: " << custom_sp2.use_count() << std::endl;
        std::cout << "custom_sp3 ref count: " << custom_sp3.use_count() << std::endl;
        std::cout << "custom_sp4 ref count: " << custom_sp4.use_count() << std::endl;

        // invokes the assignment operator
        custom_sp2 = custom_sp4;
        std::cout << std::endl;
        std::cout << "assignment operator: (custom_sp2 = custom_sp4), custom_sp2 value: " << *custom_sp2 << std::endl;
        std::cout << "custom_sp1 ref count: " << custom_sp1.use_count() << std::endl;
        std::cout << "custom_sp2 ref count: " << custom_sp2.use_count() << std::endl;
        std::cout << "custom_sp3 ref count: " << custom_sp3.use_count() << std::endl;
        std::cout << "custom_sp4 ref count: " << custom_sp4.use_count() << std::endl;
    } // custom_sp3 and custom_sp4 goes out of scope and decrease ref count

    std::cout << std::endl;
    std::cout << "custom_sp1 value: " << *custom_sp1 << ", ref count: " << custom_sp1.use_count() << std::endl;
    std::cout << "custom_sp2 value: " << *custom_sp2 << ", ref count: " << custom_sp2.use_count() << std::endl;

} // custom_sp1 and custom_sp2 auto deletes when they go out of scope

int main() {

    test_std_shared_ptr();

    test_custom_shared_ptr();



    return 0;
} 
