#ifndef LOGIC_MANAGER_HPP
#define LOGIC_MANAGER_HPP


#include <vector>
#include <string>

#include "depthai/depthai.hpp"

class MovingLogic {
private:
    static constexpr int GRID_SIZE = 10;
    // Action definitions
    static constexpr const char* ACTION_DOWN = "787003;0.1";
    static constexpr const char* ACTION_UP = "787001;0.1";
    std::vector<std::vector<int>> grid;
    std::vector<std::vector<float>> distanceGrid;
    void resetDistanceGrid();
    void resetGrid();

public:
    MovingLogic();
    void processSpatialData(const std::vector<dai::SpatialLocations>& spatialData, int frameWidth, int frameHeight);
    std::string decideAction() const;
    const std::vector<std::vector<float>>& getDistanceGrid() const;
};

#endif
