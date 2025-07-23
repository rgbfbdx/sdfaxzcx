#include <iostream>
#include <fstream>
#include <cstring>

class Reservoir {
private:
    char* name;
    char* type;
    double width;
    double length;
    double maxDepth;

public:
    explicit Reservoir(const char* n = "Без назви", const char* t = "Ставок", double w = 0, double l = 0, double d = 0)
        : width(w), length(l), maxDepth(d) {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
        type = new char[strlen(t) + 1];
        strcpy(type, t);
    }

    Reservoir(const Reservoir& other)
        : width(other.width), length(other.length), maxDepth(other.maxDepth) {
        name = new char[strlen(other.name) + 1];
        strcpy(name, other.name);
        type = new char[strlen(other.type) + 1];
        strcpy(type, other.type);
    }

    Reservoir& operator=(const Reservoir& other) {
        if (this != &other) {
            delete[] name;
            delete[] type;
            width = other.width;
            length = other.length;
            maxDepth = other.maxDepth;

            name = new char[strlen(other.name) + 1];
            strcpy(name, other.name);
            type = new char[strlen(other.type) + 1];
            strcpy(type, other.type);
        }
        return *this;
    }

    ~Reservoir() {
        delete[] name;
        delete[] type;
    }

    double volume() const {
        return width * length * maxDepth;
    }

    double area() const {
        return width * length;
    }

    bool sameType(const Reservoir& other) const {
        return strcmp(type, other.type) == 0;
    }

    const char* getName() const { return name; }
    const char* getType() const { return type; }
    double getWidth() const { return width; }
    double getLength() const { return length; }
    double getDepth() const { return maxDepth; }

    void setName(const char* n) {
        delete[] name;
        name = new char[strlen(n) + 1];
        strcpy(name, n);
    }

    void setType(const char* t) {
        delete[] type;
        type = new char[strlen(t) + 1];
        strcpy(type, t);
    }

    void setWidth(double w) { width = w; }
    void setLength(double l) { length = l; }
    void setDepth(double d) { maxDepth = d; }

    void print() const {
        std::cout << "Назва: " << name << "\nТип: " << type
                  << "\nШирина: " << width << "\nДовжина: " << length
                  << "\nГлибина: " << maxDepth
                  << "\nПлоща: " << area()
                  << "\nОбсяг: " << volume() << "\n\n";
    }

    bool compareArea(const Reservoir& other) const {
        return sameType(other) && area() > other.area();
    }

    void writeText(std::ofstream& out) const {
        out << name << "\n" << type << "\n" << width << "\n" << length << "\n" << maxDepth << "\n";
    }

    void writeBinary(std::ofstream& out) const {
        size_t nameLen = strlen(name);
        size_t typeLen = strlen(type);
        out.write((char*)&nameLen, sizeof(nameLen));
        out.write(name, nameLen);
        out.write((char*)&typeLen, sizeof(typeLen));
        out.write(type, typeLen);
        out.write((char*)&width, sizeof(width));
        out.write((char*)&length, sizeof(length));
        out.write((char*)&maxDepth, sizeof(maxDepth));
    }
};

class ReservoirArray {
private:
    Reservoir* arr;
    int size;

public:
    ReservoirArray() : arr(nullptr), size(0) {}

    ~ReservoirArray() {
        delete[] arr;
    }

    void add(const Reservoir& r) {
        Reservoir* newArr = new Reservoir[size + 1];
        for (int i = 0; i < size; ++i)
            newArr[i] = arr[i];
        newArr[size] = r;
        delete[] arr;
        arr = newArr;
        ++size;
    }

    void remove(int index) {
        if (index < 0 || index >= size) return;
        Reservoir* newArr = new Reservoir[size - 1];
        for (int i = 0, j = 0; i < size; ++i)
            if (i != index) newArr[j++] = arr[i];
        delete[] arr;
        arr = newArr;
        --size;
    }

    void printAll() const {
        for (int i = 0; i < size; ++i) {
            std::cout << "Водойма #" << i + 1 << ":\n";
            arr[i].print();
        }
    }

    void saveToTextFile(const char* filename) const {
        std::ofstream out(filename);
        for (int i = 0; i < size; ++i)
            arr[i].writeText(out);
        out.close();
    }

    void saveToBinaryFile(const char* filename) const {
        std::ofstream out(filename, std::ios::binary);
        for (int i = 0; i < size; ++i)
            arr[i].writeBinary(out);
        out.close();
    }

    int getSize() const { return size; }
};

int main() {
    ReservoirArray list;

    Reservoir r1("Озеро Світязь", "Озеро", 1000, 1500, 58);
    Reservoir r2("Азовське море", "Море", 20000, 100000, 14);
    Reservoir r3("Дніпро", "Річка", 800, 10000, 25);

    list.add(r1);
    list.add(r2);
    list.add(r3);

    list.printAll();

    list.remove(1);

    std::cout << "Після видалення:\n";
    list.printAll();

    list.saveToTextFile("reservoirs.txt");
    list.saveToBinaryFile("reservoirs.bin");

    return 0;
}
