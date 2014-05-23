#include "stdafx.h"
#include "Dndcgo.h"
#include "Source_main.h"
#include "Dndc_tool.h"

#ifdef MANURE 
extern class_manure MData;
extern class_model *DNDC;

static float DungCN[7]={0.0, 16.0, 16.0, 16.0, 18.5, 3.1, 7.9}; //dung C/N ratio for dairy cattle, beef, veal, pig, sheep, foultry
static float UrineCN[7]={0.0, 1.25, 1.25, 1.25, 4.24, 0.0, 0.0};//urine C/N ratio 
static float DungH2O[7]={0.0, 0.8, 0.8, 0.8, 0.82, 0.68, 0.8}; //water fraction of dung
static float UrineH2O[7]={0.0, 10.0, 10.0, 10.0, 3.3, 0.0, 0.0}; //kg water/head/day in urine
static float UrineNF[7]={0.0, 0.5, 0.5, 0.5, 0.786, 0.7, 0.7}; //urine fraction of total waste N
static float MilkNc[7]={0.0, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005};// N concentration in milk by weiht, kg N/kg milk
static float MeatNc[7]={0.0, 0.022, 0.022, 0.022, 0.022, 0.022, 0.024};// N concentration in meat by weight, kg N/kg meat

void class_manure::PenManureProduct(int FN)
{
	int i, FC=FN;
	float MilkN, MeatN, hd_ManureC, hd_ManureWater, ManureCN, ManureC;
	float hd_ManureN, hd_UrineN, hd_DungN, hd_DungC, hd_DungDM, hd_DungWater, hd_UrineC, hd_UrineWater;
	float day_meat, day_milk, ManureN, MilkP, MeatP, hd_UrineP, hd_DungP;	
	
	MData.mm_UrineN[FC] =0.0;//kg N/day
	MData.mm_UrineC[FC] =0.0;//kg C/day
	MData.mm_DungN[FC] =0.0;//kg N/day
	MData.mm_DungC[FC] =0.0;	//kg C/day
	MData.mm_DungWater[FC] =0.0;//kg water/day
	MData.mm_UrineWater[FC] =0.0;

	//Milk and meat production per animal per day
	//for(i=1;i<=MData.AnimalTypes[FC];i++)
	//{
		MData.mm_FeedC[FC] = MData.mm_FeedRate[FC] * 0.4; // kg dry matter/animal/day - kg C/animal/day
		MData.mm_FeedN[FC] = MData.mm_FeedProtein[FC] * 0.01 * MData.mm_FeedRate[FC] / 6.25; // kg N/animal/day
		MData.mm_FeedP[FC] = MData.mm_FeedTP[FC] * 0.01 * MData.mm_FeedRate[FC]; // kg P/animal/day
		if(MData.mm_AnimalType[FC]==1) //lactating cow
		{
			MilkN = MData.mm_FeedN[FC] * 0.25; // Powell et al, 2006
			MeatN = MData.mm_FeedN[FC] * 0.05;		
			MData.mm_MilkProduct[FC] = MilkN / 0.005; // milk production kg/animal/day
			MilkP = MData.mm_FeedP[FN] * 0.367;//milk P, kg P/cow/day
			MeatP = MData.mm_FeedP[FN] * 0.056;//meat P, kg P/cow/day
			hd_UrineP = MData.mm_FeedP[FC] * 0.011;//urine P, kg P/cow/day
			hd_DungP = MData.mm_FeedP[FC] * 0.566;//feces P, kg P/cow/day
		}
		else
		{
			MilkN = 0.0; // Powell et al, 2006
			MeatN = MData.mm_FeedN[FC] * 0.3;	
			MData.mm_MilkProduct[FC] = 0.0;
			MeatP = MData.mm_FeedP[FN] * 0.28;//meat P, kg P/cow/day
			hd_UrineP = MData.mm_FeedP[FC] * 0.02;//urine P, kg P/cow/day
			hd_DungP = MData.mm_FeedP[FC] * 0.7;//feces P, kg P/cow/day
		}
		MData.mm_MeatProduct[FC] = MeatN / 0.022; // meat production, kg/animal/day

		//enteric N2O£¬ 0.5g N2O-N/kg feed-N
		float head_enteric_n2o = 0.0005 * MData.mm_FeedN[FC];//0.9 * 0.001981 * MData.mm_FeedN[FN] / 0.225333;// * MData.mm_AnimalHeads[FN]; //kg N/day/head

		//manure production per animal per day
		hd_ManureN = MData.mm_FeedN[FN] - (MilkN + MeatN + head_enteric_n2o);//kg N/head/day
		//hd_ManureN = MData.mm_FeedN[FC] - (MilkN + MeatN);//kg N/head/day
		hd_UrineN = UrineNF[MData.mm_AnimalType[FC]] * hd_ManureN;//kg N/head/day, Broderick 2003; Wattiaux and Karg 2004
		hd_DungN = (1.0 - UrineNF[MData.mm_AnimalType[FC]]) * hd_ManureN;//kg N/head/day

		hd_DungC = hd_DungN * DungCN[MData.mm_AnimalType[FC]];//kg C/head/day
		hd_DungDM = hd_DungC / 0.58;// kg dry matter/head/day
		hd_DungWater = hd_DungDM / (1.0 - DungH2O[MData.mm_AnimalType[FC]]) * DungH2O[MData.mm_AnimalType[FC]];//kg water/head/day
		hd_UrineC = hd_UrineN * UrineCN[MData.mm_AnimalType[FC]];//kg C/head/day
		hd_UrineWater = UrineH2O[MData.mm_AnimalType[FC]];//kg water/head/day
		hd_ManureC = hd_DungC + hd_UrineC;//kg C/head/day
		ManureCN = hd_ManureC / hd_ManureN;//kg N/head/day
		hd_ManureWater = hd_DungWater + hd_UrineWater;//kg water/head/day

		MData.day_enteric_n2o[FN] = head_enteric_n2o * MData.mm_AnimalHeads[FN]; //kg N/day
		//manure production per farm per day
		ManureN = hd_ManureN * MData.mm_AnimalHeads[FC];
		ManureC = hd_ManureC * MData.mm_AnimalHeads[FC];
		MData.mm_DungDM = hd_DungDM * MData.mm_AnimalHeads[FC];
		MData.mm_ManureWater[FC] = hd_ManureWater * MData.mm_AnimalHeads[FC];
		
		MData.mm_UrineN[FC] += hd_UrineN * MData.mm_AnimalHeads[FC];//kg N/day
		MData.mm_UrineC[FC] += hd_UrineC * MData.mm_AnimalHeads[FC];//kg C/day
		MData.mm_DungN[FC] += hd_DungN * MData.mm_AnimalHeads[FC];//kg N/day
		MData.mm_DungC[FC] += hd_DungC * MData.mm_AnimalHeads[FC];	//kg C/day
		MData.mm_DungWater[FC] += hd_DungWater * MData.mm_AnimalHeads[FC];//kg water/day
		MData.mm_UrineWater[FC] += hd_UrineWater * MData.mm_AnimalHeads[FC];//kg water/day
		MData.ManureCapacity = DungH2O[MData.mm_AnimalType[FC]];
		MData.mm_UrineP[FN] = hd_UrineP * MData.mm_AnimalHeads[FN];//kg P/day
		MData.mm_DungP[FN] = hd_DungP * MData.mm_AnimalHeads[FN];//kg P/day

		MData.day_intake_C = MData.mm_FeedC[FC] * MData.mm_AnimalHeads[FC];
		MData.day_intake_N = MData.mm_FeedN[FC] * MData.mm_AnimalHeads[FC];
		MData.day_intake_P = MData.mm_FeedP[FC] * MData.mm_AnimalHeads[FC];

		MData.day_milk_N = MilkN * MData.mm_AnimalHeads[FC];
		MData.day_milk_C = MilkN * MData.mm_AnimalHeads[FC] / MilkNc[MData.mm_AnimalType[FC]] * 0.05 * 0.4;
		MData.day_meat_N = MeatN * MData.mm_AnimalHeads[FC];
		MData.day_meat_C = MeatN * MData.mm_AnimalHeads[FC] / MeatNc[MData.mm_AnimalType[FC]] * 0.2 * 0.4;
		MData.day_milk_P = MilkP * MData.mm_AnimalHeads[FN];
		MData.day_meat_P = MeatP * MData.mm_AnimalHeads[FN];
		MData.day_waste_Water = MData.mm_ManureWater[FC];
		MData.day_milk_product = MData.mm_MilkProduct[FC]*MData.mm_AnimalHeads[FC];//kg milk/MData.day
		MData.day_urine_water = MData.mm_UrineWater[FC];
		MData.day_dung_water = MData.mm_DungWater[FC];

		MData.yr_urine_water += MData.day_urine_water;
		MData.yr_dung_water += MData.day_dung_water;
		MData.yr_waste_water += MData.day_waste_Water;
		MData.yr_milk_product += MData.day_milk_product;

		float KKanimal_co2 = (MData.mm_FeedC[FC] * MData.mm_AnimalHeads[FC] - 
		MilkN * MData.mm_AnimalHeads[FC] / MilkNc[MData.mm_AnimalType[FC]] * 0.05 * 0.4 -
		MeatN * MData.mm_AnimalHeads[FC] / MeatNc[MData.mm_AnimalType[FC]] * 0.2 * 0.4 -
		ManureC);

		MData.animal_co2 = MData.mm_FeedC[FC] * 0.4805193 * MData.mm_AnimalHeads[FC];

		float ddCC = MData.animal_co2 - KKanimal_co2;
		MData.mm_DungC[FC] -= ddCC;

		MData.yr_enteric_co2 += MData.animal_co2;

		MData.yr_intake_C += MData.mm_FeedC[FC] * MData.mm_AnimalHeads[FC];
		MData.yr_intake_N += MData.mm_FeedN[FC] * MData.mm_AnimalHeads[FC];
		MData.yr_milk_N += MilkN * MData.mm_AnimalHeads[FC];
		MData.yr_milk_C += MilkN * MData.mm_AnimalHeads[FC] / MilkNc[MData.mm_AnimalType[FC]] * 0.05 * 0.4;
		MData.yr_meat_N += MeatN * MData.mm_AnimalHeads[FC];
		MData.yr_meat_C += MeatN * MData.mm_AnimalHeads[FC] / MeatNc[MData.mm_AnimalType[FC]] * 0.2 * 0.4;
		MData.yr_intake_P += MData.day_intake_P;
		MData.yr_milk_P += MData.day_milk_P;
		MData.yr_meat_P += MData.day_meat_P;
	//}
	MData.yr_urine_C += MData.mm_UrineC[FC];
	MData.yr_urine_N += MData.mm_UrineN[FC];
	MData.yr_dung_C += MData.mm_DungC[FC];
	MData.yr_dung_N += MData.mm_DungN[FC];
	MData.yr_urine_P += MData.mm_UrineP[FC];
	MData.yr_dung_P += MData.mm_DungP[FC];
}


void class_manure::PenManure_BGC(int FN, int GN, 
		int q, int jday, float rcnrvl, float rcnrl, float rcnrr, float rcnh, float rcnb, float *urea, float *doc,
		float *rcvl, float *rcl, float *rcr, float *CRB1, float *CRB2, float *dphum, float *temp, float air_temp, 
		float DayPET, float wind_speed, float *day_wfps, float *soc, float *sph, int *flood_flag, int WTL, int cwc, float day_WT, 
		float *soil_ph, float *nh4, float *ch4_prod, float *ch4_day_oxid, float *plant_day_ch4, float rcnm, float *son, float *SIN, float *no3, float *no2, 
		float *no, float *n2o, float *n2, float *nh3, float *ch4,float rhrl, float TTT, float *newh, float *w_no2, float *w_nh3, 
		float *w_no3, float *w_nh4, float *wurea, float day_clayn, float *wpool_no,
		float *DayFlux, int tilq, 
		FILE *fCB, FILE *fNB, float *w_n2o, float *w_n2, int *seed, float *day_ch4, float *anvf, float WaterLeakRate,
		float *co2, float *clay, int LandUse, int FloDays, float *crhl, float *crhr, float *mmm, float *ocini,
		float *h, float *day_C_mine, float *day_N_mine, float *day_N_assim, float DClitter,
		float *litco22, float *litco23, int *st, float DChumads, float EFFAC, float DChumus,
		int wetland_flag, float *ice, float *day_soil_nh3, float *lai, float *PlantTakeN, float *maxmn,
		float leaf_water, float cropht, float base_nh3, float *day_N_demand, float *day_nup, float *day_fixN, 
		float *day_DWater,
		float *day_plant_nh3, float *yr_NH3_deposit, float *plantn, float *clay_nh4, float base_clay_N, float max_clay_N,
		float *wrcvl, float *wrcl, float *wrcr, float *wcrb, float *wcrhl, float *wcrhr, float *whumus, float *wtcavai,
		float *day_ice_N, float *day_ice_doc, float *day_ice_n2o, float *day_ice_n2, float *IniSOC, float *IniSON, 
		float *End_SON, 
		float day_runoff, float *day_runoff_N, float *day_clay_N, 
		float *dsno3, float *dsnh4, float *dsnh3, float *day_dsurea, float *ds_doc, int *crop, float *yr_fixn,
		float till_fact, float *sts, float rainfall, float *O2_old, float *O2, float *vvv, float *ave_anvf,
		int year, double water[25][180], float total_ice, float *day_rco2, int flo_num, float *Eh,
		float *doc_w, float *nh4_w, float *no3_w, float *no2_w, float *no_w, float *n2o_w, float *n2_w,
		float *doc_d, float *nh4_d, float *no3_d, float *no2_d, float *no_d, float *n2o_d, float *ttoo, 
		float ManureAddCN, int ManureDays, float MicrobioIndex, float mRBO, float mFD, int WaterControl, 
		float *assim2, float *fldcap, float by_passf, int NIDay1, int NIDay2,
		float *day_soil_nitrify, float *day_nitrify_NO, float *day_nitrify_N2O, float *day_soil_an_no,
		float *day_soil_an_n2o, float *day_soil_n2, float *Root_Wt, float *day_soil_denitrification,
		float *R_Wt, float *vascularity, float *wiltpt, float *Odoc, float *day_frostDOC, float *yr_frostDOC,
		float *winertC, float *inert_C, int fert_flag, float *root_doc,  
		float precipitation, float manuFlag, int irri_flag, float *FreezeDOC, float *day_O2, float RFM, 
		float *SoilDepth, float rcnh2, float *Eh1, float *sop, float *OrgP, float *LabP, float *ComP, float *AdsP)
{
	int FC=FN, i;
	float mm_DayPET;

	h[GN] = h[1];
			
	//Pen manure temperature
	temp[GN] = air_temp;

	//Pen manure moisture
	if(rainfall>0.0)
	{
		water[1][GN] += rainfall * MData.mm_GroundArea[FC] * 1000.0; //rain water m -> kg water/day/pen
	}	

	//evaporation
	mm_DayPET = DayPET * (1.0 + wind_speed*0.01);//m/day	
	MData.day_PenEvapWater = mm_DayPET * MData.mm_GroundArea[FC] * 1000.0;//m/day -> kg water/pen/day

	if(water[1][GN]*0.9 > MData.day_PenEvapWater)
		water[1][GN] -= MData.day_PenEvapWater;
	else
	{
		MData.day_PenEvapWater = water[1][GN]*0.9;
		water[1][GN] *= 0.1;
	}

	//Leaching
	if(MData.mm_GroundSurface[FC]==0)//bare soil
	{
		if(water[1][GN]>MData.ManureCapacity*soc[GN])
		{		
			MData.day_PenLeachWater = (water[1][GN] - (soc[GN] *  MData.ManureCapacity));// / (float)st;
			water[1][GN] -= MData.day_PenLeachWater;
		}
	}

	if(water[1][GN]<0.0) water[1][GN]=0.0;

	///////////////////////////////////////////////
	if(rainfall>0.0)
	{
		float RFL[24] = {0.0,0.0};
		float RunoffIndex = 10.0 * rainfall; 
		RunoffIndex = min(1.0, max(0.0, RunoffIndex));

		RFL[1] = rcvl[GN] * RunoffIndex;
		RFL[2] = rcl[GN] * RunoffIndex; 
		RFL[3] =  rcr[GN] * RunoffIndex; 
		RFL[4] =   CRB1[GN] * RunoffIndex; 
		RFL[5] = CRB2[GN] * RunoffIndex; 
		RFL[6] = crhl[GN] * RunoffIndex;
		RFL[7] = crhr[GN] * RunoffIndex; 
		RFL[8] = dphum[GN] * RunoffIndex; 
		
		RFL[9] = nh4[GN] * RunoffIndex;
		RFL[10] = no3[GN] * RunoffIndex;
		RFL[11] = no2[GN] * RunoffIndex;
		RFL[12] = no[GN] * RunoffIndex; 
		RFL[13] = n2o[GN] * RunoffIndex;
		RFL[14] =  n2[GN] * RunoffIndex; 
		RFL[15] =  nh3[GN] * RunoffIndex; 	
		RFL[16] =  urea[GN] * RunoffIndex; 
		RFL[17] = co2[GN] * RunoffIndex;	
		RFL[18] =  doc[GN] * RunoffIndex; 
		RFL[19] = water[1][GN] * RunoffIndex; 
		RFL[20] = OrgP[GN] * RunoffIndex;
		RFL[21] = LabP[GN] * RunoffIndex;
		RFL[22] = ComP[GN] * RunoffIndex;

		ManureRunoff(FN, GN, RFL, rcnrvl, rcnrl,rcnrr,rcnb,rcnh,rcnm,
			rcvl,rcl,rcr,CRB1,CRB2,crhl,crhr,dphum,nh4,no3,no2,
			no,n2o,n2,nh3,urea,co2,doc,water, rcnh2,OrgP,LabP,ComP);

	}


	///////////////////////////////////////////////

	MData.Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm,
								 soc, son, SIN, rcvl, rcl, rcr, 
								 CRB1, CRB2, crhl, crhr, dphum, doc, 
								 nh4, no3, no2, no, n2o, n2, nh3, 
								 urea, rcnh2, co2, sop,OrgP, LabP, ComP);

	day_wfps[GN] = water[1][GN] / soc[GN];//relative manure moisture
	day_wfps[GN] = max(0.1, min(1.0, day_wfps[GN]));

	//pH
	sph[GN] = 8.0;//8.0 for dairy, beef; 8.1 for veal

	//convert_lot_to_ha(FC, mm_GroundArea[FC]);

	DNDC->dndc_decomposition(rhrl, CRB1, CRB2, newh, w_no2, 
		w_nh3, w_no3, w_nh4, wurea, wpool_no, 
		1.0, DayFlux, tilq, 
		fCB, fNB, w_n2o, w_n2,
		GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, 
		SIN, rcvl, rcl, rcr, dphum, doc, nh4, no3, no2, 
		no, n2o, n2, nh3, urea, jday, co2, clay, TTT, *flood_flag,
		day_wfps, WTL, temp, LandUse, FloDays, crhl, crhr, mmm, ocini,
		h, day_C_mine, day_N_mine, day_N_assim, DClitter, 
		litco22, litco23, st, DChumads, EFFAC, DChumus, sph,
		wetland_flag, wind_speed, ice, day_soil_nh3,
		lai, PlantTakeN, maxmn, leaf_water, cropht, base_nh3,
		day_N_demand, day_nup, day_fixN, day_DWater, day_plant_nh3, yr_NH3_deposit, 
		plantn, cwc, clay_nh4, base_clay_N, max_clay_N,
		wrcvl, wrcl, wrcr, wcrb, wcrhl, wcrhr, whumus, wtcavai,
		day_ice_N, day_ice_doc, day_ice_n2o, day_ice_n2, IniSOC, IniSON, End_SON,
		day_runoff, day_runoff_N, day_clay_N, 
		dsno3, dsnh4, dsnh3, day_dsurea, ds_doc, crop, yr_fixn, rainfall, soil_ph, 
		winertC, inert_C, fert_flag, root_doc, day_O2, RFM, SoilDepth, rcnh2, ttoo, 
		sop, OrgP, LabP, AdsP, ComP, sts, day_leach_water);

	DNDC->N2O_balloon(GN, till_fact, sts, h, jday, *flood_flag, FloDays, rainfall,
		O2_old, O2, TTT, vvv, ave_anvf, q, st, temp, year, water, day_wfps, day_WT, 
		WTL, total_ice, day_rco2, co2, flo_num, ice, sph, Eh, anvf,
		nh4, no3, no2, no, n2, doc, n2o, doc_w, nh4_w, no3_w, no2_w, no_w, n2o_w, n2_w,
		doc_d, nh4_d, no3_d, no2_d, no_d, n2o_d, LandUse, ttoo, 
		ManureAddCN, ManureDays, soc, MicrobioIndex, mRBO, mFD, WaterControl, 
		crop, assim2, clay, fldcap, by_passf, NIDay1, NIDay2,
		day_soil_nitrify, day_nitrify_NO, day_nitrify_N2O, day_soil_an_no,
		day_soil_an_n2o, day_soil_n2, Root_Wt, cwc, WaterLeakRate,
		rcvl, rcl, rcr, day_soil_denitrification,
		wiltpt, CRB1, CRB2, rcnb, day_frostDOC, yr_frostDOC, precipitation, 
		manuFlag, irri_flag, FreezeDOC, nh3, Eh1, urea, 0, HSM);

	//convert_ha_to_lot(FC, mm_GroundArea[FC]);

	//DNDC->nh43_equil(GN, temp, sph, nh4, nh3, q, h, day_wfps);

	//DNDC->nh3_volatilization(GN, temp, *flood_flag, wetland_flag, nh3, q, wind_speed, day_wfps, ice, day_soil_nh3, nh4);

	//NO3 and DOC leaching
	float Lindex = MData.day_PenLeachWater / (water[1][GN] + MData.day_PenLeachWater);
	Lindex=min(1.0, max(0.0, Lindex));
	
	MData.day_pen_no3Leach = Lindex * no3[GN];
	MData.day_pen_docLeach = Lindex * doc[GN];
	
	no3[GN] -= MData.day_pen_no3Leach;
	doc[GN] -= MData.day_pen_docLeach;
	
}


void class_manure::PenManureOccumulation(int FN, int GN, int q, int jday, float rcnrvl, float rcnrl, float rcnrr, 
		   float rcnh, float rcnb, float *urea, float *doc, float *rcvl, 
		   float *rcl, float *rcr, float *crhl, float *crhr, float *CRB1, float *CRB2, float *dphum, double water[25][180],
		   float rcnm, float *soc, float *son, float *SIN, float *nh4, float *no3, float *no2, float *no, float *n2o,
		   float *n2, float *nh3, float rcnh2, float *co2, float *sop, float *OrgP, float *LabP, float *ComP)
{
	float AddC, AddN, ManureAddCN, Free_N, solid_C, solid_CN, solid_MN;
	float AddC1, AddC2, AddC3, AddHum, dInertC;
	int i, j, FC=FN;

	AddC = 0.0;
	AddN = 0.0;

	urea[GN] += MData.mm_UrineN[FC];
	doc[GN] += MData.mm_UrineC[FC];
	water[1][GN] += MData.mm_UrineWater[FC] + MData.mm_DungWater[FC];
	
	AddC += MData.mm_DungC[FC];
	AddN += MData.mm_DungN[FC];
	
	ManureAddCN = AddC / AddN;

	if(ManureAddCN<rcnrvl)
	{
		float accessN = AddN - AddC/rcnrvl;
		Free_N = accessN;
		solid_CN = rcnrvl;
	}
	else if(ManureAddCN>=rcnrvl&&ManureAddCN<rcnh)
	{
		Free_N = 0.3 * AddN;
		float manureN = 0.7 * AddN;
		solid_CN = AddC / manureN;
	}
	else
	{
		Free_N = 0.1 * AddN;
		float manureN = 0.9 * AddN;
		solid_CN = AddC / manureN;
	}

	nh4[GN] += Free_N;
				 
	solid_MN = AddN - Free_N;
				  
	DNDC->partition_vll_humad(solid_CN, AddC, solid_MN, &AddC1, &AddC2, &AddC3, &AddHum, rcnrvl, rcnrl, rcnrr, rcnh);

	rcvl[GN] += AddC1;
	rcl[GN] += AddC2;
	rcr[GN] += AddC3;
	crhl[GN] += 0.25*AddHum; 
	crhr[GN] += 0.25*AddHum;
	CRB1[GN] += 0.25*AddHum;
	CRB2[GN] += 0.25*AddHum;

	//eckHMN = AddC1 / rcnrvl + AddC2 / rcnrl + AddC3 / rcnrr + AddHum / rcnh; 
			
	AddC1 = 0.0;
	AddC2 = 0.0;
	AddC3 = 0.0;
	Free_N = 0.0;	
		
	//bedding material partitioning
	if(MData.mm_PenBedFrequency>0.0)
	{
		int BF = jday%(int)MData.mm_PenBedFrequency;
		if(BF==0&&(MData.mm_PenBedType==1||MData.mm_PenBedType==2||MData.mm_PenBedType==3))
		{		
			AddC = MData.mm_PenBedAmount*0.4;
			float AddCN = MData.mm_PenBedCN;

			DNDC->alloc_litter_into_pools(&AddCN, &AddC, 2, &AddC1, &AddC2, &AddC3, &dInertC, 1, rcnrvl, rcnrl, rcnrr);

			rcvl[GN] += AddC1;
			rcl[GN] += AddC2;
			rcr[GN] += AddC3;
		}
	}
		
	LabP[GN] += MData.mm_UrineP[FN];
	OrgP[GN] += MData.mm_DungP[FN];

	Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
}

void class_manure::PenManureRemoval(int FN, int GN, int q, float rcnrvl, float rcnrl, float rcnrr, float rcnb, float rcnh, float rcnm,
								 float *soc, float *son, float *SIN, float *rcvl, float *rcl, float *rcr, 
								 float *CRB1, float *CRB2, float *crhl, float *crhr, float *dphum, float *doc, 
								 float *nh4, float *no3, float *no2, float *no, float *n2o, float *n2, float *nh3, 
								 float *urea, int jday, float *co2, float *sph, double water[25][180],
								 float *day_wfps, float *temp, float air_temp, float rcnh2, float *sop, 
								 float *OrgP, float *LabP, float *ComP)
{
	float old_HouseC, old_HouseN, new_HouseC, new_HouseN;
	float ToCompost[24], ToLagoon[24], ToDigester[24];
	int CLD, fcc = FN, FC=GN, gcc=q+8;
	if(MData.m_ManureRemoveFrequency[fcc]==0.0) CLD=1;
	else CLD = jday%(int)MData.m_ManureRemoveFrequency[fcc];

	if(CLD==0)
	{
		/*ToCompost[1] = MData.m_FC[fcc] * rcvl[FC];
		ToCompost[2] = MData.m_FC[fcc] * rcl[FC];
		ToCompost[3] = MData.m_FC[fcc] * rcr[FC];
		ToCompost[4] = MData.m_FC[fcc] * CRB1[FC];
		ToCompost[5] = MData.m_FC[fcc] * CRB2[FC];
		ToCompost[6] = MData.m_FC[fcc] * (crhl[FC]+crhr[FC]);
		ToCompost[7] = MData.m_FC[fcc] * dphum[FC];
		ToCompost[8] = MData.m_FC[fcc] * nh4[FC];
		ToCompost[9] = MData.m_FC[fcc] * no3[FC];
		ToCompost[10] = MData.m_FC[fcc] * no2[FC];
		ToCompost[11] = MData.m_FC[fcc] * no[FC];
		ToCompost[12] = MData.m_FC[fcc] * n2o[FC];
		ToCompost[13] = MData.m_FC[fcc] * n2[FC];
		ToCompost[14] = MData.m_FC[fcc] * nh3[FC];	
		ToCompost[15] = MData.m_FC[fcc] * urea[FC];
		ToCompost[16] = MData.m_FC[fcc] * co2[FC];	
		ToCompost[17] = MData.m_FC[fcc] * doc[FC];
		ToCompost[18] = sph[FC];
		ToCompost[19] = MData.m_FC[fcc] * water[1][FC];
		ToCompost[20] = MData.m_FC[fcc] * OrgP[FC];
		ToCompost[21] = MData.m_FC[fcc] * LabP[FC];
		ToCompost[22] = MData.m_FC[fcc] * ComP[FC];*/

		ToCompost[1] = MData.m_FC[fcc] * rcvl[FC] + MData.m_UC[fcc] * rcvl[gcc];
		ToCompost[2] = MData.m_FC[fcc] * rcl[FC] + MData.m_UC[fcc] * rcl[gcc];
		ToCompost[3] = MData.m_FC[fcc] * rcr[FC] + MData.m_UC[fcc] * rcr[gcc];
		ToCompost[4] = MData.m_FC[fcc] * CRB1[FC] + MData.m_UC[fcc] * CRB1[gcc];
		ToCompost[5] = MData.m_FC[fcc] * CRB2[FC] + MData.m_UC[fcc] * CRB2[gcc];
		ToCompost[6] = MData.m_FC[fcc] * (crhl[FC]+crhr[FC]) + MData.m_UC[fcc] * (crhl[gcc]+crhr[gcc]);
		ToCompost[7] = MData.m_FC[fcc] * dphum[FC] + MData.m_UC[fcc] * dphum[gcc];
		ToCompost[8] = MData.m_FC[fcc] * nh4[FC] + MData.m_UC[fcc] * nh4[gcc];
		ToCompost[9] = MData.m_FC[fcc] * no3[FC] + MData.m_UC[fcc] * no3[gcc];
		ToCompost[10] = MData.m_FC[fcc] * no2[FC] + MData.m_UC[fcc] * no2[gcc];
		ToCompost[11] = MData.m_FC[fcc] * no[FC] + MData.m_UC[fcc] * no[gcc];
		ToCompost[12] = MData.m_FC[fcc] * n2o[FC] + MData.m_UC[fcc] * n2o[gcc];
		ToCompost[13] = MData.m_FC[fcc] * n2[FC] + MData.m_UC[fcc] * n2[gcc];
		ToCompost[14] = MData.m_FC[fcc] * nh3[FC] + MData.m_UC[fcc] * nh3[gcc];	
		ToCompost[15] = MData.m_FC[fcc] * urea[FC] + MData.m_UC[fcc] * urea[gcc];
		ToCompost[16] = MData.m_FC[fcc] * co2[FC] + MData.m_UC[fcc] * co2[gcc];	
		ToCompost[17] = MData.m_FC[fcc] * doc[FC] + MData.m_UC[fcc] * doc[gcc];
		ToCompost[18] = sph[FC];
		ToCompost[19] = MData.m_FC[fcc] * water[1][FC] + MData.m_UC[fcc] * water[1][gcc];
		ToCompost[20] = MData.m_FC[fcc] * OrgP[FC];
		ToCompost[21] = MData.m_FC[fcc] * LabP[FC];
		ToCompost[22] = MData.m_FC[fcc] * ComP[FC];
	
		/*ToLagoon[1] = MData.m_FL[fcc] * rcvl[FC];
		ToLagoon[2] = MData.m_FL[fcc] * rcl[FC];
		ToLagoon[3] = MData.m_FL[fcc] * rcr[FC];
		ToLagoon[4] = MData.m_FL[fcc] * CRB1[FC];
		ToLagoon[5] = MData.m_FL[fcc] * CRB2[FC];
		ToLagoon[6] = MData.m_FL[fcc] * (crhl[FC]+crhr[FC]);
		ToLagoon[7] = MData.m_FL[fcc] * dphum[FC];
		ToLagoon[8] = MData.m_FL[fcc] * nh4[FC];
		ToLagoon[9] = MData.m_FL[fcc] * no3[FC];
		ToLagoon[10] = MData.m_FL[fcc] * no2[FC];
		ToLagoon[11] = MData.m_FL[fcc] * no[FC];
		ToLagoon[12] = MData.m_FL[fcc] * n2o[FC];
		ToLagoon[13] = MData.m_FL[fcc] * n2[FC];
		ToLagoon[14] = MData.m_FL[fcc] * nh3[FC];	
		ToLagoon[15] = MData.m_FL[fcc] * urea[FC];
		ToLagoon[16] = MData.m_FL[fcc] * co2[FC];	
		ToLagoon[17] = MData.m_FL[fcc] * doc[FC];
		ToLagoon[18] = sph[FC];
		ToLagoon[19] = MData.m_FL[fcc] * water[1][FC];
		ToLagoon[20] = MData.m_FL[fcc] * OrgP[FC];
		ToLagoon[21] = MData.m_FL[fcc] * LabP[FC];
		ToLagoon[22] = MData.m_FL[fcc] * ComP[FC];*/

		ToLagoon[1] = MData.m_FL[fcc] * rcvl[FC] + MData.m_UL[fcc] * rcvl[gcc];
		ToLagoon[2] = MData.m_FL[fcc] * rcl[FC] + MData.m_UL[fcc] * rcl[gcc];
		ToLagoon[3] = MData.m_FL[fcc] * rcr[FC] + MData.m_UL[fcc] * rcr[gcc];
		ToLagoon[4] = MData.m_FL[fcc] * CRB1[FC] + MData.m_UL[fcc] * CRB1[gcc];
		ToLagoon[5] = MData.m_FL[fcc] * CRB2[FC] + MData.m_UL[fcc] * CRB2[gcc];
		ToLagoon[6] = MData.m_FL[fcc] * (crhl[FC]+crhr[FC]) + MData.m_UL[fcc] * (crhl[gcc]+crhr[gcc]);
		ToLagoon[7] = MData.m_FL[fcc] * dphum[FC] + MData.m_UL[fcc] * dphum[gcc];
		ToLagoon[8] = MData.m_FL[fcc] * nh4[FC] + MData.m_UL[fcc] * nh4[gcc];
		ToLagoon[9] = MData.m_FL[fcc] * no3[FC] + MData.m_UL[fcc] * no3[gcc];
		ToLagoon[10] = MData.m_FL[fcc] * no2[FC] + MData.m_UL[fcc] * no2[gcc];
		ToLagoon[11] = MData.m_FL[fcc] * no[FC] + MData.m_UL[fcc] * no[gcc];
		ToLagoon[12] = MData.m_FL[fcc] * n2o[FC] + MData.m_UL[fcc] * n2o[gcc];
		ToLagoon[13] = MData.m_FL[fcc] * n2[FC] + MData.m_UL[fcc] * n2[gcc];
		ToLagoon[14] = MData.m_FL[fcc] * nh3[FC] + MData.m_UL[fcc] * nh3[gcc];	
		ToLagoon[15] = MData.m_FL[fcc] * urea[FC] + MData.m_UL[fcc] * urea[gcc];
		ToLagoon[16] = MData.m_FL[fcc] * co2[FC] + MData.m_UL[fcc] * co2[gcc];	
		ToLagoon[17] = MData.m_FL[fcc] * doc[FC] + MData.m_UL[fcc] * doc[gcc];
		ToLagoon[18] = sph[FC];
		ToLagoon[19] = MData.m_FL[fcc] * water[1][FC] + MData.m_UL[fcc] * water[1][gcc];
		ToLagoon[20] = MData.m_FL[fcc] * OrgP[FC];
		ToLagoon[21] = MData.m_FL[fcc] * LabP[FC];
		ToLagoon[22] = MData.m_FL[fcc] * ComP[FC];


		/*ToDigester[1] = MData.m_FD[fcc] * rcvl[FC];
		ToDigester[2] = MData.m_FD[fcc] * rcl[FC];
		ToDigester[3] = MData.m_FD[fcc] * rcr[FC];
		ToDigester[4] = MData.m_FD[fcc] * CRB1[FC];
		ToDigester[5] = MData.m_FD[fcc] * CRB2[FC];
		ToDigester[6] = MData.m_FD[fcc] * (crhl[FC]+crhr[FC]);
		ToDigester[7] = MData.m_FD[fcc] * dphum[FC];
		ToDigester[8] = MData.m_FD[fcc] * nh4[FC];
		ToDigester[9] = MData.m_FD[fcc] * no3[FC];
		ToDigester[10] = MData.m_FD[fcc] * no2[FC];
		ToDigester[11] = MData.m_FD[fcc] * no[FC];
		ToDigester[12] = MData.m_FD[fcc] * n2o[FC];
		ToDigester[13] = MData.m_FD[fcc] * n2[FC];
		ToDigester[14] = MData.m_FD[fcc] * nh3[FC];	
		ToDigester[15] = MData.m_FD[fcc] * urea[FC];
		ToDigester[16] = MData.m_FD[fcc] * co2[FC];	
		ToDigester[17] = MData.m_FD[fcc] * doc[FC];
		ToDigester[18] = sph[FC];
		ToDigester[19] = MData.m_FD[fcc] * water[1][FC];
		ToDigester[20] = MData.m_FD[fcc] * OrgP[FC];
		ToDigester[21] = MData.m_FD[fcc] * LabP[FC];
		ToDigester[22] = MData.m_FD[fcc] * ComP[FC];*/

		ToDigester[1] = MData.m_FD[fcc] * rcvl[FC] + MData.m_UD[fcc] * rcvl[gcc];
		ToDigester[2] = MData.m_FD[fcc] * rcl[FC] + MData.m_UD[fcc] * rcl[gcc];
		ToDigester[3] = MData.m_FD[fcc] * rcr[FC] + MData.m_UD[fcc] * rcr[gcc];
		ToDigester[4] = MData.m_FD[fcc] * CRB1[FC] + MData.m_UD[fcc] * CRB1[gcc];
		ToDigester[5] = MData.m_FD[fcc] * CRB2[FC] + MData.m_UD[fcc] * CRB2[gcc];
		ToDigester[6] = MData.m_FD[fcc] * (crhl[FC]+crhr[FC]) + MData.m_UD[fcc] * (crhl[gcc]+crhr[gcc]);
		ToDigester[7] = MData.m_FD[fcc] * dphum[FC] + MData.m_UD[fcc] * dphum[gcc];
		ToDigester[8] = MData.m_FD[fcc] * nh4[FC] + MData.m_UD[fcc] * nh4[gcc];
		ToDigester[9] = MData.m_FD[fcc] * no3[FC] + MData.m_UD[fcc] * no3[gcc];
		ToDigester[10] = MData.m_FD[fcc] * no2[FC] + MData.m_UD[fcc] * no2[gcc];
		ToDigester[11] = MData.m_FD[fcc] * no[FC] + MData.m_UD[fcc] * no[gcc];
		ToDigester[12] = MData.m_FD[fcc] * n2o[FC] + MData.m_UD[fcc] * n2o[gcc];
		ToDigester[13] = MData.m_FD[fcc] * n2[FC] + MData.m_UD[fcc] * n2[gcc];
		ToDigester[14] = MData.m_FD[fcc] * nh3[FC] + MData.m_UD[fcc] * nh3[gcc];	
		ToDigester[15] = MData.m_FD[fcc] * urea[FC] + MData.m_UD[fcc] * urea[gcc];
		ToDigester[16] = MData.m_FD[fcc] * co2[FC] + MData.m_UD[fcc] * co2[gcc];	
		ToDigester[17] = MData.m_FD[fcc] * doc[FC] + MData.m_UD[fcc] * doc[gcc];
		ToDigester[18] = sph[FC];
		ToDigester[19] = MData.m_FD[fcc] * water[1][FC] + MData.m_UD[fcc] * water[1][gcc];
		ToDigester[20] = MData.m_FD[fcc] * OrgP[FC];
		ToDigester[21] = MData.m_FD[fcc] * LabP[FC];
		ToDigester[22] = MData.m_FD[fcc] * ComP[FC];
		
		/////////////////////////////////////////// remain in pen 
		rcvl[FC] *= MData.m_FR[fcc];
		rcl[FC] *= MData.m_FR[fcc];
		rcr[FC] *= MData.m_FR[fcc];
		CRB1[FC] *= MData.m_FR[fcc];
		CRB2[FC] *= MData.m_FR[fcc];
		crhl[FC] *= MData.m_FR[fcc]; 
		crhr[FC] *= MData.m_FR[fcc];
		dphum[FC] *= MData.m_FR[fcc];
		nh4[FC] *= MData.m_FR[fcc];
		no3[FC] *= MData.m_FR[fcc];
		no2[FC] *= MData.m_FR[fcc];
		no[FC] *= MData.m_FR[fcc];
		n2o[FC] *= MData.m_FR[fcc];
		n2[FC] *= MData.m_FR[fcc];
		nh3[FC] *= MData.m_FR[fcc];
		urea[FC] *= MData.m_FR[fcc];
		co2[FC] *= MData.m_FR[fcc];
		doc[FC] *= MData.m_FR[fcc];
		sph[FC] = sph[FC];
		water[1][FC] *= MData.m_FR[fcc];
		OrgP[FC] *= MData.m_FR[fcc];
		LabP[FC] *= MData.m_FR[fcc];
		ComP[FC] *= MData.m_FR[fcc];

		rcvl[gcc] *= MData.m_UR[fcc];
		rcl[gcc] *= MData.m_UR[fcc];
		rcr[gcc] *= MData.m_UR[fcc];
		CRB1[gcc] *= MData.m_UR[fcc];
		CRB2[gcc] *= MData.m_UR[fcc];
		crhl[gcc] *= MData.m_UR[fcc]; 
		crhr[gcc] *= MData.m_UR[fcc];
		dphum[gcc] *= MData.m_UR[fcc];
		nh4[gcc] *= MData.m_UR[fcc];
		no3[gcc] *= MData.m_UR[fcc];
		no2[gcc] *= MData.m_UR[fcc];
		no[gcc] *= MData.m_UR[fcc];
		n2o[gcc] *= MData.m_UR[fcc];
		n2[gcc] *= MData.m_UR[fcc];
		nh3[gcc] *= MData.m_UR[fcc];
		urea[gcc] *= MData.m_UR[fcc];
		co2[gcc] *= MData.m_UR[fcc];
		doc[gcc] *= MData.m_UR[fcc];
		sph[gcc] = sph[gcc];
		water[1][gcc] *= MData.m_UR[fcc];

		/////////////////////////////////////////// compost receives influxes
		int CN=q+4;
		old_HouseN = Total_SOCSON(CN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
		old_HouseC = MData.ttc;

		rcvl[CN] += ToCompost[1];
		rcl[CN] += ToCompost[2];
		rcr[CN] += ToCompost[3] ;
		CRB1[CN] += ToCompost[4] ;
		CRB2[CN] += ToCompost[5] ;
		crhl[CN] += 0.5 * ToCompost[6]; 
		crhr[CN] += 0.5 * ToCompost[6] ;
		dphum[CN] += ToCompost[7] ;
		nh4[CN] += ToCompost[8] ;
		no3[CN] += ToCompost[9] ;
		no2[CN] += ToCompost[10] ;
		no[CN] += ToCompost[11] ;
		n2o[CN] += ToCompost[12] ;
		n2[CN] += ToCompost[13] ;
		nh3[CN] += ToCompost[14] ;	
		urea[CN] += ToCompost[15];
		co2[CN] += ToCompost[16] ;
		doc[CN] += ToCompost[17];
		sph[CN] = ToCompost[18];		
		water[1][CN] += ToCompost[19];
		OrgP[CN] += ToCompost[20];
		LabP[CN] += ToCompost[21];
		ComP[CN] += ToCompost[22];

		new_HouseN = Total_SOCSON(CN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
		new_HouseC = MData.ttc;

		MData.Pen_to_compost_C = new_HouseC - old_HouseC;
		MData.Pen_to_compost_N = new_HouseN - old_HouseN;
		MData.Pen_to_compost_H2O = ToCompost[19];
		MData.Pen_to_compost_P += (ToCompost[20]+ToCompost[21]+ToCompost[22]);

		day_wfps[CN] = 0.5;
		//temp[CN] = air_temp;
		
		/////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////// lagoon receives influxes
		CN=q+5;
		old_HouseN = Total_SOCSON(CN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
		old_HouseC = MData.ttc;

		rcvl[CN] += ToLagoon[1];
		rcl[CN] += ToLagoon[2];
		rcr[CN] += ToLagoon[3] ;
		CRB1[CN] += ToLagoon[4] ;
		CRB2[CN] += ToLagoon[5] ;
		crhl[CN] += 0.5 * ToLagoon[6]; 
		crhr[CN] += 0.5 * ToLagoon[6] ;
		dphum[CN] += ToLagoon[7] ;
		nh4[CN] += ToLagoon[8] ;
		no3[CN] += ToLagoon[9] ;
		no2[CN] += ToLagoon[10] ;
		no[CN] += ToLagoon[11] ;
		n2o[CN] += ToLagoon[12] ;
		n2[CN] += ToLagoon[13] ;
		nh3[CN] += ToLagoon[14] ;
		urea[CN] += ToLagoon[15];
		co2[CN] += ToLagoon[16] ;
		doc[CN] += ToLagoon[17];
		sph[CN] = ToLagoon[18];		
		water[1][CN] += ToLagoon[19];
		OrgP[CN] += ToLagoon[20];
		LabP[CN] += ToLagoon[21];
		ComP[CN] += ToLagoon[22];

		new_HouseN = Total_SOCSON(CN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
		new_HouseC = MData.ttc;

		MData.Pen_to_lagoon_C = new_HouseC - old_HouseC;
		MData.Pen_to_lagoon_N = new_HouseN - old_HouseN;
		MData.Pen_to_lagoon_H2O += ToLagoon[19];
		MData.Pen_to_lagoon_P += (ToLagoon[20]+ToLagoon[21]+ToLagoon[22]);

		/////////////////////////////////////////// digester receives influxes
		CN=q+6;
		old_HouseN = Total_SOCSON(CN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
		old_HouseC = MData.ttc;

		rcvl[CN] += ToDigester[1];
		rcl[CN] += ToDigester[2];
		rcr[CN] += ToDigester[3] ;
		CRB1[CN] += ToDigester[4] ;
		CRB2[CN] += ToDigester[5] ;
		crhl[CN] += 0.5 * ToDigester[6]; 
		crhr[CN] += 0.5 * ToDigester[6];
		dphum[CN] += ToDigester[7] ;
		nh4[CN] += ToDigester[8] ;
		no3[CN] += ToDigester[9] ;
		no2[CN] += ToDigester[10] ;
		no[CN] += ToDigester[11] ;
		n2o[CN] += ToDigester[12] ;
		n2[CN] += ToDigester[13] ;
		nh3[CN] += ToDigester[14] ;
		urea[CN] += ToDigester[15];
		co2[CN] += ToDigester[16] ;
		doc[CN] += ToDigester[17];
		sph[CN] = ToDigester[18];		
		water[1][CN] += ToDigester[19];
		OrgP[CN] += ToDigester[20];
		LabP[CN] += ToDigester[21];
		ComP[CN] += ToDigester[22];

		new_HouseN = Total_SOCSON(CN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
		new_HouseC = MData.ttc;

		MData.Pen_to_digester_C = new_HouseC - old_HouseC;
		MData.Pen_to_digester_N = new_HouseN - old_HouseN;
		MData.Pen_to_digester_H2O = ToDigester[19];
		MData.Pen_to_digester_P += (ToDigester[20]+ToDigester[21]+ToDigester[22]);

		Total_SOCSON(CN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);

		}

}

#endif