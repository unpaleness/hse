#include "optimization.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <set>
#include <vector>

int nCalcs{0};

struct Event {
    int type{0};  // 0 - begin of station, 1 - end of station
    int x{0};
    int y{0};

    bool operator<(const Event& other) const {
        if (y != other.y) {
            return y < other.y;
        }
        if (type != other.type) {
            return type < other.type;
        }
        return x < other.x;
    }
};

void calculateAndOutput(const std::set<int>& stops, const int nRaces) {
    if (nRaces <= 0) {
        return;
    }
    ++nCalcs;
    int distance{0};
    int prevStop{0};
    for (const auto stop : stops) {
        const auto tmpDistance = stop - prevStop;
        if (tmpDistance > distance) {
            distance = tmpDistance;
        }
        prevStop = stop;
    }
    for (int i = 0; i < nRaces; ++i) {
        writeInt(distance, '\n');
    }
}

int main() {
    const auto t1 = std::chrono::system_clock::now();

    const auto n = readInt<int>();
    const auto Y = readInt<int>();
    const auto X = readInt<int>();

    // fill station events
    std::vector<Event> events;
    events.reserve(n * 2);
    for (int i = 0; i < n; ++i) {
        const auto x = readInt<int>();
        const auto y1 = readInt<int>();
        const auto y2 = readInt<int>();
        events.push_back({0, x, y1});
        events.push_back({1, x, y2});
    }

    const auto t2 = std::chrono::system_clock::now();

    std::sort(events.begin(), events.end());

    const auto t3 = std::chrono::system_clock::now();

    int yPrev = 0;
    int typePrev = 0;
    std::set<int> stops{X};  // current state of stops on race (initialize with the last stop on finish)
    // scan events
    for (const auto& event : events) {
        // writeInt(event.y, ' ');
        // writeInt(event.x, ' ');
        // writeInt(event.type, '\n');
        if (event.y != yPrev) {
            if (typePrev == 0) {
                if (event.type == 1) {
                    // writeWord("recalculate and output distances on [yPrev; event.y]\n");
                    calculateAndOutput(stops, event.y - yPrev + 1);
                } else {
                    // writeWord("recalculate and output distances on [yPrev; event.y)\n");
                    calculateAndOutput(stops, event.y - yPrev);
                }
            } else {
                if (event.type == 1) {
                    // writeWord("recalculate and output distances on (yPrev; event.y]\n");
                    calculateAndOutput(stops, event.y - yPrev);
                } else {
                    // writeWord("recalculate and output distances on (yPrev; event.y)\n");
                    calculateAndOutput(stops, event.y - yPrev - 1);
                }
            }
        } else {
            if (typePrev == 0 && event.type == 1) {
                // writeWord("recalculate and output distances on [event.y]\n");
                calculateAndOutput(stops, 1);
            }
        }
        if (event.type == 0) {
            stops.insert(event.x);
        } else {
            stops.erase(event.x);
        }
        typePrev = event.type;
        yPrev = event.y;
    }

    // if some races without station remains at the end
    calculateAndOutput(stops, Y - yPrev);

    const auto t4 = std::chrono::system_clock::now();

    std::cerr << "Read: " << (t2 - t1).count() / 1000 << "ms\n";
    std::cerr << "Sort: " << (t3 - t2).count() / 1000 << "ms\n";
    std::cerr << "Calc and output: " << (t4 - t3).count() / 1000 << "ms\n";
    std::cerr << "Total: " << (t4 - t1).count() / 1000 << "ms\n";
    std::cerr << "Calculations count: " << nCalcs << '\n';

    return 0;
}
