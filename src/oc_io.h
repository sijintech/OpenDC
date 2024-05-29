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

#ifndef  _IO_H_
#define  _IO_H_

#include <string>
#include <vector>

#include "oc_poi.h"

using std::vector;
using std::string;

namespace opencorr
{
	//This module is made to input data from csv table and output data to csv data
	class IO2D
	{
	private:
		string file_path;
		string delimiter;
		int width, height;

	public:
		IO2D();
		~IO2D();

		string getPath() const;
		string getDelimiter() const;
		int getWidth() const;
		int getHeight() const;
		void setPath(string file_path);
		void setDelimiter(string delimiter);
		void setWidth(int width);
		void setHeight(int height);

		//load deformation of POIs from saved csv table
		vector<POI2D> loadTable2D();

		//load locations of POIs from csv table
		vector<Point2D> loadPoint2D(string file_path);

		void saveTable2D(vector<POI2D>& poi_queue);
		void saveDeformationTable2D(vector<POI2D>& poi_queue);

		//variable: 'u', 'v', 'c'(zncc), 'd'(convergence), 'i'(iteration), 'f'(feature), 'x' (exx), 'y' (eyy), 'r' (exy)
		void saveMap2D(vector<POI2D>& poi_queue, char variable);

		//load deformation of POIs from saved date table
		vector<POI2DS> loadTable2DS();

		void saveTable2DS(vector<POI2DS>& poi_queue);

		//variable: 'u', 'v', 'w', 'c'(r1r2_zncc), 'd'(r1t1_zncc), 'e'(r1t2_zncc), 'x' (exx), 'y' (eyy), 'z' (ezz), 'r' (exy) , 's' (eyz), 't' (ezx)
		void saveMap2DS(vector<POI2DS>& poi_queue, char variable);
	};

	class IO3D
	{
	private:
		string file_path;
		string delimiter;
		int dim_x, dim_y, dim_z;

	public:
		IO3D();
		~IO3D();

		string getPath() const;
		string getDelimiter() const;
		void setPath(string file_path);
		void setDelimiter(string delimiter);

		int getDimX();
		int getDimY();
		int getDimZ();
		void setDimX(int dim_x);
		void setDimY(int dim_y);
		void setDimZ(int dim_z);

		//load deformation of POIs from saved date table
		vector<POI3D> loadTable3D();

		//load locations of POIs from csv table
		vector<Point3D> loadPoint3D(string file_path);

		void saveTable3D(vector<POI3D>& poi_queue);

		//variable: 'u', 'v', 'w', 'c'(zncc), 'x' (exx), 'y' (eyy), 'z' (ezz), 'r' (exy) , 's' (eyz), 't' (ezx)
		void saveMap3D(vector<POI3D>& poi_queue, char variable);

		//save and load deformation of POIs into a binary matrix
		void saveMatrixBin(vector<POI3D>& poi_queue);
		vector<POI3D> loadMatrixBin();

	};

}//namespace opencorr

#endif //_IO_H_