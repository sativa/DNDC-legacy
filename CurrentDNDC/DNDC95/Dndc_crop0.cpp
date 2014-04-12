#include "stdafx.h"
#include "Dndcgo.h"
#include "Source_main.h"
#include "Dndc_tool.h"
    
class_model DData;
        
/*float OptimumT[80]={0.,25.,15.,25.,18.,18.,18.,25.,18.,18.,18.,
                       18.,10.,25.,25.,18.,18.,18.,18.,18.,20.,
                       25.,18.,18.,20.,18.,25.,18.,18.,18.,20.,
                       18.,18.,18.,18.,18.,18.,18.,18.,18.,18.,
                       18.,25.,18.,25.,20.,15.,18.,18.,20.,20.,
                       20.,20.,20.,20.,20.,20.,20.,20.,20.,20.,
                       20.,20.,20.,20.,20.,20.,20.,20.,20.,20.,
                       20.,20.,20.,20.,20.,20.,20.,20.,20.}; */				 
    
float SenesR[80]={0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
                       0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
                       0.,0.,0.,0.,0.,8.,0.,0.,0.,0.,
                       0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
                       0.,5.,0.,5.,0.,0.,0.,0.,0.,0.,
                       0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,
                       0.,0.,0.,0.,0.,0.,0.,0.,0.,0.012,
                       0.008,0.,0.,0.,0.,0.,0.,0.,0.}; 
int SenesD[80]={0,0,0,0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,0,0,
                       0,0,0,0,0,0,0,0,0,300,
                       320,0,0,0,0,0,0,0,0}; 

float CropPP=0.4;//P in crop%
             
float WinterWheat_LimitiT = 6.0;
int root_q;

void class_model::crop_N_demand(int ccwc, float DayPT2)
{
    float GrassBiomassProductivity, GrassCProductivity;
    float T_stress=1.0;

    static float TPNN=0.0;
    
    //temperature stress, using Beta distribution curve, Yan and Hunt 1999 (Annals of Botany 84:607-614).				
    float TTmax = 40.0, TTopt = OptimumT[ccwc];
    T_stress = (TTmax-air_temp)/(TTmax-TTopt)*(float)pow((air_temp/TTopt), TTopt/(TTmax-TTopt));
    T_stress = min(1.0, max(0.0, T_stress));

    if(crop[ccwc]>0)
    {
        day_N_demand[ccwc] = 0.0;
    
        float FloodEffect, WTEffect=1.0, WSEffect=1.0;

            if(wetland_flag==1||(retard==1&&st[1]>12))
            {
                FloodEffect = 1.0;
                WTEffect = 1.0;
                WiltFlag[ccwc]=0;
            }
            else 
            {
                FloodEffect = 1.0 / (FloDays+1.0);
                if(day_WT>-0.3) WTEffect = -day_WT;

                WSEffect = (float)pow(day_DWater[ccwc], (float)0.8);

                if(PGI[ccwc]>0.4&&PGI[ccwc]<0.7)
                {					
                    if(WSEffect<=0.01) Dwilt[ccwc]+=1.0;
                    else Dwilt[ccwc]=0.0;

                    if(Dwilt[ccwc]>=17.0)//20.0
                        WiltFlag[ccwc]++;
                }
                else 
                    WiltFlag[ccwc]=0;								
            }			

            if(plantn[ccwc]>=maxmn[ccwc]&&PerennialFlag[ccwc]==0) 
            {
                maturaty_flag[ccwc]=1;

                if(IrriRice_flag==1) rice_maturity_flag=1;//crop[ccwc]==20
            }

            if(maturaty_flag[ccwc]==0)
            {
                //if((CropSumN[ccwc]<(maxmn[ccwc] / (1.0 + (float)WiltFlag[ccwc])) && TDD[ccwc]<max_TDD[ccwc])||
                if((plantn[ccwc]<(maxmn[ccwc] / (1.0 + (float)WiltFlag[ccwc])) && TDD[ccwc]<max_TDD[ccwc])||
                    (PerennialFlag[ccwc]==1&&(yr_plantn[ccwc]<(maxmn[ccwc] / (1.0 + (float)WiltFlag[ccwc])) && TDD[ccwc]<max_TDD[ccwc])))
                {
                    float fPGI, dfPGI, ffPGI, GWa, GWb;

                    if(C_type[ccwc]==3)
                    {
                        GWa = 5.0;//5.0
                        GWb = 5.0;//5.0
                    }
                    else
                    {
                        GWa = 6.0;
                        GWb = 6.0;
                    }

                    if(jday==1||seed[ccwc]==1) old_fPGI[ccwc] = TDD[ccwc];//0.0

                    fPGI = TDD[ccwc];
                    
                    dfPGI = (fPGI - old_fPGI[ccwc]) / max_TDD[ccwc];
                    old_fPGI[ccwc] = TDD[ccwc];
                

                    if(PGI[ccwc]<=0.2&&crop[ccwc]!=20) //early vegetative stage
                        ffPGI = PGI[ccwc] / (0.5 + PGI[ccwc]) * (1.0 - PGI[ccwc]) / (0.5 + PGI[ccwc]) * GWa * 0.2; 
                    else if(PGI[ccwc]<=0.5) //vegetative stage
                        ffPGI = PGI[ccwc] / (0.5 + PGI[ccwc]) * (1.0 - PGI[ccwc]) / (0.5 + PGI[ccwc]) * GWa * 1.2; 
                    else//reproductive stage
                        ffPGI = PGI[ccwc] / (0.5 + PGI[ccwc]) * (1.0 - PGI[ccwc]) / (0.5 + PGI[ccwc]) * GWb * 1.5;
                    
                    day_need_N[ccwc] = 1.0 * maxmn[ccwc] * dfPGI;

                    float F_salinity = 1.0 / (1.0 + salinity * 0.02);
                                    
                    day_N_demand[ccwc] = day_need_N[ccwc] * WSEffect * T_stress * F_salinity * ffPGI;
                    
                    TPNN += day_N_demand[ccwc];

                    float wst = TPNN;
                }			  
                else  
                {
                    day_N_demand[ccwc] = 0.0;
                    if(TDD[ccwc]>=max_TDD[ccwc]) 
                    {
                        maturaty_flag[ccwc]++;
                        if(IrriRice_flag==1) rice_maturity_flag=1;//crop[ccwc]==20
                    }

                    //maturaty_flag[ccwc]++;
                }
            }
            else
                day_N_demand[ccwc] = 0.0;
    }

    if(C_type[ccwc]==3&&day_N_demand[ccwc]>0.0)
    {
        float CLAYf = 0.12 * (float)exp(9.008 * clay[1]); 
    
        if(crop[ccwc]==11||crop[ccwc]==12)
        {
            CLAYf = max(1.0, min(3.5, CLAYf));
            day_N_demand[ccwc] *= (float)pow((float)(day_DWater[ccwc]+0.01), (float)CLAYf);
        }
        else
        {
            CLAYf = max(0.5, min(3.5, CLAYf));
            day_N_demand[ccwc] *= (float)pow(day_DWater[ccwc], (float)CLAYf);
        }
    }
    
    if(ice[1]>0.0||snow_pack>0.0) day_N_demand[ccwc] = 0.0;

    day_N_demand[ccwc] *= CO2_factor;

    //if(crop[ccwc]==2&&jday>250) day_N_demand[ccwc] *= 0.2;
    if(crop[ccwc]==2&&air_temp<5.0) day_N_demand[ccwc] *= 0.2;

    ideal_DayCropC[ccwc] = day_need_N[ccwc] * plantcn[ccwc];
 
}


void class_model::n_uptake_by_crop(int ccwc)//for CropModel=0
{
    int i;
    float    layer_demand, layer_up_no3, layer_up_nh4, layer_up_nh3, UPP;
    float layer_up, day_Nuptake_from_clayN, day_SoilN_demand;

    day_Nuptake_from_clayN=0.0;

    if (seed[ccwc]==1) ddupn[ccwc] = 0.0;

    demandn[ccwc] += day_N_demand[ccwc];

    if(max_N_fix[ccwc]>0.0) 
    {
        day_fixN[ccwc] = (day_N_demand[ccwc] / maxmn[ccwc]) * max_N_fix[ccwc];
        day_SoilN_demand = day_N_demand[ccwc] - day_fixN[ccwc];
        
        day_soilN_from_Nfixation[ccwc] = 0.5 * day_fixN[ccwc];
        float dfn = day_soilN_from_Nfixation[ccwc]/20.0;
        for(i=1;i<=20;i++) nh4[i] += dfn;
    }
    else
    {
        day_fixN[ccwc] = 0.0;
        day_SoilN_demand = day_N_demand[ccwc];
    }

    /////////////////////////////////////////////
    if(day_SoilN_demand>0.0&&FertOption==333)//precision fertilization
    {
        float DADF = day_SoilN_demand * 1.1;
        day_inFertN += DADF;
        yr_addfer += DADF;
        fert_flag = 1;
        for (int i=1; i<=2; i++) nh4[i] += (DADF / 2.0);
        
    }
    ///////////////////////////////////////////////

    day_nup[ccwc] = 0.0;

    day_clay_N = 0.0;

    for(int vv=1; vv<=q; vv++) 
    {
        day_clay_N += clay_nh4[vv]; 
    }

    int QqQ = q;

    for ( i = 1; i <= QqQ; i++ )
    {
        UPP = day_SoilN_demand * (float)pow(0.5, i);
        layer_up = 0.0;

        if(ddupn[ccwc] > 0.0)
        {
            float uu = 0.25;//0.25
            float vv = ddupn[ccwc] * uu;			
            layer_demand = UPP + vv;
            ddupn[ccwc] -= vv;
        }
        else
            layer_demand = UPP;

        {
            float ava_nh4, ava_nh3, ava_no3;
            float layer_demand1, layer_demand2, layer_demand3;

            ava_no3 = no3[i];
            ava_nh4 = nh4[i];
            ava_nh3 = nh3[i];

            float TotalAN = no3[i]+ nh4[i] + nh3[i] + 0.0000001;

            layer_demand1 = layer_demand * no3[i] / TotalAN;

            layer_demand2 = layer_demand * nh4[i] / TotalAN;

            layer_demand3 = layer_demand * nh3[i] / TotalAN;

            if ( layer_demand1 <= ava_no3 )
            {
            layer_up_no3 = layer_demand1 ;
            }
            else
            {
            layer_up_no3 = ava_no3;
            }

            if ( layer_demand2 <= ava_nh4 )
            {
            layer_up_nh4 = layer_demand2 ;
            }
            else
            {
            layer_up_nh4 = ava_nh4;
            }

            if ( layer_demand3 <= ava_nh3 )
            {
            layer_up_nh3 = layer_demand3 ;
            }
            else
            {
            layer_up_nh3 = ava_nh3;
            }
        }

        float UPno3, UPnh4, UPnh3;
        if(no3[i] >= layer_up_no3) 
        {
            UPno3 = layer_up_no3;
            no3[i] -= UPno3;
        }
        else
        {
            UPno3 = no3[i];
            no3[i] = 0.0;
        }
        day_nup[ccwc] += UPno3;
    
        if(nh4[i] >= layer_up_nh4) 
        {
            UPnh4 = layer_up_nh4;
            nh4[i] -= UPnh4;
        }
        else
        {
            UPnh4 = nh4[i];
            nh4[i] = 0.0;
        }
        day_nup[ccwc] += UPnh4;

        if(nh3[i] >= layer_up_nh3) 
        {
            UPnh3 = layer_up_nh3;
            nh3[i] -= UPnh3;
        }
        else
        {
            UPnh3 = nh3[i];
            nh3[i] = 0.0;
        }
        day_nup[ccwc] += UPnh3;

        if ( no3[i] < 0.0 ) no3[i] = 0.0;
        if ( nh4[i] < 0.0 ) nh4[i] = 0.0;
        if ( nh3[i] < 0.0 ) nh3[i] = 0.0;
        
        layer_up = UPno3 + UPnh4 + UPnh3;

        if ( layer_demand > layer_up )
        {
             ddupn[ccwc] += (layer_demand - layer_up);
        }
        if ( ddupn[ccwc] < 0.0 ) ddupn[ccwc] = 0.0;

        float aN = day_clay_N - base_clay_N;
        if(aN<0.0) aN=0.0;		

        /*if(ddupn[ccwc]>0.00001&&aN>0.0&&day_clay_N>0.0)
        {
            float tN;
            
            tN = min(ddupn[ccwc], aN*0.05);//0.00005
        
            day_Nuptake_from_clayN += tN;

            day_clay_N -= tN;

            ddupn[ccwc] -= tN;			
        }	*/

    } //end of layer loop

    if(ddupn[ccwc]>0.00001&&WaterControl==2&&day_WT>-0.5&&FloodWater_N>0.0)//wetland plants
    {
            float Wn=0.0;
            
            if(FloodWater_N>0.0)
            {
                if(ddupn[ccwc]>=FloodWater_N)
                {
                    Wn = FloodWater_N; 
                    FloodWater_N = 0.0;
                    ddupn[ccwc] -= Wn;
                }
                else
                {
                    Wn = ddupn[ccwc]; 
                    FloodWater_N -= Wn;
                    ddupn[ccwc] = 0.0;
                }
            }
            
            day_nup[ccwc] += Wn;
    }

    day_nup[ccwc] += day_Nuptake_from_clayN;

    for ( i = 1; i <= q; i++ )
    {
        clay_nh4[i] = day_clay_N * (float)pow((float)0.5, (float)i);
    }

    if (seed[ccwc]==grow_days[ccwc]-1) ddupn[ccwc] = 0.0;

   // N stress
   if(day_N_demand[ccwc] > 0.0)
     N_stress[ccwc] = (day_nup[ccwc]+day_plant_nh3[ccwc]+day_fixN[ccwc])/day_N_demand[ccwc];//day_N_demand;
   else
     N_stress[ccwc] = 1.0;

   N_stress[ccwc] = min(1.0, max(0.0, N_stress[ccwc]));
 
 
}

void class_model::total_crop_N(int ccwc)
{	
    Day_GrowthN[ccwc] = day_nup[ccwc] + day_fixN[ccwc] + day_plant_nh3[ccwc];
    plantn[ccwc] += Day_GrowthN[ccwc];//N existing in plants

    PlantTakeN[ccwc] += Day_GrowthN[ccwc]; //N stored in plants in life cycle	 
    CropSumN[ccwc] += Day_GrowthN[ccwc];//crop life-time obtained N
    yr_plantn[ccwc] += Day_GrowthN[ccwc];//N taken by plants only in this calendar year
    yr_fixn[ccwc] += day_fixN[ccwc];//N fixed by plants in this calendar year
    if(Day_GrowthN[ccwc]>0.0) CropEndN[ccwc] = plantn[ccwc];
}

void class_model::biomass_partition(int ccwc)
{
   float SNS, f_shoot=0.0;

   SNS = crop_N_stress[ccwc]/crop_days[ccwc];

   float CTF, GST, GGG;				

    if(C_type[ccwc]==4) 
    {
        CTF = 0.5;//1.0;
        GST = 0.45;
        GGG = 2.0;
    }
    else 
    {
        CTF = 0.2;//0.4;
        GST = 0.5;
        GGG = 1.7;
    }

   if (PGI[ccwc] > 0.0)
   {
        if(crop[ccwc]==7) GST = 0.2;
        else if(crop[ccwc]==53) GST = 0.3;

        //f_root[ccwc] = CTF * (root[ccwc] + (0.6-root[ccwc])*(0.9-PGI[ccwc])/0.9);//0.4
        //if(f_root[ccwc]<root[ccwc]) f_root[ccwc] = root[ccwc];

        if(crop[ccwc]==7) f_root[ccwc] = root[ccwc];//sugarcane

        if(LandUse==6)//trees
        {
            /*if(PGI[ccwc] < GST) 
            {
                float TBN = 1.0/leafcn[ccwc] + 1.0/stemcn[ccwc] + 1.0/rootcn[ccwc];
                f_grain[ccwc] = 0.0;
                f_leaf[ccwc] = (1.0/leafcn[ccwc]) / TBN;
                f_stem[ccwc] = (1.0/stemcn[ccwc]) / TBN;
                f_root[ccwc] = (1.0/rootcn[ccwc]) / TBN;
            }
            else */
            {
                f_grain[ccwc] = grain[ccwc];				
                f_leaf[ccwc] = leaf[ccwc];
                f_stem[ccwc] = stem[ccwc];
                f_root[ccwc] = root[ccwc];
            }
        }
        else
        {
            float LST = leaf[ccwc] + stem[ccwc];

            f_root[ccwc] = CTF * (root[ccwc] + (0.6-root[ccwc])*(0.9-PGI[ccwc])/0.9);//0.4
            if(f_root[ccwc]<root[ccwc]) f_root[ccwc] = root[ccwc];

            if(PGI[ccwc] < GST) 
            {
                f_grain[ccwc] = 0.0;
                f_leaf[ccwc] = leaf[ccwc] / LST * (1.0 - f_root[ccwc]);
                f_stem[ccwc] = stem[ccwc] / LST * (1.0 - f_root[ccwc]);
            }
            else 
            {
                f_grain[ccwc] = GGG * (PGI[ccwc] - GST) / (1.0 - GST) * grain[ccwc];
                if(f_grain[ccwc]>grain[ccwc]) f_grain[ccwc] = grain[ccwc];

                f_leaf[ccwc] = leaf[ccwc] / LST * (1.0 - f_root[ccwc] - f_grain[ccwc]);
                f_stem[ccwc] = stem[ccwc] / LST * (1.0 - f_root[ccwc] - f_grain[ccwc]);
            }			
        }
    
   }
   else
   {
     f_root[ccwc] = root[ccwc];
     f_grain[ccwc] = grain[ccwc];
     f_leaf[ccwc] = leaf[ccwc];
     f_stem[ccwc] = stem[ccwc];
   }

   // effect of N stress
    if(LandUse==6)//trees
    {
        Day_C_growth[ccwc] = plantcn[ccwc] * Day_GrowthN[ccwc];
        dayG_grain[ccwc] = Day_C_growth[ccwc] * f_grain[ccwc];
        dayG_root[ccwc] = Day_C_growth[ccwc] * f_root[ccwc];
        dayG_leaf[ccwc] = Day_C_growth[ccwc] * f_leaf[ccwc];
        dayG_stem[ccwc] = Day_C_growth[ccwc] * f_stem[ccwc];
        
        YrG_grain[ccwc] += dayG_grain[ccwc];
        YrG_leaf[ccwc] += dayG_leaf[ccwc];
        YrG_stem[ccwc] += dayG_stem[ccwc];
        YrG_root[ccwc] += dayG_root[ccwc];

        Root_Wt[ccwc] += dayG_root[ccwc];
        Grain_Wt[ccwc] += dayG_grain[ccwc];
        Leaf_Wt[ccwc] += dayG_leaf[ccwc];
        Stem_Wt[ccwc] += dayG_stem[ccwc];

    }
    else if(grain[ccwc]<0.05)//grass, sugarcane, tea
    {
       PresentBiomass[ccwc] = Root_Wt[ccwc] + Grain_Wt[ccwc] + Leaf_Wt[ccwc] + Stem_Wt[ccwc];
       float RootFraction = Root_Wt[ccwc] / PresentBiomass[ccwc];
       
       Day_C_growth[ccwc] = plantcn[ccwc] * Day_GrowthN[ccwc];
       dayG_grain[ccwc] = Day_C_growth[ccwc] * f_grain[ccwc];
       dayG_root[ccwc] = Day_C_growth[ccwc] * f_root[ccwc];
       dayG_leaf[ccwc] = Day_C_growth[ccwc] * f_leaf[ccwc];
       dayG_stem[ccwc] = Day_C_growth[ccwc] * f_stem[ccwc];		

        float Fls;
        Fls = 0.25 * PGI[ccwc] + 0.46;

        Grain_Wt[ccwc] += dayG_grain[ccwc];
        Root_Wt[ccwc] += dayG_root[ccwc];
        
        //if(crop[ccwc]==7)// sugarcane
        //{
        //	Leaf_Wt[ccwc] += dayG_leaf[ccwc];
        //	Stem_Wt[ccwc] += dayG_stem[ccwc];
        //}
        //else
        //{
            Leaf_Wt[ccwc] += dayG_leaf[ccwc];
            Stem_Wt[ccwc] += dayG_stem[ccwc];			
        //}

        YrG_grain[ccwc] += dayG_grain[ccwc];
        YrG_leaf[ccwc] += dayG_leaf[ccwc];
        YrG_stem[ccwc] += dayG_stem[ccwc];
        YrG_root[ccwc] += dayG_root[ccwc];
        ProdC[ccwc] = YrG_grain[ccwc] + YrG_leaf[ccwc] + YrG_stem[ccwc] + YrG_root[ccwc]; 
    }
    else//if(grain[ccwc]>0.05)//normal annual crops
    {
        Day_C_growth[ccwc] = plantcn[ccwc] * Day_GrowthN[ccwc];

        if(PGI[ccwc] < GST) 
        {			
            dayG_grain[ccwc] = Day_C_growth[ccwc] * f_grain[ccwc];
            dayG_root[ccwc] = Day_C_growth[ccwc] * f_root[ccwc];
            dayG_leaf[ccwc] = Day_C_growth[ccwc] * f_leaf[ccwc];
            dayG_stem[ccwc] = Day_C_growth[ccwc] * f_stem[ccwc];
        }
        else
        {
            dayG_grain[ccwc] = Day_C_growth[ccwc];
            dayG_root[ccwc] = 0.0;
            dayG_leaf[ccwc] = 0.0;
            dayG_stem[ccwc] = 0.0;
        }
        
        YrG_grain[ccwc] += dayG_grain[ccwc];
        YrG_leaf[ccwc] += dayG_leaf[ccwc];
        YrG_stem[ccwc] += dayG_stem[ccwc];
        YrG_root[ccwc] += dayG_root[ccwc];

        PresentBiomass[ccwc] = plantcn[ccwc] * plantn[ccwc];  //kg C/ha

        if(day_nup[ccwc]>0.0)
        {
            Root_Wt[ccwc] = PresentBiomass[ccwc] * f_root[ccwc];
            Grain_Wt[ccwc] = PresentBiomass[ccwc] * f_grain[ccwc];
            Leaf_Wt[ccwc] = PresentBiomass[ccwc] * f_leaf[ccwc];
            Stem_Wt[ccwc] = PresentBiomass[ccwc] * f_stem[ccwc];

/*			Root_Wt[ccwc] += dayG_root[ccwc];
            Grain_Wt[ccwc] += dayG_grain[ccwc];
            Leaf_Wt[ccwc] += dayG_leaf[ccwc];
            Stem_Wt[ccwc] += dayG_stem[ccwc];*/
        }

/*		if(day_nup[ccwc]==0.0)
        {			
            if(Grain_Wt[ccwc]==0.0)
            {
                f_stem[ccwc] += f_grain[ccwc];
                f_grain[ccwc] = 0.0;
            }

            if((Leaf_Wt[ccwc]+Stem_Wt[ccwc])==0.0)
            {
                f_root[ccwc] += (f_leaf[ccwc]+f_stem[ccwc]);
                f_leaf[ccwc] = 0.0;
                f_stem[ccwc] = 0.0;
            }
        }
        else
        {
        }

        Root_Wt[ccwc] = PresentBiomass[ccwc] * f_root[ccwc];
        Grain_Wt[ccwc] = PresentBiomass[ccwc] * f_grain[ccwc];
        Leaf_Wt[ccwc] = PresentBiomass[ccwc] * f_leaf[ccwc];
        Stem_Wt[ccwc] = PresentBiomass[ccwc] * f_stem[ccwc];

        float Fls = 0.25 * PGI[ccwc] + 0.50;//0.46; 
        float Tshoot = Leaf_Wt[ccwc]+Stem_Wt[ccwc];
        Leaf_Wt[ccwc] = Tshoot * (1.0 - Fls);
        Stem_Wt[ccwc] = Tshoot * Fls;
*/
        //Day_C_growth[ccwc] = plantcn[ccwc] * Day_GrowthN[ccwc];
        
       float TTBIO = Root_Wt[ccwc] + Grain_Wt[ccwc] + Leaf_Wt[ccwc] + Stem_Wt[ccwc]; 	
    }

    APT[ccwc] = Day_C_growth[ccwc] * 2.5 * wreq[ccwc] / 1000.0 / 10000.0; //actual daily crop transpiration, m water
    DayAT += APT[ccwc];//  Day_C_growth[ccwc] * 2.5 * wreq[ccwc] / 1000.0 / 10000.0;//daily all crop transpiration

    float TPN = Leaf_Wt[ccwc]/leafcn[ccwc] + Stem_Wt[ccwc]/stemcn[ccwc] + Root_Wt[ccwc]/rootcn[ccwc] + Grain_Wt[ccwc]/graincn[ccwc];

    if(TPN>0.0)
    {
        LeafN[ccwc] = (Leaf_Wt[ccwc]/leafcn[ccwc]) / TPN * plantn[ccwc];
        StemN[ccwc] = (Stem_Wt[ccwc]/stemcn[ccwc]) / TPN * plantn[ccwc];
        RootN[ccwc] = (Root_Wt[ccwc]/rootcn[ccwc]) / TPN * plantn[ccwc];
        GrainN[ccwc] = (Grain_Wt[ccwc]/graincn[ccwc]) / TPN * plantn[ccwc];
    }

    dayCropDeficit += Day_C_growth[ccwc] / ideal_DayCropC[ccwc] / CropNumbers;

    //deep_doc += (0.2 * dayG_root[ccwc]);
    //deep_doc += (0.2 * Day_C_growth[ccwc]);

    if(Root_Wt[ccwc]>0.0) 
    {
        root_length[ccwc] = max_root_length[ccwc] * PGI[ccwc] * 2.0;
        if(root_length[ccwc] > max_root_length[ccwc]) root_length[ccwc] = max_root_length[ccwc];
    }
    else
        root_length[ccwc] = 0.0;

    float Ftt = min(30.0, max(0.0,air_temp));

    float f_PGI=PGI[ccwc];

    if(PGI[ccwc]>0.4) f_PGI=0.4;
    
    //root_doc[ccwc] += (2.0-PGI[ccwc]) * (float)pow(dayG_root[ccwc], 0.5) * 2.0;//Root_Wt[ccwc] * 0.1;

    if(flood_flag==1&&Root_Wt[ccwc]>0.0)
    {
        for(int l=1;l<=q;l++) R_Wt[l]=Root_Wt[ccwc]*(float)pow(0.5, l);//kg C/ha/layer
    }

    day_P_demand[ccwc] = 0.01 * CropPP * Day_C_growth[ccwc] / 0.4;//daily P demand, kg P/ha
    float dResi=day_P_demand[ccwc];
    for(int l=1;l<=q;l++) 
    {
        float dlu = 0.0;
        if(LabP[l]>=dResi) 
        {
            dlu = dResi;
            LabP[l] -= dlu;
        }
        else
        {
            dlu = LabP[l];
            LabP[l] = 0.0;
        }

        PlantP[ccwc] += dlu;//kg P/ha/layer
        dayPlantP += dlu;
    }

    //---  Rice root distribution
/*	if(wetland_flag==1)
    {
        float soil_depth=0.0;
        float ratio, root_f;
        float root_upper = -0.3004 * seed[ccwc] + 7.7072;
        if (root_upper < 0.0) root_upper = 0.0;
        if (root_upper > 5.0) root_upper = 5.0;

        float root_bottom = 1.2291 * seed[ccwc] - 5.7137;
        if (root_bottom < 5.0) root_bottom = 5.0;
        if (root_bottom > 20.0) root_bottom = 20.0;

        for(int l=1; l<=q; l++)
        {		
            soil_depth = h*100.0*(float)l;  //depth, cm
     
            if (soil_depth<=root_upper || soil_depth>=root_bottom) root_f=0.0;
            else 
            {
                ratio=(soil_depth-root_upper)/(root_bottom-root_upper);

                if (ratio<=0.0) ratio=0.0;
                if (ratio>1.0) ratio=1.0;

                root_f=0.00856+18.845*exp(-1.0*(ratio-(-0.51152))/0.13009);
                //function used to calculation root_f is given based on
                //measured data of P. Frenzel et al., 1999.

            }
            
            if (root_f<0.0) root_f=0.0;
        }
    }
*/
}

void class_model::ini_crop(int MODE, int ccwc)
{
    int i;
    if(jday==jdseed[ccwc])
    {
        WiltFlag[ccwc]=0;
        read_in_vege(MODE, 1, ccwc);

        if(crop[ccwc]==20||crop[ccwc]==64||crop[ccwc]==65) //irrigated paddy rice
        {
            wetland_flag = 1;
            IrriRice_flag = 1;
            RainfedRice_flag = 0;
            DeepRice_flag = 0;
            rice_maturity_flag = 0;
        }		
        else if(crop[ccwc]==46)//rainfed rice
        {
            wetland_flag = 1;
            IrriRice_flag = 0;
            RainfedRice_flag = 1;
            DeepRice_flag = 0;
        }
        else if(crop[ccwc]==30)//deep-water rice
        {
            wetland_flag = 1;
            IrriRice_flag = 0;
            RainfedRice_flag = 0;
            DeepRice_flag = 1;
        }
        else
        {
            wetland_flag = 0;
            IrriRice_flag = 0;
            RainfedRice_flag = 0;
            DeepRice_flag = 0;
        }

        seed[ccwc] = 1;
        crop_flag[ccwc] = 1;
        Grain_Wt[ccwc] = 0.0;
        Leaf_Wt[ccwc] = 0.0;
        Stem_Wt[ccwc] = 0.0;
        Root_Wt[ccwc] = 0.0;
        MaxLeaf[ccwc] = 0.0;
        MaxStem[ccwc] = 0.0;
        MaxRoot[ccwc] = 0.0;
        MaxGrain[ccwc] = 0.0;
    }
}

void class_model::growth_stage(int ccwc)
{
    float lai1, lai2;
    float specific_leaf_weight; //kg C/ha

    if (crop[ccwc]>0&&seed[ccwc]>0)
    {
        float T_limit, LLT;
        
        if (crop[ccwc]==2) //winter crops
            T_limit = WinterWheat_LimitiT;
        else T_limit = 0.0;

        if(LandUse==4) LLT = min_temp;//dry grassland
        else LLT = air_temp;

        //if(LLT>=0.0)
        if(LLT>=T_limit)
        {
          day_TDD[ccwc] = LLT;
        }
        else
        {
          day_TDD[ccwc] = 0.0;
          //TDD[ccwc] = 0.0;
        }

        if (TDD[ccwc] < max_TDD[ccwc]) 
            TDD[ccwc] += day_TDD[ccwc];
        
        if(GrassFlag[ccwc]==1&&air_temp<=5.0) 
            PGI[ccwc] -= 0.1;
        /*else if(crop[ccwc]>=70)
            PGI[ccwc] = TDD[ccwc] / max_TDD[ccwc];*/
        else
            PGI[ccwc] = TDD[ccwc] / max_TDD[ccwc];

        PGI[ccwc] = max(0.0, min(1.0, PGI[ccwc]));

        specific_leaf_weight = 1500.0;//2.0 * (1620.3 * PGI[ccwc] + 64.0);// kg C/ha per LAI

        lai[ccwc] = (Leaf_Wt[ccwc] + Stem_Wt[ccwc] + Grain_Wt[ccwc]) / specific_leaf_weight * lai_index[ccwc];
    }
    else
    {
        lai[ccwc] = 0.0;
        day_TDD[ccwc] = 0.0;
        PGI[ccwc] = 0.0;
    }

    u_PGI = PGI[ccwc];
}

void class_model::root_respir(int ccwc)
{
    float Tfactor, RootAge, age;
    int i;

    if (air_temp<= 0.0)
    {
       Tfactor = 0.0;
    }
    else
    {
       Tfactor = air_temp / 35.0;
    }

    if ( seed[ccwc] > 0 )
    {
        age=(float)seed[ccwc]/(float)grow_days[ccwc];
        if(age<0.0) age = 0.0;
      
      RootAge = -2.1091*age*age + 2.7486*age + 0.0614;
      if(RootAge < 0.0) RootAge = 0.0;
    }
    else
    {
      RootAge = 0.0;
    }

    if(wetland_flag==1)
    {
        float soilte = max(4.0, (float)st[1]); 
        float SoilD = 6.0 / soilte;
        float RootF;
        if(PGI[1]<0.5) RootF = PGI[1];
        else RootF = 1.0 - PGI[1];
        RootF = max(0.0, min(1.0, RootF));

        day_rco2[ccwc] = 0.1 * Root_Wt[ccwc] * RootF * Tfactor * SoilD;//RootAge

    }
    else if(crop[ccwc]==30)
    {
        float SoilD = 6.0 / (float)st[1];
        
        day_rco2[ccwc] = 500.0 * RootAge * Tfactor * SoilD; 
    }
    else if(crop[ccwc]==11||crop[ccwc]==12)//grass
    {
        float AgeFactor;
        if(PGI[ccwc]<1.0) AgeFactor = -2.352*PGI[ccwc]*PGI[ccwc] + 1.645*PGI[ccwc] +0.7561;
        else AgeFactor = 0.1;
        if(AgeFactor<0.0) AgeFactor=0.0;

        float Tf;
        if(air_temp>0.0) Tf = air_temp/1.0;
        else Tf = 0.0;

        day_rco2[ccwc] = 0.1 * Root_Wt[ccwc] * Tfactor * AgeFactor * (float)(365-jday)/365.0 + Tf;//
    }
    else
    {
        float AgeFactor;
        if(PGI[ccwc]<1.0) AgeFactor = -2.8348*PGI[ccwc]*PGI[ccwc] + 2.4848*PGI[ccwc] + 0.4554;//1.0 + (1.0-PGI)/(1.0+PGI);
        else AgeFactor = 0.1;
        if(AgeFactor<0.0) AgeFactor=0.0;
        
        if(crop[ccwc]==2) 
        {
            Tfactor = (air_temp - 4.0) / 4.0;
            Tfactor = max(0.0, Tfactor);
        }
        else
        {
            if(PGI[ccwc]>0.4&&PGI[ccwc]<0.7) 
            {
                Tfactor = (air_temp - 10.0) / 6.0;
                Tfactor = max(0.0, Tfactor);
            }
        }

        day_rco2[ccwc] = 0.025 * Root_Wt[ccwc] * Tfactor;//0.05
    }

    if(LandUse==6)
    {
        day_rco2[ccwc] = 0.001 * Root_Wt[ccwc] * Tfactor;
    }
    
    for(i=1; i<=q; i++)
    {
        root_CO2[i] = (day_rco2[1]+day_rco2[2]+day_rco2[3]) / (float)q;
    }

    //root exudation
    float TotalRootWT = Root_Wt[ccwc]; 
    
    if (TotalRootWT>0.0&&flood_flag==0)
    {
         float bq = 0.2;
         float fPGI = 1.0;
         if(u_PGI>0.5) 
         {
             fPGI = 1.0 - u_PGI * 0.5;
             fPGI *= 0.25;//Modified for CA rice of Maegen case ???
         }		
         
        day_RootInput_DOC = 0.2 * TotalRootWT * fPGI * bq * 0.3;//(kg C/ha/layer)
           
        for(i=1; i<=q; i++)
        {
            float RRdoc = (day_RootInput_DOC / (float)q);
            doc[i] += RRdoc;
            day_DOC_prod += RRdoc;
        }
    }
    else
    {
       
    }     
}

void class_model::water_demand(float DayPET, int ccwc)
{

    float FPT; 
    if(plantn[ccwc]>0.0&&TDD[ccwc]<=max_TDD[ccwc])//&&crop_PT[ccwc]<=CropWaterDemand[ccwc])
    {
        PPT[ccwc] = CropWaterDemand[ccwc] * (day_TDD[ccwc] / max_TDD[ccwc]);

        if(PPT[ccwc]>DayPET) PPT[ccwc] = DayPET;
    }
    else
    {
        PPT[ccwc] = 0.0;
    }
}

float class_model::crop_transpiration(float DayPT0, int irri_close)
{
    int l, hr=1;
    float Fptt, DayDeficit, LayerWaterDemand, LayerTrans, limit1, DayPT1=0.0, DayPT2=0.0;
    int root_layer = (int)(root_length[1]/h[1])+1; 
    if(root_layer>q) root_layer = q;

    root_layer = q;//???

    LayerDeficit = 0.0;

    for (l=1; l<=root_layer; l++)
    {
        float wiltptw = h[l] * sts[l] * wiltpt[l];
        float fldcapw = h[l] * sts[l] * fldcap[l];

        if(FilmEffect>0.0) wiltptw *= (1.0 - FilmEffect);

        LayerWaterDemand = DayPT0 / root_layer;
            
        if(LayerDeficit>0.0) 
        {
            LayerWaterDemand += LayerDeficit;
            LayerDeficit = 0.0;
        }

        if(water[1][l]<=wiltptw)
        {
            LayerTrans = 0.0;
        }
        else
        {
            limit1 = ((water[1][l] - wiltptw) / (fldcapw - wiltptw));
            if(limit1<0.0) limit1 = 0.0;

            limit1 = (float)pow(limit1, clay_f2);

            Fptt = (water[1][l] - wiltptw) * (0.001+0.4/(float)pow((float)l, (float)2.0)) * limit1 * clay_f1;

            if(Fptt>=LayerWaterDemand)
            {
                LayerTrans = LayerWaterDemand;
            }
            else
            {
                LayerTrans = Fptt;
            }

            water[1][l] -= LayerTrans;
            
            if(water[1][l]<0.0)
            {
                LayerTrans += water[1][l];
                water[1][l] = 0.0;
            }
        }
            
        LayerDeficit += LayerWaterDemand - LayerTrans;// m water
                    
        DayPT1 += LayerTrans;//m water
        LayerTrans = 0.0;
            
        DayDeficit = LayerDeficit;// m water
                
        if(DayPT1>=DayPT0) break;

        if(l==q)
        {
            //contribution of deep water pool to transpiration

            if(DayDeficit>0.0&&WaterPool>0.0001&&total_ice<=0.0) 
            {
                float add_water2=0.0;

                if(DayDeficit<=WaterPool*0.5)
                {
                    add_water2 = DayDeficit;
                    DayDeficit = 0.0;
                }
                else
                {
                    add_water2 = WaterPool*0.5;
                    DayDeficit -= add_water2;
                }

                WaterPool -= add_water2;
                DayPT2 += add_water2;
                add_water2 = 0.0;
                                   
                /*if(-HighestWTdepth>=-0.5)// in case of high ground water table
                {
                    add_water2 = DayDeficit * 0.01;//introduce imbalance due to additional water
                    DayPT2 += add_water2;
                    day_GWsuply += add_water2;
                    add_water2 = 0.0;
                }*/
            }
        }
        else
        {
            DayPT2 = DayPT1;
        }
    }//end of root layer loop

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(DayDeficit>0.0&&irri_index>0.0&&irri_close==0&&RainfedRice_flag==0) //auto-irrigation
    {
        float add_water1, left_water, IRRIID;
                    
        //if(DroughtID==1&&drc>=4&&jday>=YieldToday&&NoRainFlag==0) //for Drought-DNDC
        //	IRRIID = 1.0;
        //else
        IRRIID = irri_index;
                        
        if(IrriMethod==0)//furrow irrigation
            add_water1 = 2.5 * DayDeficit * IRRIID;	
        else if(IrriMethod==1)//sprinkler
            add_water1 = 2.0 * DayDeficit * IRRIID;
        else if(IrriMethod==2)//dripping
            add_water1 = 1.5 * DayDeficit * IRRIID;
        else if(IrriMethod==3)//dripping + film mulch
            add_water1 = 1.0 * DayDeficit * IRRIID;
        else if(IrriMethod==4)//film mulch
            add_water1 = 2.5 * DayDeficit * IRRIID;

        if(add_water1>=DayDeficit)
        {
            DayPT2 = DayPT0;
            left_water = add_water1 - DayDeficit;
            DayDeficit = 0.0;
        }
        else
        {
            DayPT2 += add_water1;
            left_water = 0.0;
            DayDeficit -= add_water1;
        }

        water[1][1] += left_water; //surf_water += left_water;
                        
        //rainfall += add_water1;
        TTT = (rainfall+add_water1) / 0.005;
        yr_irri += add_water1;
        day_irri += add_water1;
        add_water1 = 0.0;
    }
                
    return(DayPT2);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    
/* ???????????????????????????????????

        //supplement of deep water pool to crop transpiration
        float WDi = hrPT0 - *hrTrans;
        if (*hrTrans>0.0&&WDi>0.0)
        {
            //if (WDi>0.0)
            //{
                if (WDi<=WaterPool )
                {
                    float tw = 1.0 * WDi;
                    *hrTrans += tw;
                    if(*hrTrans < 0.0) *hrTrans = 0.0; //

                    WaterPool -= tw;

                    WDi -= tw;

                }
                else
                {
                    if(WRL>q&&WTL>q)
                    {				
                        if ( fabs(WaterPool) < 0.0000001 )
                        {
                            for ( l = 1; l <= q; l++ )
                            {
                                float wiltptw = h[l] * sts[l] * wiltpt[l];
                                float fldcapw = h[l] * sts[l] * fldcap[l];

                                if(FilmEffect>0.0) wiltptw *= (1.0 - FilmEffect);

                                if ( water[hr][l] <= 0.000000001)
                                {
                                }
                                else
                                {
                                    if ((water[hr][l] - wiltptw)>WDi/(float)q * 0.8 )
                                    {
                                        *hrTrans += ((float)(WDi / (float)q * 0.8));
                                        if(*hrTrans < 0.0) *hrTrans = 0.0; //

                                        water[hr][l] -= ((float)(WDi / (float)q * .8));
                                        if(water[hr][l]<0.0) water[hr][l] = 0.0;
                                    }
                                    else
                                    {
                                        if(water[hr][l] > wiltptw)
                                        {
                                            *hrTrans += ((water[hr][l] - wiltptw) * .8);
                                            if (*hrTrans < 0.0) *hrTrans = 0.0;

                                            water[hr][l] -= (water[hr][l] - wiltptw) * .8;
                                            if(water[hr][l]<0.0) water[hr][l] = 0.0;
                                        }
                                    }
                                }
                                
                            }//end of l loop
                        }
                        else//if WaterPool > 0.00001
                        {
                            *hrTrans += (WaterPool);
                            if(*hrTrans < 0.0) *hrTrans = 0.0; 

                            WaterPool = 0.0;

                            WDi = 0.0;
                        }
                    }
                    else//if(WRL <= q || WTL < q)
                    {
                        *hrTrans += ((float)(WDi / (float)q));
                    }
                }
            //}

        }
// ???????????????????????????????????????????????????
*/

}

void class_model::root_develop(int ccwc)
{
    if(jday==1&&(crop[1]==11||crop[1]==12)) 
        root_depth[ccwc] = 0.2;
    else
        root_depth[ccwc] = 0.05 + PGI[ccwc] * 0.5;//m

    root_q = (int)(root_depth[ccwc]/h[1])+1;//????
}


void class_model::weather_damage(int damage, float w_event)
{
    float Fdamage = w_event;
    float CropNLost;
    Fdamage = max(0.0, min(1.0, Fdamage));

    float hail_damage_N = 0.0;

    for(int i=1; i<=CropNumbers; i++)
    {
        CropNLost = plantn[i] * Fdamage;
        hail_damage_N += CropNLost;
        plantn[i] -= CropNLost;

        Root_Wt[i] *= (1.0 - Fdamage);
        Grain_Wt[i] *= (1.0 - Fdamage);
        Leaf_Wt[i] *= (1.0 - Fdamage);
        Stem_Wt[i] *= (1.0 - Fdamage);
        plantn[i] *= (1.0 - Fdamage);
        PlantTakeN[i] *= (1.0 - Fdamage);
        maxmn[i] *= (1.0 - Fdamage);
    }
}

void class_model::frost_damage(int ccwc)
{
    float FiD;

    //for(ccwc=1; ccwc<=CropNumbers; ccwc++)
    {
        if(crop[ccwc]!=2&&PGI[ccwc]<0.5)//not winter wheat
        {
            FiD = 1.0 + 0.08 * min_temp;
            if(FiD<0.0) FiD=0.0;

            Root_Wt[ccwc] *= FiD;
            Grain_Wt[ccwc] *= FiD;
            Leaf_Wt[ccwc] *= FiD;
            Stem_Wt[ccwc] *= FiD;
            plantn[ccwc] *= FiD;
            PlantTakeN[ccwc] *= FiD;
        }
    }
}

void class_model::nh3_uptake_release(float *lai, float *day_soil_nh3, float *PlantTakeN, float *maxmn,
                                     float leaf_water, float cropht, float base_nh3,
                                     float *day_N_demand, float *day_nup, float *day_fixN, float *day_DWater,
                                     float *day_plant_nh3, float *yr_NH3_deposit, int ccwc)
{
    float max_leaf_water;
    float fpn, flm, vg, vca, x;

    if (lai[ccwc] > 0.0 )
    {
        max_leaf_water = lai[ccwc] * .0002;

        fpn = 1.0 - PlantTakeN[ccwc] / maxmn[ccwc];
        if ( fpn < 0.0 ) fpn = 0.0;
        flm = 1.0;//leaf_water / max_leaf_water + 0.5;
        if ( flm > 1.0 ) flm = 1.0;
        vg = fpn * flm;//0.05
        vca = 0.3 * 10000.0;//cropht * 10000.0;
        //x = ((*day_soil_nh3 * (float)pow(10.0, 9.0) / vca * lai[ccwc] / (lai[ccwc] + 10.0) * .01));
        x = ((0.1 * (float)pow(10.0, 9.0) / vca * lai[ccwc] / (lai[ccwc] + 10.0) * .01));
        if (x < 0.0) x = 0.0;
        //if(x>1.0)
        //	air_nh3 = base_nh3 * (float)pow(x, 0.5);//0.5
        //else
        //	air_nh3 = base_nh3;

        //air_nh3 = base_nh3;

        float dD=0.0, pP=0.0;
        if(maxmn[ccwc]>plantn[ccwc])
        {
            //float dD = 1.0 * day_N_demand[ccwc] - day_nup[ccwc] - day_fixN[ccwc];
            dD = 0.1 * day_N_demand[ccwc];// - day_nup[ccwc] - day_fixN[ccwc];
            pP = vg * base_nh3 * lai[ccwc] * day_DWater[ccwc];// 0.864 *
        }

        if(N_stress[ccwc]<1.0&&day_N_demand[ccwc]>0.0) //dD>0.0)
        {
            //day_plant_nh3[ccwc] = 0.0;
            //if(dD>pP) day_plant_nh3[ccwc] = 0.9 * pP;//N uptake from atmospheric NH3 by crop
            //else day_plant_nh3[ccwc] = 0.9 * dD;

            dD = 1.0 * (1.0 - N_stress[ccwc]) / (1.0 + N_stress[ccwc]) * base_nh3 / (1.0 + base_nh3);//kg N/ha/day
            day_plant_nh3[ccwc] = dD;
        }
        else
            day_plant_nh3[ccwc] = 0.0;
    }
    else
    {
        day_plant_nh3[ccwc] = 0.0;

    }

    crop_NH3_absorp[ccwc] += day_plant_nh3[ccwc];
    *yr_NH3_deposit += day_plant_nh3[ccwc];
}

void class_model::crop_senescence(int ccwc)
{
    //All the litter from senescence will be incorporated in the soil
    float dPG, dPL, dPW, dPR, sLeaf=0.0, sStem=0.0, sGrain=0.0, sRoot=0.0;
    float AddC, AddN, AddCN, dInertC, AddP;
    float AddC1, AddC2, AddC3;

    if(jday>300||min_temp<=0.0)
    {
        if(LandUse==6)//trees
        {
            dPG = 0.15;
            dPL = 0.15;
            //dPW = 0.01 * stem_age - 0.0014;
            dPW = 0.0005 * stem_age;
            dPR = 0.003;

            sGrain = Grain_Wt[ccwc] * dPG;
            sLeaf = Leaf_Wt[ccwc] * dPL;
            sStem = Stem_Wt[ccwc] * dPW * 0.015;
            sRoot = Root_Wt[ccwc] * dPR;
                
            Grain_Wt[ccwc] -= sGrain;
            Leaf_Wt[ccwc] -= sLeaf;
            Stem_Wt[ccwc] -= sStem;
            Root_Wt[ccwc] -= sRoot;			
        }
        else if(crop[ccwc]==12&&PerennialFlag[ccwc]==1)//perennial grass
        {
                GrowStartDay=60;
                
                if(jday==300)//maturaty_flag[ccwc]==1||
                {
                    MaxLeaf[ccwc] += Leaf_Wt[ccwc];
                    MaxStem[ccwc] += Stem_Wt[ccwc];
                    MaxRoot[ccwc] += Root_Wt[ccwc];
                    MaxGrain[ccwc] += Grain_Wt[ccwc];
                }

                if(jday<=150||jday>=200)//(maturaty_flag[ccwc]>2||jday<=GrowStartDay) 
                {
                    if(air_temp>0.0) 
                        dPG = 0.01;// * (50.0 - air_temp); //Sheehy et al. 1980
                    else dPG = 0.02;
                }
                else dPG = 0.0;

                if(dPG>0.0)
                {
                    sLeaf = Leaf_Wt[ccwc] * dPG;
                    sStem = Stem_Wt[ccwc] * dPG;
                    sRoot = Root_Wt[ccwc] * dPG;
                    sGrain = Grain_Wt[ccwc] * dPG;

                    Leaf_Wt[ccwc] -= sLeaf;
                    Stem_Wt[ccwc] -= sStem;
                    Root_Wt[ccwc] -= sRoot;
                    Grain_Wt[ccwc] -= sGrain;
                }
        }
        else if(crop[ccwc]==11)//annual grass
        {
                GrowStartDay=60;
                
                if(jday==300)//maturaty_flag[ccwc]==1||
                {
                    MaxLeaf[ccwc] += Leaf_Wt[ccwc];
                    MaxStem[ccwc] += Stem_Wt[ccwc];
                    MaxRoot[ccwc] += Root_Wt[ccwc];
                    MaxGrain[ccwc] += Grain_Wt[ccwc];
                }

                if(jday>=300)//(maturaty_flag[ccwc]>2||jday<=GrowStartDay) 
                {
                    if(air_temp>0.0) 
                        dPG = 0.1;// * (50.0 - air_temp); //Sheehy et al. 1980
                    else dPG = 0.2;
                }
                else dPG = 0.0;

                if(dPG>0.0)
                {
                    sLeaf = Leaf_Wt[ccwc] * dPG;
                    sStem = Stem_Wt[ccwc] * dPG;
                    sRoot = Root_Wt[ccwc] * dPG;
                    sGrain = Grain_Wt[ccwc] * dPG;

                    Leaf_Wt[ccwc] -= sLeaf;
                    Stem_Wt[ccwc] -= sStem;
                    Root_Wt[ccwc] -= sRoot;
                    Grain_Wt[ccwc] -= sGrain;
                }
        }
        else if(crop[ccwc]==62&&PerennialFlag[ccwc]==1)//sedge
        {
                GrowStartDay=60;
                
                if(jday==300)//maturaty_flag[ccwc]==1||
                {
                    MaxLeaf[ccwc] += Leaf_Wt[ccwc];
                    MaxStem[ccwc] += Stem_Wt[ccwc];
                    MaxRoot[ccwc] += Root_Wt[ccwc];
                    MaxGrain[ccwc] += Grain_Wt[ccwc];
                }

                if(jday>=300)//(maturaty_flag[ccwc]>2||jday<=GrowStartDay) 
                {
                    if(air_temp>0.0) 
                        dPG = 0.1;// * (50.0 - air_temp); //Sheehy et al. 1980
                    else dPG = 0.2;
                }
                else dPG = 0.0;

                if(dPG>0.0)
                {
                    sLeaf = Leaf_Wt[ccwc] * dPG;
                    sStem = Stem_Wt[ccwc] * dPG;
                    sRoot = Root_Wt[ccwc] * dPG;
                    sGrain = Grain_Wt[ccwc] * dPG;

                    Leaf_Wt[ccwc] -= sLeaf;
                    Stem_Wt[ccwc] -= sStem;
                    Root_Wt[ccwc] -= sRoot;
                    Grain_Wt[ccwc] -= sGrain;
                }
        }
        else if(crop[ccwc]==59&&PerennialFlag[ccwc]==1)//moss
        {
                GrowStartDay=60;
                
                if(jday==300)//maturaty_flag[ccwc]==1||
                {
                    MaxLeaf[ccwc] += Leaf_Wt[ccwc];
                    MaxStem[ccwc] += Stem_Wt[ccwc];
                    MaxRoot[ccwc] += Root_Wt[ccwc];
                    MaxGrain[ccwc] += Grain_Wt[ccwc];
                }

                if(jday>=300)//(maturaty_flag[ccwc]>2||jday<=GrowStartDay) 
                {
                    if(air_temp>0.0) 
                        dPG = 0.1;// * (50.0 - air_temp); //Sheehy et al. 1980
                    else dPG = 0.1;
                }
                else dPG = 0.0;

                if(dPG>0.0)
                {
                    sLeaf = Leaf_Wt[ccwc] * dPG;
                    sStem = Stem_Wt[ccwc] * dPG;
                    sRoot = Root_Wt[ccwc] * dPG;
                    sGrain = Grain_Wt[ccwc] * dPG;

                    Leaf_Wt[ccwc] -= sLeaf;
                    Stem_Wt[ccwc] -= sStem;
                    Root_Wt[ccwc] -= sRoot;
                    Grain_Wt[ccwc] -= sGrain;
                }
        }
        else if(crop[ccwc]==52||crop[ccwc]==58)//cattail, tule
        {
                if(jday>=150)
                {
                    if(air_temp>0.0) 
                        dPG = SenesR[crop[ccwc]];//tule: 0.008; cattail: 0.012
                    else dPG = 0.2;

                    if(jday>SenesD[crop[ccwc]]) dPG = 0.05;//tule: 320; cattail: 300
                }
                else dPG = 0.0;

                if(dPG>0.0)
                {
                    sLeaf = Leaf_Wt[ccwc] * dPG;
                    sStem = Stem_Wt[ccwc] * dPG;
                    sRoot = Root_Wt[ccwc] * dPG;
                    sGrain = Grain_Wt[ccwc] * dPG;

                    Leaf_Wt[ccwc] -= sLeaf;
                    Stem_Wt[ccwc] -= sStem;
                    Root_Wt[ccwc] -= sRoot;
                    Grain_Wt[ccwc] -= sGrain;
                }
                else
                {
                    sGrain = 0.0;
                    sLeaf = 0.0;
                    sStem = 0.0;
                    sRoot = 0.0;
                }	
        }
        else
        {
                sGrain = 0.0;
                sLeaf = 0.0;
                sStem = 0.0;
                sRoot = 0.0;

                DayLeafSenes[ccwc]=0.0;
                DayStemSenes[ccwc]=0.0;
                DayRootSenes[ccwc]=0.0;
                DayGrainSenes[ccwc]=0.0;
        }

        if(sGrain+sLeaf+sStem+sRoot>0.0)
        {
                LeafN[ccwc] = (Leaf_Wt[ccwc]/leafcn[ccwc]);// / TPN * plantn[ccwc];
                StemN[ccwc] = (Stem_Wt[ccwc]/stemcn[ccwc]);// / TPN * plantn[ccwc];
                RootN[ccwc] = (Root_Wt[ccwc]/rootcn[ccwc]);// / TPN * plantn[ccwc];
                GrainN[ccwc] = (Grain_Wt[ccwc]/graincn[ccwc]);// / TPN * plantn[ccwc];
            
                plantn[ccwc] = LeafN[ccwc] + StemN[ccwc] + RootN[ccwc] + GrainN[ccwc]; 
                PlantP[ccwc] = CropP[ccwc] * 0.01 * (Leaf_Wt[ccwc]+Stem_Wt[ccwc]+Root_Wt[ccwc]+Grain_Wt[ccwc]) / 0.4;
                DayLeafSenes[ccwc]=sLeaf;
                DayStemSenes[ccwc]=sStem;
                DayRootSenes[ccwc]=sRoot;
                DayGrainSenes[ccwc]=sGrain;
        }
        
        if(sGrain+sLeaf+sStem>0.0)
        {	
            AddN = sLeaf/leafcn[ccwc] + sStem/stemcn[ccwc] + sGrain/graincn[ccwc];

            plantn[ccwc] -= AddN;
            if(plantn[ccwc]<0.0) plantn[ccwc] = 0.0;	

            AddC = sLeaf+sStem+sGrain; 
            AddCN = AddC/AddN;
            AddP = CropP[ccwc] * 0.01 * AddC /0.4;

            alloc_litter_into_pools(&AddCN, &AddC, 1, &AddC1, &AddC2, &AddC3, &dInertC, 0, rcnrvl, rcnrl, rcnrr, nh4);

            float OutN = AddC1/rcnrvl + AddC2/rcnrl + AddC3/rcnrr;

            rcvl[1] += AddC1;
            rcl[1] += AddC2;
            rcr[1] += AddC3;
            OrgP[1] += AddP;

            day_addC += (AddC1 + AddC2 + AddC3);
            day_addrn += (AddC1 / rcnrvl + AddC2 / rcnrl + AddC3 / rcnrr);
            yr_addtc += (AddC1 + AddC2 + AddC3);
            yr_addtn += (AddC1 / rcnrvl + AddC2 / rcnrl + AddC3 / rcnrr);

            AddC1 = 0.0;
            AddC2 = 0.0;
            AddC3 = 0.0;
            AddP = 0.0;
        
            sGrain = 0.0;
            sLeaf = 0.0;
            sStem = 0.0;	
        }		
        
        //root litter incorporation
        if(sRoot>0.0)
        {
            AddCN = rootcn[ccwc];
            for (int l = 1; l <= q; l++ )
            {
                if ( l >= 1 && l <= 3 )
                {
                    AddC = sRoot * 0.5 / 3.0;
                    AddP = CropP[ccwc] * 0.01 /0.4 * sRoot * 0.5 / 3.0;
                }
                if ( l >= 4 && l <= 6 )
                {
                    AddC = sRoot * 0.4 / 3.0;
                    AddP = CropP[ccwc] * 0.01 /0.4 * sRoot * 0.4 / 3.0;
                }
                if ( l >= 7 && l <= q )
                {
                    AddC = sRoot * 0.1 / (float)(q - 6);
                    AddP = CropP[ccwc] * 0.01 /0.4 * sRoot * 0.1 / (float)(q - 6);
                }

                alloc_litter_into_pools(&AddCN, &AddC, 10, &AddC1, &AddC2, &AddC3, &dInertC, 0, rcnrvl, rcnrl, rcnrr, nh4);
                rcvl[l] += AddC1;
                rcl[l] += AddC2;
                rcr[l] += AddC3;
                OrgP[l] += AddP;
                            
                float AddToSoilN = sRoot/AddCN;

                day_addC += (AddC1+AddC2+AddC3);
                yr_addrc += (AddC1+AddC2+AddC3);
                day_addrn += AddToSoilN;
                yr_addrn += AddToSoilN;

                sRoot = 0.0;
                AddC1 = 0.0;
                AddC2 = 0.0;
                AddC3 = 0.0;
                AddP = 0.0;
            }
        }

        PresentBiomass[ccwc] = Root_Wt[ccwc] + Grain_Wt[ccwc] + Leaf_Wt[ccwc] + Stem_Wt[ccwc];
    }
    else
    {
        DayLeafSenes[ccwc]=0.0;
        DayStemSenes[ccwc]=0.0;
        DayRootSenes[ccwc]=0.0;
        DayGrainSenes[ccwc]=0.0;
    }
}

