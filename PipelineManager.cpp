#include "PipelineManager.h"

PipelineManager::PipelineManager()
{
    monoLeft = pipeline.create<dai::node::MonoCamera>();
    monoRight = pipeline.create<dai::node::MonoCamera>();
    stereo = pipeline.create<dai::node::StereoDepth>();
    spatialLocationCalculator = pipeline.create<dai::node::SpatialLocationCalculator>();
    xoutDepth = pipeline.create<dai::node::XLinkOut>();
    xoutSpatialData = pipeline.create<dai::node::XLinkOut>();
    xinSpatialCalcConfig = pipeline.create<dai::node::XLinkIn>();
    colorCam = pipeline.create<dai::node::ColorCamera>();
    videnc = pipeline.create<dai::node::VideoEncoder>();
    veOut = pipeline.create<dai::node::XLinkOut>();

    configureNodes();
}

void PipelineManager::configureNodes(){
    //Color Camera
    const int FPS = 30;

    // Init and configure color camera
    colorCam->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);
    colorCam->setInterleaved(false);
    colorCam->setColorOrder(dai::ColorCameraProperties::ColorOrder::BGR);
    colorCam->setFps(FPS);

    // Create VideoEncoder
    videnc->setDefaultProfilePreset(FPS, dai::VideoEncoderProperties::Profile::H265_MAIN);
    colorCam->video.link(videnc->input);

    // XLinkOut to export video data
    veOut->setStreamName("encoded");
    videnc->bitstream.link(veOut->input);






    // Configure Mono Cameras
    monoLeft->setResolution(dai::MonoCameraProperties::SensorResolution::THE_400_P);
    monoLeft->setCamera("left");
    monoRight->setResolution(dai::MonoCameraProperties::SensorResolution::THE_400_P);
    monoRight->setCamera("right");

    // Configure StereoDepth
    stereo->setDefaultProfilePreset(dai::node::StereoDepth::PresetMode::HIGH_DENSITY);
    stereo->setLeftRightCheck(false);
    stereo->setSubpixel(false);

    // Configure SpatialLocationCalculator
    spatialLocationCalculator->inputConfig.setWaitForMessage(false);

    // Configure XLinkOut
    xoutDepth->setStreamName("depth");
    xoutSpatialData->setStreamName("spatialData");
    xinSpatialCalcConfig->setStreamName("spatialCalcConfig");

    // Link nodes
    monoLeft->out.link(stereo->left);
    monoRight->out.link(stereo->right);
    stereo->depth.link(spatialLocationCalculator->inputDepth);
    spatialLocationCalculator->passthroughDepth.link(xoutDepth->input);
    spatialLocationCalculator->out.link(xoutSpatialData->input);
    xinSpatialCalcConfig->out.link(spatialLocationCalculator->inputConfig);

    // Add 100 ROIs (10x10 grid)
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            dai::SpatialLocationCalculatorConfigData config;
            config.depthThresholds.lowerThreshold = 200;
            config.depthThresholds.upperThreshold = 30000;
            config.calculationAlgorithm = dai::SpatialLocationCalculatorAlgorithm::MEDIAN;
            config.roi = dai::Rect(
                dai::Point2f(i * 0.1f, j * 0.1f),
                dai::Point2f((i + 1) * 0.1f, (j + 1) * 0.1f)
                );
            spatialLocationCalculator->initialConfig.addROI(config);
        }
    }
}

dai::Pipeline PipelineManager::getPipeline() const {
    return pipeline;
}
