#pragma once

// --- STL Includes --- //
#include <string>

void ReadCropPara1(int *v_CropID, char v_CropName[95][100], float *v_max_biomass_C, float *v_grain_fraction, float *v_leaf_fraction,
    float *v_leafstem_fraction, float *v_root_fraction, float *v_GrainCN, 
    float *v_LeafCN, float *v_ShootCN, float *v_RootCN, float *v_WaterDemand, float *v_OptimumT, 
    float *v_TDD, float *v_Nfixatio, float *v_Vascularity, int *v_Perennial, float *v_TreeAge);

FILE *sopen (char *fname, int mode);
FILE *sopen (char *fname, char *mode);

int FileCopy(char *source, char *dest);

void CN_ratios(float *RCNRVL, float *RCNRL, float *RCNRR, float *RCNB, float *RCNH, float *RCNM, float *PassiveCCN);
