#include "stdafx.h"
#include "Dndcgo.h"
#include "Source_main.h"
#include "Dndc_tool.h"
#include "math.h"

#ifdef MANURE 
extern class_manure MData;

class_model *DNDC;

float DungCN[7]={0.0, 16.0, 16.0, 16.0, 18.5, 3.1, 7.9}; //dung C/N ratio for dairy cattle, beef, veal, pig, sheep, foultry
float UrineCN[7]={0.0, 1.25, 1.25, 1.25, 4.24, 4.0, 4.0};//urine C/N ratio 
float DungH2O[7]={0.0, 0.8, 0.8, 0.8, 0.82, 0.68, 0.8}; //water fraction of dung
float UrineH2O[7]={0.0, 10.0, 10.0, 10.0, 3.3, 0.0, 0.0}; //kg water/head/day in urine
float UrineNF[7]={0.0, 0.5, 0.5, 0.5, 0.786, 0.7, 0.0}; //urine fraction of total waste N
float MilkNc[7]={0.0, 0.005, 0.005, 0.005, 0.005, 0.005, 0.005};// N concentration in milk by weiht, kg N/kg milk
float MeatNc[7]={0.0, 0.022, 0.022, 0.022, 0.022, 0.022, 0.024};// N concentration in meat by weight, kg N/kg meat

float old_HouseC, old_HouseN, new_HouseC, new_HouseN;
float old_DigesterC, old_DigesterN, new_DigesterC, new_DigesterN;
float old_LagoonC, old_LagoonN, new_LagoonC, new_LagoonN;
float CN_partition(float addC, float addN, float rcnrr, float rcnh);

void class_manure::HouseManureProduct(int FN)
{
	int i;
	float MilkN, MeatN, hd_ManureC, hd_ManureWater, ManureCN, ManureC;
	float hd_ManureN, hd_UrineN, hd_DungN, hd_DungC, hd_DungDM, hd_DungWater, hd_UrineC, hd_UrineWater;
	float day_meat, day_milk, ManureN, MilkP, MeatP, hd_UrineP, hd_DungP;
	
	MData.day_intake_C = 0.0;
	MData.day_intake_N = 0.0;
	MData.day_intake_P = 0.0;
	MData.day_milk_N=0.0;
	MData.day_milk_C=0.0;
	MData.day_milk_P=0.0;
	MData.day_meat_N=0.0;
	MData.day_meat_C=0.0;
	MData.day_meat_P=0.0;
	
	MData.day_milk_product=0.0;
	MData.day_urine_water=0.0;
	MData.day_dung_water=0.0;
	
	MData.mm_UrineN[FN] =0.0;//kg N/day
	MData.mm_UrineC[FN] =0.0;//kg C/day
	MData.mm_UrineP[FN] =0.0;//kg P/day
	MData.mm_DungN[FN] =0.0;//kg N/day
	MData.mm_DungC[FN] =0.0;	//kg C/day
	MData.mm_DungP[FN] =0.0;
	MData.mm_DungWater[FN] =0.0;//kg water/day
	MData.mm_UrineWater[FN] =0.0;
	MData.animal_co2 = 0.0;

	//Milk and meat production per animal per day
	MData.mm_FeedC[FN] = MData.mm_FeedRate[FN] * 0.4; // kg dry matter/animal/day - kg C/animal/day
	MData.mm_FeedN[FN] = MData.mm_FeedProtein[FN] * 0.01 * MData.mm_FeedRate[FN] / 6.25; // kg N/animal/day
	MData.mm_FeedP[FN] = MData.mm_FeedTP[FN] * 0.01 * MData.mm_FeedRate[FN]; // kg P/animal/day
	if(MData.mm_AnimalType[FN]==1) //lactating cow
	{
		MilkN = MData.mm_FeedN[FN] * 0.25; // Powell et al, 2006
		MeatN = MData.mm_FeedN[FN] * 0.05;		
		MData.mm_MilkProduct[FN] = MilkN / 0.005; // milk production kg/animal/day
		MilkP = MData.mm_FeedP[FN] * 0.367;//milk P, kg P/cow/day
		MeatP = MData.mm_FeedP[FN] * 0.056;//meat P, kg P/cow/day
		hd_UrineP = MData.mm_FeedP[FN] * 0.011;//urine P, kg P/cow/day
		hd_DungP = MData.mm_FeedP[FN] * 0.566;//feces P, kg P/cow/day
	}
	else
	{
		MilkN = 0.0; // Powell et al, 2006
		MeatN = MData.mm_FeedN[FN] * 0.3;	
		MData.mm_MilkProduct[FN] = 0.0;
		MilkP = 0.0;
		MeatP = MData.mm_FeedP[FN] * 0.28;//meat P, kg P/cow/day
		hd_UrineP = MData.mm_FeedP[FN] * 0.02;//urine P, kg P/cow/day
		hd_DungP = MData.mm_FeedP[FN] * 0.7;//feces P, kg P/cow/day

		//Sheep hair contains N 16% of DM
	}
	MData.mm_MeatProduct[FN] = MeatN / 0.0484;//0.022; // meat production, kg/animal/day

	//enteric N2O£¬ 0.5g N2O-N/kg feed-N; 0.2% of intake N
	float head_enteric_n2o = 0.002 * MData.mm_FeedN[FN];//0.0005; 0.9 * 0.001981 * MData.mm_FeedN[FN] / 0.225333;// * MData.mm_AnimalHeads[FN]; //kg N/day/head

	//manure production per animal per day
	hd_ManureN = MData.mm_FeedN[FN] - (MilkN + MeatN + head_enteric_n2o);//kg N/head/day
	
	hd_UrineN = UrineNF[MData.mm_AnimalType[FN]] * hd_ManureN;//kg N/head/day, Broderick 2003; Wattiaux and Karg 2004
	//hd_DungN = (1.0 - UrineNF[MData.mm_AnimalType[FN]]) * hd_ManureN;//kg N/head/day
	hd_DungN = hd_ManureN - hd_UrineN;//kg N/head/day
			
	hd_DungC = hd_DungN * DungCN[MData.mm_AnimalType[FN]];//kg C/head/day
	hd_DungDM = hd_DungC / 0.58;// kg dry matter/head/day
	hd_UrineC = hd_UrineN * UrineCN[MData.mm_AnimalType[FN]];//kg C/head/day
		
	hd_ManureC = hd_DungC + hd_UrineC;//kg C/head/day
	ManureCN = hd_ManureC / hd_ManureN;//kg N/head/day
		
	hd_DungWater = hd_DungDM / (1.0 - DungH2O[MData.mm_AnimalType[FN]]) * DungH2O[MData.mm_AnimalType[FN]];//kg water/head/day
	hd_UrineWater = UrineH2O[MData.mm_AnimalType[FN]];//kg water/head/day
	hd_ManureWater = hd_DungWater + hd_UrineWater;//kg water/head/day

	//manure production per farm per day
	ManureN = hd_ManureN * MData.mm_AnimalHeads[FN];
	ManureC = hd_ManureC * MData.mm_AnimalHeads[FN];
	MData.mm_DungDM = hd_DungDM * MData.mm_AnimalHeads[FN];
	MData.mm_ManureWater[FN] = hd_ManureWater * MData.mm_AnimalHeads[FN];
	MData.day_enteric_n2o[FN] = head_enteric_n2o * MData.mm_AnimalHeads[FN]; //kg N/day
			
	MData.mm_UrineN[FN] = hd_UrineN * MData.mm_AnimalHeads[FN];//kg N/day
	MData.mm_UrineC[FN] = hd_UrineC * MData.mm_AnimalHeads[FN];//kg C/day
	MData.mm_DungN[FN] = hd_DungN * MData.mm_AnimalHeads[FN];//kg N/day
	MData.mm_DungC[FN] = hd_DungC * MData.mm_AnimalHeads[FN];	//kg C/day
	MData.mm_UrineP[FN] = hd_UrineP * MData.mm_AnimalHeads[FN];//kg P/day
	MData.mm_DungP[FN] = hd_DungP * MData.mm_AnimalHeads[FN];//kg P/day

	MData.mm_DungWater[FN] = hd_DungWater * MData.mm_AnimalHeads[FN];//kg water/day
	MData.mm_UrineWater[FN] = hd_UrineWater * MData.mm_AnimalHeads[FN];//kg water/day	
	MData.ManureCapacity = DungH2O[MData.mm_AnimalType[FN]];
		
	MData.day_intake_C = MData.mm_FeedC[FN] * MData.mm_AnimalHeads[FN];
	MData.day_intake_N = MData.mm_FeedN[FN] * MData.mm_AnimalHeads[FN];
	MData.day_intake_P = MData.mm_FeedP[FN] * MData.mm_AnimalHeads[FN];
		
	MData.day_milk_N = MilkN * MData.mm_AnimalHeads[FN];
	MData.day_milk_C = MilkN * MData.mm_AnimalHeads[FN] / MilkNc[MData.mm_AnimalType[FN]] * 0.05 * 0.4;
	
	MData.day_meat_N = MeatN * MData.mm_AnimalHeads[FN];
	MData.day_meat_C = MeatN * MData.mm_AnimalHeads[FN] / MeatNc[MData.mm_AnimalType[FN]] * 0.2 * 0.4;
	MData.day_milk_P = MilkP * MData.mm_AnimalHeads[FN];
	MData.day_meat_P = MeatP * MData.mm_AnimalHeads[FN];
	
	MData.day_milk_product = MData.mm_MilkProduct[FN]*MData.mm_AnimalHeads[FN];//kg milk/MData.day
	MData.day_urine_water = MData.mm_UrineWater[FN];
	MData.day_dung_water = MData.mm_DungWater[FN];

	  
	float KKanimal_co2 = (MData.mm_FeedC[FN] * MData.mm_AnimalHeads[FN] - 
		MilkN * MData.mm_AnimalHeads[FN] / MilkNc[MData.mm_AnimalType[FN]] * 0.05 * 0.4 -
		MeatN * MData.mm_AnimalHeads[FN] / MeatNc[MData.mm_AnimalType[FN]] * 0.2 * 0.4 -
		ManureC);

	MData.animal_co2 = MData.mm_FeedC[FN] * 0.4805193 * MData.mm_AnimalHeads[FN];

	float ddCC = MData.animal_co2 - KKanimal_co2;
	MData.mm_DungC[FN] -= ddCC;

	MData.yr_enteric_co2 += MData.animal_co2;
	MData.yr_intake_C += MData.day_intake_C;
	MData.yr_intake_N += MData.day_intake_N;
	MData.yr_intake_P += MData.day_intake_P;
	MData.yr_milk_N += MData.day_milk_N;
	MData.yr_milk_C += MData.day_milk_C;
	MData.yr_milk_P += MData.day_milk_P;
	MData.yr_meat_N += MData.day_meat_N;
	MData.yr_meat_C += MData.day_meat_C;
	MData.yr_meat_P += MData.day_meat_P;
	MData.yr_animal_CO2 += MData.animal_co2;
	MData.yr_milk_product += MData.day_milk_product;

	MData.yr_urine_C += MData.mm_UrineC[FN];
	MData.yr_urine_N += MData.mm_UrineN[FN];
	MData.yr_dung_C += MData.mm_DungC[FN];
	MData.yr_dung_N += MData.mm_DungN[FN];
	MData.yr_urine_P += MData.mm_UrineP[FN];
	MData.yr_dung_P += MData.mm_DungP[FN];
	MData.yr_urine_water += MData.day_urine_water;
	MData.yr_dung_water += MData.day_dung_water;
	
	
	
	
}

void class_manure::HouseManureOccumulation(int FN, int GN, int q, int jday, float rcnrvl, float rcnrl, float rcnrr, 
		   float rcnh, float rcnb, float *urea, float *doc, float *rcvl, 
		   float *rcl, float *rcr, float *crhl, float *crhr, float *CRB1, float *CRB2, float *dphum, double water[25][180],
		   float rcnm, float *soc, float *son, float *SIN, float *nh4, float *no3, float *no2, float *no, float *n2o,
		   float *n2, float *nh3, float rcnh2, float *co2, float *sop, float *OrgP, float *LabP, float *ComP,
		   float precipitation)
{
	float AddC=0.0, AddN=0.0, ManureAddCN, Free_N, solid_C, solid_CN, solid_MN, AddCN;
	int i, j;

	Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);

	if(MData.mm_GroundSurface[FN]==0) //slatted floor with gutter, urine, DOC, water go to gutter
	{
		float wb = 0.8;
		urea[q+8] += MData.mm_UrineN[FN] * wb;
		doc[q+8] += MData.mm_UrineC[FN] * wb;
		water[1][q+8] += (MData.mm_UrineWater[FN] + MData.mm_DungWater[FN]*wb);
		
		urea[GN] += MData.mm_UrineN[FN] * (1.0-wb);
		doc[GN] += MData.mm_UrineC[FN] * (1.0-wb);
		water[1][GN] += (MData.mm_DungWater[FN]*(1.0-wb));
	}
	else if(MData.mm_GroundSurface[FN]==1) //concrete floor
	{
		urea[GN] += MData.mm_UrineN[FN];
		doc[GN] += MData.mm_UrineC[FN];
		water[1][GN] += (MData.mm_DungWater[FN]+MData.mm_UrineWater[FN]);//kg water/feedlot

		if(MData.mm_GroundSurface[FN]==3)
		{
			float AddRainWater = precipitation * MData.mm_GroundArea[FN] * 1000.0;
			water[1][GN] += AddRainWater;
		}

		urea[q+8] = 0.0;
		doc[q+8] = 0.0;
		water[1][q+8] = 0.0;
	}	
	else if(MData.mm_GroundSurface[FN]==2) //sub-floor (only gutter)
	{
		urea[q+8] += MData.mm_UrineN[FN];
		doc[q+8] += MData.mm_UrineC[FN];
		water[1][q+8] += (MData.mm_UrineWater[FN] + MData.mm_DungWater[FN]);
	}
	else if(MData.mm_GroundSurface[FN]==3) //outdoor pen with bare soil
	{
		urea[GN] += MData.mm_UrineN[FN];
		doc[GN] += MData.mm_UrineC[FN];
		water[1][GN] += (MData.mm_DungWater[FN]+MData.mm_UrineWater[FN]);//kg water/feedlot

		if(precipitation>0.0)
		{
			float AddRainWater = precipitation * MData.mm_GroundArea[FN] * 1000.0;
			water[1][GN] += AddRainWater;//kg water/lot
		}

		urea[q+8] = 0.0;
		doc[q+8] = 0.0;
		water[1][q+8] = 0.0;		
	}
	else
	{
		AfxMessageBox("Error for defining floor type");
		exit(0);
	}
	
	//Partition feces C and N into ressistant litter and humads
	AddC = MData.mm_DungC[FN];
	AddN = MData.mm_DungN[FN];

	float Nh, Nl, Ch, Cl, AddH;
	float CNl = rcnrr, CNh = rcnh;

	Nh = (CNl * AddN - AddC) / (CNl - CNh);
	Nl = AddN - Nh;
	Cl = CNl * Nl;
	Ch = CNh * Nh;

	AddH = Ch;
		
	if(MData.mm_GroundSurface[FN]==0) //slatted floor with gutter
	{
		float wb = 0.2;
		crhl[q+8] += 0.4*AddH*wb; 
		crhr[q+8] += 0.4*AddH*wb;
		CRB1[q+8] += 0.1*AddH*wb;
		CRB2[q+8] += 0.1*AddH*wb;
		rcr[q+8] += Cl*wb;

		crhl[GN] += 0.4*AddH*(1.0-wb); 
		crhr[GN] += 0.4*AddH*(1.0-wb);
		CRB1[GN] += 0.1*AddH*(1.0-wb);
		CRB2[GN] += 0.1*AddH*(1.0-wb);
		rcr[GN] += Cl*(1.0-wb);

		LabP[GN] += MData.mm_UrineP[FN];
		OrgP[GN] += MData.mm_DungP[FN];
	}
	else if(MData.mm_GroundSurface[FN]==1||MData.mm_GroundSurface[FN]==3) //concrete floor or outdoor pen with bare soil
	{
		crhl[GN] += 0.4*AddH; 
		crhr[GN] += 0.4*AddH;
		CRB1[GN] += 0.1*AddH;
		CRB2[GN] += 0.1*AddH;
		rcr[GN] += Cl;
		LabP[GN] += MData.mm_UrineP[FN];
		OrgP[GN] += MData.mm_DungP[FN];
	}
	else if(MData.mm_GroundSurface[FN]==2) //sub-floor (only gutter)
	{
		crhl[q+8] += 0.4*AddH; 
		crhr[q+8] += 0.4*AddH;
		CRB1[q+8] += 0.1*AddH;
		CRB2[q+8] += 0.1*AddH;
		LabP[q+8] += MData.mm_UrineP[FN];
		OrgP[q+8] += MData.mm_DungP[FN];
		rcr[q+8] += Cl;
	}
	else
	{
		AfxMessageBox("Error for defining floor type");
		exit(0);
	}

/*	float TTTN1, TTTN2, T1N, T2N;
	
	T1N = Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);

	T2N = Total_SOCSON(q+8, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);

	TTTN1 = T1N + T2N;

	TTTN2 = MData.mm_UrineN[FN] + MData.mm_DungN[FN];*/

	//bedding material partitioning
	if(MData.mm_BedApplications>0.0)
	{
		int BF = jday%(int)MData.mm_BedApplications;
		if(BF==0&&(MData.mm_BedType==1||MData.mm_BedType==2||MData.mm_BedType==3))
		{		
			float AddC1, AddC2, AddC3, dInertC;
			AddC = MData.mm_BedAmount*0.4;
			AddCN = MData.mm_BedCNN;

			MData.day_bed_C += AddC;
			MData.day_bed_N += AddC/AddCN;

			MData.yr_bed_C += AddC;
			MData.yr_bed_N += AddC/AddCN;

			DNDC->alloc_litter_into_pools(&AddCN, &AddC, 2, &AddC1, &AddC2, &AddC3, &dInertC, 1, rcnrvl, rcnrl, rcnrr, nh4);

			if(MData.mm_GroundSurface[FN]==2) //sub-floor (only gutter)
			{
				rcvl[q+8] += AddC1;
				rcl[q+8] += AddC2;
				rcr[q+8] += AddC3;
			}
			else
			{
				rcvl[GN] += AddC1;
				rcl[GN] += AddC2;
				rcr[GN] += AddC3;
			}
		}
	}
		
	

	//Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
	//			 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, 
	//			 sop,OrgP, LabP, ComP);
	
}

float CN_partition(float addC, float addN, float rcnrr, float rcnh)
{
	float x,y;
	y = (rcnrr*addN - addC) / (rcnrr-rcnh);
	x = addN - y;

	return(y);
}

void class_manure::FeedlotManureRemoval(int FN, int GN, int q, float rcnrvl, float rcnrl, float rcnrr, float rcnb, float rcnh, float rcnm,
								 float *soc, float *son, float *SIN, float *rcvl, float *rcl, float *rcr, 
								 float *CRB1, float *CRB2, float *crhl, float *crhr, float *dphum, float *doc, 
								 float *nh4, float *no3, float *no2, float *no, float *n2o, float *n2, float *nh3, 
								 float *urea, int jday, float *co2, float *sph, double water[25][180],
								 float *day_wfps, float *temp, float air_temp, int scale, float rcnh2, float *sop, 
								 float *OrgP, float *LabP, float *ComP, FILE* fFeedOut)// float *House_to_compost_C, float *House_to_compost_N, float *House_to_compost_H2O)
{
	float ToCompost[24]={0.0,0.0}, ToLagoon[24]={0.0,0.0}, ToDigester[24]={0.0,0.0}, ToField[24]={0.0,0.0};
	float old_FloorN, old_FloorC, new_FloorN, new_FloorC;
	float old_GutterN, old_GutterC, new_GutterN, new_GutterC;
	float old_floor_H2O, new_floor_H2O, old_gutter_H2O, new_gutter_H2O;
	float old_floor_P, new_floor_P, old_gutter_P, new_gutter_P; 
	int CLD, fcc = FN, gcc=q+8, CNN=q+4, LNN=q+5, DNN=q+6, FNN=3;
	float old_N, old_C, old_P, old_H2O;
	float new_N, new_C, new_P, new_H2O;
	
	if(MData.m_ManureRemoveFrequency[fcc]==0.0) CLD=1;
	else CLD = jday%(int)MData.m_ManureRemoveFrequency[fcc];
	
	if(CLD==0)
	{
		old_FloorN = Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
					 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
		old_FloorC = MData.ttc;

		old_GutterN = Total_SOCSON(gcc, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
					 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
		old_GutterC = MData.ttc;

		if(MData.mm_GroundSurface[FN]==2) 
		{
			old_floor_P = 0.0;
			old_gutter_P = OrgP[gcc]+LabP[gcc]+ComP[gcc];
		}
		else
		{
			old_floor_P = OrgP[GN]+LabP[GN]+ComP[GN];
			old_gutter_P = 0.0;
		}

		old_floor_H2O = water[1][GN];
		old_gutter_H2O = water[1][gcc];
			
		if(MData.m_FC[fcc]>0.0||MData.m_UC[fcc]>0.0)//feedlot to compost
		{
			old_N = Total_SOCSON(CNN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
			old_C = MData.ttc;
			old_P = OrgP[CNN]+LabP[CNN]+ComP[CNN];
			old_H2O = water[1][CNN];

			ToCompost[1] = MData.m_FC[fcc] * rcvl[GN] + MData.m_UC[fcc] * rcvl[gcc];//floor and gutter
			ToCompost[2] = MData.m_FC[fcc] * rcl[GN] + MData.m_UC[fcc] * rcl[gcc];
			ToCompost[3] = MData.m_FC[fcc] * rcr[GN] + MData.m_UC[fcc] * rcr[gcc];
			ToCompost[4] = MData.m_FC[fcc] * CRB1[GN] + MData.m_UC[fcc] * CRB1[gcc];
			ToCompost[5] = MData.m_FC[fcc] * CRB2[GN] + MData.m_UC[fcc] * CRB2[gcc];
			ToCompost[6] = MData.m_FC[fcc] * (crhl[GN]+crhr[GN]) + MData.m_UC[fcc] * (crhl[gcc]+crhr[gcc]);
			ToCompost[7] = MData.m_FC[fcc] * dphum[GN] + MData.m_UC[fcc] * dphum[gcc];
			ToCompost[8] = MData.m_FC[fcc] * nh4[GN] + MData.m_UC[fcc] * nh4[gcc];
			ToCompost[9] = MData.m_FC[fcc] * no3[GN] + MData.m_UC[fcc] * no3[gcc];
			ToCompost[10] = MData.m_FC[fcc] * no2[GN] + MData.m_UC[fcc] * no2[gcc];
			ToCompost[11] = MData.m_FC[fcc] * no[GN] + MData.m_UC[fcc] * no[gcc];
			ToCompost[12] = MData.m_FC[fcc] * n2o[GN] + MData.m_UC[fcc] * n2o[gcc];
			ToCompost[13] = MData.m_FC[fcc] * n2[GN] + MData.m_UC[fcc] * n2[gcc];
			ToCompost[14] = MData.m_FC[fcc] * nh3[GN] + MData.m_UC[fcc] * nh3[gcc];	
			ToCompost[15] = MData.m_FC[fcc] * urea[GN] + MData.m_UC[fcc] * urea[gcc];
			ToCompost[16] = MData.m_FC[fcc] * co2[GN] + MData.m_UC[fcc] * co2[gcc];	
			ToCompost[17] = MData.m_FC[fcc] * doc[GN] + MData.m_UC[fcc] * doc[gcc];
			ToCompost[18] = sph[GN];
			ToCompost[19] = MData.m_FC[fcc] * water[1][GN] + MData.m_UC[fcc] * water[1][gcc];
			ToCompost[20] = MData.m_FC[fcc] * OrgP[GN] + MData.m_UC[fcc] * OrgP[gcc];
			ToCompost[21] = MData.m_FC[fcc] * LabP[GN] + MData.m_UC[fcc] * LabP[gcc];
			ToCompost[22] = MData.m_FC[fcc] * ComP[GN] + MData.m_UC[fcc] * ComP[gcc];

			rcvl[CNN] += ToCompost[1];
			rcl[CNN] += ToCompost[2];
			rcr[CNN] += ToCompost[3];
			CRB1[CNN] += ToCompost[4];
			CRB2[CNN] += ToCompost[5];
			crhl[CNN] += 0.5*ToCompost[6]; 
			crhr[CNN] += 0.5*ToCompost[6]; 
			dphum[CNN] += ToCompost[7];
			nh4[CNN] += ToCompost[8];
			no3[CNN] += ToCompost[9];
			no2[CNN] += ToCompost[10];
			no[CNN] += ToCompost[11];
			n2o[CNN] += ToCompost[12];
			n2[CNN] += ToCompost[13];
			nh3[CNN] += ToCompost[14];
			urea[CNN] += ToCompost[15];
			co2[CNN] += ToCompost[16];
			doc[CNN] += ToCompost[17];
			sph[CNN] = sph[CNN];
			water[1][CNN] += ToCompost[19];
			OrgP[CNN] += ToCompost[20];
			LabP[CNN] += ToCompost[21];
			ComP[CNN] += ToCompost[22];

			new_N = Total_SOCSON(CNN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
					 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
			new_C = MData.ttc;
			new_P = OrgP[CNN]+LabP[CNN]+ComP[CNN];
			new_H2O = water[1][CNN];

			MData.House_to_compost_N = new_N - old_N;
			MData.House_to_compost_C = new_C - old_C;
			MData.House_to_compost_P = new_P - old_P;
			MData.House_to_compost_H2O = new_H2O - old_H2O;
		}


		if(MData.m_FL[fcc]>0.0||MData.m_UL[fcc]>0.0)//feedlot to lagoon
		{
			old_N = Total_SOCSON(LNN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
					 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
			old_C = MData.ttc;
			old_P = OrgP[LNN]+LabP[LNN]+ComP[LNN];
			old_H2O = water[1][LNN];
		
			ToLagoon[1] = MData.m_FL[fcc] * rcvl[GN] + MData.m_UL[fcc] * rcvl[gcc];
			ToLagoon[2] = MData.m_FL[fcc] * rcl[GN] + MData.m_UL[fcc] * rcl[gcc];
			ToLagoon[3] = MData.m_FL[fcc] * rcr[GN] + MData.m_UL[fcc] * rcr[gcc];
			ToLagoon[4] = MData.m_FL[fcc] * CRB1[GN] + MData.m_UL[fcc] * CRB1[gcc];
			ToLagoon[5] = MData.m_FL[fcc] * CRB2[GN] + MData.m_UL[fcc] * CRB2[gcc];
			ToLagoon[6] = MData.m_FL[fcc] * (crhl[GN]+crhr[GN]) + MData.m_UL[fcc] * (crhl[gcc]+crhr[gcc]);
			ToLagoon[7] = MData.m_FL[fcc] * dphum[GN] + MData.m_UL[fcc] * dphum[gcc];
			ToLagoon[8] = MData.m_FL[fcc] * nh4[GN] + MData.m_UL[fcc] * nh4[gcc];
			ToLagoon[9] = MData.m_FL[fcc] * no3[GN] + MData.m_UL[fcc] * no3[gcc];
			ToLagoon[10] = MData.m_FL[fcc] * no2[GN] + MData.m_UL[fcc] * no2[gcc];
			ToLagoon[11] = MData.m_FL[fcc] * no[GN] + MData.m_UL[fcc] * no[gcc];
			ToLagoon[12] = MData.m_FL[fcc] * n2o[GN] + MData.m_UL[fcc] * n2o[gcc];
			ToLagoon[13] = MData.m_FL[fcc] * n2[GN] + MData.m_UL[fcc] * n2[gcc];
			ToLagoon[14] = MData.m_FL[fcc] * nh3[GN] + MData.m_UL[fcc] * nh3[gcc];	
			ToLagoon[15] = MData.m_FL[fcc] * urea[GN] + MData.m_UL[fcc] * urea[gcc];
			ToLagoon[16] = MData.m_FL[fcc] * co2[GN] + MData.m_UL[fcc] * co2[gcc];	
			ToLagoon[17] = MData.m_FL[fcc] * doc[GN] + MData.m_UL[fcc] * doc[gcc];
			ToLagoon[18] = sph[GN];
			ToLagoon[19] = MData.m_FL[fcc] * water[1][GN] + MData.m_UL[fcc] * water[1][gcc];
			ToLagoon[20] = MData.m_FL[fcc] * OrgP[GN] + MData.m_UL[fcc] * OrgP[gcc];
			ToLagoon[21] = MData.m_FL[fcc] * LabP[GN] + MData.m_UL[fcc] * LabP[gcc];
			ToLagoon[22] = MData.m_FL[fcc] * ComP[GN] + MData.m_UL[fcc] * ComP[gcc];

			rcvl[LNN] += ToLagoon[1];
			rcl[LNN] += ToLagoon[2];
			rcr[LNN] += ToLagoon[3];
			CRB1[LNN] += ToLagoon[4];
			CRB2[LNN] += ToLagoon[5];
			crhl[LNN] += 0.5*ToLagoon[6]; 
			crhr[LNN] += 0.5*ToLagoon[6]; 
			dphum[LNN] += ToLagoon[7];
			nh4[LNN] += ToLagoon[8];
			no3[LNN] += ToLagoon[9];
			no2[LNN] += ToLagoon[10];
			no[LNN] += ToLagoon[11];
			n2o[LNN] += ToLagoon[12];
			n2[LNN] += ToLagoon[13];
			nh3[LNN] += ToLagoon[14];
			urea[LNN] += ToLagoon[15];
			co2[LNN] += ToLagoon[16];
			doc[LNN] += ToLagoon[17];
			sph[LNN] = sph[LNN];
			water[1][LNN] += ToLagoon[19];
			OrgP[LNN] += ToLagoon[20];
			LabP[LNN] += ToLagoon[21];
			ComP[LNN] += ToLagoon[22];

			new_N = Total_SOCSON(LNN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
					 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
			new_C = MData.ttc;
			new_P = OrgP[LNN]+LabP[LNN]+ComP[LNN];
			new_H2O = water[1][LNN];

			MData.House_to_lagoon_N = new_N - old_N;
			MData.House_to_lagoon_C = new_C - old_C;
			MData.House_to_lagoon_P = new_P - old_P;
			MData.House_to_lagoon_H2O = new_H2O - old_H2O;
		}

		if(MData.m_FD[fcc]>0.0||MData.m_UD[fcc]>0.0)//feedlot to digester
		{
			old_N = Total_SOCSON(DNN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
					 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
			old_C = MData.ttc;
			old_P = OrgP[DNN]+LabP[DNN]+ComP[DNN];
			old_H2O = water[1][DNN];

			ToDigester[1] = MData.m_FD[fcc] * rcvl[GN] + MData.m_UD[fcc] * rcvl[gcc];
			ToDigester[2] = MData.m_FD[fcc] * rcl[GN] + MData.m_UD[fcc] * rcl[gcc];
			ToDigester[3] = MData.m_FD[fcc] * rcr[GN] + MData.m_UD[fcc] * rcr[gcc];
			ToDigester[4] = MData.m_FD[fcc] * CRB1[GN] + MData.m_UD[fcc] * CRB1[gcc];
			ToDigester[5] = MData.m_FD[fcc] * CRB2[GN] + MData.m_UD[fcc] * CRB2[gcc];
			ToDigester[6] = MData.m_FD[fcc] * (crhl[GN]+crhr[GN]) + MData.m_UD[fcc] * (crhl[gcc]+crhr[gcc]);
			ToDigester[7] = MData.m_FD[fcc] * dphum[GN] + MData.m_UD[fcc] * dphum[gcc];
			ToDigester[8] = MData.m_FD[fcc] * nh4[GN] + MData.m_UD[fcc] * nh4[gcc];
			ToDigester[9] = MData.m_FD[fcc] * no3[GN] + MData.m_UD[fcc] * no3[gcc];
			ToDigester[10] = MData.m_FD[fcc] * no2[GN] + MData.m_UD[fcc] * no2[gcc];
			ToDigester[11] = MData.m_FD[fcc] * no[GN] + MData.m_UD[fcc] * no[gcc];
			ToDigester[12] = MData.m_FD[fcc] * n2o[GN] + MData.m_UD[fcc] * n2o[gcc];
			ToDigester[13] = MData.m_FD[fcc] * n2[GN] + MData.m_UD[fcc] * n2[gcc];
			ToDigester[14] = MData.m_FD[fcc] * nh3[GN] + MData.m_UD[fcc] * nh3[gcc];	
			ToDigester[15] = MData.m_FD[fcc] * urea[GN] + MData.m_UD[fcc] * urea[gcc];
			ToDigester[16] = MData.m_FD[fcc] * co2[GN] + MData.m_UD[fcc] * co2[gcc];	
			ToDigester[17] = MData.m_FD[fcc] * doc[GN] + MData.m_UD[fcc] * doc[gcc];
			ToDigester[18] = sph[GN];
			ToDigester[19] = MData.m_FD[fcc] * water[1][GN] + MData.m_UD[fcc] * water[1][gcc];
			ToDigester[20] = MData.m_FD[fcc] * OrgP[GN] + MData.m_UD[fcc] * OrgP[gcc];
			ToDigester[21] = MData.m_FD[fcc] * LabP[GN] + MData.m_UD[fcc] * LabP[gcc];
			ToDigester[22] = MData.m_FD[fcc] * ComP[GN] + MData.m_UD[fcc] * ComP[gcc];

			rcvl[DNN] += ToDigester[1];
			rcl[DNN] += ToDigester[2];
			rcr[DNN] += ToDigester[3];
			CRB1[DNN] += ToDigester[4];
			CRB2[DNN] += ToDigester[5];
			crhl[DNN] += 0.5*ToDigester[6]; 
			crhr[DNN] += 0.5*ToDigester[6]; 
			dphum[DNN] += ToDigester[7];
			nh4[DNN] += ToDigester[8];
			no3[DNN] += ToDigester[9];
			no2[DNN] += ToDigester[10];
			no[DNN] += ToDigester[11];
			n2o[DNN] += ToDigester[12];
			n2[DNN] += ToDigester[13];
			nh3[DNN] += ToDigester[14];
			urea[DNN] += ToDigester[15];
			co2[DNN] += ToDigester[16];
			doc[DNN] += ToDigester[17];
			sph[DNN] = sph[DNN];
			water[1][DNN] += ToDigester[19];
			OrgP[DNN] += ToDigester[20];
			LabP[DNN] += ToDigester[21];
			ComP[DNN] += ToDigester[22];

			new_N = Total_SOCSON(DNN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
					 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
			new_C = MData.ttc;
			new_P = OrgP[DNN]+LabP[DNN]+ComP[DNN];
			new_H2O = water[1][DNN];

			MData.House_to_digester_N = new_N - old_N;
			MData.House_to_digester_C = new_C - old_C;
			MData.House_to_digester_P = new_P - old_P;
			MData.House_to_digester_H2O = new_H2O - old_H2O;
		}

		////////////////////////////////////////////////
		if(MData.m_FF[fcc]>0.0||MData.m_UF[fcc]>0.0)//feedlot to field
		{
			old_N = Total_SOCSON(DNN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
					 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
			old_C = MData.ttc;
			old_P = OrgP[FNN]+LabP[FNN]+ComP[FNN];
			old_H2O = water[1][FNN];

			ToField[1] = MData.m_FF[fcc] * rcvl[GN] + MData.m_UF[fcc] * rcvl[gcc];
			ToField[2] = MData.m_FF[fcc] * rcl[GN] + MData.m_UF[fcc] * rcl[gcc];
			ToField[3] = MData.m_FF[fcc] * rcr[GN] + MData.m_UF[fcc] * rcr[gcc];
			ToField[4] = MData.m_FF[fcc] * CRB1[GN] + MData.m_UF[fcc] * CRB1[gcc];
			ToField[5] = MData.m_FF[fcc] * CRB2[GN] + MData.m_UF[fcc] * CRB2[gcc];
			ToField[6] = MData.m_FF[fcc] * (crhl[GN]+crhr[GN]) + MData.m_UF[fcc] * (crhl[gcc]+crhr[gcc]);
			ToField[7] = MData.m_FF[fcc] * dphum[GN] + MData.m_UF[fcc] * dphum[gcc];
			ToField[8] = MData.m_FF[fcc] * nh4[GN] + MData.m_UF[fcc] * nh4[gcc];
			ToField[9] = MData.m_FF[fcc] * no3[GN] + MData.m_UF[fcc] * no3[gcc];
			ToField[10] = MData.m_FF[fcc] * no2[GN] + MData.m_UF[fcc] * no2[gcc];
			ToField[11] = MData.m_FF[fcc] * no[GN] + MData.m_UF[fcc] * no[gcc];
			ToField[12] = MData.m_FF[fcc] * n2o[GN] + MData.m_UF[fcc] * n2o[gcc];
			ToField[13] = MData.m_FF[fcc] * n2[GN] + MData.m_UF[fcc] * n2[gcc];
			ToField[14] = MData.m_FF[fcc] * nh3[GN] + MData.m_UF[fcc] * nh3[gcc];	
			ToField[15] = MData.m_FF[fcc] * urea[GN] + MData.m_UF[fcc] * urea[gcc];
			ToField[16] = MData.m_FF[fcc] * co2[GN] + MData.m_UF[fcc] * co2[gcc];	
			ToField[17] = MData.m_FF[fcc] * doc[GN] + MData.m_UF[fcc] * doc[gcc];
			ToField[18] = sph[GN];
			ToField[19] = MData.m_FF[fcc] * water[1][GN] + MData.m_UF[fcc] * water[1][gcc];
			ToField[20] = MData.m_FF[fcc] * OrgP[GN] + MData.m_UF[fcc] * OrgP[gcc];
			ToField[21] = MData.m_FF[fcc] * LabP[GN] + MData.m_UF[fcc] * LabP[gcc];
			ToField[22] = MData.m_FF[fcc] * ComP[GN] + MData.m_UF[fcc] * ComP[gcc];

			rcvl[FNN] += ToField[1];
			rcl[FNN] += ToField[2];
			rcr[FNN] += ToField[3];
			CRB1[FNN] += ToField[4];
			CRB2[FNN] += ToField[5];
			crhl[FNN] += 0.5*ToField[6]; 
			crhr[FNN] += 0.5*ToField[6]; 
			dphum[FNN] += ToField[7];
			nh4[FNN] += ToField[8];
			no3[FNN] += ToField[9];
			no2[FNN] += ToField[10];
			no[FNN] += ToField[11];
			n2o[FNN] += ToField[12];
			n2[FNN] += ToField[13];
			nh3[FNN] += ToField[14];
			urea[FNN] += ToField[15];
			co2[FNN] += ToField[16];
			doc[FNN] += ToField[17];
			sph[FNN] = sph[FNN];
			water[1][FNN] += ToField[19];
			OrgP[FNN] += ToField[20];
			LabP[FNN] += ToField[21];
			ComP[FNN] += ToField[22];

			new_N = Total_SOCSON(FNN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
					 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
			new_C = MData.ttc;
			new_P = OrgP[FNN]+LabP[FNN]+ComP[FNN];
			new_H2O = water[1][FNN];

			MData.House_to_field_N = new_N - old_N;
			MData.House_to_field_C = new_C - old_C;
			MData.House_to_field_P = new_P - old_P;
			MData.House_to_field_H2O = new_H2O - old_H2O;
		}

		////////////////////////////////////////////////	

		fprintf(fFeedOut, "%d\n", jday);		

		for(int i2=1; i2<=22; i2++) 
		{
			fprintf(fFeedOut, "%f ", ToCompost[i2]);
		}
		fprintf(fFeedOut, "\n");

		for(int i3=1; i3<=22; i3++) 
		{
			fprintf(fFeedOut, "%f ", ToLagoon[i3]);
		}
		fprintf(fFeedOut, "\n");

		for(int i4=1; i4<=22; i4++) 
		{
			fprintf(fFeedOut, "%f ", ToDigester[i4]);
		}
		fprintf(fFeedOut, "\n");

		for(int i5=1; i5<=22; i5++) 
		{
			fprintf(fFeedOut, "%f ", ToField[i5]);
		}
		fprintf(fFeedOut, "\n");

		//// remain on floor 
		rcvl[GN] *= MData.m_FR[fcc];
		rcl[GN] *= MData.m_FR[fcc];
		rcr[GN] *= MData.m_FR[fcc];
		CRB1[GN] *= MData.m_FR[fcc];
		CRB2[GN] *= MData.m_FR[fcc];
		crhl[GN] *= 0.5*MData.m_FR[fcc]; 
		crhr[GN] *= 0.5*MData.m_FR[fcc];
		dphum[GN] *= MData.m_FR[fcc];
		nh4[GN] *= MData.m_FR[fcc];
		no3[GN] *= MData.m_FR[fcc];
		no2[GN] *= MData.m_FR[fcc];
		no[GN] *= MData.m_FR[fcc];
		n2o[GN] *= MData.m_FR[fcc];
		n2[GN] *= MData.m_FR[fcc];
		nh3[GN] *= MData.m_FR[fcc];
		urea[GN] *= MData.m_FR[fcc];
		co2[GN] *= MData.m_FR[fcc];
		doc[GN] *= MData.m_FR[fcc];
		sph[GN] = sph[GN];
		water[1][GN] *= MData.m_FR[fcc];
		OrgP[GN] *= MData.m_FR[fcc];
		LabP[GN] *= MData.m_FR[fcc];
		ComP[GN] *= MData.m_FR[fcc];

		//// remain in gutter 
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
		OrgP[gcc] *= MData.m_UR[fcc];
		LabP[gcc] *= MData.m_UR[fcc];
		ComP[gcc] *= MData.m_UR[fcc];

		new_FloorN = Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
		new_FloorC = MData.ttc;

		if(MData.mm_GroundSurface[FN]==2) 
		{
			new_floor_P = 0.0;
			new_gutter_P = OrgP[gcc]+LabP[gcc]+ComP[gcc];
		}
		else
		{
			new_floor_P = OrgP[GN]+LabP[GN]+ComP[GN];
			new_gutter_P = 0.0;
		}

		new_floor_H2O = water[1][GN];

		new_GutterN = Total_SOCSON(gcc, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
					 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
		new_GutterC = MData.ttc;		
		new_gutter_H2O = water[1][gcc];
		
		MData.House_floor_removedN = new_FloorN - old_FloorN; 
		MData.House_floor_removedC = new_FloorC - old_FloorC; 
		MData.House_gutter_removedN = new_GutterN - old_GutterN;
		MData.House_gutter_removedC = new_GutterC - old_GutterC; 
		MData.House_floor_removed_P = new_floor_P - old_floor_P;
		MData.House_gutter_removed_P = new_gutter_P - old_gutter_P;
		MData.House_floor_removed_H2O = new_floor_H2O - old_floor_H2O;
		MData.House_gutter_removed_H2O = new_gutter_H2O - old_gutter_H2O;

/*		// compost receives influxes
		
		int CN=q+4;//compost
		
		old_HouseN = Total_SOCSON(CN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);

		old_HouseC = MData.ttc;

		if(MData.m_LitterAmount>0.0)
		{
			float AddC = MData.m_LitterAmount;
			float AddCN = MData.m_LitterCN;
			float AddC1=0.0, AddC2=0.0, AddC3=0.0, dInertC=0.0;

			DNDC->alloc_litter_into_pools(&AddCN, &AddC, 0, &AddC1, &AddC2, &AddC3, &dInertC, 1, rcnrvl, rcnrl, rcnrr, nh4);

			rcvl[CN] += AddC1;
			rcl[CN] += AddC2;
			rcr[CN] += AddC3;
		}
*/
		}	
		else
		{
			fprintf(fFeedOut, "%d\n", jday);
			for(int i6=1; i6<=22; i6++) fprintf(fFeedOut, "%f ", 0.0);
			fprintf(fFeedOut, "\n");
			for(int i7=1; i7<=22; i7++) fprintf(fFeedOut, "%f ", 0.0);
			fprintf(fFeedOut, "\n");
			for(int i8=1; i8<=22; i8++) fprintf(fFeedOut, "%f ", 0.0);
			fprintf(fFeedOut, "\n");
			for(int i9=1; i9<=22; i9++) fprintf(fFeedOut, "%f ", 0.0);
			fprintf(fFeedOut, "\n");

			MData.House_floor_removedN = 0.0; 
			MData.House_floor_removedC = 0.0; 
			MData.House_gutter_removedN = 0.0;
			MData.House_gutter_removedC = 0.0; 
			MData.House_floor_removed_P = 0.0;
			MData.House_gutter_removed_P = 0.0;
			MData.House_floor_removed_H2O = 0.0;
			MData.House_gutter_removed_H2O = 0.0;

		}
}

void class_manure::StorageManureRemoval(int FN, int GN, int q, float rcnrvl, float rcnrl, float rcnrr, float rcnb, float rcnh, float rcnm,
								 float *soc, float *son, float *SIN, float *rcvl, float *rcl, float *rcr, 
								 float *CRB1, float *CRB2, float *crhl, float *crhr, float *dphum, float *doc, 
								 float *nh4, float *no3, float *no2, float *no, float *n2o, float *n2, float *nh3, 
								 float *urea, int jday, float *co2, float *sph, double water[25][180],
								 float *day_wfps, float *temp, float air_temp,
								 float *day_addmc, float *yr_addmc, float *yr_addmn, float *day_inManureN,
								 float *day_farm_to_field_N, float *day_farm_to_field_C, float *day_farm_to_field_H2O,
								 int scale, float rcnh2, float *sop, float *OrgP, float *LabP, float *ComP, float *day_farm_to_field_P,
								 int j, float *ToField)
{
	float CTF[24], ToLagoon[24], ToMarket[24], ToDigester[24], Remain[24];
	float RemovedC, RemovedN, RemovedWater, RemovedP;
	
	//removal of manure from compost, lagoon or digester to field
	if(MData.m_ToField[FN][j]>=0.0)
	{
		CTF[1] = MData.m_ToField[FN][j] * rcvl[GN];
		CTF[2] = MData.m_ToField[FN][j] * rcl[GN];
		CTF[3] = MData.m_ToField[FN][j] * rcr[GN];
		CTF[4] = MData.m_ToField[FN][j] * CRB1[GN];
		CTF[5] = MData.m_ToField[FN][j] * CRB2[GN];
		CTF[6] = MData.m_ToField[FN][j] * (crhl[GN]+crhr[GN]);
		CTF[7] = MData.m_ToField[FN][j] * dphum[GN];
		CTF[8] = MData.m_ToField[FN][j] * nh4[GN];
		CTF[9] = MData.m_ToField[FN][j] * no3[GN];
		CTF[10] = MData.m_ToField[FN][j] * no2[GN];
		CTF[11] = MData.m_ToField[FN][j] * no[GN];
		CTF[12] = MData.m_ToField[FN][j] * n2o[GN];
		CTF[13] = MData.m_ToField[FN][j] * n2[GN];
		CTF[14] = MData.m_ToField[FN][j] * nh3[GN];	
		CTF[15] = MData.m_ToField[FN][j] * urea[GN];
		CTF[16] = MData.m_ToField[FN][j] * co2[GN];
		CTF[17] = MData.m_ToField[FN][j] * doc[GN];
		CTF[18] = MData.m_ToField[FN][j] * sph[GN];
		CTF[19] = MData.m_ToField[FN][j] * water[1][GN];
		CTF[20] = MData.m_ToField[FN][j] * OrgP[GN];
		CTF[21] = MData.m_ToField[FN][j] * LabP[GN];
		CTF[22] = MData.m_ToField[FN][j] * ComP[GN];

		ToField[1] += CTF[1];
		ToField[2] += CTF[2];
		ToField[3] += CTF[3];
		ToField[4] += CTF[4];
		ToField[5] += CTF[5];
		ToField[6] += CTF[6];
		ToField[7] += CTF[7];
		ToField[8] += CTF[8];
		ToField[9] += CTF[9];
		ToField[10] += CTF[10];
		ToField[11] += CTF[11];
		ToField[12] += CTF[12];
		ToField[13] += CTF[13];
		ToField[14] += CTF[14];
		ToField[15] += CTF[15];
		ToField[16] += CTF[16];
		ToField[17] += CTF[17];
		ToField[18] += CTF[18];
		ToField[19] += CTF[19];
		ToField[20] += CTF[20];
		ToField[21] += CTF[21];
		ToField[22] += CTF[22];

		RemovedC = (CTF[1]+CTF[2]+CTF[3]+CTF[4]+CTF[5]+CTF[6]+CTF[7]+CTF[16]+CTF[17]);	
		RemovedN = (CTF[1]/rcnrvl +CTF[2]/rcnrl +CTF[3]/rcnrr+CTF[4]/rcnb +CTF[5]/rcnb +
					CTF[6]/rcnh +CTF[7]/rcnm + CTF[8]+CTF[9]+CTF[10]+CTF[11]+CTF[12]+CTF[13]+CTF[14]+CTF[15]);
		RemovedWater = CTF[19];
		RemovedP = (CTF[20]+CTF[21]+CTF[22]);

		if(GN==(q+4)) //manure revoved from compost to field
		{
			MData.Compost_to_field_C = RemovedC;
			MData.Compost_to_field_N = RemovedN;		
			MData.Compost_to_field_H2O = RemovedWater;
			MData.yr_Compost_to_field_N += MData.Compost_to_field_N;
			MData.yr_Compost_to_field_C += MData.Compost_to_field_C;
			MData.yr_Compost_to_field_H2O += CTF[19];
			MData.yr_ManureN_toField += MData.Compost_to_field_N;
			MData.Compost_to_field_P = RemovedP;
			MData.yr_Compost_to_field_P += MData.Compost_to_field_P;

			*day_farm_to_field_N += MData.Compost_to_field_N;
			*day_farm_to_field_C += MData.Compost_to_field_C;
			*day_farm_to_field_H2O += MData.Compost_to_field_H2O;
			*day_farm_to_field_P += MData.Compost_to_field_P;
		}
	
		if(GN==(q+5)) //manure revoved from lagoon to field
		{
			MData.Lagoon_to_field_C = RemovedC;
			MData.Lagoon_to_field_N = RemovedN;		
			MData.Lagoon_to_field_H2O = RemovedWater;
			MData.Lagoon_to_field_P = RemovedP;
			MData.yr_Lagoon_to_field_N += MData.Lagoon_to_field_N;
			MData.yr_Lagoon_to_field_C += MData.Lagoon_to_field_C;
			MData.yr_Lagoon_to_field_H2O += CTF[19];
			MData.yr_Lagoon_to_field_P += MData.Lagoon_to_field_P;

			MData.yr_ManureN_toField += MData.Lagoon_to_field_N;			

			*day_farm_to_field_N += MData.Lagoon_to_field_N;
			*day_farm_to_field_C += MData.Lagoon_to_field_C;
			*day_farm_to_field_H2O += MData.Lagoon_to_field_H2O;
			*day_farm_to_field_P += MData.Lagoon_to_field_P;
		}
	
		if(GN==(q+6)) //manure revoved from digester to field
		{
			MData.Digester_to_field_C = RemovedC;
			MData.Digester_to_field_N = RemovedN;		
			MData.Digester_to_field_H2O = RemovedWater;
			MData.Digester_to_field_P = RemovedP;

			MData.yr_digester_to_field_N += MData.Digester_to_field_N;
			MData.yr_digester_to_field_C += MData.Digester_to_field_C;
			MData.yr_digester_to_field_H2O += CTF[19];
			MData.yr_digester_to_field_P += MData.Digester_to_field_P;

			MData.yr_ManureN_toField += MData.Digester_to_field_N;			

			*day_farm_to_field_N += MData.Digester_to_field_N;
			*day_farm_to_field_C += MData.Digester_to_field_C;
			*day_farm_to_field_H2O += MData.Digester_to_field_H2O;
			*day_farm_to_field_P += MData.Digester_to_field_P;
		}		
	}

	if(GN==(q+6)&&MData.m_ToMarket[FN][j]>0.0)//Removal from digester to lagoon
	{
		ToLagoon[1] = MData.m_ToMarket[FN][j] * rcvl[GN];
		ToLagoon[2] = MData.m_ToMarket[FN][j] * rcl[GN];
		ToLagoon[3] = MData.m_ToMarket[FN][j] * rcr[GN];
		ToLagoon[4] = MData.m_ToMarket[FN][j] * CRB1[GN];
		ToLagoon[5] = MData.m_ToMarket[FN][j] * CRB2[GN];
		ToLagoon[6] = MData.m_ToMarket[FN][j] * (crhl[GN]+crhr[GN]);
		ToLagoon[7] = MData.m_ToMarket[FN][j] * dphum[GN];
		ToLagoon[8] = MData.m_ToMarket[FN][j] * nh4[GN];
		ToLagoon[9] = MData.m_ToMarket[FN][j] * no3[GN];
		ToLagoon[10] = MData.m_ToMarket[FN][j] * no2[GN];
		ToLagoon[11] = MData.m_ToMarket[FN][j] * no[GN];
		ToLagoon[12] = MData.m_ToMarket[FN][j] * n2o[GN];
		ToLagoon[13] = MData.m_ToMarket[FN][j] * n2[GN];
		ToLagoon[14] = MData.m_ToMarket[FN][j] * nh3[GN];	
		ToLagoon[15] = MData.m_ToMarket[FN][j] * urea[GN];
		ToLagoon[16] = MData.m_ToMarket[FN][j] * co2[GN];
		ToLagoon[17] = MData.m_ToMarket[FN][j] * doc[GN];
		ToLagoon[18] = MData.m_ToMarket[FN][j] * sph[GN];
		ToLagoon[19] = MData.m_ToMarket[FN][j] * water[1][GN];
		ToLagoon[20] = MData.m_ToMarket[FN][j] * OrgP[GN];
		ToLagoon[21] = MData.m_ToMarket[FN][j] * LabP[GN];
		ToLagoon[22] = MData.m_ToMarket[FN][j] * ComP[GN];

		/////////////////////////////////////////// to lagoon
		int LN = q+5;

		old_DigesterN = Total_SOCSON(LN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
		old_DigesterC = MData.ttc;

		rcvl[LN] += ToLagoon[1];
		rcl[LN] += ToLagoon[2];
		rcr[LN] += ToLagoon[3] ;
		CRB1[LN] += ToLagoon[4] ;
		CRB2[LN] += ToLagoon[5] ;
		crhl[LN] += 0.5*ToLagoon[6]; 
		crhr[LN] += 0.5*ToLagoon[6] ;
		dphum[LN] += ToLagoon[7] ;
		nh4[LN] += ToLagoon[8] ;
		no3[LN] += ToLagoon[9] ;
		no2[LN] += ToLagoon[10] ;
		no[LN] += ToLagoon[11] ;
		n2o[LN] += ToLagoon[12] ;
		n2[LN] += ToLagoon[13] ;
		nh3[LN] += ToLagoon[14] ;
		urea[LN] += ToLagoon[15];
		co2[LN] += ToLagoon[16] ;
		doc[LN] += ToLagoon[17];
		sph[LN] = ToLagoon[18];		
		water[1][LN] += ToLagoon[19];
		OrgP[LN] += ToLagoon[20];
		LabP[LN] += ToLagoon[21];
		ComP[LN] += ToLagoon[22];
		
		new_DigesterN = Total_SOCSON(LN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
		new_DigesterC = MData.ttc;

		MData.Digester_to_lagoon_C = (new_DigesterC - old_DigesterC);
		MData.Digester_to_lagoon_N = (new_DigesterN - old_DigesterN);
		MData.Digester_to_lagoon_H2O = ToLagoon[19];
		MData.Digester_to_lagoon_P = (ToLagoon[20]+ToLagoon[21]+ToLagoon[22]);

		MData.yr_digester_to_lagoon_C += MData.Digester_to_lagoon_C;
		MData.yr_digester_to_lagoon_N += MData.Digester_to_lagoon_N;
		MData.yr_digester_to_lagoon_H2O += MData.Digester_to_lagoon_H2O;
		MData.yr_digester_to_lagoon_P += MData.Digester_to_lagoon_P;

		/*ManureLagoon(GN, ToLagoon, rcnrvl, rcnrl,rcnrr,rcnb,rcnh,rcnm,
			rcvl,rcl,rcr,CRB1,CRB2,crhl,crhr,dphum,nh4,no3,no2,
			no,n2o,n2,nh3,urea,co2,doc,water);*/
		
	}
	
	if(GN==(q+5)&&MData.m_ToMarket[FN][j]>0.0)//Removal from lagoon to digester
	{
		ToDigester[1] = MData.m_ToMarket[FN][j] * rcvl[GN];
		ToDigester[2] = MData.m_ToMarket[FN][j] * rcl[GN];
		ToDigester[3] = MData.m_ToMarket[FN][j] * rcr[GN];
		ToDigester[4] = MData.m_ToMarket[FN][j] * CRB1[GN];
		ToDigester[5] = MData.m_ToMarket[FN][j] * CRB2[GN];
		ToDigester[6] = MData.m_ToMarket[FN][j] * (crhl[GN]+crhr[GN]);
		ToDigester[7] = MData.m_ToMarket[FN][j] * dphum[GN];
		ToDigester[8] = MData.m_ToMarket[FN][j] * nh4[GN];
		ToDigester[9] = MData.m_ToMarket[FN][j] * no3[GN];
		ToDigester[10] = MData.m_ToMarket[FN][j] * no2[GN];
		ToDigester[11] = MData.m_ToMarket[FN][j] * no[GN];
		ToDigester[12] = MData.m_ToMarket[FN][j] * n2o[GN];
		ToDigester[13] = MData.m_ToMarket[FN][j] * n2[GN];
		ToDigester[14] = MData.m_ToMarket[FN][j] * nh3[GN];	
		ToDigester[15] = MData.m_ToMarket[FN][j] * urea[GN];
		ToDigester[16] = MData.m_ToMarket[FN][j] * co2[GN];
		ToDigester[17] = MData.m_ToMarket[FN][j] * doc[GN];
		ToDigester[18] = MData.m_ToMarket[FN][j] * sph[GN];
		ToDigester[19] = MData.m_ToMarket[FN][j] * water[1][GN];
		ToDigester[20] = MData.m_ToMarket[FN][j] * OrgP[GN];
		ToDigester[21] = MData.m_ToMarket[FN][j] * LabP[GN];
		ToDigester[22] = MData.m_ToMarket[FN][j] * ComP[GN];

		int DN = q+6;

		old_LagoonN = Total_SOCSON(DN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
		old_LagoonC = MData.ttc;

		rcvl[DN] += ToDigester[1];
		rcl[DN] += ToDigester[2];
		rcr[DN] += ToDigester[3] ;
		CRB1[DN] += ToDigester[4] ;
		CRB2[DN] += ToDigester[5] ;
		crhl[DN] += 0.5*ToDigester[6]; 
		crhr[DN] += 0.5*ToDigester[6] ;
		dphum[DN] += ToDigester[7] ;
		nh4[DN] += ToDigester[8] ;
		no3[DN] += ToDigester[9] ;
		no2[DN] += ToDigester[10] ;
		no[DN] += ToDigester[11] ;
		n2o[DN] += ToDigester[12] ;
		n2[DN] += ToDigester[13] ;
		nh3[DN] += ToDigester[14] ;
		urea[DN] += ToDigester[15];
		co2[DN] += ToDigester[16] ;
		doc[DN] += ToDigester[17];
		sph[DN] = ToDigester[18];		
		water[1][DN] += ToDigester[19];
		OrgP[DN] += ToDigester[20];
		LabP[DN] += ToDigester[21];
		ComP[DN] += ToDigester[22];
		
		new_LagoonN = Total_SOCSON(DN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);
		new_LagoonC = MData.ttc;

		MData.Lagoon_to_digester_C = (new_LagoonC - old_LagoonC);
		MData.Lagoon_to_digester_N = (new_LagoonN - old_LagoonN);
		MData.Lagoon_to_digester_H2O = ToDigester[19];
		MData.Lagoon_to_digester_P = (ToDigester[20]+ToDigester[21]+ToDigester[22]);

		MData.yr_lagoon_to_digester_C += MData.Lagoon_to_digester_C;
		MData.yr_lagoon_to_digester_N += MData.Lagoon_to_digester_N;
		MData.yr_lagoon_to_digester_H2O += MData.Lagoon_to_digester_H2O;
		MData.yr_lagoon_to_digester_P += MData.Lagoon_to_digester_P;
		
	}
	
	if(GN==(q+4)&&MData.m_ToMarket[FN][j]>0.0)//Removal from compost to market
	{
		ToMarket[1] = MData.m_ToMarket[FN][j] * rcvl[GN];
		ToMarket[2] = MData.m_ToMarket[FN][j] * rcl[GN];
		ToMarket[3] = MData.m_ToMarket[FN][j] * rcr[GN];
		ToMarket[4] = MData.m_ToMarket[FN][j] * CRB1[GN];
		ToMarket[5] = MData.m_ToMarket[FN][j] * CRB2[GN];
		ToMarket[6] = MData.m_ToMarket[FN][j] * (crhl[GN]+crhr[GN]);
		ToMarket[7] = MData.m_ToMarket[FN][j] * dphum[GN];
		ToMarket[8] = MData.m_ToMarket[FN][j] * nh4[GN];
		ToMarket[9] = MData.m_ToMarket[FN][j] * no3[GN];
		ToMarket[10] = MData.m_ToMarket[FN][j] * no2[GN];
		ToMarket[11] = MData.m_ToMarket[FN][j] * no[GN];
		ToMarket[12] = MData.m_ToMarket[FN][j] * n2o[GN];
		ToMarket[13] = MData.m_ToMarket[FN][j] * n2[GN];
		ToMarket[14] = MData.m_ToMarket[FN][j] * nh3[GN];	
		ToMarket[15] = MData.m_ToMarket[FN][j] * urea[GN];
		ToMarket[16] = MData.m_ToMarket[FN][j] * co2[GN];
		ToMarket[17] = MData.m_ToMarket[FN][j] * doc[GN];
		ToMarket[18] = MData.m_ToMarket[FN][j] * sph[GN];
		ToMarket[19] = MData.m_ToMarket[FN][j] * water[1][GN];
		ToMarket[20] = MData.m_ToMarket[FN][j] * OrgP[GN];
		ToMarket[21] = MData.m_ToMarket[FN][j] * LabP[GN];
		ToMarket[22] = MData.m_ToMarket[FN][j] * ComP[GN];

		//ManureMarket(FN, GN, ToMarket, rcnrvl, rcnrl,rcnrr,rcnb,rcnh,rcnm,
		//		rcvl,rcl,rcr,CRB1,CRB2,crhl,crhr,dphum,nh4,no3,no2,
		//		no,n2o,n2,nh3,urea,co2,doc,water, rcnh2,OrgP,LabP,ComP);
	}

	//Remain in compost, lagoon or digester
	rcvl[GN] *= MData.m_Remain[FN][j];
	rcl[GN] *= MData.m_Remain[FN][j];
	rcr[GN] *= MData.m_Remain[FN][j];
	CRB1[GN] *= MData.m_Remain[FN][j];
	CRB2[GN] *= MData.m_Remain[FN][j];
	crhl[GN] *= MData.m_Remain[FN][j];
	crhr[GN] *= MData.m_Remain[FN][j];
	dphum[GN] *= MData.m_Remain[FN][j];
	nh4[GN] *= MData.m_Remain[FN][j];
	no3[GN] *= MData.m_Remain[FN][j];
	no2[GN] *= MData.m_Remain[FN][j];
	no[GN] *= MData.m_Remain[FN][j];
	n2o[GN] *= MData.m_Remain[FN][j];
	n2[GN] *= MData.m_Remain[FN][j];
	nh3[GN] *= MData.m_Remain[FN][j];	
	urea[GN] *= MData.m_Remain[FN][j];
	co2[GN] *= MData.m_Remain[FN][j];
	doc[GN] *= MData.m_Remain[FN][j];
	sph[GN] = sph[GN];
	water[1][GN] *= MData.m_Remain[FN][j];
	OrgP[GN] *= MData.m_Remain[FN][j];
	LabP[GN] *= MData.m_Remain[FN][j];
	ComP[GN] *= MData.m_Remain[FN][j];

	temp[GN] = air_temp;

	/*Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm,
								 soc, son, SIN, rcvl, rcl, rcr, 
								 CRB1, CRB2, crhl, crhr, dphum, doc, 
								 nh4, no3, no2, no, n2o, n2, nh3, 
								 urea, rcnh2, co2, sop,OrgP, LabP, ComP);*/

	////////////////////////////////// allocate manure to field soil layer 1
	/*float old_N = Total_SOCSON(1, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm,
								 soc, son, SIN, rcvl, rcl, rcr, 
								 CRB1, CRB2, crhl, crhr, dphum, doc, 
								 nh4, no3, no2, no, n2o, n2, nh3, 
								 urea, rcnh2, co2, sop,OrgP, LabP, ComP);
	float old_C = MData.ttc;*/

	
	
/*	float new_N = Total_SOCSON(1, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm,
								 soc, son, SIN, rcvl, rcl, rcr, 
								 CRB1, CRB2, crhl, crhr, dphum, doc, 
								 nh4, no3, no2, no, n2o, n2, nh3, 
								 urea, rcnh2);
	float new_C = MData.ttc;

	if(scale==0)
	{
		if(GN==q+4)
		{
			MData.Compost_to_field_N = (new_N - old_N)*MData.ManureFieldArea;
			MData.Compost_to_field_C = (new_C - old_C)*MData.ManureFieldArea;
			MData.Compost_to_field_H2O = ToField[19];
			MData.yr_Compost_to_field_N += MData.Compost_to_field_N;
			MData.yr_Compost_to_field_C += MData.Compost_to_field_C;
			MData.yr_Compost_to_field_H2O += ToField[19];
			MData.yr_ManureN_toField += MData.Compost_to_field_N;

			*day_addmc += MData.Compost_to_field_C/MData.ManureFieldArea;
			*yr_addmc += MData.Compost_to_field_C/MData.ManureFieldArea;
			*yr_addmn += MData.Compost_to_field_N/MData.ManureFieldArea;
			*day_inManureN += MData.Compost_to_field_N/MData.ManureFieldArea;

			*day_farm_to_field_N += MData.Compost_to_field_N;
			*day_farm_to_field_C += MData.Compost_to_field_C;
			*day_farm_to_field_H2O += MData.Compost_to_field_H2O;

		}
		else if(GN==q+5)
		{
			MData.Lagoon_to_field_N = (new_N - old_N)*MData.ManureFieldArea;
			MData.Lagoon_to_field_C = (new_C - old_C)*MData.ManureFieldArea;
			MData.Lagoon_to_field_H2O = ToField[19];
			MData.yr_Lagoon_to_field_N += MData.Lagoon_to_field_N;
			MData.yr_Lagoon_to_field_C += MData.Lagoon_to_field_C;
			MData.yr_Lagoon_to_field_H2O += ToField[19];
			MData.yr_ManureN_toField += MData.Lagoon_to_field_N;

			*day_addmc += MData.Lagoon_to_field_C/MData.ManureFieldArea;
			*yr_addmc += MData.Lagoon_to_field_C/MData.ManureFieldArea;
			*yr_addmn += MData.Lagoon_to_field_N/MData.ManureFieldArea;
			*day_inManureN += MData.Lagoon_to_field_N/MData.ManureFieldArea;

			*day_farm_to_field_N += MData.Lagoon_to_field_N;
			*day_farm_to_field_C += MData.Lagoon_to_field_C;
			*day_farm_to_field_H2O += MData.Lagoon_to_field_H2O;
		}
		else if(GN==q+6)
		{
			MData.Digester_to_field_N = (new_N - old_N)*MData.ManureFieldArea;
			MData.Digester_to_field_C = (new_C - old_C)*MData.ManureFieldArea;
			MData.yr_digester_to_field_N += MData.Digester_to_field_N;
			MData.yr_digester_to_field_C += MData.Digester_to_field_C;
			MData.yr_digester_to_field_H2O += ToField[19];
			MData.yr_ManureN_toField += MData.Digester_to_field_N;

			*day_addmc += MData.Digester_to_field_C/MData.ManureFieldArea;
			*yr_addmc += MData.Digester_to_field_C/MData.ManureFieldArea;
			*yr_addmn += MData.Digester_to_field_N/MData.ManureFieldArea;
			*day_inManureN += MData.Digester_to_field_N/MData.ManureFieldArea;

			*day_farm_to_field_N += MData.Digester_to_field_N;
			*day_farm_to_field_C += MData.Digester_to_field_C;
			*day_farm_to_field_H2O += MData.Digester_to_field_H2O;
		}
	}*/
}

void class_manure::EntericGasProduction(int FN, float air_temp)
{
	int i, FC=FN;
	float houseT, Ftemp;

	//manure temperature
	if(MData.mm_Ventilation==0) houseT = air_temp-(air_temp-20.0)*0.05;
	else houseT = air_temp-(air_temp-20.0)*0.5;//*MData.mm_VentRate;

	Ftemp = 0.05 * houseT + 0.5;
	Ftemp = min(1.5, max(0.5, Ftemp));

	MData.DAY_enteric_n2o[FC] = 0.0;//kg N2O-N/day
	MData.DAY_enteric_ch4[FC] = 0.0;//kg CH4-C/day
	MData.DAY_enteric_nh3[FC] = 0.0;

	//for(int i=1;i<=MData.AnimalTypes[FC];i++)
	{
		MData.day_enteric_ch4[FC] = 0.0;
		//MData.day_enteric_n2o[FC] = 0.0;
		MData.day_enteric_nh3[FC] = 0.0;

		if(MData.mm_AnimalType[FC]==1)// milking cows
		{
			//Based on Mangino, Peterson and Jacobs at http://www.epa.gov/ttn/chief/conference/ei12/green/mangino.pdf

			float GE = MData.mm_FeedProtein[FC] * 0.01 * MData.mm_FeedRate[FC] * 17.0 * 0.6; //Gross energy demand by animal, kg protein/head/day -> MJ/head/day
			float Ym = 0.77;//CH4 conversion rate, a fraction of gross energy (GE) in feed converted to CH4 (percent)
			float F_FeedQuality = MData.mm_FeedC[FC] / 2.7;//impact of quality of feeding materials
			float dFeedN = MData.mm_FeedN[FC] / 0.225333;

			//Ellis et al. model CH4 (kg CH4/cow/day) = [3.23 + 0.809 * DMI (kg/day)]/55.6). 
			MData.day_enteric_ch4[FC] = GE * Ym / 55.65 * 12.0 / 16.0 * MData.mm_AnimalHeads[FC];// * powf(Ftemp, 0.8); //daily enteric CH4 production, kg C/day per farm

			//Based on Frank's experimets reported in August 2007
			//Enteric N2O: 2.5 lb N2O/cow/yr -> 0.001981 kg N/cow/day, 0.723 kg N/cow/yr
			//Enteric CH4: 300 lb CH4/cow/yr -> 0.3859 kg C/cow/day, 140 kg C/cow/yr
			//day_enteric_ch4 = 0.3859 * F_FeedQuality * mm_AnimalHeads; //kg C/day

			//MData.day_enteric_n2o[FC] = 0.5 * 0.001981 * dFeedN * MData.mm_AnimalHeads[FC]; //kg N/day

			//MData.day_enteric_nh3[FC] = 0.0 * dFeedN * MData.mm_AnimalHeads[FC]; //kg N/day

			//*day_enteric_ch4 = (50.0 + 0.01 * day_milk) / 365.0;// * mm_AnimalHeads; //kg CH4/day, for dairy cows
			//*day_enteric_ch4 = 65.0 / 365.0 * mm_AnimalHeads; //kg CH4/day, for heifers
			//*day_enteric_ch4 = 25.0 / 365.0 * mm_AnimalHeads; //kg CH4/day, for calves
		}
		else if(MData.mm_AnimalType[FC]==2||MData.mm_AnimalType[FC]==3)//beef or veal
		{
			//Based on Mangino, Peterson and Jacobs at http://www.epa.gov/ttn/chief/conference/ei12/green/mangino.pdf

			float GE = MData.mm_FeedProtein[FC] * 0.01 * MData.mm_FeedRate[FC] * 17.0 * 0.6; //Gross energy demand by animal, kg protein/head/day -> MJ/head/day
			float Ym = 0.4;//CH4 conversion rate, a fraction of gross energy (GE) in feed converted to CH4 (percent)
			float F_FeedQuality = MData.mm_FeedC[FC] / 2.7;//impact of quality of feeding materials

			MData.day_enteric_ch4[FC] = GE * Ym / 55.65 * 12.0 / 16.0 * MData.mm_AnimalHeads[FC];// * Ftemp; //daily enteric CH4 production, kg C/day per farm

			//MData.day_enteric_n2o[FC] = 1.7 * 0.001918 * MData.mm_AnimalHeads[FC]; //kg N/day

			//MData.day_enteric_nh3[FC] = 0.0 * MData.mm_AnimalHeads[FC]; //kg N/day
		}
		else if(MData.mm_AnimalType[FC]==4)//pig
		{
			//based on RFF report: at http://www.epa.gov/ttn/chief/ap42/ch14/final/c14s04.pdf
			MData.day_enteric_ch4[FC] = MData.mm_AnimalHeads[FC] * 1.5 / 365.0 * 12.0 / 16.0; //daily enteric CH4 production, kg C/day per farm

			//MData.day_enteric_n2o[FC] = 0.0 * MData.mm_AnimalHeads[FC]; //kg N/day

			MData.day_enteric_nh3[FC] = 0.0 * MData.mm_AnimalHeads[FC]; //kg N/day
		}
		else if(MData.mm_AnimalType[FC]==5)//sheep
		{
			MData.day_enteric_ch4[FC] = MData.mm_AnimalHeads[FC] * 8.0 / 365.0 * 12.0 / 16.0; //daily enteric CH4 production, kg C/day per farm

			//MData.day_enteric_n2o[FC] = 0.0 * MData.mm_AnimalHeads[FC]; //kg N/day

			MData.day_enteric_nh3[FC] = 0.0 * MData.mm_AnimalHeads[FC]; //kg N/day
		}
		else if(MData.mm_AnimalType[FC]==6)//poultry
		{
			MData.day_enteric_ch4[FC] = 0.0; //daily enteric CH4 production, kg C/day per farm

			//MData.day_enteric_n2o[FC] = 0.0 * MData.mm_AnimalHeads[FC]; //kg N/day

			MData.day_enteric_nh3[FC] = 0.0 * MData.mm_AnimalHeads[FC]; //kg N/day
		}
		else 
		{
			MData.day_enteric_ch4[FC] = 0.0; //daily enteric CH4 production, kg C/day per farm

			//MData.day_enteric_n2o[FC] = 0.0 * MData.mm_AnimalHeads[FC]; //kg N/day

			MData.day_enteric_nh3[FC] = 0.0 * MData.mm_AnimalHeads[FC]; //kg N/day
		}
	}//animal type loop

	MData.DAY_enteric_n2o[FC] += MData.day_enteric_n2o[FC];//kg N2O-N/day
	MData.DAY_enteric_ch4[FC] += MData.day_enteric_ch4[FC];//kg CH4-C/day
	MData.DAY_enteric_nh3[FC] += MData.day_enteric_nh3[FC];//kg NH3-N/day

	MData.animal_co2 -= MData.DAY_enteric_ch4[FC];

	MData.yr_enteric_ch4 += MData.DAY_enteric_ch4[FC];
	MData.yr_enteric_n2o += MData.DAY_enteric_n2o[FC];
	MData.yr_enteric_nh3 += MData.DAY_enteric_nh3[FC];

	MData.yr_enteric_co2 -= MData.DAY_enteric_ch4[FC];
	
}


float class_manure::Total_SOCSON(int FC, int FCC, float rcnrvl, float rcnrl, float rcnrr, float rcnb, float rcnh, float rcnm,
								 float *soc, float *son, float *SIN, float *rcvl, float *rcl, float *rcr, 
								 float *CRB1, float *CRB2, float *crhl, float *crhr, float *dphum, float *doc, 
								 float *nh4, float *no3, float *no2, float *no, float *n2o, float *n2, float *nh3, 
								 float *urea, float rcnh2, float *co2, float *sop, float *OrgP, float *LabP, float *ComP)
{
	float ttn;

	soc[FC] = rcvl[FC] + rcl[FC] + rcr[FC] + dphum[FC] + CRB1[FC] + CRB2[FC] + crhl[FC] + crhr[FC] + dphum[FC] + co2[FC] + doc[FC];
	son[FC] = rcvl[FC]/rcnrvl + rcl[FC]/rcnrl + rcr[FC]/rcnrr + CRB1[FC]/rcnb + CRB2[FC]/rcnb + crhl[FC]/rcnh + crhr[FC]/rcnh2 + dphum[FC]/rcnm;
	SIN[FC] = nh4[FC] + no3[FC] + no2[FC] + no[FC] + n2o[FC] + n2[FC] + nh3[FC] + urea[FC];
	sop[FC] = OrgP[FC] + LabP[FC] + ComP[FC];

	MData.ttc = soc[FC];
	ttn = son[FC] + SIN[FC];
	MData.ttp = sop[FC];

	return(ttn);
}

#endif