#ifndef DISPLAY_MANAGER_HPP
#define DISPLAY_MANAGER_HPP

#include "PipelineManager.h"
#include "MovingLogic.h"
#include <opencv2/opencv.hpp>
#include <QString>
#include <QDebug>
#include <QObject>
#include <QThread>

class ObstacleAvoidance : public QObject {
    Q_OBJECT
private:
    PipelineManager pipelineManager;
    MovingLogic movingLogic;

    dai::Device device;
    std::shared_ptr<dai::DataOutputQueue> depthQueue;
    std::shared_ptr<dai::DataOutputQueue> spatialCalcQueue;
    std::shared_ptr<dai::DataOutputQueue> encoded;

    void processFrame();
    void drawROIs(cv::Mat& frame, const std::vector<dai::SpatialLocations>& spatialData);
    void logDistanceGrid();
    void sendDistanceGrid();

signals:
    void encodedStreamData(const QByteArray &encodedData);
    void movingAction(const QByteArray &movingData);
    void distanceGrid(const QByteArray &distanceData);

public:
    ObstacleAvoidance();
    void run();
};

#endif
