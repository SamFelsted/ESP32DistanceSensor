#ifndef ESP32DISTANCESENSOR_MOVINGAVERAGE_H
#define ESP32DISTANCESENSOR_MOVINGAVERAGE_H
#include "vector"

class MovingAverage {
private:
    std::vector<double> buffer;
    int window_size;
    double sum;
    int index;
    int count;

public:
    MovingAverage(int size) : window_size(size), sum(0), index(0), count(0) {
        buffer.resize(size, 0.0);
    }

    double update(double new_value) {
        sum -= buffer[index];
        buffer[index] = new_value;
        sum += new_value;

        index = (index + 1) % window_size;
        count = std::min(count + 1, window_size);

        return sum / count;
    }
};
#endif //ESP32DISTANCESENSOR_MOVINGAVERAGE_H
