#include "stdafx.h"
#include "Dndcgo.h"
#include "Source_main.h"
#include "Dndc_tool.h"

#ifdef MANURE
extern class_manure MData;
class_manure *MVM;


void class_model::Main_Treatment(int year, int scale, char *F_CLIMD,
								 float w_no2, float w_nh3, float w_no3, float w_nh4, float wurea, 
								 float day_clayn, float wpool_no, int tilq, FILE* fCB, FILE* fNB, 
								 float w_n2, float litco22, float litco23, float till_fact, float *ttoo,
								 float ha, int Batch, int livestock, FILE* fout2, char *PassL1, int FarmField, char *Pass)
{
	CString TotalFeedlots;
	FILE *TFL;
	FILE* fFeedOut1, *fFeedOut2, *fFeedOut3, *fFeedOut4, *fFeedOut5, *fFeedOut6, *fFeedOut7, *fFeedOut8;
	FILE *fFeedOut9, *fFeedOut10;
	FILE *fcompost=NULL, *flagoon=NULL, *fdigester=NULL, *ftreatgas=NULL, *ftf=NULL, *fday=NULL;
	int m_FeedLots, i, junkd, jday, j, FN=0, GN=0;
	int CN=q+4, LN=q+5, DN=q+6, CompostCover=0;
	char DAYFeedOut1[100], DAYFeedOut2[100], DAYFeedOut3[100], DAYFeedOut4[100], DAYFeedOut5[100];
	char DAYFeedOut6[100], DAYFeedOut7[100], DAYFeedOut8[100], DAYFeedOut9[100], DAYFeedOut10[100];
	char DAYcompost[90], DAYlagoon[90], DAYdigester[90], DAYtreatgas[90], FTF[90];
	float ToCompost[24]={0.0,0.0}, ToLagoon[24]={0.0,0.0}, ToDigester[24]={0.0,0.0}, ToField[24]={0.0,0.0};
	float LagoonCapacity=0.0;

	initiation(1);

	if(year>1) 
		read_out_ini_manure();

	TotalFeedlots.Format("%sResult\\Inputs\\TotalFeedLots_%d", ROOTDIR, year);
	TFL=fopen(TotalFeedlots, "r");
	if(TFL==NULL) note(0, TotalFeedlots);
	fscanf(TFL, "%d", &m_FeedLots);
	fclose(TFL);

	//open files of daily manure removal from feedlots to treatment facilities
	for(i=1; i<=m_FeedLots; i++)
	{
		if(i==1)
		{
			sprintf(DAYFeedOut1, "%s\\Day_FeedlotOutput_yr%d_lot%d.txt", INTER, year, 1);
				fFeedOut1 = sopen( DAYFeedOut1, "r" );
				if ( fFeedOut1 == NULL ) note(0,DAYFeedOut1);
		}
		else if(i==2)
		{
			sprintf(DAYFeedOut2, "%s\\Day_FeedlotOutput_yr%d_lot%d.txt", INTER, year, 2);
				fFeedOut2 = sopen( DAYFeedOut2, "r" );
				if ( fFeedOut2 == NULL ) note(0,DAYFeedOut2);
		}
		else if(i==3)
		{
			sprintf(DAYFeedOut3, "%s\\Day_FeedlotOutput_yr%d_lot%d.txt", INTER, year, 3);
				fFeedOut3 = sopen( DAYFeedOut3, "r" );
				if ( fFeedOut3 == NULL ) note(0,DAYFeedOut3);
		}
		else if(i==4)
		{
			sprintf(DAYFeedOut4, "%s\\Day_FeedlotOutput_yr%d_lot%d.txt", INTER, year, 4);
				fFeedOut4 = sopen( DAYFeedOut4, "r" );
				if ( fFeedOut4 == NULL ) note(0,DAYFeedOut4);
		}
		else if(i==5)
		{
			sprintf(DAYFeedOut5, "%s\\Day_FeedlotOutput_yr%d_lot%d.txt", INTER, year, 5);
				fFeedOut5 = sopen( DAYFeedOut5, "r" );
				if ( fFeedOut5 == NULL ) note(0,DAYFeedOut5);
		}
		else if(i==6)
		{
			sprintf(DAYFeedOut6, "%s\\Day_FeedlotOutput_yr%d_lot%d.txt", INTER, year, 6);
				fFeedOut6 = sopen( DAYFeedOut6, "r" );
				if ( fFeedOut6 == NULL ) note(0,DAYFeedOut6);
		}
		else if(i==7)
		{
			sprintf(DAYFeedOut7, "%s\\Day_FeedlotOutput_yr%d_lot%d.txt", INTER, year, 7);
				fFeedOut7 = sopen( DAYFeedOut7, "r" );
				if ( fFeedOut7 == NULL ) note(0,DAYFeedOut7);
		}
		else if(i==8)
		{
			sprintf(DAYFeedOut8, "%s\\Day_FeedlotOutput_yr%d_lot%d.txt", INTER, year, 8);
				fFeedOut8 = sopen( DAYFeedOut8, "r" );
				if ( fFeedOut8 == NULL ) note(0,DAYFeedOut8);
		}
		else if(i==9)
		{
			sprintf(DAYFeedOut9, "%s\\Day_FeedlotOutput_yr%d_lot%d.txt", INTER, year, 9);
				fFeedOut9 = sopen( DAYFeedOut9, "r" );
				if ( fFeedOut9 == NULL ) note(0,DAYFeedOut9);
		}
		else if(i==10)
		{
			sprintf(DAYFeedOut10, "%s\\Day_FeedlotOutput_yr%d_lot%d.txt", INTER, year, 10);
				fFeedOut10 = sopen( DAYFeedOut10, "r" );
				if ( fFeedOut10 == NULL ) note(0,DAYFeedOut10);
		}
	}

	//open files for recording pools/fluxes of treatment facilities
	if (scale == 0)
	{
		if(Batch==0)
		{
			sprintf(DAYcompost, "%s\\Day_Compost_%d.%s", Pass, year, CSV);
			fcompost = sopen( DAYcompost, "w" );
			if ( fcompost == NULL ) note(1,DAYcompost);
			
			sprintf(DAYlagoon, "%s\\Day_Lagoon_%d.%s", Pass, year, CSV);
			flagoon = sopen( DAYlagoon, "w" );
			if ( flagoon == NULL ) note(1,DAYlagoon);
			
			sprintf(DAYdigester, "%s\\Day_Digester_%d.%s", Pass, year, CSV);
			fdigester = sopen( DAYdigester, "w" );
			if ( fdigester == NULL ) note(1,DAYdigester);

			sprintf(DAYtreatgas, "%s\\Day_TreatmentGases_%d.%s", Pass, year, CSV);
			ftreatgas = sopen( DAYtreatgas, "w" );
			if ( ftreatgas == NULL ) note(1,DAYtreatgas);

			sprintf(FTF, "%s\\FarmManureToField_%d", INTER, year);
			ftf=fopen(FTF, "w");
			if(ftf==NULL) note(1, FTF);
		}
		else
		{
			sprintf(DAYcompost, "%s\Day_Compost_%d.%s", PassL1, year, CSV);
			fcompost = sopen( DAYcompost, "w" );
			if ( fcompost == NULL ) note(1,DAYcompost);
			
			sprintf(DAYlagoon, "%s\\Day_Lagoon_%d.%s", PassL1, year, CSV);
			flagoon = sopen( DAYlagoon, "w" );
			if ( flagoon == NULL ) note(1,DAYlagoon);
			
			sprintf(DAYdigester, "%s\\Day_Digester_%d.%s", PassL1, year, CSV);
			fdigester = sopen( DAYdigester, "w" );
			if ( fdigester == NULL ) note(1,DAYdigester);

			sprintf(DAYtreatgas, "%s\\Day_TreatmentGases_%d.%s", PassL1, year, CSV);
			ftreatgas = sopen( DAYtreatgas, "w" );
			if ( ftreatgas == NULL ) note(1,DAYtreatgas);

			sprintf(FTF, "%s\\FarmManureToField_%d", INTER, year);
			ftf=fopen(FTF, "w");
			if(ftf==NULL) note(1, FTF);
		}

	}

	//open climate file
	fday = fopen(F_CLIMD, "r");
	if (fday == NULL) note(0, F_CLIMD);

	//start daily loop
	for(jday=1; jday<=365; jday++)
	{
		day_zero();

		fscanf(fday, "%d %f %f %f %f %f %f %f %f", &junkd, &air_temp, &max_temp, &min_temp, &precipitation, &PAR, &Day_Hrs, &wind_speed, &humidity);

		if(junkd!=jday) 
		{
#ifdef DEGUI		
		char EMSS[200];
		sprintf(EMSS, "Climate data error on day %d", jday);
		MData.EMDD++;
		ErrorMessage(EMSS, MData.EMDD);
#else
			CString notes;
			notes.Format("Climate data error on day %d", jday);
			AfxMessageBox(notes);
#endif
			exit(0);
		}

		if(jday==1)
		{
			MData.LagoonT[1]=air_temp;
			MData.LagoonT[2]=air_temp;
			MData.LagoonT[3]=air_temp;
			MData.LagoonT[4]=air_temp;
			MData.LagoonT[5]=air_temp;
		}

#ifdef HYDRO
		if(scale==1)
		fscanf(fday, "%f", &MaxHalfHR);
#endif
		precipitation *= 0.01; //cm -> m
		rainfall = precipitation;

		DayPET = potential_et_manure();

		for(i=1; i<=m_FeedLots; i++)
		{
			if(i==1)
			{
				fscanf(fFeedOut1, "%d", &junkd);
				if(junkd!=jday) 
				{
#ifdef DEGUI		
		char EMSS[200];
		sprintf(EMSS, "1: Feedlot output data error on day %d", jday);
		MData.EMDD++;
		ErrorMessage(EMSS, MData.EMDD);
#else
					CString notes;
					notes.Format("1: Feedlot output data error on day %d", jday);
					AfxMessageBox(notes);
#endif
					exit(0);
				}

				for(j=1; j<=22; j++) fscanf(fFeedOut1, "%f", &ToCompost[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut1, "%f", &ToLagoon[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut1, "%f", &ToDigester[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut1, "%f", &ToField[j]);
			}

			///////////////////
			else if(i==2)
			{
				fscanf(fFeedOut2, "%d", &junkd);
				if(junkd!=jday) 
				{
					CString notes;
					notes.Format("2: Feedlot output data error on day %d", jday);
					AfxMessageBox(notes);
					exit(0);
				}

				for(j=1; j<=22; j++) fscanf(fFeedOut2, "%f", &ToCompost[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut2, "%f", &ToLagoon[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut2, "%f", &ToDigester[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut2, "%f", &ToField[j]);
			}
			//////////////////
			else if(i==3)
			{
				fscanf(fFeedOut3, "%d", &junkd);
				if(junkd!=jday) 
				{
					CString notes;
					notes.Format("3: Feedlot output data error on day %d", jday);
					AfxMessageBox(notes);
					exit(0);
				}

				for(j=1; j<=22; j++) fscanf(fFeedOut3, "%f", &ToCompost[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut3, "%f", &ToLagoon[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut3, "%f", &ToDigester[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut3, "%f", &ToField[j]);
			}
			//////////////////////////
			else if(i==4)
			{
				fscanf(fFeedOut4, "%d", &junkd);
				if(junkd!=jday) 
				{
					CString notes;
					notes.Format("4: Feedlot output data error on day %d", jday);
					AfxMessageBox(notes);
					exit(0);
				}

				for(j=1; j<=22; j++) fscanf(fFeedOut4, "%f", &ToCompost[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut4, "%f", &ToLagoon[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut4, "%f", &ToDigester[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut4, "%f", &ToField[j]);
			}
			//////////////////////////
			else if(i==5)
			{
				fscanf(fFeedOut5, "%d", &junkd);
				if(junkd!=jday) 
				{
					CString notes;
					notes.Format("5: Feedlot output data error on day %d", jday);
					AfxMessageBox(notes);
					exit(0);
				}

				for(j=1; j<=22; j++) fscanf(fFeedOut5, "%f", &ToCompost[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut5, "%f", &ToLagoon[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut5, "%f", &ToDigester[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut5, "%f", &ToField[j]);
			}
			//////////////////////////
			else if(i==6)
			{
				fscanf(fFeedOut6, "%d", &junkd);
				if(junkd!=jday) 
				{
					CString notes;
					notes.Format("6: Feedlot output data error on day %d", jday);
					AfxMessageBox(notes);
					exit(0);
				}

				for(j=1; j<=22; j++) fscanf(fFeedOut6, "%f", &ToCompost[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut6, "%f", &ToLagoon[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut6, "%f", &ToDigester[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut6, "%f", &ToField[j]);
			}
			//////////////////////////
			else if(i==7)
			{
				fscanf(fFeedOut7, "%d", &junkd);
				if(junkd!=jday) 
				{
					CString notes;
					notes.Format("7: Feedlot output data error on day %d", jday);
					AfxMessageBox(notes);
					exit(0);
				}

				for(j=1; j<=22; j++) fscanf(fFeedOut7, "%f", &ToCompost[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut7, "%f", &ToLagoon[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut7, "%f", &ToDigester[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut7, "%f", &ToField[j]);
			}
			//////////////////////////
			else if(i==8)
			{
				fscanf(fFeedOut8, "%d", &junkd);
				if(junkd!=jday) 
				{
					CString notes;
					notes.Format("8: Feedlot output data error on day %d", jday);
					AfxMessageBox(notes);
					exit(0);
				}

				for(j=1; j<=22; j++) fscanf(fFeedOut8, "%f", &ToCompost[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut8, "%f", &ToLagoon[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut8, "%f", &ToDigester[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut8, "%f", &ToField[j]);
			}
			//////////////////////////
			else if(i==9)
			{
				fscanf(fFeedOut9, "%d", &junkd);
				if(junkd!=jday) 
				{
					CString notes;
					notes.Format("9: Feedlot output data error on day %d", jday);
					AfxMessageBox(notes);
					exit(0);
				}

				for(j=1; j<=22; j++) fscanf(fFeedOut9, "%f", &ToCompost[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut9, "%f", &ToLagoon[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut9, "%f", &ToDigester[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut9, "%f", &ToField[j]);
			}
			//////////////////////////
			else if(i==10)
			{
				fscanf(fFeedOut10, "%d", &junkd);
				if(junkd!=jday) 
				{
					CString notes;
					notes.Format("10: Feedlot output data error on day %d", jday);
					AfxMessageBox(notes);
					exit(0);
				}

				for(j=1; j<=22; j++) fscanf(fFeedOut10, "%f", &ToCompost[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut10, "%f", &ToLagoon[j]);
				for(j=1; j<=22; j++) fscanf(fFeedOut10, "%f", &ToDigester[j]);	
				for(j=1; j<=22; j++) fscanf(fFeedOut10, "%f", &ToField[j]);
			}//end of if

			//CN=q+4; update compost pools
				rcvl[CN] += ToCompost[1];
				rcl[CN] += ToCompost[2];
				rcr[CN] += ToCompost[3] ;
				CRB1[CN] += ToCompost[4] ;
				CRB2[CN] += ToCompost[5] ;
				crhl[CN] += 0.5*ToCompost[6]; 
				crhr[CN] += 0.5*ToCompost[6];
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
			//LN=q+5; update lagoon pools
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
			//DN=q+6; update digester pools
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

			/*MData.House_to_compost_H2O = ToCompost[19];
			MData.House_to_compost_P = (ToCompost[20]+ToCompost[21]+ToCompost[22]);
			MData.House_to_lagoon_H2O = ToLagoon[19];
			MData.House_to_lagoon_P = (ToLagoon[20]+ToLagoon[21]+ToLagoon[22]);
			MData.House_to_digester_H2O = ToDigester[19];
			MData.House_to_digester_P = (ToDigester[20]+ToDigester[21]+ToDigester[22]);*/
		}//end of feedlots loop

		//compost additive litter
		if(MData.m_LitterAmount>0.0)
		{
			float AddC = MData.m_LitterAmount;
			float AddCN = MData.m_LitterCN;
			float AddC1=0.0, AddC2=0.0, AddC3=0.0, dInertC=0.0;

			alloc_litter_into_pools(&AddCN, &AddC, 0, &AddC1, &AddC2, &AddC3, &dInertC, 1, rcnrvl, rcnrl, rcnrr, nh4);

			rcvl[CN] += AddC1;
			rcl[CN] += AddC2;
			rcr[CN] += AddC3;
		}

		//read storage/treatment facility specifications
		MVM->ManureTreatmentFacility(q, scale, &CompostCover, &LagoonCapacity);

		MVM->ManureTreatmentBGC(
			&FN, &GN, jday, rcnrr, rcnh, rcnb, urea, 
			doc, rcvl, rcl, rcr, CRB1, CRB2, dphum, temp, air_temp, DayPET, 
			wind_speed,	day_wfps, soc, sph, &flood_flag, rcnrvl, rcnrl, WTL, 
			day_WT, soil_ph, nh4, &ch4_prod, &ch4_day_oxid, &plant_day_ch4, q, rcnm, son, SIN,
			no3, no2, no, n2o, n2, nh3, ch4, TTT, newh, 
			&w_no2, &w_nh3, &w_no3, &w_nh4, &wurea, day_clayn, &wpool_no, DayFlux,tilq, fCB, 
			fNB, &w_n2o, &w_n2, seed, &day_ch4, anvf, WaterLeakRate, co2, clay,	LandUse, 
			FloDays, crhl, crhr, mmm, ocini, h, &day_C_mine, &day_N_mine, &day_N_assim,
			DClitter, &litco22, &litco23, st, DChumads, EFFAC, DChumus,	wetland_flag, ice, &day_soil_nh3, 
			lai, PlantTakeN, maxmn, leaf_water, cropht, base_nh3,day_N_demand, day_nup, day_fixN, day_DWater, 
			day_plant_nh3, &yr_NH3_deposit, plantn,clay_nh4, base_clay_N, max_clay_N,&wrcvl, &wrcl, &wrcr, &wcrb, 
			&wcrhl, &wcrhr, &whumus, &wtcavai,&day_ice_N, &day_ice_doc, &day_ice_n2o, &day_ice_n2, &IniSOC, &IniSON, &EndSON,
			day_runoff, &day_runoff_N, &day_clay_N, 
			dsno3, dsnh4, &dsnh3, &day_dsurea, &ds_doc, crop, yr_fixn, till_fact,
			sts, rainfall, O2_old, O2, vvv, ave_anvf, year, water, total_ice, day_rco2, flo_num,
			Eh, doc_w, nh4_w, no3_w, no2_w, no_w, n2o_w, n2_w,
			doc_d, nh4_d, no3_d, no2_d, no_d, n2o_d, ttoo, ManureAddCN, ManureDays, MicrobioIndex,
			RBO, FD, WaterControl, assim2, fldcap, by_passf, NIDay1, NIDay2,
			&day_soil_nitrify, &day_nitrify_NO, &day_nitrify_N2O, &day_soil_an_no,
			&day_soil_an_n2o, &day_soil_n2, Root_Wt, &day_soil_denitrification,
			R_Wt, vascularity, wiltpt, &day_addmc, &yr_addmc, &yr_addmn, &day_inManureN, Odoc,
			&day_frostDOC, &yr_frostDOC, &winertC, inert_C, fert_flag, root_doc, 
			precipitation, manuFlag, irri_flag, FreezeDOC, day_O2, ftf,
			&day_farm_to_field_N, &day_farm_to_field_C,&day_farm_to_field_H2O, RFM, SoilDepth,
			scale, rcnh2, &C_soil, Eh1, sop, OrgP, LabP, ComP, AdsP, &day_farm_to_field_P,
			LateralInflux, drain_flag, ToField, day_leach_water, CompostCover, LagoonCapacity);//, &House_to_compost_C, &House_to_compost_N, &House_to_compost_H2O);


		annu_accumu_manure_storage(jday);

		//MVM->FM_paint(pLinkInfo, cwc, crop_name[cwc], scale, ViewMode, cropping_system, jday,
		//	air_temp, rainfall, soc, day_wfps, q, nh4, no3, urea, water, temp, day_soil_nh3,
		//	day_plant_nh3, day_vol_nh3, day_soil_an_n2o, day_soil_n2);

		if (scale == 0)
		{
			MVM->Total_SOCSON(q+5, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
					 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);

			MVM->write_out_dayLagoon(flagoon, jday, temp[LN], nh4, no3, nh3, son, urea,
					site, year, q, soc, water, no2, no, n2o, n2, sop, m_unit, doc, MData.LagoonT);

			MVM->Total_SOCSON(q+4, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
					 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);

			MVM->write_out_dayCompost(fcompost, jday, site, year, q, 
					day_wfps,sph,soc,son,urea,nh4,no3,doc,temp, water, nh3, sop, m_unit);

			MVM->Total_SOCSON(q+6, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
					 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, co2, sop,OrgP, LabP, ComP);

			MVM->write_out_dayDigester(fdigester, jday, site, year, q, 
					day_wfps,sph,soc,son,urea,nh4,no3,doc,
					temp, water, nh3, sop, m_unit);

			MVM->write_out_dayTreatmentGas(ftreatgas, jday, site, year, q, day_wfps,sph,soc,son,urea,nh4,no3,doc,
					temp, water, nh3, day_soil_nh3, day_vol_nh3, day_soil_an_n2o, sop, m_unit);
			//MOM->write_out_dayFarmCN(ffarmC, jday, site, year, m_unit);
		}

	}//end of daily loop


	//close files for recording pools/fluxes of treatment facilities
	fclose(fday);
	fclose(ftf);
	fclose( fcompost );
	fclose( flagoon );
	fclose( fdigester );
	fclose( ftreatgas );

	//close files of daily manure removal from feedlots to treatment facilities
	for(i=1; i<=m_FeedLots; i++)
	{
		if(i==1) fclose(fFeedOut1);
		else if(i==2) fclose(fFeedOut2);
		else if(i==3) fclose(fFeedOut3);
		else if(i==4) fclose(fFeedOut4);
		else if(i==5) fclose(fFeedOut5);
		else if(i==6) fclose(fFeedOut6);
		else if(i==7) fclose(fFeedOut7);
		else if(i==8) fclose(fFeedOut8);
		else if(i==9) fclose(fFeedOut9);
		else if(i==10) fclose(fFeedOut10);
	}

	yr_Lagoon_to_field_N_T = MData.yr_Lagoon_to_field_N;
	yr_Lagoon_to_field_C_T = MData.yr_Lagoon_to_field_C;
	yr_Lagoon_to_field_H2O_T = MData.yr_Lagoon_to_field_H2O;
	yr_Compost_to_field_N_T = MData.yr_Compost_to_field_N;
	yr_Compost_to_field_C_T = MData.yr_Compost_to_field_C;
	yr_Compost_to_field_H2O_T = MData.yr_Compost_to_field_H2O;
	yr_Digester_to_field_N_T = MData.yr_digester_to_field_N;
	yr_Digester_to_field_C_T = MData.yr_digester_to_field_C;
	yr_Digester_to_field_H2O_T = MData.yr_digester_to_field_H2O;

	yr_Lagoon_to_field_P_T = MData.yr_Lagoon_to_field_P;
	yr_Compost_to_field_P_T = MData.yr_Compost_to_field_P;
	yr_Digester_to_field_P_T = MData.yr_digester_to_field_P;

	if(FarmField==1)
		write_out_annual_report_treatment(scale, 2, ha, Batch, livestock, fout2, 0);

	write_out_end_manure();

	
}

int class_model::write_out_annual_report_treatment(int MODE, int FarmPart, float ha, int Batch, 
												int livestock, FILE *fout2, int Feedlot)
{
	float YrLeachNO3;
	MData.yr_milk_water = MData.yr_milk_product - MData.yr_milk_C - MData.yr_milk_N;	
	float yr_meat_water = MData.yr_meat_C / 0.2;
	float yr_intake_water = MData.yr_urine_water + MData.yr_dung_water + MData.yr_milk_water + yr_meat_water;
	float _ha=MData.ManureFieldArea, ttn;
	float FIC = MData.yr_Lagoon_to_field_C+MData.yr_Compost_to_field_C+MData.yr_digester_to_field_C+(yr_addtc+yr_addrc)*_ha;
	float FOC = (yr_soil_co2)*_ha+yr_ch4*_ha;
	float DFC = FIC - FOC;
	float FIN = MData.yr_Lagoon_to_field_N+MData.yr_Compost_to_field_N+MData.yr_digester_to_field_N+yr_rainN*_ha+(yr_sln)*_ha+yr_addfer*_ha+yr_irri_N*_ha;
	float FON = yr_soil_nh3*_ha+yr_dnn2o*_ha+yr_dnno*_ha+yr_dnn2*_ha+(yr_uptaken[1]+yr_uptaken[2]+yr_uptaken[3])*_ha+YrLeachNO3*_ha;
	float DFN = FIN - FON;
	float DFW = yr_dSoilWater*10000.0*_ha*1000.0;
	///////////////////////////////////////////////////////////////////////////////////////////
	
	float ttn4=MData.Total_SOCSON(q+4, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm,soc, son, SIN, rcvl, rcl, rcr, 
								 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, 
								 co2, sop,OrgP, LabP, ComP);
	float ttc4 = MData.ttc;
	float ttp4 = MData.ttp;

	float ttn5=MData.Total_SOCSON(q+5, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, SIN, rcvl, rcl, rcr, 
								 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, 
								 co2, sop,OrgP, LabP, ComP);
	float ttc5 = MData.ttc;
	float ttp5 = MData.ttp;

	float ttn6=MData.Total_SOCSON(q+6, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm,soc, son, SIN, rcvl, rcl, rcr, 
								 CRB1, CRB2, crhl, crhr, dphum, doc, nh4, no3, no2, no, n2o, n2, nh3, urea, rcnh2, 
								 co2, sop,OrgP, LabP, ComP);
	float ttc6 = MData.ttc;	
	float ttp6 = MData.ttp;

	fprintf(fout2, "---------------------------------------------------------------------------------------------\n");
	fprintf(fout2, "%-25s %11s     %11s     %11s     %11s\n\n", "COMPOST", "C_kg", "N_kg", "Water_kg", "P_kg");
	//fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Influx_from_feedlot", ToComC, ToComN, ToComH2O, ToComP);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n\n", "Influx_from_rainfall",0.0,0.0, MData.yr_compost_InRain, 0.0);	

	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Compost_CO2_emission", MData.yr_compost_co2, 0.0,0.0, 0.0);	
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Compost_CH4_emission", MData.yr_compost_ch4, 0.0,0.0, 0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Compost_NH3_emission", 0.0, MData.yr_compost_nh3, 0.0, 0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Compost_N2O_emission", 0.0, MData.yr_compost_n2o, 0.0, 0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Compost_NO_emission", 0.0, MData.yr_compost_no, 0.0, 0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Compost_N2_emission", 0.0, MData.yr_compost_n2, 0.0, 0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Compost_evaporation", 0.0, 0.0, MData.yr_compost_EV, 0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n\n", "Compost_runoff_loss", MData.yr_runoff_MC[4], MData.yr_LOS_LeachN+MData.yr_runoff_MON[4]+MData.yr_runoff_MIN[4], MData.yr_compost_leach_H2O+MData.yr_compost_runoff_WATER,MData.yr_runoff_MOP[4]);
	
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Applied_to_field", MData.yr_Compost_to_field_C, MData.yr_Compost_to_field_N, MData.yr_Compost_to_field_H2O, MData.yr_Compost_to_field_P);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Remain_in_compost", ttc4, ttn4, water[1][q+4], ttp4);
	fprintf(fout2, "---------------------------------------------------------------------------------------------\n");

	fprintf(fout2, "%-25s %11s     %11s     %11s     %11s\n\n", "LAGOON", "C_kg", "N_kg", "Water_kg", "P_kg");
	//fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Influx_from_feedlot", MData.yr_House_to_lagoon_C, MData.yr_House_to_lagoon_N, MData.yr_House_to_lagoon_H2O, MData.yr_House_to_lagoon_P);
	//fprintf(fout2, "%-25s %e    %e     %e    %e\n\n", "Influx_from_digester", MData.yr_digester_to_lagoon_C, MData.yr_digester_to_lagoon_N, MData.yr_digester_to_lagoon_H2O, MData.yr_digester_to_lagoon_P);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n\n", "Influx_from_rainfall",0.0,0.0, MData.yr_lagoon_InRain, 0.0);	

	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Lagoon_CO2_emission", MData.yr_lagoon_co2, 0.0, 0.0, 0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Lagoon_CH4_emission", MData.yr_lagoon_ch4, 0.0, 0.0, 0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Lagoon_NH3_emission", 0.0, MData.yr_lagoon_nh3, 0.0, 0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Lagoon_N2O_emission", 0.0, MData.yr_lagoon_n2o, 0.0, 0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Lagoon_NO_emission", 0.0, MData.yr_lagoon_no, 0.0, 0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Lagoon_N2_emission", 0.0, MData.yr_lagoon_n2, 0.0, 0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n\n", "Lagoon_evaporation", 0.0, 0.0, MData.yr_lagoon_EV, 0.0);
	
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Applied_to_field", MData.yr_Lagoon_to_field_C, MData.yr_Lagoon_to_field_N, MData.yr_Lagoon_to_field_H2O, MData.yr_Lagoon_to_field_P);// MData.yr_House_to_lagoon_H2O-MData.yr_lagoon_EV-water[1][-4]);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Transfered_to_digester", MData.yr_lagoon_to_digester_C, MData.yr_lagoon_to_digester_N, MData.yr_lagoon_to_digester_H2O, MData.yr_lagoon_to_digester_P);

	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Remain_in_lagoon", ttc5, ttn5, water[1][q+5], ttp5);
	fprintf(fout2, "---------------------------------------------------------------------------------------------\n");

	fprintf(fout2, "%-25s %11s     %11s     %11s     %11s\n\n", "DIGESTER", "C_kg", "N_kg", "Water_kg", "P_kg");
	//fprintf(fout2, "%-25s %e    %e     %e    %e\n\n", "Influx_from_feedlot", MData.yr_House_to_digester_C, MData.yr_House_to_digester_N, MData.yr_House_to_digester_H2O, MData.yr_House_to_digester_P);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Digester_CO2_emission", MData.yr_digester_co2, 0.0,0.0,0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Digester_CH4_emission", MData.yr_digester_ch4, 0.0,0.0,0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Digester_NH3_emission", 0.0, MData.yr_digester_nh3, 0.0,0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Digester_N2O_emission", 0.0, MData.yr_digester_n2o, 0.0,0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Digester_NO_emission", 0.0, MData.yr_digester_no, 0.0,0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Digester_N2_emission", 0.0, MData.yr_digester_n2, 0.0,0.0);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n\n", "Evaporation", 0.0, 0.0, MData.yr_digester_EV,0.0);
	
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Applied_to_field", MData.yr_digester_to_field_C, MData.yr_digester_to_field_N, MData.yr_digester_to_field_H2O, MData.yr_digester_to_field_P);
	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Transfered_to_lagoon", MData.yr_digester_to_lagoon_C, MData.yr_digester_to_lagoon_N, MData.yr_digester_to_lagoon_H2O, MData.yr_digester_to_lagoon_P);

	fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Remain_in_digester", ttc6, ttn6, water[1][q+6], ttp6);
	fprintf(fout2, "---------------------------------------------------------------------------------------------\n");
	
	return(0);
}

void class_model::annu_accumu_manure_storage(int jday)
{

/*	MData.yr_house_nh3 += (MData.day_house_nh3+MData.day_gutter_nh3);
	MData.yr_house_n2o += (MData.day_house_n2o+MData.day_gutter_n2o);
	MData.yr_house_ch4 += (MData.day_house_ch4+MData.day_gutter_ch4); 
	MData.yr_house_no += (MData.day_house_no+MData.day_gutter_no);
	MData.yr_house_n2 += (MData.day_house_n2+MData.day_gutter_n2);
	MData.yr_house_co2 += (MData.day_house_co2+MData.day_gutter_co2);

	MData.yr_House_to_compost_C += MData.House_to_compost_C;
	MData.yr_House_to_lagoon_C += MData.House_to_lagoon_C;
	MData.yr_House_to_digester_C += MData.House_to_digester_C;

	MData.yr_House_to_compost_N += MData.House_to_compost_N;
	MData.yr_House_to_lagoon_N += MData.House_to_lagoon_N;
	MData.yr_House_to_digester_N += MData.House_to_digester_N;

	MData.yr_House_to_compost_P += MData.House_to_compost_P;
	MData.yr_House_to_lagoon_P += MData.House_to_lagoon_P;
	MData.yr_House_to_digester_P += MData.House_to_digester_P;

	MData.yr_House_to_compost_H2O += MData.House_to_compost_H2O;
	MData.yr_House_to_lagoon_H2O += MData.House_to_lagoon_H2O;
	MData.yr_House_to_digester_H2O += MData.House_to_digester_H2O;

	MData.yr_pen_nh3 += (MData.day_pen_nh3);
	MData.yr_pen_n2o += (MData.day_pen_n2o);
	MData.yr_pen_ch4 += (MData.day_pen_ch4); 
	MData.yr_pen_no += (MData.day_pen_no);
	MData.yr_pen_n2 += (MData.day_pen_n2);
	MData.yr_pen_co2 += (MData.day_pen_co2);

	MData.yr_Pen_to_compost_C += MData.Pen_to_compost_C;
	MData.yr_Pen_to_lagoon_C += MData.Pen_to_lagoon_C;
	MData.yr_Pen_to_digester_C += MData.Pen_to_digester_C;

	MData.yr_Pen_to_compost_N += MData.Pen_to_compost_N;
	MData.yr_Pen_to_lagoon_N += MData.Pen_to_lagoon_N;
	MData.yr_Pen_to_digester_N += MData.Pen_to_digester_N;

	MData.yr_Pen_to_compost_P += MData.Pen_to_compost_P;
	MData.yr_Pen_to_lagoon_P += MData.Pen_to_lagoon_P;
	MData.yr_Pen_to_digester_P += MData.Pen_to_digester_P;

	MData.yr_Pen_to_compost_H2O += MData.Pen_to_compost_H2O;
	MData.yr_Pen_to_lagoon_H2O += MData.Pen_to_lagoon_H2O;
	MData.yr_Pen_to_digester_H2O += MData.Pen_to_digester_H2O;
*/
	MData.yr_compost_nh3 += MData.day_Compost_nh3;
	MData.yr_compost_n2o += MData.day_compost_n2o;
	MData.yr_compost_ch4 += MData.day_compost_ch4; 
	MData.yr_compost_no += MData.day_compost_no;
	MData.yr_compost_n2 += MData.day_compost_n2;
	MData.yr_compost_co2 += MData.day_compost_co2;
	
	MData.yr_lagoon_nh3 += MData.day_lagoon_nh3;
	MData.yr_lagoon_n2o += MData.day_lagoon_n2o;
	MData.yr_lagoon_ch4 += MData.day_lagoon_ch4; 
	MData.yr_lagoon_no += MData.day_lagoon_no;
	MData.yr_lagoon_n2 += MData.day_lagoon_n2;
	MData.yr_lagoon_co2 += MData.day_lagoon_co2;

	for(int i=0;i<=9;i++)
	{
		MData.yr_runoff_MC[i] += MData.day_runoff_MC[i];
		MData.yr_runoff_MIN[i] += MData.day_runoff_MIN[i];
		MData.yr_runoff_MON[i] += MData.day_runoff_MON[i];
		MData.yr_runoff_MOP[i] += MData.day_runoff_MOP[i];

		MData.yr_market_MC[i] += MData.day_market_MC[i];
		MData.yr_market_MIN[i] += MData.day_market_MIN[i];
		MData.yr_market_MON[i] += MData.day_market_MON[i];
		MData.yr_market_MOP[i] += MData.day_market_MOP[i];
	}

	MData.yr_feedlot_runoff_WATER += MData.day_feedlot_runoff_WATER;
	MData.yr_compost_runoff_WATER += MData.day_compost_runoff_WATER;

	MData.yr_LOS_LeachN += MData.day_LOS_LeachN;

	if(jday==365)
	{
		MData.yr_LOS_ch4 += MData.yr_compost_ch4 + MData.yr_lagoon_ch4;
		MData.yr_LOS_n2o += MData.yr_compost_n2o + MData.yr_lagoon_n2o;
		MData.yr_LOS_nh3 += MData.yr_compost_nh3 + MData.yr_lagoon_nh3;
	}

}

void class_model::write_out_end_manure()
{
	FILE *fp;
    char  SOIL[80];
    int   FC, LL;

	FC = q + 4;
	LL = q + 5;

    sprintf(SOIL, "%s\\INPUTS\\ManureIni-%d", OUTPUT, year + 1);
	    
	fp = fopen(SOIL, "w");
	if(fp==NULL) note(1, SOIL);
	
	fprintf(fp, "Compost ");
	fprintf(fp, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", 	
				rcvl[FC], rcl[FC], rcr[FC], CRB1[FC], CRB2[FC], crhl[FC], crhr[FC], dphum[FC], co2[FC],
				doc[FC], nh4[FC], no3[FC], no2[FC], no[FC], n2o[FC], n2[FC], nh3[FC], urea[FC],OrgP[FC], 
				LabP[FC], ComP[FC]);
	fprintf(fp, "Lagoon ");
	fprintf(fp, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n", 	
				rcvl[LL], rcl[LL], rcr[LL], CRB1[LL], CRB2[LL], crhl[LL], crhr[LL], dphum[LL], co2[LL],
				doc[LL], nh4[LL], no3[LL], no2[LL], no[LL], n2o[LL], n2[LL], nh3[LL], urea[LL],OrgP[LL], 
				LabP[LL], ComP[LL]);	

	fclose(fp);
}

void class_model::read_out_ini_manure(void)
{
	FILE *fp;
    char  js[20], SOIL[80];
    int   FC, LL;

	FC = q + 4;
	LL = q + 5;

    sprintf(SOIL, "%s\\INPUTS\\ManureIni-%d", OUTPUT, year);
	    
	fp = fopen(SOIL, "r");
	if(fp==NULL) note(0, SOIL);
	
	fscanf(fp, "%s ", js);
	fscanf(fp, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", 	
				&rcvl[FC], &rcl[FC], &rcr[FC], &CRB1[FC], &CRB2[FC], &crhl[FC], &crhr[FC], &dphum[FC], &co2[FC],
				&doc[FC], &nh4[FC], &no3[FC], &no2[FC], &no[FC], &n2o[FC], &n2[FC], &nh3[FC], &urea[FC],&OrgP[FC], 
				&LabP[FC], &ComP[FC]);
	fscanf(fp, "%s ", js);
	fscanf(fp, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", 	
				&rcvl[LL], &rcl[LL], &rcr[LL], &CRB1[LL], &CRB2[LL], &crhl[LL], &crhr[LL], &dphum[LL], &co2[LL],
				&doc[LL], &nh4[LL], &no3[LL], &no2[LL], &no[LL], &n2o[LL], &n2[LL], &nh3[LL], &urea[LL],&OrgP[LL], 
				&LabP[LL], &ComP[LL]);	

	fclose(fp);
}

#endif