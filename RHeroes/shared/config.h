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

//Obstacle
namespace OBS{

extern int obstacle_algorithm;
extern double obstacle_timeout;
extern double map_renew_timeout;

extern int is_test;

extern double test_pose_x;
extern double test_pose_y;
extern double test_pose_z;
extern double test_pose_theta;

extern double test_frontier_x;
extern double test_frontier_y;
extern double test_frontier_z;
extern double test_frontier_theta;

extern double sonar_threshold;
extern double laser_threshold;

extern int emp_angle_tolerance;
extern int emp_min_rotation;
extern int emp_med_rotation;
extern int emp_max_rotation;
extern double emp_straight_meters;
extern double emp_back_meters;

extern double emp_frontier_angle_threshold;
extern double emp_left_right_threshold;

extern double dwa_laser_max_range;
extern double dwa_time;
extern double dwa_min_velocity;
extern double dwa_max_velocity;
extern double dwa_step;
extern double dwa_rotation_safety;
extern double dwa_translation_safety;
extern double dwa_rotation_threshold;
extern double dwa_translation_threshold;
extern double dwa_pose_threshold;

extern double dwa_sigma;
extern double dwa_alpha_target;
extern double dwa_beta_clearance;
extern double dwa_gamma_velocity;

}



//
}


#endif // CONFIG_H
