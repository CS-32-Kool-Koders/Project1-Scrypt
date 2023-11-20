#include <iostream>
#include <vector>
#include <stdexcept>

// Define a custom exception for array-related runtime errors
class ArrayError : public std::runtime_error {
public:
    ArrayError(const std::string& message) : std::runtime_error(message) {}
};

// Define an array data structure
class Array {
private:
    std::vector<int> data; // You can change the data type as needed

public:
    Array() {}

    // Create an array from a list of elements (array literal)
    Array(std::vector<int> elements) : data(elements) {}

    // Get the length of the array
    int len() const {
        return data.size();
    }

    // Access an element by index (array lookup)
    int& operator[](int index) {
        
        return data[index];
    }

    // Push a value onto the end of the array (push)
    void push(int value) {
        data.push_back(value);
    }

    // Pop the last value from the array (pop)
    int pop() {
        int lastValue = data.back();
        data.pop_back();
        return lastValue;
    }
};

int main() {
    try {
        // Create an array using an array literal
        Array array({1, 2, 3, 4, 5});

        // Access elements using array lookup
        std::cout << "Array[2]: " << array[2] << std::endl;

        // Print the array
        std::cout << "Array: [";
        for (int i = 0; i < array.len(); ++i) {
            std::cout << array[i];
            if (i < array.len() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;

        // Create a reference to the array
        Array& arref = array;

        // Swap two elements in the array
        int temp = arref[1];
        arref[1] = -arref[3];
        arref[3] = -temp;

        // Print the modified array
        std::cout << "Modified Array: [";
        for (int i = 0; i < arref.len(); ++i) {
            std::cout << arref[i];
            if (i < arref.len() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;

        // Push and pop values
        array.push(6);
        std::cout << "Pushed 6: [";
        for (int i = 0; i < array.len(); ++i) {
            std::cout << array[i];
            if (i < array.len() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;

        int poppedValue = array.pop();
        std::cout << "Popped " << poppedValue << ": [";
        for (int i = 0; i < array.len(); ++i) {
            std::cout << array[i];
            if (i < array.len() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    } catch (const ArrayError& e) {
        std::cerr << "Runtime Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
