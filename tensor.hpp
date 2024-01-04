
#pragma once //ensures the header file is only included once in a translation unit.

#include <iostream> //C++ Standard Library's iostream header, cout and cin
#include <memory> //provides facilities for dynamic memory allocation and smart pointers.
#include <stdexcept> //standard exception classes  like std::logic_error and std::runtime_error, that can be used for exception handling.
#include <vector> //provides the std::vector template class. Vectors are dynamic arrays that can grow or shrink in size. 
#include <fstream>
#include <sstream>


template< class T >
concept Arithmetic = std::is_arithmetic_v< T >; //Concept is a way to express requirements on template arguments.  this concept takes a type parameter T. checks if the type T is arithmetic like int, float, etc.

template< Arithmetic ComponentType > //a template parameter named ComponentType & The Arithmetic concept is applied to it so ComponentType must be an arithmetic type for the template to be instantiated.
class Tensor
{
public:
    // Constructs a tensor with rank = 0 and zero-initializes the element.
    Tensor() : T_rank(0), ele(0), element(0) {
        std::cout << "Tensor constructed with rank 0 and zero-initialized element." << std::endl;
    }

    // Constructs a tensor with arbitrary shape and zero-initializes all element.
    Tensor(const std::vector<size_t>& shape) : T_rank(shape.size()), shape(shape) {
        size_t totalelement = 1;
        for (size_t i = 0; i < shape.size(); ++i) {
            totalelement *= shape[i];
        }
        element = std::vector<ComponentType>(totalelement, 0);
        std::cout << "Tensor constructed with rank " << T_rank << " and zero-initialized element." << "element size: " << element.size() << std::endl;
    }

    // Constructs a tensor with arbitrary shape and fills it with the specified value.
    explicit Tensor(const std::vector<size_t>& shape, const ComponentType& fillValue)
        : T_rank(shape.size()), shape(shape) {
        size_t totalelement = 1;
        for (size_t i = 0; i < shape.size(); ++i) {
            totalelement *= shape[i];
        }
        element = std::vector<ComponentType>(totalelement, fillValue);

        std::cout << "Tensor constructed with rank " << T_rank << " and filled with specified value." << std::endl;
        //std::cout << element.size() << std::endl;
    }
    /*
    explicit Tensor(const std::vector<size_t>& shape, const ComponentType& fillValue)
    : rank(shape.size()), element(fillValue), shape(shape) {
    std::cout << "Tensor constructed with rank " << rank << " and filled with specified value." << std::endl;
}
*/

// Copy-constructor.
    Tensor(const Tensor<ComponentType>& other)
        : T_rank(other.T_rank), element(other.element), shape(other.shape) {
        std::cout << "Tensor copied from another instance. Rank: " << T_rank << std::endl;
    }

    // Move-constructor.
    /*
    std::exchange for rank:

std::exchange(other.rank, 0) swaps the value of other.rank with 0 and returns the original value of other.rank. This is a way to move the value from other.rank to the new object (rank in the move constructor) while leaving other.rank in a well-defined state (in this case, set to 0).
It's a safe and expressive way to perform the move for objects where you want to leave the source object in a known state after the move.
std::move for element:

std::move(other.element) casts other.element to an rvalue reference, allowing the move constructor to take ownership of the resources owned by other.element.
This is the typical usage for moving non-trivial objects, where you want to efficiently transfer ownership of resources from one object to another.
In summary, the use of std::exchange for rank is a more defensive approach, ensuring that the source object (other) is left in a well-defined state after the move. For element, the use of std::move is a standard approach for efficiently transferring ownership of resources during a move operation. You can adapt these choices based on the specific requirements and semantics of your class.*/
//The noexcept specifier indicates that the move constructor doesn't throw exceptions.
    Tensor(Tensor<ComponentType>&& other) noexcept
        : T_rank(std::exchange(other.T_rank, 0)), element(std::move(other.element)), shape(std::move(other.shape)) {
        std::cout << "Tensor moved from another instance. Rank: " << T_rank << std::endl;
    }

    // Copy-assignment
    Tensor&
        operator=(const Tensor< ComponentType >& other) {
        // Self-assignment check
        if (this != &other) {
            // Copy the values from the other instance
            T_rank = other.T_rank;
            element = other.element;
            shape = other.shape;
            std::cout << "Tensor assigned from another instance. Rank: " << T_rank << "(Copy-assignmen)" << std::endl;
        }
        return *this;
    }

    // Move-assignment
    Tensor&
        operator=(Tensor< ComponentType >&& other) noexcept {
        // Self-assignment check
        if (this != &other) {
            // Move the values from the other instance
            T_rank = std::exchange(other.T_rank, 0);
            element = std::move(other.element);
            shape = std::move(other.shape);
            std::cout << "Tensor moved from another instance. Rank: " << T_rank << std::endl;
        }
        return *this;
    }

    // Destructor
    ~Tensor() = default;

    //The [[nodiscard]] attribute is used to indicate that the return value should not be ignored. This can be useful to catch potential mistakes where the return value is unintentionally ignored.

    // Returns the rank of the tensor.
    [[nodiscard]] size_t rank() const {
        return T_rank;
    }

    // Returns the shape of the tensor.
    [[nodiscard]] std::vector< size_t > Get_shape() const {
        return shape;
    }

    // Returns the number of element of this tensor.
    [[nodiscard]] size_t numElements() const {
        size_t totalelement = 1;
        for (size_t i = 0; i < shape.size(); ++i) {
            totalelement *= shape[i];
        }
        return totalelement;

    }

    // Element access function
    const ComponentType&
        operator()(const std::vector< size_t >& idx) const {
        // Calculate the linear index

        std::cout << "linear index" << std::endl;

        size_t linearIndex = calculateLinearIndex(idx);

        // Access the element using the linear index
        return element[linearIndex];
    }

    // Helper function to calculate the linear index from multi-dimensional index
    size_t calculateLinearIndex(const std::vector<size_t>& idx) const {
        size_t linearIndex = 0;
        size_t stride = 1;
        // Assuming 'shape' is a member variable representing the tensor shape
        for (int i = shape.size() - 1; i >= 0; --i) {
            linearIndex += idx[i] * stride;
            stride *= shape[i];
        }
        return linearIndex;
    }

    // Element mutation function
    ComponentType&
        operator()(const std::vector< size_t >& idx) {
        if (element.empty()) {
            return ele;
        }
        // Calculate the linear index
        size_t linearIndex = calculateLinearIndex(idx);

        // Mutate the element at the linear index
        return element[linearIndex];
    }

    friend Tensor<ComponentType> readTensorFromFile<>(const std::string& filename);
    friend void writeTensorToFile<>(const Tensor<ComponentType>& tensor, const std::string& filename);


private:
    // TODO: Probably you need some members here...
    int T_rank;      // Rank of the tensor, The rank is a variable that represents the "dimensionality" or "number of indices" of the tensor.
    ComponentType ele;
    std::vector<ComponentType> element; // Element of the tensor -- was double
    std::vector< size_t > shape;
};

// TODO: Implement all methods of the Tensor class template.


// Returns true if the shapes and all element of both tensors are equal.
template< Arithmetic ComponentType >
bool operator==(const Tensor< ComponentType >& a, const Tensor< ComponentType >& b) {
    // Check if shapes are equal
    if (a.Get_shape() != b.Get_shape()) {
        return false;
    }

    /*
    // Check if element are equal
    for (size_t i = 0; i < a.numElements(); ++i) {
        if (a.flatten()[i] != b.flatten()[i]) {
            return false;
        }
    }
    */

    return true;
}
/*
// Pretty-prints the tensor to stdout.
// This is not necessary (and not covered by the tests) but nice to have, also for debugging (and for exercise of course...).
template< Arithmetic ComponentType >
std::ostream&
operator<<(std::ostream& out, const Tensor< ComponentType >& tensor) {
    // Print tensor shape
    out << "Tensor shape: [";
    for (size_t dim : tensor.Get_shape()) {
        out << dim << " ";
    }
    out << "]" << std::endl;

    // Print tensor element
    out << "Tensor element:" << std::endl;
    for (size_t i = 0; i < tensor.numElements(); ++i) {
        out << tensor[i] << " ";
    }
    out << std::endl;

    return out;
}
*/
//Reads a tensor from file.
template< Arithmetic ComponentType >
Tensor<ComponentType> readTensorFromFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    size_t rank;
    file >> rank;

    std::vector<size_t> shape(rank);
    for (size_t i = 0; i < rank; ++i) {
        file >> shape[i];
    }

    Tensor<ComponentType> tensor(shape);

    for (size_t i = 0; i < tensor.numElements(); ++i) {
        file >> tensor.element[i];
    }

    return tensor;
}

template< Arithmetic ComponentType >
void writeTensorToFile(const Tensor<ComponentType>& tensor, const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Error opening file: " + filename);
    }

    file << tensor.rank() << '\n';

    const auto& shape = tensor.Get_shape();
    for (size_t i = 0; i < tensor.rank(); ++i) {
        file << shape[i] << '\n';
    }

    const auto& data = tensor.element;
    for (size_t i = 0; i < tensor.numElements(); ++i) {
        file << data[i] << ' ';
    }

    std::cout << "Tensor written to file: " << filename << std::endl;
}
