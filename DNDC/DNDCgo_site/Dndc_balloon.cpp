#include "stdafx.h"
#include "Dndcgo.h"
#include "Source_main.h"
#include "Dndc_tool.h"

#ifdef MANURE 
extern class_manure MData;
#endif

void class_model::N2O_balloon(int GN, float till_fact,
        float *sts, float *h, int jday, int flood_flag, int FloDays, float rainfall,
        float *O2_old, float *O2, float TTT, float *vvv, float *ave_anvf, int q, int *st,
        float *temp, int year, double water[25][180], float *day_wfps, float day_WT,
        int WTL, float total_ice, float *day_rco2, float *co2, int flo_num, float *ice,
        float *sph, float *Eh, float *anvf,
        float *nh4, float *no3, float *no2, float *no, float *n2, float *doc, float *n2o,
        float *doc_w, float *nh4_w, float *no3_w, float *no2_w, float *no_w, float *n2o_w, float *n2_w,
        float *doc_d, float *nh4_d, float *no3_d, float *no2_d, float *no_d, float *n2o_d,
        int LandUse, float *Tttoo, float ManureAddCN, int ManureDays, float *soc, float MicrobioIndex,
        float mRBO, float mFD, int WaterControl, int *crop, float *assim2, float *clay,
        float *fldcap, float by_passf, int NIDay1, int NIDay2,
        float *day_soil_nitrify, float *day_nitrify_NO, float *day_nitrify_N2O, float *day_soil_an_no,
        float *day_soil_an_n2o, float *day_soil_n2, float *Root_Wt, float WaterLeakRate,
        float *rcvl, float *rcl, float *rcr,
        float *day_soil_denitrification, float *wiltpt, float *CRB1, float *CRB2, float rcnb, float *Odoc,
        float *day_frostDOC, float *yr_frostDOC, float precipitation, int manuFlag, int irri_flag,
        float *FreezeDOC, float *nh3, float *Eh1, float *urea, float LateralInflux, int drain_flag,
        int IrriRice_flag)
{
    float KC, KN;
    float u_no3, u_no2, u_no, u_n2o, U, DBG, DB, Nw;
    float d_no3, d_no2, d_no2_kk, d_no, d_n2o;
    float dn_co2, d_co2, bypass_f;
    float Fde=1.0, Fph=1.0, Fmn=1.0, Fst=1.0, Fsm=1.0, Ftm=1.0;

    int   T, l, rain_hrs;
    float assim_N, air_poro_max;
    float TE[MAXSOILLAYER], phk1[MAXSOILLAYER], phk2[MAXSOILLAYER], phk3[MAXSOILLAYER];
    float cck, n3k, n2k, nok, n2o_k, DBD, da_no3;
    float da_no2, da_no2_kk, da_no;
    float da_n2o, DC, day_leach_DOC, PORO;
    float aere=0.0;
    float lost_DOC, lost_no3;
    float leach_factor;
    float fact_t, fact_m, nno;
    float wwzz, ae_n2o, ae_no;
    float micro_growth, micro_death, micro_delta, k, AA;
    int flood_day=0;
    float EEE, RRR, nitrifier[MAXSOILLAYER], denitrifier[MAXSOILLAYER]; 
    int BL, EL;

    if(GN>q)//manure
    {
        BL = GN; EL = GN; 

        till_fact = 1.0;

        //if(GN==(q+5)) temp[GN] *= 0.5;
    }
    else
    {
        BL = 1; EL = q;
    }

    float sts_h = 0.451 * h[1];//sts*h;

    CString DNF;
 
    if (jday==1) flood_day = 0;
    
    if (flood_flag==1) 
    {	
        flood_day = FloDays;
        if (flood_day > 20) flood_day = 20;
    }

    PORO = 0.451 * 4.0;//sts

    if (flood_flag == 0)
    {
        rain_hrs = (int)TTT;
    }
    else
    {
        rain_hrs = 24;
        TTT = 24.0;
    }

    KC = KCI * vvv[GN];
    KN = KNI * vvv[GN];

    if (st[1]>=12) bypass_f = 2.0;//2.0
    else bypass_f = 1.0;
 
    dn_co2 = 0.0;
    assim_N = 0.0;

    air_poro_max = 0.0;

    if(GN>q) ave_anvf[GN] = 1.0;
    else for(l=1; l<=q; l++) ave_anvf[l] = 0.0;

    for ( T = 1; T <= 24; T++ )
    {
        for ( l = BL; l <= EL; l++ )
        {
            float cc=1.0;

            if(l<=q)
            {
                soil_oxygen_profile(l, T, rain_hrs, bypass_f, air_poro_max, flood_day,
                    temp, year, jday, q, water, day_wfps, h, sts, O2, day_WT, WTL, 
                    total_ice, TTT, day_rco2, co2, flo_num, ice);
            }
            else//manure
            {	
                float ps = h[l] * 0.5;//sts[l];
                float actual_poro = ps * (1.0 - day_wfps[l]) + 0.0000001;//10000.0 * (1.0 - water[T][l] / ps) + 0.000001;//m3/layer/ha, actual pores
                float air_O2 = 0.299251; 
                float full_O2 = air_O2 * ps * 10000.0;//g O2/layer/ha
                O2[l] = air_O2 * actual_poro * 10000.0;//g O2/layer/ha
                Eh1[l] = 626.1 * O2[l] / full_O2;
            }
      
            if(Eh1[l]>=0.0) 
                anvf[l] = (1.0 - Eh1[l]/650.0) * 0.1;
            else 
                anvf[l] = 0.99;

            if(st[1]==12&&TTT>T) anvf[l] = 0.99;
            //else anvf[l] = 0.01;

            //if(flood_flag==1) anvf[l] = 0.99;

            anvf[l] = min(0.99, max(0.01, anvf[l])); 
            ave_anvf[l] = anvf[l];

   // Allocate NH4+, NO3-, NO2-, NO, N2O, and DOC to anaerobic balloon 
    if(T==1) 
    {
       //if (temp[l]>9.0) TE[l] = (float)pow(2.0, ( temp[l] - 22.5) / 10.0);
       //else if (temp[l]>0.0) TE[l] = (0.0002 * (float)exp(.8255 * temp[3]));
       if (temp[l]>0.0) TE[l] = 0.001 + temp[l] / 30.0;
       else TE[l] = 0.001;
       
       //if(ice[l]>0.0) TE[l] = 0.01; 			
       
       TE[l] = min(1.0, max(0.0, TE[l]));

       phk1[l] = 2.0 * (1.0 - 1.0 / (1.0 + (float)exp((sph[l] - 4.250) / 0.5))); //0.5

       phk2[l] = 2.0 * (1.0 - 1.0 / (1.0 + (float)exp((sph[l] - 5.250) / 1.0)));
       
       if(sph[l]<=7.0||manuFlag==1)
        phk3[l] = 2.0 * (1.0 - 1.0 / (1.0 + (float)exp((sph[l] - 6.250) / 1.5))); 
       else
        phk3[l] = 5.0 * (float)pow(10.0,-8) * (float)exp(2.5538 * sph[l]); 
        //Michael Dannenmann et al. Soil Biology & Biochemistry 40(2008) 2317-2323.
   
       if ( phk1[l] < 0.0 ) phk1[l] = 0.0;
       if ( phk2[l] < 0.0 ) phk2[l] = 0.0;
       phk3[l] = min(100.0, max(0.0, phk3[l]));
    }

    if (st[l]>=12) AA = 1.0;
    else AA = 1.0;

   nh4_w[l] = (nh4[l]) * anvf[l] * AA;
   nh4_d[l] = ((nh4[l]) - nh4_w[l]);
   nh4[l] = 0.0;
   //nh3[l] = 0.0;

   float pwN = no3[l] * anvf[l] * AA;
   no3_w[l] += pwN;
   no3_d[l] = no3[l] - pwN;
   no3[l] = 0.0;

   no2_w[l] = no2[l]  * anvf[l] * AA;
   no2_d[l] = no2[l] - no2_w[l];
   no2[l] = 0.0;

   no_w[l] = no[l] * anvf[l] * AA;
   no_d[l] = no[l] - no_w[l];
   no[l] = 0.0;

   n2o_w[l] = n2o[l] * anvf[l] * AA;
   n2o_d[l] = n2o[l] - n2o_w[l];
   n2o[l] = 0.0;

   n2_w[l] = n2[l];
   n2[l] = 0.0;



   if(T==1&&l==1) Odoc[l] *= 0.0;
   else Odoc[l] = doc[l];
 
   float pwC = Odoc[l] * anvf[l] * AA;
   doc_w[l] += pwC;
   doc_d[l] = Odoc[l] - pwC;
   Odoc[l] = 0.0;

   // Allocate N deposit to top soil
    if (l == 1 && T <= rain_hrs && flood_flag==0)
    {
       nh4_w[1] += (rno3 * .025); // rainfall-N ppm -> kg N/ha/hr
       no3_w[1] += (rno3 * .025);

       day_inRainN += (rno3*0.05);
    }

    if (l <= T && T <= rain_hrs)
    {
        float F_leak = max(1.0, WaterLeakRate*50.0);//leak
        float lost_urea = 0.0;

        if(flood_flag==0)
            leach_factor = min(0.9, (0.9 * (1.0 - clay[l]) * F_leak));
        else 
            leach_factor = 0.0;

        if(leach_factor > 1.0) leach_factor = 1.0;

        lost_DOC = doc_w[l] * leach_factor;

        doc_w[l] -= lost_DOC;
        if (l == q) 
        {
            day_leach_DOC += lost_DOC;
            day_DOC_leac += lost_DOC;
        }
        else doc_w[l+1] += lost_DOC;
        
        lost_no3 = no3_w[l] * leach_factor;
        no3_w[l] -= lost_no3;

        lost_urea = urea[l] * leach_factor;
        urea[l] -= lost_urea;
        
        if (l==q) 
        {
            day_leach_NO3 += lost_no3;//no3_w[l] += lost_no3;
            day_leach_urea += lost_urea;
        }
        else 
        {
            no3_w[l+1] += lost_no3;
            urea[l+1] += lost_urea;
        }
    }

    // Initialize denitrifier and nitrifier biomass each day, kg C/layer/ha
    
    if(TTT>0.0&&irri_flag==0)
    {
        //Fsm = 2.0 * (float)pow((float)TTT, 1.5);
        Fsm = 0.5 * TTT;//50.0
    }
    else 
    {
        if(irri_flag==1&&day_irri>=0.015)// && day_wfps[l]>=0.8*fldcap[l]) 
            Fsm = 5.0 * day_irri;//5.0 * (float)irri_flag;
        else			
            Fsm = 0.1;
    }
    
    float FFde=1.0;

    if(LandUse==3) Fde=Tttoo[1]*0.035;//*(float)exp(0.2525*temp[l]);
    else if(LandUse==4) Fde= Tttoo[1] * 0.3;//0.01
    else if(LandUse==2) Fde=0.00007*(float)exp(0.2525*temp[l]);
    else 
    {
        Fde=Tttoo[1]*0.0035*(float)exp(0.2525*temp[l]);		

#ifdef MANURE
        Fde=Tttoo[GN]*1.0*(float)exp(0.2525*temp[l]);//0.0035	
#else
        if(till_flag==1) 
            Fde=1.0*Tttoo[1]*0.025*(float)exp(0.2525*temp[l])*till_fact;//1000.0 -> 1.0 modified on July 26, 2012
#endif		
        if(LandUse==6)
        {
            if (l <= T && T <= rain_hrs)// && Leaf_Wt[1]==0.0)
                FFde = 100.0 * T / (99.0 + T);
            else
                FFde = Fde;
        }
        else
        {
            if (l <= T && T <= rain_hrs)// && Leaf_Wt[1]==0.0)
            {
                FFde = 100.0 * T / (99.0 + T);
            }
            else
                FFde = Fde;
        }
    }

/*	if(total_ice>0.0) 
    {
        if((ice[l]/10000.0+water[T][l])/sts_h < wiltpt) 
            Fde = 0.0;
        else 
            Fde = 0.005 * water[T][l]/sts_h;//(float)exp(-5.4039*clay);//0.0752
    }*/

    if(st[l]==12||st[l]==13||st[l]==14) Fst = 0.05;//0.1;
    else Fst = 1.0;

    if(sph[l]<5.0) 
        Fph = -0.1797*sph[l]*sph[l] + 2.4158*sph[l]- 7.057;
    else 
        Fph = -0.1225*sph[l]*sph[l] + 1.6165*sph[l] - 4.3095;
    Fph = min(1.0, max(0.2, Fph));

    if(manuFlag==1&&l<7) 
        Fmn = 250.0 / (float)ManureDays;//250.0
    else Fmn = 5.0;
    Fmn = max(5.0, Fmn);

    //if(SlurryFlag==1&&ManureDays<60) Fmn *= 10.0;

    if(temp[l]<=0.0) Ftm = 0.0;
    else Ftm = 1.0;

    if(l>q) //livestock system
    {
        Fde = 1.1;
        Fmn = 1.1;
        Fst = 1.1;
        Fsm = 1.1;
    }	
        
    denitrifier[l] = 0.02*ave_anvf[l] * mRBO * mFD * MicrobioIndex * FFde * Fph * Fst * Fsm * Fmn * Ftm * soc[l];//0.02
    
    nitrifier[l] = 0.1*mRBO * mFD * MicrobioIndex * Fde * Fph * Fst * Fsm * Fmn * Ftm * soc[l];// 

    // Denitrification =================================================
    if(denitrifier[l]>0.0)
    {
        assim2[l] = 0.0;

        // Growth of denitrifiers (% per hour)
        cck = doc_w[l] / (KC + doc_w[l] + 0.0000001);
        n3k = no3_w[l] / (KN + no3_w[l] + 0.0000001);
        u_no3 = (um_no3 * cck * n3k);

        n2k = no2_w[l] / (KN + no2_w[l] + 0.0000001);
        u_no2 = (um_no2 * cck * n2k);

        nok = no_w[l] / (KN + no_w[l] + 0.0000001);
        u_no = (um_no * cck * nok);

        n2o_k = n2o_w[l] / (KN + n2o_w[l] + 0.0000001);
        u_n2o = (um_n2o * cck * n2o_k);

        U = u_no3 + u_no2 + u_no + u_n2o;
        DBG = U * denitrifier[l];
        DBD = (MC * YMC * denitrifier[l]);
        DB = DBG - DBD;
        if ( DB < 0.0 ) DB = 0.0;
        Nw = no3_w[l] + no2_w[l] + no_w[l] + n2o_w[l];
        if (Nw <= 0.0) goto RUN_OUT;

        // no3(-) -> no2(-)

        // Amount of no3(-) converted to no2(-)

        if ( fabs(doc_w[l]) < 0.00001 )
        {
            d_no3 = 0.0;
        }
        else
        {
            float FFF;		
            if(flood_flag==0) 
            {
                FFF = 0.01;//2.0
            }
            else FFF = 2.0;

            //if(WaterControl==1&&crop[cwc]==20) FFF = 0.005;
            if(WaterControl==1&&flood_flag==1) FFF = 0.005;
            //if(flood_flag==1) 
                

            //if(GN==(q+4)) FFF = 1.0;//compost

            //if(flood_flag==0)
                d_no3 = FFF * (u_no3 / ym_no3 + m_no3 * no3_w[l] / Nw) * phk1[l] * denitrifier[l] * TE[l];
            //else 
            //	d_no3 = 0.5 * no3_w[l];
        }

        if (no3_w[l] < d_no3)
        {
           d_no3 = no3_w[l];
           no3_w[l] = 0.0;
        }
        else no3_w[l] -= d_no3;

        no2_w[l] += d_no3;

        //*day_soil_denitrification += d_no3;

        // Amount of no3(-) synthesized into no3(-) denitrifier
        if ( fabs(Nw) < 0.00001 )
        {
            da_no3 = 0.0;
        }
        else
        {
            da_no3 = cc * R * DBG * (no3_w[l] / Nw);
        }

        if (no3_w[l] < da_no3)
        {
           da_no3 = no3_w[l];
           no3_w[l] = 0.0;
        }
        else no3_w[l] -= da_no3;

        assim2[l] += da_no3;

        // no2(-) -> n2o

        // Amount of no2(-) converted to n2o

        if ( fabs(doc_w[l]) < 0.00001 )
        {
            d_no2 = 0.0;
        }
        else
        {
            float FFF;		
            if(flood_flag==0) FFF = 10.0;
            else FFF = 10.0;

            d_no2 = FFF * (u_no2 / ym_no2 + m_no2 * no2_w[l] / Nw)
                 * phk2[l] * denitrifier[l] * TE[l] * QK;
        }

        if (no2_w[l] < d_no2)
        {
           d_no2 = no2_w[l];
           no2_w[l] = 0.0;
        }
        else no2_w[l] -= d_no2;

        n2o_w[l] += d_no2;

        // Amount of no2(-) synthesized into no2(-) denitrifier

        if ( fabs(Nw) < 0.00001 )
        {
            da_no2 = 0.0;
        }
        else
        {
            da_no2 = cc * R * DBG * (no2_w[l] / Nw);
        }

        if (no2_w[l] < da_no2)
        {
           da_no2 = no2_w[l];
           no2_w[l] = 0.0;
        }
        else no2_w[l] -= da_no2;

        assim2[l] += da_no2;

        // no2(-) -> no

        // Amount of no2(-) converted to no

        if ( fabs(doc_w[l]) < 0.00001 )
        {
            d_no2_kk = 0.0;
        }
        else
        {
            float FFF;		
            if(flood_flag==0) FFF = 0.5;
            else FFF = 5.0;

            d_no2_kk = FFF * (u_no2 / ym_no2 + m_no2 * no2_w[l] / Nw)
                   * phk1[l] * denitrifier[l] * TE[l] * QK;
        }

        if (no2_w[l] < d_no2_kk)
        {
           d_no2_kk = no2_w[l];
           no2_w[l] = 0.0;
        }
        else no2_w[l] -= d_no2_kk;

        no_w[l] += d_no2_kk;

        // Amount of no2(-) synthesized into no2(-) denitrifier

        if ( fabs(Nw) < 0.00001 )
        {
            da_no2_kk = 0.0;
        }
        else
        {
            da_no2_kk = cc * R * DBG * (no2_w[l] / Nw) / 100.0;
        }

        if (no2_w[l] < da_no2_kk)
        {
           da_no2_kk = no2_w[l];
           no2_w[l] = 0.0;
        }
        else no2_w[l] -= da_no2_kk;

        assim2[l] += da_no2_kk;

        // no -> n2o

        // Amount of NO converted to N2O

        if ( fabs(Nw) < 0.00001 )
        {
            d_no = 0.0;
        }
        else
        {
            float FFF;		
            if(flood_flag==0) FFF = 1.0;
            else FFF = 1.0;

            d_no = FFF * (u_no / ym_no + m_no * no_w[l] / Nw)
                   * phk2[l] * denitrifier[l] * TE[l]  * QK;
        }

        if (no_w[l] < d_no)
        {
           d_no = no_w[l];
           no_w[l] = 0.0;
        }
        else no_w[l] -= d_no;

        n2o_w[l] += d_no;

        // Amount of NO synthesized by NO denitrifier

        if ( fabs(Nw) < 0.00001 )
        {
            da_no = 0.0;
        }
        else
        {
            da_no = cc * 2.0 * R * DBG * (no_w[l] / Nw);
        }

        if (no_w[l] < da_no)
        {
           da_no = no_w[l];
           no_w[l] = 0.0;
        }
        else no_w[l] -= da_no;

        assim2[l] += da_no;

        // n2o -> n2

        // Amount of n2o coverted to n2

        if ( fabs(doc_w[l]) < 0.00001 )
        {
            d_n2o = 0.0;
        }
        else
        {
            float FFF;
            if (st[l]==12) FFF = 0.2;
            else FFF = 1.2 * clay[l];	//12.5; 1.2
                        
            if(TTT>5.0) FFF *= 0.01 / TTT;//0.025
            
            if(l==q+1) FFF = 1.0;//feedlot

            if(LateralInflux>1.0&&precipitation>0.0) FFF *= 0.5;

            if(irri_flag==1) FFF = 0.0;

            if(flood_flag==1)
                FFF = 1.0;
            
            if(l==q+5) //lagoon
            {
                d_n2o = n2o_w[l] + no3_w[l] + no3_d[l];
                no3_w[l] = 0.0;
                no3_d[l] = 0.0;
            }
            else
                d_n2o = FFF * (u_n2o / ym_n2o + m_n2o * n2o_w[l] / Nw) * phk3[l] * denitrifier[l] * TE[l];
            
        }

        if (n2o_w[l]<d_n2o)// || Fsm >10.0)
        {
           d_n2o = n2o_w[l];
           n2o_w[l] = 0.0;
        }
        else n2o_w[l] -= d_n2o;
        
        n2_w[l] += d_n2o;

        // Amount on n2o synthesized by n2o denitrifier

        if ( fabs(Nw) < 0.00001 )
        {
            da_n2o = 0.0;
        }
        else
        {
            da_n2o = cc * R * DBG * (n2o_w[l] / Nw);
        }

        if (n2o_w[l] < da_n2o)
        {
           da_n2o = n2o_w[l];
           n2o_w[l] = 0.0;
        }
        else n2o_w[l] -= da_n2o;

        assim2[l] += da_n2o;

        // Assimilated N in microorganisms during denitrification
        //assim2[l] = (da_no3+da_no2+da_no2_kk+da_no+da_n2o);
        da_no3 = 0.0; da_no2 = 0.0; da_no2_kk = 0.0;
        da_no = 0.0; da_n2o = 0.0;
        assim_N += assim2[l];//where to go?

        //n2_w[l] += assim2[l];
        nh4[l] += assim2[l];
        assim2[l] = 0.0;

        // DOC consumption

        // Amount of doc consumed

        if ( fabs(doc_w[l]) < 0.00001 )	DC = 0.0;
        else DC = (U / YMC + MC) * denitrifier[l];

        // Amount of doc converted to CO2

        d_co2 = DC;// - DBG;

        // New denitrifiers

        denitrifier[l] += DB;

        // New avalable c

        doc_w[l] -= DC;
        if(l<=q) day_DOC_cons += DC;
        if ( denitrifier[l] < 0.0 ) denitrifier[l] = 0.0;
        if ( doc_w[l] < 0.0 ) doc_w[l] = 0.0;
        if ( d_co2 < 0.0 ) d_co2 = 0.0;
        if ( no3_w[l] < 0.0 ) no3_w[l] = 0.0;
        if ( no2_w[l] < 0.0 ) no2_w[l] = 0.0;

        dn_co2 += d_co2;
        d_co2 = 0.0;

        RUN_OUT:;
    }

    //Nitrification
    if(nitrifier[l]>0.0)
    {
        fact_t = (float)pow(3.503,(60.0-temp[l])/(60.0-34.22))
           * (float)exp(3.503*(temp[l]-34.22)/(60.0-34.22)); //O'neill

        if ( fact_t > 1.0 ) fact_t = 1.0;
        if ( fact_t < 0.0 ) fact_t = 0.0;

        if (day_wfps[l] > 0.05)
           fact_m = 0.8 + 0.21 * (1.0 - day_wfps[l]);
        else
           fact_m = 0.0;

        if ( fact_m > 1.0 ) fact_m = 1.0;
        if ( fact_m < 0.0 ) fact_m = 0.0;

        // developed by Florian based on Blagodatsky and Richter 1998
        micro_growth = 0.0166 * (doc_d[l] / (1.0 + doc_d[l]) + fact_m / (1.0 + fact_m)) / 24.0 * MicrobioIndex;
        if (micro_growth < 0.0) micro_growth = 0.0;

        micro_death = 0.008 * nitrifier[l] * 1.0 / (1.0 + doc_d[l]) / (1.0 + fact_m) / 24.0;

        micro_delta = (micro_growth - micro_death) * nitrifier[l] * fact_t * fact_m;

        nitrifier[l] += micro_delta;
        if (nitrifier[l] < 0.0) nitrifier[l] = 0.0;

        float Fclay = -46.603 * (float)log(clay[l]) - 7.2499;
        if(Fclay<1.0) Fclay = 1.0;
        if(Fclay>100.0) Fclay = 100.0;

        if(day_wfps[l]>fldcap[l]||by_passf>0.0) k = 0.1;//0.001
        else k = 0.005 * Fclay;//0.005
        
        if(l==q+1) //barn
        {
            k = 0.01;//0.00001;
            day_wfps[1] = day_wfps[l];
        }
        else if(l==q+2||l==q+3) //grazing field
            k = 0.001;//0.00001;
        else if(l==q+4)
        {
            k = 0.0000001;//compost
            day_wfps[1] = day_wfps[l];
        }
        else if(l==q+5||l==q+6||l==q+8) //lagoon, digester, gutter
        {
            k = 0.0001;
            day_wfps[1] = 1.0;
        }

        float LFactor = 20.0 * (float)pow((float)0.5, (float)l);
        float TFactor = (temp[l]-9.0) / 20.0;
        TFactor = max(0.0, TFactor);

        if(l>q) LFactor = 0.08;//0.001;
        
        if(flood_flag==0)
        {
            //wwzz = 0.001;
            wwzz = 5.0 * k * nitrifier[l] * (1.0 - pow(clay[l], (float)0.5)) * day_wfps[1] * sph[l] * till_fact * LFactor;//5.0; 10.0
            if(IrriRice_flag==1) wwzz *= 0.0002;
        }
        else
            wwzz = 0.002;

        if(jday>=NIDay1&&jday<=NIDay2)//effective duration of nitrification inhibitor 
        {
            float NIEF = 1.04 - 1.2 * clay[1];//effectiveness of nitrification inhibitor 
            NIEF = min(1.0, max(0.0, NIEF));
            wwzz *= (1.0 - NIEfficiency) * NIEF;
        }

        if(drain_flag>0) 
        {
            wwzz *= 150.0;
            EEE = 0.1;
        }
        else
        {
            EEE = 0.003;

        }
        //if(irri_flag==1) wwzz *= 500.0;

        if (wwzz > 1.0) wwzz = 1.0;
        if (wwzz < 0.0) wwzz = 0.0;

        if(temp[1]<=0.0)
        {
            if(LandUse==3||LandUse==4)
                nno = 0.01 *fact_m * nh4_d[l];		
            else
                nno = wwzz * nh4_d[l];//+nh4_w[l]);
        }
        else
            nno = wwzz * nh4_d[l];//+nh4_w[l]);//0.0;

        if(flood_flag==1&&l<=q) 
        {
            if(Eh[l]>=-200.0) nno *= 0.1;
            else nno = 0.0;

            //nno = 0.0;
        }
    
        //nno = 0.05 *fact_t*fact_m *(nh4_d[l]);

        if ( nno < 0.0 ) nno = 0.0;

        float vNH4 = nh4_d[l]+nh4_w[l];

        if(vNH4<=0.0)
        {
            nh4_d[l] = 0.0;
            nh4_w[l] = 0.0;
        }
        else
        {
            nh4_d[l] -= nno * nh4_d[l] / vNH4;
            nh4_w[l] -= nno * nh4_w[l] / vNH4;
        }

        *day_soil_nitrify += nno;
        day_nitrification[l] += nno;

        //effect of nitrification on soil pH
/*		double wd1, Kw, V_water, increase_H, hydrogen, mol_nh4, hydroxide;
                    
        wd1 = (double)pow(10.0, -15.0);
        Kw = 1.945 * exp(0.0645 * temp[l]) * wd1;//water dissociation constant
        V_water = day_wfps[l] * h[l] * 10000.0 * 1000.0;//liter water/ha in layer l
        
        mol_nh4 = nno * 1000.0 / 14.0 / V_water; //kg N/layer -> mol N/L
        
        increase_H = 0.001 * 2.0 * mol_nh4; // soil buffer effect 
        
        hydrogen = (float)pow(10.0, -sph[l]);//mol H/L
        hydroxide = Kw / hydrogen;//mol OH/L
        
        hydrogen += increase_H;//new H+ (mol H/L)
        hydroxide = Kw / hydrogen;//new OH- (mol OH/L)

        sph[l] = (float)log(hydrogen) / -2.3026;//new pH
        sph[l] = max(3.0, min(11.0, sph[l]));
*/

        //EEE = 0.003;

        ae_no = EEE * nno * fact_t;
        ae_no = max(0.0, min(1.0, ae_no));
    
        *day_nitrify_NO += ae_no;
    
        nno -= ae_no;  
        no_d[l] += ae_no;
        ae_no = 0.0;

        if(l<=q)
        {
            if(drain_flag==0)
            {
                if(LandUse==1||LandUse==2)
                {
                    if(flood_flag==1&&l==1)
                    {
                        EEE = 10.0 * (float)pow(water[T][l]/(h[l]*sts[l]), 5.0);//0.01
                    }
                    else
                        EEE = 0.05 * (float)pow(water[T][l]/(h[l]*sts[l]), 5.0);
                }
                else 
                {
                    EEE = 0.2 * (float)pow(water[T][l]/(h[l]*sts[l]), 5.0);//0.2
                }
            }
            else 
                EEE = 0.1;
        }
        else
        {
            if(l==q+2||l==q+3) //pen, grazing field
                EEE = 0.0;
            else
                EEE = 1.0;
        }

#ifdef MANURE
#else
        if(till_flag==1) EEE *= 30.0;
#endif

        //if(till_fact>1.0) EEE *= (till_fact*till_fact);
        EEE = min(1.0, max(0.0, EEE));

        ae_n2o = EEE * nno * TFactor;// Ingwersen, 1998
        *day_nitrify_N2O += ae_n2o;
        nno -= ae_n2o;
        n2o_d[l] += ae_n2o;
        ae_n2o = 0.0;

        if (nno<0.0) nno=0.0;
        no3_d[l] += nno;
        nno = 0.0;
    }

  // Recover substrates ===============================================
  Odoc[l] += (doc_w[l] + doc_d[l]);
  nh4[l] += (nh4_w[l] + nh4_d[l]);

  no3[l] += (no3_w[l] + no3_d[l]);
  no2[l] += (no2_w[l] + no2_d[l]);
  no[l] += (no_w[l] + no_d[l]);
  n2o[l] += (n2o_w[l] + n2o_d[l]);
  n2[l] += n2_w[l];

  doc_w[l] = 0.0;
  nh4_w[l] = 0.0;
  no3_w[l] = 0.0;
  no2_w[l] = 0.0;
  no_w[l] = 0.0;
  n2o_w[l] = 0.0;
  n2_w[l] = 0.0;

  doc_d[l] = 0.0;
  nh4_d[l] = 0.0;
  no3_d[l] = 0.0;
  no2_d[l] = 0.0;
  no_d[l] = 0.0;
  n2o_d[l] = 0.0;

  //if(sph[l]<4.5) Chemodenitrification(l, temp);
  // Gas emission ===================================================
 /*if(flood_end==1&&flood_flag<=0)
 {
    float pww=0.9;		
    n2o[l] += (n2[l] * pww);
    n2[l] *= (1.0 - pww);
 }*/

  if (flood_flag==0)
  {
     float PA, AD, emsno, emsn2o, emsn2, pno, pn2o, pn2, tt, peat;

     if(temp[l] > 0.0) tt = temp[l];
     else 
         tt = 0.0;

#ifdef MANURE
     PA = 1.0 - ((water[T][l] /1000.0) / 10000.0) / (0.0222 * 0.5);//Modified on 1/7/2014
#else
     PA = 1.0 * (1.0 - (water[T][l] / sts_h));
#endif

     //if(seed[cwc]>0&&crop[cwc]==20) PA *= 1.0;//0.25;

     AD = 2.0 * clay[l] / 0.63;

     if(st[l]==12) peat=10.0;//2.0;
     else peat=1.0;

    float f_litter = (rcvl[l]+rcl[l]+rcr[l])/600.0;

    if(f_litter < 1.0) f_litter = 1.0;

    if(drain_flag>0)
    {
        pno = 0.05;
        pn2o = 0.05;
        pn2 = 0.05;

    }
    else
    {
         pno = peat*0.5*(f_litter * ((.0006 + .0013 * AD) + (.013 - .005 * AD) * PA * (float)pow(2.0, tt / 20.0)) * PORO);//Lena
         pn2o = peat*PA * (f_litter * (30.0 * (.0006 + .0013 * AD) + (.013 - .005 * AD) * (float)pow(2.0, tt / 20.0) * PORO)); //*****
         pn2 = peat*f_litter * (0.017 + (.025 - .0013 * AD) * PA * (float)pow(2.0, tt / 20.0) * PORO);
    }

     if ( pno > 1.0 ) pno = 1.0;
     if ( pn2o > 1.0 ) pn2o = 1.0;
     if ( pn2 > 1.0 ) pn2 = 1.0;
     if ( pno < 0.01 ) pno = 0.01;
     if ( pn2o < 0.01 ) pn2o = 0.01;
     if ( pn2 < 0.01 ) pn2 = 0.01;

     emsno = no[l] * pno;
     emsn2o = n2o[l] * pn2o;
     emsn2 = n2[l] * pn2;

#ifdef MANURE
     if(l==q+1)//housing
     {
        MData.day_house_no += emsno;
        MData.day_house_n2o += emsn2o;
        MData.day_house_n2 += emsn2;
        MData.day_house_co2 += dn_co2;
        dn_co2 = 0.0;
     }
     else if(l==q+4)//composting
     {
        MData.day_compost_no += emsno;
        MData.day_compost_n2o += emsn2o;
        MData.day_compost_n2 += emsn2;
     }
     else if(l==q+5)//lagoon
     {
        MData.day_lagoon_no += emsno;
        MData.day_lagoon_n2o += emsn2o;
        MData.day_lagoon_n2 += emsn2;
     }
     else if(l==q+6)//digester
     {
        MData.day_digester_no += emsno;
        MData.day_digester_n2o += emsn2o;
        MData.day_digester_n2 += emsn2;
     }	
     else if(l==q+8)//gutter
     {
        MData.day_gutter_no += emsno;
        MData.day_gutter_n2o += emsn2o;
        MData.day_gutter_n2 += emsn2;
     }	
#endif

     if(l<=q)
     {
         *day_soil_an_no += emsno;
         *day_soil_an_n2o += emsn2o;
         *day_soil_n2 += emsn2;

         day_denitrification[l] += (emsn2o + emsn2);
     }
     
     no[l] -= emsno;
     n2o[l] -= emsn2o;
     n2[l] -= emsn2;

     if (no[l] < 0.0) no[l] = 0.0;
     if (n2o[l] < 0.0) n2o[l] = 0.0;
     if (n2[l] < 0.0) n2[l] = 0.0;

     //hr_no += emsno;
     //hr_n2o += emsn2o;
     //hr_n2 += emsn2;

     emsno = 0.0;
     emsn2o = 0.0;
     emsn2 = 0.0;

   //}
  }
  else //flood_flag = 1
  {
    float emsno, emsn2o, emsn2, a;
    float no_aere, n2o_aere, n2_aere, no_ebul, n2o_ebul, n2_ebul;
    
    if(l<=q) aere = 0.00001 * (Root_Wt[1]+Root_Wt[2])/ 2200.0; 
    else aere = 0.01;
    if (aere > 1.0) aere = 1.0;

    if(l>1)
    {
        float pww=0.1;
        float Ngas = n2[l]+n2o[l];
        n2o[l] = Ngas * pww;
        n2[l] = Ngas * (1.0 - pww);
        
    }
    else
    {
        float pww;
        if(FloDays<2) pww = 0.01;
        else pww = 0.9;

        n2[l] += (no[l]+n2o[l]) * pww;
        no[l] *= (1.0-pww);
        n2o[l] *= (1.0-pww);	
    }


    no_aere = no[l] * aere;
    n2o_aere = n2o[l] * aere;
    n2_aere = n2[l] * aere;

    no[l] -= no_aere;
    n2o[l] -= n2o_aere;
    n2[l] -= n2_aere;

    if (no[l] < 0.0) no[l] = 0.0;
    if (n2o[l] < 0.0) n2o[l] = 0.0;
    if (n2[l] < 0.0) n2[l] = 0.0;

    // NOx efflux by ebulation

    if (l <= 10 )
    {
       float ActTemp, tt;

       if(temp[l] > 0.0) tt = temp[l];
       else tt = 0.0;

       //if (temp[l] < 0.0) temp[l] = 0.0;
       ActTemp = (-.1687 * (float)pow((0.1 * tt), 3.0) + 1.167 * 
                (float)pow((0.1 * tt), 2.0) - 2.0303 * (0.1 * tt) + 1.042);
       if(ActTemp<0.0) ActTemp = 0.0;

       if(l<WTL)
        a = PORO * ActTemp * 0.2 * 0.01;//0.2
       else
        a = PORO * ActTemp * 0.01 * 0.01;//1.0;//crop_t;//0.00007

       if(l==1) a = 1.0;

       //a = 0.0;

       if(crop_t<0.0) crop_t=0.0;

       no_ebul = no[l] * a;
       n2o_ebul = n2o[l] * a;
       n2_ebul = n2[l] * a;

       no[l] -= no_ebul;
       n2o[l] -= n2o_ebul;
       n2[l] -= n2_ebul;

       if (no[l] < 0.0) no[l] = 0.0;
       if (n2o[l] < 0.0) n2o[l] = 0.0;
       if (n2[l] < 0.0) n2[l] = 0.0;
    }
    else
    {
        no_ebul = 0.0;
        n2o_ebul = 0.0;
        n2_ebul = 0.0;
    }

     emsno = no_aere + no_ebul;
     emsn2o = n2o_aere + n2o_ebul;
     emsn2 = n2_aere + n2_ebul;

#ifdef MANURE
     if(l==q+1)//housing
     {
        MData.day_house_no += emsno;
        MData.day_house_n2o += emsn2o;
        MData.day_house_n2 += emsn2;
     }
     else if(l==q+4)//composting
     {
        MData.day_compost_no += emsno;
        MData.day_compost_n2o += emsn2o;
        MData.day_compost_n2 += emsn2;
     }
     else if(l==q+5)//lagoon
     {
        MData.day_lagoon_no += emsno;
        MData.day_lagoon_n2o += emsn2o;
        MData.day_lagoon_n2 += emsn2;
     }
     else if(l==q+6)//digester
     {
        MData.day_digester_no += emsno;
        MData.day_digester_n2o += emsn2o;
        MData.day_digester_n2 += emsn2;
     }
     else if(l==q+8)//gutter
     {
        MData.day_gutter_no += emsno;
        MData.day_gutter_n2o += emsn2o;
        MData.day_gutter_n2 += emsn2;
     }	
#endif

     if(l<=q)
     {
         *day_soil_an_no += emsno;
         *day_soil_an_n2o += emsn2o;
         *day_soil_n2 += emsn2;

         day_denitrification[l] += (emsn2o + emsn2);
     }

     //hr_no += emsno;
     //hr_n2o += emsn2o;
     //hr_n2 += emsn2;

     emsno = 0.0;
     emsn2o = 0.0;
     emsn2 = 0.0;
  }//flood_flag = 1

  } // End of layer loop, go to next layer

} // End of hourly loop, go to next hour

    //if(sph[l]<4.5&&TTT>0.0) Chemodenitrification(1);

   *day_soil_denitrification += (*day_soil_an_n2o+*day_soil_n2);//d_no3
   

   //N2O and N2 induced from frozen soil layers
   if(GN<=q)
   {
    for(int z=1; z<=q; z++)
    {
        float dead_micro, Tt, ACTN, IceDN; 

        if(temp[z]<-5.0)
        {		
            float Fpeat;
            if(st[1]>=12) Fpeat = 0.1;
            else Fpeat = 1.0;

            Tt = -temp[z] * 0.1;
            Tt = min(1.0, max(0.0, Tt));

            float TCRB = CRB1[z] + CRB2[z];
            if(TCRB>0.0) 
            {
                dead_micro = Fpeat * Tt * TCRB;
                if (dead_micro < 0.0) dead_micro = 0.0;

                CRB1[z] -= (dead_micro * CRB1[z] / TCRB);
                CRB2[z] -= (dead_micro * CRB2[z] / TCRB);

                nh4[z] += (dead_micro / rcnb);
                FreezeDOC[z] += dead_micro;
            }

            //ice_doc[z] = FreezeDOC[z];
        }

        //if(FreezeDOC[z]>0.0000001 && min_temp > 5.0)
        if(FreezeDOC[z]>0.0000001 && temp[z] > 1.0)
        {
            if(no3[z]>0.0)
            {
                float AssN=0.0, AssC= 0.0, GasN=0.0, Mbio=0.0;			

                IceDN = 2.0 * no3[z]/(1.0+no3[z]) * FreezeDOC[z]/(1.0+FreezeDOC[z]);//25.0
                IceDN = min(1.0, max(0.0, IceDN));

                GasN = 0.12 * IceDN * no3[z];//0.12
                no3[z] -= GasN;
                n2o[z] += GasN * 0.95;
                no[z] += GasN * 0.05;
                n2[z] += GasN * 0.0;
            }

            float AvaN = no3[z]+nh4[z], NewBioC, TAvaN = no3[z]+nh4[z];
            if(AvaN>0.0)
            {
                if((AvaN*rcnb)>FreezeDOC[z])//limiting by DOC
                {
                    NewBioC = FreezeDOC[z]*1.0;
                    FreezeDOC[z] *= 0.0;
                    AvaN -= (NewBioC / rcnb);
                    no3[z] = AvaN * (no3[z]/TAvaN);
                    nh4[z] = AvaN * (nh4[z]/TAvaN);
                }
                else//limiting by N
                {
                    NewBioC = AvaN*rcnb * 1.0;
                    AvaN *= 0.0;
                    FreezeDOC[z] -= NewBioC;
                    no3[z] = 0.0;
                    nh4[z] = 0.0;
                }
 
                CRB1[z] += (NewBioC*0.5);
                CRB2[z] += (NewBioC*0.5);
            }
        }

    }
   }

/*		//if(FreezeDOC[z]>0.0000001 && temp[z] > 0.0)
        //if(FreezeDOC[z]>0.0000001 && min_temp > 5.0)
    float TopDOC = FreezeDOC[1]+FreezeDOC[2]+FreezeDOC[3];
    float TopNO3 = no3[1]+no3[2]+no3[3], TTopNO3 = no3[1]+no3[2]+no3[3];
    float IceDN;
    if(TopDOC>0.0000001 && TopNO3>0.0 && ice[1]==0.0)
    {
        
            float AssN=0.0, AssC= 0.0, GasN=0.0, Mbio=0.0;			
            IceDN = 50.0 * TopNO3 * TopNO3/(1.0+TopNO3) * TopDOC/(1.0+TopDOC);//0.2
            IceDN = min(1.0, max(0.0, IceDN));

            GasN = IceDN * TopNO3;
            TopNO3 -= GasN;
            n2o[1] += GasN * 0.85;
            no[1] += GasN * 0.05;
            n2[1] += GasN * 0.1;

            no3[1] = TopNO3 * no3[1] / TTopNO3;
            no3[2] = TopNO3 * no3[2] / TTopNO3;
            no3[3] = TopNO3 * no3[3] / TTopNO3;
        
    }

    
    for(z=1; z<=q; z++)
    {
        if(FreezeDOC[z]>0.0000001 && ice[z]==0.0)
        {
            float AvaN = no3[z]+nh4[z], TAvaN = no3[z]+nh4[z], NewBioC;
            if(AvaN>0.0)
            {
                if((AvaN*rcnb)>FreezeDOC[z])//limiting by DOC
                {
                    NewBioC = FreezeDOC[z];
                    FreezeDOC[z] = 0.0;
                    AvaN -= (NewBioC / rcnb);
                    no3[z] = AvaN * (no3[z]/TAvaN);
                    nh4[z] = AvaN * (nh4[z]/TAvaN);
                }
                else//limiting by N
                {
                    NewBioC = AvaN*rcnb;
                    AvaN = 0.0;
                    FreezeDOC[z] -= NewBioC;
                    no3[z] = 0.0;
                    nh4[z] = 0.0;
                }
 
                CRB1[z] += (NewBioC*0.5);
                CRB2[z] += (NewBioC*0.5);
                co2[z] += FreezeDOC[z];
                FreezeDOC[z] = 0.0;
            }
        }
    }*/
   //}

} 


