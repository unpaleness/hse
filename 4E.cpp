#include <functional>
#include <iostream>
#include <string>
#include <vector>

using Comparator = std::function<bool(const uint32_t, const uint32_t)>;

class BinaryHeap {
public:
    BinaryHeap(const std::vector<uint32_t>& newValues, const Comparator& newComparator)
        : values(newValues), comparator(newComparator) {}
    void onInsert() {
        ++n;
        if (n >= heap.size()) {
            heap.push_back(values.size() - 1);
        } else {
            heap[n] = values.size() - 1;
        }
        positions.push_back(n);
        siftUp(n);
    }
    size_t extractFirst() {
        const auto result = heap[1];
        std::swap(heap[1], heap[n]);
        std::swap(positions[heap[1]], positions[heap[n]]);
        --n;
        siftDown(1);
        return result;
    }
    void del(const uint32_t index) {
        const auto i = positions[index];
        heap[i] = heap[n];
        --n;
        positions[heap[i]] = i;
        siftUp(i);
        siftDown(i);
    }
    void print() const {
        for (size_t i = 1; i <= n; ++i) {
            std::cout << ' ' << values[heap[i]];
        }
        std::cout << '\n';
    }

private:
    void siftUp(size_t i) {
        while (i > 1 && !comparator(values[heap[i / 2]], values[heap[i]])) {
            std::swap(heap[i], heap[i / 2]);
            std::swap(positions[heap[i]], positions[heap[i / 2]]);
            i /= 2;
        }
    }
    void siftDown(size_t i) {
        while (true) {
            size_t l = 2 * i;
            if (l + 1 <= n && comparator(values[heap[l + 1]], values[heap[l]])) {
                ++l;
            }
            if (!(l <= n && comparator(values[heap[l]], values[heap[i]]))) {
                break;
            }
            std::swap(heap[l], heap[i]);
            std::swap(positions[heap[l]], positions[heap[i]]);
            i = l;
        }
    }

private:
    const std::vector<uint32_t>& values;  // link to vector with real data
    Comparator comparator;
    size_t n{0};
    std::vector<size_t> heap{0};    // heap of indexes of values
    std::vector<size_t> positions;  // for each i from values stores corresponding index in heap
};

class MultiSet {
public:
    MultiSet()
        : heapMin(new BinaryHeap(values, [](const uint32_t A, const uint32_t B) { return A < B; })),
          heapMax(new BinaryHeap(values, [](const uint32_t A, const uint32_t B) { return A > B; })) {}
    ~MultiSet() {
        delete heapMin;
        delete heapMax;
    }
    void insert(const uint32_t newValue) {
        values.push_back(newValue);
        heapMin->onInsert();
        heapMax->onInsert();
    }
    uint32_t getMin() const {
        const auto valueIndex = heapMin->extractFirst();
        heapMax->del(valueIndex);
        return values[valueIndex];
    }
    uint32_t getMax() const {
        const auto valueIndex = heapMax->extractFirst();
        heapMin->del(valueIndex);
        return values[valueIndex];
    }
    void print() const {
        heapMin->print();
        heapMax->print();
        // for (const auto val : values) {
        //     std::cout << ' ' << val;
        // }
        // std::cout << '\n';
    }

private:
    std::vector<uint32_t> values;  // vector with real data
    mutable BinaryHeap* heapMin;   // heap of indices to find minimum
    mutable BinaryHeap* heapMax;   // heap of indices to find maximum
};

int main() {
    uint32_t nOperations{0};
    std::cin >> nOperations;
    std::string operation;
    MultiSet* multiSet = new MultiSet();
    for (uint32_t i = 0; i < nOperations; ++i) {
        std::cin >> operation;
        // std::cout << operation << '\n';
        const auto braceOpenPos = operation.find('(');
        const auto command = operation.substr(0, braceOpenPos == std::string::npos ? operation.size() : braceOpenPos);
        if (command == "Insert") {
            const auto braceClosePos = operation.find(')');
            const auto value = std::stoul(operation.substr(braceOpenPos + 1, braceClosePos - braceOpenPos - 1));
            multiSet->insert(value);
        } else if (command == "GetMin") {
            std::cout << multiSet->getMin() << '\n';
        } else if (command == "GetMax") {
            std::cout << multiSet->getMax() << '\n';
        }
        // multiSet->print();
    }
    // for (const uint32_t val : {15, 16, 15, 14, 19, 12, 19, 15, 16, 10, 10, 11, 14, 19, 13, 10, 15, 12, 18, 14}) {
    //     minBinaryHeap->insert(val);
    //     minBinaryHeap->print();
    //     maxBinaryHeap->insert(val);
    //     maxBinaryHeap->print();
    // }
    // for (size_t i = 0; i < 10; ++i) {
    //     std::cout << minBinaryHeap->extractFirst() << '\n';
    //     minBinaryHeap->print();
    //     std::cout << maxBinaryHeap->extractFirst() << '\n';
    //     maxBinaryHeap->print();
    // }

    delete multiSet;
    return 0;
}
