#include "stdafx.h"
#include "Dndcgo.h"
#include "Source_main.h"
#include "Dndc_tool.h"
#include <stdio.h>

#include "RunPaths.h"

#ifdef MANURE
extern class_manure MData;
#endif

float day_clay_n2;
int File_Copy(char *source, char *dest);

void class_model::write_out_dayClim(FILE *fclim, int jday, char*site, int year, float air_temp, float precipitation,
                                     float wind_speed, float radiation, float humidity)
{
    if (jday==1)
     {
        fprintf(fclim, "Daily climate at site %s in year %d\n", site, year);
        fprintf(fclim, "Day,Temp.(C),Prec.(mm),WindSpeed(m/s),Radiation(MJ/m2/d),Humidity(%),PET(mm),Actual_ET(mm),Evaporation(mm),Transpiration(mm)\n");
    }
        fprintf(fclim, "%4d, ", jday);
        fprintf(fclim, "%7.2f, %7.2f, %7.2f, %7.2f, %7.2f, %7.2f, %7.2f, %7.2f, %7.2f\n", 
            air_temp, precipitation*1000.0, wind_speed, radiation, humidity, DayPET*1000.0, 
            (day_transp+day_snow_ev + dfol_ev + day_soil_ev + day_pond_ev)*1000.0, (day_snow_ev + dfol_ev + day_soil_ev + day_pond_ev)*1000.0,
            day_transp*1000.0);
}

int class_model::write_out_days(FILE *fs, float DayPET)
{
    

    //int ww=(int)((float)q/5.0);
    
     //if(LL1==0) LL1=1;

     if (jday==1)
     {
        fprintf(fs, "Daily soil climate at site %s in year %d\n", site, year);
        fprintf(fs, ",,,,Soil_temperature,,,,,,,Soil_moisture(wfps),,,,,,,Soil_oxygen(mol/L),,,,,,Soil_Eh(mv),,,,,,WT(m), Ice(wfps),,,,,,Ice,Snowpack,SoilWater,DeepWater,Soil_pH,,,,,,SurfaceWaterT\n");
        fprintf(fs, "Day,Temp.(C),Prec.(mm),PET,1,5,10,20,30,40,50,1,5,10,20,30,40,50,1,10,20,30,40,50,1,10,20,30,40,50,,1,10,20,30,40,50,(mm),(mm),(mm),(mm),1,10,20,30,40,50, degreeC\n\n");
    }
        fprintf(fs, "%3d,", jday);
        fprintf(fs, "%7.2f,%7.2f,%7.1f,", air_temp, precipitation*1000, DayPET*1000);
        fprintf(fs, "%7.1f,%7.1f,%7.1f,%7.1f,%7.1f,%7.1f,%7.1f,", temp[LL0],temp[LL6], temp[LL1], temp[LL2], temp[LL3], temp[LL4], temp[LL5]);
        fprintf(fs, "%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,", day_wfps[LL0], day_wfps[LL6],day_wfps[LL1], day_wfps[LL2], day_wfps[LL3], day_wfps[LL4], day_wfps[LL5]); //wfps
        fprintf(fs, "%7.6f,%7.6f,%7.6f,%7.6f,%7.6f,%7.6f,", O2_old[LL0], O2_old[LL1], O2_old[LL2], O2_old[LL3], O2_old[LL4], O2_old[LL5]);
        fprintf(fs, "%7.1f,%7.1f,%7.1f,%7.1f,%7.1f,%7.1f,", Eh_old[LL0], Eh_old[LL1], Eh_old[LL2], Eh_old[LL3], Eh_old[LL4], Eh_old[LL5]);
        fprintf(fs, "%7.4f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.1f,%7.2f,",
            day_WT,
            ice[LL0],ice[LL1],ice[LL2],ice[LL3],ice[LL4],ice[LL5], 
            //ice[LL1] /10/( h[LL1]*sts[LL1] * 1000), ice[LL2] /10/( h[LL2]*sts[LL2] * 1000), ice[LL3] /10/( h[LL3]*sts[LL3] * 1000), 
            total_ice * 1000, snow_pack*1000, day_SoilWater*1000, WaterPool*1000); 
        fprintf(fs, "%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,", sph[LL0], sph[LL1], sph[LL2], sph[LL3], sph[LL4], sph[LL5]);
        fprintf(fs, "%7.2f\n", SurfaceWaterT);
     return( 0 );
}

#ifdef HOURLY
void class_model::write_out_HrSoilMoist(FILE *HSM)
{
    //int ww=(int)((float)q/5.0);
    
     if(LL1==0) LL1=1;

    if (jday==1)
     {
        fprintf(HSM, "Hourly soil moisture in wfps at site %s in year %d\n\n", site, year);
        fprintf(HSM, "Day,Hour,1cm,5cm,10cm,20cm,30cm,40cm,50cm\n\n");
     }
    
    for(int i=1;i<=24;i++)
    {
        fprintf(HSM, "%3d,%3d,",jday,i);			
        fprintf(HSM, "%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f\n", 
            water[i][1]/(h[1] * sts[1]),water[i][LL6]/(h[LL6] * sts[LL6]),water[i][LL1]/(h[LL1] * sts[LL1]), water[i][LL2]/(h[LL2] * sts[LL2]), water[i][LL3]/(h[LL3] * sts[LL3]), water[i][LL4]/(h[LL4] * sts[LL4]), water[i][LL5]/(h[LL5] * sts[LL5])); 
            
    }
}

void class_model::write_out_HrSoilN(FILE *HSM, int T, int JDY)
{
    if (jday==JDY&&T==1)
    {
        fprintf(HSM, "Hourly soil N at site %s in year %d\n\n", site, year);
        fprintf(HSM, "Day,Hour,NH4+,NO3-,NO2-,NO,N2O,N2,DOC\n");
    }
    
    fprintf(HSM, "%3d,%3d,",jday,T);			
    fprintf(HSM, "%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f\n", 
            nh4[1], no3[1], no2[1], no[1], n2o[1], n2[1], doc[1]); 
            
}

#endif

int class_model::write_out_dayw(FILE *fw, float DayPET)
{
    float SoilWaterBalance;

     int L1, L2, L3;
     for(int i=1; i<=q; i++)
    {
        if(SoilDepth[i]>0.0&&SoilDepth[i]<=0.05) L1 = i;
        else if(SoilDepth[i]>0.05&&SoilDepth[i]<=0.15) L2 = i;
        else if(SoilDepth[i]>0.15&&SoilDepth[i]<=0.3) L3 = i;
        //else if(SoilDepth[i]>0.2&&SoilDepth[i]<=0.3) LL3 = i;
        //else if(SoilDepth[i]>0.3&&SoilDepth[i]<=0.4) LL4 = i;
        //else if(SoilDepth[i]>=0.4&&SoilDepth[i]<=0.5) LL5 = i;
        else {}
    }
    /* L1 = (int)(.05/h);
     L2 = (int)(.15/h);
     L3 = (int)(.30/h);*/

     float soil_water_change = end_soil_water - ini_soil_water;
     float free_water = 0.0;
     for(int w=1; w<=q; w++)
     {
        free_water += water[24][w];
     }
     
     if(flood_flag==0) 
         SoilWaterBalance = soil_water_change - day_water_in + day_soil_ev + day_transp + day_leach_water;
     else 
         SoilWaterBalance = soil_water_change - day_water_in + day_soil_ev + day_transp + day_leach_water;
     //float dBalance2 = (day_water_in + day_bypass_influx) - (day_transp + day_soil_ev + day_leach_water + soil_water_change + dPool);
    
     float DayIniPool = ini_soil_water + DayIniWaterPool;
     float DayEndPool = end_soil_water + EndWaterPool;
     float DayInFlux = precipitation + day_irri;
     float DayOutFlux = day_soil_ev + day_transp + day_leach_water + day_runoff + day_snow_ev + dfol_ev + day_pond_ev;

     //float dDaySoilWater = ((end_soil_water+EndWaterPool)-(ini_soil_water+DayIniWaterPool))*1000;
     //float dDayOutFlow = (day_soil_ev+day_transp+day_leach_water)*1000;
     float balance = (DayEndPool+DayOutFlux)-(DayIniPool+DayInFlux);//dDaySoilWater - day_water_in*1000 + dDayOutFlow;

     if (jday==1)
     {
        fprintf(fw, "Daily soil water budget at site %s in year %d. Unit: mm water\n", site, year);
        //fprintf(fw, "____________________________________________________________________________________________________________________________________________\n");
        fprintf(fw, "Day,IniSoilWater,IniDeepWater,IniWaterPool,Precipitation,Irrigation,DayInFlow,EndSoilWater,EndDeepWater,EndWaterPool,Evaporation,Transpiration,Leaching,Runoff,DayOutFlow,Balance,SedimentYield, SOC_loss, ION_loss, OrgP_loss, AdsP_loss, LabP_loss,FreeWater,SoilIce,Ponding,SnowPack,\n");
        //fprintf(fw, "____________________________________________________________________________________________________________________________________________\n");
     }

    fprintf(fw, "%3d,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.4f,%7.4f,%7.4f,%7.4f\n",
            jday,
            ini_soil_water*1000.0,
            DayIniWaterPool*1000.0,
            DayIniPool*1000.0,

            precipitation*1000.0,
            day_irri*1000.0,
            DayInFlux*1000.0,

            end_soil_water*1000.0,				
            EndWaterPool*1000.0,
            DayEndPool*1000.0,			
                    
            (day_soil_ev+day_snow_ev + dfol_ev + day_pond_ev)*1000.0,			
            day_transp*1000.0,
            day_leach_water * 1000.0,
            day_runoff * 1000.0,			
            DayOutFlux*1000.0,
            
            balance*1000.0,
            
            sedyld*1000.0, socmusle, nh4musle+no3musle, OrgPmusle,AdsPmusle,LabPmusle,
            free_water * 1000.0,
            total_ice,surf_water * 1000.0, snow_pack * 1000.0);

            //SoilWaterBalance*1000,			
            //day_surplus_water*1000);

/*			day_bypass_influx * 1000,
            day_intercept*1000,
            
            day_snow_ev*1000, //mm/day
            dfol_ev*1000,
            
            day_paddy_water_ev * 1000,
            
            day_runoff*1000,
            soil_water_change*1000,
            ini_soil_water*1000,
            end_soil_water*1000);
*/			
            //SoilWaterBalance*1000);
            
     return( 0 );
}

int class_model::write_out_dayc(FILE *fc )
{

    if (jday==1)
    {
    fprintf(fc, "Daily C pools (kg C/ha) and fluxes (kg C/ha/day) at site %s in year %d\n", site, year);
    fprintf(fc, "Day,Very labile litter,Labile litter,Resistant litter,Microbe,Humads,Humus,CharC,SOC,SOC0-10cm, SOC10-20cm,SOC20-30cm,SOC30-40cm,SOC40-50cm,SOC0-10cm, SOC10-20cm,SOC20-30cm,SOC30-40cm,SOC40-50cm,dSOC,DOC,DOC_produce,DOC_consume,DOC_leach,Photosynthesis,Leaf-respiration, Stem-respiration, Root-respiration,Soil-heterotrophic-respiration, NPP,Eco-respiration, NEE,Stub,CH4-prod.,CH4-oxid.,CH4-flux,CH4-pool,DOC-leach,Litter-C,Manure-C, SoilCO2_0-10cm,SoilCO2_10-20cm,SoilCO2_20-30cm,SoilCO2_30-40cm,SoilCO2_40-50cm,\n");
    }
    fprintf(fc, "%3d,", jday);
    fprintf(fc, "%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.3f,%7.3f,%7.3f,%7.3f,%7.3f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,",
            wrcvl, wrcl, wrcr, wcrb, wcrhl+wcrhr, whumus, winertC, T_SOC, layerc[1],layerc[2],layerc[3],layerc[4],layerc[5],topc[1],
            topc[2],topc[3],topc[4],topc[5],day_dSOC, wtcavai, day_DOC_prod, day_DOC_cons, day_DOC_leac);
    fprintf(fc, "%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,", 
        Psn, sum_shoot_CO2, sum_stem_CO2, day_root_resp, soil_hete_CO2, dPlantBiomass, EcoCO2, NEE, stub1+stub2+stub3);
    fprintf(fc, "%7.5f,%7.5f,%7.5f,%7.2f,", ch4_prod, -ch4_day_oxid, day_ch4, ch4_pool);
    fprintf(fc, "%7.2f,%7.2f,%7.2f, %7.2f,%7.2f,%7.2f,%7.2f,%7.2f\n", day_leachhu,day_addC, day_addmc, day_decomp[1],day_decomp[2],day_decomp[3],day_decomp[4],day_decomp[5]);//, C_soil);//+day_frostMicrobe);

    return( 0 );
}

int class_model::write_out_dayn( FILE *fn )
{
    float total_nh3=0.0;
    for(int i=1; i<=q; i++) total_nh3 += nh3[i];

   if (jday==1)
   {
    fprintf(fn, "Daily N pools and fluxes at site %s in year %d\n", site, year);
    fprintf(fn, ",N uptake,,N pools (kg N/ha),,,,,,,,,,,,,,,,N gas fluxes (kg N/ha/day)\n");
    fprintf(fn, "Day,Crop,Urea,NH4+,NO3-,exchengeable-NH4,NH4+,NO3-,exchengeable-NH4,NH4+,NO3-,exchengeable-NH4,NH4+,NO3-,exchengeable-NH4,NH4+,NO3-,exchengeable-NH4,NH3,N2O-flux,NO-flux,N2-flux,NH3-flux,NO3-leach,Urea-leach,DON-leach,ION-runoff,Gross N mineralization,N assimilation,Ice_DOC,Ice_N,Ice_N2O,Ice_N2,nitrification,nitrification,nitrification,nitrification,nitrification,denitrification,denitrification,denitrification,denitrification,denitrification,N_fixation,Litter_N,SON0-10cm, SON10-20cm,SON20-30cm,SON30-40cm,SON40-50cm,SON0-10cm, SON10-20cm,SON20-30cm,SON30-40cm,SON40-50cm,\n");
    fprintf(fn, ",kgN/ha/day,,0-10cm,0-10cm,0-10cm,10-20cm,10-20cm,10-20cm,20-30cm,20-30cm,20-30cm,30-40cm,30-40cm,30-40cm,40-50cm,40-50cm,40-50cm,40-50cm,,,,,,,,,,,,,,,0-10cm,10-20cm,20-30cm,30-40cm,40-50cm,0-10cm,10-20cm,20-30cm,30-40cm,40-50cm,\n\n");   
   }

    fprintf(fn, " %3d,", jday);
    fprintf(fn, "%7.2f,%7.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,%6.2f,",
            day_nup[1]+day_nup[2]+day_nup[3],
            
            day_dsurea+DayFixFert3,
            dsnh4[1], dsno3[1], dsClayN[1],
            dsnh4[2], dsno3[2], dsClayN[2],
            dsnh4[3], dsno3[3], dsClayN[3],
            dsnh4[4], dsno3[4], dsClayN[4],
            dsnh4[5], dsno3[5],	dsClayN[5],	
            total_nh3);	 
    fprintf(fn, "%8.6f,%8.6f,%8.6f,%8.2f,",
            day_soil_an_n2o,
            day_soil_an_no,
            day_soil_n2,
            day_soil_nh3+day_vol_nh3); 
    fprintf(fn, "%8.4f,%8.4f,%8.4f,%8.4f,%8.2f,%8.2f,%8.2f,%8.2f,%8.6f,%8.6f,%8.6f,%8.6f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.2f,",		    
            day_leach_NO3,
            day_leach_urea,
            day_leachun,
            day_runoff_N,
            day_N_mine,
            day_N_assim,
            day_ice_doc,
            day_ice_N,
            day_ice_n2o,
            day_ice_n2,
            day_nitrif[1],day_nitrif[2],day_nitrif[3],day_nitrif[4],day_nitrif[5], //day_soil_nitrify,
            day_denitrif[1],day_denitrif[2],day_denitrif[3],day_denitrif[4],day_denitrif[5],//day_soil_denitrification,
            day_soilN_from_Nfixation[1]+day_soilN_from_Nfixation[2]+day_soilN_from_Nfixation[3],
            day_inLitterN);
    fprintf(fn, "%8.2f,%8.2f,%8.2f,%8.2f,%8.2f,%8.4f,%8.4f,%8.4f,%8.4f,%8.4f\n",
        layern[1],layern[2],layern[3],layern[4],layern[5],topn[1],topn[2],topn[3],topn[4],topn[5]);
            
    return( 0 );
}

int class_model::write_out_daysp(FILE *fsp )
{
    if (jday==1)
    {
    fprintf(fsp, "Daily P pools (kg P/ha) and fluxes (kg P/ha/day) at site %s in year %d\n", site, year);
    fprintf(fsp, "Day,OrganicP,LabileP,SorbedP,ComplexP,CropDemandP,DayUptakeP,CropP, Leach_P, SO4, H2S_flux\n");
    }
    fprintf(fsp, "%3d,", jday);
    fprintf(fsp, "%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f\n", Profile_OrgP, Profile_LabP, Profile_AdsP, Profile_ComP, 
        day_P_demand[1], dayPlantP, PlantP[1], day_leach_P, SO4[1], day_H2S_flux);
            
    return( 0 );
}

int class_model::write_out_dayg( FILE *fg )
{
    float Tgrass=0.0;
    float dung_C=0.0, dung_N=0.0, urine_C=0.0, urine_N=0.0, dung_P=0.0;
    for (int i=1; i<=CropNumbers; i++)
    {
        Tgrass = Grain_Wt[i] + Leaf_Wt[i] + Stem_Wt[i];
    }

    for(int i2=1; i2<=5; i2++)
    {
        dung_C += day_graze_dung_C[i2];
        dung_N += day_graze_dung_N[i2];
        urine_C += day_graze_urine_C[i2];
        urine_N += day_graze_urine_N[i2];
        dung_P += day_graze_dung_P[i2];
    }

   if (jday==1)
   {
    fprintf(fg, "Daily grazing-induced C and N fluxes at site %s in year %d\n", site, year);
    fprintf(fg, "______________________________________________________________________________________________________________________________________________________\n");   
    fprintf(fg, "Day,Grass shoot-C (kgC/ha),Dairy,Beef,Pig,Sheep,Horse,Hours,Grazed-C,Grazed-N,Dung-C,Dung-N,Urine-C, Urine-N,Dung_P,Food deficit\n"); 
    fprintf(fg, ",kg C/ha,heads/ha,heads/ha,heads/ha,heads/ha,heads/ha,heads/ha,kgC/ha/day,kgN/ha/day,kgC/ha/day,kgN/ha/day,kgC/ha/day,kgN/ha/day,kgP/ha/day\n");   
    fprintf(fg, "______________________________________________________________________________________________________________________________________________________\n");
   }

    fprintf(fg, "%3d,", jday);
    fprintf(fg, "%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%5.2f,", 
        Tgrass, H_dairy, H_beef, H_pig, H_sheep, H_horse, H_hrs);
    fprintf(fg, "%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f\n",
            day_GrazeC, day_GrazeN, dung_C, dung_N, urine_C, urine_N, dung_P, day_deficit); //day_grass_consume
    
    return( 0 );
}

int class_model::write_out_dayp(FILE *fcrop)
{
    int i;

    if (jday==1)
    {
        fprintf(fcrop, "Daily crop growth at site %s",site);
        fprintf(fcrop, " in year %d\n", year);
        fprintf(fcrop, "Day,");
        fprintf(fcrop, "Temperature,Precipitation,Irrigation,Radiation,PET,Transpiration,Evaporation,Total_crops,,,");		
        for(int i=1;i<=CropNumbers;i++) fprintf(fcrop, ",Crop %d,,,,,,,,,,,,,,,,,,,,,,,,,,,", i);
        fprintf(fcrop, "\n");		
        fprintf(fcrop, ",,,,,,,,LeafC,StemC,RootC,GrainC,");
        
        for(int i3=1;i3<=CropNumbers;i3++) 
        fprintf(fcrop, "CropID,TDD,GrowthIndex,Water_demand,Water_stress,N_demand,N_stress,LAI,N_from_soil,N_from_air_NH3,N_fixation,Day_N_increase,TotalCropN,DailyCropGrowth,DayLeafGrowth,DayStemGrowth,DayRootGrowth,DayGrainGrowth,DayShootSenes,DayRootSenes,DayGrainSenes,LeafC,StemC,RootC,GrainC,LeafN,StemN,RootN,GrainN,");
        fprintf(fcrop, "\n");
        
        fprintf(fcrop, ",degree_C,mm,mm,MJ/m2/d,mm,mm,mm,kgC/ha/d,kgC/ha,kgC/ha,kgC/ha,");		
        for(int i4=1;i4<=CropNumbers;i4++) 
        fprintf(fcrop, ",degree_C,,mm,,,kgN/ha/d,,kgN/ha/d,kgN/ha/d,kgN/ha/d,kgN/ha/d,kgN/ha,kgC/ha/d,kgC/ha,kgC/ha,kgC/ha,kgC/ha,kgC/ha,kgC/ha,kgC/ha,kgC/ha,kgC/ha,kgC/ha,kgC/ha,kgN/ha,kgN/ha,kgN/ha,kgN/ha,");
        fprintf(fcrop, "\n");	
    }

    fprintf(fcrop, "%4d,", jday);
    fprintf(fcrop, "%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,", 
        air_temp, precipitation*1000.0,day_irri*1000.0, PAR,DayPET*1000.0, day_transp*1000.0, day_evapor*1000.0,
        Leaf_Wt[1]+Leaf_Wt[2]+Leaf_Wt[3], Stem_Wt[1]+Stem_Wt[2]+Stem_Wt[3], Root_Wt[1]+Root_Wt[2]+Root_Wt[3], 
        Grain_Wt[1]+Grain_Wt[2]+Grain_Wt[3]);
    for(cwc=1;cwc<=CropNumbers;cwc++) 
    {	
        fprintf(fcrop, "%10d,%10.2f,%10.2f,%10.2f,%10.2f,", 
            crop[cwc],TDD[cwc], PGI[cwc],PPT[cwc]*1000.0, day_DWater[cwc]);
        fprintf(fcrop, "%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,",
            day_N_demand[cwc], N_stress[cwc], lai[cwc], day_nup[cwc], day_plant_nh3[cwc], day_fixN[cwc], Day_GrowthN[cwc], 
            plantn[cwc]);
        fprintf(fcrop, "%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,",
                   Day_C_growth[cwc], dayG_leaf[cwc],dayG_stem[cwc],dayG_root[cwc],dayG_grain[cwc],DayLeafSenes[cwc]+DayStemSenes[cwc], DayRootSenes[cwc], DayGrainSenes[cwc],
                   Leaf_Wt[cwc], Stem_Wt[cwc], Root_Wt[cwc], Grain_Wt[cwc],
                   LeafN[cwc], StemN[cwc], RootN[cwc], GrainN[cwc]);//cut_root_litter[cwc]+fine_root_pool[cwc]		
    }
    fprintf(fcrop, "\n");

    return( 0 );
}

int class_model::write_out_dayEh( FILE *fEh )
{
   if (jday==1)
   {
    fprintf(fEh, "Daily Eh-driven processes at site %s",site);
    fprintf(fEh, " in year %d\n", year);
    fprintf(fEh, "____________________________________________________");
    fprintf(fEh, "____________________________________________________");
    fprintf(fEh, "________\n");
    fprintf(fEh, "Julian    Eh     Decom.  Nitri. Denit. CH4-pr CH4-ox    CH4     N2O      SOC      O2   soil_N2O\n");
    fprintf(fEh, "day       mV     kg C or N/ha/dayg\n");
    fprintf(fEh, "____________________________________________________");
    fprintf(fEh, "____________________________________________________");
    fprintf(fEh, "________\n");
    }
    fprintf(fEh, "%3d,", jday);
    fprintf(fEh, "%7.2f,%7.2f,%7.4f,%7.4f,%7.2f,%7.2f,%7.2f,%7.4f,%9.2f,%7.2f,%7.2f\n",
                    Eh[1],			
                    day_co2, 
                    day_soil_nitrify,
                    day_soil_denitrification,
                    ch4_prod,
                    ch4_day_oxid,
                    day_ch4,
                    day_soil_an_n2o,
                    day_co2 - day_addC,
                    O2_old[3],
                    w_n2o);

    return( 0 );
}

void class_model::write_out_dayCB(FILE *fCB, FILE *fNB)
{	
    //static float old_SOC;
    float Balance1, Balance2, net;//  = (day_co2 + day_balance_ch4 + day_leachhu)-(day_addC + day_addmc + day_RootInput_DOC +day_root_resp);//+day_RootDOC

    if (jday==1)
    {
        fprintf(fCB, "Daily soil C balance at site %s in year %d. Unit: kg C/ha\n", site, year);
        fprintf(fCB, "Day,IniPool  ,IniPool  ,IniPool ,IniPool  ,IniPool,IniPool,IniPool,IniPool,EndPool  ,EndPool  ,EndPool ,EndPool  ,EndPool,EndPool,EndPool,EndPool, InputC,InputC,InputC       ,InputC         ,OutputC,OutputC,OutputC ,C-balance,\n");
        fprintf(fCB, "Day,Litter,Humads,Humus,BlackC,DOC ,CO2 ,CH4 ,SOC ,Litter,Humads,Humus,BlackC,DOC ,CO2 ,CH4 ,SOC, Litter,Manure,RootExudation,RootRespiration,CO2flux,CH4flux,LeachSOC,error,\n");
    }
 
    Balance1 = (end_prof_SOC - ini_prof_SOC) - (day_addC+day_addmc+day_RootInput_DOC+day_root_resp) + (day_co2+day_balance_ch4+day_leachhu);
    
    fprintf(fCB, "%3d,", jday);
    fprintf(fCB, "%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,", 	
        ini_prof_litter, ini_prof_humad, ini_prof_humus, ini_prof_blackc, ini_prof_doc, ini_prof_co2, ini_prof_ch4, ini_prof_SOC);
    fprintf(fCB, "%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,", 
        end_prof_litter, end_prof_humad, end_prof_humus, end_prof_blackc, end_prof_doc, end_prof_co2, end_prof_ch4, end_prof_SOC);
    fprintf(fCB, "%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,\n", 	
        day_addC, day_addmc, day_RootInput_DOC, day_root_resp, day_co2, day_balance_ch4, day_leachhu, Balance1);
        
        
    //	ini_prof_SOC, prof_SOC, Balance1, day_addC, day_addmc, day_RootInput_DOC,day_root_resp,day_co2, day_balance_ch4, day_leachhu, Balance2, net);

    //old_SOC = prof_SOC;  

    ///////////////////////////////////////////////////////////////////////////////////
    //static float old_SON, old_SIN, old_clay_N;
    
    Balance2 = day_inN - day_outN;
    Balance1 = (end_SON+end_SIN)-(ini_SON+ini_SIN);
    net = Balance2-Balance1;

    if (jday==1)
    {
        fprintf(fNB, "Daily soil N balance at site %s in year %d. Unit: kg N/ha\n", site, year);
        fprintf(fNB, "___________________________________________________________________________________________________________________________________________________________________________\n");
        fprintf(fNB, ",N Pools,,,,,Influx N,,,,,,,Efflux N\n");
        fprintf(fNB, "Day,iniSON,iniSIN,endSON,endSIN,dSoilN,Deposit,Fertilizer,Manure,Litter,Irrigation,Fixation,Sum-In,Uptake,NH3,N2O,NO,N2,N-leach,Sum-Out,dFluxN,Error\n");
        fprintf(fNB, "___________________________________________________________________________________________________________________________________________________________________________\n");

        //old_SON = prof_SON;
        //old_SIN = prof_SIN;
        //old_clay_N = day_clay_N;
        
    }
    
    fprintf(fNB, "%3d,", jday);
    fprintf(fNB, "%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,",					
                //old_SON, prof_SON, old_SIN, prof_SIN, old_Pool_leach_NO3, Pool_leach_NO3, Balance1);
                ini_SON, ini_SIN, end_SON, end_SIN, Balance1);
    
    fprintf(fNB, "%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,",					
                day_inRainN, day_inFertN, day_inManureN, day_inLitterN, day_irri_N, 
                day_soilN_from_Nfixation[1]+day_soilN_from_Nfixation[2]+day_soilN_from_Nfixation[3], day_inN);
    
    fprintf(fNB, "%7.2f,%7.2f,%7.6f,%7.6f,%7.6f,%7.2f,%7.2f,",					
                day_nup[1]+day_nup[2]+day_nup[3], day_soil_nh3+day_vol_nh3, 
                day_soil_an_n2o, day_soil_an_no, day_soil_n2, 
                day_leach_NO3+day_leach_urea+day_leachhu/rcnm+by_pass_no3, day_outN);

    fprintf(fNB, "%7.2f,%7.2f\n", Balance2, net);

    //old_SON = prof_SON;
    //old_SIN = prof_SIN;
    //old_clay_N = day_clay_N;
}

int class_model::write_out_daym(FILE *fma)
{
    if (jday==1)
     {
        fprintf(fma, "Daily management at site %s in year %d\n\n", site, year);
        fprintf(fma, "Day,Irrigation (mm),Fertilizer (kgN/ha),Fertilizer (kgP/ha),Manure (kgN/ha),Manure (kgP/ha),Plant_cut (kg C/ha)\n\n");
     }
    
    fprintf(fma, "%3d,%7.4f,%7.1f,%7.1f,%7.1f,%7.1f,%7.1f\n",
            jday, day_irri*1000.0, day_inFertN, day_inFertP,day_inManureN, day_inManureP, DayGrassCut);

    return (0);
}

#ifdef ISOTOPE
int class_model::write_out_dayisotope(FILE *fis)
{
    if (jday==1)
    {
        fprintf(fis, "Daily soil C isotope at site %s in year %d, unit: kg C/ha\n\n", site, year);
        fprintf(fis, "Day,day_prod_DOC, day_DOC_from_root, day_DOC_from_SOM, DOC_pool, day_prod_CO2, day_CO2_from_root, day_CO2_from_SOM, CO2_pool, day_prod_H2, day_SOM_H2, H2_pool, CH4_prod_from_CO2,CH4_prod_from_DOC,CH4_prod_sum, CH4_oxid,CH4_flux,CH4_pool,	DOC-13,DOC-12,delta-DOC,CO2-13,CO2-12,delta-CO2,CH4_prod_13,CH4_prod_12,CH4_oxid_13,CH4_oxid_12,CH4_flux_13,CH4_flux_12,delta_flux_CH4,CH4_pool_13,CH4_pool_12,delta-sCH4(10cm),delta-sCH4(20cm),delta-sCH4(30cm),delta-sCH4(40cm),delta-sCH4(50cm)\n"); 
    }
    
    fprintf(fis, "%3d,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.2f,%7.4f,%7.4f,%7.4f,%7.2f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.2f,%7.4f,%7.4f,%7.4f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f\n",
            
            jday, 
            day_DOC_prod, day_Root_DOC, day_SOM_DOC, day_ch4_doc_pool, 
            day_CO2_prod, day_Root_CO2, day_SOM_CO2, day_ch4_co2_pool,
            day_H2_PROD, day_SOM_H2, day_ch4_H2_pool,
            ch4_prod1,ch4_prod2,ch4_prod, ch4_day_oxid,day_ch4,day_ch4_pool,

            day_DOC_13, day_DOC_12, del13C_DOC*1000.0,
            day_CO2_13, day_CO2_12, del13C_CO2*1000.0,			
            day_prod_CH4_13, day_prod_CH4_12,			
            day_oxid_CH4_13, day_oxid_CH4_12,
            fCH4_13, fCH4_12, 
            del_fCH4*1000.0,
            day_CH4_13, day_CH4_12, 			
            delCH4[LL1]*1000.0,delCH4[LL2]*1000.0,delCH4[LL3]*1000.0,delCH4[LL4]*1000.0,delCH4[LL5]*1000.0);

    return(0);
}
#endif

int class_model::write_out_daymicrobe(FILE *fmb)
{
    if (jday==1)
     {
        fprintf(fmb, "Daily soil microbial activity at site %s in year %d\n\n", site, year);
        fprintf(fmb, "Day,Decomposition (kgC/ha),Nitrification (kgN/ha),Denitrification (kgN/ha),Methanogenesis (kgC/ha),Methanotrophy (kgC/ha), Methanogen_index (1cm), Methanogen_index (10cm), Methanogen_index (20cm), Methanogen_index (30cm), Methanogen_index (40cm),Methanogen_index (50cm),"); 
        fprintf(fmb, "H2(kgH/ha/day-10cm), H2(kgH/ha/day-20cm), H2(kgH/ha/day-30cm), H2(kgH/ha/day-40cm), H2(kgH/ha/day-50cm),");  
        fprintf(fmb, "DOC(kgC/ha/day-10cm), DOC(kgC/ha/day-20cm), DOC(kgC/ha/day-30cm), DOC(kgC/ha/day-40cm), DOC(kgC/ha/day-50cm),"); 
        fprintf(fmb, "CO2(kgC/ha/day-10cm), CO2(kgC/ha/day-20cm), CO2(kgC/ha/day-30cm), CO2(kgC/ha/day-40cm), CO2(kgC/ha/day-50cm),");
        fprintf(fmb, "DOC_CH4(kgC/ha/day-10cm), DOC_CH4(kgC/ha/day-20cm), DOC_CH4(kgC/ha/day-30cm), DOC_CH4(kgC/ha/day-40cm), DOC_CH4(kgC/ha/day-50cm),");
        fprintf(fmb, "CO2_CH4(kgC/ha/day-10cm), CO2_CH4(kgC/ha/day-20cm), CO2_CH4(kgC/ha/day-30cm), CO2_CH4(kgC/ha/day-40cm), CO2_CH4(kgC/ha/day-50cm),");
        fprintf(fmb, "CH4_Oxi(kgC/ha/day-10cm), CH4_Oxi(kgC/ha/day-20cm), CH4_Oxi(kgC/ha/day-30cm), CH4_Oxi(kgC/ha/day-40cm), CH4_Oxi(kgC/ha/day-50cm)\n\n");
     }
    
    fprintf(fmb, "%3d,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.2f,%7.5f,%7.5f,%7.5f,%7.5f,%7.5f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f,%7.4f\n",
            jday, soil_hete_CO2, day_soil_nitrify, day_soil_denitrification,ch4_prod, ch4_day_oxid, F_methanogen[LL0], F_methanogen[LL1], F_methanogen[LL2], F_methanogen[LL3], F_methanogen[LL4], F_methanogen[LL5],
            day_H2_prod[LL1], day_H2_prod[LL2], day_H2_prod[LL3], day_H2_prod[LL4], day_H2_prod[LL5], 
            ch4_doc_pool[LL1], ch4_doc_pool[LL2], ch4_doc_pool[LL3], ch4_doc_pool[LL4], ch4_doc_pool[LL5],
            day_ch4_CO2[LL1],day_ch4_CO2[LL2],day_ch4_CO2[LL3],day_ch4_CO2[LL4],day_ch4_CO2[LL5],
            day_ch4_2[LL1], day_ch4_2[LL2], day_ch4_2[LL3], day_ch4_2[LL4], day_ch4_2[LL5], 
            day_ch4_1[LL1], day_ch4_1[LL2], day_ch4_1[LL3], day_ch4_1[LL4], day_ch4_1[LL5], 
            day_ch4_3[LL1], day_ch4_3[LL2], day_ch4_3[LL3], day_ch4_3[LL4], day_ch4_3[LL5]);

    return (0);
}

int class_model::write_out_cropini(int MODE )
{
    int i, KK=0, LeftDays[8];
    FILE *fp;
    char F_INI[300];
    
    {
    for(int i=1; i<=CropNumbers; i++)
    {
        if(crop_flag[i]==1) 
        {
            KK++;
            crop[KK]=crop[i];
            sprintf(crop_name[KK], "%s", crop_name[i]);
            seed[KK]=seed[i];
            plantn[KK]=plantn[i];
            PlantTakeN[KK]=PlantTakeN[i];
            demandn[KK]=demandn[i];
            lai[KK]=lai[i];
            Leaf_Wt[KK]=Leaf_Wt[i];
            Stem_Wt[KK]=Stem_Wt[i];
            Grain_Wt[KK]=Grain_Wt[i];
            Root_Wt[KK]=Root_Wt[i];
            TDD[KK]=TDD[i];
            crop_trans[KK]=crop_trans[i];

            WCLeftDays[KK]=grow_days[i]-seed[i];
            GrainYield[KK]=GrainYield[i];
            HarvestYear[KK]=HarvestYear[i]-1;
            jdseed[KK]=jdseed[i];
            grow_days[KK]=grow_days[i];
            left_perc[KK]=left_perc[i];
            maxmn[KK]=maxmn[i];
            InitialBiomass[KK]=InitialBiomass[i];
            GrowthVegetative[KK]=GrowthVegetative[i];
            GrowthReproductive[KK]=GrowthReproductive[i];
            PsnEfficiency[KK]=PsnEfficiency[i];
            PsnMax[KK]=PsnMax[i];
            CoverCrop[KK]=CoverCrop[i];
            PerennialFlag[KK]=PerennialFlag[i];
            grain[KK]=grain[i];
            stem[KK]=stem[i];
            root[KK]=root[i];
            base_graincn[KK]=base_graincn[i];
            base_leafcn[KK]=base_leafcn[i];
            base_stemcn[KK]=base_stemcn[i];
            base_rootcn[KK]=base_rootcn[i];
            max_TDD[KK]=max_TDD[i];
            wreq[KK]=wreq[i];
            base_wreq[KK]=base_wreq[i];
            //max_root_length[KK]=max_root_length[i];
            OptimumT[KK]=OptimumT[i];
            ini_N_fix[KK]=ini_N_fix[i];
            vascularity[KK]=vascularity[i];
            maxmn[KK] = ini_maxmn[i];
        }
    }
    }

    IniCropNumbers = KK;

    if(LandUse==6) stem_age++;
    else stem_age = 0;

    sprintf(F_INI, "%s\\INPUTS\\cropini-%d", OUTPUT, year+1);    
    fp = fopen(F_INI, "w");
    if (fp == NULL) note(1, F_INI); 
    fprintf(fp, "%d\n", IniCropNumbers);
    {
        for(int i=1; i<=IniCropNumbers; i++)
        {
            fprintf(fp, "%d %d %f %f %f %f\n",crop[i], seed[i], plantn[i], PlantTakeN[i], demandn[i], lai[i]);
            fprintf(fp, "%f %f %f %f %f\n", Leaf_Wt[i], Stem_Wt[i], Grain_Wt[i], Root_Wt[i], TDD[i]);
            fprintf(fp, "%f %f\n", crop_trans[i], plantcn[i]);
            //////////////////////////////////////////
            fprintf(fp, "%d %s %d %d %f %d\n", 
                crop[i], crop_name[i], 1, WCLeftDays[i], GrainYield[i], HarvestYear[i]);
            fprintf(fp, "%d %d %f %f %d\n", 
                jdseed[i], grow_days[i], left_perc[i], maxmn[i], 111);			
            fprintf(fp, "%f %d %f %f %f\n", 
                InitialBiomass[i], Transplant[i], GrowthReproductive[i],
                PsnEfficiency[i], PsnMax[i]);
            fprintf(fp, "%d\n", CoverCrop[i]);
            fprintf(fp, "%d\n", PerennialFlag[i]);
            fprintf(fp, "%f %f %f %f\n", grain[i], leaf[i], stem[i], root[i]);
            fprintf(fp, "%f %f %f %f\n", base_graincn[i], base_leafcn[i], base_stemcn[i], base_rootcn[i]);//Brian
            fprintf(fp, "%f %f %f %f %f\n", max_TDD[i], base_wreq[i], OptimumT[i], ini_N_fix[i], vascularity[i]);//Brian
            fprintf(fp, "%f %d\n", PlantP[i], stem_age);
            ///////////////////////////////////////////
        }
    }

    fclose( fp );

    return( 0 );
}

int class_model::write_out_soil_parm(int MODE, int SoilYear, int SimuYears)
{
    FILE *fp;
    char  SoilYr[200], SOIL[300];
    int   l;
    float wrcvl=0.0, wrcl=0.0, wrcr=0.0, wcrb=0.0, wcrhl=0.0, wcrhr=0.0; 
    float whumus=0.0, wtcavai=0.0, winertC=0.0, wFreezedoc=0.0;
    float wrnvl=0.0, wrnl=0.0, wrnr=0.0, wnrb=0.0, wnrhl=0.0, wnrhr=0.0, whumusn=0.0;//, winertC/PassiveCCN, ;
    float w_n2o=0.0, w_n2=0.0, w_nh3=0.0, w_no3=0.0, w_nh4=0.0, wurea=0.0, day_clay_N=0.0, wpool_no=0.0; 
    float InertN=0.0;

    if (MODE==0)
       sprintf(SOIL, "%s\\INPUTS\\soil-%d", OUTPUT, year + 1);
    else 
    {
        if(SoilYear<=1) 
        {
            sprintf(SoilYr, "Soil-A");
            sprintf(SOIL, "%s\\INPUTS\\%s%d", OUTPUT, SoilYr, year + 1);
        }
        else 
        {
            sprintf(SoilYr, "Soil-B");
            sprintf(SOIL, "%s\\INPUTS\\%s%d", OUTPUT, SoilYr, year + 1);
        }
    }
    
        fp = fopen(SOIL, "w");
        if ( fp == NULL )
        {
            char err[60];
            sprintf(err,"can not create file %s",SOIL);
            AfxMessageBox(err);
            exit(0);
        }

        fprintf(fp, "%d\n", st[1]);
        fprintf(fp, "%s\n", soil);
    
        fprintf(fp, "%f %f %f %f %f %f %f %f\n", clay[1],sts[1], sks[1], ini_fldcap,wiltpt[1],LateralInflux,0.0,0.0);
        fprintf(fp, "%f %f %d %f %f %f %f %f %f\n", sd[1],soil_ph[1], idc, ini_ttoo[1], ini_no3, ini_nh4, resoc, ahsoc, phsoc);
        fprintf(fp, "%f %f %f %f %f %d\n", byp,h[1], th, mmm[1],vvv[1], q);
        fprintf(fp, "%f %f %f %f %d\n", clay_f1, clay_f2,qstar, by_passf, retard);
        fprintf(fp, "%f %f %f %f %f %f\n", rcnb, rcnh,rcnm, rcnrvl,rcnrl, rcnrr);
        fprintf(fp, "%f %f %f %f %f %f\n", leaf_water,surf_water, snow_pack,stub1, stub2, stub3);
        fprintf(fp, "%f %f %f\n",PassiveCCN, 0.0, 0.0);

        for ( l = 1; l <= q; l++ )
        {
            fprintf(fp, "\n");
            fprintf(fp, "%f %f %f\n", rcvl[l], rcl[l], rcr[l]);
            fprintf(fp, "%f %f %f %f %f\n", CRB1[l], CRB2[l], crhl[l], crhr[l], dphum[l]);
            fprintf(fp, "%f %f %f %f %f %f\n", no2[l], no3[l], nh4[l], clay_nh4[l], urea[l], nh3[l]);
            fprintf(fp, "%f %f %f\n", no[l], n2o[l], n2[l]);
            fprintf(fp, "%f %f %f\n", temp[l], day_wfps[l], ice[l]);
            fprintf(fp, "%f %f\n", Eh[l], ch4[l]);
            fprintf(fp, "%f %f %f %f\n", FreezeDOC[l], F_methanogen[l], 0.0, 0.0);
            fprintf(fp, "%f %f\n", inert_C[l], doc[l]);

            fprintf(fp, "%d %f %f %f\n", st[l], h[l], SoilDepth[l], ini_ttoo[l]);
            fprintf(fp, "%f %f %f %f %f %f\n", sts[l], clay[l], ini_fldcap, wiltpt[l], sd[l], soil_ph[l]);
            fprintf(fp, "%f %f %f\n", mmm[l], vvv[l], sks[l]);
            fprintf(fp, "%f %f %f\n", Fe3[l], Mn4[l], SO4[l]);
            fprintf(fp, "%f %f %f %f\n", OrgP[l], AdsP[l], LabP[l], ch4_doc_pool[l]);

            wrcvl += rcvl[l]; wrcl += rcl[l]; wrcr += rcr[l]; wcrb += (CRB1[l]+CRB2[l]); wcrhl += crhl[l]; wcrhr += crhr[l]; 
            whumus += dphum[l]; winertC += inert_C[l]; wFreezedoc += FreezeDOC[l];
            wtcavai += doc[l]; 

            w_n2o += n2o[l]; w_n2 += n2[l]; w_nh3 += nh3[l]; w_no3 += no3[l];
            w_nh4 += nh4[l]; wurea += urea[l]; day_clay_N += clay_nh4[l]; wpool_no += no[l]; 
        }

        fprintf(fp, "%f\n", BaseMicroIndex); 		
        fprintf(fp, "%f\n", WaterRetentionDepth); //m
        fprintf(fp, "%f %f\n", sslope, salinity); //0-100
        fprintf(fp, "%f\n", WaterPool); 
        fprintf(fp, "%d\n", ManureDays);
        fprintf(fp, "%f\n", day_WT);
        fprintf(fp, "%d %d\n", flood_flag, FloDays);
        fprintf(fp, "%f\n", DrainEfficiency);
        fprintf(fp, "%d\n", SCSuse);
        fprintf(fp, "%f\n", PassiveC);
        fprintf(fp, "%f\n", DClitter);
        fprintf(fp, "%f\n", DChumads);
        fprintf(fp, "%f\n", DChumus);
        fprintf(fp, "%f %f %f\n", remain_root, FineRootCN, 0.0);//, fine_root_pool[1]
        fprintf(fp, "%f %f\n", frost_doc, C_soil);
        fprintf(fp, "%f %f %f %f %f %f %f\n", DayFixFert1,DayFixFert2,DayFixFert3,DayFixFert4,DayFixFert5,DayFixFert6,DayFixFert7); 
        fprintf(fp, "%f %f %f %f %f %f %f\n", sfer1,sfer2,sfer3,sfer4,sfer5,sfer6,sfer7);
        fprintf(fp, "%d\n", UreaAppDays);
        fclose( fp );

        if(SimuYears==year)
        {
            char FileB[300];
            sprintf(FileB, "%s\\Inter\\%s-%s.txt", OUTPUT, SoilYr, site);
            File_Copy(SOIL, FileB);
        }

        wrnvl = wrcvl/rcnrvl;
        wrnl = wrcl/rcnrl;
        wrnr = wrcr/rcnrr;
        wnrb = wcrb/rcnb;
        wnrhl = wcrhl/rcnh;
        wnrhr = wcrhr/rcnh2;
        whumusn = whumus/rcnm;
        InertN = winertC/PassiveCCN;

        wno32 = w_no3;
        wnh42 = w_nh4;
        wnh32 = w_nh3;
        wurea2 = wurea;
        wno2 = wpool_no;
        day_clay_n2 = day_clay_N;
        end_N_gases = w_n2o + w_n2;
        EndSIN = wno32 + wnh42 + wnh32 + wurea2 + wno2 + day_clay_n2 + end_N_gases;

        whumusn2 = whumusn;
        whumus2 = whumus;

        EndLitter = wrcvl + wrcl + wrcr;
        EndActHumus = wcrb + wcrhl + wcrhr;
        EndSOC = T_SOC;//EndLitter + EndActHumus + whumus + wtcavai + winertC + wFreezedoc;		

        EndLitterN = wrnvl + wrnl + wrnr;
        EndActHumusN = wnrb + wnrhl + wnrhr;
        EndSON = EndLitterN + EndActHumusN + whumusn + InertN;
        //end_N_gases = w_n2o + w_n2;
        //EndSIN = w_nh3 + w_no3 + w_nh4 + wurea + day_clay_N + wpool_no + end_N_gases;
    
    return( 0 );
}

int File_Copy(char *source, char *dest)
{
    FILE *fpi,*fpo;
    long flen,rt,num,adr;
    char *p;

    p=(char*)malloc(4096);
    if (p==NULL) {
        MessageBox(NULL,"No enough memory!","Running Error:",0);
        return -1;
    }

    fpi=fopen(source,"rb");
    if (fpi==NULL) {
        CString note;
        note.Format("Cannot open source file %s",source);
        AfxMessageBox(note);
        exit(0);
    }
    fpo=fopen(dest,"wb");
    if (fpo==NULL) {
        CString note;
        note.Format("Cannot create file %s", dest); 
        AfxMessageBox(note);
        exit(0);
    }
    flen=_filelength(_fileno(fpi));
    num=4096;
    adr=0;
    for (;;) {
        fseek(fpi,adr,SEEK_SET);
        fseek(fpo,adr,SEEK_SET);
        if (adr+num>flen) num=flen-adr;
        rt=fread(p,sizeof(char),num,fpi);
        rt=fwrite(p,sizeof(char),num,fpo);
        adr+=num;
        if (adr==flen) break;
    }
    fclose(fpi);
    fclose(fpo);
    free(p);
    return 0;
}

int class_model::write_out_wb(int Batch, char *BatchFileName, char *PassC1)
{
    FILE *fout;
    char  junkc[300];

    if(Batch==1) 
    {
#ifdef DEGUI
        sprintf(junkc, "%s\\RECORD\\Site\\Multi_year_water.%s", OUTPUT, CSV);
#else
        sprintf(junkc, "%s\\Multi_year_water.%s", PassC1, CSV);
#endif
    }
    else
    {
#ifdef MANURE
        sprintf(junkc, "%s\\Multi_year_water.%s", PassC1, CSV);
#else
        sprintf(junkc, "%s\\RECORD\\Site\\Multi_year_water.%s", OUTPUT, CSV);
#endif
    }

    if (year==1)
    {		
        fout = fopen(junkc, "w" );
        if ( fout==NULL )
        {
            char err[60];
            sprintf(err,"cannot create %s",junkc);
                MessageBox(NULL,err,"Error Message",0);
                return( FILE_NOT_FOUND );
        }

        fprintf(fout, "Annual water budget for site: %s\n", site);
        fprintf(fout, "Initial water pools (mm),,,,,Input (mm/yr),,,Output (mm/yr),,,,,,,,End water pools (mm)\n");
        fprintf(fout, "Year,Soil,Snow,Leaf,Pond,Deep,Precipitation,Irrigation,Interception,Snow evap,Leaf evap,Pond evap,Soil evap,Tranpiration,Leaching,Runoff,Soil,Snow,Leaf,Pond,Deep\n");
    }
    else
    {
        fout = fopen(junkc, "a" );
        if ( fout==NULL )
        {
            char err[60];
            sprintf(err,"cannot create %s",junkc);
                MessageBox(NULL,err,"Error Message",0);
                return( FILE_NOT_FOUND );
        }
    }

    /*float wa, wb;
    if(irr_num==0)
    {
        wa = yr_prec_water;
        wb = yr_irri;
    }
    else
    {
        wa = yr_prec_water-yr_irri;
        wb = yr_irri;
    }*/

    fprintf(fout, "%3d,", year);
    fprintf(fout, "%5.1f,%5.1f,%5.1f,%5.1f,%5.1f,",
        yr_ini_water * 1000.0,
        ini_snow_pack * 1000,
        ini_leaf_water * 1000,
        ini_surf_water * 1000,
        PoolWater0 * 1000);
    fprintf(fout, "%5.1f,%5.1f,%5.1f,",
        yr_prec_water * 1000,
        yr_irri * 1000,
        yr_intercept * 1000);
    fprintf(fout, "%5.1f,%5.1f,%5.1f,%5.1f,%5.1f,%5.1f,%5.1f,",
        yr_snow_ev * 1000,
        yr_fol_ev * 1000,
        yr_pond_ev * 1000,
        yr_soil_ev * 1000,
        //yr_paddy_water_ev * 1000,
        yr_trans * 1000,
        yr_leach_water * 1000,
        yr_runoff * 1000);
    fprintf(fout, "%5.1f,%5.1f,%5.1f,%5.1f,%5.1f\n",
        yr_end_water * 1000,
        snow_pack * 1000,
        leaf_water * 1000,
        surf_water * 1000,
        WaterPool * 1000);
    //fprintf(fout, "\n");
    fclose( fout );

    return( 0 );
}

int class_model::multi_yr_record(int MODE, int Batch, float EndSOC, char *Pass, char *PassC1)
{
    char F_RECORD[400], BatchFileName[150];
    FILE *fp;

    float dSN = -IniSON - IniSIN + EndSON + EndSIN;

    if(Batch==0)
    {
#ifdef MANURE
        sprintf(F_RECORD, "%s\\Multi_year_summary.%s", PassC1, CSV);
#else
        sprintf(F_RECORD, "%s\\RECORD\\Site\\Multi_year_summary.%s", OUTPUT, CSV);	
#endif
    }
    else
    {
#ifdef DEGUI
        sprintf(F_RECORD, "%s\\Multi_year_summary.%s", Pass, CSV);
#else
        
#ifdef MANURE
        sprintf(F_RECORD, "%s\\Multi_year_summary.%s", PassC1, CSV);
#else		
        sprintf(F_RECORD, "%s\\Multi_year_summary.%s", Pass, CSV);
#endif			

#endif
        
    }	

    float TotalCutC = 0.0;
    for(int i=1;i<=CropNumbers;i++) 
    {
        TotalCutC += (yr_CutGrain[i]+yr_CutLeaf[i]+yr_CutStem[i]+yr_CutRoot[i]);
    }
    
    if (year == 1)
    {
        fp = fopen(F_RECORD, "w");
        if (fp==NULL) note(0, F_RECORD);
        
        fprintf(fp, "Multi-year report for site %s\n", site);
        fprintf(fp, "\n"); 
        
        fprintf(fp, "%-5s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s\n",	
            "Year","GrainC1","LeafC1","StemC1","RootC1","GrainC2","LeafC2","StemC2","RootC2","GrainC3","LeafC3","StemC3","RootC3","SOC 0-10cm frac","SOC 10-20cm frac","SOC 20-30cm frac","SOC 30-40cm frac","SOC 40-50cm frac","SOC 0-10cm","SOC 10-20cm","SOC 20-30cm","SOC 30-40cm","SOC 40-50cm",
            "IniSOC","End_SOC","dSOC","LitterC_input","RootC_input","ManureC_input",
            "Soil-CO2","Leach_DOC","CH4","IniSON","IniSIN","EndSON","EndSIN",
            "dSN","Atmo_N_input","Fertilizer_N_input","Manure_N_input","Litter_N_input","N_fixation",
            "Crop_N_uptake","Leach_NO3","Runoff_SON","Leach_DON","N2O_flux","NO_flux","N2_flux","NH3_flux","exchengeable-NH4",
            "PET","Transpiration","Evaporation","WaterLeach","Runoff","Irrigation",
            "Precipitation", "MeanT","WindSpeed","ColdStress","WaterStress","N_Stress","Cut_CropC",
            "yr_end_grain", "yr_end_leaf", "yr_end_stem", "yr_end_root", "yr_end_stub");
        fprintf(fp, "%-5s,%-11s,%-11s,%-5s,%-11s,%-11s,%-5s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,,%-11s%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s,%-11s\n\n",
            "","kgC/ha/y","kgC/ha/y","kgC/ha/y","kgC/ha/y","kgC/ha/y","kgC/ha/y","kgC/ha/y","kgC/ha/y","kgC/ha/y","kgC/ha/y","kgC/ha/y","kgC/ha/y","kgC/kg","kgC/kg","kgC/kg","kgC/kg","kgC/kg","kgC/ha","kgC/ha","kgC/ha","kgC/ha","kgC/ha","kgC/ha","kgC/ha","kgC/ha/y","kgC/ha/y",
            "kgC/ha/y","kgC/ha/y","kgC/ha/y","kgC/ha/y","kgC/ha/y","kgN/ha","kgN/ha","kgN/ha",
            "kgN/ha","kgN/ha/y","kgN/ha/y","kgN/ha/y","kgN/ha/y","kgN/ha/y","kgN/ha/y","kgN/ha/y","kgN/ha/y","kgN/ha/y","kgN/ha/y","kgN/ha/y",
            "kgN/ha/y","kgN/ha/y","kgN/ha/y","kgN/ha","mm","mm","mm","mm","mm","mm","mm","degree","m/s","","","","kgC/ha/y","kgC/ha","kgC/ha","kgC/ha","kgC/ha","kgC/ha");
    }
    else
    {
        fp = fopen(F_RECORD, "a");
        if (fp==NULL) note(1, F_RECORD); 
    }
    fprintf(fp, "%5d,%11.2f,%11.2f,%11.2f,%11.2f,%11.2f,%11.2f,%11.2f,%11.2f,%11.2f,%11.2f,%11.2f,%11.2f,%15.4f,%15.4f,%15.4f,%15.4f,%15.4f,%11.2f,%11.2f,%11.2f,%11.2f,%11.2f,%11.2f,%11.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%15.2f,%11.2f,%11.2f,%11.2f,%11.2f,%11.2f,%11.2f,%11.2f,%11.2f\n",
        year,	
        YrG_grain[1],//+yr_CutGrain[1]+yr_GrazeGrain[1],
        YrG_leaf[1],
        YrG_stem[1],//+yr_CutLeaf[1]+yr_CutStem[1]+yr_GrazeShoot[1],
        YrG_root[1],//+yr_CutRoot[1]+yr_GrazeRoot[1],		
        YrG_grain[2],//+yr_CutGrain[2]+yr_GrazeGrain[2],
        YrG_leaf[2],
        YrG_stem[2],//+yr_CutLeaf[2]+yr_CutStem[2]+yr_GrazeShoot[2],
        YrG_root[2],//+yr_CutRoot[2]+yr_GrazeRoot[2],
        YrG_grain[3],//+yr_CutGrain[3]+yr_GrazeGrain[3],
        YrG_leaf[3],
        YrG_stem[3],//+yr_CutLeaf[3]+yr_CutStem[3]+yr_GrazeShoot[3],
        YrG_root[3],//+yr_CutRoot[3]+yr_GrazeRoot[3],
        topc[1],topc[2],topc[3],topc[4],topc[5],layerc[1],layerc[2],layerc[3], layerc[4], layerc[5], 
        IniSOC,EndSOC,EndSOC-IniSOC,yr_addtc,yr_addrc,yr_addmc,//end_SOC
        yr_soil_co2,yr_leach_hu, yr_ch4,
        IniSON,IniSIN,EndSON,EndSIN,dSN, 		
        yr_rainN,yr_addfer,yr_addmn,yr_sln,yr_soilN_from_Nfixation,//yr_addtn+yr_sln+yr_addrn
        yr_uptaken[1]+yr_uptaken[2]+yr_uptaken[3], yr_leach_NO3+yr_leach_urea,yr_runoff_N, yr_leach_un,yr_dnn2o, yr_dnno, yr_dnn2, yr_soil_nh3, day_clay_N,
        yr_PET*1000.0,yr_trans*1000.0, (yr_pond_ev+yr_soil_ev)*1000.0, yr_leach_water*1000.0, yr_runoff*1000.0,
        yr_irri*1000.0, yr_rain*1000.0, yr_avet, yr_WindSpeed/365.0, TDD[1]/max_TDD[1], crop_water_stress[1]/crop_days[1],crop_N_stress[1]/crop_days[1],
        TotalCutC, yr_end_grain, yr_end_leaf, yr_end_stem, yr_end_root,
        stub1+stub2+stub3); 
     fclose( fp );
    return( 0 );
}

int class_model::write_out_annual_report(int MODE, float ha, int Batch, int livestock, FILE *fout, 
                                         FILE *fout2, int CL, float *FarmCropArea, 
                                         float *FarmCropManureFraction, int drc, char *Pass, char *PassC1, char *PassL1)
{
    //FILE  *fout;
    char  YR[300], date_time[300];
    
    time_t  t;
    float CNRatio, GrainC, LeafC, StemC, RootC;
    float YrLeachNO3, R_plantn[8];

    time(&t);
    sprintf(date_time, "%s", ctime(&t));

    yr_dSON = EndSON - IniSON;
    yr_dSIN = EndSIN - IniSIN;

    CNRatio = EndSOC / EndSON;

    for(int i=1;i<=4; i++)
    {
        GrainC = YrG_grain[i];// +yr_CutGrain[i]+yr_GrazeGrain[i];
        LeafC = YrG_leaf[i];
        StemC = YrG_stem[i];// + yr_CutLeaf[i]+yr_CutStem[i]+yr_GrazeShoot[i];
        RootC = YrG_root[i];// +yr_CutRoot[i]+yr_GrazeRoot[i];
    }
    
    YrLeachNO3 = yr_leach_NO3+yr_leach_urea;
    if (YrLeachNO3 < 0.0) YrLeachNO3 = 0.0;

    char BatchFileName[400];

    float G_days[8];
    for(int mb=1; mb<=7; mb++)
    {
        if(crop_days[mb]==0) crop_days[mb]=1;
    
        G_days[mb]=crop_days[mb];

        R_plantn[mb]=yr_plantn[mb];
    }

    //fout = fopen( YR, "w" );
    //if (fout==NULL) note(1, YR);

    //YIELD2(yr_uptaken[1]+yr_fixn[1],yr_uptaken[2]+yr_fixn[2],yr_uptaken[3]+yr_fixn[3]);			
    //YIELD3(plantn[1],plantn[2],plantn[3]);	
    
    //if(yr_pond_ev>0.0) yr_irri = (yr_pond_ev+yr_trans+yr_soil_ev+yr_runoff+yr_leach_water) - yr_rain/100.0;
    //if(yr_irri<0.0) yr_irri = 0.0;
    yr_plant_demand = (yr_pond_ev+yr_trans+yr_soil_ev)*1000.0;
    yr_TotalEvap = (yr_pond_ev + yr_soil_ev)*1000.0;
    yr_PrecWater = yr_rain*10.0;
    yr_dSoilWater = (yr_rain+yr_irri) - (yr_trans+yr_soil_ev+yr_runoff+yr_leach_water); //m


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

//if(livestock==0)
{
    int i;
    float _ha=FarmCropArea[CL];
    float FIC = (yr_Lagoon_to_field_C_T+yr_Compost_to_field_C_T+yr_Digester_to_field_C_T)*FarmCropManureFraction[CL] + (yr_addtc+yr_addrc)*_ha;
    float FOC = (yr_soil_co2+yr_ch4)*_ha;
    float DFC = FIC - FOC;
    
    float FIN = (yr_Lagoon_to_field_N_T+yr_Compost_to_field_N_T+yr_Digester_to_field_N_T)*FarmCropManureFraction[CL] + 
        (yr_rainN+yr_sln+yr_addfer+yr_irri_N+yr_soilN_from_Nfixation+yr_NH3_deposit)*_ha;
    float FON = (yr_soil_nh3+yr_dnn2o+yr_dnno+yr_dnn2+yr_uptaken[1]+yr_uptaken[2]+yr_uptaken[3]+YrLeachNO3+yr_runoff_N)*_ha;
    float DFN = FIN - FON;
    
    float FIP = (yr_Lagoon_to_field_P_T+yr_Compost_to_field_P_T+yr_Digester_to_field_P_T)*FarmCropManureFraction[CL];
    float FOP = (PlantP[1]+PlantP[2]+PlantP[3]+yr_runoff_P)*_ha;	
    float DFP = FIP - FOP;
    
    float DFW = yr_dSoilWater*10000.0*_ha*1000.0;
    float TCN = (yr_uptaken[1]+crop_NH3_absorp[1]+yr_fixn[1]) * _ha;
    float CNF = (crop_NH3_absorp[1]+yr_fixn[1])*_ha;
    float GNC = YrG_grain[1]/graincn[1]*_ha;
    float SNC = (YrG_leaf[1]/leafcn[1]+YrG_stem[1]/stemcn[1])*_ha;
    float RNC = TCN-GNC-SNC;

#ifdef MANURE

if(m_unit==0)
{
    CString FFD;
    FFD.Format("FIELD_%d_%s_%.2fha", CL, crop_name[1], FarmCropArea[CL]);
    fprintf(fout2, "%-25s %11s     %11s     %11s     %11s\n\n", FFD, "C_kg", "N_kg", "Water_kg", "P_kg");
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Influx_from_lagoon", yr_Lagoon_to_field_C_T*FarmCropManureFraction[CL], yr_Lagoon_to_field_N_T*FarmCropManureFraction[CL], yr_Lagoon_to_field_H2O_T*FarmCropManureFraction[CL], yr_Lagoon_to_field_P_T*FarmCropManureFraction[CL]);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Influx_from_compost", yr_Compost_to_field_C_T*FarmCropManureFraction[CL], yr_Compost_to_field_N_T*FarmCropManureFraction[CL], yr_Compost_to_field_H2O_T*FarmCropManureFraction[CL], yr_Compost_to_field_P_T*FarmCropManureFraction[CL]);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n\n", "Influx_from_digester", yr_Digester_to_field_C_T*FarmCropManureFraction[CL], yr_Digester_to_field_N_T*FarmCropManureFraction[CL], yr_Digester_to_field_H2O_T*FarmCropManureFraction[CL], yr_Digester_to_field_P_T*FarmCropManureFraction[CL]);

    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Influx_from_precipitation", 0.0, yr_rainN*_ha, yr_rain*10000.0*_ha*1000.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Influx_from_crop_residue", (yr_addtc+yr_addrc)*_ha, (yr_sln)*_ha, 0.0, 0.0);//yr_addtn + +yr_addrn
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Influx_from_fertilizer", 0.0, yr_addfer*_ha, 0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Influx_from_irrigation", 0.0, yr_irri_N*_ha, yr_irri*10000.0*_ha*1000.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Influx_from_grazing_herd", (yr_graze_urine_C+yr_graze_dung_C)*_ha, (yr_graze_urine_N+yr_graze_dung_N)*_ha, 0.0, yr_graze_dung_P);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n\n", "Influx_from_N_fixation", 0.0, yr_soilN_from_Nfixation*_ha, 0.0, 0.0);

    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Soil_CO2_emission", (yr_soil_co2)*_ha, 0.0,0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Soil_CH4_emission", yr_ch4*_ha, 0.0,0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Soil_NH3_emission", 0.0, yr_soil_nh3*_ha, 0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Soil_N2O_emission", 0.0, yr_dnn2o*_ha, 0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Soil_NO_emission", 0.0, yr_dnno*_ha, 0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Soil_N2_emission", 0.0, yr_dnn2*_ha, 0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Uptake_by_crop", 0.0, (yr_uptaken[1]+yr_uptaken[2]+yr_uptaken[3])*_ha, yr_trans*10000.0*_ha*1000.0, PlantP[1]*_ha);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Soil_N_loss_by_leaching", 0.0, YrLeachNO3*_ha, (yr_leach_water)*10000.0*_ha*1000.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Soil_runoff_loss", yr_runoff_SOC*_ha, yr_runoff_ION*_ha, yr_runoff*10000.0*_ha*1000.0, yr_runoff_P*_ha);

    fprintf(fout2, "%-25s %e    %e     %e    %e\n\n", "Soil_water_evaporation", 0.0, 0.0, (yr_soil_ev+yr_pond_ev)*10000.0*_ha*1000.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n\n", "Soil_sequestration", DFC, DFN, DFW, DFP);

    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Crop_N_fixation", 0.0, CNF, 0.0, 0.0); 
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Crop_grain", YrG_grain[1]*_ha, GNC, 0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Crop_shoot", (YrG_leaf[1]+YrG_stem[1])*_ha, SNC, 0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Crop_root", YrG_root[1]*_ha, RNC, 0.0, 0.0);
    fprintf(fout2, "---------------------------------------------------------------------------------------------\n");
}
else
{
    float w1=1.0/ac_ha, w2=1.0/lb_kg;

    CString FFD;
    FFD.Format("FIELD %d: %s (%.0f ac)", CL, crop_name[1], FarmCropArea[CL]*w1);
    fprintf(fout2, "%-25s %11s     %11s     %11s     %11s\n\n", FFD, "C_lb", "N_lb", "Water_lb", "P_lb");
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Influx from lagoon", yr_Lagoon_to_field_C_T*FarmCropManureFraction[CL]*w2, yr_Lagoon_to_field_N_T*FarmCropManureFraction[CL]*w2, yr_Lagoon_to_field_H2O_T*FarmCropManureFraction[CL]*w2, yr_Lagoon_to_field_P_T*FarmCropManureFraction[CL]*w2);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Influx from compost", yr_Compost_to_field_C_T*FarmCropManureFraction[CL]*w2, yr_Compost_to_field_N_T*FarmCropManureFraction[CL]*w2, yr_Compost_to_field_H2O_T*FarmCropManureFraction[CL]*w2, yr_Compost_to_field_P_T*FarmCropManureFraction[CL]*w2);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n\n", "Influx from digester", yr_Digester_to_field_C_T*FarmCropManureFraction[CL]*w2, yr_Digester_to_field_N_T*FarmCropManureFraction[CL]*w2, yr_Digester_to_field_H2O_T*FarmCropManureFraction[CL]*w2, yr_Digester_to_field_P_T*FarmCropManureFraction[CL]*w2);

    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Influx from precipitation", 0.0, yr_rainN*_ha*w2, yr_rain*10000.0*_ha*1000.0*w2, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Influx from crop litter", (yr_addtc+yr_addrc)*_ha*w2, (yr_sln)*_ha*w2, 0.0, 0.0);//yr_addtn + +yr_addrn
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Influx from fertilizer", 0.0, yr_addfer*_ha*w2, 0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Influx from irrigation", 0.0, yr_irri_N*_ha*w2, yr_irri*10000.0*_ha*1000.0*w2, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n\n", "Influx from N fixation", 0.0, yr_soilN_from_Nfixation*_ha*w2, 0.0, 0.0);

    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Soil CO2 emission", (yr_soil_co2)*_ha*w2, 0.0,0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Soil CH4 emission", yr_ch4*_ha*w2, 0.0,0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Soil NH3 emission", 0.0, yr_soil_nh3*_ha*w2, 0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Soil N2O emission", 0.0, yr_dnn2o*_ha*w2, 0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Soil NO emission", 0.0, yr_dnno*_ha*w2, 0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Soil N2 emission", 0.0, yr_dnn2*_ha*w2, 0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Uptake by crop", 0.0, (yr_uptaken[1]+yr_uptaken[2]+yr_uptaken[3])*_ha*w2, yr_trans*10000.0*_ha*1000.0*w2, PlantP[1]*_ha*w2);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Soil N loss by leaching", 0.0, YrLeachNO3*_ha*w2, (yr_leach_water)*10000.0*_ha*1000.0*w2, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Soil runoff loss", yr_runoff_SOC*_ha*w2, yr_runoff_ION*_ha*w2, yr_runoff*10000.0*_ha*1000.0*w2, yr_runoff_P*_ha*w2);

    fprintf(fout2, "%-25s %e    %e     %e    %e\n\n", "Soil water evaporation", 0.0, 0.0, (yr_soil_ev+yr_pond_ev)*10000.0*_ha*1000.0*w2, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n\n", "Soil sequestration", DFC*w2, DFN*w2, DFW*w2, 0.0);

    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Crop N fixation", 0.0, CNF*w2, 0.0, 0.0); 
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Crop grain", YrG_grain[1]*_ha*w2, GNC*w2, 0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Crop shoot", (YrG_leaf[1]+YrG_stem[1])*_ha*w2, SNC*w2, 0.0, 0.0);
    fprintf(fout2, "%-25s %e    %e     %e    %e\n", "Crop root", YrG_root[1]*_ha*w2, RNC*w2, 0.0, 0.0);
    fprintf(fout2, "---------------------------------------------------------------------------------------------\n");
}
#endif
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

if(m_unit==0)//Metric system
{
    fprintf(fout, "  -------------------------------------------------------------------------------------\n");
    fprintf(fout, "  ANNUAL REPORT: ");
    fprintf(fout, "  Site %s     Year %d     %s\n",	site, year, date_time );
    fprintf(fout, "  -------------------------------------------------------------------------------------\n");
    fprintf(fout, "  SOIL SECTION: Unit kg C or N/ha \n");	
    fprintf(fout, "  -------------------------------------------------------------------------------------\n");
    
    fprintf(fout, "  SOM pools       -----Litter-----  -----Humads-----  -----Humus------  -----Total-----\n");
    fprintf(fout, "                    C        N        C        N        C        N        C        N   \n");
    fprintf(fout, "  Day 1         %7.2f  %7.2f  %7.2f  %7.2f %7.2f  %7.2f  %7.2f  %7.2f\n",
        IniLitter, IniLitterN, IniActHumus, IniActHumusN, whumus1, whumusn1, IniSOC, IniSON);
    fprintf(fout, "  Day 365       %7.2f  %7.2f  %7.2f  %7.2f %7.2f  %7.2f  %7.2f  %7.2f\n",
        EndLitter, EndLitterN, EndActHumus, EndActHumusN, whumus2, whumusn2, EndSOC, EndSON);
    fprintf(fout, "  -------------------------------------------------------------------------------------\n");
    fprintf(fout, "  Inorganic N pools in kg N/ha\n\n");
    fprintf(fout, "                   NO3-     NH4+    NH3(w)   Urea     NO(w)    clay-NH4   N2O+N2   Total\n");
    fprintf(fout, "  Day 1         %7.2f  %7.2f  %7.2f  %7.2f  %7.2f  %7.2f  %7.2f  %7.2f\n",
                        wno31, wnh41, wnh31, wurea1, wno1, day_clay_n1, ini_N_gases, IniSIN); 
    fprintf(fout, "  Day 365       %7.2f  %7.2f  %7.2f  %7.2f  %7.2f  %7.2f  %7.2f  %7.2f\n",
                        wno32, wnh42, wnh32, wurea2, wno2, day_clay_n2, end_N_gases, EndSIN);//	
    
    fprintf(fout, "  -------------------------------------------------------------------------------------\n");
    fprintf(fout, "  Fluxes                         C (kg C/ha/yr)              N (kg N/ha/yr)\n\n");
    fprintf(fout, "  Inputs -------------------------------------------------------------------------------\n");
    fprintf(fout, "  Manure                            %7.2f			%9.2f\n", yr_addmc, yr_addmn);
    fprintf(fout, "  Shoot litter                      %7.2f			%9.2f\n", yr_addtc, yr_sln-yr_addrn);//yr_addtn + 
    fprintf(fout, "  Root litter			    %7.2f                     %9.2f\n", yr_addrc, yr_addrn);
    fprintf(fout, "  Root exudation		    %7.2f							\n", yr_InputRoot_DOC);	
    fprintf(fout, "  Rain-N deposit                                  ");
    fprintf(fout, "               %8.2f\n", yr_rainN);
    fprintf(fout, "  Irrigation N input                              ");
    fprintf(fout, "               %8.2f\n", yr_irri_N);
    fprintf(fout, "  Fertilizer-N                                    ");
    fprintf(fout, "               %8.2f\n", yr_addfer);
    fprintf(fout, "  Soil N fixation                                 ");
    fprintf(fout, "               %8.2f\n", yr_soilN_from_Nfixation);
    fprintf(fout, "  NH3 deposition                                  ");
    fprintf(fout, "               %8.2f\n\n", yr_NH3_deposit);
    fprintf(fout, "  Outputs ------------------------------------------------------------------------------\n");
    fprintf(fout, "  Soil-CO2 emission                ");	
    fprintf(fout, " %7.2f\n", yr_soil_co2);//+yr_rootDOC_co2
    fprintf(fout, "  CH4 emission                      %7.2f\n", yr_ch4);
    fprintf(fout, "  DOM leaching                     ");
    fprintf(fout, " %7.2f          ", yr_leach_hu);//+yr_FrostBio);
    fprintf(fout, "             %7.2f\n", yr_leach_un);
    fprintf(fout, "  Crop N uptake from soil                         ");
    fprintf(fout, "               %8.2f\n", yr_uptaken[1]+yr_uptaken[2]+yr_uptaken[3]);
    fprintf(fout, "  ION runoff                                      ");
    fprintf(fout, "               %8.2f\n", yr_runoff_N);
    fprintf(fout, "  ION leaching                                    ");
    fprintf(fout, "               %8.2f\n", YrLeachNO3);
    fprintf(fout, "  NH3 volatilization                              ");
    fprintf(fout, "               %8.2f\n", yr_soil_nh3);
    fprintf(fout, "  N2O                                             ");
    fprintf(fout, "               %8.2f\n", yr_dnn2o);
    fprintf(fout, "  NO                                              ");
    fprintf(fout, "               %8.2f\n", yr_dnno);
    fprintf(fout, "  N2                                              ");
    fprintf(fout, "               %8.2f\n", yr_dnn2);
    
    fprintf(fout, "  -------------------------------------------------------------------------------------\n");
    fprintf(fout, "  Mineralization: %6.1f kg C/ha and %6.1f kg N/ha; ", yr_C_mine, yr_minern);
    fprintf(fout, "  Soil C/N ratio: %4.1f\n", CNRatio);
    fprintf(fout, "  --------------------------------------------------------------------------------------\n");
    fprintf(fout, "  Depth (cm)         kg C/kg           kg C/ha		Soil_weight_kg/ha\n");
    fprintf(fout, "    0 - 10         %8.4f             %8.2f		%6.0f\n", topc[1], layerc[1], LayerSoilWeight[1]);
    fprintf(fout, "   10 - 20         %8.4f             %8.2f		%6.0f\n", topc[2], layerc[2], LayerSoilWeight[2]);
    fprintf(fout, "   20 - 30         %8.4f             %8.2f		%6.0f\n", topc[3], layerc[3], LayerSoilWeight[3]);
    fprintf(fout, "   30 - 40         %8.4f             %8.2f		%6.0f\n", topc[4], layerc[4], LayerSoilWeight[4]);
    fprintf(fout, "   40 - 50         %8.4f             %8.2f		%6.0f\n", topc[5], layerc[5], LayerSoilWeight[5]);
  
    fprintf(fout, "  -------------------------------------------------------------------------------------\n");
    fprintf(fout, "  CROP SECTION %d: Unit kg C or N/ha \n", CL);	
    fprintf(fout, "  -------------------------------------------------------------------------------------\n");

    fprintf(fout, "  Cropping season	");
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12d ", i);
    fprintf(fout, "\n");

    fprintf(fout, "  Crop name			");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%-12s ", crop_name[i]);
    }
    fprintf(fout, "\n");			
        
    fprintf(fout, "  Planting date		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12d ", jdseed[i]);
    }
    fprintf(fout, "\n");

    fprintf(fout, "  Growing days		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12d ", grow_days[i]);
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  Growing season TDD	");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.0f ", TDD[i]);
    }
    fprintf(fout, "\n");

    fprintf(fout, "  Water demand (mm)	");	
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", crop_PT[i]*1000.0);
    }
    fprintf(fout, "\n");

    fprintf(fout, "  Water uptake (mm)	");	
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", crop_trans[i]*1000.0);
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  Water stress		");	
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", crop_water_stress[i]/G_days[i]);
    }
    fprintf(fout, "\n");
                            
    fprintf(fout, "  Crop N demand		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", maxmn[i]);// yr_need_N[i], demandn[i]
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  Crop N from soil	");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", yr_uptaken[i]);
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  Crop N from air NH3	");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", crop_NH3_absorp[i]);
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  Crop N fixation	");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", yr_fixn[i]);
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  Nitrogen stress	");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", crop_N_stress[i]/G_days[i]);
    }
    fprintf(fout, "\n\n");
    
    fprintf(fout, "  Annual crop biomass production:\n");
    fprintf(fout, "  Crop N (kg N/ha)	");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", yr_plantn[i]);
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  Crop C (kg C/ha)	");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", Yr_PlantBiomass[i]);
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  -- Grain C		");
    {
    for(int i=1;i<=CropNumbers;i++) 
        fprintf(fout, "%12.2f ", YrG_grain[i]);
    }
    fprintf(fout, "\n");
                                                
    fprintf(fout, "  -- Leaf C		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", YrG_leaf[i]);
    }
    fprintf(fout, "\n");

    fprintf(fout, "  -- stem C		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", YrG_stem[i]);
    }
    fprintf(fout, "\n");
                        
    fprintf(fout, "  -- Root C		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", YrG_root[i]);
    }
    fprintf(fout, "\n\n");
    
    fprintf(fout, "  Photosynthesis (kg C/ha)       %7.1f\n", -yr_Psn);
    fprintf(fout, "  Shoot respiration              %7.1f\n", yr_shoot_CO2);
    fprintf(fout, "  Root respiration               %7.1f\n", yr_root_co2);	
    fprintf(fout, "  Crop NPP                       %7.1f\n", yr_CropGPP);
    fprintf(fout, "  NEE                            %7.1f\n", yr_NEE);
    fprintf(fout, "  Stubble (kg C/ha)		%8.1f\n", stub1+stub2+stub3);

    fprintf(fout, "  Fruit cut (kg C/ha)		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%8.0f", yr_CutGrain[i]);
    }
    fprintf(fout, "\n");

    fprintf(fout, "  Leaf cut (kg C/ha)		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%8.0f", yr_CutLeaf[i]);
    }
    fprintf(fout, "\n");

    fprintf(fout, "  Stem cut (kg C/ha)		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%8.0f", yr_CutStem[i]);
    }
    fprintf(fout, "\n");

    fprintf(fout, "  Root cut (kg C/ha)		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%8.0f", yr_CutRoot[i]);
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  Livestock feed demand (kg C/ha) %8.0f\n", yr_GrazeDemandC);
    fprintf(fout, "  Grazed biomass (kg C/ha)	  %8.0f\n", yr_GrazeGrain[1]+yr_GrazeShoot[1]+yr_GrazeStub);
    
    fprintf(fout, "  --------------------------------------------------------------------------------------\n");
    fprintf(fout, "  WATER SECTION: Unit mm water/year\n");	
    fprintf(fout, "  -------------------------------------------------------------------------------------\n");

    yr_plant_demand = (yr_pond_ev+yr_trans+yr_soil_ev)*1000.0;
    yr_TotalEvap = (yr_pond_ev + yr_soil_ev)*1000.0;
    yr_PrecWater = yr_rain*10.0;

    yr_dSoilWater = (yr_rain+yr_irri) - (yr_trans+yr_soil_ev+yr_runoff+yr_leach_water); //m
    
    fprintf(fout, "  Precipitation			%.2f\n", yr_rain*1000.0); 
    fprintf(fout, "  Irrigation			%.2f\n", yr_irri*1000.0);
    fprintf(fout, "  Manure water			%.2f\n", yr_manure_water*1000.0);	
    fprintf(fout, "  PET				%.2f\n", yr_PET*1000.0);
    fprintf(fout, "  Transpiration			%.2f\n", yr_trans*1000.0);
    fprintf(fout, "  Soil evaporation		%.2f\n", (yr_soil_ev+yr_pond_ev)*1000.0);
    fprintf(fout, "  Run off			%.2f\n", yr_runoff*1000.0);
    fprintf(fout, "  Leaching			%.2f\n", yr_leach_water*1000.0);
    //fprintf(fout, "  Change in soil water		%.2f\n", yr_dSoilWater*1000.0);
    fprintf(fout, "  Soil profile water:	Initial %.2f;	End: %.2f\n", IniProfWater*1000.0, day_SoilWater*1000.0);
    fprintf(fout, "  Deep water pool   :	Initial %.2f;	End: %.2f\n", IniWaterPool*1000.0, WaterPool*1000.0);
    fprintf(fout, "  Mean wind speed (m/s) 	%.2f\n", yr_WindSpeed/365.0);

    fprintf(fout, "  -------------------------------------------------------------------------------------\n");

    if(UseION==1) 
    {
        fprintf(fout, "  Soil inorganic N adjustment due to artificially altering ION profile\n"); 
        fprintf(fout, "  Adjusted NO3 (kg N/ha)			%.2f\n", AdjustNO3);
        fprintf(fout, "  Adjusted NH4 (kg N/ha)			%.2f\n", AdjustNH4);
    }
}
else//English system
{
    float w3=1.0/lboac_kgoha;

    fprintf(fout, "  -------------------------------------------------------------------------------------\n");
    fprintf(fout, "  ANNUAL REPORT: ");
    fprintf(fout, "  Site %s     Year %d     %s\n",	site, year, date_time );
    fprintf(fout, "  -------------------------------------------------------------------------------------\n");
    fprintf(fout, "  SOIL SECTION: Unit lb C or N/ac \n");	
    fprintf(fout, "  -------------------------------------------------------------------------------------\n");
    
    fprintf(fout, "  SOM pools       -----Litter-----  -----Humads-----  -----Humus------  -----Total-----\n");
    fprintf(fout, "                    C        N        C        N        C        N        C        N   \n");
    fprintf(fout, "  Day 1         %7.2f  %7.2f  %7.2f  %7.2f %7.2f  %7.2f  %7.2f  %7.2f\n",
        IniLitter*w3, IniLitterN*w3, IniActHumus*w3, IniActHumusN*w3, whumus1*w3, whumusn1*w3, IniSOC*w3, IniSON*w3);
    fprintf(fout, "  Day 365       %7.2f  %7.2f  %7.2f  %7.2f %7.2f  %7.2f  %7.2f  %7.2f\n",
        EndLitter*w3, EndLitterN*w3, EndActHumus*w3, EndActHumusN*w3, whumus2*w3, whumusn2*w3, EndSOC*w3, EndSON*w3);
    fprintf(fout, "  -------------------------------------------------------------------------------------\n");
    fprintf(fout, "  Inorganic N pools in kg N/ha\n\n");
    fprintf(fout, "                   NO3-     NH4+    NH3(w)   Urea     NO(w)    clay-NH4   N-gases   Total\n");
    fprintf(fout, "  Day 1         %7.2f  %7.2f  %7.2f  %7.2f  %7.2f  %7.2f  %7.2f  %7.2f\n",
                        wno31*w3, wnh41*w3, wnh31*w3, wurea1*w3, wno1*w3, day_clay_n1*w3, ini_N_gases*w3, IniSIN*w3); 
    fprintf(fout, "  Day 365       %7.2f  %7.2f  %7.2f  %7.2f  %7.2f  %7.2f  %7.2f  %7.2f\n",
                        wno32*w3, wnh42*w3, wnh32*w3, wurea2*w3, wno2*w3, day_clay_n2*w3, end_N_gases*w3, EndSIN*w3);//	
    
    fprintf(fout, "  -------------------------------------------------------------------------------------\n");
    fprintf(fout, "  Fluxes                         C (kg C/ha/yr)              N (kg N/ha/yr)\n\n");
    fprintf(fout, "  Inputs -------------------------------------------------------------------------------\n");
    fprintf(fout, "  Manure                           ");
    fprintf(fout, " %7.2f  ", yr_addmc*w3);
    fprintf(fout, "                   %9.2f\n", yr_addmn*w3);
    fprintf(fout, "  Shoot litter                      %7.2f			%9.2f\n", yr_addtc*w3, yr_sln-yr_addrn*w3);//yr_addtn + 
    fprintf(fout, "  Root litter			    %7.2f                     %9.2f\n", yr_addrc*w3, yr_addrn*w3);
    fprintf(fout, "  Rain-N deposit                                  ");
    fprintf(fout, "               %8.2f\n", yr_rainN*w3);
    fprintf(fout, "  Irrigation N input                              ");
    fprintf(fout, "               %8.2f\n", yr_irri_N*w3);
    fprintf(fout, "  Fertilizer-N                                    ");
    fprintf(fout, "               %8.2f\n", yr_addfer*w3);
    fprintf(fout, "  Soil N fixation                                 ");
    fprintf(fout, "               %8.2f\n", yr_soilN_from_Nfixation*w3);
    fprintf(fout, "  NH3 deposition                                  ");
    fprintf(fout, "               %8.2f\n\n", yr_NH3_deposit*w3);
    fprintf(fout, "  Outputs ------------------------------------------------------------------------------\n");
    fprintf(fout, "  Soil-CO2 emission                ");	
    fprintf(fout, " %7.2f\n", yr_soil_co2*w3);//+yr_rootDOC_co2
    fprintf(fout, "  CH4 emission                      %7.2f\n", yr_ch4*w3);
    fprintf(fout, "  DOC leached                      ");
    fprintf(fout, " %7.2f          ", yr_leach_hu*w3);//+yr_FrostBio);
    fprintf(fout, "             %7.2f\n", yr_leach_un*w3);
    fprintf(fout, "  Crop N uptake from soil                         ");
    fprintf(fout, "               %8.2f\n", yr_uptaken[1]+yr_uptaken[2]+yr_uptaken[3]*w3);
    fprintf(fout, "  ION runoff                                      ");
    fprintf(fout, "               %8.2f\n", yr_runoff_N*w3);
    fprintf(fout, "  ION leaching                                    ");
    fprintf(fout, "               %8.2f\n", YrLeachNO3*w3);
    fprintf(fout, "  NH3 volatilization                              ");
    fprintf(fout, "               %8.2f\n", yr_soil_nh3*w3);
    fprintf(fout, "  N2O                                             ");
    fprintf(fout, "               %8.2f\n", yr_dnn2o*w3);
    fprintf(fout, "  NO                                              ");
    fprintf(fout, "               %8.2f\n", yr_dnno*w3);
    fprintf(fout, "  N2                                              ");
    fprintf(fout, "               %8.2f\n", yr_dnn2*w3);
    
    fprintf(fout, "  -------------------------------------------------------------------------------------\n");
    fprintf(fout, "  Mineralization: %6.1f lb C/ac and %6.1f lb N/ac; ", yr_C_mine, yr_minern);
    fprintf(fout, "  Soil C/N ratio: %4.1f\n", CNRatio);
    fprintf(fout, "  --------------------------------------------------------------------------------------\n");
    fprintf(fout, "  Depth (cm)         lb C/ac           lb C/ac	\n");
    fprintf(fout, "    0 - 10         %8.4f             %6.2f\n", topc[1]*w3, layerc[1]*w3);
    fprintf(fout, "   10 - 20         %8.4f             %6.2f\n", topc[2]*w3, layerc[2]*w3);
    fprintf(fout, "   20 - 30         %8.4f             %6.2f\n", topc[3]*w3, layerc[3]*w3);
    fprintf(fout, "   30 - 40         %8.4f             %6.2f\n", topc[4]*w3, layerc[4]*w3);
    fprintf(fout, "   40 - 50         %8.4f             %6.2f\n", topc[5]*w3, layerc[5]*w3);
  
    fprintf(fout, "  -------------------------------------------------------------------------------------\n");
    fprintf(fout, "  CROP SECTION %d: Unit lb C or N/ac \n", CL);	
    fprintf(fout, "  -------------------------------------------------------------------------------------\n");

    fprintf(fout, "  Cropping season	");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12d ", i);
    }
    fprintf(fout, "\n");

    fprintf(fout, "  Crop name			");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%-12s ", crop_name[i]);
    }
    fprintf(fout, "\n");			
        
    fprintf(fout, "  Planting date		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12d ", jdseed[i]);
    }
    fprintf(fout, "\n");

    fprintf(fout, "  Growing days		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12d ", grow_days[i]);
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  Growing season TDD	");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.0f ", TDD[i]);
    }
    fprintf(fout, "\n");

    fprintf(fout, "  Water demand (mm)	");	
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", crop_PT[i]*1000.0);
    }
    fprintf(fout, "\n");

    fprintf(fout, "  Water stress		");	
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", crop_water_stress[i]/G_days[i]);
    }
    fprintf(fout, "\n");
                            
    fprintf(fout, "  Crop N demand		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", maxmn[i]*w3);// yr_need_N[i], demandn[i]
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  Crop N from soil	");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", yr_uptaken[i]*w3);
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  Crop N from air NH3	");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", crop_NH3_absorp[i]*w3);
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  Crop N fixation	");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", yr_fixn[i]*w3);
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  Nitrogen stress	");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", crop_N_stress[i]/G_days[i]);
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  Crop biomass:\n");
    fprintf(fout, "  Crop N (kg N/ha)	");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", yr_plantn[i]*w3);
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  Crop C (kg C/ha)	");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", Yr_PlantBiomass[i]*w3);
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  -- Grain C		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", YrG_grain[i]*w3);
    }
    fprintf(fout, "\n");
                                                
    fprintf(fout, "  -- Leaf C	");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", (YrG_leaf[i])*w3);
    }
    fprintf(fout, "\n");	
    
    fprintf(fout, "  -- stem C	");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", (YrG_stem[i])*w3);
    }
    fprintf(fout, "\n");
                        
    fprintf(fout, "  -- Root C		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%12.2f ", YrG_root[i]*w3);
    }
    fprintf(fout, "\n\n");
    
    fprintf(fout, "  Photosynthesis (lb C/ac)       %7.2f\n", -yr_Psn*w3);
    fprintf(fout, "  Shoot respiration              %7.2f\n", yr_shoot_CO2*w3);
    fprintf(fout, "  Root respiration               %7.2f\n", yr_root_co2*w3);	
    fprintf(fout, "  Crop NPP                       %7.2f\n", yr_CropGPP*w3);
    fprintf(fout, "  NEE                            %7.2f\n", yr_NEE*w3);
    fprintf(fout, "  Stubble (lb C/ac)		%8.0f\n", (stub1+stub2+stub3)*w3);

    fprintf(fout, "  Fruit cut (lb C/ac)		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%8.0f", yr_CutGrain[i]*w3);
    }
    fprintf(fout, "\n");

    fprintf(fout, "  Leaf cut (lb C/ac)		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%8.0f", yr_CutLeaf[i]*w3);
    }
    fprintf(fout, "\n");

    fprintf(fout, "  Stem cut (lb C/ac)		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%8.0f", yr_CutStem[i]*w3);
    }
    fprintf(fout, "\n");

    fprintf(fout, "  Root cut (lb C/ac)		");
    {
    for(int i=1;i<=CropNumbers;i++) fprintf(fout, "%8.0f", yr_CutRoot[i]*w3);
    }
    fprintf(fout, "\n");
    
    fprintf(fout, "  Grazed biomass (lb C/ac)	%8.0f\n", (yr_GrazeGrain[1]+yr_GrazeShoot[1]+yr_GrazeStub)*w3);
    fprintf(fout, "  Livestock demand for grass biomass (lb C/ac)	%8.0f\n", yr_GrazeDemandC*w3);

    fprintf(fout, "  --------------------------------------------------------------------------------------\n");
    fprintf(fout, "  WATER SECTION: Unit mm water/year\n");	
    fprintf(fout, "  -------------------------------------------------------------------------------------\n");

    yr_plant_demand = (yr_pond_ev+yr_trans+yr_soil_ev)*1000.0;
    yr_TotalEvap = (yr_pond_ev + yr_soil_ev)*1000.0;
    yr_PrecWater = yr_rain*10.0;

    yr_dSoilWater = (yr_rain+yr_irri) - (yr_trans+yr_soil_ev+yr_runoff+yr_leach_water); //m
    
    fprintf(fout, "  Precipitation			%.2f\n", yr_rain*1000.0); 
    fprintf(fout, "  Irrigation			%.2f\n", yr_irri*1000.0);
    fprintf(fout, "  PET				%.2f\n", yr_PET*1000.0);
    fprintf(fout, "  Transpiration			%.2f\n", yr_trans*1000.0);
    fprintf(fout, "  Soil evaporation		%.2f\n", (yr_soil_ev+yr_pond_ev)*1000.0);
    fprintf(fout, "  Run off			%.2f\n", yr_runoff*1000.0);
    fprintf(fout, "  Leaching			%.2f\n", yr_leach_water*1000.0);
//	fprintf(fout, "  Change in soil water		%.2f\n", yr_dSoilWater*1000.0);
    fprintf(fout, "  Deep water pool    		%.2f\n", WaterPool*1000.0);
    fprintf(fout, "  Mean wind speed (m/s) 	%.2f\n", yr_WindSpeed/365.0);

    fprintf(fout, "  -------------------------------------------------------------------------------------\n");

    if(UseION==1) 
    {
        fprintf(fout, "  Soil inorganic N adjustment due to artificially altering ION profile\n"); 
        fprintf(fout, "  Adjusted NO3 (lb N/ac)			%.2f\n", AdjustNO3*w3);
        fprintf(fout, "  Adjusted NH4 (lb N/ac)			%.2f\n", AdjustNH4*w3);
    }
}
}	/////////////////////////////////////////////////////////////////////////////////////////////////////

//#endif		
        
    //fclose( fout );

    int IDID=0;

    sprintf(YR, "%s\\Inter\\Summary-%d", OUTPUT, year);
    
    fout = fopen( YR, "w" );
    if ( fout == NULL ) note(1, YR);
        
    fprintf(fout, "%d	Site_ID:	\n", IDID);
    fprintf(fout, "%s	Site_name:	\n", site);
    fprintf(fout, "%.2f	Mean annual temperature (degree C):	\n", yr_avet);
    
    fprintf(fout, "%.2f	Soil_bulk_density (g/cm3):	\n", sd[1]);
    fprintf(fout, "%.2f	Initial_SOC (kg C/kg soil):	\n", Tttoo[1]);
    fprintf(fout, "%.2f	Clay_fraction:	\n", clay[1]);
    fprintf(fout, "%.2f	Soil_pH:	\n", soil_ph[1]);
    fprintf(fout, "%s	Crop_type:	\n", crop_name[1]);
    fprintf(fout, "%s	Crop_type:	\n", crop_name[2]);
    fprintf(fout, "%s	Crop_type:	\n", crop_name[3]);
    //fprintf(fout, "%.2f	Planted_area (ha):			\n", ha);
    
    fprintf(fout, "%.2f	Irrigation_index:	\n", irri_index);
    fprintf(fout, "%.2f	Manure_amended (kg C/ha/yr):	\n\n", yr_addmc);

    fprintf(fout, "%.2f	N_deposit (kg N/ha/yr):	\n", yr_rainN);
    fprintf(fout, "%.2f	Fertilizer_N (kg N/ha/yr):	\n", yr_addfer);
    fprintf(fout, "%.2f	Manure_N (kg N/ha/yr):	\n", yr_addmn);
    fprintf(fout, "%.2f	Irrigation_N (kg N/ha/yr):	\n", yr_irri_N);
    fprintf(fout, "%.2f	Root_N (kg N/ha/yr):	\n", 0.0);
    fprintf(fout, "%.2f	Crop_residue_N (kg N/ha/yr):	\n", yr_sln);
    fprintf(fout, "%.2f	Soil_fixed_N (kg N/ha/yr):	\n", yr_soilN_from_Nfixation);
    fprintf(fout, "%.2f	N_fixation1 (kg N/ha/yr):	\n", yr_fixn[1]);
    fprintf(fout, "%.2f	N_fixation2 (kg N/ha/yr):	\n", yr_fixn[2]);
    fprintf(fout, "%.2f	N_fixation3 (kg N/ha/yr):	\n", yr_fixn[3]);

    fprintf(fout, "%.2f	N_uptaken_by_crop1 (kg N/ha/yr):	\n", yr_uptaken[1]);//R_plantn[1]);
    fprintf(fout, "%.2f	N_uptaken_by_crop2 (kg N/ha/yr):	\n", yr_uptaken[2]);//R_plantn[2]);
    fprintf(fout, "%.2f	N_uptaken_by_crop3 (kg N/ha/yr):	\n", yr_uptaken[3]);//R_plantn[3]);

    fprintf(fout, "%.2f	Runoff N (kg N/ha/yr):	\n", yr_runoff_N);	
    fprintf(fout, "%.2f	Nitrate_leached (kg N/ha/yr):	\n", YrLeachNO3);
    fprintf(fout, "%.2f	NH3_volatilization (kg N/ha/yr):	\n", yr_soil_nh3);
    fprintf(fout, "%.2f	N2O_emission (kg N/ha/yr):	\n", yr_dnn2o);
    fprintf(fout, "%.2f	NO_emission (kg N/ha/yr):	\n", yr_dnno);
    fprintf(fout, "%.2f	N2_emission (kg N/ha/yr):	\n", yr_dnn2);
    //fprintf(fout, "%.2f	Weed_N (kg N/ha/yr):	\n\n", yr_weedn);
    
    fprintf(fout, "%.2f	C_input_from_crop_root (kg C/ha/yr):	\n", yr_addrc);
    fprintf(fout, "%.2f	C_input_from_crop_shoot (kg C/ha/yr):	\n", yr_addtc);
    fprintf(fout, "%.2f	C_input_from_roots (kg C/ha/yr):	\n", yr_InputRoot_DOC);
    fprintf(fout, "%.2f	C_input_from_manure (kg C/ha/yr):	\n", yr_addmc);
    fprintf(fout, "%.2f	Soil_heterotrophic_CO2 (kg C/ha/yr):	\n", yr_soil_co2);
    fprintf(fout, "%.2f	CH4_emission (kg C/ha/yr):	\n", yr_ch4);
    fprintf(fout, "%.2f	SOC_leaching (kg C/ha/yr):	\n\n",yr_leach_hu);//+yr_FrostBio);
    fprintf(fout, "%.2f	Change_in_SOC (kg C/ha/yr):	\n\n", EndSOC-IniSOC);

    fprintf(fout, "%.2f	Annual precipitation (mm):	\n", yr_rain*1000.0);	
    fprintf(fout, "%.0f	Irrigation			\n", yr_irri*1000.0);
    fprintf(fout, "%.0f	Transpiration1			\n", crop_trans[1]*1000.0);
    fprintf(fout, "%.0f	Transpiration2			\n", crop_trans[2]*1000.0);
    fprintf(fout, "%.0f	Transpiration3			\n", crop_trans[3]*1000.0);
    fprintf(fout, "%.0f	Soil evaporation		\n", yr_soil_ev*1000.0);
    fprintf(fout, "%.0f	Surface water evaporation	\n", yr_pond_ev*1000.0);
    fprintf(fout, "%.0f	Run off			\n", yr_runoff*1000.0);
    fprintf(fout, "%.0f	Leaching			\n", yr_leach_water*1000.0);
    fprintf(fout, "%.0f	Groundwater suply		\n\n", yr_GWsuply*1000.0);
    
    fprintf(fout, "%.2f	Crop_biomass1 (kg C/ha):	\n", Yr_PlantBiomass[1]);
    fprintf(fout, "%.2f	Crop_biomass2 (kg C/ha):	\n", Yr_PlantBiomass[2]);
    fprintf(fout, "%.2f	Crop_biomass3 (kg C/ha):	\n", Yr_PlantBiomass[3]);
    
    for(int ia=1; ia<=3; ia++)
        fprintf(fout, "%.2f	Crop_grain1 (kg C/ha):	\n", YrG_grain[ia]);//+ yr_CutGrain[i]+yr_GrazeGrain[i]);
    
    for(int ib=1; ib<=3; ib++)	
        fprintf(fout, "%.2f	Crop_shoot1 (kg C/ha):	\n", YrG_leaf[ib]+YrG_stem[ib]);// + yr_CutLeaf[i]+yr_CutStem[i]+yr_GrazeShoot[i]);
    
    for(int ic=1; ic<=3; ic++)
        fprintf(fout, "%.2f	Crop_root1 (kg C/ha):	\n", YrG_root[ic]);// + yr_CutRoot[i]+yr_GrazeRoot[i]);

    for(int id=1; id<=3; id++)
        fprintf(fout, "%.2f	Crop_N1 (kg N/ha):	\n", yr_plantn[id]);
    
    
/*	float OPY;
    if(grain[1]<=0.05)//crop[1]==4||crop[1]==5||crop[1]==10||crop[1]==11||crop[1]==12||crop[1]==22||crop[1]==32)
    {
        OPY=OptGrainC[1]+OptGrainC[2]+OptGrainC[3]+OptShootC[1]+OptShootC[2]+OptShootC[3];
    }
    else
    {
        OPY=OptGrainC[1]+OptGrainC[2]+OptGrainC[3];
    }

    OPY = (total_Ndemand[1]+yr_fixn[1]) * plantcn[1] * grain[1];
*/
    for(int ie=1; ie<=3; ie++)
        fprintf(fout, "%.2f	Optimum_grain1 (kg C/ha):	\n", OptGrainC[ie]);
    
    for(int ifw=1; ifw<=3; ifw++)
        fprintf(fout, "%.2f	Optimum_straw1 (kg C/ha):	\n", OptLeafC[ifw]+OptStemC[ifw]);
    
    for(int ig=1; ig<=3; ig++)
        fprintf(fout, "%.2f	Optimum_root1 (kg C/ha):	\n", OptRootC[ig]);
    
    for(int ih=1; ih<=3; ih++)
        fprintf(fout, "%.2f	Crop_water_demand1 (mm):	\n", crop_PT[ih]*1000.0);//CropWaterDemand[1]*1000.0);
    
    for(int ii=1; ii<=3; ii++)
        fprintf(fout, "%.2f	Crop_N_demand1 (kg N/ha):	\n", maxmn[ii]);//maxmn[1]);

    for(int ij=1; ij<=3; ij++)
        fprintf(fout, "%.2f	Crop_T_demand1 (degree):	\n", crop_T_demand[ij]);

    for(int ik=1; ik<=3; ik++)
        fprintf(fout, "%.2f	TDD (degree):	\n", TDD[ik]);

    for(int il=1; il<=3; il++)
        fprintf(fout, "%d %d	Crop1 plant/harvest dates:	\n", jdseed[il], HarvestDay[il]);

    fclose( fout );

    multi_yr_record(MODE, Batch, EndSOC, Pass, PassC1);//+

    return( 0 );
}

#ifdef DEGUI
#else	
void class_model::write_MonteCarlo(int sample, int county_ID1, char* cropping_system)
{
    float dSOC;
    
    CString para[6]={"None","None","None","None","None","None"};
    CString MCname[48]={"",
        "Temp ", "Prec ", "CO2  ", "NH3  ", "N_dep", 
        "Clay ", "Dens ", "SKS  ", "Fldcp", "Wiltp", 
        "Poro ", "SOC  ", "IniLi", "IniHu", "NO3  ", 
        "NH4  ", "pH   ", "Yield", "FeedRate ", "CP ", 
        "Phosphorus ", "P_C/N", "N_fix", "R_H2O", "LAI  ", 
        "High ", "TDD  ", "Resid", "T_day", "T_dep", 
        "F_day", "F_dep", "F_rat", "M_day", "M_C/N", 
        "M_rat", "Irri ", "     ", "G_num", "G_day", 
        "G_hrs", "     ", "G_cut", "Top_s", "dSOC ",
        "     ", "     "};

    dSOC = EndSOC - IniSOC;

    //////////////////////////////////////////
    float MC_grainC=0.0, MC_shootC=0.0, MC_rootC=0.0, MC_CutPlantC=0.0, MC_GrazePlantC=0.0;
    for(int i=1;i<=CropNumbers;i++) 
    {
        MC_grainC += (YrG_grain[i]);//+yr_CutGrain[i]+yr_GrazeGrain[i]); 
        MC_shootC += (YrG_leaf[i]+YrG_stem[i]);//+yr_CutLeaf[i]+yr_CutStem[i]+yr_GrazeShoot[i]);
        MC_rootC += (YrG_root[i]);//+yr_CutRoot[i]+yr_GrazeRoot[i]);
        MC_CutPlantC += (yr_CutGrain[i]+yr_CutLeaf[i]+yr_CutStem[i]+yr_CutRoot[i]);
        MC_GrazePlantC += (yr_GrazeGrain[i]+yr_GrazeShoot[i]+yr_GrazeRoot[i]);
    }
    ////////////////////////////////////////////////////////////

    CString MCC;
    FILE *mc;

    if(Option>100)
    {
        int item[10], k, i;
        float value[10]={0.0,0.0};
        CString SMCabc;
        FILE *smc;
        SMCabc.Format("%s\\Inter\\SiteMCinput", OUTPUT);
        smc=fopen(SMCabc, "r");
        if(smc==NULL) note(0, SMCabc);
        
        fscanf(smc, "%d", &k);
        for(int im=1; im<=k; im++)
        {
            fscanf(smc, "%d %f", &item[im], &value[im]);
            para[im] = MCname[item[im]];
        }
        fclose(smc);

        MCC.Format("%s\\Record\\MonteCarlo\\Site_%s.%s",  OUTPUT,site, CSV);
        if(year==1) //sample==1&&
        {
            mc=fopen(MCC, "w");
            
            fprintf(mc, "Results of Monte Carlo test\n");
            fprintf(mc, ",,Output parameters,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,Variation of input parameters\n");

            fprintf(mc, "%-5s,  %-5s, ", "No.","Year");
    
#ifdef MANURE
            fprintf(mc, "%-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, ",
                         "Intake-C", "Intake-N", "Barn-CO2","Barn-CH4","Barn-N2O","Barn-NH3", "Compost-CO2","Compost-CH4","Compost-N2O","Compost-NH3","Lagoon-CO2","Lagoon-CH4","Lagoon-N2O","Lagoon-NH3","Digester-CO2","Digester-CH4","Digester-N2O","Digester-NH3");
#endif

            fprintf(mc, "%-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s\n",
                         "Grain-C","Shoot-C","Root-C","CutBiomassC","GrazedBiomassC","dSOC","Field-CH4","Field-N2O","Field-NO","Field-N2","Field-NH3","Field-N-leach","Field-N-runoff","Field_runoff",para[1],para[2],para[3],para[4],para[5]);
            
            fprintf(mc, "%-5s,  %-5s, ", "No.","Year");			

#ifdef MANURE
            fprintf(mc, "%-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, ",
                        "kgC","kgN","kgC","kgC","kgN","kgN","kgC","kgC","kgN","kgN","kgC","kgC","kgN","kgN","","","","");
#endif			
            fprintf(mc, "%-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s\n",
                        "kgC/ha","kgC/ha","kgC/ha","kgC/ha","kgC/ha","kgC/ha", "kgC/ha","kgN/ha","kgN/ha","kgN/ha","kgN/ha","kgN/ha","kgN/ha","mm","","","","","");
        }
        else 
        {
            mc=fopen(MCC, "a");
        }

        fprintf(mc, "%5d,  %5d, ", Option-100, year);

#ifdef MANURE
        fprintf(mc, "%10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, ",
                 MData.yr_intake_C, MData.yr_intake_N, MData.yr_house_co2+MData.yr_enteric_co2, MData.yr_enteric_ch4+MData.yr_house_ch4,MData.yr_enteric_n2o+MData.yr_house_n2o, MData.yr_house_nh3,
                 MData.yr_compost_co2, MData.yr_compost_ch4, MData.yr_compost_n2o, MData.yr_compost_nh3, 
                 MData.yr_lagoon_co2, MData.yr_lagoon_ch4, MData.yr_lagoon_n2o, MData.yr_lagoon_nh3, 
                 MData.yr_digester_co2,MData.yr_digester_ch4,MData.yr_digester_n2o,MData.yr_digester_nh3);

#endif
        fprintf(mc, "%10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f\n",
                 MC_grainC,MC_shootC,MC_rootC,MC_CutPlantC,MC_GrazePlantC, dSOC, yr_ch4, yr_dnn2o, yr_dnno, yr_dnn2, yr_soil_nh3, yr_leach_NO3+yr_leach_urea, yr_runoff_N, yr_runoff*1000.0, value[1],value[2],value[3],value[4],value[5]);
    }
    else
    {
        float SOC, Clay, pH, BD;
        int RN1, RN2, RN3, RN4;
        CString note;
            FILE *mcc;
            note.Format("%s\\Inter\\Combination_%d", OUTPUT, sample);
            mcc=fopen(note, "r");
            if ( mcc == NULL )
            {
                    CString mess;
                    mess.Format("Can not open %s.", note);
                    AfxMessageBox(mess);
                    exit(0);
            }
            fscanf(mcc, "%f %f %f %f", &SOC, &Clay, &pH, &BD);
            fscanf(mcc, "%d %d %d %d", &RN1, &RN2, &RN3, &RN4);
            fclose(mcc);

        MCC.Format("%s\\Record\\MonteCarlo\\MonteCarlo.txt", OUTPUT);
        if(sample==1) 
        {
            mc=fopen(MCC, "w");
            fprintf(mc, "Result of Monte Carlo test for crop %s in grid %d\n", cropping_system, county_ID1);
            fprintf(mc, "No.       dSOC        CH4         N2O         NO         N2         NH3        N_leaching          Combination of soil parameters\n");
            fprintf(mc, "          kg C/ha     kg C/ha     kg N/ha     kg N/ha    kg N/ha    kg N/ha    kg N/ha/yr          SOC       Clay      pH        Density\n");
        }
        else 
        {
            mc=fopen(MCC, "a");
        }

        
        fprintf(mc, "%6d,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,  %10.2f,            %2d,%6.3f,%2d,%6.2f,%2d,%6.2f,%2d,%6.2f\n",
                         sample, dSOC, yr_ch4, yr_dnn2o, yr_dnno, yr_dnn2, yr_soil_nh3, yr_leach_NO3+yr_leach_urea, RN1, SOC, RN2, Clay, RN3, pH, RN4, BD);
    }
    fclose(mc);
    
}

void class_model::write_BatchSummary(int sample, char* site, char* PassC1)
{
    float dSOC;
    FILE  *fp;
    char  IE[120];
    
    CString para[6]={"None","None","None","None","None","None"};
    CString MCname[48]={"",
        "Temp ", "Prec ", "CO2  ", "NH3  ", "N_dep", 
        "Clay ", "Dens ", "SKS  ", "Fldcp", "Wiltp", 
        "Poro ", "SOC  ", "IniLi", "IniHu", "NO3  ", 
        "NH4  ", "pH   ", "Yield", "FeedRate ", "CP ", 
        "Phosphorus ", "P_C/N", "N_fix", "R_H2O", "LAI  ", 
        "High ", "TDD  ", "Resid", "T_day", "T_dep", 
        "F_day", "F_dep", "F_rat", "M_day", "M_C/N", 
        "M_rat", "Irri ", "     ", "G_num", "G_day", 
        "G_hrs", "     ", "G_cut", "Top_s", "dSOC ",
        "     ", "     "};


    dSOC = EndSOC - IniSOC;

    //////////////////////////////////////////
    float MC_grainC=0.0, MC_shootC=0.0, MC_rootC=0.0, MC_CutPlantC=0.0, MC_GrazePlantC=0.0;
    for(int i=1;i<=CropNumbers;i++) 
    {
        MC_grainC += (YrG_grain[i]);//+yr_CutGrain[i]+yr_GrazeGrain[i]); 
        MC_shootC += (YrG_leaf[i]+YrG_stem[i]);//+yr_CutLeaf[i]+yr_CutStem[i]+yr_GrazeShoot[i]);
        MC_rootC += (YrG_root[i]);//+yr_CutRoot[i]+yr_GrazeRoot[i]);
        MC_CutPlantC += (yr_CutGrain[i]+yr_CutLeaf[i]+yr_CutStem[i]+yr_CutRoot[i]);
        MC_GrazePlantC += (yr_GrazeGrain[i]+yr_GrazeShoot[i]+yr_GrazeRoot[i]);
    }
    ////////////////////////////////////////////////////////////

    CString MCC;
    FILE *mc;

    //if(Option>100)
    {
        int item[10], k, i;
        float value[10]={0.0,0.0};
        
        /*CString SMCabc;
        FILE *smc;
        SMCabc.Format("%s\\Inter\\SiteMCinput", OUTPUT);
        smc=fopen(SMCabc, "r");
        if(smc==NULL) note(0, SMCabc);
        
        fscanf(smc, "%d", &k);
        for(int i=1; i<=k; i++)
        {
            fscanf(smc, "%d %f", &item[i], &value[i]);
            para[i] = MCname[item[i]];
        }
        fclose(smc);*/

        MCC.Format("%s\\BatchSummary.%s",  PassC1, CSV);
        if(year==1) //sample==1&&
        {
            mc=fopen(MCC, "w");
            //fprintf(mc, "Results of Monte Carlo test for crop %s in grid %s\n", cropping_system, county_ID1);
            fprintf(mc, "Results of a batch run\n");
            
            fprintf(mc, "%-5s,  %-5s, ", "File","Year");
    
#ifdef MANURE
            fprintf(mc, "%-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, ",
                         "Intake-C", "Intake-N", "Barn-CO2","Barn-CH4","Barn-N2O","Barn-NH3", "Compost-CO2","Compost-CH4","Compost-N2O","Compost-NH3","Lagoon-CO2","Lagoon-CH4","Lagoon-N2O","Lagoon-NH3","Digester-CO2","Digester-CH4","Digester-N2O","Digester-NH3");
#endif

            fprintf(mc, "%-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-40s\n",
                         "Grain-C","Shoot-C","Root-C","CutBiomassC","GrazedBiomassC","dSOC","Field-CH4","Field-N2O","Field-NO","Field-N2","Field-NH3","Field-N-leach","Site_name");
            
            fprintf(mc, "%-5s,  %-5s, ", "File","Year");			

#ifdef MANURE
            fprintf(mc, "%-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, ",
                        "kgC","kgN","kgC","kgC","kgN","kgN","kgC","kgC","kgN","kgN","kgC","kgC","kgN","kgN","","","","");
#endif			
            fprintf(mc, "%-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-10s, %-40s\n",
                        "kgC/ha","kgC/ha","kgC/ha","kgC/ha","kgC/ha","kgC/ha", "kgC/ha","kgN/ha","kgN/ha","kgN/ha","kgN/ha","kgN/ha","Site_name");
        }
        else 
        {
            mc=fopen(MCC, "a");
        }

        fprintf(mc, "%5d,  %5d, ", sample, year);

#ifdef MANURE
        fprintf(mc, "%10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, ",
                 MData.yr_intake_C, MData.yr_intake_N, MData.yr_house_co2+MData.yr_enteric_co2, MData.yr_enteric_ch4+MData.yr_house_ch4,MData.yr_enteric_n2o+MData.yr_house_n2o, MData.yr_house_nh3,
                 MData.yr_compost_co2, MData.yr_compost_ch4, MData.yr_compost_n2o, MData.yr_compost_nh3, 
                 MData.yr_lagoon_co2, MData.yr_lagoon_ch4, MData.yr_lagoon_n2o, MData.yr_lagoon_nh3, 
                 MData.yr_digester_co2,MData.yr_digester_ch4,MData.yr_digester_n2o,MData.yr_digester_nh3);

#endif
        fprintf(mc, "%10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %10.2f, %-40s\n",
                 MC_grainC,MC_shootC,MC_rootC,MC_CutPlantC,MC_GrazePlantC, dSOC, yr_ch4, yr_dnn2o, yr_dnno, yr_dnn2, yr_soil_nh3, yr_leach_NO3+yr_leach_urea, site);
    }
/*	else
    {
        float SOC, Clay, pH, BD;
        int RN1, RN2, RN3, RN4;
        CString note;
            FILE *mcc;
            note.Format("%s\\Inter\\Combination_%d", OUTPUT, sample);
            mcc=fopen(note, "r");
            if ( mcc == NULL )
            {
                    CString mess;
                    mess.Format("Can not open %s.", note);
                    AfxMessageBox(mess);
                    exit(0);
            }
            fscanf(mcc, "%f %f %f %f", &SOC, &Clay, &pH, &BD);
            fscanf(mcc, "%d %d %d %d", &RN1, &RN2, &RN3, &RN4);
            fclose(mcc);

        MCC.Format("%s\\Record\\MonteCarlo\\MonteCarlo.txt", OUTPUT);
        if(sample==1) 
        {
            mc=fopen(MCC, "w");
            fprintf(mc, "Result of Monte Carlo test for crop %s in grid %s\n", cropping_system, county_ID1);
            fprintf(mc, "No.       dSOC        CH4         N2O         NO         N2         NH3        N_leaching          Combination of soil parameters\n");
            fprintf(mc, "          kg C/ha     kg C/ha     kg N/ha     kg N/ha    kg N/ha    kg N/ha    kg N/ha/yr          SOC       Clay      pH        Density\n");
        }
        else 
        {
            mc=fopen(MCC, "a");
        }

        
        fprintf(mc, "%6d,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,%10.2f,  %10.2f,            %2d,%6.3f,%2d,%6.2f,%2d,%6.2f,%2d,%6.2f\n",
                         sample, dSOC, yr_ch4, yr_dnn2o, yr_dnno, yr_dnn2, yr_soil_nh3, yr_leach_NO3+yr_leach_urea, RN1, SOC, RN2, Clay, RN3, pH, RN4, BD);
    }*/
    fclose(mc);
    
}

#endif
