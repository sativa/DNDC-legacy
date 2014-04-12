#include "stdafx.h"
#include "Dndcgo.h"
#include "Source_main.h"
#include "Dndc_tool.h"

void class_model::daily_WT(FILE* fWT)
{
    int jjday;

    //day_WT: unit; m, positive is above ground; negative is below ground
    if (year==1&&jday==1)//upland or irrigation-controlled rice paddy
    {
        if(WaterControl==0||WaterControl==1)
        {
            day_WT = -WaterRetentionDepth;
        }

        if(WaterControl==3) day_WT = m_IniWT;
    }

    ///////////////////////////////////////////////////////////////////////
    if(rainfall>0.0)
    {
        float logW = rainfall * (1.0 - DrainEfficiency);
        day_WT += logW;
    }
    else
    {
        if(flood_flag==0&&day_WT>-WaterRetentionDepth&&DrainEfficiency<1.0)
        {
            float Fcc = 3.2699 * (float)exp(-7.712*clay[1]) * DrainEfficiency;
            Fcc = min(1.0, max(0.0, Fcc));
            float releaseW = (day_WT + WaterRetentionDepth) * Fcc;			
            day_WT -= releaseW;				
        }
    }

    ///////////////////////////////////////////////////////////////////////

    //if(WaterControl==0)//upland or irrigation-controlled rice paddy
    if(flo_num>0) //WT in flooded field
    {
        WRL = q;

        for (int oz = 1; oz <= flo_num; oz++ )
        {
            if (jday==jdflo1[oz]) 
            {
                flood_num ++;
                day_WT = 0.1;
                day_irri = 0.1;
                flood_flag = 1;
                
                FloodWater_N = flood_N[oz];//added N in kg N/ha when WT changes from -01 to +0.1 m due to flooding
                nh4[1] += 0.5 * FloodWater_N;
                day_irri_N += FloodWater_N;
                //FloodWater_N *= 0.5;
                flood_N[oz] = 0.0;

                if(MargenalFlood[oz]==1) ShallowFloodFlag=1;
                else ShallowFloodFlag=0;
                
            }
            else if (jday==jdflo2[oz]) 
            {
                //if(MargenalFlood[oz]==0)
                //{
                    day_WT = -9.99;//0.1;
                    //day_WT -= 0.1;
                    //day_WT = max(-9.99, day_WT);

                    flood_flag = -10;
                    ShallowFloodFlag=0;
                    FloodWater_N = 0.0;

                    flood_end = 1;
                //}
                //else
                //{
                //	day_WT = 0.0;
                //	flood_flag = 1;
                //	ShallowFloodFlag=0;
                //}
            }
            else 
            {
                            
            }			
        }

        if(ShallowFloodFlag==1)
        {
            if(day_WT<=-0.1) dFlag=1;
            if(day_WT>=0.1) dFlag=0;

            if(dFlag==1) day_WT += 0.025;
            if(dFlag==0) day_WT -= 0.025;				
        }

        if(day_WT>=0.1&&precipitation>0.0)
        {
            float OL = 0.12;
            day_WT += precipitation;

            if(day_WT>OL)
            {
                float OverFlow = (day_WT - OL) ;
                day_runoff += OverFlow;
                day_WT = 0.1;

                if(OverFlow>0.0) 
                {
                    float Layer1N = nh4[1]+no3[1]+urea[1];

                    if(Layer1N>0.0)
                    {
                        float WaterN = 0.12 * Layer1N * OverFlow / (OverFlow + day_WT);
                        float Layer1LeftN = Layer1N - WaterN;	
                        day_runoff_N += WaterN;
                        nh4[1] = Layer1LeftN * nh4[1] / Layer1N;
                        no3[1] = Layer1LeftN * no3[1] / Layer1N;
                        urea[1] = Layer1LeftN * urea[1] / Layer1N;
                    }

                    float OverFlowN = FloodWater_N * OverFlow / (OverFlow + day_WT);
                    FloodWater_N -= OverFlowN;
                    if(FloodWater_N<0.0) FloodWater_N = 0.0;
                    day_runoff_N += OverFlowN;
                }
            }
        }

        //supplement water to maitain WT of rice field
        if(flood_flag==1) 
            day_irri = rainfall - day_soil_ev - day_pond_ev - act_DayAT - day_leach_water;
    }
    else//			if(flood_flag==0&&ShallowFloodFlag==0)
    {
        if(IrriRice_flag==0&&ShallowFloodFlag==0&&flood_flag==0&&RainfedRice_flag==0)
            day_WT = -WaterRetentionDepth;

        WRL = 664;
    }
    
    if(DeepRice_flag==1) day_WT = 1.0; // WT in deepwater rice

    if(RainfedRice_flag==1) //WT in rainfed rice,
    {
        if(year==1&&jday==1) day_WT = -0.6;//60 cm below ground
        else
        {
            //if(crop[1]==46)
            {
                day_dWT = (rainfall * WaterGetherIndex - day_soil_ev - day_pond_ev - act_DayAT - day_leach_water);// / sts[1];//????
                    
                day_WT += day_dWT; 

                if(day_WT > 0.2)
                {
                    day_runoff += (day_WT - 0.2);
                    day_WT = 0.2;
                }
                if(day_WT < -0.6) day_WT = -0.6;
            }

            if(day_WT>0.0) flood_num = 1;
                
        }		
    }
    
    if(WaterControl==2) //observed WT data control
    {
        fscanf(fWT, "%d %f", &jjday, &day_WT);//water table, m (positive is above-ground)
        
        wetland_flag = 1;

        if(day_WT>-0.5) flood_flag = 1;

    }
    
    if(WaterControl==3) //empirical-parameter controlled WT
    {
        CreateEmpiricalDayWT();

        wetland_flag = 1;
    }
        
    WTL = (int)(-day_WT/h[1]);//Depth_convert_to_layer(-day_WT, h, q)+1;//(int)(-day_WT/h[1]);//????
    //WTL = max(0, WTL);

    if(WTL<=0) flood_flag = 1;
    //if(WTL<=q) flood_flag = 1;
}

void class_model::init_soil_moisture()
{
    int l;

    for(l=1; l<=q; l++)
    {
        float ps = h[l]*sts[l];
        if(year==1&&jday==1)
        {	
            //day_wfps[l] = fldcap[l];
            water[24][l] = day_wfps[l] * ps;//m water/layer
            water[1][l] = day_wfps[l] * ps;
        }
        
        if(jday==1)
            yr_ini_water += water[1][l]+ice[l]/ 10000.0;//m water in soil profile, //ice[l]: m3 water/lay/ha
        
        if(WTL<=l) 
        {
            day_wfps[l] = 1.0;
            water[1][l] = ps;
        }

    }

    if(year==1&&jday==1)
    {
        ini_leaf_water = leaf_water;
        ini_surf_water = surf_water;
        ini_snow_pack = snow_pack;    
        ini_soil_water = yr_ini_water;
    }

    DayIniWaterPool = WaterPool;

/*	if(jday==1)
    {
        for(l=1; l<=q; l++)
        {
            if(year==1)
            {				
                day_wfps[l] = fldcap;
                water[24][l] = day_wfps[l] * ps;//m water/layer
                water[1][l] = day_wfps[l] * ps;	
            }

            yr_ini_water += water[1][l]+ice[l]/ 10000.0;//m water in soil profile, //ice[l]: m3 water/lay/ha
        }

        if(year==1)
        {
            ini_leaf_water = leaf_water;
            ini_surf_water = surf_water;
            ini_snow_pack = snow_pack;    
            ini_soil_water = yr_ini_water;
            //WaterPool = 0.0;
        }
    }

    if(WTL<q)
    {
        for(l=1; l<=q; l++)	day_wfps[l] = 1.0;
    }

*/
        
    
}

void class_model::CreateEmpiricalDayWT()
{
    day_WT += (rainfall * m_WatershedIndex);// m 

    if(day_WT > m_LWTceasingSurfFlow) //occurance of surface flow
    {
        float SurfDischarge = (day_WT - m_LWTceasingSurfFlow) * m_SurfOutflowIntensity;// m water/day
        day_WT -= SurfDischarge;
    }

    if(day_WT > m_LWTceasingGroungFlow) //occurance of surface flow
    {
        float GroundDischarge = (day_WT - m_LWTceasingGroungFlow) * m_GroundOutflowIntensity;// m water/day
        day_WT -= GroundDischarge;
    }
}

void class_model::CreateObservedDayWT(void)
{
    CString f_land, ff, WT_file_name;
    FILE *Fland;
    FILE *FF;

    int jjday[366], i, j, kk, xx; 
    float WTdepth[366], today_WT;

    ff.Format("%s\\Inputs\\WaterTable", OUTPUT);	
    FF = fopen(ff, "w");
    if(FF==NULL) note(1, ff);
    
    f_land.Format("%s", WT_file);	
    Fland = fopen(f_land, "r");

    if(Fland==NULL) note(0,f_land);
    
    char ju[300];
    fgets(ju, 200, Fland);
    fgets(ju, 200, Fland);

    i=0;
    
    for(;;)
    {
        i++;

        if(fscanf(Fland, "%d %f", &jjday[i], &WTdepth[i])!=2) 
        {
            xx=i-1;
            break;
        }
        else WTdepth[i] *= 0.01; //cm -> m		
    }

    kk=1;
    for(j=1; j<=365; j++)
    {
        if(j<jjday[1]) 
        {
            today_WT = WTdepth[1];
            
        }
        else if(j==jjday[kk])
        {			
            today_WT = WTdepth[kk];
            kk++;
        }
        else if(j>jjday[kk-1]&&j<jjday[kk])
        {
            today_WT += (WTdepth[kk] - WTdepth[kk-1])/(jjday[kk] - jjday[kk-1]);
        }

        else if(j>jjday[xx])
        {
            today_WT = WTdepth[xx];
        }
        else
        {
            
        }
        
        fprintf(FF, "%4d  %10.4f\n", j, today_WT);
    }

    fclose(Fland);
    fclose(FF);

}