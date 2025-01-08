#include "ObstacleAvoidance.h"

ObstacleAvoidance::ObstacleAvoidance()
    : device(pipelineManager.getPipeline(),pipelineManager.getDeviceInfo()){
    device.setIrLaserDotProjectorBrightness(1000);

    depthQueue = device.getOutputQueue("depth", 8, false);
    spatialCalcQueue = device.getOutputQueue("spatialData", 8, false);
    encoded = device.getOutputQueue("encoded", 30, true);

}

void ObstacleAvoidance::processFrame() {



    auto data = encoded->get<dai::ImgFrame>();
    auto encodedData = data->getData();
    QByteArray byteArray(reinterpret_cast<const char*>(encodedData.data()), encodedData.size());
    emit encodedStreamData(byteArray);

    qDebug() << "--> ObstacleAvoidance::processFrame: " << byteArray.size();

    auto inDepth = depthQueue->get<dai::ImgFrame>();
    cv::Mat depthFrame = inDepth->getFrame();

    // Normalize and apply color map
    cv::Mat depthFrameColor;
    cv::normalize(depthFrame, depthFrameColor, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    cv::applyColorMap(depthFrameColor, depthFrameColor, cv::COLORMAP_HOT);

    auto spatialData = spatialCalcQueue->get<dai::SpatialLocationCalculatorData>()->getSpatialLocations();
    movingLogic.processSpatialData(spatialData, depthFrameColor.cols, depthFrameColor.rows);

    //drawROIs(depthFrameColor, spatialData);

    // Display action decision
    auto action = movingLogic.decideAction();
    //std::cout << "Action: " << action << std::endl;
    QByteArray actionDataBuffer = QString::fromStdString(action).toUtf8();
    emit movingAction(actionDataBuffer);
    sendDistanceGrid();
    // Log the distanceGrid
    // logDistanceGrid();

    // Show the frame
    //cv::imshow("depth", depthFrameColor);
}

void ObstacleAvoidance::drawROIs(cv::Mat& frame, const std::vector<dai::SpatialLocations>& spatialData) {
    for (const auto& data : spatialData) {
        auto roi = data.config.roi.denormalize(frame.cols, frame.rows);
        auto coords = data.spatialCoordinates;
        float distance = coords.z;

        int xmin = static_cast<int>(roi.topLeft().x);
        int ymin = static_cast<int>(roi.topLeft().y);
        int xmax = static_cast<int>(roi.bottomRight().x);
        int ymax = static_cast<int>(roi.bottomRight().y);

        cv::Scalar color = (distance / 2000.0f < 1.0f) ? cv::Scalar(0, 0, 255) : cv::Scalar(0, 255, 0);
        cv::rectangle(frame, cv::Rect(cv::Point(xmin, ymin), cv::Point(xmax, ymax)), color, 2);
    }
}


void ObstacleAvoidance::logDistanceGrid() {
    // Log the distanceGrid (from movingLogic)
    const auto& distanceGrid = movingLogic.getDistanceGrid();
    std::cout << "[10x10]" << std::endl;
    for (int i = 0; i < 10; i++) {
        std::cout << "[ ";
        for (int j = 0; j < 10; j++) {
            float distanceInMeters = distanceGrid[i][j] / 1000.0f;
            std::cout << std::fixed << std::setprecision(1) << distanceInMeters << " ";
        }
        std::cout << "]"  << std::endl;
    }
    std::cout << std::endl;
}
void ObstacleAvoidance::sendDistanceGrid(){
    const auto& distanceGrid = movingLogic.getDistanceGrid();
    QStringList dataList;
    for (const auto& row : distanceGrid) {
        for (float distance : row) {
            float distanceInMeters = distance / 1000.0f;
            dataList << QString::number(distanceInMeters, 'f', 2); // Chuyển khoảng cách thành chuỗi, với 1 chữ số thập phân
        }
    }
    QString dataString = dataList.join(';'); // Ghép các giá trị bằng dấu ';'

    // Tạo thông điệp theo định dạng <CMD><DATA>
    // QString message = QString("788%1").arg(dataString);
    // Gửi thông điệp qua WebSocket
    //qDebug() << message;
    QByteArray distanceDataBuffer = dataString.toUtf8();
    emit distanceDataGrid(distanceDataBuffer);

}

void ObstacleAvoidance::run() {

    Dưới đây là các file được chỉnh sửa đầy đủ với cơ chế xử lý lỗi khi mất kết nối thiết bị:

                                                                                               ObstacleAvoidance.cpp
                                                                                                   cpp
                                                                                                       Copy code
#include "ObstacleAvoidance.h"

                                                                                                       ObstacleAvoidance::ObstacleAvoidance()
        : device(pipelineManager.getPipeline(), pipelineManager.getDeviceInfo()) {
        device.setIrLaserDotProjectorBrightness(1000);

        depthQueue = device.getOutputQueue("depth", 8, false);
        spatialCalcQueue = device.getOutputQueue("spatialData", 8, false);
        encoded = device.getOutputQueue("encoded", 30, true);
    }

    void ObstacleAvoidance::processFrame() {
        try {
            auto data = encoded->get<dai::ImgFrame>();
            auto encodedData = data->getData();
            QByteArray byteArray(reinterpret_cast<const char*>(encodedData.data()), encodedData.size());
            emit encodedStreamData(byteArray);

            qDebug() << "--> ObstacleAvoidance::processFrame: " << byteArray.size();

            auto inDepth = depthQueue->get<dai::ImgFrame>();
            cv::Mat depthFrame = inDepth->getFrame();

            // Normalize and apply color map
            cv::Mat depthFrameColor;
            cv::normalize(depthFrame, depthFrameColor, 0, 255, cv::NORM_MINMAX, CV_8UC1);
            cv::applyColorMap(depthFrameColor, depthFrameColor, cv::COLORMAP_HOT);

            auto spatialData = spatialCalcQueue->get<dai::SpatialLocationCalculatorData>()->getSpatialLocations();
            movingLogic.processSpatialData(spatialData, depthFrameColor.cols, depthFrameColor.rows);

            auto action = movingLogic.decideAction();
            QByteArray actionDataBuffer = QString::fromStdString(action).toUtf8();
            emit movingAction(actionDataBuffer);
            sendDistanceGrid();
        } catch (const std::runtime_error& e) {
            qDebug() << "[ERROR] " << e.what();
            emit errorOccurred(QString::fromStdString(e.what()));
        }
    }

    void ObstacleAvoidance::run() {
        while (true) {
            try {
                processFrame();
            } catch (const std::runtime_error& e) {
                qDebug() << "[FATAL ERROR] Device connection lost: " << e.what();
                // Thử kết nối lại
                try {
                    device = dai::Device(pipelineManager.getPipeline(), pipelineManager.getDeviceInfo());
                    depthQueue = device.getOutputQueue("depth", 8, false);
                    spatialCalcQueue = device.getOutputQueue("spatialData", 8, false);
                    encoded = device.getOutputQueue("encoded", 30, true);
                    qDebug() << "Device reconnected successfully.";
                } catch (const std::runtime_error& reconnectError) {
                    qDebug() << "[ERROR] Failed to reconnect: " << reconnectError.what();
                    break; // Thoát khỏi vòng lặp
                }
            }
            QThread::msleep(300);
        }
}
