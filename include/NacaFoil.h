/*
Copyright (C) 2011          Gianluca Montecchi <gian@grys.it>
              2000-2009     Marko Marijc <marko@factavetra.com>

This file is part of BulbCalculator.

BulbCalculator is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation, either version 2 of the License, or (at your
option) any later version.

BulbCalculator is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with BulbCalculator.  If not, see <http://www.gnu.org/licenses/>.
*/

//---------------------------------------------------------------------------
#ifndef naca6H
#define naca6H
//---------------------------------------------------------------------------
#include <math.h>
#include <vector>
using namespace std;

double ellipse_circumference1(double a, double b);
double ellipse_circumference2(double a, double b);
double ellipse_get_x(double a, double b, double y);
double vd_average(const vector<double>&vd);

void mono_spline(const vector<double>& xi, const vector<double>& yi, double xd, double& yd);
void linear_intrp(double x1, double y1, double x2, double y2, double xd, double& yd);

class Foil
{
protected:
	vector<double> xcc;
	vector<double> yt_u;
	vector<double> yt_l;
public:
	Foil() {}
	virtual void get_y(double x, double& u, double& l);
    virtual ~Foil() {}
};

class NACA4Foil : public Foil
{
public:
	NACA4Foil() {}
	void get_y(double x, double& u, double& l);
};

class NACA6Foil : public Foil
{
	void set_params(unsigned form = 3, double thickness = 0.15, char type = '-');
public:
	NACA6Foil(unsigned f = 3, double th = 0.15, char ty = '-')
	{
		set_params(f, th, ty);
	}
};

class ExternalFoil : public Foil
{
	void set_params(vector<double>& xu, vector<double>& yu, vector<double>& xl, vector<double>& yl, bool flatten);
public:
	ExternalFoil(vector<double>& xu, vector<double>& yu, vector<double>& xl, vector<double>& yl, bool flatten)
	{
		set_params(xu, yu, xl, yl, flatten);
	}
};

struct profile_data
{
	double width;
	double height_u;
	double height_l;

	double circumference;
	double wetted_surface;

	double area;
	double volume;

	profile_data()
	{
		width		= 0.0;
		height_u	= 0.0;
		height_l	= 0.0;
		area		= 0.0;
		volume	= 0.0;
	}
	profile_data(const profile_data& pd)
	{
		width		= pd.width;
		height_u	= pd.height_u;
		height_l	= pd.height_l;
		area		= pd.area;
        volume      = pd.volume;
        circumference = pd.circumference;
        wetted_surface = pd.wetted_surface;
	}
	void calc(double whr, double prev_volume);
	double get_height()			{return fabs(height_u-height_l);}
	double get_half_height()	{return get_height()/2.0;}
};

struct profile : public vector<profile_data>
{
	static unsigned num_step;
	static double step;

	double wetted_surface;
	double volume;
	double gcentre;

	double max_width;
	double max_height_u;
	double max_height_l;
	unsigned max_chord_pos;

	string foil_name;
	double HLRatio;
	double WHRatio;
	vector<double> xu;
	vector<double> yu;
	vector<double> xl;
	vector<double> yl;
	bool flatten;

	profile() : vector<profile_data>(num_step+1)
	{
		volume		= 0.0;
		gcentre		= 0.0;

		foil_name	= "00XX";
		HLRatio		= 0.15;
		WHRatio		= 1.0;
		xu.clear();
		yu.clear();
		xl.clear();
		yl.clear();
		flatten		= false;
		calc();
	}
	profile(const profile& p) : vector<profile_data>(p)
	{
		volume		= p.volume;
		gcentre		= p.gcentre;
		foil_name	= p.foil_name;
		HLRatio		= p.HLRatio;
		WHRatio		= p.WHRatio;
		xu				= p.xu;
		yu				= p.yu;
		xl				= p.xl;
		yl				= p.yl;
		flatten		= p.flatten;
        wetted_surface = p.wetted_surface;
        max_width = p.max_width;
        max_height_u = p.max_height_u;
        max_height_l = p.max_height_l;
        max_chord_pos = p.max_chord_pos;
	}
	void calc();
	bool import_foil(string filename, int format = 0); // 0 = UIUC, 1 = NASG
	bool import_uiuc_foil(istream& is);
	bool import_nasg_foil(istream& is);
};
#endif
