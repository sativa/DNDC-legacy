#include "stdafx.h"
#include "Dndcgo.h"
#include "Source_main.h"
#include "Dndc_tool.h"

#ifdef MANURE 
extern class_manure MData;
#endif

void class_model::profile_accum(int l, float* newh, float sumn, float fb_nh4, float fh_nh4,
           float* w_no2, float* w_no3, float* w_nh4, float* w_nh3, float* wurea, float* wpool_no, 
           FILE*fCB, FILE*fNB, float* w_n2o, float* w_n2,
           float *wrcvl, float *wrcl, float *wrcr, float *wcrb, float *wcrhl, float *wcrhr, float *whumus, float *wtcavai,
           float *day_ice_N, float *day_ice_doc, float *day_ice_n2o, float *day_ice_n2, float *IniSOC, float *IniSON, 
           float *EndSON, 
           float *soc, float *rcvl, float *rcl, float *rcr, float *CRB1, float *CRB2, float *crhl, float *crhr,
           float *dphum, float *doc, float *no3, float *nh4, float *nh3, float *n2, float *no, float *n2o, float *urea,
           int flood_flag, int jday, int q,
           float *no2, float *winertC)
{

    *wrcvl += rcvl[l];
    *wrcl += rcl[l];
    *wrcr += rcr[l];
    *wcrb += (CRB1[l] + CRB2[l]);
    *wcrhl += crhl[l];
    *wcrhr += crhr[l];
    *whumus += dphum[l];
    *wtcavai += doc[l];//root_CO2[l]
    *winertC += inert_C[l];
    wFreezedoc += FreezeDOC[l];
    
    soc[l] = rcvl[l] + rcl[l] + rcr[l] + CRB1[l] + CRB2[l] + crhl[l] + crhr[l] + dphum[l] + doc[l] + FreezeDOC[l] + inert_C[l] + ch4_doc_pool[l];
    
    Tttoo[l] = soc[l] / mmm[l];
    fldcap[l] = ini_fldcap - 2.9 / sts[l] * (ini_ttoo[l] - Tttoo[l]);
    fldcap[l] = min(0.9, max(wiltpt[l], fldcap[l]));

    TotalSoilC += soc[l];

    //if(flood_flag==0) ch4_doc_pool[l]=0.0;

    //*day_ice_N += ice_N[l];
    *day_ice_doc += FreezeDOC[l];
    //*day_ice_n2o += ice_n2o[l];
    //*day_ice_n2 += ice_n2[l];    
    
    *w_no3 += no3[l];
    *w_nh4 += nh4[l];
    *w_nh3 += nh3[l];

    ch4_pool += ch4[l];
    
    *w_no2 += no2[l];
    *wurea += urea[l];
    *wpool_no += no[l];
    *w_n2o += n2o[l];
    *w_n2 += n2[l];

    day_SoilWater += water[24][l];//m water/layer

    if ( jday == 365 && l==q)
    {
        yr_end_grain = 0.0;
        yr_end_leaf = 0.0;
        yr_end_stem = 0.0;
        yr_end_root = 0.0;

        for(int i=1;i<=CropNumbers;i++)
        {
            yr_end_grain += Grain_Wt[i];
            yr_end_leaf += Leaf_Wt[i];
            yr_end_stem += Stem_Wt[i];
            yr_end_root += Root_Wt[i];
        }
    }
}

void class_model::cn_pools(float w_no3, float w_nh4, float w_nh3, 
                           float wurea, float wpool_no, float w_n2o, float w_n2)
{
    float wrnvl, wrnl, wrnr, wnrb, wnrhl, wnrhr, whumusn, winertn;
    //static float oldSOC=0.0;
    float wrcvl1, wrcl1, wrcr1, wcrb1, wcrhl1, wcrhr1, whumus1, wtcavai1, winertC1;
  
    FILE  *fp;
    char  IE[120];

    wrnvl = wrcvl / rcnrvl;
    wrnl = wrcl / rcnrl;
    wrnr = wrcr / rcnrr;
    wnrb = wcrb / rcnb;
    wnrhl = wcrhl / rcnh;
    wnrhr = wcrhr / rcnh2;
    whumusn = whumus / rcnm;
    winertn = winertC / PassiveCCN;

    //T_SOC = wrcvl+wrcl+wrcr+wcrb+wcrhl+wcrhr+whumus + wtcavai + winertC + wFreezedoc + wCH4_C;
    float T_SON = wrnvl + wrnl + wrnr + wnrb + wnrhl + wnrhr + whumusn + winertn;

    //day_dSOC = T_SOC - old_T_SOC;
    //old_T_SOC = T_SOC;

    day_inLitterN = day_inStubN + day_addrn;// + day_fineroot_N;

    Profile_OrgP =0.0;
    Profile_LabP=0.0;
    Profile_ComP=0.0;
    Profile_AdsP=0.0;

    for(int l=1; l<=q; l++)
    {
        day_ch4_doc_pool += ch4_doc_pool[l];
        wCH4_C += ch4_doc_pool[l];

        day_ch4_co2_pool += day_ch4_CO2[l];
        day_ch4_H2_pool += day_H2_pool[l];

        Profile_OrgP += OrgP[l];
        Profile_LabP += LabP[l];
        Profile_ComP += ComP[l];
        Profile_AdsP += AdsP[l];
    }

}


void class_model::annu_accumu_manure_house(int jday)
{
#ifdef MANURE
    MData.yr_house_nh3 += (MData.day_house_nh3+MData.day_gutter_nh3);
    MData.yr_house_n2o += (MData.day_house_n2o+MData.day_gutter_n2o);
    MData.yr_house_ch4 += (MData.day_house_ch4+MData.day_gutter_ch4); 
    MData.yr_house_no += (MData.day_house_no+MData.day_gutter_no);
    MData.yr_house_n2 += (MData.day_house_n2+MData.day_gutter_n2);
    MData.yr_house_co2 += (MData.day_house_co2+MData.day_gutter_co2);

    MData.yr_House_to_compost_C += MData.House_to_compost_C;
    MData.yr_House_to_lagoon_C += MData.House_to_lagoon_C;
    MData.yr_House_to_digester_C += MData.House_to_digester_C;
    MData.yr_House_to_field_C += MData.House_to_field_C;

    MData.yr_House_to_compost_N += MData.House_to_compost_N;
    MData.yr_House_to_lagoon_N += MData.House_to_lagoon_N;
    MData.yr_House_to_digester_N += MData.House_to_digester_N;
    MData.yr_House_to_field_N += MData.House_to_field_N;

    MData.yr_House_to_compost_P += MData.House_to_compost_P;
    MData.yr_House_to_lagoon_P += MData.House_to_lagoon_P;
    MData.yr_House_to_digester_P += MData.House_to_digester_P;
    MData.yr_House_to_field_P += MData.House_to_field_P;

    MData.yr_House_to_compost_H2O += MData.House_to_compost_H2O;
    MData.yr_House_to_lagoon_H2O += MData.House_to_lagoon_H2O;
    MData.yr_House_to_digester_H2O += MData.House_to_digester_H2O;
    MData.yr_House_to_field_H2O += MData.House_to_field_H2O;

    
/*	MData.yr_compost_nh3 += MData.day_Compost_nh3;
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

    MData.yr_compost_runoff_WATER += MData.day_compost_runoff_WATER;

    MData.yr_LOS_LeachN += MData.day_LOS_LeachN;
*/
    if(jday==365)
    {
        MData.yr_LOS_ch4 += MData.yr_enteric_ch4 + MData.yr_house_ch4;// + MData.yr_compost_ch4 + MData.yr_lagoon_ch4;
        MData.yr_LOS_n2o += MData.yr_enteric_n2o + MData.yr_house_n2o;// + MData.yr_compost_n2o + MData.yr_lagoon_n2o;
        MData.yr_LOS_nh3 += MData.yr_enteric_nh3 + MData.yr_house_nh3;// + MData.yr_compost_nh3 + MData.yr_lagoon_nh3;
    }
#endif
}




void class_model::annu_accumu()
{
    for(cwc=1; cwc<=CropNumbers; cwc++) 
    {
        if(APT[cwc]>0.0)
        {
            yr_trans += APT[cwc]; 			
            crop_trans[cwc] += APT[cwc];			
            APT[cwc] = 0.0;
        }
        
        yr_root_co2 += day_rco2[cwc];
        day_root_resp += day_rco2[cwc];
        day_rco2[cwc] = 0.0;
                
        yr_plant_nh3 += day_plant_nh3[cwc];
        day_plant_nh3[cwc] = 0.0;
        yr_uptaken[cwc] += day_nup[cwc];
        day_nup[cwc]=0.0;
        yr_soilN_from_Nfixation += day_soilN_from_Nfixation[cwc];
        day_soilN_from_Nfixation[cwc] = 0.0;		
        yr_need_N[cwc] += day_need_N[cwc];
        day_need_N[cwc] = 0.0;	
        Yr_PlantBiomass[cwc] += Day_C_growth[cwc];
        Day_C_growth[cwc]=0.0;
    }

    yr_soil_co2 += soil_hete_CO2;//day_co2;
    day_co2 = 0.0;
    root_dCO2 = 0.0;
    
    yr_ch4 += day_ch4;
    day_ch4 = 0.0;
   
    yr_dnno += day_soil_an_no;
    day_soil_an_no = 0.0;
    yr_dnn2o += day_soil_an_n2o;
    day_soil_an_n2o = 0.0;
    yr_dnn2 += day_soil_n2;
    day_soil_n2 = 0.0;
    
    yr_soil_nh3 += (day_soil_nh3+day_vol_nh3);

    day_soil_nh3 = 0.0;
    day_vol_nh3 = 0.0;    

    yr_C_mine += day_C_mine;
    day_C_mine = 0.0;

    yr_minern += day_N_mine;
    day_N_mine = 0.0;

    day_N_assim = 0.0;

    yr_leach_hu += day_leachhu;
    day_leachhu = 0.0;
    yr_leach_un += day_leachun;
    day_leachun = 0.0;

    yr_runoff_N += day_runoff_N;
    day_runoff_N = 0.0;
    yr_leach_NO3 += (day_leach_NO3 + by_pass_no3);
    day_leach_NO3 = 0.0;
    yr_leach_urea += day_leach_urea;
    day_leach_urea = 0.0;

    yr_runoff_SOC += socmusle;
    yr_runoff_ION += nh4musle+no3musle;
    yr_runoff_P += (OrgPmusle+AdsPmusle+LabPmusle);

    by_pass_no3 = 0.0;

    //yr_leach_un = yr_leach_hu / rcnm;
    yr_leach_DOC += day_leach_DOC;
    day_leach_DOC = 0.0;
    
    yr_balance_ch4 += day_balance_ch4;
    day_balance_ch4 = 0.0;
        
    yr_leach_water += (day_leach_water);  
    day_leach_water = 0.0;
    
    yr_soil_ev += day_soil_ev;
    yr_total_ev += day_snow_ev + dfol_ev + day_soil_ev + day_pond_ev;
    day_soil_ev = 0.0;

    yr_water_in += day_water_in; //m water
    day_water_in = 0.0;

    if(crop[1]==0) 
    {
        float RunoffF = (1.0 - clay[1]) * 0.38;
        WaterPool += day_runoff * RunoffF;//fallow year effect
        yr_runoff += day_runoff * (1.0-RunoffF);//0.7;
    }
    else yr_runoff += day_runoff;
    day_runoff = 0.0;

    yr_pond_ev += day_pond_ev;
    day_pond_ev = 0.0;

    yr_GWsuply += day_GWsuply;
    day_GWsuply=0.0;

    yr_shoot_CO2 += (sum_shoot_CO2 + sum_stem_CO2);
    sum_shoot_CO2=0.0;
    sum_stem_CO2=0.0;
    yr_CropGPP += dPlantBiomass;
    dPlantBiomass=0.0;
    yr_Psn += -Psn;
    Psn=0.0;
    yr_EcoCO2 += EcoCO2;
    EcoCO2=0.0;
    yr_NEE += NEE;
    NEE=0.0;

    yr_irri_N += day_irri_N;
    day_irri_N = 0.0;

    yr_rainN += day_inRainN;
    day_inRainN = 0.0;

    yr_sln += day_inLitterN;
    day_inLitterN = 0.0;

    yr_InputRoot_DOC += day_RootInput_DOC;

}

void class_model::soc_distribution(float *CRB1, float* CRB2, float* newh)
{
    int i, l, qqq[6]={0,0}, Tq=0, accumL=0, BDL;
    float culdept[6], finaoc;//, accumC=0.0,accumN=0.0;

    culdept[0] = 0.0;
    culdept[1] = 0.1;
    culdept[2] = 0.2;
    culdept[3] = 0.3;
    culdept[4] = 0.4;
    culdept[5] = 0.5;
    BDL = (int)((float)q/5.0);

    T_SOC = 0.0;

    for ( i = 1; i <= 5; i++ )
    {
        if(i<5) qqq[i] = i * BDL;//(int)((culdept[i]-culdept[i-1]) / h[i]);//+1;	
        else qqq[i] = q;
        
        layerc[i] = 0.0;
        layern[i] = 0.0;
        LayerSoilWeight[i] = 0.0;
        accumL = 0;
            
        for ( l = Tq+1; l <= qqq[i]; l++ )
        {
            accumL ++;

            finaoc = rcvl[l] + rcl[l] + rcr[l] + CRB1[l] + CRB2[l] 
                + crhl[l] + crhr[l] + dphum[l] + inert_C[l] + doc[l] 
                + FreezeDOC[l];
            layerc[i] += finaoc;
            //accumC += finaoc;			

            float Wson = (rcvl[l]/ rcnrvl + rcl[l]/ rcnrl + rcr[l]/ rcnrr + CRB1[l]/ rcnb + CRB2[l]/ rcnb + crhl[l]/ rcnh + crhr[l]/ rcnh2 + dphum[l]/ rcnm + inert_C[l]/PassiveCCN);
            layern[i] += Wson;
            
            LayerSoilWeight[i] += mmm[l];
        }

        topc[i] = layerc[i] / LayerSoilWeight[i]; //layerc[i] / (m * qqq[i]);
        topn[i] = layern[i] / LayerSoilWeight[i];

        Tq = qqq[i];

        T_SOC += layerc[i];
    }

    day_dSOC = T_SOC - old_T_SOC;
    old_T_SOC = T_SOC;
}

void class_model::MC_annu_accumu()
{
    
    yr_soil_co2 += day_co2;
    //yr_RootExude_co2 += root_dCO2; //day_RootExudate;
    day_co2 = 0.0;
    root_dCO2 = 0.0;

    //yr_RootDOC += day_RootDOC;
    //day_RootDOC = 0.0;

    yr_root_co2 += (day_rco2[1]+day_rco2[2]+day_rco2[3]);
    day_rco2[1]=0.0;
    day_rco2[2]=0.0;
    day_rco2[3]=0.0;
    
    yr_ch4 += day_ch4;
    day_ch4 = 0.0;
    //yr_dcn2o += day_soil_ae_n2o;
    //day_soil_ae_n2o = 0.0;
    //yr_dcno += day_soil_ae_no;
    //day_soil_ae_no = 0.0;
    yr_dnno += day_soil_an_no;
    day_soil_an_no = 0.0;
    yr_dnn2o += day_soil_an_n2o;
    day_soil_an_n2o = 0.0;
    yr_dnn2 += day_soil_n2;
    day_soil_n2 = 0.0;
    
    yr_soil_nh3 += (day_soil_nh3+day_vol_nh3);

    day_soil_nh3 = 0.0;
    day_vol_nh3 = 0.0;

    yr_plant_nh3 += (day_plant_nh3[1]+day_plant_nh3[2]+day_plant_nh3[3]);
    day_plant_nh3[1] = 0.0;
    day_plant_nh3[2] = 0.0;
    day_plant_nh3[3] = 0.0;

    yr_C_mine += day_C_mine;
    day_C_mine = 0.0;

    yr_minern += day_N_mine;
    day_N_mine = 0.0;

    //yr_assim += day_N_assim;
    day_N_assim = 0.0;

    yr_leach_hu += (day_leachhu);
    day_leachhu = 0.0;
    yr_leach_un += day_leachun;
    day_leachun = 0.0;

    yr_runoff_N += day_runoff_N;
    day_runoff_N = 0.0;
    yr_leach_NO3 += (day_leach_NO3 + by_pass_no3);
    day_leach_NO3 = 0.0;
    yr_leach_urea += day_leach_urea;
    day_leach_urea = 0.0;
    by_pass_no3 = 0.0;

    //yr_leach_un = yr_leach_hu / rcnm;
    yr_leach_DOC += day_leach_DOC;
    day_leach_DOC = 0.0;
    //plantn[cwc] += day_nup;
    //if (crop[cwc]==20) liupt[cwc] = plantn[cwc];
    for(cwc=1; cwc<=CropNumbers; cwc++)
    {
        yr_uptaken[cwc] += day_nup[cwc];
        day_nup[cwc]=0.0;
    }
    //weedn += day_weednup;
    //yr_weedn += day_weednup;
    //day_weednup = 0.0;

    yr_balance_ch4 += day_balance_ch4;
    day_balance_ch4 = 0.0;
        
    yr_leach_water += (day_leach_water);  
    day_leach_water = 0.0;
    
    yr_soil_ev += day_soil_ev;
    yr_total_ev += day_snow_ev + dfol_ev + day_soil_ev + day_pond_ev;
    day_soil_ev = 0.0;

    yr_water_in += day_water_in; //m water
    day_water_in = 0.0;

    yr_trans += day_transp; 
    for(cwc=1; cwc<=CropNumbers; cwc++)
    {
        crop_trans[cwc] += APT[cwc];//day_transp;
    }
    day_transp = 0.0;	
    
    if(crop[1]==0) 
    {
        float RunoffF = (1.0 - clay[1]) * 0.38;
        WaterPool += day_runoff * RunoffF;//fallow year effect
        yr_runoff += day_runoff * (1.0-RunoffF);//0.7;
    }
    else yr_runoff += day_runoff;
    day_runoff = 0.0;

    yr_pond_ev += day_pond_ev;
    day_pond_ev = 0.0;

    //yr_DNassimN += day_DNassimN;
    //day_DNassimN = 0.0;

    //yr_FrostBio += day_frostMicrobe;
    //day_frostMicrobe = 0.0;

    yr_GWsuply += day_GWsuply;
    day_GWsuply=0.0;

    yr_shoot_CO2 += (sum_shoot_CO2+ sum_stem_CO2);
    sum_shoot_CO2=0.0;
    sum_stem_CO2=0.0;
    yr_CropGPP += dPlantBiomass;
    dPlantBiomass=0.0;
    yr_Psn += -Psn;
    Psn=0.0;
    yr_EcoCO2 += EcoCO2;
    EcoCO2=0.0;
    yr_NEE += NEE;
    NEE=0.0;

    yr_irri_N += day_irri_N;
    day_irri_N = 0.0;

    yr_rainN += day_inRainN;
    day_inRainN = 0.0;

    yr_soilN_from_Nfixation += (day_soilN_from_Nfixation[1]+day_soilN_from_Nfixation[2]+day_soilN_from_Nfixation[3]);
    day_soilN_from_Nfixation[1] = 0.0;
    day_soilN_from_Nfixation[2] = 0.0;
    day_soilN_from_Nfixation[3] = 0.0;
}



