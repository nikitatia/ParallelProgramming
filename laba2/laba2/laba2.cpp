#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <string>
#include <memory>

class IMatrix {
public:
    virtual ~IMatrix() = default;
    virtual void Import(const std::string& filename) = 0;
    virtual void Export(const std::string& filename) const = 0;
    virtual void Print() const = 0;
};

template <typename T = double>
class MatrixDense : public IMatrix 
{
private:
    unsigned _m, _n;
    T* data;

public:
    MatrixDense(unsigned m, unsigned n) : _m(m), _n(n), data(new T[m * n]()) {}
    ~MatrixDense() { delete[] data; }

    T& operator()(unsigned i, unsigned j) 
    {
        if (i >= _m || j >= _n)
            throw std::out_of_range("Index out of range");
        return data[i * _n + j];
    }

    const T& operator()(unsigned i, unsigned j) const 
    {
        if (i >= _m || j >= _n)
            throw std::out_of_range("Index out of range");
        return data[i * _n + j];
    }

    MatrixDense operator+(const MatrixDense& other) const
    {
        if (_m != other._m || _n != other._n)
            throw std::invalid_argument("Matrix dimensions do not match for addition");
        MatrixDense result(_m, _n);
        for (unsigned i = 0; i < _m * _n; ++i)
            result.data[i] = data[i] + other.data[i];
        return result;
    }

    MatrixDense operator-(const MatrixDense& other) const 
    {
        if (_m != other._m || _n != other._n)
            throw std::invalid_argument("Matrix dimensions do not match for subtraction");
        MatrixDense result(_m, _n);
        for (unsigned i = 0; i < _m * _n; ++i)
            result.data[i] = data[i] - other.data[i];
        return result;
    }

    MatrixDense operator*(const MatrixDense& other) const 
    { 
        if (_m != other._m || _n != other._n)
            throw std::invalid_argument("Matrix dimensions do not match for element-wise multiplication");
        MatrixDense result(_m, _n);
        for (unsigned i = 0; i < _m * _n; ++i)
            result.data[i] = data[i] * other.data[i];
        return result;
    }

    MatrixDense MatrixMultiply(const MatrixDense& other) const 
    {
        if (_n != other._m)
            throw std::invalid_argument("Matrix dimensions do not match for matrix multiplication");
        MatrixDense result(_m, other._n);
        for (unsigned i = 0; i < _m; ++i)
            for (unsigned j = 0; j < other._n; ++j)
                for (unsigned k = 0; k < _n; ++k)
                    result(i, j) += (*this)(i, k) * other(k, j);
        return result;
    }

    MatrixDense Transpose() const 
    {
        MatrixDense result(_n, _m);
        for (unsigned i = 0; i < _m; ++i)
            for (unsigned j = 0; j < _n; ++j)
                result(j, i) = (*this)(i, j);
        return result;
    }

    void Import(const std::string& filename) override 
    {
        std::ifstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file for importing");

        std::string matrixType;
        file >> matrixType;
        if (matrixType != "MatrixDense")
            throw std::runtime_error("Incorrect matrix type in file");

        file >> _m >> _n;
        delete[] data;
        data = new T[_m * _n];
        for (unsigned i = 0; i < _m * _n; ++i)
            file >> data[i];
        file.close();
    }

    void Export(const std::string& filename) const override 
    {
        std::ofstream file(filename);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file for exporting");

        file << "MatrixDense\n";
        file << _m << " " << _n << "\n";
        for (unsigned i = 0; i < _m; ++i) 
        {
            for (unsigned j = 0; j < _n; ++j)
                file << (*this)(i, j) << " ";
            file << "\n";
        }
        file.close();
    }

    void Print() const override 
    {
        for (unsigned i = 0; i < _m; ++i) 
        {
            for (unsigned j = 0; j < _n; ++j)
                std::cout << (*this)(i, j) << " ";
            std::cout << "\n";
        }
    }
};


class MatrixDiagonal : public IMatrix 
{

};

class MatrixBlock : public IMatrix 
{

};

void Test() 
{
    MatrixDense<> mat1(2, 3);
    mat1(0, 0) = 1; mat1(0, 1) = 2; mat1(0, 2) = 3;
    mat1(1, 0) = 4; mat1(1, 1) = 5; mat1(1, 2) = 6;

    MatrixDense<> mat2 = mat1.Transpose();

    std::cout << "Matrix 1:\n";
    mat1.Print();

    std::cout << "Matrix 2 (Transpose of Matrix 1):\n";
    mat2.Print();
}

int main() 
{
    Test();
    return 0;
}
