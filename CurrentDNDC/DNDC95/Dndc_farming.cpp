#include "stdafx.h"
#include "Dndcgo.h"
#include "Source_main.h"
#include "Dndc_tool.h"

/*void AddFarmManure(int adp, float CFT, float *MAC, float *MAN, float *MAP, float GoField[3][25],
				   float *rcvl, float *rcl, float * rcr, float * CRB1, float * CRB2, float * crhl, float * crhr, float * dphum, float * nh4, float * no3, float * no2, float * no, float *
				   n2o, float * n2, float * nh3, float * urea, float * co2, float * doc, 
				   double water[25][180], float * OrgP, float * LabP, float * ComP,
				   float rcnrvl, float rcnrl, float rcnrr, float rcnb, float rcnh, float rcnm);*/

float NPratio[7]={0.0,0.347,0.347, 0.513, 0.3, 0.34,0.58}; //1 dairy, 2 beef, 3 swine, 4 sheep, 5 horse, 6 poultry

float class_model::irrigating(int MODE, float DayPET)
{
	/*if ( rainfall <= .0001 )
    	TTT = 0.0;
    else
    	TTT = rainfall / 0.005;//m/m, hrs*/

    int   csl;
	//IrriWater = 0.0;
	   
	if(fer_num==-1)
	{	
		if(day_irri>0.0)
		{
			IrriWater = day_irri;
			yr_irri += day_irri; // m water 
			IrriMethod = 2;
			fertigate_flag = 1;
		}
		else
		{
			IrriMethod = 0;
			fertigate_flag = 0;
		}		
	}
	
	if (irr_num > 0)
	{
	  for ( csl = 1; csl <= irr_num;  csl++ )
	  {
		if ( jdirr[csl] == jday )
		{
			irri_flag=1;//++;

			IrriMethod = irrimethod[csl]; //0: flood, 1: sprinkler, 2: drip

			IrriWater = irria[csl];
						
			//rainfall += irria[csl];
			yr_irri += irria[csl]; // m water 
			day_irri += irria[csl];	
			
			//if(IrriMethod==0) TTT += (irria[csl] / 0.005);//
		}
	  }
	}
	else
	{
		IrriMethod = irrimethod[0];
	}
		
    return( TTT );
}

void class_model::fertilizing()
{
    int   ox, l, i, FertType=0; 
 	
	if(fert_flag==1) fert_flag=0;
	if(FertOption==222)//auto-fertilization
	{
		float ATF[7]={0.0, 1.0, 1.1, 1.2, 1.3, 1.4, 1.5};
		float ProfileNN = 0.0;
		for(i=1; i<=q; i++) ProfileNN += (nh4[i]+no3[i]);

		if(crop[1]==2&&jday==1)
		{
			FertAddN[1] = (maxmn[1] / ini_N_fix[1] - ProfileNN) * ATF[3];
			if(FertAddN[1]<0.0) FertAddN[1] = 0.0;
			day_inFertN += FertAddN[1];
			yr_addfer += FertAddN[1];
			if(FertAddN[1]>0.0)
			{
				fert_flag = 1;
				for (l=1; l<=2; l++) urea[l] += (FertAddN[1] / 2.0);
			}
		}

		for(i=1; i<=5; i++)
		{
			if(seed[i]==1) 
			{
				FertAddN[i] = (maxmn[i] / ini_N_fix[i] - ProfileNN) * ATF[3];
				if(FertAddN[i]<0.0) FertAddN[i] = 0.0;
				day_inFertN += FertAddN[i];
				yr_addfer += FertAddN[i];
				if(FertAddN[i]>0.0)
				{
					fert_flag = 1;
					for (l=1; l<=2; l++) urea[l] += (FertAddN[i] / 2.0);
				}
			}
		}
	}
	else if(FertOption==111)//fertigation
	{
		fert_flag = 0;
		IrriMethod = 2;

		float day_add_fert = day_fert1+day_fert2+day_fert3+day_fert4+day_fert5+day_fert6;
		if(day_add_fert>0.0)
		{
			yr_addfer += day_add_fert;
			day_inFertN += day_add_fert;
			int sqq=(int)(0.1/h[1]);
			
			for(int ss=sqq-1; ss<=sqq; ss++)
			{
				no3[ss] += (day_fert1 / 2.0);//nitrate
				NH4HCO3[ss] += (day_fert2 / 2.0);//ammnium bicarbonate
				urea[ss] += (day_fert3*0.5 / 2.0);//urea
				nh3[ss] += (day_fert4 / 2.0);//anhydrous ammonia
				nh4[ss] += ((day_fert5+day_fert3*0.5) / 2.0);//ammonium
				//nh4[ss] += (day_fert6 / 2.0);//sulphate
				//nh4[ss] += (day_fert7 / 2.0);//phosphate

				float FFn2o;
				FFn2o = 0.002 * nh4[ss];
				nh4[ss] -= FFn2o;
				n2o[ss] += FFn2o;

				FFn2o = 0.005 * no3[ss];
				no3[ss] -= FFn2o;
				n2o[ss] += FFn2o;

				FFn2o=0.0;
			}
		}
	}
	else//FertOption==0
	{
		for ( ox = 1; ox <= fer_num; ox++ )
		{
			if ( jday == jdfer[ox])
			{
				fert_flag = 1;

				UreaHydroInhibEf = UreaseEfficiency[ox];
				UreaHydroInhibDu = UreaseDuration[ox];
				if(UreaHydroInhibDu>0.0) UreaHydroInhibDays = 1;
				else UreaHydroInhibDays = 0;
				
				FertDepth = fer_dept[ox];
				fer_layer = (int)(fer_dept[ox] / h[1] + 1);//????
				if(fer_layer<1) fer_layer=1;

				NIEfficiency = NIefficiency[ox];
				NIDuration0 = NIduration[ox];
				NIDuration1 = NIduration[ox];

				NIDay1 = jday;
				NIDay2 = NIDay1 + (int)NIduration[ox];


				//day_inFertN = (fer1[ox] + fer2[ox] + fer3[ox] + fer4[ox] + fer5[ox] + fer6[ox] + fer7[ox]);
				//yr_addfer += day_inFertN;

/*				float FFy = 0.0222 * (float)log(FertDayRelease[ox]) + 0.876£»
				float RemainFert1 = fer1[ox] * (float)pow(FFy, FertDays));
*/					
				AppliedUrea = fer3[ox];

				if(fer3[ox]>0.0) UreaAppDays = 1;

				//slow release fertilizer
				if(FertDayRelease[ox]>1.0) 
				{
					SlowReleaseDays = FertDayRelease[ox];
					
					DayFixFert1 += fer1[ox];
					DayFixFert2 += fer2[ox];
					DayFixFert3 += fer3[ox];
					DayFixFert4 += fer4[ox];
					DayFixFert5 += fer5[ox];
					DayFixFert6 += fer6[ox];
					DayFixFert7 += fer7[ox];

					IniFixFert1 = fer1[ox];
					IniFixFert2 = fer2[ox];
					IniFixFert3 = fer3[ox];
					IniFixFert4 = fer4[ox];
					IniFixFert5 = fer5[ox];
					IniFixFert6 = fer6[ox];
					IniFixFert7 = fer7[ox];

					DayFertRelease[1] = fer1[ox] / SlowReleaseDays;
					DayFertRelease[2] = fer2[ox] / SlowReleaseDays;
					DayFertRelease[3] = fer3[ox] / SlowReleaseDays;
					DayFertRelease[4] = fer4[ox] / SlowReleaseDays;
					DayFertRelease[5] = fer5[ox] / SlowReleaseDays;
					DayFertRelease[6] = fer6[ox] / SlowReleaseDays;
					DayFertRelease[7] = fer7[ox] / SlowReleaseDays;

					/*DayFert1[jday] += fer1[ox];
					DayFert2[jday] += fer2[ox];
					DayFert3[jday] += fer3[ox];
					DayFert4[jday] += fer4[ox];
					DayFert5[jday] += fer5[ox];
					DayFert6[jday] += fer6[ox];
					DayFert7[jday] += fer7[ox];*/
				}
				else
				{
					DayFert1[jday] += fer1[ox];
					DayFert2[jday] += fer2[ox];
					DayFert3[jday] += fer3[ox];//urea
					DayFert4[jday] += fer4[ox];
					DayFert5[jday] += fer5[ox];
					DayFert6[jday] += fer6[ox];
					DayFert7[jday] += fer7[ox];

					fer1[ox] = 0.0;
					fer2[ox] = 0.0;
					fer3[ox] = 0.0;
					fer4[ox] = 0.0;
					fer5[ox] = 0.0;
					fer6[ox] = 0.0;
					fer7[ox] = 0.0;
				}
			}
		}

		if(fert_flag==0) DAA = 0;
		else DAA++;
		
		if(SlowReleaseDays>0)
		{
			if((DayFixFert1+DayFixFert2+DayFixFert3+DayFixFert4+DayFixFert5+DayFixFert6+DayFixFert7)>0.000001)
			{
				//float TFdaa = 0.5963 * (float)pow((float)SlowReleaseDays, -0.7433);
				sfer1 = DayFertRelease[1];//DayFixFert1 * TFdaa;
				sfer2 = DayFertRelease[2];//DayFixFert2 * TFdaa;
				sfer3 = DayFertRelease[3];//DayFixFert3 * TFdaa;
				sfer4 = DayFertRelease[4];//DayFixFert4 * TFdaa;
				sfer5 = DayFertRelease[5];//DayFixFert5 * TFdaa;
				sfer6 = DayFertRelease[6];//DayFixFert6 * TFdaa;
				sfer7 = DayFertRelease[7];//DayFixFert7 * TFdaa;

				DayFert1[jday] += sfer1;
				DayFert2[jday] += sfer2;
				DayFert3[jday] += sfer3;
				DayFert4[jday] += sfer4;
				DayFert5[jday] += sfer5;
				DayFert6[jday] += sfer6;
				DayFert7[jday] += sfer7;
				
				DayFixFert1 -= sfer1;
				DayFixFert2 -= sfer2;
				DayFixFert3 -= sfer3;
				DayFixFert4 -= sfer4;
				DayFixFert5 -= sfer5;
				DayFixFert6 -= sfer6;
				DayFixFert7 -= sfer7;

				DayFixFert1 = max(DayFixFert1, 0.0);
				DayFixFert2 = max(DayFixFert2, 0.0);
				DayFixFert3 = max(DayFixFert3, 0.0);
				DayFixFert4 = max(DayFixFert4, 0.0);
				DayFixFert5 = max(DayFixFert5, 0.0);
				DayFixFert6 = max(DayFixFert6, 0.0);
				DayFixFert7 = max(DayFixFert7, 0.0);

				day_inFertN += sfer1+sfer2+sfer3+sfer4+sfer5;//+sfer6;
				yr_addfer += day_inFertN;
			}
			else
				SlowReleaseDays = 0;				
		}
		else //normal fertilizer
		{
			//Nitrate, NH4HCO3, Urea, Anhydrous_ammonia, NH4+, SO4, NH42HPO4 
			if(DayFert1[jday]+DayFert2[jday]+DayFert3[jday]+DayFert4[jday]+DayFert5[jday]>0.00001)
			{
				day_inFertN += DayFert1[jday]+DayFert2[jday]+DayFert3[jday]+DayFert4[jday]+DayFert5[jday];//+DayFert7[jday];
	
				yr_addfer += day_inFertN;
			}

			if(DayFert4[jday]>0.0)//ammonia
			{
				float v_nh3;
				float v_d = FertDepth*100.0;
				if(v_d<1.5) v_d=1.5;
				v_nh3 = 0.02 * DayFert4[jday] * air_temp / v_d ;
				day_soil_nh3 += v_nh3;
				DayFert4[jday] -= v_nh3;
			}

			if(DayFert7[jday]>0.0)//phosphate
			{
				float addP = DayFert7[jday] * 30.97 / 28.0;
				LabP[1] += addP;
				day_inFertP = addP;
			}
			
			if(DayFert6[jday]>0.0)//sulphate
			{
				for(int j=1; j<=2; j++)
					SO4[j] += DayFert6[jday]/28.0*1000000.0/mmm[j]; //mmol SO4/kg soil
			}
		}
		
		if(DayFert1[jday]+DayFert2[jday]+DayFert3[jday]+DayFert4[jday]+DayFert5[jday]>0.00001)
		{
			for (l=fer_layer; l<=(fer_layer + 1); l++ )
			{
				no3[l] += ((DayFert1[jday]) / 2.0);
				NH4HCO3[l] += (DayFert2[jday] / 2.0);
				urea[l] += (DayFert3[jday] / 2.0);
				nh4[l] += (DayFert4[jday] / 2.0);
				nh4[l] += (DayFert5[jday] / 2.0);
				//nh4[l] += (DayFert6[jday] / 2.0);
				//nh4[l] += (DayFert7[jday] / 2.0);
			
				float Nitrify=0.0;//y = 2E+07x-11.432
				float Factor_ph = 2E+07 * (float)pow(sph[q], -11.432);//0.00041+2*0.01221/3.14*2.029/(4*pow((sph[1]-7.5),2)+2.029*2.029);//lena, Lorentz
				float temp_fact = min(1.0, 0.1654 * exp(0.0742*temp[1]));	//Ralf, changed
				float TopNH4 = nh4[l]+0.2*urea[l];

				Factor_ph = min(0.01, max(0.002, Factor_ph));
				float Ef = 2.0 * temp_fact * Factor_ph * day_wfps[l];//2.0
				if(Ef>1.0) Ef=1.0;

				if(TopNH4>0.0&&temp[1]>0.0)//&&day_wfps[l]>(0.5*fldcap[l]))//TTT>0.0) 
				{			
					Nitrify = Ef*TopNH4*MicrobioIndex;// * day_wfps[l];//0.65
					nh4[l] -= (Nitrify * nh4[l]/TopNH4);
					urea[l] -= (Nitrify * (0.2*urea[l])/TopNH4);
				}
				else Nitrify = 0.0;

				float Fdpt = 1.0;
				int LPW = (int)(0.12/h[1]);
				if(l>LPW) Fdpt = (float)pow(0.9, (l-LPW));

				no[l] += (Nitrify * 0.4 * Fdpt);
				n2o[l] += (Nitrify * 0.6 * Fdpt);
			}
		}
	}

	if(DayFixFert1<0.0) DayFixFert1=0.0;
	if(DayFixFert2<0.0) DayFixFert2=0.0;
	if(DayFixFert3<0.0) DayFixFert3=0.0;
	if(DayFixFert4<0.0) DayFixFert4=0.0;
	if(DayFixFert5<0.0) DayFixFert5=0.0;
	if(DayFixFert6<0.0) DayFixFert6=0.0;
	if(DayFixFert7<0.0) DayFixFert7=0.0;

}

float class_model::manuring(int livestock, int scale, int FarmNumber, float *FarmCropArea, 
							float *FarmCropManureFraction, int FarmField, FILE *ftf)
{
    int   oz=0, l, manuType, adp;
    float AddC=0.0, liquid_N, manure_add_N=0.0;
	float AddC1=0.0, AddC2=0.0, AddC3=0.0, dUrea=0.0;	
	float AddCN=100.0, dInertC=0.0;

	manuFlag=0;
	
    for ( oz = 1; oz <= man_num; oz++ )
    {
		manuMethod = ManureMethod[oz];
		
		if(manuMethod==1) adp = (int)(0.15/h[1]); //injection to 15 cm
		else adp = 1;//surface application

		if(jday==jdman[oz]&&manuc[oz]>0.0)
		{
			manuFlag=1;
			ManureDays = 1;

			AddC = manuc[oz];
			AddCN = manucn[oz];
			ManureAddCN = AddCN;
			manuType = ManureType[oz];
			//manuMethod = ManureMethod[oz];
		
			if (ManureType[oz]==1)//farmyard manure
			{
				float TN = AddC / AddCN;
				float TC = AddC;

				/*float dcrhr, dcrhl;
				dcrhr = (TN - TC/rcnh) / (1.0/rcnh2 - 1.0/rcnh);
				dcrhl = TC - dcrhr;

				float TNTN = dcrhr/rcnh2 + dcrhl/rcnh;
				float TCTC = dcrhr + dcrhl;
				for ( l = 1; l <= 2; l++ )
				{
					crhl[l] += dcrhl * 0.5;
					crhr[l] += dcrhr * 0.5;
				}*/
				
				float UreaN = 0.1 * TN;//0.2
				manure_add_N += UreaN;

				//NH3 volatilization from application
				float fV;			
				if(manuMethod!=1) fV = 0.05*air_temp/40.0;
				else fV = 0.01*air_temp/40.0;
				fV = min(1.0, max(0.0, fV));

				float FYM_NH3 = fV * UreaN;
				day_vol_nh3 += FYM_NH3;

				nh4[1] += (UreaN - FYM_NH3) * 0.8;
				no3[1] += (UreaN - FYM_NH3) * 0.2;

				float NewTotN = TN - UreaN;
				int FF = (int)(NewTotN / rcnh);

				float AddHumN = 0.5 * (float)FF * rcnh;
				float LeftTotN = NewTotN - AddHumN;
				AddHum = AddHumN * rcnh;

				float NewAddC = AddC - AddHum;
				float NewAddCN = NewAddC / (LeftTotN + 0.0000001);

				alloc_manure_into_pools(NewAddCN, NewAddC, ManureType[oz], &AddC1, &AddC2, &AddC3, &dInertC);

				manure_add_N += (AddC1 / rcnrvl + AddC2 / rcnrl + AddC3 / rcnrr + AddHum / rcnh + dUrea);

				//partition manure into crhl and crhr
				
				for ( l = adp; l <= adp+1; l++ )
				{
					rcvl[l] += (AddC1 * 0.5);
					rcl[l] += (AddC2 * 0.5);
					rcr[l] += (AddC3 * 0.5);
					crhr[l] += AddHum * 0.5;
					//crhr[l] += dcrhr * 0.5;
					//dphum[l] += AddHum * 0.5;
					//doc[l] += AddHum * 0.5;
					//nh4[l] += AddHum * 0.25 / rcnh;
				}
			}
			else if (ManureType[oz] == 2)//green manure
			{
				alloc_litter_into_pools(&AddCN, &AddC, manuType, &AddC1, &AddC2, &AddC3, &dInertC, 0, rcnrvl, rcnrl, rcnrr, nh4);
				AddHum = 0.0;

				manure_add_N += (AddC1 / rcnrvl + AddC2 / rcnrl + AddC3 / rcnrr + AddHum / rcnh);

				for ( l = adp; l <= adp+1; l++ )
				{
					rcvl[l] += (AddC1 * 0.5);
					rcl[l] += (AddC2 * 0.5);
					rcr[l] += (AddC3 * 0.5);
					crhl[l] += AddHum * 0.5;
					//doc[l] += AddHum * 0.5;
					//nh4[l] += AddHum * 0.25 / rcnh;
				}
			}
			else if (ManureType[oz] == 3)//straw
			{
				AddHum = 0.0;

				alloc_litter_into_pools(&AddCN, &AddC, manuType, &AddC1, &AddC2, &AddC3, &dInertC, 0, rcnrvl, rcnrl, rcnrr, nh4);
				manure_add_N += (AddC1 / rcnrvl + AddC2 / rcnrl + AddC3 / rcnrr + AddHum / rcnh);
				
				if(manuMethod==0)
				{
					stub1 += AddC1;
					stub2 += AddC2;
					stub3 += AddC3;
				}
				else
				{
					for ( l = adp; l <= adp+1; l++ )
					{
						rcvl[l] += (AddC1 * 0.5);
						rcl[l] += (AddC2 * 0.5);
						rcr[l] += (AddC3 * 0.5);
						crhl[l] += AddHum * 0.5;
						//doc[l] += AddHum * 0.5;
						//nh4[l] += AddHum * 0.25 / rcnh;
					}
				}
			}
			else if (ManureType[oz] == 4)//slurry animal waste
			{
				float totN, F_urea, AsHum, NewTotN, NewAddC, NewAddN, NewAddCN, UreaN, SlurryNH3;

				SlurryFlag=1;
				
				totN= AddC / AddCN;
				manure_add_N += totN;

				F_urea = 0.4;//0.2
				UreaN = totN * F_urea;
				NewTotN = totN - UreaN;

				for(;;)
				{
					NewAddCN = AddC / NewTotN;
					if(NewAddCN<rcnh) 
					{
						UreaN += NewTotN * 0.5;
						NewTotN *= 0.5;
					}
					else break;
				}

				float AddDoc = AddC * 0.1;
				slurry_doc += AddDoc;
				AddC3 = ((AddC-AddDoc)-rcnh*NewTotN) / (1.0 - rcnh/rcnrr);;
				AddHum = AddC - AddC3 - AddDoc;
				AddC1 = 0.0;
				AddC2 = 0.0;
		
				//NH3 volatilization from slurry application
				float fV;			
				if(manuMethod!=1) fV = 0.1*air_temp/40.0;//fV = 0.1*air_temp/40.0;//
				else fV = 0.05*air_temp/40.0;//0.05				

				SlurryNH3 = fV * UreaN;
				if(SlurryNH3<0.0) SlurryNH3 = 0.0;
				day_vol_nh3 += SlurryNH3;

				nh4[1] += (UreaN - SlurryNH3);	
				
				//for(int k=1;k<=10;k++) sph[k]=7.5;

				for ( l = adp; l <= adp+1; l++ )
				{
					rcvl[l] += (AddC1 * 0.5);
					rcl[l] += (AddC2 * 0.5);
					rcr[l] += (AddC3 * 0.5);
					crhl[l] += AddHum * 0.5;
					//doc[l] += AddHum * 0.5;
					//nh4[l] += AddHum * 0.25 / rcnh;
				}
			}
			else if(ManureType[oz]==5)//compost
			{
				alloc_litter_into_pools(&AddCN, &AddC, manuType, &AddC1, &AddC2, &AddC3, &dInertC, 0, rcnrvl, rcnrl, rcnrr, nh4);
				AddHum = 0.0;
				manure_add_N += (AddC1 / rcnrvl + AddC2 / rcnrl + AddC3 / rcnrr + AddHum / rcnh);

				for ( l = adp; l <= adp+1; l++ )
				{
					rcvl[l] += (AddC1 * 0.5);
					rcl[l] += (AddC2 * 0.5);
					rcr[l] += (AddC3 * 0.5);
					crhl[l] += AddHum * 0.5;
					//doc[l] += AddHum * 0.5;
					//nh4[l] += AddHum * 0.25 / rcnh;
				}
			}
			else if(ManureType[oz]==6&&AddC>0.0)//bean cake etc., modified on Oct 18, 2006
			{
				float ff=0.8;
				float avai_C = AddC * ff;
				float avai_N = avai_C/AddCN;			
				
				for (int i=1; i<=2; i++)
				{
				  nh4[i] += (avai_N * 0.5);
				  doc[i] += (avai_C * 0.5);
				  day_DOC_prod += (avai_C * 0.5);
				}

				AddC -= avai_C;

				alloc_litter_into_pools(&AddCN, &AddC, manuType, &AddC1, &AddC2, &AddC3, &dInertC, 0, rcnrvl, rcnrl, rcnrr, nh4);
				AddHum = 0.0;

				manure_add_N += (AddC1 / rcnrvl + AddC2 / rcnrl + AddC3 / rcnrr + AddHum / rcnh) + avai_N;

				for ( l = adp; l <= adp+1; l++ )
				{
					rcvl[l] += (AddC1 * 0.5);
					rcl[l] += (AddC2 * 0.5);
					rcr[l] += (AddC3 * 0.5);
					crhl[l] += AddHum * 0.5;
					//doc[l] += AddHum * 0.5;
					//nh4[l] += AddHum * 0.25 / rcnh;
				}

			}
			else if(ManureType[oz]==7&&AddC>0.0)////human waste, poultry waste, sludge, modified on Oct 18, 2006
			{
				float totN, NewTotN, NewAddCN, UreaN, SlurryNH3;
				
				totN= AddC / (AddCN+0.000001);

				UreaN = 0.6 * totN;
				NewTotN = 0.4 * totN; 

				float fPH = 2.0 * (0.2671 * sph[1] - 1.4762);
				if(fPH>1.0) fPH = 1.0;

				SlurryNH3 =  fPH * UreaN;
				if(SlurryNH3<0.0) SlurryNH3 = 0.0;
				day_vol_nh3 += SlurryNH3;

				nh4[1] += (UreaN - SlurryNH3);			

				NewAddCN = AddC / (NewTotN+0.0000001);

				ManureAddCN = NewAddCN;

				alloc_manure_into_pools(NewAddCN, AddC, ManureType[oz], &AddC1, &AddC2, &AddC3, &dInertC);

				float ddn = NewTotN - (AddC1 / rcnrvl + AddC2 / rcnrl + AddC3 / rcnrr + AddHum / rcnh);

				//day_vol_nh3 -= ddn;
				if(ddn<0.0) ddn = 0.0;
				nh4[1] += ddn;
							
				manure_add_N += totN;

				for ( l = adp; l <= adp+1; l++ )
				{
					rcvl[l] += (AddC1 * 0.5);
					rcl[l] += (AddC2 * 0.5);
					rcr[l] += (AddC3 * 0.5);
					crhl[l] += AddHum * 0.5;
					//doc[l] += AddHum * 0.5;
					//nh4[l] += AddHum * 0.25 / rcnh;
				}
			}
			else if ((ManureType[oz]==10||ManureType[oz]==8||ManureType[oz]==9)&&AddC>0.0)//meat/blood meal
			{
				float TN = AddC / AddCN;
				float UreaN = 0.9 * TN;
				manure_add_N += UreaN;

				//NH3 volatilization from slurry application
				float fV;			
				if(manuMethod!=1) fV = 0.5*air_temp/40.0;
				else fV = 0.01*air_temp/40.0;
				fV = min(1.0, max(0.0, fV));

				float FYM_NH3 = fV * UreaN;
				day_vol_nh3 += FYM_NH3;

				nh4[1] += (UreaN - FYM_NH3) * 0.9;
				no3[1] += (UreaN - FYM_NH3) * 0.1;

				float NewTotN = TN - UreaN;
				int FF = (int)(NewTotN / rcnh);

				float AddHumN = 0.5 * (float)FF * rcnh;
				float LeftTotN = NewTotN - AddHumN;
				AddHum = AddHumN * rcnh;

				float NewAddC = AddC - AddHum;
				float NewAddCN = NewAddC / (LeftTotN + 0.0000001);

				alloc_manure_into_pools(NewAddCN, NewAddC, ManureType[oz], &AddC1, &AddC2, &AddC3, &dInertC);

				manure_add_N += (AddC1 / rcnrvl + AddC2 / rcnrl + AddC3 / rcnrr + AddHum / rcnh + dUrea);
				
				for ( l = adp; l <= adp+1; l++ )
				{
					rcvl[l] += (AddC1 * 0.5);
					rcl[l] += (AddC2 * 0.5);
					rcr[l] += (AddC3 * 0.5);
					crhl[l] += AddHum * 0.5;
					//doc[l] += AddHum * 0.5;
					//nh4[l] += AddHum * 0.25 / rcnh;
				}
			}
			else {}			
	    
			day_addmc += (AddC1 + AddC2 + AddC3 + AddHum);
			yr_addmc += (AddC1 + AddC2 + AddC3 + AddHum);
			yr_addmn += manure_add_N;
			day_inManureN += manure_add_N;	   
    
			AddC1 = 0.0;
			AddC2 = 0.0;
			AddC3 = 0.0;
			liquid_N = 0.0;

			//EFFAC = 0.6;

			yr_addmn += add_liq_N;
			day_inManureN += add_liq_N;

			if(add_liq_N>0.0)
			{
				float dNH4= add_liq_N * 0.25;
				float dNO3= add_liq_N * 0.0;
				float dNH3 = add_liq_N * 0.15; 
				float dUrea = add_liq_N * 0.6; 

				for (int i=1; i<=2; i++)
				{
					nh4[i] += (dNH4 * 0.5);
					no3[i] += (dNO3 * 0.5);
					urea[i] += (dUrea * 0.5);
				}
			   
				day_vol_nh3 += dNH3;
				
				add_liq_N = 0.0;

			}
		}//if(jday==jdman[oz])
		else 
		{
			manuFlag=0;
		}
	}//manure application loop

/////////////////////////////////////////////////////
#ifdef MANURE

	int jjd;
	float ToField[20];

	fscanf(ftf, "%d %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", 
			&jjd, &ToField[1],&ToField[2],&ToField[3],&ToField[4],&ToField[5],&ToField[6],&ToField[6],&ToField[7],&ToField[8],
			&ToField[9],&ToField[10],&ToField[11],&ToField[12],&ToField[13],&ToField[14],&ToField[15],&ToField[16],
			&ToField[17],&ToField[18],&ToField[19],&ToField[20],&ToField[21],&ToField[22]);
	
	float MAC=0.0, MAN=0.0, MAP=0.0;
	float CFT = FarmCropArea[FarmField] / FarmCropManureFraction[FarmField];
	
	if(manuMethod==1) adp = (int)(0.15/h[1]); //injection to 15 cm
	else adp = 1;//surface application
	
		rcvl[adp]  += ToField[1] / CFT;//kg C -> kg C/ha
		rcl[adp]  += ToField[2] / CFT;
		rcr[adp]  += ToField[3] / CFT;
		CRB1[adp]  += ToField[4] / CFT;
		CRB2[adp]  += ToField[5] / CFT;
		crhl[adp] += ToField[6] / CFT;
		crhr[adp] += ToField[6] / CFT;
		dphum[adp]  += ToField[7] / CFT;
		nh4[adp]  += ToField[8] / CFT;
		no3[adp]  += ToField[9] / CFT;
		no2[adp]  += ToField[10] / CFT;
		no[adp]  += ToField[11] / CFT;
		n2o[adp]  += ToField[12] / CFT;
		n2[adp]  += ToField[13] / CFT;
		nh3[adp]  += ToField[14] / CFT;	
		urea[adp]  += ToField[15] / CFT;
		co2[adp]  += ToField[16] / CFT;
		doc[adp]  += ToField[17] / CFT;
		day_DOC_prod += ToField[17] / CFT;
		water[1][adp]  += ToField[19] /1000.0 / CFT/10000.0;//m water/ha
		OrgP[adp] += ToField[20] / CFT;
		LabP[adp] += ToField[21] / CFT;
		ComP[adp] += ToField[22] / CFT;

		yr_manure_water += ToField[19] /1000.0 / CFT/10000.0;//m water/ha

		MAC += (ToField[1]+ToField[2]+ToField[3]+ToField[4]+ToField[5]+
			ToField[6]+ToField[6]+ToField[7]+ToField[16]+ToField[17])/CFT;

		//float UreaN = (ToField[8]+ToField[14]+ToField[15])/CFT;

		MAN += (ToField[1]/rcnrvl +ToField[2]/rcnrl +ToField[3]/rcnrr 
			+ToField[4]/rcnb +ToField[5]/rcnb +
			ToField[6]/rcnh+ToField[6]/rcnh +ToField[7]/rcnm +
			ToField[9]+ToField[10]+ToField[11]
			+ToField[12]+ToField[13]+ToField[8]+ToField[14]+ToField[15])/CFT;//

		MAP += (ToField[20]+ToField[21]+ToField[22])/CFT;

		if(ToField[7]>0.0) sph[1] = 10.5;
	
	day_addmc += MAC;// + straw_DOC);
	yr_addmc += MAC;// + straw_DOC);
	yr_addmn += MAN;
	day_inManureN += MAN;

	day_addmP += MAP;// + straw_DOC);
	yr_addmP += MAP;

	day_inManureP += MAP;

	////////////////////////////////////////////////////
	if(MAC>0.0)
	{
		float totN, UreaN, F_urea, AsHum, NewTotN, NewAddC, NewAddN, NewAddCN, SlurryNH3;

		AddC = MAC;
		AddCN = MAC/MAN;
				
				totN= AddC / AddCN;
				manure_add_N += totN;

				F_urea = 0.4;//0.2
				UreaN = totN * F_urea;
				NewTotN = totN - UreaN;

				for(;;)
				{
					NewAddCN = AddC / NewTotN;
					if(NewAddCN<rcnh) 
					{
						UreaN += NewTotN * 0.5;
						NewTotN *= 0.5;
					}
					else break;
				}

				float AddDoc = AddC * 0.1;
				slurry_doc += AddDoc;
				AddC3 = ((AddC-AddDoc)-rcnh*NewTotN) / (1.0 - rcnh/rcnrr);;
				AddHum = AddC - AddC3 - AddDoc;
				AddC1 = 0.0;
				AddC2 = 0.0;
		
				//NH3 volatilization from slurry application
				float fV;			
				if(manuMethod!=1) fV = 0.3 + 0.25*air_temp/40.0;//fV = 0.1*air_temp/40.0;//
				else fV = 0.25*air_temp/40.0;//0.05				

				SlurryNH3 = fV * UreaN;
				if(SlurryNH3<0.0) SlurryNH3 = 0.0;
				day_vol_nh3 += SlurryNH3;

				nh4[1] += (UreaN - SlurryNH3);	
				
				//for(int k=1;k<=10;k++) sph[k]=7.5;

				for ( l = adp; l <= adp+1; l++ )
				{
					rcvl[l] += (AddC1 * 0.5);
					rcl[l] += (AddC2 * 0.5);
					rcr[l] += (AddC3 * 0.5);
					crhl[l] += AddHum * 0.5;
					//doc[l] += AddHum * 0.5;
					//nh4[l] += AddHum * 0.25 / rcnh;
				}
		}

#endif
///////////////////////////////////////////////////////////////////////////////////////////////

	float Nitrify=0.0;
	float Factor_ph = 1.0;//0.00041+2*0.01221/3.14*2.029/(4*pow((sph[1]-7.5),2)+2.029*2.029);//lena, Lorentz
	float temp_fact = min(1.0, 0.1654 * exp(0.0742*temp[1]));	//Ralf, changed
	float Fr = 1.1;
	float TopNH4 = nh4[1]+nh4[2];

	if(TopNH4>0.0&&temp[1]>0.0&&slurry_doc>0.0) 
	{
		float Ef = 0.01*temp_fact * Factor_ph;//0.003
		if(Ef>1.0) Ef=1.0;
		Nitrify = Ef*(TopNH4)*MicrobioIndex;
		//float fraction;
		//fraction = nh4[l]/(nh4[l]+urea[l]+nh3[l]);
		//float fraction_urea;
		//fraction_urea = urea[l]/(nh4[l]+urea[l]+nh3[l]);
		nh4[1] -= Nitrify*(nh4[1]/TopNH4);// * fraction;
		nh4[2] -= Nitrify*(nh4[2]/TopNH4);
		//urea[l] -= Nitrify * fraction_urea;
		//nh3[l] -= Nitrify * (1.0-fraction-fraction_urea);
	}
	else Nitrify = 0.0;

	no[1] += (Nitrify * 0.4);// * (Fr - 1.0) / Fr;
	n2o[1] += (Nitrify * 0.6);//0.0;//Nitrify / Fr;

	if(slurry_doc>0.000001)
	{
		float Sdoc = slurry_doc * 0.000001;
		float Sco2 = slurry_doc * 0.2;
		day_co2 += Sco2;
		day_decomp[1] += Sco2;
		doc[1] += (0.5 * Sdoc);
		doc[2] += (0.5 * Sdoc);
		slurry_doc -= (Sdoc + Sco2);
		day_DOC_prod += Sdoc;
	}
 
    return( 0 );
}

/*void AddFarmManure(int adp, float CFT, float *MAC, float *MAN, float *MAP, float GoField[3][25],
				   float *rcvl, float *rcl, float * rcr, float * CRB1, float * CRB2, float * crhl, float * crhr, float * dphum, float * nh4, float * no3, float * no2, float * no, float *
				   n2o, float * n2, float * nh3, float * urea, float * co2, float * doc, double water[25][180], float * OrgP, float * LabP, float * ComP,
				   float rcnrvl, float rcnrl, float rcnrr, float rcnb, float rcnh, float rcnm, float *day_DOC_prod)
{
	for(int i=0; i<=2; i++)
	{
		rcvl[adp]  += GoField[i][1] / CFT;//kg C -> kg C/ha
		rcl[adp]  += GoField[i][2] / CFT;
		rcr[adp]  += GoField[i][3] / CFT;
		CRB1[adp]  += GoField[i][4] / CFT;
		CRB2[adp]  += GoField[i][5] / CFT;
		crhl[adp] += GoField[i][6] / CFT;
		crhr[adp] += GoField[i][6] / CFT;
		dphum[adp]  += GoField[i][7] / CFT;
		nh4[adp]  += GoField[i][8] / CFT;
		no3[adp]  += GoField[i][9] / CFT;
		no2[adp]  += GoField[i][10] / CFT;
		no[adp]  += GoField[i][11] / CFT;
		n2o[adp]  += GoField[i][12] / CFT;
		n2[adp]  += GoField[i][13] / CFT;
		nh3[adp]  += GoField[i][14] / CFT;	
		urea[adp]  += GoField[i][15] / CFT;
		co2[adp]  += GoField[i][16] / CFT;
		doc[adp]  += GoField[i][17] / CFT;
		*day_DOC_prod += GoField[i][17] / CFT;
		water[1][adp]  += GoField[i][19] /1000.0 / CFT/10000.0;//m water/ha
		OrgP[adp] += GoField[i][20] / CFT;
		LabP[adp] += GoField[i][21] / CFT;
		ComP[adp] += GoField[i][22] / CFT;

		*MAC += (GoField[i][1]+GoField[i][2]+GoField[i][3]+GoField[i][4]+GoField[i][5]+
			GoField[i][6]+GoField[i][6]+GoField[i][7]+GoField[i][16]+GoField[i][17])/CFT;
		*MAN += (GoField[i][1]/rcnrvl +GoField[i][2]/rcnrl +GoField[i][3]/rcnrr 
			+GoField[i][4]/rcnb +GoField[i][5]/rcnb +
			GoField[i][6]/rcnh+GoField[i][6]/rcnh +GoField[i][7]/rcnm +
			GoField[i][9]+GoField[i][10]+GoField[i][11]
			+GoField[i][12]+GoField[i][13]+GoField[i][8]+GoField[i][14]+GoField[i][15])/CFT;//
		*MAP += (GoField[i][20]+GoField[i][21]+GoField[i][22])/CFT;
	}
}*/

void class_model::alloc_litter_into_pools(float *AddCN, float *AddC, int oz, float *AddC1, float *AddC2, 
										  float *AddC3, float *dInertC, int IfStub, 
										  float rcnrvl, float rcnrl, float rcnrr, float *nh4)
{
	float  RR1, RR2, RR3;
    float a, b, c, d, e, f;
	*dInertC = 0.0;
	float AddN = *AddC / *AddCN;

	if ( *AddCN >= rcnrvl && *AddCN < rcnrl )
	{
		RR1 = rcnrvl;
		RR2 = rcnrl;
		*AddC2 = (AddN - *AddC/RR1) / (1.0/RR2 - 1.0/RR1);
		*AddC1 = (*AddC - *AddC2);
		*AddC3 = 0.0;		

		/*c = (1.0 / *AddCN);
		d = (1.0 / RR1);
		a = c - d;
		e = (1.0 / RR2);
		f = (1.0 / RR1);
		b = e - f;

		*AddC2 = *AddC * (a / b);
		*AddC1 = (*AddC - *AddC2);
		*AddC3 = 0.0;*/
	}
	else if ( *AddCN >= rcnrl && *AddCN <= rcnrr )
	{
		RR2 = rcnrl;
		RR3 = rcnrr;
		c = (1.0 / *AddCN);
		d = (1.0 / RR2);
		a = c - d;
		e = (1.0 / RR3);
		f = (1.0 / RR2);
		b = e - f;
		*AddC3 = (float)(*AddC * (a / b));
		*AddC2 = (float)(*AddC - *AddC3);
		*AddC1 = (float)0.0;
		}
		else if ( *AddCN < rcnrvl )
		{
			float ActN = *AddC/rcnrvl; 
			*AddC3 = 0.0;
			*AddC2 = 0.0;
			*AddC1 = *AddC;
			nh4[1] += (*AddC / (*AddCN+0.0000001) - ActN);
			*AddCN = rcnrvl; 
		}
		else if ( *AddCN > rcnrr )
		{
			//float ActC = *AddC/ *AddCN * rcnrr;
			*AddC3 = *AddC;//ActC;
			*AddC2 = 0.0;
			*AddC1 = 0.0;

			/**dInertC = *AddC - ActC;
			//inert_C[1] += dInertC;
			day_inert_C += *dInertC;
			yr_inert_C += *dInertC;*/
			//*AddC = ActC;
			//*AddCN = rcnrr;
		}

    if (*AddC1 < 0.0) *AddC1 = 0.0;
    if (*AddC2 < 0.0) *AddC2 = 0.0;
    if (*AddC3 < 0.0) *AddC3 = 0.0;

	

/*	if(IfStub==0)
	{
		AddToSoilC = 0.05 * (*AddC1 + *AddC2 + *AddC3 + dInertC);
		AddToSoilN = 0.05* (*AddC1/rcnrvl + *AddC2/rcnrl + *AddC3/rcnrr);
		*AddC1 *= 0.95; 
		*AddC2 *= 0.95;
		*AddC3 *= 0.95;
		dInertC *= 0.95;
	}
	else
	{
		AddToSoilC = dInertC;
		AddToSoilN = 0.0;
		dInertC = 0.0;
	}*/

/*	if(IfStub==1)//litter turn into stub
	{
	}
	else//litter turn into soil C pools
	{
		float AddToSoilC, AddToSoilN;
		AddToSoilC = (*AddC1 + *AddC2 + *AddC3 + dInertC);
		AddToSoilN = (*AddC1/rcnrvl + *AddC2/rcnrl + *AddC3/rcnrr);

		day_addC += AddToSoilC;
		yr_addrc += AddToSoilC;
		day_addrn += AddToSoilN;
		yr_addrn += AddToSoilN;
	}
*/
}

void class_model::alloc_manure_into_pools(float AddCN, float AddC, int oz, float *AddC1, 
										  float *AddC2, float *AddC3, float *dInertC)
{
	/*float FU, TN, ResidueN, ResidueCN, MaxC, NewAddC, NewAddCN;
	
	ResidueN = AddC / (AddCN+0.0000001);

	MaxC = 0.9 * ResidueN * rcnm;

	if(AddC>MaxC)
	{
		AddHum = MaxC;
		NewAddC = AddC - AddHum;
		NewAddCN = NewAddC / (ResidueN - AddHum/rcnm);
	}
	else
	{
		AddHum = 0.9 * AddC;
		NewAddC = AddC - AddHum;
		NewAddCN = NewAddC / (ResidueN - AddHum/rcnm);
	}*/
	
	alloc_litter_into_pools(&AddCN, &AddC, oz, AddC1, AddC2, AddC3, dInertC, 0, rcnrvl, rcnrl, rcnrr, nh4);

    /*if (AddC1 < 0.0) AddC1 = 0.0;
    if (AddC2 < 0.0) AddC2 = 0.0;
    if (AddC3 < 0.0) AddC3 = 0.0;
	if (AddHum < 0.0) AddHum = 0.0;*/
}

float class_model::grazing()
{
   /*int ManureType=0, oz, i, a;
   float dung_CN, Ftt, NH3_emit;
   float C1, C2, C3, N1, N2, N3, urine1, urine2, urine3;
   float fresh_food, dead_food, total_food, food_demand;
   float food_need1, food_need2, food_need3;
   float plant_loss[8], stub_loss, stub1_loss, stub2_loss, stub3_loss;
   float F_dung;
   float sLeaf, sStem, sGrain, sRoot, sPlant;
   float AddC1, AddC2, AddC3, dInertC;
   float old_GrassN, Nratio, sLeaf_N, sStem_N, sGrain_N;

   for ( oz = 1; oz <= gra_num; oz++ )
   {
		if ( jday >= jdgra1[oz] && jday <= jdgra2[oz] && (TotalHeads[oz]>0.0) && (hrs[oz]>0.0))//grazing period
		{
			grazFlag=1;
			H_dairy = AnimalHead[1][oz];
			H_beef = AnimalHead[2][oz];
			H_pig = AnimalHead[3][oz];			
			H_sheep = AnimalHead[4][oz];
			H_horse = AnimalHead[5][oz];
			H_hrs = hrs[oz];

			//Feed available 
			fresh_food = 0.0;
			for(i=1; i<=CropNumbers; i++)
			{
				fresh_food += (Grain_Wt[i] + Leaf_Wt[i] + Stem_Wt[i]); //kg C/ha
			}
			dead_food = stub1 + stub2 + stub3;
			total_food = fresh_food + dead_food;

			//Feed demand
			food_demand = total_C_demand[oz]; //kg C/ha/day
			yr_GrazeDemandC += food_demand;

			//Feed consumption
			float total_grass_consum = 0.0, GrassCNT = 0.0, GPT;

			for(i=1; i<=CropNumbers; i++) GrassCNT += leafcn[i];

			if(total_food > 0.0)
			{
				for(i=1; i<=CropNumbers; i++)
				{
					GPT = (Grain_Wt[i] + Leaf_Wt[i] + Stem_Wt[i]) / fresh_food; 
					if (food_demand <= total_food)
					{					
						  day_grass_consume[i] = food_demand * GPT;
						  plant_loss[i] = day_grass_consume[i] * fresh_food / (fresh_food + dead_food);
						  stub_loss = day_grass_consume[i] * dead_food / (fresh_food + dead_food);		
					}
					else
					{
						  day_grass_consume[i] = total_food * GPT;
						  plant_loss[i] = fresh_food * GPT;
						  stub_loss = dead_food * GPT;
					}

					total_grass_consum += day_grass_consume[i];
				}
			}
			else
			{
				for(i=1; i<=CropNumbers; i++)
				{
					day_grass_consume[i] = 0.0;
					plant_loss[i] = 0.0;
					stub_loss = 0.0;
				}
			}

			if(food_demand<=0.0) day_deficit = 1.0;
			else day_deficit = total_grass_consum / food_demand;

			//Grazed plant C
			for(int p=1; p<=CropNumbers; p++)
			{
				sPlant = Grain_Wt[p] + Leaf_Wt[p] + Stem_Wt[p];

				float GQI = base_leafcn[p];

				if(sPlant>0.0)
				{
					float FDF = plant_loss[p] / fresh_food;
					sLeaf = FDF * Leaf_Wt[p];
					sStem = FDF * Stem_Wt[p];
					sGrain = FDF * Grain_Wt[p];
				}
				else
				{
					sLeaf = 0.0;
					sStem = 0.0;
					sGrain = 0.0;					
				}

				Leaf_Wt[p] -= sLeaf;
				Stem_Wt[p] -= sStem;
				Grain_Wt[p] -= sGrain;

				sLeaf_N = sLeaf / leafcn[p];
				sStem_N = sStem / stemcn[p];
				sGrain_N = sGrain / graincn[p];

				float sPlantP = CropP[p] * 0.01 * sPlant / 0.4;
				PlantP[p] -= sPlantP;

				old_GrassN = plantn[p];
				plantn[p] -= (sLeaf_N+sStem_N+sGrain_N);
				if(plantn[p] < 0.0) plantn[p] = 0.0;
				Nratio = plantn[p] / old_GrassN;

				day_GrazeC += (sLeaf + sStem + sGrain);
				day_GrazeN += (sLeaf_N + sStem_N + sGrain_N);

				yr_GrazeGrain[p] += sGrain;
				yr_GrazeShoot[p] += (sLeaf+sStem);

			}//end of p loop

			//Grazed stub
			if(dead_food > 0.000001)
			{
				stub1_loss = stub_loss * stub1 / dead_food;
				stub2_loss = stub_loss * stub2 / dead_food;
				stub3_loss = stub_loss * stub3 / dead_food;
				stub1 -= stub1_loss;
				stub2 -= stub2_loss;
				stub3 -= stub3_loss;
			}
			else
			{
				stub1_loss = 0.0;
				stub2_loss = 0.0;
				stub3_loss = 0.0;
			}

			day_GrazeC += stub_loss;//kg C/ha/day
			day_GrazeN += (stub1_loss/rcnrvl +  stub2_loss/ rcnrl + stub3_loss / rcnrr);//kg N/ha/day
			yr_GrazeStub += stub_loss;	
			
			//Intaken C and N partitioned to animal C and N fluxes
			for(a=1; a<=5; a++) //1 dairy, 2 beef, 3 swine, 4 sheep, 5 horse, 6 poultry
			{
				day_graze_intake_C[a] = day_GrazeC * livestock_C_demand[a] / total_C_demand[oz];
				day_graze_intake_N[a] = day_GrazeN * livestock_C_demand[a] / total_C_demand[oz];

				day_graze_milk_C[a] = day_graze_intake_C[a] * Milk_C_fraction[a];
				day_graze_milk_N[a] = day_graze_intake_N[a] * Milk_N_fraction[a];
				day_graze_meat_C[a] = day_graze_intake_C[a] * Meat_C_fraction[a];
				day_graze_meat_N[a] = day_graze_intake_N[a] * Meat_N_fraction[a];

				day_graze_urine_C[a] = day_graze_intake_C[a] * Urine_C_fraction[a];
				day_graze_urine_N[a] = day_graze_intake_N[a] * Urine_N_fraction[a];
				day_graze_dung_C[a] = day_graze_intake_C[a] * Dung_C_fraction[a];
				day_graze_dung_N[a] = day_graze_intake_N[a] * Dung_N_fraction[a];

				day_graze_dung_P[a] = NPratio[a] * (day_graze_dung_N[a] + day_graze_urine_N[a]);//kg P/ha

				day_graze_enteric_CH4[a] = day_graze_intake_C[a] * Enteric_CH4_C_fraction[a];
				day_graze_enteric_N2O[a] = day_graze_intake_N[a] * Enteric_N2O_N_fraction[a];
				day_graze_respiration_CO2[a] = day_graze_intake_C[a] * Respiration_C_fraction[a];

				yr_graze_dung_C += day_graze_dung_C[a];
				yr_graze_dung_N += day_graze_dung_N[a];
				yr_graze_urine_C += day_graze_urine_C[a];
				yr_graze_urine_N += day_graze_urine_N[a];
				yr_graze_urine_P += 0.0;
				yr_graze_dung_P += day_graze_dung_P[a];

				yr_addmc += (day_graze_dung_C[a] + day_graze_urine_C[a]);
				//grazedN += day_graze_intake_N[a];
			}
			
			//incorporate urine and dung into soil
			float manure_add_N=0.0;
			for(a=1; a<=5; a++)
			{
				doc[1] += day_graze_urine_C[a];
				urea[1] += day_graze_urine_N[a];

				float TN = day_graze_dung_N[a];
				float TC = day_graze_dung_C[a];
			
				float NewTotN = TN;
				int FF = (int)(NewTotN / rcnh);

				float AddHumN = 0.5 * (float)FF * rcnh;
				float LeftTotN = NewTotN - AddHumN;
				AddHum = AddHumN * rcnh;

				float NewAddC = TC - AddHum;
				float NewAddCN = NewAddC / (LeftTotN + 0.0000001);

				alloc_manure_into_pools(NewAddCN, NewAddC, 1, &AddC1, &AddC2, &AddC3, &dInertC);

				manure_add_N = (AddC1 / rcnrvl + AddC2 / rcnrl + AddC3 / rcnrr + AddHum / rcnh + day_graze_urine_N[a]);
				day_inManureN += manure_add_N;
				yr_addmn += manure_add_N;

				rcvl[1] += AddC1;
				rcl[1] += AddC2;
				rcr[1] += AddC3;
				crhr[1] += AddHum;
				OrgP[1] += day_graze_dung_P[a];
				
			}
		}//end of if grazing period
   }//end of gra_num*/

   int ManureType=0, oz, i, a;
   float dung_CN, Ftt, NH3_emit;
   float C1, C2, C3, N1, N2, N3, urine1, urine2, urine3;
   float fresh_food, dead_food, total_food, food_demand, FoodDemand;
   float food_need1, food_need2, food_need3;
   float plant_loss[8], stub1_loss, stub2_loss, stub3_loss;
   float F_dung;
   float sLeaf, sStem, sGrain, sRoot, sPlant;
   float AddC1, AddC2, AddC3, dInertC;
   float old_GrassN, Nratio, sLeaf_N, sStem_N, sGrain_N;
   float AdditionFeed, FedC=0.0;//, IniFoodDemand=0.0, IniFreshFood, IniDeadFood;

   for ( oz = 1; oz <= gra_num; oz++ )
   {
	   int ExcretaFlag=0;
	   if(Excreta[oz]==2) ExcretaFlag = 1;//manure removal

		if ( jday >= jdgra1[oz] && jday <= jdgra2[oz] && (TotalHeads[oz]>0.0) && (hrs[oz]>0.0))//grazing period
		{
			grazFlag=1;
			H_dairy = AnimalHead[1][oz];
			H_beef = AnimalHead[2][oz];
			H_pig = AnimalHead[3][oz];			
			H_sheep = AnimalHead[4][oz];
			H_horse = AnimalHead[5][oz];
			H_hrs = hrs[oz];

			if(AddFeed[oz]>15.0) 
			{
				//ResidueFeed += (AddFeed[oz] - 15.0);
				AddFeed[oz] = 15.0;
			}

			AdditionFeed = AddFeed[oz] * (H_dairy+H_beef+H_pig+H_sheep+H_horse);

			//Feed available 
			fresh_food = 0.0;
			for(i=1; i<=CropNumbers; i++)
			{
				fresh_food += (Grain_Wt[i] + Leaf_Wt[i] + Stem_Wt[i]); //kg C/ha
			}
			dead_food = stub1 + stub2 + stub3;
			total_food = fresh_food + dead_food + AdditionFeed;

			//Feed demand
			FoodDemand = total_C_demand[oz]; //kg C/ha/day
			food_demand = total_C_demand[oz]; //kg C/ha/day
			yr_GrazeDemandC += food_demand;

			//Feed consumption
			float total_grass_consum = 0.0, GrassCNT = 0.0, GPT=0.0;
			float fresh_grass_consume=0.0, stub_grass_consume=0.0, fresh_grass[8]={0.0,0.0};
			float consumed_grain[8]={0.0,0.0}, consumed_leaf[8]={0.0,0.0}, consumed_stem[8]={0.0,0.0};

			for(i=1; i<=CropNumbers; i++) GrassCNT += leafcn[i];

			if(total_food > 0.0)
			{
				if(fresh_food>0.0) 
				{
					for(i=1; i<=CropNumbers; i++) 
					{
						fresh_grass[i] = Grain_Wt[i] + Leaf_Wt[i] + Stem_Wt[i];
						GPT = fresh_grass[i] / fresh_food; 
					}
				}
			
				if(AdditionFeed>=food_demand)
				{
					FedC = food_demand;
					AdditionFeed -= food_demand;
					day_GrazeC += food_demand;
					day_GrazeN += (food_demand / FeedCN[oz]);
					//day_GrazeAddFeed = food_demand;
					food_demand = 0.0;
				}
				else
				{
					FedC = AdditionFeed;
					food_demand -= AdditionFeed;
					day_GrazeC += AdditionFeed;
					day_GrazeN += (AdditionFeed / FeedCN[oz]);
					//day_GrazeAddFeed = AdditionFeed;
					AdditionFeed = 0.0;
				}

				if(fresh_food>=food_demand) //enough fresh grass
				{
					fresh_grass_consume = food_demand;
					food_demand = 0.0;
					fresh_food -= fresh_grass_consume;
				}
				else
				{
					fresh_grass_consume = fresh_food;
					fresh_food = 0.0;
					food_demand -= fresh_grass_consume;
				}

				for(i=1; i<=CropNumbers; i++)
				{
					if(fresh_grass[i]>0.0)
					{
						consumed_grain[i] = fresh_grass_consume * GPT * (Grain_Wt[i] / fresh_grass[i]);
						consumed_leaf[i] = fresh_grass_consume * GPT * (Leaf_Wt[i] / fresh_grass[i]);
						consumed_stem[i] = fresh_grass_consume * GPT * (Stem_Wt[i] / fresh_grass[i]);
						Grain_Wt[i] -= consumed_grain[i];
						Leaf_Wt[i] -= consumed_leaf[i];
						Stem_Wt[i] -= consumed_stem[i];
					}
				}
				
				//////////////////////
				stub1_loss=0.0;
				stub2_loss=0.0;
				stub3_loss=0.0;
			
				if(food_demand>0.0&&dead_food>0.0) 
				{
					if(dead_food>=food_demand)
					{
						stub_grass_consume = food_demand;
						food_demand = 0.0;
						dead_food -= stub_grass_consume;
					}
					else
					{
						stub_grass_consume = dead_food;
						dead_food = 0.0;
						food_demand -= stub_grass_consume;
					}

					float stb = stub_grass_consume;
					if(stb>=stub1)
					{
						stb -= stub1;
						stub1_loss = stub1;
						stub1 = 0.0;
						 
					}
					else
					{
						stub1 -= stb;
						stub1_loss = stb;
						stb = 0.0;
					}

					if(stb>=stub2)
					{
						stb -= stub2;
						stub2_loss = stub2;
						stub2 = 0.0;
					}
					else
					{
						stub2 -= stb;
						stub2_loss = stb;
						stb = 0.0;
					}

					if(stb>=stub3)
					{
						stb -= stub3;
						stub3_loss = stub3;
						stub3 = 0.0;
					}
					else
					{
						stub3 -= stb;
						stub3_loss = stb;
						stb = 0.0;
					}
				}
			}
			else
			{
				fresh_grass_consume = 0.0;
				stub_grass_consume = 0.0;	
				stub1_loss=0.0;
				stub2_loss=0.0;
				stub3_loss=0.0;
			}

			total_grass_consum = fresh_grass_consume + stub_grass_consume;
			if(FoodDemand<=0.0) day_deficit = 1.0;
			else day_deficit = total_grass_consum / FoodDemand;

			//Grazed plant C
			for(int p=1; p<=CropNumbers; p++)
			{
				float GQI = base_leafcn[p];

				sPlant = consumed_grain[p] + consumed_leaf[p] + consumed_stem[p];
				sLeaf = consumed_leaf[p];
				sStem = consumed_stem[p];
				sGrain = consumed_grain[p];
				
				sLeaf_N = sLeaf / leafcn[p];
				sStem_N = sStem / stemcn[p];
				sGrain_N = sGrain / graincn[p];

				float sPlantP = CropP[p] * 0.01 * sPlant / 0.4;
				PlantP[p] -= sPlantP;

				old_GrassN = plantn[p];
				plantn[p] -= (sLeaf_N+sStem_N+sGrain_N);
				if(plantn[p] < 0.0) plantn[p] = 0.0;
				Nratio = plantn[p] / old_GrassN;

				day_GrazeC += (sLeaf + sStem + sGrain);
				day_GrazeN += (sLeaf_N + sStem_N + sGrain_N);

				yr_GrazeGrain[p] += sGrain;
				yr_GrazeShoot[p] += (sLeaf+sStem);

			}//end of p loop

			//Grazed stub
			day_GrazeC += stub_grass_consume;//kg C/ha/day
			day_GrazeN += (stub1_loss/rcnrvl +  stub2_loss/ rcnrl + stub3_loss / rcnrr);//kg N/ha/day
			yr_GrazeStub += (stub1_loss+stub2_loss+stub3_loss);	
				
			//Intaken C and N partitioned to animal C and N fluxes
			for(a=1; a<=5; a++) //1 dairy, 2 beef, 3 swine, 4 sheep, 5 horse, 6 poultry
			{
				day_graze_intake_C[a] = day_GrazeC * livestock_C_demand[a][oz] / total_C_demand[oz];
				day_graze_intake_N[a] = day_GrazeN * livestock_C_demand[a][oz] / total_C_demand[oz];

				day_graze_milk_C[a] = day_graze_intake_C[a] * Milk_C_fraction[a];
				day_graze_milk_N[a] = day_graze_intake_N[a] * Milk_N_fraction[a];
				day_graze_meat_C[a] = day_graze_intake_C[a] * Meat_C_fraction[a];
				day_graze_meat_N[a] = day_graze_intake_N[a] * Meat_N_fraction[a];

				day_graze_urine_C[a] = day_graze_intake_C[a] * Urine_C_fraction[a];
				day_graze_urine_N[a] = day_graze_intake_N[a] * Urine_N_fraction[a];
				day_graze_dung_C[a] = day_graze_intake_C[a] * Dung_C_fraction[a];
				day_graze_dung_N[a] = day_graze_intake_N[a] * Dung_N_fraction[a];

				day_graze_dung_P[a] = NPratio[a] * (day_graze_dung_N[a] + day_graze_urine_N[a]);//kg P/ha

				day_graze_enteric_CH4[a] = day_graze_intake_C[a] * Enteric_CH4_C_fraction[a];
				day_graze_enteric_N2O[a] = day_graze_intake_N[a] * Enteric_N2O_N_fraction[a];
				day_graze_respiration_CO2[a] = day_graze_intake_C[a] * Respiration_C_fraction[a];

				yr_graze_dung_C += day_graze_dung_C[a];
				yr_graze_dung_N += day_graze_dung_N[a];
				yr_graze_urine_C += day_graze_urine_C[a];
				yr_graze_urine_N += day_graze_urine_N[a];
				yr_graze_urine_P += 0.0;
				yr_graze_dung_P += day_graze_dung_P[a];

				yr_addmc += (day_graze_dung_C[a] + day_graze_urine_C[a]);
				//grazedN += day_graze_intake_N[a];
			}
		
			//incorporate urine and dung into soil
			if(ExcretaFlag == 0)
			{
				float manure_add_N=0.0;
				for(a=1; a<=5; a++)
				{
					doc[1] += day_graze_urine_C[a];
					day_DOC_prod += day_graze_urine_C[a];
					urea[1] += day_graze_urine_N[a];

					float TN = day_graze_dung_N[a];
					float TC = day_graze_dung_C[a];
				
					float NewTotN = TN;
					int FF = (int)(NewTotN / rcnh);

					float AddHumN = 0.5 * (float)FF * rcnh;
					float LeftTotN = NewTotN - AddHumN;
					AddHum = AddHumN * rcnh;

					float NewAddC = TC - AddHum;
					float NewAddCN = NewAddC / (LeftTotN + 0.0000001);

					alloc_manure_into_pools(NewAddCN, NewAddC, 1, &AddC1, &AddC2, &AddC3, &dInertC);

					manure_add_N = (AddC1 / rcnrvl + AddC2 / rcnrl + AddC3 / rcnrr + AddHum / rcnh + day_graze_urine_N[a]);
					day_inManureN += manure_add_N;
					yr_addmn += manure_add_N;

					rcvl[1] += AddC1;
					rcl[1] += AddC2;
					rcr[1] += AddC3;
					crhr[1] += AddHum;
					OrgP[1] += day_graze_dung_P[a];				
				}
			}
		}//end of if grazing period
   }//end of gra_num
			
/* Observed data from Donna Giltrap in New Zealand
	For cows:
	Daily consumption: 6 kg C/head/day, 0.3 kg N/head/day
	Total daily N deposit = 0.29 kg N/head/day
	Total daily C deposit = 1.943 kg C/cow

	For sheep:
	Daily consumption: 0.85 kg C/head/day, 0.0425 kg N/head/day
	Total daily N deposit = 0.04 kg/ewe
	Total daily C deposit = 0.268 kg/ewe

	In both cases the nitrogen in the dung is 40% of the total N, the C/N
	ratio for dung is 16 and the C/N ratio for urine is 0.5.*/	

   return(0);
}

float class_model::cutting()
{
	int oz, i;
	float LeftGrass, Rr;
	float day_CutGrain=0.0, day_CutLeaf=0.0, day_CutStem=0.0, day_CutRoot=0.0;
	
	//All cut materials will be taken out from the field
	for ( oz = 1; oz <= cut_num; oz++ )
	{
		if ( jday==jdcut[oz])
		{
			for(i=1; i<=1; i++)//only crop 1 is cut
			{
			
				if(CutPart[oz]==0)//nothing cut
				{
				}
				else if(CutPart[oz]==1)//fruit cut
				{
					day_CutGrain = Grain_Wt[i] * CutFraction[oz];
					Grain_Wt[i] -= day_CutGrain;
				

				}
				else if(CutPart[oz]==2)//leaf cut
				{
					day_CutLeaf = Leaf_Wt[i] * CutFraction[oz];
					Leaf_Wt[i] -= day_CutLeaf;
					
				}
				else if(CutPart[oz]==3)//stem cut
				{
					day_CutStem = Stem_Wt[i] * CutFraction[oz];
					Stem_Wt[i] -= day_CutStem;
					
				}
				else if(CutPart[oz]==4)//root cut
				{
					day_CutRoot = Root_Wt[i] * CutFraction[oz];
					Root_Wt[i] -= day_CutRoot;
				
				}
				else if(CutPart[oz]==5)//fruit+leaf cut
				{
					day_CutGrain = Grain_Wt[i] * CutFraction[oz];
					Grain_Wt[i] -= day_CutGrain;
					day_CutLeaf = Leaf_Wt[i] * CutFraction[oz];
					Leaf_Wt[i] -= day_CutLeaf;

					
				}
				else if(CutPart[oz]==6)//grain+leaf+stem cut
				{
					day_CutGrain = Grain_Wt[i] * CutFraction[oz];
					Grain_Wt[i] -= day_CutGrain;
					day_CutLeaf = Leaf_Wt[i] * CutFraction[oz];
					Leaf_Wt[i] -= day_CutLeaf;
					day_CutStem = Stem_Wt[i] * CutFraction[oz];
					Stem_Wt[i] -= day_CutStem;

					
				}
				else if(CutPart[oz]==7)//grain+leaf+stem+root cut
				{
					day_CutGrain = Grain_Wt[i] * CutFraction[oz];
					Grain_Wt[i] -= day_CutGrain;
					day_CutLeaf = Leaf_Wt[i] * CutFraction[oz];
					Leaf_Wt[i] -= day_CutLeaf;
					day_CutStem = Stem_Wt[i] * CutFraction[oz];
					Stem_Wt[i] -= day_CutStem;
					day_CutRoot = Root_Wt[i] * CutFraction[oz];
					Root_Wt[i] -= day_CutRoot;
					
				}
				else if(CutPart[oz]==8)//leaf+stem cut
				{
					day_CutLeaf = Leaf_Wt[i] * CutFraction[oz];
					Leaf_Wt[i] -= day_CutLeaf;
					day_CutStem = Stem_Wt[i] * CutFraction[oz];
					Stem_Wt[i] -= day_CutStem;
					
				}

				DayGrassCut = day_CutGrain+day_CutLeaf+day_CutStem+day_CutRoot;			

	/*			LeftGrass = TotalGrass - DayGrassCut;
				if(TotalGrass==0.0) Rr = 0.0;
				else Rr = LeftGrass / TotalGrass;

				day_CutGrain = Grain_Wt[i] *(1.0-Rr);
				day_CutLeaf = Leaf_Wt[i] *(1.0-Rr);
				day_CutStem = Stem_Wt[i] *(1.0-Rr);
				day_CutRoot = 0.0;//Root_Wt[i] *(1.0-Rr);
				remain_root = Root_Wt[i];			

				Grain_Wt[i] *= Rr;
				Leaf_Wt[i] *= Rr;
				Stem_Wt[i] *= Rr;
				//Root_Wt[i] *= Rr;*/

				plantn[i] -= (day_CutGrain/graincn[i]+day_CutLeaf/leafcn[i]+day_CutStem/stemcn[i]);//+day_CutRoot/rootcn[i]);

				yr_CutGrain[i] += day_CutGrain;
				yr_CutLeaf[i] += day_CutLeaf;
				yr_CutStem[i] += day_CutStem;
				yr_CutRoot[i] += day_CutRoot;

				if(LandUse==6) stem_age = 0;
			}
		}
		else
		{
			day_CutGrain = 0.0;
			day_CutLeaf = 0.0;
			day_CutStem = 0.0;
			day_CutRoot = 0.0;
		}

	}
	return(0);
}

float class_model::PlasticCover()
{
    int oz;

	if(plastic_num > 0)
	{
		for (oz=1; oz<=plastic_num; oz++)
		{
			if ( jday == jdplastic1[oz])//start film mulching
			{
				plastic_flag = 1;
				FilmEffect = 1.0 * (float)log(FilmCoverF[oz] + 1.0);
				if(FilmEffect>1.0) FilmEffect = 1.0;
				FilmCoverFraction = FilmCoverF[oz];
			}

			if ( jday == jdplastic2[oz])//end film mulching
			{
				plastic_flag = 0;
				FilmEffect = 0.0;
				FilmCoverFraction = 0.0;
			}
		}		
	}
	else
	{
		plastic_flag = 0;
		FilmEffect = 0.0;
		FilmCoverFraction = 0.0;
	}


    return( 0 );
}


void class_model::day_flooding(int MODE)
{
 	if(day_WT>0.0)//||flood_flag==0)
	{
		flood_flag = 1;
		FloodingDays++;
		if(drain_flag>0) drain_flag--;
		
	}
	else
	{
		if(FloodingDays>0) drain_flag = 5;

		if(flood_flag<0)
		{
			flood_flag++;
			if(flood_flag>0) flood_flag = 0;
		}
		//flood_flag = 0;
		FloodingDays = 0;	
	}
	
}

void class_model::tree_leaves_fall()
{
    float leaf_falln, leaf_fallc, AddC, AddCN;
	float AddC1, AddC2, AddC3, dInertC;
    int   l, oz=0;

    if ( crop[1] == 17 )
    {
	leaf_falln = day_nup[1] * leaf[1];
	leaf_fallc = leaf_falln * leafcn[1];
    AddC = leaf_fallc;
	AddCN = leafcn[1];

	/* #22-1, Add new Litter into soil litter pools */
	alloc_litter_into_pools(&AddCN, &AddC, oz, &AddC1, &AddC2, &AddC3, &dInertC, 0, rcnrvl, rcnrl, rcnrr, nh4);

	for ( l = 1; l <= 2; l++ )
	{
	    rcvl[l] += (AddC1 * 0.5);
	    rcl[l] += (AddC2 * 0.5);
	    rcr[l] += (AddC3 * 0.5);
	}

	yr_addleafc = yr_addleafc + AddC1 +
			       AddC2 + AddC3;
	yr_addleafn = yr_addleafn +
			       AddC1 / rcnrvl +
			       AddC2 / rcnrl +
			       AddC3 / rcnrr;
	AddC1 = 0.0;
	AddC2 = 0.0;
	AddC3 = 0.0;

	/* leaf_falln = 0;
	leaf_fallc = 0; */

    }
}

float class_model::tilling(int* tilq_p, float *CRB1, float* CRB2,
		float *newh, float till_fact)//float* rhrl_p, 
{
    int    oz, l, tilq, cwc;
    float rcvl_xyz=0.0, rcl_xyz=0.0, rcr_xyz=0.0, crb1_xyz=0.0, inertC_xyz=0.0;
    float crb2_xyz=0.0, crh1_xyz=0.0, crh2_xyz=0.0,dp_xyz=0.0, newh_xyz=0.0;
    float tc_xyz=0.0, no3_xyz=0.0, nh4_xyz=0.0, clayn_xyz=0.0;
    float urea_xyz=0.0, nh3_xyz=0.0, NH4HCO3_xyz=0.0, OrgP_xyz=0.0, AdsP_xyz=0.0, LabP_xyz=0.0;	
	float AddC, AddCN, AddC1, AddC2, AddC3, dInertC, harvest_AddC;

   
    tilq = *tilq_p;
    for (oz=1; oz<=til_num; oz++)
    {
		if (jday==jdtil[oz])
		{
			//terminating crop
			if(crop_terminator==1)
			{
				for(cwc=1; cwc<=CropNumbers; cwc++)
				{
					if(Root_Wt[cwc]>0.0)
					{
						AddC = Root_Wt[cwc];
						AddCN = rootcn[cwc];
						alloc_litter_into_pools(&AddCN, &AddC, oz, &AddC1, &AddC2, &AddC3, &dInertC, 0, rcnrvl, rcnrl, rcnrr, nh4);
						float AddP = CropP[cwc] * 0.01 * AddC / 0.4;

						for ( l = 1; l <= q; l++ )
						{
							if ( l >= 1 && l <= 3 )
							{
								rcvl[l] += AddC1 * 0.5 / 3.0;
								rcl[l] += AddC2 * 0.5 / 3.0;
								rcr[l] += AddC3 * 0.5 / 3.0;
								inert_C[l] += day_inert_C * 0.5 / 3.0;
								OrgP[l] += AddP * 0.5 / 3.0;
							}
							if ( l >= 4 && l <= 6 )
							{
								rcvl[l] += AddC1 * 0.4 / 3.0;
								rcl[l] += AddC2 * 0.4 / 3.0;
								rcr[l] += AddC3 * 0.4 / 3.0;
								inert_C[l] += day_inert_C * 0.4 / 3.0;
								OrgP[l] += AddP * 0.4 / 3.0;
							}
							if ( l >= 7 && l <= 9 )
							{
								rcvl[l] += AddC1 * 0.1 / 3.0;
								rcl[l] += AddC2 * 0.1 / 3.0;
								rcr[l] += AddC3 * 0.1 / 3.0;
								inert_C[l] += day_inert_C * 0.1 / 3.0;
								OrgP[l] += AddP * 0.1 / 3.0;
							}
						}

						float AddToSoilC, AddToSoilN;
						AddToSoilC = AddC1 + AddC2 + AddC3 + dInertC;
						AddToSoilN = AddC1/rcnrvl + AddC2/rcnrl + AddC3/rcnrr;

						day_addC += AddToSoilC;
						yr_addrc += AddToSoilC;
						day_addrn += AddToSoilN;
						yr_addrn += AddToSoilN;

						float RootAddC = AddC1+AddC2+AddC3;
						harvest_AddC += RootAddC;
						AddC1 = 0.0;
						AddC2 = 0.0;
						AddC3 = 0.0;

						Root_Wt[cwc]=0.0;
						Grain_Wt[cwc]=0.0;
						Leaf_Wt[cwc]=0.0;
						Stem_Wt[cwc]=0.0;
						PerennialFlag[cwc]=0;
						crop_terminator=0;
						crop[cwc] = 0;
					}
				}
			}


			/////////////////////////////////////////////////////////////////
			till_flag = 1;
			
			tilld = til_dept[oz];

			tilq = (int)(til_dept[oz] / h[1])+1;//????
			tilq = min(q, max(0, tilq));
			
			if(tilq>0) 
			{				
				rcvl_xyz=0.0;
				rcl_xyz=0.0;
				rcr_xyz=0.0;
				crb1_xyz=0.0;
				crb2_xyz=0.0;
				crh1_xyz=0.0;
				crh2_xyz=0.0;
				dp_xyz=0.0;
				inertC_xyz=0.0;
				newh_xyz=0.0;
				tc_xyz=0.0;
				no3_xyz=0.0;
				nh4_xyz=0.0;
				clayn_xyz=0.0;
				urea_xyz=0.0;
				nh3_xyz=0.0;
				NH4HCO3_xyz=0.0;
				OrgP_xyz=0.0;
				AdsP_xyz=0.0;
				LabP_xyz=0.0;

				for ( l = 1; l <= tilq; l++ )
				{
					rcvl_xyz += rcvl[l];
					rcl_xyz += rcl[l];
					rcr_xyz += rcr[l];
					crb1_xyz += CRB1[l];
					crb2_xyz += CRB2[l];
					crh1_xyz += crhl[l];
					crh2_xyz += crhr[l];
					dp_xyz += dphum[l];
					inertC_xyz += inert_C[l];
					newh_xyz += newh[l];
					tc_xyz += doc[l];
					no3_xyz += no3[l];
					nh4_xyz += nh4[l];
					clayn_xyz += clay_nh4[l];
					urea_xyz += urea[l];
					nh3_xyz += nh3[l];
					NH4HCO3_xyz += NH4HCO3[l];
					OrgP_xyz += OrgP[l];
					AdsP_xyz += AdsP[l];
					LabP_xyz += LabP[l];
				}

				if ((stub1+stub2+stub3) > 0.0 )
				{
					yr_addtc += (stub1 + stub2 + stub3);
					yr_addtn += (stub1 / rcnrvl + stub2 / rcnrl + stub3 / rcnrr);
					day_addC += (stub1 + stub2 + stub3);
					day_inStubN += (stub1 / rcnrvl + stub2 / rcnrl + stub3 / rcnrr);

					if(til_method[oz]==6)
					{
						rcvl[tilq+1] += stub1;
						rcl[tilq+1] += stub2;
						rcr[tilq+1] += stub3;
						OrgP[tilq+1] += stubP;
					}
					else
					{
						if(tilq<5)
						{
							float qst = 0.2;
							rcvl_xyz += ((1.0-qst)*stub1);
							rcl_xyz += ((1.0-qst)*stub2);
							rcr_xyz +=((1.0-qst)*stub3);
							OrgP_xyz += ((1.0-qst)*stubP);

							stub1 *= qst;
							stub2 *= qst;
							stub3 *= qst;
							stubP *= qst;
						}
						else
						{
							rcvl_xyz += stub1;
							rcl_xyz += stub2;
							rcr_xyz += stub3;
							OrgP_xyz += stubP;

							stub1 = 0.0;
							stub2 = 0.0;
							stub3 = 0.0;
							stubP = 0.0;
						}
					}
					
					stub1 = 0.0;
					stub2 = 0.0;
					stub3 = 0.0;
					stubP = 0.0;
				}
			
				rcvl[1] = rcvl_xyz / tilq;
				rcl[1] = rcl_xyz / tilq;
				rcr[1] = rcr_xyz / tilq;
				CRB1[1] = crb1_xyz / tilq;
				CRB2[1] = crb2_xyz / tilq;			
				crhl[1] = crh1_xyz / tilq;
				crhr[1] = crh2_xyz / tilq;
				dphum[1] = dp_xyz / tilq;
				inert_C[1] = inertC_xyz / tilq;
				newh[1] = newh_xyz / tilq;
				doc[1] = tc_xyz / tilq;
				no3[1] = no3_xyz / (float)tilq;
				nh4[1] = nh4_xyz / tilq;
				nh3[1] = nh3_xyz / tilq;
				urea[1] = urea_xyz / tilq;
				NH4HCO3[1] = NH4HCO3_xyz / tilq;
				clay_nh4[1] = clayn_xyz / tilq;
				OrgP[1] = OrgP_xyz / tilq;
				AdsP[1] = AdsP_xyz / tilq;
				LabP[1] = LabP_xyz / tilq;			

				ocini[1] = CRB1[1] + CRB2[1] + crhl[1] + crhr[1];

				float CFT=0.0;// 0.0001*air_temp;
				if(CFT<0.0) CFT=0.0;
				for ( l = 1; l <= tilq; l++ )
				{
					rcvl[l] = rcvl[1];
					rcl[l] = rcl[1];
					rcr[l] = rcr[1];
					CRB1[l] = CRB1[1];
					CRB2[l] = CRB2[1];				
					crhl[l] = crhl[1];
					crhr[l] = crhr[1];
					dphum[l] = dphum[1];
					inert_C[l] = inert_C[1];
					newh[l] = newh[1];
					doc[l] = doc[1];
					no3[l] = no3[1];
					nh4[l] = nh4[1];
					nh3[l] = nh3[1];
					urea[l] = urea[1];
					NH4HCO3[l] = NH4HCO3[1];
					clay_nh4[l] = clay_nh4[1];
					ocini[l] = ocini[1];
					OrgP[l] = OrgP[1];
					AdsP[l] = AdsP[1];
					LabP[l] = LabP[1];

					float dHumus = dphum[l] * CFT;
					dphum[l] -= dHumus;
					crhr[l] += dHumus;
				}
				
				if ( tilld < 0.00001 ) till_fact = 1.0;
				else if ( fabs(tilld - 0.05) < 0.00001 ) till_fact = 3.0;//4.5;//3
				else if ( fabs(tilld - 0.1) < 0.00001 ) till_fact = 4.0;//5.0;//4
				else if ( fabs(tilld - 0.2) < 0.00001 ) till_fact = 5.0;//7.0;//5
				else if ( fabs(tilld - 0.3) < 0.00001 ) till_fact = 6.0;//9.0;//6
				else till_fact = 1.0;
			}
		}
    }

	if(TTT>1.0) till_fact -= 0.1;
	else till_fact -= 0.01;
	if(till_fact <= 1.0) till_fact = 1.0;

    *tilq_p = tilq;
 
    return( till_fact );
}

void class_model::stub_fall()
{
    if ( stub1 + stub2 + stub3 > 0.0 )
    {
		float yy, dep1, dep2, dep3;
		//if(air_temp>0.0) yy = 0.000001*air_temp;
		//else yy = 0.0;
		
		yy = 0.001;//0.005;

		dep1 = stub1 * yy;
		dep2 = stub2 * yy;
		dep3 = stub3 * yy;

		yr_addtc += (dep1 + dep2 + dep3);
		day_addC += (dep1 + dep2 + dep3);
		yr_addtn += (dep1 / rcnrvl + dep2 / rcnrl + dep3 / rcnrr);

		rcvl[1] += dep1;
		rcl[1] += dep2;
		rcr[1] += dep3;

		stub1 *= (1.0 - yy);
		stub2 *= (1.0 - yy);
		stub3 *= (1.0 - yy);
    }
}


void class_model::crop_harvesting(int scale, int ccwc)
{
/*    int    l, oz=0;
    float  AddC, AddCN;
    float  Stuble;
    float haradd2, harvest_AddC=0.0;
	float AddC1, AddC2, AddC3, dInertC;

	if(crop[ccwc]==20||crop[ccwc]==46) 
	{
		IrriRice_flag = 0;
		ShallowFloodFlag = 0;
		flood_flag = 0;
	}

	HarvestDay[ccwc] = jday;

	CropEndN[ccwc] = plantn[ccwc];
	MaxLeaf[ccwc] += Leaf_Wt[ccwc];
	MaxStem[ccwc] += Stem_Wt[ccwc];
	MaxRoot[ccwc] += Root_Wt[ccwc];
	MaxGrain[ccwc] += Grain_Wt[ccwc];
	
	PresentBiomass[ccwc] = Root_Wt[ccwc] + Leaf_Wt[ccwc] + Stem_Wt[ccwc] + Grain_Wt[ccwc];
	ProdC[ccwc] = PresentBiomass[ccwc];
	YrG_grain[ccwc] = Grain_Wt[ccwc];
	YrG_leaf[ccwc] = Leaf_Wt[ccwc];
	YrG_stem[ccwc] = Stem_Wt[ccwc];
	YrG_root[ccwc] = Root_Wt[ccwc];
	biomass += Root_Wt[ccwc] + Leaf_Wt[ccwc] + Stem_Wt[ccwc] + Grain_Wt[ccwc];

	//if(grain[ccwc]<0.05)//grass crops
	if(CoverCrop[ccwc]==1)//||grain[ccwc]<0.05)//cover crop
	{
		float AboveGroundBiomass = YrG_grain[ccwc]+YrG_leaf[ccwc]+YrG_stem[ccwc];
		YrG_grain[ccwc] = 0.0;
		YrG_leaf[ccwc] = 0.0;
		YrG_stem[ccwc] = 0.0;//AboveGroundBiomass;

		Stuble = AboveGroundBiomass;
	}
	else
	{
		if(scale==1)
		{
			if(crop[ccwc]==4||crop[ccwc]==5) left_perc[ccwc] = 0.05;
		}
		
		if(grain[ccwc]>0.05) Stuble = (YrG_leaf[ccwc]+YrG_stem[ccwc]) * left_perc[ccwc]; // grain crops
		else Stuble = (YrG_grain[ccwc]+YrG_leaf[ccwc]+YrG_stem[ccwc]) * left_perc[ccwc]; //0.0;
	}
	AddC = Stuble;
	AddCN = leafcn[ccwc];

	// Add new stub litter into stub litter pools 
	alloc_litter_into_pools(&AddCN, &AddC, oz, &AddC1, &AddC2, &AddC3, &dInertC, 1, rcnrvl, rcnrl, rcnrr, nh4);

	stub1 += AddC1;
	stub2 += AddC2;
	stub3 += AddC3;//where to put inert_C[]?
	stubP = CropP[ccwc] * 0.01 * AddC / 0.4;

	day_addC += (AddC1+AddC2+AddC3);
	yr_addtc += (AddC1+AddC2+AddC3);
	day_inStubN += AddC1 / rcnrvl + AddC2 / rcnrl + AddC3 / rcnrr;
	yr_addtn += day_inStubN;

	AddC1 = 0.0;
	AddC2 = 0.0;
	AddC3 = 0.0;

	AddC = YrG_root[ccwc];
	AddCN = rootcn[ccwc];
	alloc_litter_into_pools(&AddCN, &AddC, oz, &AddC1, &AddC2, &AddC3, &dInertC, 0, rcnrvl, rcnrl, rcnrr, nh4);
	float AddP = CropP[ccwc] * 0.01 * AddC / 0.4;

	for ( l = 1; l <= q; l++ )
	{
	    if ( l >= 1 && l <= 3 )
		{
			rcvl[l] += AddC1 * 0.5 / 3.0;
			rcl[l] += AddC2 * 0.5 / 3.0;
			rcr[l] += AddC3 * 0.5 / 3.0;
			inert_C[l] += day_inert_C * 0.5 / 3.0;
			OrgP[l] += AddP * 0.5 / 3.0;
		}
	    if ( l >= 4 && l <= 6 )
		{
			rcvl[l] += AddC1 * 0.4 / 3.0;
			rcl[l] += AddC2 * 0.4 / 3.0;
			rcr[l] += AddC3 * 0.4 / 3.0;
			inert_C[l] += day_inert_C * 0.4 / 3.0;
			OrgP[l] += AddP * 0.4 / 3.0;
		}
	    if ( l >= 7 && l <= 9 )
		{
			rcvl[l] += AddC1 * 0.1 / 3.0;
			rcl[l] += AddC2 * 0.1 / 3.0;
			rcr[l] += AddC3 * 0.1 / 3.0;
			inert_C[l] += day_inert_C * 0.1 / 3.0;
			OrgP[l] += AddP * 0.1 / 3.0;
		}
	}

	float AddToSoilC, AddToSoilN;
	AddToSoilC = AddC1 + AddC2 + AddC3 + dInertC;
	AddToSoilN = AddC1/rcnrvl + AddC2/rcnrl + AddC3/rcnrr;

	day_addC += AddToSoilC;
	yr_addrc += AddToSoilC;
	day_addrn += AddToSoilN;
	yr_addrn += AddToSoilN;

	float RootAddC = AddC1+AddC2+AddC3;
	harvest_AddC += RootAddC;
    AddC1 = 0.0;
    AddC2 = 0.0;
    AddC3 = 0.0;

	AddC = 0.0;
	
	haradd2 = 0.0;
	//demandn[ccwc] = 0.0;
	lai[ccwc] = 0.0;
	crop_flag[ccwc] = 0;
	//ccwc = ccwc + 1;
	seed[ccwc] = 0;
	//crop_days[ccwc]=0;
	PGI[ccwc] = 0.0;
	//*cwc_p = ccwc;
	//I_Stage = 7;
	plantn[ccwc]=0.0;
	Root_Wt[ccwc] = 0.0;
	Grain_Wt[ccwc] = 0.0;
	Leaf_Wt[ccwc] = 0.0;
	Stem_Wt[ccwc] = 0.0;
	LeafN[ccwc] = 0.0;
	StemN[ccwc] = 0.0;
	GrainN[ccwc] = 0.0;
	RootN[ccwc] = 0.0;

	PresentBiomass[ccwc] = Root_Wt[ccwc] + Leaf_Wt[ccwc] + Stem_Wt[ccwc] + Grain_Wt[ccwc];*/

    int    l, oz=0;
    float  AddC, AddCN;
    float  Stuble;
    float haradd2, harvest_AddC=0.0;
	float AddC1, AddC2, AddC3, dInertC;

	if(crop[ccwc]==20||crop[ccwc]==46) 
	{
		IrriRice_flag = 0;
		ShallowFloodFlag = 0;
		flood_flag = 0;
		RainfedRice_flag = 0;
	}

	HarvestDay[ccwc] = jday;

	CropEndN[ccwc] = plantn[ccwc];
		
	PresentBiomass[ccwc] = Root_Wt[ccwc] + Leaf_Wt[ccwc] + Stem_Wt[ccwc] + Grain_Wt[ccwc];
	ProdC[ccwc] = PresentBiomass[ccwc];
	if(grain[ccwc]>0.05)
	{
		YrG_grain[ccwc] = Grain_Wt[ccwc];
		YrG_leaf[ccwc] = Leaf_Wt[ccwc];
		YrG_stem[ccwc] = Stem_Wt[ccwc];
		YrG_root[ccwc] = Root_Wt[ccwc];
	}
	biomass += Root_Wt[ccwc] + Leaf_Wt[ccwc] + Stem_Wt[ccwc] + Grain_Wt[ccwc];

	//if(grain[ccwc]<0.05)//grass crops
	if(CoverCrop[ccwc]==1)//||grain[ccwc]<0.05)//cover crop
	{
		float AboveGroundBiomass = Leaf_Wt[ccwc] + Stem_Wt[ccwc] + Grain_Wt[ccwc];//YrG_grain[ccwc]+YrG_leaf[ccwc]+YrG_stem[ccwc];
		//YrG_grain[ccwc] = 0.0;
		//YrG_leaf[ccwc] = 0.0;
		//YrG_stem[ccwc] = 0.0;//AboveGroundBiomass;

		Stuble = AboveGroundBiomass;
	}
	else
	{
		if(scale==1)
		{
			if(crop[ccwc]==4||crop[ccwc]==5) left_perc[ccwc] = 0.05;
		}
		
		if(grain[ccwc]>0.05) 
			Stuble = (Leaf_Wt[ccwc] + Stem_Wt[ccwc]) * left_perc[ccwc]; // grain crops
		else 
			Stuble = (Leaf_Wt[ccwc] + Stem_Wt[ccwc] + Grain_Wt[ccwc]) * left_perc[ccwc]; //0.0;
	}

	Leaf_Wt[ccwc] = 0.0;
	Stem_Wt[ccwc] = 0.0;
	Grain_Wt[ccwc] = 0.0;

	AddC = Stuble;
	AddCN = leafcn[ccwc];

	/* Add new stub litter into stub litter pools */
	alloc_litter_into_pools(&AddCN, &AddC, oz, &AddC1, &AddC2, &AddC3, &dInertC, 1, rcnrvl, rcnrl, rcnrr, nh4);

	stub1 += AddC1;
	stub2 += AddC2;
	stub3 += AddC3;//where to put inert_C[]?
	stubP = CropP[ccwc] * 0.01 * AddC / 0.4;
	inert_C[1] += dInertC;

	day_addC += dInertC;//(AddC1+AddC2+AddC3);
	yr_addtc += dInertC;;//(AddC1+AddC2+AddC3);
	//day_inStubN += AddC1 / rcnrvl + AddC2 / rcnrl + AddC3 / rcnrr;
	//yr_addtn += day_inStubN;

	AddC1 = 0.0;
	AddC2 = 0.0;
	AddC3 = 0.0;

	
	if(Root_Wt[ccwc]>0.0)
	{
		AddC = Root_Wt[ccwc];
		AddCN = rootcn[ccwc];
		alloc_litter_into_pools(&AddCN, &AddC, oz, &AddC1, &AddC2, &AddC3, &dInertC, 0, rcnrvl, rcnrl, rcnrr, nh4);
		float AddP = CropP[ccwc] * 0.01 * AddC / 0.4;

		for ( l = 1; l <= q; l++ )
		{
			if ( l >= 1 && l <= 3 )
			{
				rcvl[l] += AddC1 * 0.5 / 3.0;
				rcl[l] += AddC2 * 0.5 / 3.0;
				rcr[l] += AddC3 * 0.5 / 3.0;
				inert_C[l] += day_inert_C * 0.5 / 3.0;
				OrgP[l] += AddP * 0.5 / 3.0;
			}
			if ( l >= 4 && l <= 6 )
			{
				rcvl[l] += AddC1 * 0.4 / 3.0;
				rcl[l] += AddC2 * 0.4 / 3.0;
				rcr[l] += AddC3 * 0.4 / 3.0;
				inert_C[l] += day_inert_C * 0.4 / 3.0;
				OrgP[l] += AddP * 0.4 / 3.0;
			}
			if ( l >= 7 && l <= 9 )
			{
				rcvl[l] += AddC1 * 0.1 / 3.0;
				rcl[l] += AddC2 * 0.1 / 3.0;
				rcr[l] += AddC3 * 0.1 / 3.0;
				inert_C[l] += day_inert_C * 0.1 / 3.0;
				OrgP[l] += AddP * 0.1 / 3.0;
			}
		}

		float AddToSoilC, AddToSoilN;
		AddToSoilC = AddC1 + AddC2 + AddC3 + dInertC;
		AddToSoilN = AddC1/rcnrvl + AddC2/rcnrl + AddC3/rcnrr;

		day_addC += AddToSoilC;
		yr_addrc += AddToSoilC;
		day_addrn += AddToSoilN;
		yr_addrn += AddToSoilN;

		float RootAddC = AddC1+AddC2+AddC3;
		harvest_AddC += RootAddC;
		AddC1 = 0.0;
		AddC2 = 0.0;
		AddC3 = 0.0;
	}

	AddC = 0.0;
	
	haradd2 = 0.0;
	//demandn[ccwc] = 0.0;
	lai[ccwc] = 0.0;
	crop_flag[ccwc] = 0;
	//ccwc = ccwc + 1;
	seed[ccwc] = 0;
	//crop_days[ccwc]=0;
	PGI[ccwc] = 0.0;
	//*cwc_p = ccwc;
	//I_Stage = 7;
	plantn[ccwc]=0.0;
	Root_Wt[ccwc] = 0.0;
	Grain_Wt[ccwc] = 0.0;
	Leaf_Wt[ccwc] = 0.0;
	Stem_Wt[ccwc] = 0.0;
	LeafN[ccwc] = 0.0;
	StemN[ccwc] = 0.0;
	GrainN[ccwc] = 0.0;
	RootN[ccwc] = 0.0;

	PresentBiomass[ccwc] = Root_Wt[ccwc] + Leaf_Wt[ccwc] + Stem_Wt[ccwc] + Grain_Wt[ccwc];
}

void class_model::partition_vll_humad(float solid_CN, float AddC, float solid_MN, float *AddC1, float *AddC2, float *AddC3,
									  float *AddHum, float rcnrvl, float rcnrl, float rcnrr, float rcnh)
{
	if(solid_CN<rcnrvl)
	{
		*AddHum = AddC; 
		*AddC1 = 0.0;
		*AddC2=0.0;
		*AddC3=0.0;
	}
	else if(solid_CN>=rcnrvl&&solid_CN<rcnh)
	{
		*AddHum = (rcnrvl*rcnh*solid_MN - rcnh*AddC)/(rcnh-rcnrvl);
		if(*AddHum<0.0) *AddHum=0.0;
		*AddC1 = AddC - *AddHum;
		*AddC2=0.0;
		*AddC3=0.0;
	}
	else if(solid_CN>=rcnh&&solid_CN<rcnrl)
	{
		*AddHum = (rcnh*rcnrl*solid_MN - rcnh*AddC)/(rcnrl-rcnh);
		if(*AddHum<0.0) *AddHum=0.0;
		*AddC1 = 0.0;
		*AddC2 = AddC - *AddHum;
		*AddC3=0.0;
	}
	else
	{
		*AddC3 = (rcnrr*rcnrl*solid_MN - rcnrr*AddC)/(rcnrl-rcnrr);
		if(*AddC3<0.0) *AddC3=0.0;
		*AddC2 = AddC - *AddC3;
		*AddC1 = 0.0;
		*AddHum = 0.0;
	}

}

