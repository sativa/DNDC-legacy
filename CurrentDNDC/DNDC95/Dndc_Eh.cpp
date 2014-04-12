#include "stdafx.h"
#include "Dndcgo.h"
#include "Source_main.h"
#include "Dndc_tool.h"
#include <math.h>

void class_model::soil_oxygen_profile(int l, int T, int rain_hrs, float bypass_f,
						 float air_poro_max, int flood_day, float *temp, int year, int jday, int q, 
						 double water[25][180], float *day_wfps, float *h, float *sts, float *O2,
						 float day_WT, int WTL, float total_ice, float TTT, float *day_rco2, float *co2,
						 int flo_num, float *ice)
{
	float layer_gas_O2_mass, air_O2, V_air_layer, full_O2;
	float dis_volume_O2, dis_O2_mass, layer_liquid_O2;
	float total_pore, actual_poro, L_water;
	float dLO, F_rain;
	int F_surface;
	
	//Surface conditions
	if((plastic_flag==1&&plastic_method==1)||T<=rain_hrs||ice[1]>0.0) F_surface=1;		
	else F_surface=0;

	//pores in a lyaer
	float ps = h[l] * sts[l];
	total_pore = ps * 10000.0;//m3/layer/ha, total pores 

	
	if((T<=rain_hrs&&l<=T)||flood_flag==1&&l>WTL||(l>WTL)||ice[l]>0.0) 
		actual_poro = 0.0;
	else
		actual_poro = total_pore * (1.0 - water[T][l] / ps) + 0.000001;//m3/layer/ha, actual pores
		//actual_poro = total_pore * (1.0 - 0.5 * water[T][l] / ps) + 0.000001;//m3/layer/ha, actual pores
	
	//O2 mass, kg O2/layer
	air_O2 = 0.299251; //g O2/m3 of air = 1000.0 * 0.209476 / 22.4 * 32.0 / 1000.0; [O2 fraction of air: 0.209476]
	full_O2 = air_O2 * total_pore;//g O2/layer/ha
	
	if(jday==1&&T==1) O2[l] = air_O2 * actual_poro;//g O2/layer/ha

	O2[l] = air_O2 * actual_poro;//g O2/layer/ha
	
	if(l>1)//diffusion
	{
		float ave_O2 = (O2[l] + O2[l-1]) * 0.5;
		O2[l-1] = ave_O2;
		O2[l] = ave_O2;
	}

	if(O2[l]<0.0) O2[l] = 0.0;

	if(O2[l]>0.0)
	{	 
		Eh[l] = 626.1 * O2[l] / full_O2;
	}
	else
	{
		if(Eh[l]>0.0) Eh[l] = 0.0;
	
	}

	if(T==1) 
	{
		O2_old[l] = O2[l];
		Eh_old[l] = Eh[l];
	}

	Eh1[l] = Eh[l];
}

void class_model::saturation_Eh(void) //Eh dynamics under flooding
{
	float F_SOC = 0.05;
	float EhSOC=100;
	float fNO3, dNO3, dMn, dFe, dS;

	if(day_WT<0.0) Eh[1] = 620.0;
	else Eh[1] = 100.0 - 1000.0 * day_WT;

	for(int l=1; l<=q; l++)
	{
		if(WTL>l) //unsaturated layer
		{
			Eh[l] = Eh[1];
		}
		else
		{
			/*if(Eh[l]<=250.0)
			{
				O2[l] = 0.0;
				Mn4[l]=0.0;
				Fe3[l] = 0.0;
				SO4[l]=0.0;
			}*/

			float Fdoc = pow(MicrobioIndex, 2.0) * soc[l] / mmm[l];
			
			if(O2[l]>0.0000001)
			{
				O2[l] = 0.0;
				Eh[l] = 0.0;
			}
			else if(no3[l]>0.1)//0000001)
			{
				fNO3 = 0.5 * (Fdoc / (0.0000001 + Fdoc)) * (no3[l] / (0.0001 + no3[l]));

				dNO3 = no3[l] * fNO3;
				no3[l] -= dNO3;
				no3[l] = max(0.0, no3[l]);
				n2[l] += 0.0;//0.5*dNO3;
				nh4[l] += 0.5*dNO3;
				Eh[l] = -5.0;
			}
			else if(Mn4[l]>0.0000001)
			{
				float Q10Mn = 2.4; //! Q10 value for Mn reduction 
				float Km_Mn = 0.15; //! Km (mol/kg soil) with respct to Mn4+.
				float Vmax_Mn = 0.108; //! Vmax (mol/kg/day) of Mn reduction at 30C.
				float Km_DOC_Mn = 0.0005; //! Km (mol/L) with respct to DOC.
					
				dMn = 300.0 * Fdoc * Vmax_Mn * Mn4[l]/(Km_Mn + Mn4[l]) * pow(Q10Mn, (temp[l] - 30.0)/10.0 ); //! DOC effect is provisionally assumed as 0.5.
				Mn4[l] -= dMn;
				Mn4[l] = max(0.0, Mn4[l]);
				Mn2[l] += dMn;
				Eh[l] = -50.0;
			}
			else if(Fe3[l]>0.0000001)
			{
				//! Eh below 300mV is determined by Fe redox process. 
				//! Michaelis-Menten kinetic constants of Fe reduction in paddy soil.
				//! Van Bodegom and Stams (1999) Chemosphere 39:167-182.
				//! Van Bodegom and Scholten (2001) Geochim. Cosmochim. Acta 65:2055-2066. 
				float Q10Fe = 2.4; //! Q10 value for Fe reduction 
				float Km_Fe = 0.15; //! Km (mol/kg soil) with respct to Fe3+.
				float Vmax_Fe = 0.108; //! Vmax (mol/kg/day) of Fe reduction at 30C.
				float Km_DOC_Fe = 0.0005; //! Km (mol/L) with respct to DOC.
					
				dFe = 300.0 * Fdoc * Vmax_Fe * Fe3[l]/(Km_Fe + Fe3[l]) * pow(Q10Fe, (temp[l] - 30.0)/10.0 ); //! DOC effect is provisionally assumed as 0.5.
				Fe3[l] -= dFe;
				Fe3[l] = max(0.0, Fe3[l]);
				Fe2[l] += dFe;
				Eh[l] = -100.0;
			}
			else if(SO4[l]>0.0001)
			{
				//! Kinetic SO4 reduction at Eh <= -100.0 mV
				//! SO4 reduction by Michaelis-Menten kinetics with respect to SO4 and DOC
				//! Vmax = 0.2 mmol SO4/L/day
				//! Km (SO4 ) = 0.08 mmol SO4/L
				//! Km (DOC) = 0.0016 mol DOC/L
				float bulk_density = mmm[l] / 10000.0 / h[l] / 1000.0;//! soil bulk density (kg/dm3)
				float so4_conc = SO4[l] * bulk_density / (sts[l] * day_wfps[l]); //! sts, SO4 conc. (mol/L)
				float so4_reduction = 0.5 * 0.0002*so4_conc/(0.00008+so4_conc);//! DOC effect assumed as 0.5.
								
				dS = 300.0 * Fdoc * so4_reduction * sts[l] * day_wfps[l] / bulk_density;//! sts, SO4 reduction rate (mol/kg/day)
				SO4[l] -= dS;
				SO4[l] = max(0.0, SO4[l]);
				H2S[l] += dS;
				Eh[l] = -150.0;
			}
			else
				Eh[l] = -250.0;

			if(l==1&&SO4[1]<=1.0) 
			{
				if(day_WT>0.2) Eh[1] = -200.0;
				else if(day_WT<=0.2&&day_WT>0.1) Eh[1] = -100.0;
				else if(day_WT<=0.1&&day_WT>0.0) Eh[1] = 0.0;
				else if(day_WT<=0.0) Eh[1] = 100.0;
				else {}
			}
		}		
	}
}

void class_model::Mn_Fe_S_oxidation()
{
	for(int l=1; l<=q; l++)
	{
		if(Eh[l] > 100.0)
		{
			if(Mn2[l] > 0.0) 
			{
				float dMn2 = 0.2*Mn2[l];
				Mn4[l] += dMn2;
				Mn2[l] -= dMn2;
			}
			if(Fe2[l] > 0.0) 
			{
				float dFe2 = 0.2*Fe2[l];
				Fe3[l] += dFe2;
				Fe2[l] -= dFe2;
			}
			
			if(H2S[l] > 0.0) 
			{
				float dH2S = 0.2*H2S[l];
				SO4[l] += dH2S;
				H2S[l] -= dH2S;
			}
		}
	}

}

/*
void class_model::O2_driven_Eh(int l, int T, float *sph, float *O2, float *Eh, float sts, float h) //<--- Eh (hourly time step)
{
	float Fp=7.0;
	
	if(sph[l]>=6.2 && sph[l] <= 7.2) Fp = sph[l];
	else if (sph[l] > 7.2) Fp = 7.2;
	else Fp = 6.2;
	
	if(O2[l] >= 0.0)
	{			
		float O2_mole = (O2[l] * 1000)/ 32 / (h * sts * 10000 * 1000); //kg O2/ha/layer -> mole O2/l
		float bb=(float)log(O2_mole)/2.3026;

		if(bb<-9999.9) bb = -9999.9;

		Eh[l] = 10.0 * (460.0 + 0.2 * (283) / 4 * (bb - 4 * Fp)) + 0.0; //460
		// mV, Eo=820 mV, electron transferre=4
	}
	else
		Eh[l] = -250.0;

	
	Eh[l] = min(650.0, max(-250.0, Eh[l]));

	

	
}

*/