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

#ifndef _STEREOVISION_H_
#define _STEREOVISION_H_

#include "oc_array.h"
#include "oc_calibration.h"
#include "oc_point.h"

namespace opencorr
{
	class Stereovision
	{
	protected:
		Calibration* view1_cam = nullptr; //intrinsics and extrinsics of the primary camera
		Calibration* view2_cam = nullptr; //intrinsics and extrinsics of the secondary camera
		int thread_number; //CPU thread number

	public:
		Eigen::Matrix3f fundamental_matrix; //fundamental matrix of binocular stereovision system

		Stereovision(Calibration* view1_cam, Calibration* view2_cam, int thread_number);
		~Stereovision();

		void updateCameras(Calibration* view1_cam, Calibration* view2_cam);
		void updateFundementalMatrix();
		void prepare();

		Point3D reconstruct(Point2D& view1_2d_point, Point2D& view2_2d_point);
		void reconstruct(std::vector<Point2D>& view1_2d_point_queue, std::vector<Point2D>& view2_2d_point_queue, std::vector<Point3D>& space_3d_point_queue);
	};

}//namespace opencorr
#endif //_STEREOVISION_H_
