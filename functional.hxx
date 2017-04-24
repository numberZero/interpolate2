#pragma once
#include <functional>
#include <string>
#include <vector>

struct FunctionData
{
private:
	std::vector<double> xx;
	std::vector<double> yy;
	std::vector<double> ff;

	void init(std::vector<double> &&xx, std::vector<double> &&yy, std::function<double(double, double)> f);

public:
	int size_x() const;
	int size_y() const;
	double get_x(int i) const;
	double get_y(int j) const;
	double get_f(int i, int j) const;

	FunctionData() = default;
	FunctionData(FunctionData const &) = default;
	FunctionData(FunctionData &&) = default;
	FunctionData(std::vector<double> const &xx, std::vector<double> const &yy, std::function<double(double, double)> f);
	FunctionData(std::vector<double> &&xx, std::vector<double> &&yy, std::function<double(double, double)> f);
	FunctionData(double x1, double x2, double y1, double y2, int x_cuts, int y_cuts, std::function<double(double, double)> f);

	FunctionData &operator= (FunctionData const &) = default;
	FunctionData &operator= (FunctionData &&) = default;
};

std::vector<double> make_uniform_points(double x1, double x2, int N_cuts);
std::vector<double> make_chebyshev_points(double x1, double x2, int N_cuts);

inline int FunctionData::size_x() const
{
	return xx.size();
}

inline int FunctionData::size_y() const
{
	return yy.size();
}

inline double FunctionData::get_x(int i) const
{
	return xx[i];
}

inline double FunctionData::get_y(int j) const
{
	return yy[j];
}

inline double FunctionData::get_f(int i, int j) const
{
	return ff[size_x() * j + i];
}
