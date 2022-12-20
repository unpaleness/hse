#include "optimization.h"

#include <set>
#include <vector>

int main() {
    const auto n = readInt<uint32_t>();
    const auto Y = readInt<uint32_t>();
    const auto X = readInt<uint32_t>();
    uint32_t point;
    uint32_t indexRaceStart;
    uint32_t indexRaceEnd;

    // fill stations
    std::set<uint32_t>* stations = new std::set<uint32_t>[X + 1];
    for (uint32_t i = 0; i < n; ++i) {
        point = readInt<uint32_t>();
        indexRaceStart = readInt<uint32_t>();
        indexRaceEnd = readInt<uint32_t>();
        stations[point].insert(indexRaceStart);
        stations[point].insert(indexRaceEnd);
    }

    // scan race stops and calculate distance
    std::vector<uint32_t> distances(Y + 1);
    std::vector<uint32_t> last_stops(Y + 1);
    for (uint32_t i = 0; i < X + 1; ++i) {
        if (stations[i].empty()) {
            continue;
        }
        for (auto it = stations[i].begin(); it != stations[i].end(); ++it) {
            const auto iRaceBegin = *it;
            const auto iRaceEnd = *(++it);
            for (auto j = iRaceBegin; j <= iRaceEnd; ++j) {
                const auto tmpDistance = i - last_stops[j];
                if (tmpDistance > distances[j]) {
                    distances[j] = tmpDistance;
                }
                last_stops[j] = i;
            }
        }
    }

    // finalize races
    for (uint32_t i = 0; i < Y + 1; ++i) {
        const auto tmpDistance = X - last_stops[i];
        if (tmpDistance > distances[i]) {
            writeInt(tmpDistance, '\n');
        } else {
            writeInt(distances[i], '\n');
        }
    }

    delete[] stations;

    return 0;
}
