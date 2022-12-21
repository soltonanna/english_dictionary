#include "projection.hpp"

void projection::
set_camera_position(const double x, const double y, const double z)
{
	location_x = x;
	location_y = y;
	location_z = z;
}

void projection::
set_camera_angles(const double x, const double y, const double z)
{
	Sx = sin(x);
	Sy = sin(y);
	Sz = sin(z);
	Cx = cos(x);
	Cy = cos(y);
	Cz = cos(z);
}

void projection::
set_display_size(const double w, const double h)
{
	width = w;
	height = h;
}

void projection::
set_canvas_size(const double w, const double h)
{	
	canvas_width = w;	
	canvas_height = h;
}

void projection::
set_distance(const double d)
{	
	distance = d;
}

double projection::
get_x_projection(const double a, const double b, const double c)
{
	double x = a - location_x; 
	double y = b - location_y;
	double z = c - location_z;
	double distance_x = Cy * (Sz * y + Cz * x) - Sy * z;
	double distance_z = Cx * (Cy * z + Sy * (Sz * y + Cz * x)) - Sx * (Cz * y - Sz * x);
	
	return distance_x * distance / distance_z * width / canvas_width;
}

double projection::
get_y_projection(const double a, const double b, const double c)
{
	double x = a - location_x;
	double y = b - location_y;
	double z = c - location_z;
	double distance_y = Sx * (Cy * z + Sy * (Sz * y + Cz * x)) + Cx * (Cz * y - Sz * x);
	double distance_z = Cx * (Cy * z + Sy * (Sz * y + Cz * x)) - Sx * (Cz * y - Sz * x);

	return distance_y * distance / distance_z * height / canvas_height;
}

projection::
projection(){}

projection::
~projection(){}
