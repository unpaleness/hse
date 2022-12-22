#include "optimization.h"

#include <algorithm>
#include <map>
#include <set>
#include <vector>

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

void output(const std::map<int, int>& distances, const int nRaces) {
    if (nRaces <= 0) {
        return;
    }
    const auto distance = distances.rbegin()->first;
    for (int i = 0; i < nRaces; ++i) {
        writeInt(distance, '\n');
    }
}

void addDistance(std::map<int, int>& distances, const int newDistance) {
    if (auto distance = distances.find(newDistance); distance != distances.end()) {
        ++(distance->second);
    } else {
        distances.insert({newDistance, 1});
    }
}

void removeDistance(std::map<int, int>& distances, const int newDistance) {
    auto distance = distances.find(newDistance);
    if (distance->second > 1) {
        --(distance->second);
    } else {
        distances.erase(distance);
    }
}

int main() {
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

    std::sort(events.begin(), events.end());

    int yPrev{0};
    int typePrev{0};
    std::set<int> stops{0, X};             // current state of stops on race
    std::map<int, int> distances{{X, 1}};  // map with current distance fragments and their amount
    int xPrev{0};
    int xNext{0};
    // scan events
    for (const auto& event : events) {
        if (event.y != yPrev) {
            if (typePrev == 0) {
                if (event.type == 1) {
                    // writeWord("recalculate and output distances on [yPrev; event.y]\n");
                    output(distances, event.y - yPrev + 1);
                } else {
                    // writeWord("recalculate and output distances on [yPrev; event.y)\n");
                    output(distances, event.y - yPrev);
                }
            } else {
                if (event.type == 1) {
                    // writeWord("recalculate and output distances on (yPrev; event.y]\n");
                    output(distances, event.y - yPrev);
                } else {
                    // writeWord("recalculate and output distances on (yPrev; event.y)\n");
                    output(distances, event.y - yPrev - 1);
                }
            }
        } else {
            if (typePrev == 0 && event.type == 1) {
                // writeWord("recalculate and output distances on [event.y]\n");
                output(distances, 1);
            }
        }
        if (event.type == 0) {
            auto it = stops.insert(event.x).first;
            auto itPrev = it;
            auto itNext = it;
            xPrev = *(--itPrev);
            xNext = *(++itNext);
            removeDistance(distances, xNext - xPrev);
            addDistance(distances, event.x - xPrev);
            addDistance(distances, xNext - event.x);
        } else {
            auto it = stops.find(event.x);
            auto itPrev = it;
            auto itNext = it;
            xPrev = *(--itPrev);
            xNext = *(++itNext);
            stops.erase(it);
            addDistance(distances, xNext - xPrev);
            removeDistance(distances, event.x - xPrev);
            removeDistance(distances, xNext - event.x);
        }
        typePrev = event.type;
        yPrev = event.y;
    }

    // if some races without station remains at the end
    output(distances, Y - yPrev);

    return 0;
}
