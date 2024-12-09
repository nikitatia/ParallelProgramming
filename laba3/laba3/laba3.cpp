#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>
#include <stdexcept>
#include <chrono>
#include <cmath>
#include <random>
#include <numeric>
#include <algorithm>

template<typename T>
class Vector
{
private:
    T* data;
    size_t n;
    bool is_initialized;
    std::mutex mtx;

public:
    Vector(size_t size) : n(size), is_initialized(false)
    {
        data = new T[n];
    }

    ~Vector()
    {
        delete[] data;
    }

    void initialize_const(T value)
    {
        for (size_t i = 0; i < n; ++i)
        {
            data[i] = value;
        }
        is_initialized = true;
    }

    void initialize_random(T min_value, T max_value)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<T> dist(min_value, max_value);
        for (size_t i = 0; i < n; ++i)
        {
            data[i] = dist(gen);
        }
        is_initialized = true;
    }

    void export_to_file(const std::string& filename)
    {
        if (!is_initialized) throw std::logic_error("Vector is not initialized!");
        std::ofstream file(filename);
        if (!file.is_open()) throw std::ios_base::failure("Failed to open file!");
        file << n << "\n";
        for (size_t i = 0; i < n; ++i)
        {
            file << data[i] << " ";
        }
        file.close();
    }

    void import_from_file(const std::string& filename)
    {
        std::ifstream file(filename);
        if (!file.is_open()) throw std::ios_base::failure("Failed to open file!");
        size_t size;
        file >> size;
        if (size != n) throw std::logic_error("Size mismatch during import!");
        for (size_t i = 0; i < n; ++i)
        {
            file >> data[i];
        }
        is_initialized = true;
        file.close();
    }

    T find_min()
    {
        if (!is_initialized) throw std::logic_error("Vector is not initialized!");
        return *std::min_element(data, data + n);
    }

    T find_max()
    {
        if (!is_initialized) throw std::logic_error("Vector is not initialized!");
        return *std::max_element(data, data + n);
    }

    size_t find_min_index()
    {
        if (!is_initialized) throw std::logic_error("Vector is not initialized!");
        return std::min_element(data, data + n) - data;
    }

    size_t find_max_index()
    {
        if (!is_initialized) throw std::logic_error("Vector is not initialized!");
        return std::max_element(data, data + n) - data;
    }

    T calculate_sum()
    {
        if (!is_initialized) throw std::logic_error("Vector is not initialized!");
        return std::accumulate(data, data + n, static_cast<T>(0));
    }

    T calculate_mean()
    {
        if (!is_initialized) throw std::logic_error("Vector is not initialized!");
        return calculate_sum() / n;
    }

    T calculate_euclidean_norm()
    {
        if (!is_initialized) throw std::logic_error("Vector is not initialized!");
        T sum = 0;
        for (size_t i = 0; i < n; ++i)
        {
            sum += data[i] * data[i];
        }
        return std::sqrt(sum);
    }

    T calculate_manhattan_norm()
    {
        if (!is_initialized) throw std::logic_error("Vector is not initialized!");
        T sum = 0;
        for (size_t i = 0; i < n; ++i)
        {
            sum += std::abs(data[i]);
        }
        return sum;
    }

    T calculate_dot_product(const Vector& other)
    {
        if (!is_initialized || !other.is_initialized)
            throw std::logic_error("Vectors are not initialized!");
        if (n != other.n) throw std::logic_error("Size mismatch for dot product!");
        T result = 0;
        for (size_t i = 0; i < n; ++i)
        {
            result += data[i] * other.data[i];
        }
        return result;
    }

    void parallel_sum(size_t num_threads)
    {
        if (!is_initialized) throw std::logic_error("Vector is not initialized!");

        std::vector<std::thread> threads;
        T result = 0;
        std::mutex result_mtx;

        auto worker = [&](size_t start, size_t end)
        {
            T local_sum = 0;
            for (size_t i = start; i < end; ++i)
            {
                local_sum += data[i];
            }
            std::lock_guard<std::mutex> lock(result_mtx);
            result += local_sum;
        };

        size_t chunk_size = n / num_threads;
        size_t remainder = n % num_threads;

        size_t start = 0;
        for (size_t i = 0; i < num_threads; ++i)
        {
            size_t end = start + chunk_size + (i < remainder ? 1 : 0);
            threads.emplace_back(worker, start, end);
            start = end;
        }

        for (auto& th : threads)
        {
            if (th.joinable()) th.join();
        }

        std::cout << "Parallel Sum: " << result << std::endl;
    }
};

int main()
{
    Vector<double> v(1000000);
    v.initialize_random(-10.0, 10.0);
    std::cout << "Min: " << v.find_min() << std::endl;
    std::cout << "Max: " << v.find_max() << std::endl;
    v.parallel_sum(4);
    return 0;
}
