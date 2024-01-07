#pragma once

#include "tensor.hpp"

template<typename ComponentType>
class Vector
{
public:
    // Default-constructor.
    Vector() = default;

    // Constructor for vector of certain size.
    explicit Vector(size_t size);

    // Constructor for vector of certain size with constant fill-value.
    Vector(size_t size, const ComponentType& fillValue);

    // Constructing vector from file.
    Vector(const std::string& filename);

    // Number of elements in this vector.
    [[nodiscard]] size_t size() const;

    // Element access function
    const ComponentType& operator()(size_t idx) const;

    // Element mutation function
    ComponentType& operator()(size_t idx);

    // Reference to internal tensor.
    Tensor<ComponentType>& tensor();

private:
    Tensor<ComponentType> tensor_;
};

template<typename ComponentType>
Vector<ComponentType>::Vector(size_t size) : tensor_(std::vector<size_t>{size})
{
    std::cout << "Vector constructor with size called. Size: " << size << std::endl;
}

template<typename ComponentType>
Vector<ComponentType>::Vector(size_t size, const ComponentType& fillValue)
    : tensor_(std::vector<size_t>{size}, fillValue)
{
    std::cout << "Vector constructor with size and fill-value called. Size: " << size << ", Fill Value: " << fillValue << std::endl;
}

template<typename ComponentType>
Vector<ComponentType>::Vector(const std::string& filename) : tensor_(readTensorFromFile<ComponentType>(filename))
{
    std::cout << "Vector constructor from file called. Filename: " << filename << std::endl;
}

template<typename ComponentType>
size_t Vector<ComponentType>::size() const
{
    std::cout << "Vector size method called." << std::endl;
    return tensor_.numElements();
}

template<typename ComponentType>
const ComponentType& Vector<ComponentType>::operator()(size_t idx) const
{
    std::cout << "Vector element access (const) method called. Index: " << idx << std::endl;
    return tensor_({ idx });
}

template<typename ComponentType>
ComponentType& Vector<ComponentType>::operator()(size_t idx)
{
    std::cout << "Vector element mutation method called. Index: " << idx << std::endl;
    return tensor_({ idx });
}

template<typename ComponentType>
Tensor<ComponentType>& Vector<ComponentType>::tensor()
{
    std::cout << "Vector tensor method called." << std::endl;
    return tensor_;
}

template<typename ComponentType>
class Matrix
{
public:
    // Default-constructor.
    Matrix() = default;

    // Constructor for matrix of a certain size.
    explicit Matrix(size_t rows, size_t cols);

    // Constructor for matrix of a certain size with constant fill-value.
    Matrix(size_t rows, size_t cols, const ComponentType& fillValue);

    // Constructing matrix from file.
    Matrix(const std::string& filename);

    // Number of rows.
    [[nodiscard]] size_t rows() const;

    // Number of columns.
    [[nodiscard]] size_t cols() const;

    // Element access function
    const ComponentType& operator()(size_t row, size_t col) const;

    // Element mutation function
    ComponentType& operator()(size_t row, size_t col);

    // Reference to internal tensor.
    Tensor<ComponentType>& tensor();

private:
    Tensor<ComponentType> tensor_;
};

template<typename ComponentType>
Matrix<ComponentType>::Matrix(size_t rows, size_t cols) : tensor_(std::vector<size_t>{rows, cols})
{
    std::cout << "Matrix constructor with size called. Rows: " << rows << ", Cols: " << cols << std::endl;
}

template<typename ComponentType>
Matrix<ComponentType>::Matrix(size_t rows, size_t cols, const ComponentType& fillValue)
    : tensor_(std::vector<size_t>{rows, cols}, fillValue)
{
    std::cout << "Matrix constructor with size and fill-value called. Rows: " << rows << ", Cols: " << cols
        << ", Fill Value: " << fillValue << std::endl;
}

template<typename ComponentType>
Matrix<ComponentType>::Matrix(const std::string& filename) : tensor_(readTensorFromFile<ComponentType>(filename))
{
    std::cout << "Matrix constructor from file called. Filename: " << filename << std::endl;
}

template<typename ComponentType>
size_t Matrix<ComponentType>::rows() const
{
    std::cout << "Matrix rows method called." << std::endl;
    return tensor_.Get_shape()[0];
}

template<typename ComponentType>
size_t Matrix<ComponentType>::cols() const
{
    std::cout << "Matrix cols method called." << std::endl;
    return tensor_.Get_shape()[1];
}

template<typename ComponentType>
const ComponentType& Matrix<ComponentType>::operator()(size_t row, size_t col) const
{
    std::cout << "Matrix element access (const) method called. Row: " << row << ", Col: " << col << std::endl;
    return tensor_({ row, col });
}

template<typename ComponentType>
ComponentType& Matrix<ComponentType>::operator()(size_t row, size_t col)
{
    std::cout << "Matrix element mutation method called. Row: " << row << ", Col: " << col << std::endl;
    return tensor_({ row, col });
}

template<typename ComponentType>
Tensor<ComponentType>& Matrix<ComponentType>::tensor()
{
    std::cout << "Matrix tensor method called." << std::endl;
    return tensor_;
}

// TODO: Implement all methods.

// Performs a matrix-vector multiplication.
template<typename ComponentType>
Vector<ComponentType> matvec(const Matrix<ComponentType>& mat, const Vector<ComponentType>& vec)
{
    // Get the number of rows and columns of the matrix
    size_t rows = mat.rows();
    size_t cols = mat.cols();

    // Create a vector to store the result of the multiplication
    Vector<ComponentType> result(rows);

    // Perform matrix-vector multiplication
    for (size_t i = 0; i < rows; ++i)
    {
        ComponentType sum = 0;
        for (size_t j = 0; j < cols; ++j)
        {
            // Multiply corresponding elements and accumulate the sum
            sum += mat(i, j) * vec(j);
        }
        // Set the result vector element
        result(i) = sum;
    }

    // Return the result vector
    return result;
}
