#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using Comparator = std::function<bool(const uint32_t, const uint32_t)>;

class BinaryHeap {
public:
    BinaryHeap(const Comparator& newComparator) : comparator(newComparator) {}
    void insert(const uint32_t newValue) {
        values.push_back(newValue);
        heap.push_back(values.size() - 1);
        siftUp(heap.size() - 1);
    }
    uint32_t extractFirst() {
        const auto result = values[heap[1]];
        heap[1] = heap.back();
        heap.pop_back();
        siftDown(1);
        return result;
    }
    void print() const {
        for (size_t i = 1; i < heap.size(); ++i) {
            std::cout << ' ' << values[heap[i]];
        }
        std::cout << '\n';
    }

private:
    void siftUp(size_t i) {
        while (i > 1 && !comparator(values[heap[i / 2]], values[heap[i]])) {
            std::swap(heap[i], heap[i / 2]), i /= 2;
        }
    }
    void siftDown(size_t i) {
        while (true) {
            size_t l = 2 * i;
            if (l + 1 <= heap.size() - 1 && comparator(values[heap[l + 1]], values[heap[l]])) {
                ++l;
            }
            if (!(l <= heap.size() - 1 && comparator(values[heap[l]], values[heap[i]]))) {
                break;
            }
            std::swap(heap[l], heap[i]);
            i = l;
        }
    }

private:
    std::vector<size_t> heap{0};   // heap of indexes
    std::vector<uint32_t> values;  // vector with real data
    Comparator comparator;
};

int main() {
    uint32_t nOperations{0};
    std::cin >> nOperations;
    std::string operation;
    auto minBinaryHeap = std::make_shared<BinaryHeap>([](const uint32_t A, const uint32_t B) -> bool { return A < B; });
    auto maxBinaryHeap = std::make_shared<BinaryHeap>([](const uint32_t A, const uint32_t B) -> bool { return A > B; });
    for (uint32_t i = 0; i < nOperations; ++i) {
        std::cin >> operation;
        const auto braceOpenPos = operation.find('(');
        const auto command = operation.substr(0, braceOpenPos == std::string::npos ? operation.size() : braceOpenPos);
        if (command == "Insert") {
            const auto braceClosePos = operation.find(')');
            const auto value = std::stoul(operation.substr(braceOpenPos + 1, braceClosePos - braceOpenPos - 1));
            minBinaryHeap->insert(value);
            maxBinaryHeap->insert(value);
        } else if (command == "GetMin") {
            std::cout << minBinaryHeap->extractFirst() << '\n';
        } else if (command == "GetMax") {
            std::cout << maxBinaryHeap->extractFirst() << '\n';
        }
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

    return 0;
}
