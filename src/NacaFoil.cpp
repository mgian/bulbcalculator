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
//#pragma hdrstop
#include <math.h>
#include <vector>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include <QString>

using namespace std;

#include "../include/NacaFoil.h"

unsigned profile::num_step = 10000;
double profile::step = 0.0001;
//---------------------------------------------------------------------------
//#pragma package(smart_init)
#define N6P		26	// NUMBER OF 6 SERIES X POINTS IN DATABASE
#define N6S		 5	// NUMBER OF 6 SERIES IN DATABASE (63 - 67)
#define N6T		 8	// NUMBER OF 6 SERIES THICKNESS FORMS FOR EACH SUBSERIES
#define N6AS	 3	// NUMBER OF 6A SERIES IN DATABASE (63 - 65)
#define N6AT	 7	// NUMBER OF 6A SERIES THICKNESS FORMS FOR EACH SUBSERIES

// NACA X/C LOCATIONS
double xc[N6P] = {0.0000, 0.5000, 0.7500, 1.2500, 2.5000, 5.0000, 7.5000, 10.0000, 15.0000, 20.0000, 25.0000, 30.0000, 35.0000, 40.0000, 45.0000, 50.0000, 55.0000, 60.0000, 65.0000, 70.0000, 75.0000, 80.0000, 85.0000, 90.0000, 95.0000, 100.0000};

// Thickness forms in the database
double tfth6[N6T]	 = {0.06, 0.08, 9.000001e-02, 0.1, 0.12, 0.15, 0.18, 0.21};
double tfth6A[N6AT] = {0.06, 0.08, 0.1, 0.12, 0.15, 0.18, 0.21};

// NACA THICKNESS FORMS 6-21%, 63-67 SERIES
double tf6[N6S*N6T][N6P] = {
	{0.0000, 0.5030, 0.6090, 0.7710, 1.0570, 1.4620, 1.7660, 2.0100, 2.3860, 2.6560, 2.8410, 2.9540, 3.0000, 2.9710, 2.8770, 2.7230, 2.5170, 2.2670, 1.9820, 1.6700, 1.3420, 1.0080, 0.6830, 0.3830, 0.1380, 0.0000},
	{0.0000, 0.6683, 0.8089, 1.0244, 1.4073, 1.9517, 2.3588, 2.6858, 3.1896, 3.5501, 3.7965, 3.9459, 3.9999, 3.9579, 3.8254, 3.6133, 3.3338, 2.9984, 2.6173, 2.2003, 1.7616, 1.3171, 0.8870, 0.4963, 0.1777, 0.0000},
	{0.0000, 0.7490, 0.9060, 1.1510, 1.5820, 2.1960, 2.6550, 3.0240, 3.5910, 3.9970, 4.2750, 4.4420, 4.5000, 4.4470, 4.2960, 4.0560, 3.7390, 3.3580, 2.9280, 2.4580, 1.9660, 1.4710, 0.9900, 0.5500, 0.1960, 0.0000},
	{0.0000, 0.8290, 1.0040, 1.2750, 1.7560, 2.4400, 2.9500, 3.3620, 3.9940, 4.4450, 4.7530, 4.9380, 5.0000, 4.9380, 4.7660, 4.4960, 4.1400, 3.7150, 3.2340, 2.7120, 2.1660, 1.6180, 1.0880, 0.6040, 0.2140, 0.0000},
	{0.0000, 0.9850, 1.1940, 1.5190, 2.1020, 2.9250, 3.5420, 4.0390, 4.7990, 5.3420, 5.7120, 5.9300, 6.0000, 5.9200, 5.7040, 5.3700, 4.9350, 4.4200, 3.8400, 3.2100, 2.5560, 1.9020, 1.2740, 0.7070, 0.2500, 0.0000},
	{0.0000, 1.2040, 1.4620, 1.8780, 2.6100, 3.6480, 4.4270, 5.0550, 6.0110, 6.6930, 7.1550, 7.4210, 7.5000, 7.3860, 7.0990, 6.6650, 6.1080, 5.4530, 4.7210, 3.9340, 3.1190, 2.3100, 1.5410, 0.8520, 0.3000, 0.0000},
	{0.0000, 1.4040, 1.7130, 2.2170, 3.1040, 4.3620, 5.3080, 6.0680, 7.2250, 8.0480, 8.6000, 8.9130, 9.0000, 8.8450, 8.4820, 7.9420, 7.2560, 6.4550, 5.5670, 4.6220, 3.6500, 2.6910, 1.7870, 0.9850, 0.3480, 0.0000},
	{0.0000, 1.5830, 1.9370, 2.5270, 3.5770, 5.0650, 6.1820, 7.0800, 8.4410, 9.4100, 10.0530, 10.4120, 10.5000, 10.2980, 9.8540, 9.2060, 8.3900, 7.4410, 6.3960, 5.2900, 4.1600, 3.0540, 2.0210, 1.1130, 0.3920, 0.0000},
	{0.0000, 0.4940, 0.5960, 0.7540, 1.0240, 1.4050, 1.6920, 1.9280, 2.2980, 2.5720, 2.7720, 2.9070, 2.9810, 2.9950, 2.9190, 2.7750, 2.5750, 2.3310, 2.0500, 1.7400, 1.4120, 1.0720, 0.7370, 0.4230, 0.1570, 0.0000},
	{0.0000, 0.6580, 0.7940, 1.0050, 1.3650, 1.8750, 2.2590, 2.5740, 3.0690, 3.4370, 3.7040, 3.8840, 3.9790, 3.9920, 3.8830, 3.6840, 3.4110, 3.0810, 2.7040, 2.2910, 1.8540, 1.4040, 0.9610, 0.5500, 0.2060, 0.0000},
	{0.0000, 0.7390, 0.8920, 1.1280, 1.5330, 2.1090, 2.5430, 2.8980, 3.4550, 3.8680, 4.1700, 4.3730, 4.4790, 4.4900, 4.3640, 4.1360, 3.8260, 3.4520, 3.0260, 2.5610, 2.0690, 1.5640, 1.0690, 0.6110, 0.2270, 0.0000},
	{0.0000, 0.8200, 0.9890, 1.2500, 1.7010, 2.3430, 2.8260, 3.2210, 3.8420, 4.3020, 4.6390, 4.8640, 4.9800, 4.9880, 4.8430, 4.5860, 4.2380, 3.8200, 3.3450, 2.8270, 2.2810, 1.7220, 1.1760, 0.6710, 0.2480, 0.0000},
	{0.0000, 0.9780, 1.1790, 1.4900, 2.0350, 2.8100, 3.3940, 3.8710, 4.6200, 5.1730, 5.5760, 5.8440, 5.9780, 5.9810, 5.7980, 5.4800, 5.0560, 4.5480, 3.9740, 3.3500, 2.6950, 2.0290, 1.3820, 0.7860, 0.2880, 0.0000},
	{0.0000, 1.2080, 1.4560, 1.8420, 2.5280, 3.5040, 4.2400, 4.8420, 5.7850, 6.4800, 6.9850, 7.3190, 7.4820, 7.4730, 7.2240, 6.8100, 6.2660, 5.6200, 4.8950, 4.1130, 3.2960, 2.4720, 1.6770, 0.9500, 0.3460, 0.0000},
	{0.0000, 1.4280, 1.7200, 2.1770, 3.0050, 4.1860, 5.0760, 5.8030, 6.9420, 7.7820, 8.3910, 8.7890, 8.9790, 8.9520, 8.6300, 8.1140, 7.4450, 6.6580, 5.7820, 4.8420, 3.8660, 2.8880, 1.9510, 1.1010, 0.4000, 0.0000},
	{0.0000, 1.6460, 1.9850, 2.5170, 3.4850, 4.8710, 5.9150, 6.7690, 8.1080, 9.0950, 9.8070, 10.2690, 10.4810, 10.4310, 10.0300, 9.4040, 8.6070, 7.6780, 6.6490, 5.5490, 4.4160, 3.2870, 2.2130, 1.2450, 0.4490, 0.0000},
	{0.0000, 0.4760, 0.5740, 0.7170, 0.9560, 1.3100, 1.5890, 1.8240, 2.1970, 2.4820, 2.6970, 2.8520, 2.9520, 2.9980, 2.9830, 2.9000, 2.7410, 2.5180, 2.2460, 1.9350, 1.5940, 1.2330, 0.8650, 0.5100, 0.1950, 0.0000},
	{0.0000, 0.6270, 0.7560, 0.9450, 1.2670, 1.7450, 2.1180, 2.4320, 2.9310, 3.3120, 3.5990, 3.8050, 3.9380, 3.9980, 3.9740, 3.8570, 3.6380, 3.3370, 2.9710, 2.5530, 2.0960, 1.6170, 1.1310, 0.6640, 0.2520, 0.0000},
	{0.0000, 0.7000, 0.8450, 1.0580, 1.4210, 1.9610, 2.3830, 2.7360, 3.2990, 3.7270, 4.0500, 4.2820, 4.4310, 4.4960, 4.4690, 4.3360, 4.0860, 3.7430, 3.3280, 2.8560, 2.3420, 1.8050, 1.2600, 0.7380, 0.2800, 0.0000},
	{0.0000, 0.7720, 0.9320, 1.1690, 1.5740, 2.1770, 2.6470, 3.0400, 3.6660, 4.1430, 4.5030, 4.7600, 4.9240, 4.9960, 4.9630, 4.8120, 4.5300, 4.1460, 3.6820, 3.1560, 2.5840, 1.9870, 1.3850, 0.8100, 0.3060, 0.0000},
	{0.0000, 0.9230, 1.1090, 1.3870, 1.8750, 2.6060, 3.1720, 3.6470, 4.4020, 4.9750, 5.4060, 5.7160, 5.9120, 5.9970, 5.9490, 5.7570, 5.4120, 4.9430, 4.3810, 3.7430, 3.0590, 2.3450, 1.6300, 0.9470, 0.3560, 0.0000},
	{0.0000, 1.1240, 1.3560, 1.7020, 2.3240, 3.2450, 3.9590, 4.5550, 5.5040, 6.2230, 6.7640, 7.1520, 7.3960, 7.4980, 7.4270, 7.1680, 6.7200, 6.1180, 5.4030, 4.6000, 3.7440, 2.8580, 1.9770, 1.1440, 0.4280, 0.0000},
	{0.0000, 1.3370, 1.6080, 2.0140, 2.7510, 3.8660, 4.7330, 5.4570, 6.6060, 7.4760, 8.1290, 8.5950, 8.8860, 8.9990, 8.9010, 8.5680, 8.0080, 7.2670, 6.3950, 5.4260, 4.3960, 3.3380, 2.2950, 1.3190, 0.4900, 0.0000},
	{0.0000, 1.5220, 1.8380, 2.3010, 3.1540, 4.4720, 5.4980, 6.3520, 7.7000, 8.7200, 9.4870, 10.0360, 10.3750, 10.4990, 10.3660, 9.9520, 9.2770, 8.3900, 7.3600, 6.2240, 5.0240, 3.8000, 2.5980, 1.4840, 0.5460, 0.0000},
	{0.0000, 0.4610, 0.5540, 0.6930, 0.9180, 1.2570, 1.5240, 1.7520, 2.1190, 2.4010, 2.6180, 2.7820, 2.8990, 2.9710, 3.0000, 2.9850, 2.9250, 2.8150, 2.6110, 2.3160, 1.9530, 1.5430, 1.1070, 0.6650, 0.2620, 0.0000},
	{0.0000, 0.6100, 0.7350, 0.9190, 1.2190, 1.6730, 2.0310, 2.3350, 2.8260, 3.2010, 3.4900, 3.7090, 3.8650, 3.9620, 4.0000, 3.9780, 3.8960, 3.7400, 3.4590, 3.0620, 2.5740, 2.0270, 1.4470, 0.8640, 0.3380, 0.0000},
	{0.0000, 0.6870, 0.8240, 1.0300, 1.3680, 1.8800, 2.2830, 2.6260, 3.1780, 3.6010, 3.9270, 4.1730, 4.3480, 4.4570, 4.4990, 4.4750, 4.3810, 4.2040, 3.8820, 3.4280, 2.8770, 2.2630, 1.6110, 0.9610, 0.3740, 0.0000},
	{0.0000, 0.7590, 0.9130, 1.1410, 1.5160, 2.0870, 2.5360, 2.9170, 3.5300, 4.0010, 4.3630, 4.6360, 4.8320, 4.9530, 5.0000, 4.9710, 4.8650, 4.6650, 4.3020, 3.7870, 3.1760, 2.4940, 1.7730, 1.0540, 0.4080, 0.0000},
	{0.0000, 0.9060, 1.0870, 1.3580, 1.8080, 2.4960, 3.0370, 3.4960, 4.2340, 4.8010, 5.2380, 5.5680, 5.8030, 5.9470, 6.0000, 5.9650, 5.8360, 5.5880, 5.1390, 4.5150, 3.7670, 2.9440, 2.0830, 1.2340, 0.4740, 0.0000},
	{0.0000, 1.1220, 1.3430, 1.6750, 2.2350, 3.1000, 3.7810, 4.3580, 5.2860, 5.9950, 6.5430, 6.9560, 7.2500, 7.4300, 7.4950, 7.4500, 7.2830, 6.9590, 6.3720, 5.5760, 4.6320, 3.5980, 2.5300, 1.4890, 0.5660, 0.0000},
	{0.0000, 1.3230, 1.5710, 1.9520, 2.6460, 3.6900, 4.5130, 5.2100, 6.3330, 7.1880, 7.8480, 8.3460, 8.7010, 8.9180, 8.9980, 8.9420, 8.7330, 8.3230, 7.5800, 6.5970, 5.4510, 4.2060, 2.9340, 1.7140, 0.6460, 0.0000},
	{0.0000, 1.5250, 1.8040, 2.2400, 3.0450, 4.2690, 5.2330, 6.0520, 7.3690, 8.3760, 9.1530, 9.7380, 10.1540, 10.4070, 10.5000, 10.4340, 10.1860, 9.6920, 8.7930, 7.6100, 6.2510, 4.7960, 3.3240, 1.9240, 0.7170, 0.0000},
	{0.0000, 0.4879, 0.5847, 0.7351, 0.9888, 1.3236, 1.5808, 1.7909, 2.1262, 2.3856, 2.5886, 2.7464, 2.8649, 2.9451, 2.9902, 2.9985, 2.9686, 2.8954, 2.7713, 2.5854, 2.3063, 1.9056, 1.4274, 0.8994, 0.3585, 0.0000},
	{0.0000, 0.6462, 0.7740, 0.9739, 1.3118, 1.7595, 2.1036, 2.3844, 2.8325, 3.1791, 3.4503, 3.6611, 3.8196, 3.9267, 3.9869, 3.9980, 3.9579, 3.8592, 3.6917, 3.4395, 3.0581, 2.5134, 1.8738, 1.1706, 0.4637, 0.0000},
	{0.0000, 0.7240, 0.8669, 1.0915, 1.4715, 1.9759, 2.3637, 2.6800, 3.1848, 3.5752, 3.8807, 4.1182, 4.2967, 4.4174, 4.4853, 4.4977, 4.4525, 4.3410, 4.1514, 3.8652, 3.4308, 2.8123, 2.0917, 1.3013, 0.5139, 0.0000},
	{0.0000, 0.8008, 0.9586, 1.2078, 1.6297, 2.1911, 2.6229, 2.9748, 3.5365, 3.9709, 4.3108, 4.5750, 4.7737, 4.9081, 4.9837, 4.9975, 4.9471, 4.8227, 4.6109, 4.2901, 3.8013, 3.1081, 2.3061, 1.4288, 0.5628, 0.0000},
	{0.0000, 0.9507, 1.1374, 1.4354, 1.9412, 2.6173, 3.1376, 3.5615, 4.2377, 4.7605, 5.1696, 5.4876, 5.7269, 5.8889, 5.9802, 5.9971, 5.9365, 5.7860, 5.5290, 5.1373, 4.5356, 3.6902, 2.7246, 1.6744, 0.6565, 0.0000},
	{0.0000, 1.1651, 1.3925, 1.7625, 2.3940, 3.2443, 3.8994, 4.4325, 5.2826, 5.9396, 6.4537, 6.8536, 7.1547, 7.3588, 7.4747, 7.4966, 7.4209, 7.2307, 6.9048, 6.4020, 5.6210, 4.5399, 3.3270, 2.0209, 0.7880, 0.0000},
	{0.0000, 1.3646, 1.6289, 2.0689, 2.8258, 3.8530, 4.6458, 5.2904, 6.3174, 7.1106, 7.7315, 8.2148, 8.5791, 8.8268, 8.9683, 8.9963, 8.9060, 8.6761, 8.2797, 7.6591, 6.6858, 5.3628, 3.8991, 2.3435, 0.9097, 0.0000},
	{0.0000, 1.5470, 1.8438, 2.3506, 3.2327, 4.4397, 5.3736, 6.1323, 7.3397, 8.2720, 9.0017, 9.5703, 9.9994, 10.2922, 10.4609, 10.4966, 10.3924, 10.1232, 9.6546, 8.9093, 7.7296, 6.1596, 4.4414, 2.6443, 1.0229, 0.0000}
};

double tf6a[][N6P] = {
	{0.0000, 0.4950, 0.5950, 0.7540, 1.0450, 1.4470, 1.7470, 1.9890, 2.3620, 2.6310, 2.8200, 2.9420, 2.9960, 2.9850, 2.9140, 2.7880, 2.6130, 2.3960, 2.1430, 1.8590, 1.5560, 1.2480, 0.9390, 0.6300, 0.3220, 0.0130},
	{0.0000, 0.6580, 0.7910, 1.0030, 1.3910, 1.9300, 2.3320, 2.6560, 3.1550, 3.5150, 3.7660, 3.9260, 3.9950, 3.9780, 3.8780, 3.7050, 3.4680, 3.1760, 2.8370, 2.4570, 2.0550, 1.6470, 1.2400, 0.8330, 0.4250, 0.0180},
	{0.0000, 0.8160, 0.9830, 1.2500, 1.7370, 2.4120, 2.9170, 3.3240, 3.9500, 4.4000, 4.7140, 4.9130, 4.9950, 4.9680, 4.8370, 4.6130, 4.3110, 3.9430, 3.5170, 3.0440, 2.5450, 2.0400, 1.5350, 1.0300, 0.5250, 0.0210},
	{0.0000, 0.9730, 1.1730, 1.4920, 2.0780, 2.8950, 3.5040, 3.9940, 4.7470, 5.2870, 5.6640, 5.9010, 5.9950, 5.9570, 5.7920, 5.5170, 5.1480, 4.7000, 4.1860, 3.6210, 3.0260, 2.4260, 1.8260, 1.2250, 0.6250, 0.0250},
	{0.0000, 1.2030, 1.4480, 1.8440, 2.5790, 3.6180, 4.3820, 4.9970, 5.9420, 6.6190, 7.0910, 7.3840, 7.4960, 7.4350, 7.2150, 6.8580, 6.3870, 5.8200, 5.1730, 4.4680, 3.7310, 2.9910, 2.2520, 1.5120, 0.7720, 0.0320},
	{0.0000, 1.3510, 1.6410, 2.1220, 3.0270, 4.2910, 5.2280, 5.9790, 7.1290, 7.9510, 8.5200, 8.8710, 8.9980, 8.9200, 8.6470, 8.2100, 7.6360, 6.9470, 6.1650, 5.3160, 4.4340, 3.5510, 2.6710, 1.7850, 0.9200, 0.0402},
	{0.0000, 1.4970, 1.8260, 2.3840, 3.4570, 4.9590, 6.0690, 6.9580, 8.3150, 9.2810, 9.9500, 10.3570, 10.4990, 10.3960, 10.0650, 9.5430, 8.8610, 8.0480, 7.1310, 6.1400, 5.1190, 4.0980, 3.0820, 2.0560, 1.0670, 0.0496},
	{0.0000, 0.4850, 0.5850, 0.7390, 1.0160, 1.3990, 1.6840, 1.9190, 2.2830, 2.5570, 2.7570, 2.8960, 2.9770, 2.9990, 2.9450, 2.8250, 2.6530, 2.4380, 2.1880, 1.9070, 1.6020, 1.2850, 0.9670, 0.6490, 0.3310, 0.0130},
	{0.0000, 0.6460, 0.7780, 0.9830, 1.3530, 1.8630, 2.2450, 2.5590, 3.0470, 3.4140, 3.6810, 3.8660, 3.9720, 3.9980, 3.9210, 3.7570, 3.5240, 3.2340, 2.8970, 2.5210, 2.1170, 1.6980, 1.2780, 0.8580, 0.4380, 0.0180},
	{0.0000, 0.8040, 0.9690, 1.2250, 1.6880, 2.3270, 2.9050, 3.1990, 3.8130, 4.2720, 4.6060, 4.8370, 4.9680, 4.9950, 4.8940, 4.6840, 4.3880, 4.0210, 3.5970, 3.1270, 2.6230, 2.1030, 1.5820, 1.0620, 0.5410, 0.0210},
	{0.0000, 0.9610, 1.1580, 1.4640, 2.0180, 2.7880, 3.3640, 3.8390, 4.5800, 5.1320, 5.5340, 5.8090, 5.9650, 5.9930, 5.8630, 5.6050, 5.2440, 4.8010, 4.2890, 3.7210, 3.1180, 2.5000, 1.8820, 1.2630, 0.6440, 0.0250},
	{0.0000, 1.1930, 1.4360, 1.8150, 2.5080, 3.4770, 4.2020, 4.7990, 5.7320, 6.4230, 6.9260, 7.2700, 7.4630, 7.4870, 7.3130, 6.9780, 6.5170, 5.9560, 5.3110, 4.6000, 3.8470, 3.0840, 2.3210, 1.5580, 0.7950, 0.0320},
	{0.0000, 1.3300, 1.6160, 2.0740, 2.9240, 4.1140, 4.9990, 5.7280, 6.8610, 7.7030, 8.3130, 8.7280, 8.9570, 8.9780, 8.7530, 8.3390, 7.7760, 7.0940, 6.3150, 5.4600, 4.5640, 3.6530, 2.7480, 1.8370, 0.9460, 0.0402},
	{0.0000, 1.4710, 1.7950, 2.3260, 3.3300, 4.7430, 5.7920, 6.6540, 7.9920, 8.9850, 9.7030, 10.1900, 10.4550, 10.4670, 10.1890, 9.6920, 9.0240, 8.2200, 7.3030, 6.3060, 5.2660, 4.2130, 3.1690, 2.1140, 1.0950, 0.0496},
	{0.0000, 0.4640, 0.5630, 0.7180, 0.9810, 1.3130, 1.5910, 1.8240, 2.1940, 2.4740, 2.6870, 2.8420, 2.9450, 2.9960, 2.9920, 2.9250, 2.7930, 2.6020, 2.3640, 2.0870, 1.7750, 1.4370, 1.0830, 0.7270, 0.3700, 0.0130},
	{0.0000, 0.6150, 0.7460, 0.9510, 1.3030, 1.7490, 2.1200, 2.4320, 2.9260, 3.3010, 3.5850, 3.7910, 3.9280, 3.9950, 3.9880, 3.8950, 3.7140, 3.4560, 3.1350, 2.7630, 2.3480, 1.8980, 1.4300, 0.9600, 0.4890, 0.0180},
	{0.0000, 0.7650, 0.9280, 1.1830, 1.6230, 2.1820, 2.6500, 3.0400, 3.6580, 4.1270, 4.4830, 4.7420, 4.9120, 4.9950, 4.9830, 4.8630, 4.6320, 4.3040, 3.8990, 3.4320, 2.9120, 2.3520, 1.7710, 1.1880, 0.6040, 0.0210},
	{0.0000, 0.9130, 1.1060, 1.4140, 1.9420, 2.6140, 3.1760, 3.6470, 4.3920, 4.9560, 5.3830, 5.6930, 5.8970, 5.9950, 5.9770, 5.8280, 5.5440, 5.1430, 4.6540, 4.0910, 3.4670, 2.7980, 2.1060, 1.4130, 0.7190, 0.0250},
	{0.0000, 1.1310, 1.3710, 1.7500, 2.4120, 3.2550, 3.9620, 4.5530, 5.4880, 6.1980, 6.7340, 7.1220, 7.3760, 7.4960, 7.4670, 7.2690, 6.9030, 6.3930, 5.7720, 5.0630, 4.2820, 3.4510, 2.5980, 1.7430, 0.8870, 0.0320},
	{0.0000, 1.2550, 1.5400, 2.0080, 2.8180, 3.8460, 4.7080, 5.4290, 6.5650, 7.4230, 8.0720, 8.5420, 8.8490, 8.9910, 8.9500, 8.7030, 8.2480, 7.6230, 6.8730, 6.0180, 5.0790, 4.0820, 3.0660, 2.0490, 1.0500, 0.0402},
	{0.0000, 1.3840, 1.7080, 2.2560, 3.2150, 4.4280, 5.4470, 6.3000, 7.6400, 8.6510, 9.4150, 9.9670,  10.3270, 10.4910, 10.4360, 10.1330, 9.5870, 8.8460, 7.9620, 6.9580, 5.8620, 4.7040, 3.5310, 2.3540, 1.2120, 0.0496}
};

double ellipse_circumference1(double a, double b)
{
	return 2.0 * M_PI * sqrt((a*a + b*b)/2);
}

double ellipse_circumference2(double a, double b)
{
	double x = (fabs(a)-fabs(b))/(fabs(a)+fabs(b));
	return (fabs(a)+fabs(b))*(1 + 3.0*x*x/(10.0 + sqrt(4.0 - 3*x*x)));
}

double ellipse_get_x(double a, double b, double y)
{
	b=fabs(b); y=fabs(y);
	if(y==0.0) return a;
	if(b==0.0) return 0.0;
	if(y>=b) return 0.0;
	double t = asin(y/b);
	return a*cos(t);
}

double vd_average(const vector<double>&vd)
{
	if(vd.empty()) return 0.0;
	double vd_avg = 0.0;
	for(vector<double>::const_iterator it=vd.begin(); it!=vd.end(); it++) vd_avg += *it;
	return vd_avg/vd.size();
}

void mono_spline(const vector<double>& xi, const vector<double>& yi, double xd, double& yd)
{
// Monotone spline subroutine
// Host must dimension ENDSLP(2), DYDX(2)
// if only one point is in set, return that value
	unsigned n = xi.size()-1;
	if(n == 0) {
		xd = xi[0];
		yd = yi[0];
		return;
	}

	// find points in set that bound xd via binary search
	unsigned lowlim = 0, upplim = n;
	while((upplim - lowlim) > 1) {
		unsigned tstlcn = (upplim + lowlim) / 2;
		if(xi[tstlcn] > xd) upplim = tstlcn;
		else lowlim = tstlcn;
		if(xi[lowlim] == xd) {
			yd = yi[lowlim];
			return;
		}
	}

	if(lowlim==n) lowlim--;
	upplim = lowlim + 1;

	// calculate linear interpolant
	double endslp[2], dydx[2];
	double lnslp = (yi[upplim] - yi[lowlim]) / (xi[upplim] - xi[lowlim]);
	double ylinr = lnslp * (xd - xi[lowlim]) + yi[lowlim];

	// calculate end slopes
	unsigned ptbfr = lowlim;
	unsigned whchend = 1;
	for(unsigned sgmn = 0; sgmn < 2; sgmn++) {
		unsigned pnt1, pnt2, pnt3;
		if(ptbfr > 0 && ptbfr < n - 1)	pnt1 = ptbfr - 1;
		else if (ptbfr > n - 2)				pnt1 = n - 2;
		else										{pnt1 = 0; whchend = 0;}
		pnt2 = pnt1 + 1;
		pnt3 = pnt1 + 2;
		double deltx1 = xi[pnt1] - xi[pnt2];
		double deltx2 = xi[pnt3] - xi[pnt2];
		double delty1 = yi[pnt1] - yi[pnt2];
		double delty2 = yi[pnt3] - yi[pnt2];
		double knst;
		double cfcnt;
		if(deltx2 != deltx1) {
			knst = (delty1 * deltx2 / deltx1 - delty2 * deltx1 / deltx2) / (deltx2 - deltx1);
			cfcnt = (delty1 / deltx1 - delty2 / deltx2) / (deltx1 - deltx2);
		}
		else {
			knst = 0;
			cfcnt = 0;
		}

		endslp[sgmn] = knst - 2 * cfcnt * xi[pnt2] + 2 * cfcnt * xi[ptbfr];
		ptbfr++;
		if(whchend > 0) pnt1++;
	}
	// find dist end slopes put point above linear
	for(unsigned sgmn = 0; sgmn < 2; sgmn++) {
		ptbfr = lowlim + sgmn;
		dydx[sgmn] = endslp[sgmn] * (xd - xi[ptbfr]) + yi[ptbfr] - ylinr;
	}

	// test for above, below or on linear and produce yd
	double posn = dydx[0] * dydx[1];
	if(posn == 0)
		yd = ylinr;
	else if(posn > 0)
		yd = ylinr + posn / (dydx[0] + dydx[1]);
	else
		yd = ylinr + (posn * (2 * xd - xi[lowlim] - xi[upplim])) / ((dydx[0] - dydx[1]) * (xi[upplim] - xi[lowlim]));
}

void linear_intrp(double x1, double y1, double x2, double y2, double xd, double& yd)
{
	double dx = x2-x1;
	double dy = y2-y1;
	if(dx==0.0) return;
	double m = dy/dx;
	double b = y1 - m*x1;
	yd = m* xd + b;
}


void generate_naca6(vector<double>& xcc, vector<double>& yt, unsigned form /* 3 - 7 */, double tc, char type /* A ? */)
{
	unsigned np = N6P;
	unsigned ns = N6S;
	unsigned nt = N6T;
	double mintc = .06;
	double maxtc = .21;
	double tfdata[N6S*N6T][N6P];
	vector<double> tfth(tfth6, tfth6+nt);
	memcpy(tfdata, tf6, sizeof(tf6));

	if(type == 'A') {
		ns = N6AS;
		nt = N6AT;
		mintc = .06;
		maxtc = .21;
		memcpy(tfdata, tf6a, sizeof(tf6a));
		tfth.assign(tfth6A, tfth6A+nt);
	}

	xcc.resize(np);
	yt.resize(np);

	vector<double> t6(ns);
	for(unsigned i=0; i<np; i++) {
		xcc[i] = xc[i] / 100.0;
		if(tc < mintc) {
			for(unsigned j=0; j<ns; j++)
				t6[j] = tfdata[j*nt][i] * tc / mintc / 100.00;
		}
		else if(tc > maxtc) {
			for(unsigned j=0; j<ns; j++)
				t6[j] = tfdata[(j+1)*nt-1][i] * tc / maxtc / 100.00;
		}
		else {
			for(unsigned j=0; j<ns; j++) {
				vector<double> tfthy(nt);
				for(unsigned k = 0; k<nt; k++)
					tfthy[k] = tfdata[k + j*nt][i] / 100.0;
				mono_spline(tfth, tfthy, tc, t6[j]);
			}
		}
		vector<double> forms(ns);
		for(unsigned j=0; j<ns; j++) forms[j] = j+3;
		mono_spline(forms, t6, (double)form, yt[i]);
	}
}

//------------------------------------------------------------------------------
// class Foil
//------------------------------------------------------------------------------
void Foil::get_y(double x, double& u, double& l)
{
	if(xcc.empty() || yt_u.empty() || yt_l.empty()) {
		u = 0.0;
		l = 0.0;
		return;
	}
	mono_spline(xcc, yt_u, x, u);
	mono_spline(xcc, yt_l, x, l);
}

void NACA4Foil::get_y(double x, double& u, double& l)
{
	u = 5.0*(0.2969*sqrt(x) - 0.126*x - 0.3516*x*x + 0.2843*x*x*x - 0.1015*x*x*x*x);
	l = -u;
}

void NACA6Foil::set_params(unsigned form, double thickness, char type)
{
	if(form<3) form = 3;
	else if(form>7) form=7;
	if(thickness<0.06) thickness = 0.06;
	if(thickness>0.21) thickness = 0.21;
	if(type!='A') type = '-';
	xcc.clear();
	yt_u.clear();
	yt_l.clear();
	generate_naca6(xcc, yt_u, form, thickness, type);
	yt_l.resize(yt_u.size());
	for(unsigned i=0; i<yt_u.size(); i++) {
		yt_u[i] = yt_u[i]/thickness;
		yt_l[i] = -yt_u[i];
	}
}

void ExternalFoil::set_params(vector<double>& xu, vector<double>& yu, vector<double>& xl, vector<double>& yl, bool flatten)
{
	if(xu.empty() || yu.empty() || xl.empty() || yl.empty()) return;
	if(xu.size() >= xl.size()) {
		xcc  = xu;
		yt_u = yu;
		yt_l.resize(yt_u.size());
		for(unsigned i = 0; i < xcc.size(); i++)
			mono_spline(xl, yl, xcc[i], yt_l[i]);
	}
	else {
		xcc  = xl;
		yt_l = yl;
		yt_u.resize(yt_l.size());
		for(unsigned i = 0; i < xcc.size(); i++)
			mono_spline(xu, yu, xcc[i], yt_u[i]);
	}

	// normalize (so that the max distance between yt_u and yt_l = 1 (as it is for other foils)
	double max_d = 0.0;
	for(unsigned i = 0; i < xcc.size(); i++) {
		double dt = fabs(yt_u[i]-yt_l[i]);
		if(dt>max_d) max_d = dt;
	}
	if(max_d>0.0) {
		for(unsigned i = 0; i < xcc.size(); i++) {
			yt_u[i] /= max_d;
			yt_l[i] /= max_d;
		}
	}
	if(flatten) {
		for(unsigned i = 0; i < xcc.size(); i++) {
			yt_u[i] = fabs(yt_u[i]-yt_l[i])/2;
			yt_l[i] = -yt_u[i];
		}
	}
}

//------------------------------------------------------------------------------
// class profile_data & profile
//------------------------------------------------------------------------------
void profile_data::calc(double whr, double prev_volume)
{
}

void profile::calc()
{
	Foil *foil;

	if(!xu.empty()) foil = new ExternalFoil(xu, yu, xl, yl, flatten);
	else if(foil_name.size()>=3 && foil_name[0]=='6') {
		foil = new NACA6Foil(foil_name[1]-'0', HLRatio, foil_name[2]);
	}
	else {
		foil_name = "00XX";
		foil = new NACA4Foil();
	}
	wetted_surface = 0.0;
	volume = 0.0;

	max_width = 0.0;
	max_height_u = 0.0;
	max_height_l = 0.0;
	max_chord_pos = 0;

	for(iterator it=begin(); it!=end(); it++) {
		foil->get_y((double)(it - begin())*step, it->height_u, it->height_l);
		it->height_u *= HLRatio;
		it->height_l *= HLRatio;
		double half_height = fabs(it->height_u - it->height_l) / 2.0;
		it->width = half_height * WHRatio;

		if(fabs(it->width) > fabs(max_width)) {
			max_width = it->width;
			max_chord_pos = it-begin();
		}
		if(fabs(it->height_u) > fabs(max_height_u)) max_height_u = it->height_u;
		if(fabs(it->height_l) > fabs(max_height_l)) max_height_l = it->height_l;

		it->area = M_PI * it->width * half_height;
		it->volume = volume + it->area*step;
		volume = it->volume;

//		it->circumference = 2.0 * M_PI * sqrt((it->width*it->width + half_height*half_height)/2);
//		it->circumference = M_PI * sqrt((it->width*it->width + it->height_u*it->height_u)/2) + M_PI * sqrt((it->width*it->width + it->height_l*it->height_l)/2);
		
//		it->circumference = ellipse_circumference1(it->width, half_height);
		it->circumference = (ellipse_circumference1(it->width, it->height_u)+ellipse_circumference1(it->width, it->height_l))/2.0;
		
		it->wetted_surface = wetted_surface + it->circumference*step;
		wetted_surface = it->wetted_surface;
	}

	double half_vol = volume/2.0;
	for(iterator it=begin(); it!=end(); it++) {
		if(half_vol < it->volume) {
			gcentre = (double)(it - begin())*step;
			break;
		}
	}
}

bool get_number_pair(istream& is, double& x, double& y)
{
	if(!is) return false;

	char linebuf[512];
        QString str;
        QString sx, sy;
        char t1[16];
        char t2[16];
        int ret;

	for(;;) {
                is.getline(linebuf, sizeof linebuf);
		if(!is) return false;
		if(strlen(linebuf)>0) break;
	}
        str = QString(linebuf);
        /*
	char *end_ptr;
	x = strtod(linebuf, &end_ptr);
	if(end_ptr==NULL) return false;
	y = strtod(end_ptr, &end_ptr);
	if(end_ptr==NULL) return false;
        */

        ret = sscanf(linebuf, "%s  %s", t1, t2);
        sx = QString(t1);
        sy = QString(t2);

        x = sx.toDouble();
        y = sy.toDouble();

	return true;
}

bool profile::import_foil(string filename, int format)
{
	xu.clear();
	yu.clear();
	xl.clear();
	yl.clear();
	foil_name = "";

	char linebuf[512];

	ifstream ifs(filename.c_str());
	if(!ifs) return false;
	ifs.getline(linebuf, sizeof linebuf);
	if(!ifs) return false;
	foil_name = linebuf;

	if(format==0) {
            if(!import_uiuc_foil(ifs)) return false;
        } else {
            if(!import_nasg_foil(ifs)) return false;
	}

	if(xu.empty()||yu.empty()||xl.empty()||yl.empty()) return false;
	if(xu.back()==0.0 || xl.back()==0.0) return false; // to prevent a disaster in normalizing routine...

	// normalize x so that the range is from 0 to 1
	for(vector<double>::iterator it=xu.begin(); it!=xu.end(); it++) *it /= xu.back();
	for(vector<double>::iterator it=xl.begin(); it!=xl.end(); it++) *it /= xl.back();

	// check to see if yl is bigger than yu and if so swap them
	double yu_avg = vd_average(yu);
	double yl_avg = vd_average(yl);

	if(yu_avg<yl_avg) {
		xu.swap(xl);
		yu.swap(yl);
	}

	if(fabs(xu[0]) > 0.0) {
		xu.insert(xu.begin(), 0.0);
		yu.insert(yu.begin(), 0.0);
	}
	if(fabs(xl[0]) > 0.0) {
		xl.insert(xl.begin(), 0.0);
		yl.insert(yl.begin(), 0.0);
	}

	return true;
}

bool profile::import_uiuc_foil(istream& ifs)
{
	double num_u, num_l;
	double x, y;

	if(!get_number_pair(ifs, num_u, num_l)) return false;
	if(num_u<4) return false; // assume at least 4 points define the foil shape - less means it's probably in a different format
	if(num_u!=floor(num_u)) return false; // there should be no decimal points in the first number pair as it simply defines number of samples
	if(num_l!=0) { // num_l can be 0 (in which case the inverted "u" samples are used, but if it is not 0, then the same restrictions apply
		if(num_l<4) return false;
		if(num_l!=floor(num_l)) return false;
	}

	xu.resize(num_u);
	yu.resize(num_u);
	for(int i=0; i<num_u; i++) {
		if(!get_number_pair(ifs, x, y)) return false;
                
		if(i>0 && xu[i-1]>=x) return false; // x[] **MUST** be an increasing sequence!
		xu[i] = x;
		yu[i] = y;
	}

	if(num_l>0) {
		xl.resize(num_l);
		yl.resize(num_l);
		for(int i=0; i<num_l; i++) {
			if(!get_number_pair(ifs, x, y)) return false;
			if(i>0 && xl[i-1]>=x) return false; // x[] **MUST** be an increasing sequence!
			xl[i] = x;
			yl[i] = y;
		}
	}
	else {
		xl.resize(num_u);
		yl.resize(num_u);
		for(int i=0; i<num_u; i++) {
			xl[i] = xu[i];
			yl[i] = -yu[i];
		}
	}
	return true;
}

bool profile::import_nasg_foil(istream& is)
{
	double tmp_x, tmp_y;
	vector<double> tmp_vec_x, tmp_vec_y;

	while(get_number_pair(is, tmp_x, tmp_y)) {
		tmp_vec_x.push_back(tmp_x);
		tmp_vec_y.push_back(tmp_y);
	}

	if(tmp_vec_x.size()<3) return false;

	// find where sequence of x is at a minimum (0.0)
	unsigned zero_index;
        for(zero_index=1;
            zero_index<tmp_vec_x.size() && tmp_vec_x[zero_index]<tmp_vec_x[zero_index-1];
            zero_index++);
        if(zero_index==1 || zero_index>=tmp_vec_x.size()) {
            return false;
        }
	zero_index--;

	for(int i=zero_index; i>=0; i--) {
		if(!xu.empty() && xu.back()>=tmp_vec_x[i]) return false; // x[] **MUST** be an increasing sequence!
		xu.push_back(tmp_vec_x[i]);
		yu.push_back(tmp_vec_y[i]);
	}
	// handle the case when the mid-point (i.e. zero_index) may not belong to both sets...
	if(tmp_vec_x[zero_index]>=tmp_vec_x[zero_index+1]) zero_index++;
	for(int i=zero_index; i<tmp_vec_x.size(); i++) {
		if(!xl.empty() && xl.back()>=tmp_vec_x[i]) return false; // x[] **MUST** be an increasing sequence!
		xl.push_back(tmp_vec_x[i]);
		yl.push_back(tmp_vec_y[i]);
	}

	return true;
}


/*


double calc_volume(double wlratio, double hwratio, double length)
{
	double step = 0.0001;
	double max_y = 0.0;
	double max_area = 0.0;
	double volume = 0.0;
	for(double x = 0.0; x<1.0; x+= step) {
		double y = 5.0*length*wlratio*(0.2969*sqrt(x) - 0.126*x - 0.3516*x*x + 0.2843*x*x*x - 0.1015*x*x*x*x);
		double area = M_PI * y * y * hwratio;
		max_y = max(y,max_y);
		max_area = max(area,max_area);
		volume += area*step*length;
	}
	return volume;
}

*/

