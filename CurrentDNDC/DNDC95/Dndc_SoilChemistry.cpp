#include "stdafx.h"
#include "Dndcgo.h"
#include "Source_main.h"
#include "Dndc_tool.h"

#ifdef MANURE
extern class_manure MData;
#endif

float ttn, KTT=10.0;

void class_model::dndc_decomposition(float* CRB1, float* CRB2, float* newh, float* w_no2, float* w_nh3, 
			float* w_no3, float* w_nh4, float* wurea, float* wpool_no, float till_fact, 
			float* DayFlux, int tilq, FILE*fCB, FILE*fNB, float* w_n2o, float* w_n2,
			int GN, int q, float rcnrvl, float rcnrl, float rcnrr, float rcnb, float rcnh, float rcnm,
			float *soc, float *son, float *SIN, float *rcvl, float *rcl, float *rcr, 
			float *dphum, float *doc, float *nh4, float *no3, float *no2, float *no, float *n2o, 
			float *n2, float *nh3, float *urea, int jday, float *co2, float *clay, float TTT, int flood_flag,
			float *day_wfps, int WTL, float *temp, int LandUse, int FloDays, float *crhl, float *crhr, float *mmm, 
			float *ocini, float *h, float *day_C_mine, float *day_N_mine, float *day_N_assim,
			float DClitter, float *litco22, float *litco23, int *st, float DChumads, float EFFAC, 
			float DChumus, float *sph, int wetland_flag, float wind_speed, float *ice, float *day_soil_nh3,
			float *lai, float *PlantTakeN, float *maxmn, float leaf_water, float cropht, float base_nh3,
			float *day_N_demand, float *day_nup, float *day_fixN, float *day_DWater, float *day_plant_nh3, 
			float *yr_NH3_deposit, float *plantn, float *clay_nh4, float base_clay_N, float max_clay_N,
			float *wrcvl, float *wrcl, float *wrcr, float *wcrb, float *wcrhl, float *wcrhr, float *whumus, float *wtcavai,
		    float *day_ice_N, float *day_ice_doc, float *day_ice_n2o, float *day_ice_n2, float *IniSOC, float *IniSON, 
		    float *EndSON, 
			float day_runoff, float *day_runoff_N, float *day_clay_N, 
			float *dsno3, float *dsnh4, float *dsnh3, float *day_dsurea, float *ds_doc,
			int *crop, float *yr_fixn, float rainfall, float *soil_ph, float *Odoc, float *winertC,
			float *inert_C, int fert_flag, float *root_doc, float *day_O2, float RFM,
			float *SoilDepth, float rcnh2, float *ttoo, float *sop, float *OrgP, float *LabP, float *AdsP,
			float *ComP, float *sts, float day_leach_water)
{
    float  clayc=0.0, krh=0.0, hrh=0.0, DDRF=0.0;
	float drcvl=0.0, DRCB=0.0, DRCB1=0.0, DRCB2=0.0, lit_co2=0.0, sumn=0.0, fb_nh4=0.0;
	float fb_co2=0.0, fh_nh4=0.0, dcbavai=0.0, fh_co2=0.0;
	*litco22=0.0;
	*litco23=0.0;
	
	int    l;
    if ( jday==1 )
    {
		drcvl=(float)0.0;
		DRCB=(float)0.0;
		DRCB1=(float)0.0;
		DRCB2=(float)0.0;
		lit_co2=(float)0.0;
		sumn=(float)0.0;
		fb_nh4=(float)0.0;
		fb_co2=(float)0.0;
		dcbavai=(float)0.0;
		fh_co2=(float)0.0;
		fh_nh4=(float)0.0;		
		
    }

    clayc = (float)(log(.14 / clay[1]) / 2.3026 + 1);
    krh = (float).16 * clayc;
    hrh = (float).006 * clayc;

	if(krh>1.0) krh=1.0;
	if(hrh>1.0) hrh=1.0;

	int BL, EL;

	if(GN>q)//manure
	{
		BL = GN; EL = GN; 
	}
	else
	{
		BL = 1; EL = q;
	}

    for (l=BL; l<=EL; l++ )
    {
		if(l>q) 
		{
			clay[l] = 0.01;
			soil_ph[l] = sph[l];
		}
	
		float Fl=1.0;
		
		Fl = 0.6 * (float)pow(l, -0.1366);
		if(Fl>1.3) Fl = 1.3;
		if(Fl<0.0) Fl = 0.0;

		DDRF = (DRF - 0.02 * clay[l]) * Fl; 
		if ( DDRF < 0.0 ) DDRF = 0.0;
		
		float dev_ph, Frain, D_ph;

		Frain = 0.5; //20.0 * rainfall;
		if(flood_flag==0) 
			D_ph = soil_ph[l];
		else 
			D_ph = 7.0;
		
		if(sph[l]!=D_ph)
		{
		   dev_ph = (sph[l] - D_ph) * Frain;
		   sph[l] -= dev_ph;
		}
		
		// Effect of temp/mois on decomposition 
	
		effect_temp_mois(l, day_wfps, WTL, temp, LandUse, flood_flag, FloDays, &RFM, st, ice, q);
//ttn = MData.Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);


		// Redefine daily microbes/humads 
		redefine_microbe(l, RFM, TTT, jday, soc, rcvl, rcl, rcr, CRB1, CRB2, crhl,
				crhr, dphum, mmm, clay, ocini, no2, no3);
	    
//ttn = MData.Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
		
		// Decomposition of surface litter 
		//if(l==1) decomp_of_litter(l, co2, nh4, h, temp, day_wfps, day_C_mine, day_N_assim, &DRCB, DRCB1, DRCB2, RFM);
	
		// Decomposition of very labile litter 
		if(rcvl[l]>0.0)
		{
			rcvl_decomp(l, &drcvl, RFM, DDRF, till_fact, &DRCB1, tilq,
				DClitter, rcvl, nh4,
				co2, CRB1, CRB2, day_C_mine, day_N_mine, day_N_assim, rcnrvl, rcnb, day_O2, flood_flag);
		}

		// Decomposition of labile litter 
		if(rcl[l]>0.0)
		{
			rcl_decomp(l, RFM, DDRF, till_fact, &DRCB2, tilq, DClitter, rcl, rcnrl, rcnb, nh4,
				no3, co2, CRB1, CRB2, day_C_mine, day_N_mine, day_N_assim, litco22, day_O2, h, flood_flag);
		}
//ttn = MData.Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);

		// Decomposition of resistant litter 
		if(rcr[l]>0.0)
		{
			rcr_decomp(l, RFM, DDRF, till_fact, DRCB1, DRCB2,
				&lit_co2, drcvl, CRB1, CRB2, &DRCB, &sumn, tilq, litco22,
				litco23, DClitter, rcr, nh4, co2, day_C_mine, day_N_mine, day_N_assim,
				rcnrvl, rcnb, rcnrr, no3, rcnrl, day_O2, h, q, flood_flag, doc);
		}

//ttn = MData.Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);

		// Life and death of soil microbes 
		decomp_microbes(l, RFM, DDRF, till_fact, CRB1, CRB2,&fb_nh4, &fb_co2, &dcbavai, tilq,
			st, nh4, rcnb, crhr, crhl, co2, day_C_mine, day_N_mine, day_N_assim, DChumads, EFFAC, rcnh,
			doc, q, day_O2, rcnh2);
//ttn = MData.Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);

		//if(nh4[l]<0.0) nh4[l]=0.0;
		// Decomposition of humads (this function has error source) 
		decomp_humads(l, RFM, DDRF, till_fact, hrh, krh, &fh_co2,newh, &fh_nh4, CRB1, CRB2, dcbavai,DRCB, tilq,
			clay, DChumads, crhr, crhl, nh4, rcnh, rcnb, co2, doc, temp, day_C_mine, day_N_mine, day_N_assim, 
			st, dphum, rcnm, Odoc, q, day_O2, rcnh2, OrgP, LabP, ComP);
//ttn = MData.Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);

		decomp_humus(l, DDRF, tilq, till_fact, clay, h, DChumus, dphum, nh4, co2, rcnm, 
					day_C_mine, day_N_mine, day_N_assim, day_O2, SoilDepth, temp, RFM, q,
					flood_flag, TTT, day_leach_water, doc);//(this function has error source) 
			
//ttn = MData.Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
				
		urea_hydrolysis(l, urea, temp, day_wfps, nh4, sph, h, 0, ttoo, 0, flood_flag, q);
//ttn = MData.Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
	
		// NH4+/NH3(l) equilibrium in liquid phase 
		//if(l<=q) 
			nh43_equil(l, temp, sph, nh4, nh3, q, h, day_wfps, flood_flag);
//ttn = MData.Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
	
		// NH3 volatilization from soil 
		//if(l<=q) 
			nh3_volatilization(l, temp, flood_flag, wetland_flag, nh3, q, wind_speed,
				day_wfps, ice, day_soil_nh3, nh4, jday);
//ttn = MData.Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);

		// NH4+ adsorption on clay 
        if(l<=q)
			clay_adsorption(l, RFM, nh4, clay_nh4, base_clay_N, max_clay_N, q, clay);

//ttn = MData.Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
	
		//Chemodenitrification
		//if(sph[l]<4.5&&TTT>0.0) Chemodenitrification(1);

/*		if(doc[l]>0.0)
		{
			co2[l] += (0.1 * doc[l]);
			doc[l] *= 0.9;
		}
*/
		earth_worm(l, RFM, h, rcvl, rcl, rcr);

		// Recover layer lables 
		recover_layer_lables(l, CRB1, CRB2, crhl, crhr, ocini);

		//DOC consumption in soil
		if(l<=q)//&&doc[l]>0.0)
		{
			//float TB = CRB1[l]+CRB2[l];
			//float BAC = temp[l]/(40.0+temp[l]) * TB/(500.0+TB);
			float VRmax = temp[l]/(40.0+temp[l]) * day_wfps[l]/(0.3+day_wfps[l]) * (1.0-clay[l])/(0.5+(1.0-clay[l]));
			
			float BAC = VRmax * doc[l]/(0.5+doc[l]) * O2[l]/(0.1+O2[l]);
			BAC = max(0.0, min(1.0, BAC));
			float dDOC = BAC * doc[l];
			co2[l] += dDOC;
			doc[l] -= dDOC;
			day_DOC_cons += dDOC;

			//day_O2[l] += dDOC;
			//if(day_O2[l]<0.0) day_O2[l]=0.0;
		}
		else
		{
#ifdef MANURE
			float TB = CRB1[l]+CRB2[l];
			float BAC = 0.0;

			if(l<=q+3) //feedlots
			{				
				BAC = temp[l]/(40.0+temp[l]) * TB/(500.0+TB);
			}
			else if(l<=q+4) //compost
			{
				BAC = temp[l]/(40.0+temp[l]) * TB/(500.0+TB);
			}
			else if(l<=q+5) //lagoon
			{
				float Assimilation = 1.0 - (float)pow((float)MData.lagoon_days,1.1) / 365.0;

				Assimilation = min(1.0, max(0.0, Assimilation));

				BAC = 0.5 * temp[l]/(500.0+temp[l]) * TB/(100.0+TB) * Assimilation;

				if(MData.lagoon_days<80)
				{
					float dHH = (float)MData.lagoon_days/800.0 * crhr[l];
					doc[l] += dHH;
					crhr[l] -= dHH;
					if(l<=q) day_DOC_prod += dHH;
				}
				
				//doc[l] -= ddC;

				
			}
			
			if(till_fact>1.0) BAC *= 4.0;

			BAC = max(0.0, min(1.0, BAC));
			float dDOC = BAC * doc[l];
			co2[l] += dDOC;
			doc[l] -= dDOC;
			if(l<=q) day_DOC_cons += dDOC;
#endif			
		}

		// Daily profile accumulation 
		if(l<=q)
		{
			profile_accum(l, newh, sumn, fb_nh4, fh_nh4, w_no2, w_no3,
				w_nh4, w_nh3, wurea, wpool_no, fCB, fNB, w_n2o, w_n2,
				wrcvl, wrcl, wrcr, wcrb, wcrhl, wcrhr, whumus, wtcavai,
			   day_ice_N, day_ice_doc, day_ice_n2o, day_ice_n2, IniSOC, IniSON, 
			   EndSON, 
			   soc, rcvl, rcl, rcr, CRB1, CRB2, crhl, crhr,
			   dphum, doc, no3, nh4, nh3, n2, no, n2o, urea,
			   flood_flag, jday, q,
			   no2, winertC);
		}
//ttn = MData.Total_SOCSON(24, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2);

		if(day_runoff>0.0)
		{
			float fw=day_runoff * 1.0;//0.01
			fw = min(0.9, max(0.0, fw));
			*day_runoff_N += fw * no3[l];

			no3[l] *= (1.0 - fw);
			//nh4[l] *= (1.0 - fw);
		}

		/*if(inert_C[l]>0.0)
		{
			co2[l] += inert_C[l]*0.001;
			inert_C[l] *= 0.999;

			day_O2[l] += inert_C[l]*0.001;
			if(day_O2[l]<0.0) day_O2[l]=0.0;
		}*/

		if(l==q) P_adsorption(q, LabP, AdsP, h, day_wfps, sts, mmm);//P adsorption 
 
	} //end of layer loop

	*day_clay_N = 0.0;

	if(BL!=EL) for(int vv=1; vv<=q; vv++) *day_clay_N += clay_nh4[vv];

	//float wsoc = *wcrb + *wcrh; 
	//float soilt=max(0.0, temp[1]);
	//float SumRootDOC = root_doc[1] + root_doc[2] + root_doc[3];

    *dsnh3 = 0.0;
    *day_dsurea = 0.0;
    *ds_doc = 0.0;

	if(BL!=EL)
	{
		//float SoilDepth = 0.0;
		for(l=1; l<=q; l++)
		{
			if(SoilDepth[l]<=0.1)
			{
				dsno3[1] += no3[l];
				dsnh4[1] += nh4[l];
				dsClayN[1] += clay_nh4[l];
				*dsnh3 += nh3[l];
				*day_dsurea += urea[l];
				*ds_doc += doc[l];

				day_nitrif[1] += day_nitrification[l];
				day_denitrif[1] += day_denitrification[l];
				day_decomp[1] += co2[l];

			}
			else if(SoilDepth[l]<=0.2)
			{
				dsno3[2] += no3[l];
				dsnh4[2] += nh4[l];
				dsClayN[2] += clay_nh4[l];
				*day_dsurea += urea[l];
				day_nitrif[2] += day_nitrification[l];
				day_denitrif[2] += day_denitrification[l];
				day_decomp[2] += co2[l];
			}
			else if(SoilDepth[l]<=0.3)
			{
				dsno3[3] += no3[l];
				dsnh4[3] += nh4[l];
				dsClayN[3] += clay_nh4[l];
				*day_dsurea += urea[l];
				day_nitrif[3] += day_nitrification[l];
				day_denitrif[3] += day_denitrification[l];
				day_decomp[3] += co2[l];
			}
			else if(SoilDepth[l]<=0.4)
			{
				dsno3[4] += no3[l];
				dsnh4[4] += nh4[l];
				dsClayN[4] += clay_nh4[l];
				*day_dsurea += urea[l];
				day_nitrif[4] += day_nitrification[l];
				day_denitrif[4] += day_denitrification[l];
				day_decomp[4] += co2[l];
			}
			else if(SoilDepth[l]<=0.5)
			{
				dsno3[5] += no3[l];
				dsnh4[5] += nh4[l];
				dsClayN[5] += clay_nh4[l];
				*day_dsurea += urea[l];
				day_nitrif[5] += day_nitrification[l];
				day_denitrif[5] += day_denitrification[l];
				day_decomp[5] += co2[l];
			}
			else
			{
				dsno3[6] += no3[l];
				dsnh4[6] += nh4[l];
				dsClayN[6] += clay_nh4[l];
				*day_dsurea += urea[l];
				day_nitrif[6] += day_nitrification[l];
				day_denitrif[6] += day_denitrification[l];
				day_decomp[6] += co2[l];
			}

			//co2[l] = 0.0;

		}

/*
		for ( l = 1; l <= (int)(0.1 / h[1]); l++ )
		{
			dsno3[1] += no3[l];
			dsnh4[1] += nh4[l];
			dsClayN[1] += clay_nh4[l];
			*dsnh3 += nh3[l];
			*day_dsurea += urea[l];
			*ds_doc += doc[l];
		}

		for ( l = (int)(0.1 / h)+1; l <= (int)(0.2 / h); l++ )
		{
			dsno3[2] += no3[l];
			dsnh4[2] += nh4[l];
			dsClayN[2] += clay_nh4[l];
			*day_dsurea += urea[l];
		}

		for ( l = (int)(0.2 / h)+1; l <= (int)(0.3 / h); l++ )
		{
			dsno3[3] += no3[l];
			dsnh4[3] += nh4[l];
			dsClayN[3] += clay_nh4[l];
			*day_dsurea += urea[l];
		}

		for ( l = (int)(0.3 / h)+1; l <= (int)(0.4 / h); l++ )
		{
			dsno3[4] += no3[l];
			dsnh4[4] += nh4[l];
			dsClayN[4] += clay_nh4[l];
			*day_dsurea += urea[l];
		}

		for ( l = (int)(0.4 / h)+1; l <= (int)(0.5 / h); l++ )
		{
			dsno3[5] += no3[l];
			dsnh4[5] += nh4[l];
			dsClayN[5] += clay_nh4[l];
			*day_dsurea += urea[l];
		}

		for ( l = (int)(0.5 / h)+1; l <= q; l++ )
		{
			dsno3[6] += no3[l];
			dsnh4[6] += nh4[l];
			dsClayN[6] += clay_nh4[l];
			*day_dsurea += urea[l];
		}
*/
		if(crop[1]==0&&temp[1]>0.0) //fungi N fixation
		{
			float MicroNfix = soc[1] * 0.0000001 * temp[1];

			for(int i=4;i<=8;i++) nh4[i] += (MicroNfix*0.2);

			yr_fixn[1] += MicroNfix;
		}
	}
//ttn = MData.Total_SOCSON(24, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2);

	
}

void class_model::redefine_microbe(int l, float RFM, 
	float TTT, int jday, float *soc, float *rcvl, float *rcl, float *rcr, float *CRB1, float *CRB2, float *crhl,
	float *crhr, float *dphum, float *mmm, float *clay, float *ocini, float *no2, float *no3)
{
/*	if(TTT>0.0||jday==1)
    {
		soc[l] = rcvl[l] + rcl[l] + rcr[l] + CRB1[l] + CRB2[l] + crhl[l]+crhr[l] + dphum[l];
		ocini[l] = CRB1[l] + CRB2[l] + crhl[l]+crhr[l];

		//CRB1[l] = ocini[l] * RBO * SRB;
		//CRB2[l] = ocini[l] * RBO * (1.0 - SRB);
		CRB1[l] = soc[l] * RBO * SRB;
		CRB2[l] = soc[l] * RBO * (1.0 - SRB);

		if((ocini[l]*0.8) < (CRB1[l] + CRB2[l]))
		{
			float dBC = (CRB1[l] + CRB2[l]) - ocini[l]*0.8;
			float df1=dBC * CRB1[l] / (CRB1[l] + CRB2[l]);
			float df2=dBC * CRB2[l] / (CRB1[l] + CRB2[l]);
			CRB1[l] -=df1; 
			CRB2[l] -=df2;

		}
		
		float day_hdcx = ocini[l] - (CRB1[l] + CRB2[l]);

		crhl[l] = day_hdcx * srh;
		crhr[l] = day_hdcx * (1.0 - srh);		
    }
*/
	soc[l] = rcvl[l] + rcl[l] + rcr[l] + CRB1[l] + CRB2[l] + crhl[l]+crhr[l] + dphum[l];

	if(TTT>0.0)//||jday==1)
    {
		/*soc[l] = rcvl[l] + rcl[l] + rcr[l] + CRB1[l] + CRB2[l] + crhl[l]+crhr[l] + dphum[l];
		float total_humus = CRB1[l] + CRB2[l] + crhl[l]+crhr[l] + dphum[l];		
		float rbo = 0.02 * RFM + 0.0000001;
		float clay_humus = 0.01 * m * clay;
		float CRB;		
		if(total_humus>clay_humus)
			CRB = (total_humus - clay_humus) * rbo;
		else
			CRB = 0.0000001;
		CRB1[l] = CRB * SRB;
		CRB2[l] = CRB * (1.0 - SRB);
		float AHM = crhl[l] + crhr[l];
		ocini[l] = CRB + AHM;
		dphum[l] = total_humus - ocini[l];*/

		double Fhc, ActSOC, CRB, Thc;
		ActSOC = CRB1[l] + CRB2[l] + crhl[l] + crhr[l];
		if(ActSOC<=0.0) Fhc = 0.0;
		else Fhc = crhl[l] / (crhl[l] + crhr[l]);
		CRB = 0.01 * ActSOC;		
		CRB1[l] = CRB * SRB;
		CRB2[l] = CRB * (1.0 - SRB);
		Thc = ActSOC - CRB;
		crhl[l] = Thc * Fhc;
		crhr[l] = Thc - crhl[l];
		double new_ActSOC = CRB1[l] + CRB2[l] + crhl[l] + crhr[l];
		double dActSOC;
		if(new_ActSOC!=ActSOC) 
		{
			dActSOC = new_ActSOC - ActSOC;
			crhr[l] -= dActSOC;
		}	
    }

    no3[l] += no2[l];
    no2[l] = 0.0;
}

void class_model::effect_temp_mois(int l, float *day_wfps, int WTL, float *temp, int LandUse, int flood_flag, 
									int FloDays, float *RFM, int *st, float *ice, int q)
{
    float  RFMM, RFMT, Fpeat;

    if (day_wfps[l]<0.00001) day_wfps[l] = 0.00001;

    //RFMM = (-4.3717 * day_wfps[l] * day_wfps[l] + 5.1364 * day_wfps[l] - 0.5455);
	//RFMM = (-5.9991 * day_wfps[l] * day_wfps[l] + 7.8176 * day_wfps[l] - 1.6204);
	//RFMM = -4.1107* day_wfps[l]* day_wfps[l] + 5.1597* day_wfps[l] - 0.6429;
	RFMM = -2.8516* day_wfps[l]* day_wfps[l]* day_wfps[l] + 1.4936* day_wfps[l]* day_wfps[l] + 1.7699* day_wfps[l] - 0.0301;

	RFMM = max(0.0, RFMM);
	//if(day_wfps[1]<wiltpt) RFMM *= 0.01;
	
/*	if(l>WTL) 
		RFMM *= 0.001;//0.005;
	else
	{
		if(st[1]>=12&&day_wfps[l]>0.7) 
			RFMM *= 0.001;//0.005; 
		else 
			RFMM = RFMM;

		if(ice[l]>0.0) RFMM = 0.0;
	}
*/
    if (temp[l]<=0.0) 
		RFMT = 0.01;
    else 
		RFMT = (-.0014 * temp[l] * temp[l]) + 0.0992 * temp[l] + 0.0198;

#ifdef MANURE
#else
	if(day_WT>0.0)
		RFMT = (-.0014 * SurfaceWaterT * SurfaceWaterT) + 0.0992 * SurfaceWaterT + 0.0198;
#endif
	
    if (RFMT <= 0.0) RFMT = 0.0;
    
	if(LandUse==5) //wetland, anaerobic fermentation
		*RFM = RFMM * RFMT * 1.0;//5.0;// * 20.0 
	else
	{
		/*if(flood_flag==0)
			*RFM = RFMM * RFMT;
		else
		{
			if(FloDays>=1)
			{
				float F_phenol; 

				F_phenol = 1.0 / FloDays;
				*RFM = F_phenol * RFMM * RFMT;
			}
			else
				*RFM = RFMM * RFMT;
			
		}*/

		*RFM = RFMM * RFMT;
	}

	*RFM=min(1.0, max(0.0, *RFM));

	//if(ice[l]>0.0) *RFM = (0.9 - day_wfps[l]) * 0.05;//0.015;//

	if(st[l]==12) *RFM *= 0.05;
	if(st[l]>12) *RFM *= 0.025;//0.1;

#ifdef MANURE
	if(l==q+4) *RFM = *RFM;//compost
	else if(l==q+5) //lagoon
	{
		*RFM = (temp[l] - 8.0) * 0.000007;
	}
	else if(l==q+6) *RFM = RFMT * 0.00001;//anaerobic digester
	else if(l==q+8) *RFM = RFMT * 0.00001;//gutter
	else {}	
#else
	if(Eh[1]<-250.0) *RFM = 0.0;
#endif

	*RFM=min(1.0, max(0.0, *RFM));

	//else Fpeat = 1.0;
	//*RFM *= 0.0;//Fpeat;
    //return( RFM );
}

void class_model::decomp_of_litter(int l, float *co2, float *nh4, float *h, float *temp, float *day_wfps,
								   float *day_C_mine, float *day_N_assim, float *DRCB, float DRCB1, float DRCB2,
								   float RFM)
{
	if(st[l] < 12)
	{
		if((stub1+stub2+stub3)>0.0&&air_temp>0.0)//&&Eh[1]>-200.0)
		{
			float Ftw= air_temp * 0.0005;//0.005
			float FungiLitterDecomposition2, FungiN, FungiMicrobe, AdditionCO22, DRCB3=0.0;
			float Ftill=0.005;//0.0001

			if(flood_flag==1) Ftill = 0.1;
			
			if(flood_flag!=1)
			{
				FungiLitterDecomposition2 = stub1 * Ftill * RFM;
				FungiN = FungiLitterDecomposition2 / rcnrr;
				FungiMicrobe = FungiN * rcnb;
				AdditionCO22 = FungiLitterDecomposition2 - FungiMicrobe;
				co2[l] += AdditionCO22;
				//day_O2[l] += AdditionCO22;
				//if(day_O2[l]<0.0) day_O2[l]=0.0;
				stub1 -= FungiLitterDecomposition2;
				CRB1[l] += (FungiMicrobe * SRB);
				CRB2[l] += (FungiMicrobe * (1.0 - SRB));
				*day_C_mine += AdditionCO22;
				*day_N_assim += FungiN;
				DRCB3 += FungiMicrobe;

				day_addC += FungiMicrobe;
				day_inStubN += (FungiMicrobe / rcnb);
				yr_addtc += FungiMicrobe;
				yr_addtn += (FungiMicrobe / rcnb);

				FungiLitterDecomposition2 = stub2 * Ftill * RFM;
				FungiN = FungiLitterDecomposition2 / rcnrr;
				FungiMicrobe = FungiN * rcnb;
				AdditionCO22 = FungiLitterDecomposition2 - FungiMicrobe;
				co2[l] += AdditionCO22;
				//day_O2[l] += AdditionCO22;
				//if(day_O2[l]<0.0) day_O2[l]=0.0;
				stub2 -= FungiLitterDecomposition2;
				CRB1[l] += (FungiMicrobe * SRB);
				CRB2[l] += (FungiMicrobe * (1.0 - SRB));
				*day_C_mine += AdditionCO22;
				*day_N_assim += FungiN;
				DRCB3 += FungiMicrobe;

				day_addC += FungiMicrobe;
				day_inStubN += (FungiMicrobe / rcnb);
				yr_addtc += FungiMicrobe;
				yr_addtn += (FungiMicrobe / rcnb);
			
				FungiLitterDecomposition2 = stub3 * Ftill * RFM;
				FungiN = FungiLitterDecomposition2 / rcnrr;
				FungiMicrobe = FungiN * rcnb;
				AdditionCO22 = FungiLitterDecomposition2 - FungiMicrobe;
				co2[l] += AdditionCO22;
				//day_O2[l] += AdditionCO22;
				//if(day_O2[l]<0.0) day_O2[l]=0.0;
				stub3 -= FungiLitterDecomposition2;
				CRB1[l] += (FungiMicrobe * SRB);
				CRB2[l] += (FungiMicrobe * (1.0 - SRB));
				*day_C_mine += AdditionCO22;
				*day_N_assim += FungiN;
				DRCB3 += FungiMicrobe;

				day_addC += FungiMicrobe;
				day_inStubN += (FungiMicrobe / rcnb);
				yr_addtc += FungiMicrobe;
				yr_addtn += (FungiMicrobe / rcnb);
			}

			//*DRCB = DRCB3; //DRCB1 + DRCB2 + 

/*			float dS1, dS2, dS3;
			//Ftw = temp[1] / 40.0 * day_wfps[1]; 
			Ftw = max(0.0, min(Ftw, 0.5));

			dS1 = stub1 * Ftw;
			co2[l] += dS1;
			day_O2[l] += dS1;
			if(day_O2[l]<0.0) day_O2[l]=0.0;
			nh4[l] += (dS1 / rcnrvl);
			stub1 -= dS1;
			if ( stub1 <= 0.0000001 ) stub1 = 0.0;

			dS2 = stub2 * Ftw * 0.5;
			co2[l] += dS2;
			day_O2[l] += dS2;
			if(day_O2[l]<0.0) day_O2[l]=0.0;
			nh4[l] += (dS2 / rcnrl);
			stub2 -= dS2;
			if ( stub2 <= 0.0000001 ) stub2 = 0.0;

			dS3 = stub3 * Ftw * 0.1;
			co2[l] += dS3;
			day_O2[l] += dS3;
			if(day_O2[l]<0.0) day_O2[l]=0.0;
			nh4[l] += (dS3 / rcnrr);
			stub3 -= dS3;
			if ( stub3 <= 0.0000001 ) stub3 = 0.0;

			//day_inStubN += dS1 / rcnrvl + dS2 / rcnrl + dS3 / rcnrr;
*/
		}	

	}
	else
	{
		rcvl[l] += stub1;
		rcl[l] += stub2;
		rcr[l] += stub3;
		stub1 = 0.0;
		stub2 = 0.0;
		stub3 = 0.0;
	}
}

void class_model::rcvl_decomp(int l, float* drcvl, float RFM, float DDRF, float till_fact, float* DRCB1, int tilq,
							  float DClitter, float *rcvl, float *nh4, float *co2, float *CRB1, float *CRB2, 
							  float *day_C_mine, float *day_N_mine, float *day_N_assim, float rcnrvl, float rcnb,
							  float *day_O2, int flood_flag)
{
    float sum_drcvl, f_till_fact;

    if (l>=tilq) f_till_fact = 1.0;// + 3
    else  f_till_fact = till_fact;

    // CO2 from rcvl decomposition 
	float r_RFM = max(0.0, min(1.0, RFM));//max(0.7, min(1.0, RFM));
	
	float Frcvl = 2.0 * r_RFM * krcvl * DDRF * DClitter * f_till_fact;//* f_till_fact

	if(flood_flag==1) 
		Frcvl *= KTT;

	if(Frcvl>1.0) Frcvl=1.0;
	if(Frcvl<0.0) Frcvl=0.0;
    
	*drcvl = Frcvl * rcvl[l];
	// microbes formed from rcvl decomposition 
    *DRCB1 = (*drcvl) * EFFRB;
	sum_drcvl = (*drcvl) + (*DRCB1);
	rcvl[l] -= sum_drcvl;

	nh4[l] += (sum_drcvl / rcnrvl);	
	
	//co2[l] += *drcvl;
	//day_O2[l] += *drcvl;
	//if(day_O2[l]<0.0) day_O2[l]=0.0;

	CRB1[l] += ((*DRCB1) * SRB);

    CRB2[l] += ((*DRCB1) * (1.0 - SRB));
	
	nh4[l] -= (*DRCB1 / rcnb);	

    // N demand for microbes formation 
    //DRNB1 = (*DRCB1) / rcnb;
    // suplas N 
    //FreeN1 = sum_drcvl / rcnrvl - DRNB1;

	*day_C_mine += *drcvl;
	*day_N_mine += *drcvl/rcnrvl;//1 RCVL decomposition
	*day_N_assim += (*DRCB1 / rcnb);

}

void class_model::rcl_decomp(int l, float RFM, float DDRF, float till_fact,float* DRCB2, int tilq,
							 float DClitter, float *rcl, float rcnrl, float rcnb, float *nh4,
							 float *no3, float *co2, float *CRB1, float *CRB2, float *day_C_mine, float *day_N_mine, 
							 float *day_N_assim, float *litco22, float *day_O2, float *h, int flood_flag)
{
    float PTAN, PDN, p_drcl, p_DRCB2, total_drcl, f_till_fact;

    //if ( l > (tilq + 3) ) f_till_fact = 1.0;
	if (l>=tilq) f_till_fact = 1.0;// + 3
    else  f_till_fact = till_fact;

    /* potential CO2 from rcl decomposition */
	float Frcl = 2.0*RFM * krcl * DDRF * DClitter * f_till_fact* f_till_fact; 

	if(flood_flag) 
		Frcl *= KTT;

	if(Frcl>1.0) Frcl=1.0;

    p_drcl = rcl[l] * Frcl;

    /* potential microbe formation from rcl decomposition */
    p_DRCB2 = p_drcl * EFFRB;

    /* potential total rcl decomposition */
    total_drcl = p_drcl + p_DRCB2;

    /* N release from potential total rcl decomposition */
    PTAN = total_drcl / rcnrl;

    /* potential N demand for microbial formation */
    PDN = p_DRCB2 / rcnb;

    /* net release of N */
	float ddN = PTAN - PDN;

	if(ddN >= 0.0)
	{
		*litco22 = p_drcl;
		*DRCB2 = p_DRCB2;

		//no3[l] += 0.5 * ddN;
		nh4[l] += ddN;

		//day_minen += ddN;
	}
	else
	{
		if((no3[l] + nh4[l]) >= (-ddN))
		{
			*litco22 = p_drcl;
			*DRCB2 = p_DRCB2;
			no3[l] -= no3[l]/ (no3[l] + nh4[l]) * (-ddN);
			nh4[l] -= nh4[l]/ (no3[l] + nh4[l]) * (-ddN);
		}
		else
		{
			float rf = (no3[l] + nh4[l]) / (-ddN);

			*litco22 = rf * p_drcl;
			*DRCB2 = rf * p_DRCB2;

			no3[l] = 0.0;
			nh4[l] = 0.0;
		}

		//day_minen += ddN;
	}

	rcl[l] -= (*litco22+*DRCB2);
	//CRB1[l] += *DRCB2;
	//co2[l] += *litco22;
	//day_O2[l] += *litco22;
	//if(day_O2[l]<0.0) day_O2[l]=0.0;
	CRB1[l] += ((*DRCB2) * SRB);
    CRB2[l] += ((*DRCB2) * (1.0 - SRB));

	*day_C_mine += *litco22;//net C release
	*day_N_mine += *litco22 / rcnrl;//1 RCL decomposition
	*day_N_assim += *DRCB2 / rcnb;//N assimilation

	if(l<=(int)(0.15/h[1]))
	{
		float FungiLitterDecomposition;
		float Ftill=0.01;	
		FungiLitterDecomposition = rcl[l] * Ftill * (RFM);
		float FungiN = FungiLitterDecomposition / rcnrl;
		float FungiMicrobe = FungiN * rcnb;	
		float AdditionCO21 = FungiLitterDecomposition - FungiMicrobe;
		co2[l] += AdditionCO21;
		day_O2[l] += AdditionCO21;
		if(day_O2[l]<0.0) day_O2[l]=0.0;
		rcl[l] -= FungiLitterDecomposition;
		*day_C_mine += AdditionCO21;
		*day_N_assim += FungiN;
		CRB1[l] += (FungiMicrobe * SRB);
		CRB2[l] += (FungiMicrobe * (1.0 - SRB));
		*DRCB2 += FungiMicrobe;
	}
}

void class_model::rcr_decomp(int l, float RFM, float DDRF, float till_fact, float DRCB1, float DRCB2, float* lit_co2,
		   float drcvl, float *CRB1, float* CRB2, float* DRCB, float* sumn, int tilq, float *litco22,
		   float *litco23, float DClitter, float *rcr, float *nh4,
		   float *co2, float *day_C_mine, float *day_N_mine, float *day_N_assim,
		   float rcnrvl, float rcnb, float rcnrr, float *no3, float rcnrl, float *day_O2, float *h,
		   int q, int flood_flag, float *doc)
{
    float drcra, DRCB3, TN1, TN2, dTN;
    float PTAN, PDN, p_drcr, p_DRCB3, total_drcr, f_till_fact;
	
	TN1 = rcr[l]/rcnrr + (CRB1[l]+CRB2[l])/rcnb + no3[l]+nh4[l];
    //if ( l > (tilq + 3) ) f_till_fact = 1.0;
	if (l>=tilq) f_till_fact = 1.0;// + 3
    else  f_till_fact = till_fact;

    /* potential CO2 from rcr decomposition */
	float Frcr = 2.0*RFM * krcr * DDRF * DClitter * f_till_fact;//2.0

	if(flood_flag==1) Frcr *= KTT;
	
	if(Frcr>1.0) Frcr=1.0;
	if(l==q+1) Frcr *= 1000.0;
	Frcr = min(1.0, max(0.0, Frcr));

    p_drcr = Frcr * rcr[l];

    /* potential microbe formation from rcr decomposition */
    p_DRCB3 = p_drcr * EFFRB;

    /* potential total rcr decomposition */
    total_drcr = p_drcr + p_DRCB3;

    /* N release from potential total rcr decomposition */
    PTAN = total_drcr / rcnrr;

    /* potential N demand for microbe formation */
    PDN = p_DRCB3 / rcnb;

	/* deficiency of N */
	float ddN = PTAN - PDN;

	if(ddN >= 0.0)
	{
		*litco23 = p_drcr;
		DRCB3 = p_DRCB3;

		//no3[l] += 0.5 * ddN;
		nh4[l] += ddN;
		//day_minen += ddN;
	}
	else
	{
		if((no3[l] + nh4[l]) >= (-ddN))
		{
			*litco23 = p_drcr;
			DRCB3 = p_DRCB3;
			float tin = no3[l] + nh4[l];
			no3[l] -= no3[l]/ tin * (-ddN);
			nh4[l] -= nh4[l]/ tin * (-ddN);
		}
		else
		{
			float rf = (no3[l] + nh4[l]) / (-ddN);

			*litco23 = rf * p_drcr;
			DRCB3 = rf * p_DRCB3;

			no3[l] = 0.0;
			nh4[l] = 0.0;
		}

		//day_minen += ddN;
	}

	rcr[l] -= (*litco23+DRCB3);
	rcr[l] = max(0.0, rcr[l]);

	//co2[l] += *litco23;	
	//day_O2[l] += *litco23;
	//if(day_O2[l]<0.0) day_O2[l]=0.0;

	CRB1[l] += (DRCB3 * SRB);
    CRB2[l] += (DRCB3 * (1.0 - SRB));

	TN2 = rcr[l]/rcnrr + (CRB1[l]+CRB2[l])/rcnb + no3[l]+nh4[l];

	dTN = TN2 - TN1;

	nh4[l] -= dTN;
	if(nh4[l]<0.0) nh4[l] = 0.0;

	*day_C_mine += *litco23;//net C release
	*day_N_mine += *litco23 / rcnrr;//1 RCR decomposition
	*day_N_assim += DRCB3 / rcnb;//N assimilation

	drcra = p_drcr;

    //*lit_co2 = drcvl + *litco22 + *litco23;//!!!
	doc[l] += drcvl + *litco22 + *litco23;
	if(l<=q) day_DOC_prod += drcvl + *litco22 + *litco23;

	*sumn = drcvl / rcnrvl + *litco22 / rcnrl + drcra / rcnrr;   /* not accurate */

    *litco22 = 0.0;
	drcvl = 0.0;
	*litco23 = 0.0;

	if(l<=(int)(0.1/h[1])&&air_temp>5.0)
	{
		float FungiLitterDecomposition2;
		float Ftill=0.005;//0.0001
		FungiLitterDecomposition2 = rcr[l] * Ftill * RFM;
		float FungiN = FungiLitterDecomposition2 / rcnrr;
		float FungiMicrobe = FungiN * rcnb;
		float AdditionCO22 = FungiLitterDecomposition2 - FungiMicrobe;
		co2[l] += AdditionCO22;
		//day_O2[l] += AdditionCO22;
		//if(day_O2[l]<0.0) day_O2[l]=0.0;
		rcr[l] -= FungiLitterDecomposition2;
		CRB1[l] += (FungiMicrobe * SRB);
		CRB2[l] += (FungiMicrobe * (1.0 - SRB));
		*day_C_mine += AdditionCO22;
		*day_N_assim += FungiN;
		DRCB3 += FungiMicrobe;
	}

	*DRCB = DRCB1 + DRCB2 + DRCB3; 	 
}



void class_model::decomp_humads(int l, float RFM, float DDRF, float till_fact,
			  float hrh, float krh, float* fh_co2, float* newh, float* fh_nh4,
			  float* CRB1, float* CRB2, float dcbavai, float DRCB, int tilq,
			  float *clay, float DChumads, float *crhr, float *crhl, float *nh4, float rcnh, float rcnb,
			  float *co2, float *doc, float *temp, float *day_C_mine, float *day_N_mine, float *day_N_assim, 
			  int *st, float *dphum, float rcnm, float *Odoc, int q, float *day_O2, float rcnh2,
			  float *OrgP, float *LabP, float *ComP)

{
    float dchr, dchl, fhc, dhbcr, dhbcl, nhumr, nhuml, f_till_fact, k1, k2;
	float crhl_co2, crhr_co2, TN1, TN2, dTN;
 
    //if ( l > (tilq + 3) ) f_till_fact = 1.0;
	if (l>=tilq) f_till_fact = 1.0;// + 3
    else  f_till_fact = (float)pow(till_fact, 2.0);

    // humads decomposition */
	if(l>q) clay[l] = 0.01;
	float Fclay = 0.5 * pow(clay[l], -0.471);
	
	k1 = 0.8 * RFM * hrh * DDRF * f_till_fact * Fclay * DChumads;//0.8
	k2 = 0.8 * RFM * krh * DDRF * f_till_fact * Fclay * DChumads;//0.8

	if(l==(q+5))
	{
		k1 *= 10000.0;
		k2 *= 10000.0;
	}

	if(k1>1.0) k1=1.0;
	if(k2>1.0) k2=1.0;
    
	dchr = k1 * crhr[l];
    dchl = k2 * crhl[l];

	float dPP = k1 * OrgP[l];
	LabP[l] += dPP;
	OrgP[l] -= dPP;

	TN1 = crhr[l]/rcnh2 + crhl[l]/rcnh + nh4[l] + (CRB1[l]+CRB2[l])/rcnb + dphum[l]/rcnm;//new humads

	crhr[l] -= dchr;
    crhl[l] -= dchl;

	crhr_co2 = dchr * 0.4;//0.5
	//co2[l] += crhr_co2;
	//day_O2[l] += crhr_co2;
	//if(day_O2[l]<0.0) day_O2[l]=0.0;
	doc[l] += crhr_co2;
	if(l<=q) day_DOC_prod += crhr_co2;

	nh4[l] += (crhr_co2 / rcnh2);

	// efficiency */
	dhbcr = dchr * 0.2;
	// new microbe 
	CRB1[l] += (dhbcr * SRB);
	CRB2[l] += (dhbcr * (1.0 - SRB));
	nhumr = dchr - (dhbcr + crhr_co2);
	dphum[l] += nhumr;

	crhl_co2 = dchl * 0.6;//0.8
	//co2[l] += crhl_co2;
	//day_O2[l] += crhl_co2;
	//if(day_O2[l]<0.0) day_O2[l]=0.0;
	doc[l] += crhl_co2;
	if(l<=q) day_DOC_prod += crhl_co2;

	nh4[l] += (crhl_co2 / rcnh);
	// efficiency */
	dhbcl = dchl * 0.15;
	// new microbe 
	CRB1[l] += (dhbcl * SRB);
	CRB2[l] += (dhbcl * (1.0 - SRB));
	nhuml = dchl - (dhbcl + crhl_co2);
	dphum[l] += nhuml;

	TN2 = crhr[l]/rcnh2 + crhl[l]/rcnh + nh4[l] + (CRB1[l]+CRB2[l])/rcnb + dphum[l]/rcnm;
	dTN = TN2 - TN1;
	nh4[l] -= dTN;

	nh4[l] = max(0.0, nh4[l]);
		
	float humad_co2 = crhr_co2 + crhl_co2;
	*day_C_mine += humad_co2;
	*day_N_mine += (crhl_co2/rcnh + crhr_co2/rcnh2);//1 humads decomposition

/*    if ( temp[l] >= 0.0)
    {
       Odoc[l] += (dcbavai + dhbcr + dhbcl + DRCB);
	   
    }
    else
    {
    }*/
}

void class_model::decomp_humus(int l, float DDRF, int tilq, float till_fact,
		float *clay, float *h, float DChumus, float *dphum, float *nh4, float *co2, float rcnm, 
		float *day_C_mine, float *day_N_mine, float *day_N_assim, float *day_O2, float *SoilDepth,
		float *temp, float RFM, int q, int flood_flag, float TTT, float day_leach_water,
		float *doc)
{
	float d_humus, d_factor, f_till_fact, Fclay, Fdepth, Ftemp; 
	float TN1, TN2, dTN;

    //d_factor = dphum[l] * 0.000005 - 0.004; 
	//if ( l > (tilq + 3) ) f_till_fact = 1.0;
	if (l>=tilq) f_till_fact = 1.0;// + 3
    else  f_till_fact = till_fact;

	if(l>q) 
	{
		clay[l] = 0.01;
		SoilDepth[l] = 0.01;
	}

	//if(st[l]<12)
		Fclay = 0.1793 * pow(clay[l], -0.471) * 20.0;
	
	Fclay=max(0.0, min(1.0, Fclay));
	
	//if(st==12) Fclay *= 0.1;

	Fdepth = (float)pow(10.0, (-20.0 * SoilDepth[l]) + 1.0);
	Fdepth=max(0.0, min(1.0, Fdepth));

	Ftemp = temp[l] / 25.0;
	Ftemp = min(1.0, max(0.001, Ftemp));

	float SOCf = 1.2E-06 * dphum[l] - 0.0013;
	SOCf = max(0.0, SOCf);
	
	d_factor = SOCf * RFM * DDRF * f_till_fact * Fclay * Fdepth * DChumus * Ftemp;//0.00001  

	if(d_factor < 0.0) d_factor = 0.0;

	d_humus = dphum[l] * d_factor; 

	TN1 = dphum[l]/rcnm + nh4[l];

      dphum[l] -= d_humus;
	  if(dphum[l] < 0.0)
	  {
		  dphum[l] = 0.0;
		  d_humus = 0.0;
	  }

	  nh4[l] += (d_humus / rcnm);
	  //co2[l] += d_humus;
	  //day_O2[l] += d_humus;
	  //if(day_O2[l]<0.0) day_O2[l]=0.0;
	  doc[l] += d_humus;
	  if(l<=q) day_DOC_prod += d_humus;

	  *day_N_mine += (d_humus / rcnm);//6
     
	  *day_C_mine += d_humus;

	  TN2 = dphum[l]/rcnm + nh4[l];

	  dTN = TN2 - TN1;

	  nh4[l] -= dTN;

	  if(nh4[l]<0.0) nh4[l] = 0.0;

	  if(day_leach_water>0.0)
	  {
		float leached_humus = dphum[l] * day_leach_water * 0.001;
		
		dphum[l] -= leached_humus;
		day_leachhu += leached_humus;

		day_leachun = day_leachhu / rcnm;
	  }
		/*
			if(dphum[l] < 0.0)
			  {
				  //AfxMessageBox("2. -humus");
				  dphum[l] = 0.0;
				  leached_humus = 0.0;
			  }
			dphum[l+1] += leached_humus;
		}
		else
			day_leachhu += leached_humus;
	  }*/
}

////////////////////////////////////////
void class_model::NH4_HCO3(void)
{
   int l;
 
	for (l=1; l<=q; l++)
	{
		if(NH4HCO3[l] > 0.0000001)
		{
			float dFF=0.1*sph[l]-0.5;
			dFF = max(0.0, min(0.5, dFF));

			//float Fsf = -3.6735 * (float)pow((float)l, 2.0)/(float)q - 0.7551;
			float Fsf = -3.6735 * (float)l/(float)q - 0.7551;
			Fsf = 6.0 * dFF * (float)pow(10.0, Fsf);

			if(Fsf > 0.5) Fsf = 0.5;

			day_vol_nh3 += Fsf * NH4HCO3[l];

			float dABC = 0.5 * (1.0-Fsf) * NH4HCO3[l];

			nh4[l] += dABC;
			NH4HCO3[l] -= dABC;
		}
		else NH4HCO3[l] = 0.0;
    }
   
   /*else
   {
	   total_ABC = 0.0;

	   for (l=1; l<=q; l++)
	   {
		  total_ABC += NH4HCO3[l];
	   }

	   if (total_ABC == 0.0)
	   {
		  ABC_day = 0;
		  for (l=1; l<=q; l++)
		  {
			old_f_lost[l] = 0.0;
		  }
	   }
	   else
	   {
		 ABC_day += 1;

		 for (l=1; l<=q; l++)
		 {
		   if (ABC_day == 1)
		   {
			ABC[l] = NH4HCO3[l];
		   }

		   if (NH4HCO3[l] > 0.0)
		   {
			  f_lost[l] = 0.01 * ((27.628 + ((temp[l] - 20.0) * 0.374) - (1.0 - day_wfps[l] * 27.41)
						) * (float)log(ABC_day) + (7.6393 + ((temp[l] - 20.0) * 0.886)
						- (1.0 - day_wfps[l]) * 6.17)) * (Eh[l] / 600.0) * (sph[l] / 7.0);

			  if (f_lost[l] < 0.0) f_lost[l] = 0.0;
			  if (f_lost[l] > 1.0) f_lost[l] = 1.0;

			  day_lost = f_lost[l] - old_f_lost[l];

			  if(day_lost < 0.0) day_lost = 0.0;

			  old_f_lost[l] = f_lost[l];

			  ABC_lost = day_lost * ABC[l];

			  if (NH4HCO3[l] >= ABC_lost)
			  {
				NH4HCO3[l] -= ABC_lost;
				day_vol_nh3 += ABC_lost;

			  }
			  else
			  {
				day_vol_nh3 += NH4HCO3[l];
				NH4HCO3[l] = 0.0;
			  }

			  NH4_lost = 0.1 * NH4HCO3[l];
			  nh4[l] += NH4_lost;
			  NH4HCO3[l] -= NH4_lost;
			  if (NH4HCO3[l] < 0.0) NH4HCO3[l] = 0.0;

		   } 
		   else
		   {
				ABC[l] = 0.0;
		   }
		 } // end of layer loop 
	} //if flood_flag = 0
   }*/
}

void class_model::urea_hydrolysis(int l, float *urea, float *temp, float *day_wfps, float *nh4, 
								  float *sph, float *h, int UreaAppDays, float *ttoo, int UreaHydroInhibDays,
								  int flood_flag, int q)
{
	//Urea hydrolysis: CO(NH2)2 + 2H+ + 2H2O -> 2NH4 + H2CO3
	float ddf=0.0;

	if(l==1)
	{
		if(UreaHydroInhibDays>0) UreaHydroInhibDays++;
		if(UreaHydroInhibDays==(int)UreaHydroInhibDu) 
			UreaHydroInhibDays=0;
	}

    if(urea[l]>0.0)
    {
		//*UreaAppDays = *UreaAppDays + 1;

		float eff_temp, urea_hydro, soil_T;

		if(temp[l]<0.0) soil_T = 0.0;
		else soil_T = temp[l];

		/*float Fpo = day_wfps[l];//effect of pores
		float Fst = soil_T / (10.0 + soil_T);
		float Fin = 1.0;//UreaHydroInhibDays, UreaHydroInhibEf, UreaHydroInhibDu

		float ddf = Fpo * Fst * Fin;	*/		
		
		//based on experimental data from Helen Suter, Univ of Melbourne, Australia
		float Fpo = day_wfps[l];//effect of pores
		//float Fclay = 0.79 * clay[1] + 0.71;
		float Fsoc = 5.0 * ttoo[1];//5.0
		Fsoc = min(1.0, max(0.0, Fsoc));
		
		if(UreaHydroInhibDays==0)
		{
			//ddf = (0.02 * soil_T + 0.4) * Fsoc;
			ddf = (0.02 * soil_T + 0.4) * Fsoc;//0.02; 0.1
		}
		else
		{
			ddf = 0.0364 * (float)exp(0.0805 * soil_T) * Fsoc;//0.0364			
			
		}

		if(flood_flag==1) ddf *= 0.9;

		if(l>q) ddf *= 2.0;

#ifdef MANURE
		if(MData.m_Flash[l-q]>=1) ddf =  0.01 * (float)pow(soil_T, 0.9) + 0.2;
		else 
		{
			if(soil_T>1.0) ddf =  0.05 * (float)pow(soil_T, 1.0);
			else ddf =  0.02 * soil_T;
		}

		ddf *= 10.0;
#endif

/*		ddf = 0.07 * (float)pow(UreaAppDays, (0.05*soil_T + 0.75));

		if(UreaHydroInhibDays>0)
		{
			float Fsoc = (-3.8944 * ttoo[1] + 0.0041) * (float)log(UreaAppDays) + (-1.3567 * ttoo[1] + 1.0068);
			float Fclay = 0.79 * clay[1] + 0.71;
			float FpH = 1.5 * sph[1] - 0.24;

			ddf = 1.0 - (1.0 - 0.0092 * (float)exp(0.0805*air_temp * UreaAppDays) * Fsoc * Fclay * FpH);
		}
*/
		ddf = max(0.0, min(1.0, ddf));

		if(ddf>0.0)
		{
			//float TotalUrea = 0.0;
			//for(int ul=1;ul<=q;ul++) TotalUrea += urea[ul];

			urea_hydro = urea[l] * ddf;//0.15
			//float dU = urea_hydro * urea[l] / TotalUrea;
			urea[l] -= urea_hydro;
			/*if(urea[l]<0.0)
			{
				urea[l] = 0.0;
				dU = 0.0;
			}*/
			nh4[l] += urea_hydro;

			//if(flood_flag==0)
			{
				//impact of urea hydrolisis on soil pH		
				double wd1, Kw, V_water, consume_H, hydrogen, mol_hydro_urea, hydroxide;
					
				wd1 = (double)pow(10.0, -15.0);
				Kw = 1.945 * exp(0.0645 * temp[l]) * wd1;//water dissociation constant
				V_water = day_wfps[l] * h[l] * 10000.0 * 1000.0;//liter water/ha in layer l
				mol_hydro_urea = urea_hydro * 1000.0 / 14.0 / V_water; //kg N/layer -> mol N/L
				consume_H = 0.005 * mol_hydro_urea; // soil buffer effect 
				hydrogen = (float)pow(10.0, -sph[l]);//mol H/L
				hydroxide = Kw / hydrogen;//mol OH/L

				hydroxide += consume_H;
				hydrogen = Kw / hydroxide;

				sph[l] = (float)log(hydrogen) / -2.3026;
				sph[l] = max(3.0, min(11.0, sph[l]));
			}
		}
		
    }    
}


void class_model::nh43_equil(int l, float *temp, float *sph, float *nh4, float *nh3, int q, float *h, float *day_wfps, int flood_flag)
{
	//Chemical equilibrium: NH4+ + OH- = NH3 + H2O
	double wd1, wd2, Kw, Ka, hydrogen, hydroxide, V_water, mol_nh4, mol_nh3, old_nh3=nh3[l]; 

	wd1 = (double)pow(10.0, -15.0);
	wd2 = (double)pow(10.0, -5.0);
	Kw = 1.945 * exp(0.0645 * temp[l]) * wd1;//water dissociation constant
	Ka = (1.416 + 0.01357 * temp[l]) * wd2;//NH4+/NH3 equilibrium constant
	hydrogen = (double)pow(10.0, -sph[l]);//mol/L
	hydroxide = Kw / hydrogen;//mol/L

	V_water = day_wfps[l] * h[l] * 10000.0 * 1000.0;//liter water/ha in layer l
	
	mol_nh4 = 1000.0 * nh4[l] / 14.0 / V_water;//kg N -> mol/L
	mol_nh3 = mol_nh4 * hydroxide / Ka;//mol/L

	if((mol_nh4 + mol_nh3)<=0.0)
	{
		nh4[l] = 0.0;
		nh3[l] = 0.0;
	}
	else
	{
		float cvf = mol_nh4 / (mol_nh4 + mol_nh3); 
		float TtN = nh4[l]+nh3[l];
//#ifdef MANURE				
		//if(MData.m_Flash[l-q]>=1) cvf = 0.1;
		//else cvf = 0.1;
//#endif
		//if(flood_flag==1) cvf *= 0.2;
		nh4[l] = TtN * cvf; 
		nh3[l] = TtN - nh4[l];//kg N/ha in layer l		
	}


	////////////////////////////////
    // soil ph change by nh4/nh3 transfer 
/*	float d_nh3 = nh3[l] - old_nh3;
    if (d_nh3 != 0.0 && total_ice < 0.00001)
    {
		sph[l] -= (0.5 * d_nh3);// / V_water);
		sph[l] = max(3.0, min(11.0, sph[l]));
    }*/
}

void class_model::nh3_volatilization(int l, 
	float *temp, int flood_flag, int wetland_flag, float *nh3, int q, float wind_speed, float *day_wfps, 
	float *ice,	float *day_soil_nh3, float *nh4, int jday)
{
    float vol_nh3=0.0, clay_nh3=0.0, soil_T, r_t=0.0;

    if (temp[l] < 0.0) soil_T = 0.0;
    else soil_T = temp[l];

	/////////////////////////////////////////////////////////////
#ifdef MANURE

	if(nh3[l] > 0.0)
	{
		float qk=0.0, BDS=1.0;
		
		if(l>0&&l<=q) r_t = 0.05 * soil_T / (float)l;
		else r_t = 0.075 * temp[l];// * (1.0 + MData.mm_VentRate)
		
		float Fwind = wind_speed / (wind_speed + 3.0);
		
		r_t *= Fwind;

		if(jday==1&&MData.BarnDays==1)
		{
			BDS = 1.0;
			MData.BarnDays = 50;
		}
		else
		{
			if(MData.BarnDays<50) BDS = (float)MData.BarnDays/50.0;
			else BDS = 1.0;
		}

		r_t *= BDS;

		r_t = max(0.1, min(1.0, r_t));
		
		if(l==q+1) //feedlot floor
		{
			if(MData.m_Flash[l-q]==0)//water flush 
				qk = r_t * 0.9;
			else if(MData.m_Flash[l-q]==1) //fresh manure liquid flush
				qk = r_t * 1.1;
			else//-1, no flush
				qk = r_t * 1.5;
			
			qk = max(0.0, min(1.0, qk));
			
			vol_nh3 = qk * nh3[l];
			
			MData.day_house_nh3 += vol_nh3;
			nh3[l] -= vol_nh3;
			if (nh3[l] < 0.0) nh3[l] = 0.0;
		}

		if(l==q+8) //feedlot gutter
		{
			if(MData.m_Flash[l-q]==0)//water flash 
				qk = 0.025 * r_t;
			else if(MData.m_Flash[l-q]==1) //fresh manure liquid flash
				qk = r_t;
			else//-1, no flash
				qk = r_t;// * r_t;

			qk = max(0.0, min(1.0, qk));

			vol_nh3 = qk * nh3[l];

			MData.day_gutter_nh3 += vol_nh3;
			nh3[l] -= vol_nh3;
			if (nh3[l] < 0.0) nh3[l] = 0.0;
		}

		
		if(l==q+4) //compost
		{

			vol_nh3 = nh3[l] * r_t * 0.1 * (1.0 - day_wfps[l] + 0.2);
			MData.day_Compost_nh3 += vol_nh3;
			nh3[l] -= vol_nh3;
			if (nh3[l] < 0.0) nh3[l] = 0.0;
		}
		
		if(l==q+5) //lagoon
		{
			float F_am = 1.0 * r_t * MData.m_LagoonSurfaceArea / MData.m_LagoonCapacity / (1.0 + 5.0 * MData.m_LagoonCover);//0.01
			
			vol_nh3 = nh3[l] * F_am;
			MData.day_lagoon_nh3 += vol_nh3;
			nh3[l] -= vol_nh3;
			if (nh3[l] < 0.0) nh3[l] = 0.0;
		}
		
	}

#endif

	if(l<=q&&nh3[l]>0.0) //field soil
	{
		//effects on gas difussion
		float Fsd = 1.0 * (float)(q-l)/(float)q;//-3.6735 * (float)pow((float)l, 2.0)/(float)q - 0.7551;//effect of depth
		float Fpo = 1.0;// - day_wfps[l];//effect of pores
		float Fwi = 1.5 * wind_speed / (1.0 + wind_speed);//0.5, effect of wind speed
		float Fst = soil_T / (50.0 + soil_T);//10.0

		//if(flood_flag==0)//upland soils, &&wetland_flag==0
		{
			float ddf = 1.0 * Fsd * Fpo * Fwi * Fst;//0.25
			if(flood_flag==1) ddf *= 0.2;
			ddf = max(0.0, min(1.0, ddf));

			vol_nh3 = nh3[l] * ddf;
			nh3[l] -= vol_nh3;

			*day_soil_nh3 += vol_nh3; 
		}    
/*		else
		{
			r_t = 0.0001 * soil_T;//0.05
			if(r_t > 0.1) r_t = 0.1;
			  
			vol_nh3 = nh4[l] * r_t; 

			*day_soil_nh3 += vol_nh3;
			nh4[l] -= vol_nh3;
			if(nh4[l] < 0.0) nh4[l] = 0.0;
		}
*/
	}
	else
	{
		vol_nh3 = 0.0;
	}

	nh4[l] += nh3[l];
	nh3[l] = 0.0;
}

void class_model::recover_layer_lables(int l, float* CRB1, float* CRB2, float* crhl, float* crhr, float* ocini)
{
    ocini[l] = CRB1[l] + CRB2[l] + crhl[l]+crhr[l];	
}

void class_model::earth_worm(int l, float RFM, float *h, float *rcvl, float *rcl, float *rcr)
{
	float EarthWormActivity = (float)pow(0.0001, (float)l);//0.01
	float ConsumedLitter1, ConsumedLitter2,ConsumedLitter3;

	if(l<(int)(0.2/h[1]))
	{
		if(rcvl[l]>rcvl[l+1])
		{
		ConsumedLitter1 = EarthWormActivity * rcvl[l];
		rcvl[l] -= ConsumedLitter1;
		rcvl[l+1] += ConsumedLitter1;
		}
		
		if(rcl[l]>rcl[l+1])
		{
		ConsumedLitter2 = EarthWormActivity * rcl[l];
		rcl[l] -= ConsumedLitter2;
		rcl[l+1] += ConsumedLitter2;
		}

		if(rcr[l]>rcr[l+1])
		{
		ConsumedLitter3 = EarthWormActivity * rcr[l];	
		rcr[l] -= ConsumedLitter3;	

		rcr[l] = max(0.0, rcr[l]);

		rcr[l+1] += ConsumedLitter3;
		}
	}
}

void class_model::clay_adsorption(int l, float RFM, float *nh4, float *clay_nh4, float base_clay_N, float max_clay_N, int q, float *clay)////////N_adsorption
{
    float active_clay_nh4, totalN, FIXRATE, lbcn, lmcn;
	float TotalNH4 = nh4[l] + clay_nh4[l];

	lbcn = base_clay_N * (float)pow(0.5, (float)l);
	lmcn = max_clay_N * (float)pow(0.5, (float)l);

	if(clay_nh4[l] > lbcn) active_clay_nh4 = clay_nh4[l] - lbcn;
	else active_clay_nh4 = 0.0;

	totalN = nh4[l] + active_clay_nh4;

	FIXRATE = 0.99 * (7.2733*clay[l]*clay[l]*clay[l] - 11.22*clay[l]*clay[l] + 5.7198*clay[l] + 0.0263);//0.5

	//FIXRATE =  0.0144 * (float)exp(0.0981*CEC[l]); 

	if ( FIXRATE < 0.01 ) FIXRATE = 0.01;
	if ( FIXRATE > 0.999) FIXRATE = 0.999;

    active_clay_nh4 = FIXRATE * totalN;

	nh4[l] = (1.0 - FIXRATE) * totalN;

	clay_nh4[l] = TotalNH4 - nh4[l];//lbcn + active_clay_nh4;

	if(clay_nh4[l] > lmcn)
	{
		nh4[l] += (clay_nh4[l] - lmcn);
		clay_nh4[l] = lmcn;		
	}

   
	//Old code /////////////////////////////////////////////////////////////
/*	float active_clay_nh4, totalN, FIXRATE, d_clay_nh4, lbcn;

	lbcn = base_clay_N/(float)q;

	if(clay_nh4[l] > lbcn) active_clay_nh4 = clay_nh4[l] - lbcn;
	else active_clay_nh4 = 0.0;

	totalN = nh4[l] + active_clay_nh4;

    FIXRATE = (0.3325*log(clay)/2.30259 + 1.114)*0.98;

	if ( FIXRATE < 0.01 ) FIXRATE = 0.01;
	if ( FIXRATE > 0.999) FIXRATE = 0.999;

    active_clay_nh4 = FIXRATE * totalN;

	nh4[l] = (1.0 - FIXRATE) * totalN;

	clay_nh4[l] = lbcn + active_clay_nh4;
*/
}



void class_model::soilCEC()
{
	float xx = clay[1]*100.0;// + (float)exp(soc[i]/m*1000.0-20.0);
	float CEC = 1.0802*xx + 14.442 * 14.0 / 100000.0 * mmm[1];//meq/100 g soil	-> kg N/ha/layer		
		
	base_clay_N = 0.003 * CEC;//0.01
	max_clay_N = 10.0 * base_clay_N;

}

void class_model::ReDefineION()
{
	int i, k;

	float cc = mmm[1] * 0.000001; //mg N/kg soil -> kg N/ha/layer

	float IniNO3=0.0, IniNH4=0.0, EndNO3=0.0, EndNH4=0.0;

	for(i=1; i<=q; i++)
	{
		IniNO3 += no3[i];
		IniNH4 += nh4[i];
	}

	for(i=1; i<=IONdata; i++)
	{
		if(i==1) 
		{
			for(k=1; k<=IONlayer[i]; k++) 
			{
				no3[k]=NO3[i]*cc;
				nh4[k]=NH4[i]*cc;
			}
		}
		else if(i==IONdata) 
		{
			for(k=IONlayer[i]; k<=q; k++) 
			{
				no3[k]=NO3[i]*cc;
				nh4[k]=NH4[i]*cc;
			}
		}
		else 
		{
			for(k=IONlayer[i-1]+1; k<=IONlayer[i]; k++) 
			{
				no3[k]=NO3[i]*cc;
				nh4[k]=NH4[i]*cc;
			}
		}
	}

	for(i=1; i<=q; i++)
	{
		EndNO3 += no3[i];
		EndNH4 += nh4[i];
	}

	AdjustNO3 = EndNO3 - IniNO3;
	AdjustNH4 = EndNH4 - IniNH4;
}


void class_model::decomp_microbes(int l, float RFM, float DDRF, float till_fact,float* CRB1, float* CRB2, 
		float* fb_nh4, float* fb_co2, float* dcbavai,int tilq,
		int *st, float *nh4, float rcnb, float *crhr, float *crhl, float *co2, float *day_C_mine, float *day_N_mine, 
		float *day_N_assim, float DChumads, float EFFAC, float rcnh, float *doc, int q, float *day_O2, float rcnh2)
{
    float dlhc, DCRB1, DCRB2, DCRB, FBBC, FBHC, f_till_fact, Fo;
	float TN1, TN2, dTN;

	//if(st[l]>=12) Fo=0.1;
	//else Fo=0.7;
	Fo=0.7;
	
    //if ( l > (tilq + 3) ) f_till_fact = 1.0;
	if (l>=tilq) f_till_fact = 1.0;// + 3
    else  f_till_fact = till_fact;

	TN1 = (CRB1[l]+CRB2[l])/rcnb + nh4[l] + crhr[l]/rcnh2;

    /* microbe decomposition */
    DCRB1 = RFM * KRB * DDRF * f_till_fact * CRB1[l] * Fo * DChumads;
    DCRB2 = RFM * hrb * DDRF * f_till_fact * CRB2[l] * Fo * DChumads;
    DCRB = DCRB1 + DCRB2;

	CRB1[l] -= (DCRB1);
    CRB2[l] -= (DCRB2);

	nh4[l] += (DCRB / rcnb);	

    /* efficiency */
    FBBC = DCRB * EFFAC;
	/* new microbe */
    CRB1[l] += (FBBC * SRB);
    CRB2[l] += (FBBC * (1.0 - SRB));

	nh4[l] -= (FBBC / rcnb);
	*day_N_assim += (FBBC / rcnb);

    *dcbavai = FBBC;

    /* new resistant humads from microbe decomposition */
    FBHC = DCRB * EFFNO;
    crhr[l] += FBHC;

	nh4[l] -= (FBHC / rcnh2);
	//day_N_assim += (FBHC / rcnh);
	

    /* CO2 from net microbe decomposition */
    *fb_co2 = DCRB - FBBC - FBHC;
	//co2[l] += *fb_co2;
	//day_O2[l] += *fb_co2;
	//if(day_O2[l]<0.0) day_O2[l]=0.0;
	doc[l] += *fb_co2;
	if(l<=q) day_DOC_prod += *fb_co2;

	*day_C_mine += *fb_co2;
	*day_N_mine += *fb_co2/ rcnb;//4

    /* NH4 from net microbe decomposition */
    *fb_nh4 = (*fb_co2) / rcnb;
     

	TN2 = (CRB1[l]+CRB2[l])/rcnb + nh4[l] + crhr[l]/rcnh2;
	dTN = TN2 - TN1;

	nh4[l] -= dTN;

	nh4[l] = max(0.0, nh4[l]);
}

void class_model::NO3_leaching()//N_leaching
{
	int i;
	float LeachN_f, layer_leachN, layer_leachP, layer_leachDOC, fCLAY; //total_N=0.0, F_NO3, leached_NO3;

	fCLAY= 0.1 * (float)pow(clay[1],-2.5);//-0.4907);//1.0, 0.8, 0.5

	float LeachFlow = day_leach_water;//+IrriWater;

    if(LeachFlow>0.00001)
	{
		//NO3- leaching 
		
		if(st[1]>=12||by_passf>0.0) LeachN_f = 0.8;
		else LeachN_f = fCLAY * LeachFlow;//5.0; 0.05, 25.0

		if(LandUse==6) LeachN_f *= 0.01;
				
		LeachN_f = min(1.0, max(0.0, LeachN_f));
	
		for(i=1; i<=q; i++) 
		{
			layer_leachN = no3[i] * LeachN_f;
			no3[i] -= layer_leachN;

			layer_leachP = LabP[i] * LeachN_f;
			LabP[i] -= layer_leachP;

			layer_leachDOC = doc[i] * LeachN_f;
			doc[i] -= layer_leachDOC;
			day_DOC_leac += layer_leachDOC;
			
			if(i<q)
			{
				float zz = 0.8;
				no3[i+1] += (layer_leachN * zz);
				LabP[i+1] += (layer_leachP * zz);
				doc[i+1] += (layer_leachDOC * zz);
				day_leach_NO3 += (layer_leachN * (1.0-zz));
				day_leach_P += (layer_leachP * (1.0-zz));
				day_leach_DOC += (layer_leachDOC * (1.0-zz));
			}
			else
			{
				day_leach_NO3 += layer_leachN;
				day_leach_P += layer_leachP;
				day_leach_DOC += layer_leachDOC;
			}
			
			if(root_CO2[i]>0.0)
			{
				layer_leachDOC = root_CO2[i] * LeachN_f;
				root_CO2[i] -= layer_leachDOC;
				day_leach_DOC += layer_leachDOC;
			}
				
			if(flood_flag==1)//wetland leaching
			{
				if(i==q)
				{
					float leached_humus = dphum[q] * TTT * LeachFlow * 0.1;
					dphum[q] -= leached_humus;
					day_leachhu += leached_humus;
				}
			
				float leach_urea = 3.0 * urea[i] * LeachFlow;
				urea[i] -= leach_urea;
				day_leach_urea += leach_urea;
			}
		}
	}
	else
	{
		day_leach_NO3 = 0.0;
		day_leachhu = 0.0;
		day_leach_DOC = 0.0;
	}

	if(irri_flag==1&&day_IrriWater>=0.12)//Modified on July 26, 2012
	{		
		if(day_leach_NO3>0.0&&sph[1]>5.0)
		{
			float Fd, Fs;
			if(st[1]>=12) 
			{
				Fs=0.6;
				Fd=0.75;			
			}
			else
			{
				Fs = 0.7;//clay[1];// * 0.5;//0.25;
				Fd = (1.0-clay[1]); //0.5;			
			}

			float LeachDenN = Fs * day_leach_NO3;// / (1.0 + day_leach_NO3) * ocini[q] / (50.0 + ocini[q]);
			day_leach_NO3 -= LeachDenN;

			float LeachN2 = LeachDenN * Fd;
			float LeachN2O = LeachDenN * (1.0 - Fd);

			n2[q] += LeachN2;
			n2o[q] += LeachN2O;
			
			day_soil_denitrification += LeachDenN;
		}
	}
}


void class_model::Chemodenitrification(int l, float *temp)
{
	
	///////////////////////////////////////////////////////
	float f1, f2, f3, soil_T, fa=0.0001, chem_no, Tph;

	if (temp[l] < 0.0) soil_T = 0.0;
    else soil_T = temp[l];

    f1 = (-.0014 * soil_T * soil_T) + 0.0992 * soil_T + 0.0198;
	f2 = 90706.0 * (float)exp(-4.2532 * sph[l]);
    f3 = TTT * f1 * f2;
	f3 = max(0.0, min(1.0,f3));

	chem_no = f3 * (float)pow(day_soil_an_no, 1.5);
	if(chem_no>day_soil_an_no) chem_no = day_soil_an_no;
	day_soil_an_no -= chem_no;
	day_soil_an_n2o += chem_no;

	/////////////////////////////
/*	f1 = 0.03 * (float)pow(soil_T, 2.0) + 0.2;//  (based on Yamulki et al., 1997);
	f2 = 2236 * (float)exp(-2.5*sph[l]);//  (based on Blackmer and Cerrato, 1986);
	f3 = TTT * f1 * f2;
	f3 = max(0.0, min(1.0, f3));

	chem_no = fa * nh4[l] * f3;
	nh4[l] -= chem_no;
	day_soil_an_no += 0.9*chem_no;
	day_soil_an_n2o += 0.1*chem_no;
*/
}