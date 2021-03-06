/// @copyright (c) 2007 CSIRO
/// Australia Telescope National Facility (ATNF)
/// Commonwealth Scientific and Industrial Research Organisation (CSIRO)
/// PO Box 76, Epping NSW 1710, Australia
///
/// This file is part of the ASKAP software distribution.
///
/// The ASKAP software distribution is free software: you can redistribute it
/// and/or modify it under the terms of the GNU General Public License as
/// published by the Free Software Foundation; either version 2 of the License,
/// or (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program; if not, write to the Free Software
/// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
///
/// This program was modified so as to use it in the contest.
/// The last modification was on January 12, 2015.
///

#ifndef BENCHMARK_H
#define BENCHMARK_H

#pragma offload_attribute(push, target(mic))

// System includes
#include <vector>
#include <complex>
#include <stdio.h>

// Typedefs
typedef double Coord;
typedef float Real;
//typedef std::complex<Real> Value;
typedef std::complex<Coord> Value;

struct Sample
{
    Value data;
    int iu;
    int iv;
    int cOffset;
};

struct Index
{
     int gind, cind, dind;
};
inline bool cmp_gind(Index a, Index b);
inline bool cmp_cind(Index a, Index b);
void parallel_sort(int l, int r, Index *index, int parallel_num);//suggest parallel number not excess

#pragma offload_attribute(pop)

class Benchmark
{
public:
    Benchmark();

    int randomInt();
    void init();
    void runGrid();
    void runDegrid();

    void gridKernel(const int support,
                    const std::vector<Value>& C,
                    std::vector<Value>& grid, const int gSize);

    void initC(const std::vector<Coord>& freq,
               const Coord cellSize, const int wSize,
               int& support, int& overSample,
               Coord& wCellSize, std::vector<Value>& C);

    void initCOffset(const std::vector<Coord>& u, const std::vector<Coord>& v,
                     const std::vector<Coord>& w, const std::vector<Coord>& freq,
                     const Coord cellSize, const Coord wCellSize, const int wSize,
                     const int gSize, const int support, const int overSample);

    int getSupport();

    void printGrid();

// Change these if necessary to adjust run time
    int nSamples; // Number of data samples
    int wSize; // Number of lookup planes in w projection
    int nChan; // Number of spectral channels

// Don't change any of these numbers unless you know what you are doing!
    int gSize; // Size of output grid in pixels
    Coord cellSize; // Cellsize of output grid in wavelengths
    int baseline; // Maximum baseline in meters

private:
    std::vector<Value> grid;
    std::vector<Coord> u;
    std::vector<Coord> v;
    std::vector<Coord> w;
    std::vector<Sample> samples;
    std::vector<Value> outdata;

    std::vector< Value > C;
    int m_support;
    int overSample;

    Coord wCellSize;

    // For random number generator
    unsigned long next;
};
#endif
