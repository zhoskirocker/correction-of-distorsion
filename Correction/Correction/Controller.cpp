#include "GraphicsScene.h"
#include "Controller.h"
#include "DataTransfer.h"
#include "Model.h"
#include "ImageDisplay.h"
#include "AlgorithmsImages.h"

#include <opencv2/highgui/highgui.hpp>


Controller::Controller(Model* model, GraphicsScene* scene)
{
	model_ = model;
	scene_ = scene;
	connect(scene, &GraphicsScene::nodePosChangedS, model, &Model::setNodePosition);
	connect(model, &Model::sendProgressS, this, &Controller::sendProgressS);
	connect(model, &Model::operationfinishedS, this, [&] {sendProgressS(0); });
	connect(this, &Controller::doOperationS, model, &Model::doOperation);
}

Controller::~Controller()
{

}

void Controller::loadImage()
{
	cv::Mat cvImage;
	DataTransfer::loadCvImage(NULL, cvImage);
	if (cvImage.empty())
		return;
	scene_->deleteImageBlocks();
	model_->setImage(cvImage);
	ImageDisplay imageDisplay;
	createVisualImageBlocks(cvImage, imageDisplay);
	scene_->addImageBlocks(imageDisplay);
}

void Controller::createVisualImageBlocks(const cv::Mat& cvImage, ImageDisplay& imageDisplay)
{
	Q_ASSERT(cvImage.size > 0);
	const int c_block_width = 2000;
	const int c_block_height = 2000;
	AlgorithmsImages::createVisualImageBlocks(cvImage, c_block_width, c_block_height, imageDisplay);
}

//void Controller::findNodesApproximately(int rows_count, int cols_count)
//{
//	model_->findNodesApproximately(rows_count,  cols_count);
//	scene_->addNodesItems(model_->getNodesVisual());
//}
//
//void Controller::findNodesAccurately(int rows_count, int cols_count)
//{
//	model_->findNodesAccurately( rows_count,  cols_count);
//	scene_->addNodesItems(model_->getNodesVisual());
//}
//
//void Controller::calculateCorrectionTable()
//{
//	emit doOperationS(OPERATION_WRITE_CORRECTION_TABLE);
//}

void Controller::doOperation(Operation operation, const QVariantList& params)
{
	switch (operation)
	{

	case OPERATION_FIND_NODES_APPROX:

		emit doOperationS(OPERATION_FIND_NODES_APPROX, params);
		scene_->addNodesItems(model_->getNodesVisual());
		break;

	case OPERATION_FIND_NODES_ACCURATE:
		emit doOperationS(OPERATION_FIND_NODES_ACCURATE, params);
		scene_->addNodesItems(model_->getNodesVisual());
		break;

	case OPERATION_WRITE_CORRECTION_TABLE:
		emit doOperationS(OPERATION_WRITE_CORRECTION_TABLE, params);
		break;
	default:
		break;
	}
}