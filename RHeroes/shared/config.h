#ifndef CONFIG_H
#define CONFIG_H

#include "exploration/evaluationfunction.h"

namespace Config {

extern int robotID;

extern QString poaretBinary;

extern QString usarAddress;
extern QString upisAddress;
extern QString wssAddress;

extern quint16 usarPort;
extern quint16 upisPort;
extern quint16 wssPort;

extern Data::Pose baseStationPose;

/**
      * Global variable to set the policy for the exploration.
      */
extern Exploration::EvalType policy;

/**
      * Number of robot spawned.
      */
extern int robotCount;

/**
      * HSV default configuration
      */
extern double HMin, SMin, VMin, HMax, SMax, VMax;

/**
      * Attenuation Factor of the evnironment for the WSS.
      */
extern double attenuationFactor;

extern double signalCutoff;

extern double eDo;

extern double robotMaxSpeed;


// 0 - P3AT
// 1 - Kenaf
// 2 - AirRobot
extern int robotType;

namespace SLAM {
extern double maxSpatialErrorPerMeter;
extern double maxAngularErrorPerMeter;
extern double maxSpatialErrorPerRadian;
extern double maxAngularErrorPerRadian;

extern double lookupThresholdElseberg;
extern double lookupThresholdAmigoni;
extern double lookupThresholdLiGriffiths;
extern double lookupThresholdProbabilistic;
extern double lookupThresholdPoseCentric;

extern double broadLookupThresholdElseberg;
extern double broadLookupThresholdAmigoni;
extern double broadLookupThresholdLiGriffiths;
extern double broadLookupThresholdProbabilistic;
extern double broadLookupThresholdPoseCentric;

extern int ransacMaximumIterations;
extern int ransacMinimumIterations;

extern double odometryMinVarianceX;
extern double odometryMinVarianceY;
extern double odometryMinVarianceTheta;

extern double laserRangeVariance;
extern double laserAngleVariance;
extern double laserOutOfRange;

extern double splitThreshold;
extern double mergeThreshold;
extern double collinearityThreshold;
extern double thinningThreshold;

extern double landmarkSpatialDistance;
extern double landmarkAngularDistance;

} /* namespace SLAM */

//PRM
namespace PRM {
extern double edgeThreshold;
extern double movementRadius;
extern int precision;
extern int pointNumber;
extern int pointNumberFrontier;
extern int maxPointDistance;
extern int pathNumber;
}
//
}


#endif // CONFIG_H
