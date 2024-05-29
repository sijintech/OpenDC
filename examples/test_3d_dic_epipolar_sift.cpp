/*
 This example demonstrates how to use OpenCorr to realize 3D/stereo DIC.
 In the implementation, the stereo matching between the two views uses
 epipolar constraint aided method and the ICGN algorithm with the 2nd order
 shape function, while the matching between the same view before and after
 deformation uses the SIFT feature guided method and the ICGN algorithm with
 the 1st order shape function.
*/

#include <fstream>

#include "opencorr.h"

using namespace opencorr;
using namespace std;

int main()
{
	//set paths of images.
	//in this example, the right image of initial state is used as the reference image
	string ref_view1_image_path = "d:/dic_tests/3d_dic/GT4-0000_0.tif"; //replace it with the path on your computer
	string ref_view2_image_path = "d:/dic_tests/3d_dic/GT4-0000_1.tif"; //replace it with the path on your computer
	string tar_view1_image_path = "d:/dic_tests/3d_dic/GT4-0273_0.tif"; //replace it with the path on your computer
	string tar_view2_image_path = "d:/dic_tests/3d_dic/GT4-0273_1.tif"; //replace it with the path on your computer

	//create the instances of images
	Image2D ref_view1_img(ref_view1_image_path);
	Image2D ref_view2_img(ref_view2_image_path);
	Image2D tar_view1_img(tar_view1_image_path);
	Image2D tar_view2_img(tar_view2_image_path);

	//create instances to read and write csv files
	string delimiter = ",";
	ofstream csv_out; //instance for output calculation time
	IO2D in_out; //instance for input and output DIC data
	in_out.setDelimiter(delimiter);
	in_out.setHeight(ref_view1_img.height);
	in_out.setWidth(ref_view1_img.width);

	//load the coordinates of POIs in principal view
	string file_path = "d:/dic_tests/3d_dic/GT4-POIs.csv"; //replace it with the path on your computer
	vector<Point2D> ref_view1_pt_queue = in_out.loadPoint2D(file_path);
	int queue_length = (int)ref_view1_pt_queue.size();

	//initialize papameters for timing
	double timer_tic, timer_toc, consumed_time;
	vector<double> computation_time;

	//get the time of start
	timer_tic = omp_get_wtime();

	//set OpenMP parameters
	int cpu_thread_number = omp_get_num_procs() - 1;
	omp_set_num_threads(cpu_thread_number);

	//create the instances of camera parameters
	CameraIntrinsics view1_cam_intrinsics, view2_cam_intrinsics;
	CameraExtrinsics view1_cam_extrinsics, view2_cam_extrinsics;
	view1_cam_intrinsics.fx = 6673.315918f;
	view1_cam_intrinsics.fy = 6669.302734f;
	view1_cam_intrinsics.fs = 0.f;
	view1_cam_intrinsics.cx = 872.15778f;
	view1_cam_intrinsics.cy = 579.95532f;
	view1_cam_intrinsics.k1 = 0.032258954f;
	view1_cam_intrinsics.k2 = -1.01141417f;
	view1_cam_intrinsics.k3 = 29.78838921f;
	view1_cam_intrinsics.k4 = 0;
	view1_cam_intrinsics.k5 = 0;
	view1_cam_intrinsics.k6 = 0;
	view1_cam_intrinsics.p1 = 0;
	view1_cam_intrinsics.p2 = 0;

	view1_cam_extrinsics.tx = 0;
	view1_cam_extrinsics.ty = 0;
	view1_cam_extrinsics.tz = 0;
	view1_cam_extrinsics.rx = 0;
	view1_cam_extrinsics.ry = 0;
	view1_cam_extrinsics.rz = 0;

	view2_cam_intrinsics.fx = 6607.618164f;
	view2_cam_intrinsics.fy = 6602.857422f;
	view2_cam_intrinsics.fs = 0.f;
	view2_cam_intrinsics.cx = 917.9733887f;
	view2_cam_intrinsics.cy = 531.6352539f;
	view2_cam_intrinsics.k1 = 0.064598486f;
	view2_cam_intrinsics.k2 = -4.531373978f;
	view2_cam_intrinsics.k3 = 29.78838921f;
	view2_cam_intrinsics.k4 = 0;
	view2_cam_intrinsics.k5 = 0;
	view2_cam_intrinsics.k6 = 0;
	view2_cam_intrinsics.p1 = 0;
	view2_cam_intrinsics.p2 = 0;

	view2_cam_extrinsics.tx = 122.24886f;
	view2_cam_extrinsics.ty = 1.8488892f;
	view2_cam_extrinsics.tz = 17.624638f;
	view2_cam_extrinsics.rx = 0.00307711f;
	view2_cam_extrinsics.ry = -0.33278773f;
	view2_cam_extrinsics.rz = 0.00524556f;

	//create the instances for stereovision
	Calibration cam_view1_calib(view1_cam_intrinsics, view1_cam_extrinsics);
	Calibration cam_view2_calib(view2_cam_intrinsics, view2_cam_extrinsics);
	cam_view1_calib.prepare(ref_view1_img.height, ref_view1_img.width);
	cam_view2_calib.prepare(ref_view2_img.height, ref_view2_img.width);
	Stereovision stereo_reconstruction(&cam_view1_calib, &cam_view2_calib, cpu_thread_number);

	//create the the queues of 2D points for stereo matching and reconstruction
	Point2D point_2d;
	vector<Point2D> ref_view2_pt_queue(queue_length, point_2d);
	vector<Point2D> tar_view1_pt_queue(queue_length, point_2d);
	vector<Point2D> tar_view2_pt_queue(queue_length, point_2d);

	//create the the queues of 3D points for stereo reconstruction
	Point3D point_3d;
	vector<Point3D> ref_pt_3d_queue(queue_length, point_3d);
	vector<Point3D> tar_pt_3d_queue(queue_length, point_3d);

	//create the queues of POIs for matching
	POI2D poi_2d(point_2d);
	vector<POI2D> poi_queue(queue_length, poi_2d);
	vector<POI2D> poi_round_queue(queue_length, poi_2d);
	vector<POI2DS> poi_result_queue(queue_length, poi_2d);

	//assign the coordinates to POI queues
#pragma omp parallel for
	for (int i = 0; i < (int)queue_length; i++)
	{
		poi_queue[i].x = ref_view1_pt_queue[i].x;
		poi_queue[i].y = ref_view1_pt_queue[i].y;

		poi_result_queue[i].x = ref_view1_pt_queue[i].x;
		poi_result_queue[i].y = ref_view1_pt_queue[i].y;
	}

	//set DIC parameters
	int subset_radius_x = 16;
	int subset_radius_y = 16;
	float conv_criterion = 0.001f;
	int stop_condition = 10;

	//initialize ICGN with the 1st order shape function
	ICGN2D1* icgn1 = new ICGN2D1(subset_radius_x, subset_radius_y, conv_criterion, stop_condition, cpu_thread_number);
	//initialize ICGN with the 2nd order shape function
	ICGN2D2* icgn2 = new ICGN2D2(subset_radius_x, subset_radius_y, conv_criterion, stop_condition, cpu_thread_number);

	//create the instances for SIFT feature aided matching
	SIFT2D* sift = new SIFT2D();
	FeatureAffine2D* feature_affine = new FeatureAffine2D(subset_radius_x, subset_radius_y, cpu_thread_number);

	//create an instance for epipolar constraint aided matching
	EpipolarSearch* epipolar_search = new EpipolarSearch(cam_view1_calib, cam_view2_calib, cpu_thread_number);

	//set search parameters in epipolar constraint aided matching
	Point2D parallax_guess(-30, -40);
	epipolar_search->setParallax(parallax_guess);
	int search_radius = 30;
	int search_step = 5;
	epipolar_search->setSearch(search_radius, search_step);

	//initialize an ICGN2D1 instance in epipolar constraint aided matching
	subset_radius_x = 20;
	subset_radius_y = 20;
	conv_criterion = 0.05f;
	stop_condition = 5;
	epipolar_search->createICGN(subset_radius_x, subset_radius_y, conv_criterion, stop_condition);

	//get the time of end 
	timer_toc = omp_get_wtime();
	consumed_time = timer_toc - timer_tic;
	computation_time.push_back(consumed_time); //0

	//display the time of initialization on screen
	cout << "Initialization with " << queue_length << " POIs takes " << consumed_time << " sec, " << cpu_thread_number << " CPU threads launched." << std::endl;

	//get the time of start
	timer_tic = omp_get_wtime();

	//stereo matching between reference view1 and reference view2
	epipolar_search->setImages(ref_view1_img, ref_view2_img);
	epipolar_search->prepare();

	//coarse registration
	epipolar_search->compute(poi_queue);

	//refined registration
	icgn2->setImages(ref_view1_img, ref_view2_img);
	icgn2->prepare();
	icgn2->compute(poi_queue);

	//store the results of stereo matching between the two reference views
#pragma omp parallel for
	for (int i = 0; i < queue_length; i++)
	{
		Point2D current_location(poi_queue[i].x, poi_queue[i].y);
		Point2D current_offset(poi_queue[i].deformation.u, poi_queue[i].deformation.v);
		ref_view2_pt_queue[i] = current_location + current_offset;
		poi_result_queue[i].result.r2_x = ref_view2_pt_queue[i].x;
		poi_result_queue[i].result.r2_y = ref_view2_pt_queue[i].y;
		poi_result_queue[i].result.r1r2_zncc = poi_queue[i].result.zncc;
	}

	//get the time of end 
	timer_toc = omp_get_wtime();
	consumed_time = timer_toc - timer_tic;
	computation_time.push_back(consumed_time); //1

	//display the time of processing on the screen
	cout << "Stereo matching between reference view1 and view2 takes " << consumed_time << " sec." << std::endl;

	//get the time of start
	timer_tic = omp_get_wtime();

	//temporal matching between reference view1 and target view 1
	//extract and match the SIFT features
	sift->setImages(ref_view1_img, tar_view1_img);
	sift->prepare();
	sift->compute();

	//estimate the deformation according to the SIFT features around each POIs
	feature_affine->setImages(ref_view1_img, tar_view1_img);
	feature_affine->setKeypointPair(sift->ref_matched_kp, sift->tar_matched_kp);
	feature_affine->prepare();
	feature_affine->compute(poi_queue);

	//high accuracy registration
	icgn1->setImages(ref_view1_img, tar_view1_img);
	icgn1->prepare();
	icgn1->compute(poi_queue);

	//store the results of temporal matching
#pragma omp parallel for
	for (int i = 0; i < queue_length; i++)
	{
		Point2D current_location(poi_queue[i].x, poi_queue[i].y);
		Point2D current_offset(poi_queue[i].deformation.u, poi_queue[i].deformation.v);
		tar_view1_pt_queue[i] = current_location + current_offset;
		poi_result_queue[i].result.t1_x = tar_view1_pt_queue[i].x;
		poi_result_queue[i].result.t1_y = tar_view1_pt_queue[i].y;
		poi_result_queue[i].result.r1t1_zncc = poi_queue[i].result.zncc;

		poi_round_queue[i].x = round(tar_view1_pt_queue[i].x);
		poi_round_queue[i].y = round(tar_view1_pt_queue[i].y);
	}

	//get the time of end 
	timer_toc = omp_get_wtime();
	consumed_time = timer_toc - timer_tic;
	computation_time.push_back(consumed_time); //2

	//display the time of processing on the screen
	cout << "Temporal matching between reference view1 and target view1 takes " << consumed_time << " sec." << std::endl;

	//get the time of start
	timer_tic = omp_get_wtime();

	//stereo matching between reference view1 and target view2
	//coase stereo matching between target view1 and taget view2
	parallax_guess.x = -30.f;
	parallax_guess.y = -40.f;
	epipolar_search->setParallax(parallax_guess);
	epipolar_search->setImages(tar_view1_img, tar_view2_img);
	epipolar_search->prepare();
	epipolar_search->compute(poi_round_queue);

	//combine the dispalcements obtained by stereo matching and temperoal matching
#pragma omp parallel for
	for (int i = 0; i < queue_length; i++)
	{
		poi_queue[i].deformation.u += poi_round_queue[i].deformation.u;
		poi_queue[i].deformation.v += poi_round_queue[i].deformation.v;
	}

	//stereo matching between reference view1 and target view2
	icgn2->setImages(ref_view1_img, tar_view2_img);
	icgn2->prepare();
	icgn2->compute(poi_queue);

	//store the results of matching
#pragma omp parallel for
	for (int i = 0; i < queue_length; i++)
	{
		Point2D current_location(poi_queue[i].x, poi_queue[i].y);
		Point2D current_offset(poi_queue[i].deformation.u, poi_queue[i].deformation.v);
		tar_view2_pt_queue[i] = current_location + current_offset;
		poi_result_queue[i].result.t2_x = tar_view2_pt_queue[i].x;
		poi_result_queue[i].result.t2_y = tar_view2_pt_queue[i].y;
		poi_result_queue[i].result.r1t2_zncc = poi_queue[i].result.zncc;
	}

	//get the time of end 
	timer_toc = omp_get_wtime();
	consumed_time = timer_toc - timer_tic;
	computation_time.push_back(consumed_time); //3

	//display the time of processing on the screen
	cout << "Stereo matching between reference view1 and target view2 takes " << consumed_time << " sec." << std::endl;

	//get the time of start
	timer_tic = omp_get_wtime();

	//reconstruct the 3D coordinates in world coordinate system
	stereo_reconstruction.prepare();
	stereo_reconstruction.reconstruct(ref_view1_pt_queue, ref_view2_pt_queue, ref_pt_3d_queue);
	stereo_reconstruction.reconstruct(tar_view1_pt_queue, tar_view2_pt_queue, tar_pt_3d_queue);

	//calculate the 3D displacements of POIs
#pragma omp parallel for
	for (int i = 0; i < queue_length; i++)
	{
		poi_result_queue[i].ref_coor = ref_pt_3d_queue[i];
		poi_result_queue[i].tar_coor = tar_pt_3d_queue[i];
		poi_result_queue[i].deformation.u = tar_pt_3d_queue[i].x - ref_pt_3d_queue[i].x;
		poi_result_queue[i].deformation.v = tar_pt_3d_queue[i].y - ref_pt_3d_queue[i].y;
		poi_result_queue[i].deformation.w = tar_pt_3d_queue[i].z - ref_pt_3d_queue[i].z;
	}

	//get the time of end 
	timer_toc = omp_get_wtime();
	consumed_time = timer_toc - timer_tic;
	computation_time.push_back(consumed_time); //4

	//display the time of processing on the screen
	cout << "Stereo reconstruction and calculation of displacements takes " << consumed_time << " sec." << std::endl;

	//save the results
	file_path = tar_view1_image_path.substr(0, tar_view1_image_path.find_last_of(".")) + "_epipolar_sift_r16.csv";
	in_out.setPath(file_path);
	in_out.saveTable2DS(poi_result_queue);

	//save the computation time
	file_path = tar_view1_image_path.substr(0, tar_view1_image_path.find_last_of(".")) + "_epipolar_sift_r16_time.csv";
	csv_out.open(file_path);
	if (csv_out.is_open())
	{
		csv_out << "POI number" << delimiter << "Initialization" << delimiter << "r1_to_r2" << delimiter << "r1_to_t1" << delimiter << "r1_to_t2" << delimiter << "reconstruction" << endl;
		csv_out << poi_queue.size() << delimiter << computation_time[0] << delimiter << computation_time[1] << delimiter << computation_time[2] << delimiter << computation_time[3] << delimiter << computation_time[4] << endl;
	}
	csv_out.close();

	delete icgn1;
	delete icgn2;
	delete sift;
	delete feature_affine;
	delete epipolar_search;

	cout << "Press any key to exit..." << std::endl;
	cin.get();

	return 0;
}
