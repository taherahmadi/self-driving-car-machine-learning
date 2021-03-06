#ifndef UKF_H
#define UKF_H

#include "measurement_package.h"
#include "Eigen/Dense"
#include <vector>
#include <string>
#include <fstream>

using Eigen::MatrixXd;
using Eigen::VectorXd;

class UKF {
public:

  ///* initially set to false, set to true in first call of ProcessMeasurement
  bool is_initialized_;

  ///* if this is false, laser measurements will be ignored (except for init)
  bool use_laser_;

  ///* if this is false, radar measurements will be ignored (except for init)
  bool use_radar_;

  ///* state vector: [pos1 pos2 vel_abs yaw_angle yaw_rate] in SI units and rad
  VectorXd x_;

  ///* state covariance matrix
  MatrixXd P_;

  ///* measurement radar noise covariance matrix
  MatrixXd R_radar_;

  ///* measurement lidar noise covariance matrix
  MatrixXd R_lidar_;

  ///* predicted sigma points matrix
  MatrixXd x_sigma_points_predicted_;

  ///* time when the state is true, in us
  long previous_timestamp_;

  ///* Process noise standard deviation longitudinal acceleration in m/s^2
  double std_a_;

  ///* Process noise standard deviation yaw acceleration in rad/s^2
  double std_yawdd_;

  ///* Laser measurement noise standard deviation position1 in m
  double std_laspx_;

  ///* Laser measurement noise standard deviation position2 in m
  double std_laspy_;

  ///* Radar measurement noise standard deviation radius in m
  double std_radr_;

  ///* Radar measurement noise standard deviation angle in rad
  double std_radphi_;

  ///* Radar measurement noise standard deviation radius change in m/s
  double std_radrd_;

  ///* Weights of sigma points
  VectorXd weights_;

  ///* State dimension
  int n_x_;

  ///* Augmented state dimension
  int n_aug_;

  ///* Sigma point spreading parameter
  double lambda_;

  ///* Radar State dimension
  int n_radar_;

  ///* Lidar State dimension
  int n_lidar_;

  double NIS_radar_;

  double NIS_lidar_;

  /**
   * Constructor
   */
  UKF();

  /**
   * Destructor
   */
  virtual ~UKF();

  /**
   * ProcessMeasurement
   * @param meas_package The latest measurement data of either radar or laser
   */
  void ProcessMeasurement(MeasurementPackage meas_package);

  /**
   * Prediction Predicts sigma points, the state, and the state covariance
   * matrix
   * @param delta_t Time between k and k+1 in s
   */
  void Prediction(float delta_t);

  /**
   * Updates the state and the state covariance matrix using a laser measurement
   * @param meas_package The measurement at k+1
   */
  void UpdateLidar(MeasurementPackage meas_package);

  /**
   * Updates the state and the state covariance matrix using a radar measurement
   * @param meas_package The measurement at k+1
   */
  void UpdateRadar(MeasurementPackage meas_package);

private:

  void updateLocalTimestamp(const MeasurementPackage meas_package);
  float calculateElapsedTime(const MeasurementPackage meas_package);
  void InitialiseStateVector(const MeasurementPackage meas_package);
  void InitialiseCovarianceMatrix(float covariance);
  void normaliseAngleIn(double *angle);
  MatrixXd GenerateSigmaPoints();
  VectorXd GeneratedAugmentedState();
  MatrixXd GenerateAugmentedCovarianceMatrix();
  MatrixXd GenerateAugmentedSigmaPoints(const VectorXd x_augmented,
                                        const MatrixXd P_augmented);
  void PredictSigmaPoints(const MatrixXd x_sigma_points_augmented,
                          const float delta_t);
  void PredictStateAndCovariance();
  MatrixXd
  PredictRadarCovarianceMatrix(const MatrixXd z_sigma_points,
                               const MatrixXd z_predicted);
  MatrixXd
  PredictLidarCovarianceMatrix(const MatrixXd z_sigma_points,
                               const MatrixXd z_predicted);
  MatrixXd TransformSigmaPointsToRadarSpace();
  MatrixXd TransformSigmaPointsToLidarSpace();

  VectorXd PredictMean(const MatrixXd z_sigma_points, VectorXd &z_predicted);
  MatrixXd
  CalculateRadarCrossCorrelationMatrix(const MatrixXd z_sigma_points,
                                       const MatrixXd z_predicted);
  MatrixXd
  CalculateLidarCrossCorrelationMatrix(const MatrixXd z_sigma_points,
                                       const MatrixXd z_predicted);

};

#endif /* UKF_H */
