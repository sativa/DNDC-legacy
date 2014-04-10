#include "stdafx.h"
#include "Dndcgo.h"
#include "Source_main.h"
#include "Dndc_tool.h"

#ifdef MANURE 
extern class_manure MData;
#endif

float class_model::CH4_peat(int MODE, int GN, float *temp, float *clay, int flood_num, int FloDays, float *Root_Wt,
							  int q, float *Eh, float *sph, float *vascularity, int LandUse, float *day_rco2,
							  float *R_Wt, float *day_co2, float root_dCO2, int flood_flag, float *rcvl,
							  float *rcl, float *rcr, int *st, float rcnrvl, float rcnrl, float rcnrr,
							  float MicrobioIndex, float *ch4, int WTL, float day_WT,
							  float *soil_ph, float *doc, float *nh4, float *ch4_prod, float *ch4_day_oxid,
							  float *plant_day_ch4, float *day_wfps, int *seed, float *day_ch4, float *anvf,
							  float WaterLeakRate, float *h, float *CRB1, float rcnb, float *co2)
{
	int l;
	float F_temp, CH4_P1, CH4_P3, CH4_P2, CH4_O;
	float ch4_balloon=0.0, InputRoot_DOC[70]={0.0,0.0}, C_soil[70]={0.0,0.0};
	float soil_CO2=0.0, root_CO2=0.0;

#ifdef ISOTOPE // Define constant factors	
	
	float pCH4mc_12, pCH4mc_13;// 12C and 13C in CO2-converted CH4, kg C/ha
	float pCH4md_12, pCH4md_13;// 12C and 13C in DOC-converted CH4, kg C/ha
	float fCH4ep_12, fCH4ep_13;// 12C and 13C in CH4 emitted through plant, kg C/ha/day
	float fCH4eb_12, fCH4eb_13;// 12C and 13C in CH4 emitted through ibulation, kg C/ha/day
	float fCH4ed_12, fCH4ed_13;// 12C and 13C in CH4 emitted through diffusion, kg C/ha/day
	float fCO2_12, fCO2_13;// 12C and 13C in CO2 from CH4 oxidation, kg C/ha/day

	float PDB = 0.0112372;// Standard d13C
	float alf_CO2_CH4 = 1.095;//alfa from CO2 to CH4:1.080 [Conrad 2005: 1.045-1.082]
	float alf_DOC_CH4 = 1.026;//alfa from DOC to CH4:1.040 [Conrad 2005: 1.000-1.032]
	float alf_CH4ep = 1.016;//alfa for CH4 transport by plant
	float alf_CH4eb = 1.0;//alfa for CH4 transport by ibulation
	float alf_CH4ed = 1.001;//alfa for CH4 transport by diffusion
	float alf_CH4_CO2 = 1.025;//alfa from CH4 to CO2
	del13C_CO2 = -0.026;//initial d13C of peat_CO2
	del13C_DOC = -0.026;//initial d13C of peat_DOC

#endif	
 
	CH4_P1=0.0;
	CH4_P2=0.0;
	CH4_P3=0.0;
	
	float TotalRootWT = Root_Wt[1]+Root_Wt[2]+Root_Wt[3];	 
	float TotalVas = max(vascularity[1], max(vascularity[2],vascularity[3]));	 
	
	for (l=1; l<=q; l++)
	{
		if(l>=WTL)//layer is saturated  
		{
			Eh[l] = -65.0 * (float)l + 25.0;
			if(Eh[l]<-250.0) Eh[l] = -250.0;
			
			ch4_balloon = -0.0018 * Eh[l] + 0.5651;
			ch4_balloon = min(1.0, max(0.1, ch4_balloon));    
		}
		else 
		{
			Eh[l] = 100.0;
			ch4_balloon = 0.1;
		}

		//--- temperature factor: Arrhenius equation (ref. Ding & Wang,1996)
		// CH4 production rate is a first order of temperature
	
		float tTT;
		if(temp[l]<1.0) tTT = 0.0;
		else tTT = temp[l];
		F_temp = tTT/10.0;//20.0
		F_temp = max(0.0, min(1.0, F_temp));

		//// C sources ////////////////////////////////////////////////////////////////////////////////////////////////
	
		//--- DOC: from root exudation
		float WTF;
		if(day_WT>0.1) WTF = ((day_WT - 0.1) * 500.0 + 1.0) * F_temp;
		else WTF = 1.0 * F_temp;

		if (TotalRootWT>0.0)
		{
		   //InputRoot_DOC[l] = 0.005 * TotalRootWT / (float)q;// 0.005 * WTF * Day_C_growth[1]  / (1.0 + PGI[1]);(kg co2-C/ha/d/layer)£¬ 0.025	 
		
			InputRoot_DOC[l] = 0.00015 * Day_C_growth[1] / (1.0 + PGI[1]) * TotalRootWT / (float)q;//0.000007

			day_RootInput_DOC += InputRoot_DOC[l];//daily root DOC input for balance 

			//TotalDOC[l] += InputRoot_DOC[l];
		}
		else
		{
		   InputRoot_DOC[l] = 0.0;
		}     
		
		//--- DOC: from soil
		float InputSoil_DOC = 0.0;
		if(ch4_balloon>=0.0)
		{ 
			//--- DOC from soil litter due to anaerobic decomposition
			float LitterC1=0.0, LitterC2=0.0, LitterC3=0.0, HumadsC1=0.0, HumadsC2=0.0;			

			if(rcvl[l]>0.0) 
			{
				LitterC1 = rcvl[l] * F_temp * 0.00025;//kg C/ha
				rcvl[l] -= LitterC1;
				nh4[l] += (LitterC1 / rcnrvl);	
				
				InputSoil_DOC += LitterC1;
			}

			if(rcl[l]>0.0) 
			{
				LitterC2 = rcl[l] * F_temp * 0.0005;
				rcl[l] -= LitterC2;
				nh4[l] += (LitterC2 / rcnrl);
				
				InputSoil_DOC += LitterC2;
			}
			
			if(rcr[l]>0.0) 
			{
				LitterC3 = rcr[l] * F_temp * 0.000025;
				rcr[l] -= LitterC3;
				nh4[l] += (LitterC3 / rcnrr);
				
				InputSoil_DOC += LitterC3;
			}

/*			if(crhl[l]>0.0)
			{
				HumadsC2 = crhl[l] * F_temp * 0.001;
				crhl[l] -= HumadsC2;
				nh4[l] += (HumadsC2 / rcnh);
				
				TotalDOC[l] += HumadsC2;
			}

			if(crhr[l]>0.0)
			{
				HumadsC1 = crhr[l] * F_temp * 0.001;
				crhr[l] -= HumadsC1;
				nh4[l] += (HumadsC1 / rcnh);
				
				TotalDOC[l] += HumadsC1;
			}
*/
			
			//--- DOC from stub due to anaerobic decomposition
			float dFDC1=0.0, dFDC2=0.0, dFDC3=0.0;
			if(l==1&&flood_flag==1 && (stub1+stub2+stub3)>0.0)
			{
				float DD = 0.1 * F_temp;//0.001
				float dFDC,  dLN;
				DD=min(1.0, max(0.0, DD));

				dFDC1 = stub1 * DD;
				stub1 -= dFDC1;
				nh4[l] += (dFDC1 / rcnrvl);

				dFDC2 = stub2 * DD * 0.1;
				stub2 -= dFDC2;
				nh4[l] += (dFDC2 / rcnrl);

				dFDC3 = stub3 * DD * 0.05;
				stub3 -= dFDC3;
				nh4[l] += (dFDC3 / rcnrr);

				InputSoil_DOC += dFDC1 + dFDC2 + dFDC3;
			}
		}

		peat_DOC[l] += (InputRoot_DOC[l] + InputSoil_DOC);

		//day_DOC_prod += (InputRoot_DOC[l] + InputSoil_DOC);
		day_Root_DOC += InputRoot_DOC[l];
		day_SOM_DOC += InputSoil_DOC;
		InputRoot_DOC[l] = 0.0;
		InputSoil_DOC = 0.0;

		ch4_doc_pool[l] += (InputRoot_DOC[l] + InputSoil_DOC);

		//CO2 sources
		
		if(TotalRootWT>0.0)
		{
			root_CO2 = 0.000001 * Day_C_growth[1] / (1.0 + PGI[1]) * TotalRootWT / (float)q;
		}
		else
		{
			root_CO2 = 0.0;
		}

		if(ch4_balloon>=0.0)
		{ 
			//--- DOC from soil litter due to anaerobic decomposition
			float LitterC1=0.0, LitterC2=0.0, LitterC3=0.0, HumadsC1=0.0, HumadsC2=0.0;			

			if(rcvl[l]>0.0) 
			{
				LitterC1 = rcvl[l] * F_temp * 0.005;//kg C/ha
				rcvl[l] -= LitterC1;
				nh4[l] += (LitterC1 / rcnrvl);	
				
				soil_CO2 += LitterC1;
			}

			if(rcl[l]>0.0) 
			{
				LitterC2 = rcl[l] * F_temp * 0.0005;
				rcl[l] -= LitterC2;
				nh4[l] += (LitterC2 / rcnrl);
				
				soil_CO2 += LitterC2;
			}
			
			if(rcr[l]>0.0) 
			{
				LitterC3 = rcr[l] * F_temp * 0.00005;
				rcr[l] -= LitterC3;
				nh4[l] += (LitterC3 / rcnrr);
				
				soil_CO2 += LitterC3;
			}
		}

		//soil_CO2 = (rcvl[l]+rcl[l]+rcr[l]) * F_temp * (1.0 - ch4_balloon) * 0.00001;

		peat_CO2[l] += (root_CO2 + soil_CO2);
		day_CO2_prod += (root_CO2 + soil_CO2);
		day_Root_CO2 += root_CO2;
		day_SOM_CO2 += soil_CO2;
		day_ch4_CO2[l] += (root_CO2 + soil_CO2);
		root_CO2 = 0.0;
		soil_CO2 = 0.0;

		H2[l] = 1.0 * (peat_CO2[l] * 2.0/72.0);

		day_H2_PROD += H2[l];
		day_SOM_H2 += H2[l];
		day_H2_pool[l] += H2[l];
	

			//CO2 and H2 are produced from anaerobic decomposition of soil DOC:
			// C6H12O6 -> CH3COOH + CH3CH2COOH + CO2 + H2

/*			float ch4_max_30 = 5.0E-5 * 1.0E-6 * 12.01 * 3600.0 * 24.0 * mmm[l];//! (kgC/ha/day per layer)
			float ch4_max;
			float Q10CH4 = 4.6;

			if (temp[l] <= 0.0) ch4_max = 0.0;
		     else if (temp[l] < 40.0) ch4_max = ch4_max_30 * pow(Q10CH4, (temp[l] - 0.0)/10.0); //!
			 else ch4_max = ch4_max_30 * Q10CH4 ;//! fixed Vmax over 40C

			 float ch4_max_h2 = ch4_max * 0.5;//! Vmax of CH4 production from H2
			 float ch4_max_doc = ch4_max * 0.5;//! Vmax of CH4 production from DOC
*/

/*			if(root[1]>0.15) //sedge
			{
				C_soil[l] = TotalDOC[l];
				co2[l] = 0.0;
				H2[l] = 0.0;
				TotalDOC[l] = 0.0;
			}
			else// moss
			{
				co2[l] = 5.0 * TotalDOC[l];
				C_soil[l] = 0.0;
				H2[l] = 5.0 * (co2[l] * 2.0/72.0);
				TotalDOC[l] = 0.0;
			}
*/
/*			float Rcc = max(0.4, min(0.6, ch4_balloon));//5.0/6.0;
			float Rhh = 2.0/72.0;//by weight

			C_soil[l] = (TotalDOC[l] * Rcc);					
			co2[l] += (TotalDOC[l] * (1.0 - Rcc));
			
			//H2[l] += (TotalDOC[l] * Rhh);
			//day_H2_prod[l] += (TotalDOC[l] * Rhh);//kg H/ha
			float PHR = 1.0;
			if(root[1]<=0.15) PHR = 5.0 * PGI[1]; 

			H2[l] += (PHR * (C_soil[l] * Rhh));
			day_H2_prod[l] += (C_soil[l] * Rhh);//kg H/ha

			TotalDOC[l] = 0.0;
*/				 
			/////////Refer to Fumoto's routines//////////////////////////
			 
			 //--- kinetic parameters and variables for CH4 production ------------------------------
			 float ch4_max_30, Q10CH4, ch4_max, ch4_max_h2, ch4_max_doc, km_h2, km_doc;//, K1_DOC;
			 float H2_conc, avail_h2, doc_conc, dAcetate;
			 
			 // saturated H2 conc. in water at 25C, from Henry const. (Atkins 1998, Physical Chemistry 6th ed., Oxford U. Press)
			 float saturate_h2 = 0.78E-3; //! (mol H2 /L)
			 float V_water = day_wfps[l] * h[l] * 10000.0 * 1000.0;//liter water/ha in layer l
			 saturate_h2 = saturate_h2 * 2.0 * V_water / 1000.0; // kg H/ha/layer

			 // Max. CH4 production rate at 30C. 2.0E-5 (mmol/L/s) or 5.0E-5 (mmol/kg soil/s) (Van Bodegom and Scholten, 2001) 
			 ch4_max_30 = 5.0E-5 * 1.0E-6 * 12.01 * 3600.0 * 24.0 * mmm[l];//! (kgC/ha/day per layer)
			 //ch4_max_30 = 2.0E-5 *1.0E-6 *12.01 *3600.0 * soil_water;//! (kgC/ha/h) <--- looks too low

			 Q10CH4 = 4.6;//! Q10 for CH4 production (van Bodegom and Stams,1999, Chemosphere 39:167-182)

			 if (temp[l] <= 0.0) ch4_max = 0.0;
		     else if (temp[l] < 40.0) ch4_max = ch4_max_30 * pow(Q10CH4, (temp[l] - 0.0)/10.0); //!
			 else ch4_max = ch4_max_30 * Q10CH4 ;//! fixed Vmax over 40C

			 ch4_max_h2 = ch4_max * 0.5;//! Vmax of CH4 production from H2
			 ch4_max_doc = ch4_max * 0.5;//! Vmax of CH4 production from DOC

			 km_h2 = 13.3E-6 * 2.0 * V_water / 1000.0;//!  Km for H2 (mol H2/L -> kg H/ha/layer) (Van Bodegom and Scholten, 2001)
		//	 km_h2 = 13.3E-4;//! 
			 km_doc = 5.12E-3 * 12.0 * V_water / 1000.0;//! Km for acetate (mol DOC /L -> kg C/ha/layer) (Van Bodegom and Scholten, 2001)

			// K1_DOC = 0.286;//! virtual 1st-order rate constant at 30C (/d) (Van Bodegom and Scholten, 2001; Bodegom and Stams, 1999) 
		//	 K1_DOC = 0.5;//! theoretical maximum
		
			//----- available H2 
			//H2_conc = min(saturate_h2, hour_h2 / n * m /soil_water); //! (mol H2/L)
			H2_conc = min(saturate_h2, H2[l]); //! (kg H/ha/layer)
			avail_h2 = H2_conc;// kg H/ha/layer /1000.0 * water[1][l];//! (kmol H2/ha)

			//----- CH4 production from H2 and CO2 
			if(peat_CO2[l]>0.0)
			{
				day_ch4_CO2[l] = 0.5 * peat_CO2[l];

				CH4_P1 = ch4_max_h2 * H2_conc/(km_h2 + H2_conc); //! (kgC/ha) 
				//pro_ch41 = ch4_max_h2;//! CH4 production with high affinity to H2 (kgC/ha)

				if (CH4_P1 > day_ch4_CO2[l]) CH4_P1 = day_ch4_CO2[l];//! limit by CO2, kg C/ha/layer
				if (CH4_P1/12.01*4.0 > avail_h2) CH4_P1 = avail_h2 /4.0*12.01;//! limit by H2 
				
				H2[l] -= CH4_P1 /12.01 * 4.0;//! H2 after CH4 production (kg H/ha/layer)

				day_H2_pool[l] -= CH4_P1 /12.01 * 4.0;
			
#ifdef ISOTOPE
				CO2_12[l] = CH4_P1 /((1.0 + del13C_CO2) * PDB + 1);// = CO2(n) * 0.9891734;
				CO2_13[l] = CH4_P1 - CO2_12[l];

				day_CO2_13 += CO2_13[l];
				day_CO2_12 += CO2_12[l];				

				pCH4mc_12 = CH4_P1 / ((CO2_13[l] / CO2_12[l]) / alf_CO2_CH4 + 1.0);
				pCH4mc_13 = CH4_P1 - pCH4mc_12;

				CH4_13[l] += pCH4mc_13;
				CH4_12[l] += pCH4mc_12;
				day_prod_CH4_13 += pCH4mc_13;
				day_prod_CH4_12 += pCH4mc_12;
				CO2_13[l] -= pCH4mc_13;
				CO2_12[l] -= pCH4mc_12;
				pCH4mc_13 = 0.0;
				pCH4mc_12 = 0.0;
#endif
				
			}
			else
			{
				CH4_P1 = 0.0;
#ifdef ISOTOPE
				CO2_12[l] = 0.0;
				CO2_13[l] = 0.0;

				pCH4mc_12 = 0.0;
				pCH4mc_13 = 0.0;
#endif
			}

			ch4[l] += CH4_P1;//kg C/ha/layer
			*ch4_prod += CH4_P1; // daily ch4 production (kg CH4-C/ha/d)
			ch4_prod1 += CH4_P1;
			day_ch4_1[l] = CH4_P1;
			peat_CO2[l] -= CH4_P1; //! CO2 after CH4 production (kgC/ha/layer)
			day_ch4_pool += CH4_P1;
			day_ch4_CO2[l] -= CH4_P1;

		
			//----- CH4 production from DOC (acetate) 
			
			if(peat_DOC[l]>0.0)
			{
				dAcetate = ch4_max_doc * peat_DOC[l]/(km_doc + peat_DOC[l]); //consumption of acetate, kg C/ha/layer// ! (kg CH4-C/ha)
				//	pro_ch42 = K1_DOC * doc[l];//! 1st-order model (kg CH4-C/ha) 

				if (dAcetate > peat_DOC[l]) dAcetate = peat_DOC[l];//! limit by DOC
				if ((dAcetate * 0.5) > (ch4_max - CH4_P1)) dAcetate = (ch4_max - CH4_P1) * 2.0;//! 

				CH4_P2 = dAcetate;

				//InputRoot_DOC[l] -= (dAcetate * InputRoot_DOC[l] / Totdoc);
								
				//CH4_P2 = dAcetate * 0.75;//! CH4 production, kg C/ha/layer
				//co2[l] += dAcetate * 0.25;//! CO2 after CH4 production (CH3COOH ---> CH4 + CO2), kg C/ha/layer 				

#ifdef ISOTOPE
				DOC_12[l] = peat_DOC[l] / ((1.0 + del13C_DOC) * PDB + 1);
				DOC_13[l] = peat_DOC[l] - DOC_12[l];
				day_DOC_13 += DOC_13[l];
				day_DOC_12 += DOC_12[l];

				pCH4md_12 = CH4_P2 / ((DOC_13[l] / DOC_12[l]) / alf_DOC_CH4 + 1.0);
				pCH4md_13 = CH4_P2 - pCH4md_12;

				day_prod_CH4_13 += pCH4md_13;
				day_prod_CH4_12 += pCH4md_12;

				CH4_13[l] += pCH4md_13;
				CH4_12[l] += pCH4md_12;	
				
				day_CH4_13 += CH4_13[l];
				day_CH4_12 += CH4_12[l];
				
				//DOC_13[l] -= pCH4md_13;
				//DOC_12[l] -= pCH4md_12;
				delCH4[l] = (CH4_13[l] / CH4_12[l]) / PDB - 1.0;
#endif	
				/*if(peat_DOC[l]>0.0)
				{
					TotalDOC[l] += peat_DOC[l];//
					//doc[l] = TotalDOC[l];
					C_soil[l] = 0.0;
				}*/
			}
			else
			{
				CH4_P2 = 0.0;
				//ch4_doc_pool[l] = 0.0;
#ifdef ISOTOPE
				DOC_12[l] = 0.0;
				DOC_13[l] = 0.0;
				pCH4md_13 = 0.0;
				pCH4md_12 = 0.0;
#endif
			}	
			
			peat_DOC[l] -= CH4_P2;			
			ch4_prod2 += CH4_P2;
			day_ch4_2[l] = CH4_P2;
			day_ch4_pool += CH4_P2;
			ch4[l] += CH4_P2;//kg C/ha/layer
			*ch4_prod += CH4_P2; // daily ch4 production (kg CH4-C/ha/d)	
			
			ch4_doc_pool[l] -= CH4_P2;
			if(ch4_doc_pool[l]<0.0) ch4_doc_pool[l] = 0.0;

			//----- CH4 production on the day 
			CH4_P1 = 0.0;	
			CH4_P2 = 0.0;

			
			
			/////////End of Fumoto's routines////////////////////
			 
			 
			 
			/* if(nh4[l] >= InputRoot_DOC[l] / rcnb)
			 {
				CRB1[l] += InputRoot_DOC[l];
				nh4[l] -= (InputRoot_DOC[l] / rcnb);
				InputRoot_DOC[l] = 0.0;
			 }
			 else
			 {
				CRB1[l] += nh4[l] * rcnb;
				InputRoot_DOC[l] -= nh4[l] * rcnb;
				NH4[l] = 0.0;
			 }*/
			 		 
			 //InputRoot_DOC[l] = 0.0;
			 

		//}//end of if(ch4_balloon>=0.5)
	}//end of layer loop

	//// CH4 transport ////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ISOTOPE	
		fCH4ep_13 = 0.0;
		fCH4ep_12 = 0.0;
		fCH4eb_13 = 0.0;
		fCH4eb_12 = 0.0;
		fCH4ed_13 = 0.0;
		fCH4ed_12 = 0.0;
#endif

		for (l=1; l<=q; l++)
		{
			//plant trnsport
			if(TotalVas>0.0&&ch4[l]>0.0&&Leaf_Wt[1]) 
			{		
				float TFT = 0.03 * TotalVas;// * F_temp;
				TFT = min(0.8, max(0.0, TFT));

				float dPF = TFT * ch4[l];// 0.2,;
				//float dPFC = 1.0 * co2[l] * TotalVas;

				*day_ch4 += dPF;
				//day_co2 += dPFC;
				ch4[l] -= dPF;
				//co2[l] -= dPFC;
#ifdef ISOTOPE
				if(CH4_12[l]>0.0&&CH4_13[l]>0.0)
				{
					float qw = dPF / (1.0 + (alf_CH4ep * CH4_12[l] / CH4_13[l]));
					fCH4ep_13 += qw;
					fCH4ep_12 += (dPF - qw);

					CH4_13[l] -= qw;
					CH4_12[l] -= (dPF - qw);
					
					CH4_13[l] = max(0.0, CH4_13[l]);
					CH4_12[l] = max(0.0, CH4_12[l]);
				}
#endif
			}
			
			//ebullition
			if(ch4[l]>0.0)
			{
				float Ttt = temp[1];
				if(Ttt < 0.0) Ttt = 0.0;
				float ebul_ch4 = 0.1 * ch4[l] * Ttt/40.0;
				ch4[l] -= ebul_ch4;
				*day_ch4 += ebul_ch4;
#ifdef ISOTOPE

				if(CH4_12[l]>0.0&&CH4_13[l]>0.0)
				{
					float qw = ebul_ch4 / (1.0 + (alf_CH4eb * CH4_12[l] / CH4_13[l]));
					fCH4eb_13 += qw;
					fCH4eb_12 += (ebul_ch4 - qw);

					CH4_13[l] -= qw;
					CH4_12[l] -= (ebul_ch4 - qw);

					CH4_13[l] = max(0.0, CH4_13[l]);
					CH4_12[l] = max(0.0, CH4_12[l]);
				}
#endif
			}

			//diffusion
			if(ch4[l]>0.0)
			{
				float dDF, TFT;

				TFT = max(0.0, air_temp) / 50.0;

				if(day_WT>0.0) dDF = 0.0001 * ch4[l] / (float)l;// * F_temp; 0.0001					
				else dDF = 0.0 * ch4[l] * TFT;//1.0

				//if(total_ice>0.0) dDF *= 0.01;
				
				*day_ch4 += dDF;					
				ch4[l] -= dDF;
								
#ifdef ISOTOPE
				if(CH4_12[l]>0.0&&CH4_13[l]>0.0)
				{
					float qw = dDF / (1.0 + (alf_CH4ed * CH4_12[l] / CH4_13[l]));
					fCH4ed_13 += qw;
					fCH4ed_12 += (dDF - qw);

					CH4_13[l] -= qw;
					CH4_12[l] -= (dDF - qw);

					CH4_13[l] = max(0.0, CH4_13[l]);
					CH4_12[l] = max(0.0, CH4_12[l]);
				}

#endif
				
/*				else 
				{
					float dDF = 0.5 * (ch4[l] - ch4[l-1]);// * F_temp;// / clay[l];
					//float dCO2 = 0.5 * (co2[l] - co2[l-1]);// / clay[l];
					ch4[l] -= dDF;
					ch4[l-1] += dDF;
					//co2[l] -= dCO2;
					//co2[l-1] += dCO2;
#ifdef ISOTOPE
					float qw = dDF / (1.0 + (alf_CH4ed * CH4_12[l] / CH4_13[l]));
					
					CH4_13[l] -= qw;
					CH4_13[l-1] += qw;
					CH4_12[l] -= (dDF - fCH4ed_13);
					CH4_12[l-1] += (dDF - fCH4ed_13);
#endif
				}*/
			}
		}//end of layer loop
	
#ifdef ISOTOPE			
		fCH4_13 = fCH4ep_13 + fCH4eb_13 + fCH4ed_13;
		fCH4_12 = fCH4ep_12 + fCH4eb_12 + fCH4ed_12;
		if(fCH4_12==0.0) del_fCH4 = -0.075;//0.0;
		else
			del_fCH4 = (fCH4_13 / fCH4_12) / PDB - 1.0;

	///// CH4 oxidation //////////////////////////////////////////////////////////////////////////////////////

	fCO2_13 = 0.0;
	fCO2_12 = 0.0;
#endif

	for (l=1; l<=q; l++)
	{
		if(ch4[l]>0.0)
		{
			if(TotalVas>0.0)
			{
				//if(ch4_balloon>0.1) CH4_O = ch4[l] * 0.6;//0.2
				//else CH4_O = ch4[l] * 0.6;
				CH4_O = ch4[l] * 0.6;
			}
			else
			{
				if(l>=WTL)//lower than WT
					CH4_O = ch4[l]*0.8;//0.05
				else
					CH4_O = ch4[l];// * 0.05; //(1.0 - ch4_balloon);

				//if(ch4_balloon>0.1) CH4_O = ch4[l] * 0.005;
				//else CH4_O = ch4[l] * 0.05;
			}

			day_ch4_3[l] = CH4_O;
			ch4[l] -= CH4_O;
			*ch4_day_oxid += CH4_O;//daily oxidation rate (kg CH4-C/ha/d)
			peat_CO2[l] += CH4_O;
			day_ch4_pool -= CH4_O;
			day_ch4_CO2[l] += CH4_O;
			//*day_co2 += peat_CO2[l];
			
#ifdef ISOTOPE
			if(CH4_12[l]>0.0&&CH4_13[l]>0.0)
			{
				float wss = CH4_O / (1.0 + (alf_CH4_CO2 * CH4_12[l] / CH4_13[l]));
				fCO2_13 += wss;
				fCO2_12 += (CH4_O - wss);

				day_oxid_CH4_13 += wss;
				day_oxid_CH4_12 += (CH4_O - wss);

				CH4_13[l] -= wss;
				CH4_12[l] -= (CH4_O - wss);	
				
				day_CH4_13 += CH4_13[l];
				day_CH4_12 += CH4_12[l];

				//delCH4[l] = (CH4_13[l] / CH4_12[l]) / PDB - 1.0;
			}			
#endif
		}	
		
		*day_co2 += (0.1 * peat_CO2[l]);
		peat_CO2[l] *= 0.9;
	} 
#ifdef ISOTOPE	
	del_fCO2 = (fCO2_13 / fCO2_12) / PDB - 1.0;
#endif
 
	return(*day_ch4);
}