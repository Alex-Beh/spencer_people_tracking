/*
* Software License Agreement (BSD License)
*
*  Copyright (c) 2014-2015, Timm Linder, Social Robotics Lab, University of Freiburg
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*
*  * Redistributions of source code must retain the above copyright notice, this
*    list of conditions and the following disclaimer.
*  * Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution.
*  * Neither the name of the copyright holder nor the names of its contributors
*    may be used to endorse or promote products derived from this software
*    without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
*  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <srl_laser_detectors/learned_detectors/adaboost_detector.h>



namespace srl_laser_detectors {

AdaboostDetector::AdaboostDetector(ros::NodeHandle& nodeHandle, ros::NodeHandle& privateNodeHandle) : OpenCvDetector(nodeHandle, privateNodeHandle)
{
    m_adaboost = cv::ml::Boost::create();
    loadModel();

    m_privateNodeHandle.param<double>("decision_threshold", m_decisionThreshold, -10.0);
    ROS_INFO("Adaboost detector initialized!");
}

const std::string AdaboostDetector::getName()
{
    return "adaboost";
}

cv::ml::StatModel* AdaboostDetector::getStatModel()
{
    return m_adaboost.get();
}

void AdaboostDetector::classifyFeatureVector(const cv::Mat& featureVector, Label& label, double& confidence)
{
    confidence = m_adaboost->predict(featureVector);
    label = confidence > m_decisionThreshold ? FOREGROUND : BACKGROUND;
}

void AdaboostDetector::trainOnFeatures(const cv::Mat& featureMatrix, const cv::Mat& labelVector)
{
    int weakCount = 100; double weightTrimRate = 0.0; int boostType = cv::ml::Boost::DISCRETE;
    m_privateNodeHandle.getParamCached("adaboost_type", boostType);
    m_privateNodeHandle.getParamCached("adaboost_weight_trim_rate", weightTrimRate);
    m_privateNodeHandle.getParamCached("adaboost_weak_count", weakCount);
    
    m_adaboost->setBoostType(boostType);
    m_adaboost->setWeakCount(weakCount);
    m_adaboost->setWeightTrimRate(weightTrimRate);

    ROS_INFO_STREAM("Training Adaboost classifier with " << weakCount << " weak classifiers... this may take a while!");

    cv::Ptr<cv::ml::TrainData> trainData = cv::ml::TrainData::create(featureMatrix, cv::ml::ROW_SAMPLE, labelVector, cv::noArray(), maskSamplesWithNonfiniteValues(featureMatrix));
    m_adaboost->train(trainData);
}



} // end of namespace srl_laser_segmentation
