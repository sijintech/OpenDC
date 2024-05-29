/*
 * This file is part of OpenCorr, an open source C++ library for
 * study and development of 2D, 3D/stereo and volumetric
 * digital image correlation.
 *
 * Copyright (C) 2021-2024, Zhenyu Jiang <zhenyujiang@scut.edu.cn>
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one from http://mozilla.org/MPL/2.0/.
 *
 * More information about OpenCorr can be found at https://www.opencorr.org/
 */

#pragma once

#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

#include "oc_array.h"
#include "oc_point.h"

namespace opencorr
{
	union CameraIntrinsics
	{
		struct
		{
			float fx, fy, fs;
			float cx, cy;
			float k1, k2, k3, k4, k5, k6;
			float p1, p2;
		};
		float cam_i[13];
	};

	union CameraExtrinsics
	{
		struct
		{
			float tx, ty, tz;
			float rx, ry, rz;
		};
		float cam_e[6];
	};

	class Calibration
	{
	public:
		CameraIntrinsics intrinsics;
		CameraExtrinsics extrinsics;

		Eigen::Matrix3f intrinsic_matrix; //camera intrinsic matrix of camera
		Eigen::Matrix3f rotation_matrix; //camera rotation matrix of camera
		Eigen::Vector3f translation_vector; //camera translation matrix of camera
		Eigen::MatrixXf projection_matrix; //projection matrix of camera

		float convergence; //convergence criterion in undistortion
		int iteration; //stop condition in undistortion

		//map of distorted coordinates in image system corresponding to the integral pixel coordinates in sensor system
		Eigen::MatrixXf map_x;
		Eigen::MatrixXf map_y;

		Calibration();
		Calibration(CameraIntrinsics& intrinsics, CameraExtrinsics& extrinsics);
		~Calibration();

		void updateIntrinsicMatrix();
		void updateRotationMatrix();
		void updateTranslationVector();
		void updateProjectionMatrix();
		void updateMatrices();
		void updateCalibration(CameraIntrinsics& intrinsics, CameraExtrinsics& extrinsics);

		//get and set convergence criterion and stop condition
		float getConvergence() const;
		int getIteration() const;
		void setUndistortion(float convergence, int iteration);

		//convert the coordinate between image/retina system and sensor/pixel system
		Point2D image_to_sensor(Point2D& point);
		Point2D sensor_to_image(Point2D& point);

		/* create a map of distorted coordinates in image/retina system
		corresponding to the integral pixel coordinates in sensor/pixel system */
		void prepare(int height, int width);

		//distort the coordinate of a point in image coordinate system
		Point2D distort(Point2D& point);

		//undistortion throught interpolation on image-sensor coordinate map
		Point2D undistort(Point2D& point);
	};

} //opencorr
#endif //_CALIBRATION_H_
