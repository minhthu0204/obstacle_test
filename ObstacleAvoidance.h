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
    bool isTurning = true;
    int retryCount = 0;          // Đếm số lần thử kết nối
    const int maxRetries = 5;    // Số lần thử kết nối tối đa
    const int reconnectDelay = 500; // Thời gian chờ giữa các lần tái kết nối (ms)

    std::unique_ptr<dai::Device> device;

    std::shared_ptr<dai::DataOutputQueue> depthQueue;
    std::shared_ptr<dai::DataOutputQueue> spatialCalcQueue;
    std::shared_ptr<dai::DataOutputQueue> encoded;

    void processFrame();
    void drawROIs(cv::Mat& frame, const std::vector<dai::SpatialLocations>& spatialData);
    void logDistanceGrid();
    void sendDistanceGrid();

    void createDevice();


signals:
    void encodedStreamData(QByteArray encodedData);
    void movingAction(const QByteArray &movingData);
    void distanceDataGrid(const QByteArray &distanceData);

public:
    ObstacleAvoidance();



public slots:
    void run();
};

#endif
