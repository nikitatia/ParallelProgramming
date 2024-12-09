#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Класс GpuSpec: сохраняет сведения о GPU
class GpuSpec 
{
public:
    std::string model;
    int cores;
    float frequency;

    void Print() const 
    {
        std::cout << "GPU: Model: " << model << ", Cores: " << cores << ", Frequency: " << frequency << " GHz\n";
    }

    void Import(std::ifstream& file) 
    {
        file >> model >> cores >> frequency;
    }

    void Export(std::ofstream& file) const 
    {
        file << model << " " << cores << " " << frequency << "\n";
    }
};

// Класс CpuSpec: сохраняет сведения о CPU
class CpuSpec 
{
public:
    std::string model;
    int cores;
    float frequency;

    void Print() const 
    {
        std::cout << "CPU: Model: " << model << ", Cores: " << cores << ", Frequency: " << frequency << " GHz\n";
    }

    void Import(std::ifstream& file) 
    {
        file >> model >> cores >> frequency;
    }

    void Export(std::ofstream& file) const 
    {
        file << model << " " << cores << " " << frequency << "\n";
    }
};

// Класс RamSpec: сохраняет сведения о RAM
class RamSpec 
{
public:
    int capacity;
    float frequency;

    void Print() const 
    {
        std::cout << "RAM: Capacity: " << capacity << " GB, Frequency: " << frequency << " MHz\n";
    }

    void Import(std::ifstream& file) 
    {
        file >> capacity >> frequency;
    }

    void Export(std::ofstream& file) const 
    {
        file << capacity << " " << frequency << "\n";
    }
};

// Класс LanSpec: сохраняет сведения о сети
class LanSpec 
{
public:
    float bandwidth;
    std::string type;

    void Print() const 
    {
        std::cout << "LAN: Bandwidth: " << bandwidth << " Gbps, Type: " << type << "\n";
    }

    void Import(std::ifstream& file) 
    {
        file >> bandwidth >> type;
    }

    void Export(std::ofstream& file) const 
    {
        file << bandwidth << " " << type << "\n";
    }
};

// Класс ClusterNode: моделирует узел кластера
class ClusterNode 
{
public:
    GpuSpec gpu;
    CpuSpec cpu;
    RamSpec ram;
    LanSpec lan;

    void Print() const 
    {
        std::cout << "Cluster Node:\n";
        gpu.Print();
        cpu.Print();
        ram.Print();
        lan.Print();
    }

    void Import(std::ifstream& file) 
    {
        gpu.Import(file);
        cpu.Import(file);
        ram.Import(file);
        lan.Import(file);
    }

    void Export(std::ofstream& file) const 
    {
        gpu.Export(file);
        cpu.Export(file);
        ram.Export(file);
        lan.Export(file);
    }
};

// Класс Cluster: моделирует структуру кластера
class Cluster 
{
public:
    std::vector<ClusterNode> nodes;

    void Print() const 
    {
        std::cout << "Cluster:\n";
        for (size_t i = 0; i < nodes.size(); ++i) 
        {
            std::cout << "Node " << i + 1 << ":\n";
            nodes[i].Print();
        }
    }

    void Import(const std::string& filename) 
    {
        std::ifstream file(filename);
        if (!file.is_open()) 
        {
            std::cerr << "Error: Unable to open file for importing.\n";
            return;
        }

        int nodeCount;
        file >> nodeCount;
        nodes.resize(nodeCount);

        for (auto& node : nodes) 
        {
            node.Import(file);
        }
        file.close();
    }

    void Export(const std::string& filename) const 
    {
        std::ofstream file(filename);
        if (!file.is_open()) 
        {
            std::cerr << "Error: Unable to open file for exporting.\n";
            return;
        }

        file << nodes.size() << "\n";
        for (const auto& node : nodes) 
        {
            node.Export(file);
        }
        file.close();
    }
};

// Функция для тестирования
void Test() 
{
    Cluster cluster;

    // Создаём узел 1
    ClusterNode node1;
    node1.gpu = { "RTX4090", 16384, 2.5f };
    node1.cpu = { "Ryzen9-7950X", 16, 4.5f };
    node1.ram = { 64, 5200.0f };
    node1.lan = { 25.0f, "Ethernet" };

    // Создаём узел 2
    ClusterNode node2;
    node2.gpu = { "RTX3060", 3584, 1.8f };
    node2.cpu = { "Intel i5-12600K", 10, 3.7f };
    node2.ram = { 32, 3200.0f };
    node2.lan = { 10.0f, "WiFi" };

    cluster.nodes.push_back(node1);
    cluster.nodes.push_back(node2);

    cluster.Export("cluster_data.txt");
    cluster.Print();

    Cluster importedCluster;
    importedCluster.Import("cluster_data.txt");

    std::cout << "\nImported Cluster:\n";
    importedCluster.Print();
}

// Точка входа
int main() 
{
    Test();
    return 0;
}
