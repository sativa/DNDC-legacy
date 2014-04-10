#include "stdafx.h"
#include "Dndcgo.h"
#include "Source_main.h"
#include "Dndc_tool.h"

#ifdef MANURE 
extern class_manure MData;
extern class_model *DNDC;

float conc_uear, conc_nh4, conc_no3, conc_doc, lagoon_windspeed;
static float ManurePH[7]={7.0, 7.0, 7.0, 7.0, 8.1, 7.5, 7.0}; 
//for dairy cattle, beef, veal, pig, sheep, foultry

void class_manure::lagoon_BGC(int FN, int GN, 
		int q, int jday, float rcnrvl, float rcnrl, float rcnrr, float rcnh, float rcnb, float *urea, float *doc,
		float *rcvl, float *rcl, float *rcr, float *CRB1, float *CRB2, float *dphum, float *temp, float air_temp, 
		float DayPET, float wind_speed, float *day_wfps, float *soc, float *sph, int *flood_flag, int WTL, float day_WT, 
		float *soil_ph, float *nh4, float *ch4_prod, float *ch4_day_oxid, float *plant_day_ch4, float rcnm, float *son, float *SIN, float *no3, float *no2, 
		float *no, float *n2o, float *n2, float *nh3, float *ch4, float TTT, float *newh, float *w_no2, 
		float *w_nh3, 
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
		float *EndSON, 
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
		float *R_Wt, float *vascularity, float *Odoc, float *winertC, float *inert_C, int fert_flag, 
		float *root_doc, float *day_O2, float RFM, float *SoilDepth, float rcnh2,
		float *wiltpt, float *day_frostDOC, float *yr_frostDOC, float precipitation, int manuFlag, int irri_flag, 
		float *FreezeDOC, float *Eh1, float *sop, float *OrgP, float *LabP, float *ComP, float *AdsP,
		float LateralInflux, int drain_flag, float day_leach_water, int facility, float LagoonCapacity,
		float disturbance)
{
	h[GN] = h[1];

	lagoon_climate(GN, temp, air_temp, day_wfps, Eh, wind_speed, DayPET, water, jday, rainfall);
	
	if(water[1][GN]>0.0)
	{
		DNDC->dndc_decomposition(CRB1, CRB2, newh, w_no2, 
			w_nh3, w_no3, w_nh4, wurea, wpool_no, 
			1.0+disturbance, DayFlux, tilq, 
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
			plantn, clay_nh4, base_clay_N, max_clay_N,
			wrcvl, wrcl, wrcr, wcrb, wcrhl, wcrhr, whumus, wtcavai,
			day_ice_N, day_ice_doc, day_ice_n2o, day_ice_n2, IniSOC, IniSON, EndSON,
			day_runoff, day_runoff_N, day_clay_N,
			dsno3, dsnh4, dsnh3, day_dsurea, ds_doc, crop, yr_fixn, rainfall, soil_ph, Odoc,
			winertC, inert_C, fert_flag, root_doc, day_O2, RFM, SoilDepth, rcnh2, ttoo, 
			sop, OrgP, LabP, AdsP, ComP, sts, day_leach_water);

		lagoon_nh3(GN, temp, nh3, water, nh4, sph, LagoonCapacity, disturbance);

		lagoon_denitrification(GN, temp, nh4, no3, soc, doc,no2,no,n2o,n2, dphum);
		
/*		DNDC->N2O_balloon(GN, till_fact, sts, h, jday, *flood_flag, FloDays, rainfall,
			O2_old, O2, TTT, vvv, ave_anvf, q, st, temp, year, water, day_wfps, day_WT, 
			WTL, total_ice, day_rco2, co2, flo_num, ice, sph, Eh, anvf,
			nh4, no3, no2, no, n2, doc, n2o, doc_w, nh4_w, no3_w, no2_w, no_w, n2o_w, n2_w,
			doc_d, nh4_d, no3_d, no2_d, no_d, n2o_d, LandUse, ttoo, 
			ManureAddCN, ManureDays, soc, MicrobioIndex, mRBO, mFD, WaterControl, 
			crop, assim2, clay, fldcap, by_passf, NIDay1, NIDay2,
			day_soil_nitrify, day_nitrify_NO, day_nitrify_N2O, day_soil_an_no,
			day_soil_an_n2o, day_soil_n2, Root_Wt, WaterLeakRate,
			rcvl, rcl, rcr, day_soil_denitrification,
			wiltpt, CRB1, CRB2, rcnb, Odoc, day_frostDOC, yr_frostDOC, precipitation, 
			manuFlag, irri_flag, FreezeDOC, nh3, Eh1, urea, LateralInflux, drain_flag,
			0);
*/		
		lagoon_ch4(GN, temp, dphum, doc, disturbance, jday);

		//Crust formation
		Total_SOCSON(GN, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
				 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);

		float ManureConc = soc[GN] / water[1][GN];
		float EV_m = MData.yr_lagoon_EV / (1000.0 * MData.m_LagoonSurfaceArea); 
		if(ManureConc>0.015 && EV_m>0.12)//0.03
		{
			MData.LagoonCrustC += 0.0015 * soc[GN];  
		}
		
	}
	else
	{
		MData.day_lagoon_ch4 = 0.0;
		MData.day_lagoon_nh3 = 0.0;
		MData.day_lagoon_n2o = 0.0;
		MData.day_lagoon_no = 0.0;
		MData.day_lagoon_n2 = 0.0;
		MData.LagoonCrustC = 0.0;
	}

}

void class_manure::lagoon_climate(int GN, float *temp, float air_temp, float *day_wfps, float *Eh, float wind_speed,
								  float DayPET, double water[25][180], int jday, float rainfall)
{
	float mm_DayPET, LInRain=0.0;

	//lagoon temperature
	if(MData.lagoon_days==1) 
	{
		MData.LagoonT[1] = temp[GN];	
		MData.LagoonT[2] = temp[GN];
		MData.LagoonT[3] = temp[GN];
		MData.LagoonT[4] = temp[GN];
		MData.LagoonT[5] = temp[GN];
		MData.bottomT = temp[GN];
	}
	else
	{
		float ddT = 0.003 * (temp[GN] - MData.bottomT);

		MData.LagoonT[1] = temp[GN];

		//if(MData.LagoonT[2]<MData.LagoonT[1]) 
		MData.LagoonT[2] += 7.0 * ddT;
		//else  MData.LagoonT[2] -= 20.0 * ddT;

		//if(MData.LagoonT[3]<MData.LagoonT[2]) 
		MData.LagoonT[3] += 3.0 * ddT;
		//else  MData.LagoonT[3] -= 10.0 * ddT;

		//if(MData.LagoonT[4]<MData.LagoonT[3]) 
		MData.LagoonT[4] += 2.0 * ddT;
		//else  MData.LagoonT[4] -= 5.0 * ddT;

		//if(MData.LagoonT[5]<MData.LagoonT[4]) 
		MData.LagoonT[5] += 1.0 * ddT;
		//else  MData.LagoonT[5] -= 1.0 * ddT;
		
		//MData.LagoonT[3] += 3.0 * ddT;
		//MData.LagoonT[4] += 2.0 * ddT;
		//MData.LagoonT[5] += 1.0 * ddT;
	}

	temp[GN] = MData.LagoonT[2];
	//bottomT = (MData.LagoonT[1]+MData.LagoonT[2]+MData.LagoonT[3]+MData.LagoonT[4]+MData.LagoonT[5])/5.0;
	//temp[GN] = bottomT;
	//T(d) = Ta * (1 - Fd) + Tb * Fd;
	
/*	MData.LagoonT[5]=MData.LagoonT[4];
	MData.LagoonT[4]=MData.LagoonT[3];
	MData.LagoonT[3]=MData.LagoonT[2];
	MData.LagoonT[2]=MData.LagoonT[1];
	MData.LagoonT[1]=air_temp;*/	
	
	//DayPET = potential_et_manure();

	day_wfps[GN] = 1.0;
	Eh[GN] = -250.0;

	if(MData.m_LagoonCover==1)//air open
		lagoon_windspeed = wind_speed;
	else
		lagoon_windspeed = 0.0;

	mm_DayPET = DayPET * (1.0 + wind_speed * 0.01);//m water/day

	if(MData.m_LagoonCover==1)//air open 
		MData.day_lagoon_EV = mm_DayPET * MData.m_LagoonSurfaceArea * 1000.0;//m3 -> kg water// * 0.09 
	else if(MData.m_LagoonCover==2)//sheltered 
		MData.day_lagoon_EV = 0.4 * mm_DayPET * MData.m_LagoonSurfaceArea * 1000.0;
	else //covered
		MData.day_lagoon_EV = 0.1 * mm_DayPET * MData.m_LagoonSurfaceArea * 1000.0;

	if(water[1][GN] <= MData.day_lagoon_EV) 
	{
		MData.day_lagoon_EV = water[1][GN];
		water[1][GN]=0.0;
	}
	else
	{
		water[1][GN] -= MData.day_lagoon_EV;
	}

	MData.yr_lagoon_EV += MData.day_lagoon_EV;

	if(MData.m_LagoonCover==1)//air open
	{
		LInRain = rainfall * MData.m_LagoonSurfaceArea * 1000.0; //kg water

		water[1][GN] += LInRain;

		MData.yr_lagoon_InRain += LInRain;
	}

	if(water[1][GN] > MData.m_LagoonCapacity * 1000.0)
	{
#ifdef LINUX		
		char EMSS[200];
		sprintf(EMSS, "Lagoon is overflowed");
		MData.EMDD++;
		ErrorMessage(EMSS, MData.EMDD);
#else
		CString note;
		note.Format("Lagoon is overflowed!");
		AfxMessageBox(note);
#endif
		exit(0);
	}	
}

void class_manure::lagoon_nh3(int GN, float *temp, float *nh3, double water[25][180], float *nh4, float *sph,
							  float LagoonCapacity, float disturbance)
{
	//Chemical equilibrium: NH4+ + OH- = NH3 + H2O
	double wd1, wd2, Kw, Ka, hydrogen, hydroxide, V_water, mol_nh4, mol_nh3;//, old_nh3=nh3[GN]; 

	wd1 = (double)pow(10.0, -15.0);
	wd2 = (double)pow(10.0, -5.0);
	Kw = 1.945 * exp(0.0645 * temp[GN]) * wd1;//water dissociation constant
	Ka = (1.416 + 0.01357 * temp[GN]) * wd2;//NH4+/NH3 equilibrium constant
	hydrogen = (double)pow(10.0, -sph[GN]);//mol/L
	hydroxide = Kw / hydrogen;//mol/L

	//V_water = LagoonCapacity * 1000.0;//liter water

	float EmiF=0.0;
	if(MData.m_LagoonCover==1) EmiF = 0.8;
	else if(MData.m_LagoonCover==2) EmiF = 0.7;
	else if(MData.m_LagoonCover==3) EmiF = 0.05;
	else {}
	
	mol_nh4 = nh4[GN] / 14.0;// / V_water;//kg N -> mol/L
	mol_nh3 = mol_nh4 * EmiF;//hydroxide / Ka;//mol/L

	if((mol_nh4 + mol_nh3)<=0.0)
	{
		nh4[GN] = 0.0;
		nh3[GN] = 0.0;
	}
	else
	{
		float cvf = mol_nh4 / (mol_nh4 + mol_nh3); 
		float TtN = nh4[GN]+nh3[GN];
				
		//if(flood_flag==1) cvf *= 0.2;
		nh4[GN] = TtN * cvf; 
		nh3[GN] = TtN - nh4[GN];//kg N/ha in layer l		
	}

	if(disturbance>0.0)
	{
		if(MData.m_LagoonCover==3)
			MData.day_lagoon_nh3 = (0.001 + 0.15 * disturbance) * nh3[GN] ;
		else
			MData.day_lagoon_nh3 = (0.001 + 0.02 * disturbance) * nh3[GN] ;
	}
	else
		MData.day_lagoon_nh3 = 0.1 * nh3[GN];

	nh3[GN] -= MData.day_lagoon_nh3;

/*	//Modified Two-film model by Visscher et al. 2002
	float FLUX_AItoBA_NH3;

	float Air_nh3 = 0.0;//1.0;//background NH3 concentration in the air (ug N/m3)
	float Tk = temp[GN] + 273.16; //Kelvins teperature, degree K
	float KH = 239000.0 / Tk * (float)exp(-4151.0/Tk);//Henry's law constant
	float u8 = lagoon_windspeed; //u8 is wind speed (m/sec) at 8 meter height
	float Patm = 760.0; //atmospheric pressure (mmHg) or 101.325 kPa
	
	float Mair = 28.97; //molecular weight of air
	float Mnh3 = 17.0; //molecular weight of NH3
	float Mh2o = 18.0; //molecular weight of H2O
	float ADVair = 20.1; //atomic diffusion volume (cm3/mol) of air
	float ADVnh3 = 14.9; //atomic diffusion volume (cm3/mol) of NH3
	float ADVh2o = 12.7; //atomic diffusion volume (cm3/mol) of water

	float KG_h2o = 18.568 + 703.61 * u8 / 360000.0;//H2O mass tranfer coefficient (m/sec) in the air boundary layer

	float ww1 = (float)pow(10,-7) * pow(Tk, 1.75) * pow((Mair+Mnh3)/(Mair*Mnh3), 0.5);
	float ww2 = Patm * pow(pow(ADVair, 0.3333)+pow(ADVnh3, 0.3333), 2.0);
	float DA_nh3 = ww1 / ww2; //diffusion coefficient (m2/sec) of NH3 in the air phase
	float uu1 = (float)pow(10,-7) * pow(Tk, 1.75) * pow((Mair+Mh2o)/(Mair*Mh2o), 0.5);
	float uu2 = Patm * pow(pow(ADVair, 0.3333)+pow(ADVh2o, 0.3333), 2.0);
	float DA_h2o = uu1 / uu2;//diffusion coefficient (m2/sec) of H2O in the air phase	
	float DW_nh3 = 6.14526 * (float)pow(10.0, -15.0) * Tk / (float)exp(1622.0/Tk - 12.40518);//diffusion coefficient (m2/sec) of NH3 in the liquid phase
	float DW_o2 = 7.28236 * (float)pow(10.0, -15.0) * Tk / (float)exp(1622.0/Tk - 12.40518);////diffusion coefficient (m2/sec) of O2 in the liquid phase
	float KL_o2 = 0.6034 * (float)exp(0.2361*u8) / 360000.0;//KL_o2 is O2 mass tranfer coefficient (m/sec) in the liquid boundary layer
	float KL_nh3 = KL_o2 * (float)pow((DW_nh3/DW_o2), 0.57);//KL_nh3 is NH3 mass tranfer coefficient (m/sec) in the liquid boundary layer
	float KG_nh3 = 0.2;//KG_h2o * (float)pow((DA_nh3/DA_h2o), 0.67);//KG_nh3 is NH3 mass tranfer coefficient (m/sec) in the air boundary layer
	
	float BL_NH3 = (nh3[GN] * 1000000000.0) / (water[1][GN]/1000.0);//ug N/m3

	float FLUX_NH3_LiquidInterface = 2.5 * KL_nh3 * (BL_NH3 - MData.LI_NH3);//NH3 transferred from bulk water to the liquid interface, ug N/m2/second

	BL_NH3 -= FLUX_NH3_LiquidInterface;//ug N/m3
	MData.LI_NH3 += FLUX_NH3_LiquidInterface;//ug N/m3

	float FLUX_NH3_GasInterface = KL_nh3 * (MData.LI_NH3 - MData.AI_NH3);//NH3 transferred from the liquid interface to the air interface, ug N/m2/second

	MData.LI_NH3 -= FLUX_NH3_GasInterface;//ug N/m3
	MData.AI_NH3 += FLUX_NH3_GasInterface;//ug N/m3

	Air_nh3 *= 1.0 / (lagoon_windspeed + 1.0);
	
	if(MData.AI_NH3 > Air_nh3) 
		FLUX_AItoBA_NH3 = KG_nh3 * (MData.AI_NH3 - Air_nh3);//NH3 transferred from the air interface to the bulk air, ug N/m2/second
	else
		FLUX_AItoBA_NH3 = 0.0;

	MData.AI_NH3 -= FLUX_AItoBA_NH3;

	MData.day_lagoon_nh3 = 3.0 * FLUX_AItoBA_NH3 * MData.m_LagoonSurfaceArea * 86400.0 / 1000000000.0; //0.7//kg NH3-N/day from the lagoon surface
	
	if(MData.m_LagoonCover>1)
	{
		float retain_nh3 = MData.day_lagoon_nh3 * (0.5 * (float)MData.m_LagoonCover - 1.2);

		MData.day_lagoon_nh3 -= retain_nh3;

		Air_nh3 += retain_nh3;
	}

	nh3[GN] -= MData.day_lagoon_nh3;*/
}

void class_manure::lagoon_denitrification(int GN, float *temp, float *nh4, float *no3, float *soc, float *doc,
										  float *no2,float *no,float *n2o,float *n2, float *dphum)
{
	float fact_t, fact_m, micro_growth, micro_death, micro_delta, wwzz, k, nno, ae_no, ae_n2o;
	float nitrifier, denitrifier;

	//nitrification
	nitrifier = soc[GN] * RBO * FD;// * Fmn;

	fact_t = (float)pow(3.503,(60.0-temp[GN])/(60.0-34.22))* (float)exp(3.503*(temp[GN]-34.22)/(60.0-34.22)); //O'neill
    if ( fact_t > 1.0 ) fact_t = 1.0;
    if ( fact_t < 0.0 ) fact_t = 0.0;

    fact_m = 0.1;
    
    // developed by Florian based on Blagodatsky and Richter 1998
    micro_growth = 0.01 * (doc[GN] / (1.0 + doc[GN]) + fact_m / (1.0 + fact_m)) / 24.0;
    if (micro_growth < 0.0) micro_growth = 0.0;

    micro_death = 0.01 * (nitrifier) * 1.0 / (1.0 + doc[GN]) / (1.0 + fact_m) / 24.0;

    micro_delta = (micro_growth - micro_death) * nitrifier * fact_t * fact_m;

    nitrifier += micro_delta;
    if (nitrifier<0.0) nitrifier = 0.0;

	k = 0.01;

	/////////////////////////////////////////////////////////////////////////////
	float Km, Ft, Vs, Bo, Tt, Tref, dE, Rr, Ut; 

	Km = 0.13; //conversion factor at 15 degree C

	//dE = 122000.0; //enthalpy of CH4 formation, J/mol
	//Rr = 8.31; //gas constant, J/mol/K   
	Tt = 273.15 + MData.LagoonT[1];//temp[GN];//absolute temperature, K
	Tref = 273.15 + 15.0;// reference temperature, K, 15.0
	Ft = (float)exp((-1.0/Tt + 1.0/Tref));//effect of temperature

	if(MData.m_LagoonCover==1)
		Bo = 3.0;//open
	if(MData.m_LagoonCover==2)
		Bo = 2.5;//sheltered
	else 
		Bo = 0.3;//covered

	if(MData.lagoon_days>1)
	{
		Ut = (float)(MData.lagoon_days - 55) * 0.01;

		if(MData.lagoon_days>100) Ut *= (float)pow((100.0/(float)MData.lagoon_days),3.0);

		Ut = min(1.0, Ut);

		float STL = 0.01;
		float stl = dphum[GN]*STL;
		doc[GN] += stl;
		dphum[GN] -= stl;

		if(dphum[GN]<0.0) dphum[GN] = 0.0;
	}
	else Ut = 0.0;

	float KWK = Km * Ft * Bo * Ut;
	KWK = min(1.0, max(0.0, KWK));
	///////////////////////////////////////////////////////////////////////////////
	
	nno = KWK * nitrifier * fact_m * nh4[GN];
	
    if ( nno < 0.0 ) nno = 0.0;

	if(nh4[GN]>=nno) nh4[GN] -= nno;
	else 
	{
		nno = nh4[GN];
		nh4[GN] = 0.0;
	}

    ae_n2o = 0.001 * nno;// Ingwersen, 1998
	  
    nno -= ae_n2o;

    //day_lagoon_n2o += ae_n2o;
    //ae_n2o = 0.0;

    no3[GN] += (nno);

	//denitrification
	denitrifier = soc[GN] * RBO * FD;

	micro_growth = 0.0166 * (doc[GN] / (1.0 + doc[GN]) + fact_m / (1.0 + fact_m)) / 24.0;
    if (micro_growth < 0.0) micro_growth = 0.0;

    micro_death = 0.008 * (denitrifier) * 1.0 / (1.0 + doc[GN]) / (1.0 + fact_m) / 24.0;

    micro_delta = (micro_growth - micro_death) * denitrifier * fact_t * fact_m;

    denitrifier += micro_delta;
    if (denitrifier < 0.0) denitrifier = 0.0;

	float de_rate = 0.001 * denitrifier / (1.0 + denitrifier) * no3[GN];

	if(no3[GN] > de_rate) 
	{
		no3[GN] -= de_rate;
	}
	else
	{
		de_rate = no3[GN];
		no3[GN] = 0.0;
	}

	MData.day_lagoon_n2o = 0.001 * de_rate + n2o[GN] + no[GN] + ae_n2o;
	MData.day_lagoon_n2 = 0.999 * de_rate + n2[GN] + no2[GN];
	n2o[GN] = 0.0;
	no[GN] = 0.0;
	no2[GN] = 0.0;
	n2[GN] = 0.0;
}

void class_manure::lagoon_ch4(int GN, float *temp, float *dphum, float *doc, float disturbance, int jday)
{
	float Km, Ft, Vs, Bo, Tt, Tref, dE, Rr, Ut; 

	Km = 0.04; //conversion factor at 15 degree C

	dE = 122000.0; //enthalpy of CH4 formation, J/mol
	Rr = 8.31; //gas constant, J/mol/K   
	Tt = 273.15 + MData.LagoonT[1];//temp[GN];//absolute temperature, K
	Tref = 273.15 + 15.0;// reference temperature, K, 15.0
	Ft = (float)exp(dE/Rr * (-1.0/Tt + 1.0/Tref));//effect of temperature

	if(MData.m_LagoonCover==1)//open
		Bo = 2.0;//maximum CH4 producing capacity, kg CH4-C/kg SOC
	if(MData.m_LagoonCover==2)//sheltered
		Bo = 2.5;
	else //covered
		Bo = 3.2;//0.5 * (float)MData.m_LagoonCover;

	float STL = 0.0;
	if(MData.lagoon_days>1)
	{
		if(MData.lagoon_days<35) 
		{
			Ut = (float)(35 - MData.lagoon_days) * 0.00001;
			STL = 0.0;
		}
		else 
		{
			Ut = 0.0;
			STL = 0.01*(float)MData.lagoon_days;
		}

		Ut = min(1.0, max(0.0,Ut));
		STL = min(1.0, max(0.0,STL));
		
		float stl = dphum[GN]*Ut;//STL;
		doc[GN] += stl;
		dphum[GN] -= stl;

		if(dphum[GN]<0.0) dphum[GN] = 0.0;
	}
	else STL = 0.0;

	float KWK = 0.015 * Km * Ft * Bo * STL;// * Ut;
	KWK = min(1.0, max(0.0, KWK));

	MData.day_lagoon_ch4 = doc[GN] * KWK;

	if(MData.day_lagoon_ch4<0.0) MData.day_lagoon_ch4 = 0.0;

	doc[GN] -= MData.day_lagoon_ch4;


}


#endif


