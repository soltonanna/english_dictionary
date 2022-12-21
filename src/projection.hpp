#ifndef PROJECTION_HPP
#define PROJECTION_HPP

#include <math.h>
#define PI 3.14159265

class projection
{
private:
	double location_x;
	double location_y;
	double location_z;
	double distance;
	double width;
	double height;
	double Sx;
	double Sy;
	double Sz;
	double Cx;
	double Cy;
	double Cz;
	double canvas_width;
	double canvas_height;
public:
	void set_camera_position(const double, const double, const double);
	void set_camera_angles(const double, const double, const double);
	void set_display_size(const double, const double);
	void set_canvas_size(const double, const double);
	void set_distance(const double);
	double get_x_projection(const double, const double, const double);
	double get_y_projection(const double, const double, const double);
public:
	projection();
	~projection();
};

#endif
