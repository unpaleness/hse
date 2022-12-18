#include "optimization.h"

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

private:
    std::vector<uint32_t> values;  // vector with real data
    mutable BinaryHeap* heapMin;   // heap of indices to find minimum
    mutable BinaryHeap* heapMax;   // heap of indices to find maximum
};

int main() {
    auto nOperations = readInt();
    char s[101];
    MultiSet* multiSet = new MultiSet();
    for (int i = 0; i < nOperations; ++i) {
        readWord(s);
        const std::string operation{s};
        const auto braceOpenPos = operation.find('(');
        const auto command = operation.substr(0, braceOpenPos == std::string::npos ? operation.size() : braceOpenPos);
        if (command == "Insert") {
            const auto braceClosePos = operation.find(')');
            const auto value = std::stoul(operation.substr(braceOpenPos + 1, braceClosePos - braceOpenPos - 1));
            multiSet->insert(value);
        } else if (command == "GetMin") {
            writeInt(multiSet->getMin(), '\n');
        } else if (command == "GetMax") {
            writeInt(multiSet->getMax(), '\n');
        }
    }

    delete multiSet;
    return 0;
}
