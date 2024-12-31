#ifndef DISPLAY_MANAGER_HPP
#define DISPLAY_MANAGER_HPP

#include "PipelineManager.h"
#include "LogicManager.h"
#include "WebSocketClient.h"
#include <opencv2/opencv.hpp>
#include <QString>
#include <QDebug>
#include <QObject>
#include <QThread>

class DisplayManager : public QObject {
    Q_OBJECT
private:
    PipelineManager pipelineManager;
    LogicManager logicManager;
    WebSocketClient webSocketClient;

    dai::Device device;
    std::shared_ptr<dai::DataOutputQueue> depthQueue;
    std::shared_ptr<dai::DataOutputQueue> spatialCalcQueue;
    std::shared_ptr<dai::DataOutputQueue> encoded;

    void processFrame();
    void drawROIs(cv::Mat& frame, const std::vector<dai::SpatialLocations>& spatialData);
    void logDistanceGrid();
    void sendDistanceGrid();

signals:
    void encodedDataReady(const QByteArray &encodedData);

public:
    DisplayManager();
    void run();
};

#endif
