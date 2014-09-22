//DNDC coded by Changsheng Li, University of New Hampshire in 1989-2014
//Note: 
//Eliminating graphics requires to 
//      (1) activate #define DEGUI in dndc_main.h and Source_main.h, 
//      (2) activate "OpenSesame @3" and comment off "Model_link @1" in DNDCgo.def, 
//      
//Recovering graphics requires to 
//      (1) comment off #define DEGUI in Dndc_main.cpp and Source_main.cpp, 
//      (2) comment off "OpenSesame @3" and activate "Model_link @1" in DNDCgo.def, 
//      


#include "stdafx.h"
#include "Dndcgo.h"
#include "Source_main.h"

#include "Dndc_tool.h"
#include "direct.h"

#include "RunPaths.h"

#include <iostream>

///
/*
void WINAPI SetWorkingDir( const char* rootPath )
{
    ROOTDIR = rootPath;
    //OUTPUT = "C:\\dev\\leaf\\install-32-bit\\share\\leaf\\dndc\\RESULT";

}
*/

///
int Depth_convert_to_layer(float DepthO, float *hh, int q);

///
class_model::class_model()
{

}
class_model::~class_model()
{

}

#ifdef MANURE
class_manure::class_manure()
{

}
class_manure::~class_manure()
{

}

class_manure *MOM;
extern class_manure MData;

#endif

#ifdef DEGUI
int class_model::DNDClinuxgo(int MODE, int SoilYear, float ha, 
                        char* cropping_system, int province, int county, int CountyIDX, 
                        int LandOrder, int MonteCarlo, int scale, int S_ThisYear,
                        char* r_Province, int ZX, int Batch, int sample, int RRRRR, 
                        int IrriType, char* country_ID, int livestock, int Monitor, int drc,
                        int DroughtID, char f_region[6][150], char* DroughtYield, int FFN,
                        char *BatchName, int FarmField)
#else
int class_model::DNDC100go(int MODE, int SoilYear, float ha, 
                        char* cropping_system, int province, int county, int CountyIDX, 
                        int LandOrder, int MonteCarlo, int scale, int S_ThisYear,
                        char* r_Province, int ZX, int Batch, int sample, int RRRRR, 
                        int IrriType, char* country_ID, int livestock, int Monitor, int drc,
                        int DroughtID, char f_region[6][150], char* DroughtYield, int FFN, 
                        int FarmField, char *BatchPass)
#endif
{

    FILE   *fc=NULL, *fn=NULL, *fw=NULL, *fcrop=NULL, *fEh=NULL, *fday=NULL,*fpp=NULL,*fWT=NULL;
    FILE   *fNB=NULL, *fCB=NULL, *fLAI=NULL, *fs=NULL, *fg=NULL, *fma=NULL, *HSM=NULL, *fsp=NULL, *fmb=NULL, *fis=NULL;
    FILE   *ftf1=NULL,*ftf2=NULL,*ftf3=NULL,*ftf4=NULL,*ftf5=NULL,*ftf6=NULL, *fclim=NULL, *ftf=NULL;
    float  wrcvl01=0.0, wrcl01=0.0, wrcr01=0.0, wsoc01=0.0, wn2o01=0.0, wno01=0.0, wn201=0.0, wdoc01=0.0, winertC01=0.0, wcrb01=0.0, wcrhl01=0.0, wcrhr01;
    float  whumus01=0.0, wno201=0.0, wno301=0.0, wnh401=0.0, wnh301=0.0, wurea01=0.0, day_clay_n01=0;
    float till_fact=0.0, w_no2=0.0, w_nh3=0.0, w_n2o=0.0, w_n2=0.0, ttoo[90];
    float w_no3=0.0, w_nh4=0.0, wurea=0.0, wpool_no=0.0, OverT=0.0, wpool_no2=0.0, day_clayn=0;
    float dby_flux=0.0, jf=0.0, hrPE0=0.0, S_CO2IncreaseRate=0.0, AddC1=0.0, AddC2=0.0, AddC3=0.0, litco22=0.0, litco23=0.0, FM_C[7], FM_N[7], FM_H2O[7];
    float FarmCropArea[10]={0.0,0.0}, FarmCropManureFraction[10]={0.0,0.0};
    
    int i=0, l=0, err_no=0, TTTFlag=0, tilq=0, junkd=0, ShallowFlood=0, SelectedCrops=0, SC[150], GlobeData=0, StartGridID=0;
    int TotalCells=0, SimulatedCells=0, TotalManureCrops=0, ManureCropOrder=0, CropIDID=0, GN=0, FN=0;
    int FarmCropID[10], ETM=0, YieldSite=0, YieldCrop=0, YieldYear=0, YieldToday=0, SoilDataUse=0;
    
    char DAYW[200], DAYC[200], DAYN[200], DAYP[200], DAYS[200], DAYM[200],DAYnBalance[200],DAYSP[200],DAYMB[200],DAYIS[200];
    char js[200],IE[200], F_CLIMD[300], F_WT[300], F_LV[200], Zz[200], DAYclim[200];
    char DAYcBalance[200], DAYG[200];//, BatchFileName[300];
    char HrSoilMoist[100], YieldCountyName[300], LB[300];
    char Pass[300], PassC1[300],PassL1[300];

    FILE *fout, *fout2=NULL;
    FILE* ddb;
    sprintf(LB,"%sResult\\inter\\UnitSystem", ROOTDIR);
    ddb=fopen(LB, "r");
    if(ddb==NULL) note(0, LB);
    fscanf(ddb, "%d", &m_unit);
    fclose(ddb);

    char FCT20[400], FCT40[400];
    cwc = 1;

    if(scale==0)
    {
        if(Batch==1)
        {
#ifdef DEGUI
            sprintf(Pass, "%s", BatchName);
            
#else
        
#ifdef MANURE
            sprintf(PassC1, "%s\\CropSystem\\Field_%d", BatchPass, FarmField);
            mkdir(PassC1);

            sprintf(PassL1, "%s\\LivestockSystem", BatchPass);
            mkdir(PassL1);

#else
            sprintf(Pass, "%s", BatchPass);
            sprintf(PassC1, "%s", BatchPass);
#endif

            
#endif
        }
        else
        {
            sprintf(Pass, "%s\\RECORD\\Site", OUTPUT);
#ifdef MANURE
            sprintf(PassC1, "%s\\RECORD\\Site\\FarmField_%d", OUTPUT, FarmField);
            mkdir(PassC1);
#else

#endif
        }
    }

#ifdef HYDRO
        char DAYMUSLE[90];
        FILE *fmusle=NULL;
#endif
        
    DailyCO2 = 0;

    for(int i1=1;i1<=55;i1++) SMC[i1]=0.0; 

    Option = Batch;

    char r_Country[120], r_Scenario[240];

    int FarmFile, SimuYears, r_NewScenario=0, _ShallowWater;
    char Fregion[300];
    FILE *region;
    FILE *fertg;
    IFDAILY = 0;
    ViewMode = 0;

    

    if(scale==1)
    {
        year = S_ThisYear;

        if(MonteCarlo==0)
        {
            sprintf(Fregion, "%s\\INTER\\region_input", OUTPUT);
            region = fopen(Fregion, "r");
            if(region==NULL)
            {
                AfxMessageBox("Can not open file dndcdata\\inter\\region");
                exit(0);
            }
            fscanf(region, "%d %d %d", &RegionCode, &CountryCode, &StateCode);
            fscanf(region, "%s", r_Country);
            fscanf(region, "%s %s", js, js);
            fscanf(region, "%s", mDatabaseName);
            fscanf(region, "%s", r_Scenario);
            fscanf(region, "%d", &DBType);
            fscanf(region, "%f %f %f", &jf, &jf, &jf);
            fscanf(region, "%d", &MajorConcern);
            fscanf(region, "%d", &DailyRecord);
            fscanf(region, "%d", &SimuYears);
            fscanf(region, "%d", &StartYear);
            fscanf(region, "%d", &ViewMode); 
            fscanf(region, "%d", &FarmFile);
            fscanf(region, "%d", &GlobeData);
            fscanf(region, "%d", &StartGridID);
            fscanf(region, "%d", &TotalCells);
            fscanf(region, "%d", &SimulatedCells);
            fscanf(region, "%d", &SoilDataUse);//0: FullIrr/NoIrri + SoilMax/min; 1: FullIrr/NoIrri + Soil ave; 2: NoIrri + Soil max/min; 3: NoIrri + Soil ave
            fscanf(region, "%d", &DroughtID);
            fclose(region);	

            if(FarmFile==1)
            {
                //for(int yy=1; yy<=F_SimuYears; yy++)
                float jf;
                int jd;

                {
                    sprintf(Fregion, "%s\\region_yr%d", INTER, year);
                    region = fopen(Fregion, "r");	
                    
                    fscanf(region, "%f %f %f", &jf, &jf, &jf);//&RY_AtmoCO2[yy], &RY_AirTemp[yy], &RY_RainTimes[yy]);
                    fscanf(region, "%f %f %f", &jf, &jf, &jf);//&RY_LitterBackField[yy], &RY_FertilizerRate[yy], &RY_AnimalManure[yy]);
                    fscanf(region, "%d %d %d", &jd, &jd, &jd);//&RY_AutoFert[yy], &RY_FloodAlter[yy], &RY_TillAlter[yy]);	
                    fscanf(region, "%f %f %f", &jf, &jf, &jf);//&RY_YieldIncrease[yy], &RY_WaterDemand[yy], &RY_MaxTemp[yy]);
                    fscanf(region, "%f %f %f %f %f", &jf, &jf, &jf, &jf, &jf);//&RY_CropCN[yy], &RY_IrriIndex[yy], &jf, &jf, &jf);
                    fscanf(region, "%d %d %d %d %d", &jd, &RiceDrySeeding, &jd,&jd,&jd);//&RY_IrriAlter[yy], &RY_RicePlantAlter[yy], &jd, &jd, &jd);
                    
                    fclose(region);						
                }
            }

            if(DailyRecord==0) IFDAILY=0;
            else IFDAILY=1;
        }
        else
        {
            IFDAILY=0;
            ViewMode=1;
            sprintf(stateName, "");
        }

        CropModel = 0;
    }
    else 
    {
        CropModel = 0;

        if(Option>100&&Option<100000)//Monte Carlo
        {
            CString SMCC;
            FILE *smc;
            SMCC.Format("%s\\Inter\\SiteMCinput", OUTPUT);
            smc=fopen(SMCC, "r");
            if(smc==NULL) note(0, SMCC);
            
            fscanf(smc, "%d", &SMC_k);
            for(int i=1; i<=SMC_k; i++)
            {
                fscanf(smc, "%d %f", &SMC_item[i], &SMC_value[i]);
    
                SMC[SMC_item[i]]=SMC_value[i];
            }
            fscanf(smc, "%d %d", &MC_StartDay, &MC_EndDay);
            fclose(smc);
        }
    } 
    

        CString txt,pro;
        txt=r_Country;
        int pos=txt.Find(',');
        if (pos==-1) 
        {
        }
        else
        {
            r_Country[pos]='\0';
        }

    //initiation();
    read_in_site(scale, Batch);
    read_in_clim(DroughtID);
    ETM = read_in_climy(scale, drc, DroughtID);	

    if(DroughtID==1&&year==2)
    {
        int DYR=drc;
        if(drc>=4) DYR=2;
        sprintf(F_CLIMD, "%s\\INPUTS\\%s", OUTPUT, climd[DYR]);
    }
    else
    {
        sprintf(F_CLIMD, "%s\\INPUTS\\%s", OUTPUT, climd[0]);
    }

    char YR[250], YR2[250];
    //FILE *fout, *fout2;
    CString DMW[4]={"","dry","mid","wet"};


    if(Batch==1)
    {
#ifdef DEGUI
        sprintf(YR, "%s\\AnnualReport_CroppingSystem_Yr%d.txt", Pass, year);		
#else
        /*CString Fregion;
        FILE *region;
        Fregion.Format("\%s\\Inter\\BatchRecord", OUTPUT);
        region = fopen(Fregion, "r");
        if (region==NULL)
        {
            char err[120];
            sprintf(err, "Can not open %s", Fregion);
            AfxMessageBox(err);
            exit(0); 
        }
        fscanf(region, "%s", BatchFileName);		
        fclose (region);*/
#ifdef MANURE
        sprintf(YR, "%s\\AnnualReport_CroppingSystem_Yr%d.txt", PassC1, year);	//BatchPass
#else

        sprintf(YR, "%s\\AnnualReport_CroppingSystem_Yr%d.txt", Pass, year);
#endif

#endif
        fout = fopen(YR, "w");
        if (fout==NULL) note(1, YR);
    }
    else
    {
        if(DroughtID==0)
        {
            if(scale==0)
            {
#ifdef MANURE
            sprintf(YR, "%s\\AnnualReport_CroppingSystem_Yr%d.txt", PassC1, year);	//BatchPass
#else
            sprintf(YR, "%s\\AnnualReport_CroppingSystem_yr%d.txt", Pass, year);
#endif
            fout = fopen(YR, "w");
            if (fout==NULL) note(1, YR);
            }
        }
        else
        {
            CString YF;
            FILE* yf;
            YF.Format("%s\\YieldPrediction", INTER);
            yf=fopen(YF, "r");
            if(yf==NULL) note(0,YF);
            fscanf(yf, "%d %d %d %d %s", &YieldSite, &YieldCrop, &YieldYear, &YieldToday, YieldCountyName);
            fclose(yf);
        }
    }
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef MANURE

    MData.EMDD = 0;
    read_in_soil_parm(scale, 0, 0);

    if(Batch==0) sprintf(YR2, "%s\\AnnualReport_LivestockSystem_yr%d.txt", Pass, year);
    else sprintf(YR2, "%s\\AnnualReport_LivestockSystem_yr%d.txt", PassL1, year);

    if(FarmField==1) fout2 = fopen(YR2, "w");	
    else fout2 = fopen(YR2, "a");	

    if (fout2==NULL) note(1, YR2);

    Main_Feedlot(year, scale, fout2, F_CLIMD, w_no2, w_nh3, w_no3, w_nh4, wurea, day_clayn, wpool_no,
                 tilq, fCB, fNB, w_n2, litco22, litco23, till_fact, ttoo, ha, Batch, livestock, 
                 q, PassL1, FarmField, Pass, HSM);

    Main_Treatment(year, scale, F_CLIMD, w_no2, w_nh3, w_no3, w_nh4, wurea, day_clayn, wpool_no, tilq, 
                fCB, fNB, w_n2, litco22, litco23, till_fact, ttoo, ha, Batch, livestock, fout2, PassL1,
                FarmField, Pass, HSM);

    int FarmCropOrder;

    CString XFF;
    FILE *xff;		
    XFF.Format("%s\\ManureFieldCrops", INTER);		
    xff=fopen(XFF, "r");
    if(xff==NULL) note(0,XFF);
    fscanf(xff, "%d", &TotalManureCrops);
    {
    for(int i=1;i<=TotalManureCrops;i++)
    fscanf(xff, "%d %d %f %f", &FarmCropOrder, &FarmCropID[i], &FarmCropArea[i], &FarmCropManureFraction[i]);
    }
    fclose(xff);
        
#else
    TotalManureCrops = 1;
    sprintf(FCT20, "%s\\Record\\Site", OUTPUT);
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
//for(int CL=1; CL<=TotalManureCrops; CL++)
//{
    initiation(2);

#ifdef DEGUI
#else
    if(scale==0)
    {	
        //if(Monitor==0) frame(pLinkInfo, "None", "None", site, scale, year);
    }
    else
    {
        sprintf(site, "%d", CountyIDX);


        if(MonteCarlo==1) 
        {
            //if(Monitor==0) frame(pLinkInfo, countryName, stateName, site, scale, LandOrder);
        }
        else 
        {
            //if(Monitor==0) frame(pLinkInfo, countryName, stateName, site, scale, year);//+
        }
    }
#endif

#ifdef MANURE
    char FTF[100];
    sprintf(FTF, "%s\\FarmManureToField_%d", INTER, year);
    ftf=fopen(FTF, "r");
    if(ftf==NULL) note(0, FTF);	
#endif

    read_in_soil_parm(scale, SoilYear, MonteCarlo);

    soc_distribution(CRB1, CRB2, newh);

    ini_clay_N = day_clay_N;
    yr_leach_NO3 = 0.0;
    yr_leach_urea = 0.0;
    yr_plant_nh3 = 0.0;
    yr_soil_nh3 = 0.0;	
    
    LL0 = 1;
    LL6 = Depth_convert_to_layer(0.05, h, q);
    LL1 = Depth_convert_to_layer(0.1, h, q);
    LL2 = Depth_convert_to_layer(0.2, h, q);
    LL3 = Depth_convert_to_layer(0.3, h, q);
    LL4 = Depth_convert_to_layer(0.4, h, q);
    LL5 = Depth_convert_to_layer(0.5, h, q)-1;
    
#ifdef MANURE
    if(scale==0&&Batch==1) 
    {
        sprintf(FCT20, "%s\\FarmCrop_%d", BatchPass, FarmField);//pass for writing output
        sprintf(FCT40, "%s\\FarmCrop_%d", INPUTS, FarmField);//pass for reading input
    }
    else
    {
        sprintf(FCT20, "%s", PassC1);//pass for reading input
        sprintf(FCT40, "%s\\FarmCrop_%d", INPUTS, FarmField);//pass for reading input
    } 
#else
    if(scale==0&&Batch==1) sprintf(FCT20,"%s", Pass);
    else sprintf(FCT20,"%s\\Record\\Site", OUTPUT);

    sprintf(FCT40,"%s", INPUTS);
#endif	
    if(DroughtID==1)
    {
        if(year==2&&(drc>=1&&drc<=3)&&IrriType==0)
        {

                if(drc==1)
                    sprintf(DAYS, "%s\\Day_SoilClimate_%d_dry.%s", FCT20, year, CSV);
                else if(drc==2)
                    sprintf(DAYS, "%s\\Day_SoilClimate_%d_mid.%s", FCT20, year, CSV);
                else if(drc==3)
                    sprintf(DAYS, "%s\\Day_SoilClimate_%d_wet.%s", FCT20, year, CSV);
                else 
                    sprintf(DAYS, "%s\\Day_SoilClimate_%d.%s", FCT20, year, CSV);

                fs = fopen( DAYS, "w" );
                if ( fs == NULL ) note(1,DAYS);

                if(drc==1)
                    sprintf(DAYP, "%s\\Day_FieldCrop_%d_dry.%s", FCT20, year, CSV);
                else if(drc==2)
                    sprintf(DAYP, "%s\\Day_FieldCrop_%d_mid.%s", FCT20, year, CSV);
                else if(drc==3)
                    sprintf(DAYP, "%s\\Day_FieldCrop_%d_wet.%s", FCT20, year, CSV);
                else 
                    sprintf(DAYP, "%s\\Day_FieldCrop_%d.%s", FCT20, year, CSV);

                fcrop = fopen( DAYP, "w" );
                if ( fcrop == NULL ) note(1,DAYP);	
        }
    }
    else
    {
        if (scale == 0 && day_file ==1)
        {
            if(Batch==1) sprintf(FCT20, "%s", PassC1);

            sprintf(DAYclim, "%s\\Day_Climate_%d.%s", FCT20, year, CSV);
            fclim = fopen( DAYclim, "w" );
            if ( fclim == NULL ) note(1,DAYclim);

            sprintf(DAYC, "%s\\Day_SoilC_%d.%s", FCT20, year, CSV);
            fc = fopen( DAYC, "w");
            if ( fc == NULL ) note(1,DAYC);
            
            sprintf(DAYW, "%s\\Day_SoilWater_%d.%s", FCT20, year, CSV);
            fw = fopen( DAYW, "w" );
            if ( fw == NULL ) note(1,DAYW);

            sprintf(DAYN, "%s\\Day_SoilN_%d.%s", FCT20, year, CSV);
            fn = fopen( DAYN, "w" );
            if ( fn == NULL ) note(1,DAYN);

            sprintf(DAYG, "%s\\Day_Graze_%d.%s", FCT20, year, CSV);
            fg = fopen( DAYG, "w" );
            if ( fg == NULL ) note(1,DAYG);		

            sprintf(DAYM, "%s\\Day_FieldManage_%d.%s", FCT20, year, CSV);
            fma = fopen( DAYM, "w" );
            if ( fma == NULL ) note(1,DAYM);

            sprintf(DAYS, "%s\\Day_SoilClimate_%d.%s", FCT20, year, CSV);
            fs = fopen( DAYS, "w");
            if ( fs == NULL ) note(1,DAYS);

            sprintf(DAYP, "%s\\Day_FieldCrop_%d.%s", FCT20, year, CSV);
            fcrop = fopen( DAYP, "w");
            if ( fcrop == NULL ) note(1,DAYP);

            sprintf(DAYSP, "%s\\Day_SoilP_%d.%s", FCT20, year, CSV);
            fsp = fopen( DAYSP, "w");
            if (fsp == NULL ) note(1,DAYSP);

            sprintf(DAYMB, "%s\\Day_SoilMicrobe_%d.%s", FCT20, year, CSV);
            fmb = fopen( DAYMB, "w" );
            if ( fmb == NULL ) note(1,DAYMB);
#ifdef ISOTOPE
            sprintf(DAYIS, "%s\\Day_C_isotope_%d.%s", FCT20, year, CSV);
            fis = fopen( DAYIS,"w" );
            if ( fis == NULL ) note(1,DAYIS);
#endif
        }
    }
    
        
#ifdef BALANCE
        sprintf(DAYnBalance, "%s\\Day_SoilBalanceN_%d.%s", FCT20, year, CSV);
        fNB = fopen( DAYnBalance, "w");
        if ( fNB == NULL ) note(1,DAYnBalance);

        sprintf(DAYcBalance, "%s\\Day_SoilBalanceC_%d.%s", FCT20, year, CSV);
        fCB = fopen( DAYcBalance, "w");
        if ( fCB == NULL ) note(1,DAYcBalance);
#endif

#ifdef HOURLY
        //sprintf(HrSoilMoist, "%s\\Hour_SoilMoisture_%d.%s", FCT20, year, CSV);
        sprintf(HrSoilMoist, "%s\\Hour_SoilN_%d.%s", FCT20, year, CSV);
        HSM = fopen( HrSoilMoist, "w" );
        if ( HSM == NULL ) note(1,HrSoilMoist);
#endif


#ifdef HYDRO//===Create file for SCS and Musle result;dengjia=== 
        sprintf(DAYMUSLE, "%s\\Day_MUSLE_%d.%s", FCT20,year, CSV);
        fmusle = fopen( DAYMUSLE, "w" );
        if ( fmusle == NULL ) note(1,DAYMUSLE);	
#endif

    //}	

    wrcvl01 = 0.0; wrcl01 = 0.0; wrcr01 = 0.0; wcrb01 = 0.0; wcrhl01=0.0; wcrhr01=0.0; whumus01 = 0.0;
    wno201 = 0.0; wno301 = 0.0; wnh401 = 0.0; wnh301 = 0.0; wurea01 = 0.0;
    day_clay_n01 = 0.0; wn2o01=0.0; wno01=0.0; wn201=0.0; wdoc01=0.0; winertC01=0.0;
    wFreezedoc = 0.0;

    for ( l = 1 ; l <= q; l++ )
    {
        wrcvl01 += rcvl[l];
        wrcl01 += rcl[l];
        wrcr01 += rcr[l];
        wcrb01 += CRB1[l]+CRB2[l];
        wcrhl01 += crhl[l];
        wcrhr01 += crhr[l];
        whumus01 += dphum[l];
        wdoc01 += doc[l];
        winertC01 += inert_C[l];

        wno201 += no2[l];
        wno301 += no3[l];
        wnh401 += (nh4[l] + nh3[l] + NH4HCO3[l]);
        wurea01 += urea[l];
        day_clay_n01 += clay_nh4[l];
        sph[l] = soil_ph[1];
        wn2o01 += n2o[l];
        wno01 += no[l];
        wn201 += n2[l];
        wFreezedoc += FreezeDOC[l];
    }

    err_no = read_in_flood(MODE, FCT40);
    if ( err_no ) return( err_no );

    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////
    read_in_crop(MODE, GlobeData, FCT40);	
    ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////

    if((year==1&&scale==1&&WinterCropFlag[1]==1)||(year==1&&scale==0&&Batch==3))
        read_in_cropini(MODE, GlobeData, FCT40);//only for region mode


    PoolWater0 = WaterPool;

    read_in_irri(scale, IrriType, FCT40);//+

    err_no = read_in_till(FCT40);
    if ( err_no ) return( err_no );

    err_no = read_in_fert(MODE, FCT40);
    if ( err_no ) return( err_no );

    err_no = read_in_manu(FCT40);
    if ( err_no ) return( err_no );
    
    err_no = read_in_graz(FCT40);
    if ( err_no ) return( err_no );

    err_no = read_in_cut(FCT40);
    if ( err_no ) return( err_no );
    
    read_in_plastic(FCT40);
    if(fer_num==-1)
    {
        char  FERT[180], WSD[200];
            
        sprintf(FERT, "%s", FertigationFile);
        fertg = fopen(FERT, "r");
        if ( fertg == NULL ) 
        {
            char err[60];
            sprintf(err,"%s not found",FERT);
            MessageBox(NULL, err, "Error Message", 0);
            return( FILE_NOT_FOUND );
        }
        fgets(WSD, 200, fertg);
        //fgets(WSD, 200, fertg);
        //fgets(WSD, 200, fertg);
    }
    
    //if(ViewMode==0) map_note(pLinkInfo, r_Country, stateName);

    LaiFlag = 0;
  
    fday = fopen(F_CLIMD, "r");
    if (fday == NULL) note(0,F_CLIMD);
    
    if(WaterControl==2)
    {
        sprintf(F_WT, "%s\\Inputs\\WaterTable", OUTPUT);
        fWT = fopen(F_WT, "r");
        if(fWT==NULL) note(0, F_WT);
    }

    TTTFlag = 0;
   
    
    tilq = 0;
    //PGI[cwc] = 0.0;
    till_fact = 0.0;
    
    day_leach_DOC = 0.0;
    //yr_fixn[cwc] = 0.0;
    //irri_water = 0.0;
    
    total_ice = 0.0;
    dw = 1.0;
    yr_ini_water = 0.0;

    jday = 0;

    if(year==1) 
    {
        frost_doc=0.0;
    }
    
    char Sday[250];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Start daily loop
for(jday=1; jday<=365; jday++)
{

    int junk;
    sprintf(Sday,"%d",jday);

#ifdef DEGUI
#else
    /*
    if (pLinkInfo->StopRun==1)//stop
    {
        break;
    }
    else if (pLinkInfo->StopRun==2)//pause
    {
        //AfxMessageBox("Pause here");
        int k;
        for (;;) 
        {
            for (k=1;k<1000;k++) {}

            CWnd* pDC = pLinkInfo->pWnd;
            CDC& dc = *(pLinkInfo->pDC);

            MSG message;
            if (::PeekMessage(&message,NULL,0,0,PM_REMOVE)) {
                ::TranslateMessage(&message);
                ::DispatchMessage(&message);
            }

            if (pLinkInfo->StopRun!=2) 
            {				
                break;
            }
        }
    }
    */
#endif

    day_zero();
    
    for(int i=1; i<=q; i++) 
    {
        if(jday>1) water[1][i] = water[24][i];
        
        ini_soil_water += (water[1][i] + ice[i] / 10000.0);
    }
#ifdef BALANCE
    CheckBalance(1);//reserved //+
#endif

    //ini_prof_SOC = prof_SOC;
    //ini_SON = prof_SON;
    //ini_SIN = prof_SIN;
    //ini_ClayNH4 = prof_ClayNH4;

    //T_SOC = old_T_SOC;

    w_no2 = 0.0;
    w_nh3 = 0.0;
    w_no3 = 0.0;
    w_nh4 = 0.0;
    w_n2o = 0.0;
    w_n2 = 0.0;
    wurea = 0.0;
    wpool_no = 0.0;
    wpool_no2 = 0.0;
    //w_no = 0.0;

    if(manuFlag==1) ManureDays++;
    else ManureDays = 0;
    if(ManureDays>60) manuFlag=0;

    // Read in jday and daily climate data. PAR: total daily energy MJ/m2/day
    float dT, dR;

    fscanf(fday, "%d %f %f %f %f %f %f %f %f", &junk, &air_temp, &max_temp, &min_temp, &precipitation, &PAR, &Day_Hrs, &wind_speed, &humidity);

    if(PAR<=0.0) PAR = 0.01;

#ifdef HYDRO//===Read extra climate parameter;dengjia===
    if(scale==1)
    fscanf(fday, "%f", &MaxHalfHR);//mm water
#endif

    if(SCSuse==1)
    {
        MaxHalfHR = precipitation / 5.0;//estimated max half-hour precipitation, cm 
    }
    NoRainFlag = 0;
    if(DroughtID==1&&drc>=4)
    {
        if(drc==4&&(junk>=YieldToday&&junk<(YieldToday+0))) 
        {
            precipitation = 0.0;
            NoRainFlag = 1;
        }
        if(drc==5&&(junk>=YieldToday&&junk<(YieldToday+5))) 
        {
            precipitation = 0.0;
            NoRainFlag = 1;
        }
        else if(drc==6&&(junk>=YieldToday&&junk<(YieldToday+10))) 
        {
            precipitation = 0.0;
            NoRainFlag = 1;
        }
        else if(drc==7&&(junk>=YieldToday&&junk<(YieldToday+20))) 
        {
            precipitation = 0.0;
            NoRainFlag = 1;
        }
        else if(drc==8&&(junk>=YieldToday&&junk<(YieldToday+30))) 
        {
            precipitation = 0.0;
            NoRainFlag = 1;
        }
        else if(drc==9&&(junk>=YieldToday&&junk<(YieldToday+50))) 
        {
            precipitation = 0.0;
            NoRainFlag = 1;
        }
        else 
        {
            NoRainFlag = 0;
        }
    }
    //ref_PAR = day_ref_par(jday, latitude);
    if(plastic_num>0&&plastic_flag==1)
    {
        if(plastic_method==0)//greenhouse
        {
            air_temp += 5.0;
            max_temp += 5.0;
            min_temp += 5.0;
            wind_speed = 0.0;
            precipitation = 0.0;
        }
        else if(plastic_method==1)//film mulch
        {
            air_temp += 0.0;
            max_temp += 0.0;
            min_temp += 0.0;
            wind_speed = 0.0;
            //precipitation = 0.0;
        }
        else{}
    }

    yr_WindSpeed += wind_speed;

    if(LateralInflux>1.0&&precipitation>0.0)
    {
        precipitation *= LateralInflux;
    }

    horricane_event = 0.0;
    if(precipitation==999.0)
    {
        hail_event=1.0;
        precipitation=0.0;
    }
    else if(precipitation>888.0&&precipitation<888.9)
    {
        horricane_event=(precipitation - 888.0);
        precipitation=0.0;
    }
    else
    {
        hail_event=0.0;
        
        /*StormIndex = 12.0;//cm/day
        if(precipitation>StormIndex) 
            horricane_event=0.3;
        else horricane_event=0.0;*/
    }

    precipitation *= 0.01; //cm -> m
    
    if(Option>100)
    {
        dT = SMC[1];//temperature
        dR = SMC[2];//precipitation
    }
    else
    {
        dT = 0.0;//degree C
        dR = 0.0;//changing fraction
    }
    //DSRad = PAR;//MJ/m2/day
    //WDSAV = wind_speed;//m/s
    //TPAV = air_temp;

    if(max_temp < min_temp)
    {
        float tt;
        tt = max_temp;
        max_temp = min_temp;
        min_temp = tt;
    }
    
    air_temp += dT;
    max_temp += dT;
    min_temp += dT;
    
    precipitation *= (1.0 + dR);
    rainfall = precipitation;//m

    if (rainfall<=0.0001)
        TTT = 0.0;
    else
        TTT = rainfall / 0.005;//m/m, hrs

    yr_avet += (air_temp / 365.0);
    yr_rain += precipitation; //m	

    if(SCSuse==1) 
    {
        SCS_MUSLE(scale, ha, livestock);
        scs_runoff *= 0.001; //SCS runoff, mm -> m
    }
    
    //fertigation
    if(fer_num==-1)
    {
        int fjday;

        fscanf(fertg, "%d %f %f %f %f %f %f %f %f", &fjday, &day_irri, &day_fert1, &day_fert2,
                &day_fert3, &day_fert4, &day_fert5, &day_fert6, &day_fert7);
        if(fjday!=jday)
        {
            CString note;
            note.Format("Error at day %d if file %s", jday, FertigationFile);
            AfxMessageBox(note);
            exit(0);
        }

        if(FertOption==111)
        {
            day_irri *= 0.01;//cm -> m water
        }

        if(FertOption==999)
        {
            day_irri = day_irri * DayPET;
        }
        
        IrriMethod;// = 2;//drip
                

        
    }
    // irrigating
    TTT = irrigating(scale, DayPET);//+

    // manuring
    manuring(livestock, scale, TotalManureCrops, FarmCropArea, FarmCropManureFraction, FarmField, ftf);
    if(MicrobioIndex > BaseMicroIndex)
        MicrobioIndex -= 0.01;
    else
        MicrobioIndex = BaseMicroIndex;

    float TTOO = soc[1]/mmm[1];
    if(TTOO>=0.02) MicrobioIndex = BaseMicroIndex;
    else MicrobioIndex = BaseMicroIndex;
    if(MicrobioIndex>1.0) MicrobioIndex=1.0;

    //grazing
    if(gra_num>0) grazing();
    
    // grass cutting
    if (cut_num > 0) cutting();

    //Plastic cover
    PlasticCover();

    //tilling
    till_fact = tilling(&tilq, CRB1, CRB2, newh, till_fact);

    // fertilizing
    if(UreaAppDays>0) UreaAppDays++;
    fertilizing();

    //stub deposition
    stub_fall();

    soilCEC();
    // Soil climate
    // In case of heavy rainfall
    if (TTTFlag==1)
    {
        if ( jday == 1 ) OverT = 0.0;
        TTT += OverT;
        OverT = 0.0;
        TTTFlag = 0;
    }
    if ( TTT > 24.0 )
    {
        TTTFlag = 1;
        OverT = (TTT - 24.0);
        TTT = 24.0;
    }
    else
    {
        TTTFlag = 0;
        OverT = 0.0;
    }
    snowfall_snowpack();

    //flooding
    if(WaterControl==0) day_flooding(scale);

    if(WaterControl==3) CreateEmpiricalDayWT();

    if(WTL<=1) 
    {
        FloDays++;
        SatuDays++;
    }
    else 
    {
        FloDays = 0;

        if(SatuDays>0)
        {
            int yy = int(148.41 * (float)exp(-7.933 * clay[1]))+1;
            SatuDays -= 2;
            if(SatuDays<0) SatuDays = 0;
        }
    }
    //if(WaterControl==1&&crop_flag==0)
    //	FloDays = 0;

    init_soil_moisture();

    dby_flux = 0.0;	
    
    ini_day_PoolWater = WaterPool;

    //if(day_WT>0.0) water_temperatue();
    if(SatuDays>0) water_temperatue();	

    soil_temperature();	

    DayPET = potential_et(ETM);

//////////////////////////////////////////////////////

///////////////////////////////////////////////////////

    float DayPE=0.0;
    int ccwc;

    TotalCropBiomass = 0.0;
    
    dPlantBiomass=0.0;
    //day_RootCO2 = 0.0;
    //day_root_resp = 0.0;
    
    sum_shoot_CO2 = 0.0;
    sum_stem_CO2 = 0.0;
    float DeepSoilT;
    if(temp[q]<0.0) DeepSoilT = 0.0;
    else DeepSoilT = temp[q];
    for(ccwc=1; ccwc<=CropNumbers; ccwc++)
    {
        if(seed[ccwc] > 0) seed[ccwc]++;

        if(WinterCropFlag[ccwc]==1&&jday==1) 
            read_in_vege(scale, 1, ccwc);

        if(seed[ccwc]==0) 
            ini_crop(scale, ccwc);		
    
        if (seed[ccwc] > 0)
        {
            crop_days[ccwc]++;

            // crop harvesting
            if ((seed[ccwc]-1)==grow_days[ccwc]&&PerennialFlag[ccwc]==0)
                crop_harvesting(scale, ccwc);

            if(jday==365&&(grow_days[ccwc]-seed[ccwc])<2&&PerennialFlag[ccwc]==0)
                crop_harvesting(scale, ccwc);

            //flooding damage on upland crops

            //if(seed[ccwc]>1&&FloDays>30&&wetland_flag==0)
            //	crop_harvesting(scale, ccwc);
            
            if(crop_flag[ccwc]==1)
            {
                growth_stage(ccwc);

                water_demand(DayPET, ccwc);
                
                crop_PT[ccwc] += PPT[ccwc];
                day_SumPT += PPT[ccwc];
                
                DayPT0 += PPT[ccwc]; 
            }
        }

        if(crop[ccwc]==46) irri_close = 1;
        else irri_close = 0;
    }
    TodayPT = DayPT0;

    //Potential transpiration rate based soil water availability
    if(DayPT0>0.0)
    {
        DayPT2 = crop_transpiration(irri_close);
        DayWaterStress = DayPT2 / (TodayPT+0.0000001);	
        DayWaterStress = min(1.0, max(0.0, DayWaterStress));
    }
    else
    {
        DayPT2 = 0.0;	
        DayWaterStress = 1.0;
    }
                    
    for(ccwc=1; ccwc<=CropNumbers; ccwc++)
    {
        day_DWater[ccwc] = DayWaterStress;//crop_PT[ccwc] / day_SumPT;

        if(seed[ccwc]>0)
        {
            if(WaterControl>=1&&wetland_flag==1)
            {
                if(WTL<q) 
                    day_DWater[ccwc] = 1.0;
                else 
                    day_DWater[ccwc] = DayWaterStress;
            }
            else
            {
                if(flood_flag==1||(total_ice>0.0&&st[1]>=12)) 
                    day_DWater[ccwc] = 1.0;
                else 
                    day_DWater[ccwc] = DayWaterStress;
            }

            crop_water_stress[ccwc]+=day_DWater[ccwc];
        
                if (WiltFlag[ccwc]==0)
                { 				
                    crop_N_demand(ccwc);

                    if (day_N_demand[ccwc] > 0.0)
                       n_uptake_by_crop(ccwc);
                    else
                        day_nup[ccwc] = 0.0;

                    // NH3 uptake by and release from canopy 
                    nh3_uptake_release(lai, &day_soil_nh3, PlantTakeN, maxmn, leaf_water, cropht, base_nh3,
                                day_N_demand, day_nup, day_fixN, day_DWater, day_plant_nh3, &yr_NH3_deposit, ccwc);
        
                    total_crop_N(ccwc);

                    //if(maturity_flag[ccwc]==0)
                    biomass_partition(ccwc);

                    root_develop(ccwc);
                }
            
                crop_N_stress[ccwc]+=N_stress[ccwc];

                //water_demand(DayPET, ccwc);	

                float ddd;
                if(seed[ccwc] - grow_days[ccwc] * 0.5<0.0) ddd = -(seed[ccwc] - grow_days[ccwc] * 0.5);
                else ddd = (seed[ccwc] - grow_days[ccwc] * 0.5);

                crop_t = 1.0 - ddd / (grow_days[ccwc] * 0.5);

                //total_crop_N();
                if(seed[ccwc] > grow_days[ccwc] * 0.5) crop_t += 0.5;
                if(crop_t > 1.0) crop_t = 1.0;

                root_respir(ccwc);	

                //crop_PT[ccwc] += APT[ccwc];
                //day_SumPT += APT[ccwc];
                //DayPE = DayPET - PT[ccwc];
                //if(DayPE<0.0) DayPE = 0.0;
                //dtrans += APT[ccwc]; 
            
        }
        else
        {
            N_stress[ccwc] = 1.0;
            //TDD[ccwc] = 0.0;
            crop_t = 1.0;
            APT[ccwc] = 0.0;
            day_SumPT += 0.000000001;
        }
    //}
        TotalCropBiomass += (Grain_Wt[ccwc]+Leaf_Wt[ccwc]+Stem_Wt[ccwc]+Root_Wt[ccwc]);

        float ShootpBiomass = Leaf_Wt[ccwc] + Grain_Wt[ccwc];
        float StempBiomass = Stem_Wt[ccwc];

        if(ShootpBiomass+StempBiomass>0.0&&crop_flag[ccwc]==1&&air_temp>0.0)
        {
            if(crop[ccwc]==11||crop[ccwc]==12)
            {
                day_shoot_CO2[ccwc] = 0.25 * Day_C_growth[ccwc] * (1.0 - PGI[ccwc]);
                day_stem_CO2[ccwc] = 0.25 * Day_C_growth[ccwc] * (1.0 - PGI[ccwc]);
            }
            else
            {
                float FAT = air_temp / 50.0;
                day_shoot_CO2[ccwc] = 0.03 * ShootpBiomass * (1.0 - PGI[ccwc]) * FAT;//0.015 * ShootpBiomass;
                
                if(LandUse==6) day_stem_CO2[ccwc] = 0.001 * StempBiomass * (1.0 - PGI[ccwc]) * FAT;
                else day_stem_CO2[ccwc] = 0.03 * StempBiomass * (1.0 - PGI[ccwc]) * FAT;
                
                CropEndDay = jday;
            }			
        }
        else
        {
            day_shoot_CO2[ccwc] = 0.0;
            day_stem_CO2[ccwc] = 0.0;
        }
        sum_shoot_CO2 += day_shoot_CO2[ccwc];
        sum_stem_CO2 += day_stem_CO2[ccwc];

        dPlantBiomass += Day_C_growth[ccwc];
        
        //day_RootCO2 += day_rco2[ccwc];
        day_root_resp += day_rco2[ccwc];

        //if(ShootpBiomass>0.0) crop_senescence();
        if(TotalCropBiomass>0.0) crop_senescence(ccwc);

        //daily_WT(fWT, ccwc);

    }

    if(hail_event>0.0) weather_damage(1, hail_event);
    if(horricane_event>0.0) weather_damage(2, horricane_event);
    
    if(CropNumbers==0)
    {
        act_DayAT = 0.0;
        act_DayPE = DayPET;
    }
    else
    {
        for(ccwc=1; ccwc<=CropNumbers; ccwc++)
        {
            if(DayAT > DayPET) 
            {
                float ddW = DayAT - DayPET;
                APT[ccwc] -= ddW * APT[ccwc] / DayAT;//actual daily crop transpiration constrained by PET, m water
                act_DayAT = DayPET;
                act_DayPE = 0.0;
            }
            else
            {
                act_DayAT = DayAT;
                act_DayPE = DayPET - DayAT;
            }		
        }
    }

    if(act_DayAT != DayPT2) 
    {
        water[1][10] += DayPT2 - act_DayAT;//??????????????????????
    }

    //if(DayPT0<=0.0) DayWaterStress = 1.0;
    //else DayWaterStress = act_DayAT / DayPT0;

    //DayWaterStress = DayAT / DayPT0;
    
    //Evaporation and water flow in soil matrix

    //if(DayPE<0.0) DayPE = 0.0;
    yr_PE += act_DayPE;
    hrPE0 = act_DayPE / 24.0;

    hour_loop(hrPE0, DayFlux, &dby_flux, DroughtID, YieldToday, drc, scale, ha, livestock);

    daily_WT(fWT);

    zero_DayFlux();

    //hour_loop(hrPE0, DayFlux, &dby_flux, DroughtID, YieldToday, drc, scale, ha, livestock);

    // Annual water budget 
    annu_water_budget(dby_flux);
    NH4_HCO3();

    N2O_balloon(q, till_fact, sts, h, jday, flood_flag, FloDays, rainfall,
        O2_old, O2, TTT, vvv, ave_anvf, q, st, temp, year, water, day_wfps,
        day_WT, WTL, total_ice, day_rco2, co2, flo_num, ice, sph, Eh, anvf,
        nh4, no3, no2, no, n2, doc, n2o,
        doc_w, nh4_w, no3_w, no2_w, no_w, n2o_w, n2_w,
        doc_d, nh4_d, no3_d, no2_d, no_d, n2o_d, LandUse, Tttoo,
        ManureAddCN, ManureDays, soc, MicrobioIndex, RBO, FD, WaterControl, 
        crop, assim2, clay, fldcap, by_passf, NIDay1, NIDay2,
        &day_soil_nitrify, &day_nitrify_NO, &day_nitrify_N2O, &day_soil_an_no,
        &day_soil_an_n2o, &day_soil_n2, Root_Wt, WaterLeakRate,
        rcvl, rcl, rcr, &day_soil_denitrification,
        wiltpt, CRB1, CRB2, rcnb, &day_frostDOC, &yr_frostDOC, precipitation, 
        manuFlag, irri_flag, FreezeDOC, nh3, Eh1, urea, LateralInflux, drain_flag,
        IrriRice_flag, HSM);

    Mn_Fe_S_oxidation();

    dndc_decomposition(CRB1, CRB2, newh, &w_no2, 
        &w_nh3, &w_no3, &w_nh4, &wurea, &wpool_no, 
        till_fact, DayFlux, tilq, fCB, fNB, &w_n2o, &w_n2,
        q, q, rcnrvl, rcnrl, rcnrr, rcnb, rcnh, rcnm, soc, son, 
        SIN, rcvl, rcl, rcr, dphum, doc, nh4, no3, no2, 
        no, n2o, n2, nh3, urea, jday, co2, clay, TTT, flood_flag,
        day_wfps, WTL, temp, LandUse, FloDays, crhl, crhr, mmm, ocini,
        h, &day_C_mine, &day_N_mine, &day_N_assim, DClitter, 
        &litco22, &litco23, st, DChumads, EFFAC, DChumus, sph, wetland_flag, wind_speed, ice, &day_soil_nh3,
        lai, PlantTakeN, maxmn,	leaf_water, cropht, base_nh3,day_N_demand, day_nup, day_fixN, day_DWater,
        day_plant_nh3, &yr_NH3_deposit, plantn, clay_nh4, base_clay_N, max_clay_N,
        &wrcvl, &wrcl, &wrcr, &wcrb, &wcrhl, &wcrhr, &whumus, &wtcavai,
        &day_ice_N, &day_ice_doc, &day_ice_n2o, &day_ice_n2, &IniSOC, &IniSON, &EndSON,
        day_runoff, &day_runoff_N, &day_clay_N, 
        dsno3, dsnh4, &dsnh3, &day_dsurea, &ds_doc, crop, yr_fixn, rainfall, soil_ph,
        &winertC, inert_C, fert_flag, root_doc, day_O2, RFM, SoilDepth, rcnh2, ini_ttoo, 
        sop, OrgP, LabP, AdsP, ComP, sts, day_leach_water);//++

    if(Eh[1]>0.0)//(flood_flag==0&&st[1]<=12)
    {
        if(rice_maturity_flag==1) 
            ch4_oxidation2();
        else 
            ch4_oxidation1();

        day_balance_ch4 += day_ch4;
    }

    end_soil_water=0.0;
    day_co2 = 0.0;

    //if(st[1]<12) deep_doc = 0.1 * old_T_SOC;
    //else deep_doc = 0.002 * old_T_SOC;
    
    float DeepDOC=0.0;

    //if(air_temp>0.0)
    //{
    for ( l = 1; l <= q; l++ ) 
    {
        end_soil_water += (water[24][l] + ice[l] / 10000.0);
    }

    if(flood_flag==1)
    {
        if(st[1]>=12&&IrriRice_flag==0&&DeepRice_flag==0&&RainfedRice_flag==0)
        {
            if(WTL<=q) 
            {
                saturation_Eh();
            }

            day_ch4 = CH4_peat(scale, q, temp, clay, flood_num, FloDays, Root_Wt,
                                  q, Eh, sph, vascularity, LandUse, day_rco2,
                                  R_Wt, &day_co2, root_dCO2, flood_flag, rcvl,
                                  rcl, rcr, st, rcnrvl, rcnrl, rcnrr,
                                  MicrobioIndex, ch4, WTL, day_WT, soil_ph, 
                                  doc, nh4, &ch4_prod, &ch4_day_oxid, &plant_day_ch4, 
                                  day_wfps, seed, &day_ch4, anvf, WaterLeakRate, h, CRB1, rcnb, co2);

            day_balance_ch4 += day_ch4;
        }
        else
        {
        
            if(WTL<=0) //q
            {

                saturation_Eh();
                CH4_paddy(scale, q, temp, clay, flood_num, FloDays, Root_Wt,
                                  q, Eh, sph, vascularity, LandUse, day_rco2,
                                  R_Wt, day_co2, root_dCO2, flood_flag, rcvl,
                                  rcl, rcr, st, rcnrvl, rcnrl, rcnrr,
                                  MicrobioIndex, ch4, WTL, day_WT, soil_ph, 
                                  doc, nh4, &ch4_prod, &ch4_day_oxid, &plant_day_ch4, 
                                  day_wfps, seed, &day_ch4, anvf, WaterLeakRate, h, CRB1, rcnb, 
                                  co2, crhl, crhr, air_temp, Eh1, ch4_doc_pool, F_methanogen,
                                  WaterControl, Tttoo, jday, rcnh2, rcnh, H2, day_ch4_CO2, 
                                  day_ch4_1, day_ch4_2, SO4, day_root_resp);
//day_ch4 = 0.0;
                day_balance_ch4 += day_ch4;
            }
        }
    }
    for ( l = 1; l <= q; l++ ) 
    {
            day_co2 += co2[l];
            co2[l] = 0.0;
    }
    
    for ( l = 1; l <= q; l++ ) 
    {
            if(ice[l]>0.0||temp[l]<=0.0||WTL<l)
            {
                if(co2[l]>0.0 && nh4[l]>0.0)
                {
                    float MBIO;
                    if(co2[l]<= nh4[l]*rcnb) 
                    {
                        MBIO = co2[l];
                        nh4[l] -= (co2[l]/rcnb);
                        co2[l] = 0.0;
                    }
                    else 
                    {
                        MBIO = nh4[l]*rcnb;
                        co2[l] -= (nh4[l]*rcnb);
                        nh4[l] = 0.0;
                    }

                    CRB1[l] += MBIO;
                }
            }
    }
    soil_hete_CO2 = day_co2;// - day_root_resp;	
    Psn = dPlantBiomass + sum_shoot_CO2 + sum_stem_CO2 + day_root_resp;//day_RootCO2;
    EcoCO2 = soil_hete_CO2 + sum_shoot_CO2 + sum_stem_CO2 + day_root_resp;//day_RootCO2;
    NEE = soil_hete_CO2 - dPlantBiomass;
    
    Eh000 = Eh[3];

    NO3_leaching();

    // Impact of soil frost 
    //soil_freeze();

    if(jday==ReDefineIONday&&UseION==1) 
        ReDefineION();	

    // Daily summary and record	
    
    EndWaterPool = WaterPool;

    //water_balance();

    if ( jday == 365) yr_end_water = end_soil_water;

    // C/N pools at the end of each day 
    cn_pools(w_no3, w_nh4, w_nh3, wurea, wpool_no, w_n2o, w_n2);

#ifdef BALANCE
    CheckBalance(2); //reserved
#endif
    if(DroughtID==1&&year==2&&(drc>=1&&drc<=3)&&IrriType==0)
    {
            // Write daily soil climate
            write_out_days(fs, DayPET);
            // Write daily crop parameters
            write_out_dayp(fcrop);
    }
    else
    {
        if (scale == 0 && day_file == 1)
        {
            soc_distribution(CRB1, CRB2, newh );

            write_out_dayClim(fclim, jday, site, year, air_temp, precipitation, wind_speed, PAR, humidity);

            // Write daily water budget
            write_out_dayw(fw, DayPET);

            // Write daily soil climate
            write_out_days(fs, DayPET);

            // Write daily C pools/fluxes
            write_out_dayc(fc );

            // Write daily N pools/fluxes
            write_out_dayn(fn );

            write_out_dayg(fg );
            // Write daily crop parameters
            write_out_dayp(fcrop);

            write_out_daym(fma);

            write_out_daysp(fsp);

            write_out_daymicrobe(fmb);
            
#ifdef ISOTOPE
            write_out_dayisotope(fis);
#endif

#ifdef BALANCE
            //write_out_C, N balance;
            write_out_dayCB(fCB, fNB);
#endif

#ifdef HOURLY
            //write_out_HrSoilMoist(HSM);
            //write_out_HrSoilN(HSM, T);
#endif

#ifdef HYDRO
            if (jday==1)
            {
                fprintf(fmusle, "Daily musle result in year %d\n", year);
                fprintf(fmusle, "Just calculate surface runoff in site scale\n");
                fprintf(fmusle, "Day,runoff(mm),sedyld(ton),socero(kgC/ha),sonero(kgN/ha),nh4ero(kgN/ha),no3ero(kgN/ha),nh3ero(kgN/ha),ureaero(kgN/ha),claynero(kgN/ha)\n");
            }
            fprintf(fmusle, "%3d,", jday);
            fprintf(fmusle, "%7.3f,%7.5f,%7.5f,%7.5f,%7.5f,%7.5f,%7.5f,%7.5f,%7.5f\n",day_runoff,sedyld,socmusle,sonmusle,nh4musle,no3musle,nh3musle,ureamusle,claynmusle);
#endif


        }
        else
        {
            if(IFDAILY==1)
            {
#ifdef DROUGHT
                RDbiomas[jday]=Leaf_Wt[1]+Leaf_Wt[2]+Leaf_Wt[3]+Stem_Wt[1]+Stem_Wt[2]+Stem_Wt[3];			
                RDwaterstress[jday]=day_DWater[1];
                RDtotalwater[jday]=day_SoilWater*1000;
                RDwater1[jday]=day_wfps[LL1];
                RDwater2[jday]=day_wfps[LL2];
                RDwater3[jday]=day_wfps[LL3];
                RDwater4[jday]=day_wfps[LL4];
                RDwater5[jday]=day_wfps[LL5];
                RDtrans[jday]= day_transp*1000.0;
                RDevap[jday]= (day_soil_ev+day_pond_ev)*1000.0;
                RDleach[jday]=Grain_Wt[1]+Grain_Wt[2]+Grain_Wt[3];				
                RDirri[jday]=(day_irri+precipitation)*1000.0;

#else
                RDco2[jday]=day_co2+ (day_rco2[1]+day_rco2[2]+day_rco2[3]);
                RDn2o[jday]=day_soil_an_n2o;
                RDno[jday] =day_soil_an_no;
                RDch4[jday]=day_ch4;	
                RDnh3[jday]=(day_soil_nh3+day_vol_nh3);	
                RDLeachN[jday]=(day_leach_NO3+by_pass_no3);			
                RDbiomas[jday]=Leaf_Wt[1]+Leaf_Wt[2]+Leaf_Wt[3]+Stem_Wt[1]+Stem_Wt[2]+Stem_Wt[3]+Grain_Wt[1]+Grain_Wt[2]+Grain_Wt[3];			
                RDdSOC[jday]= day_dSOC;			
                RDtrans[jday]= day_transp*1000.0;
                RDevap[jday]= (day_soil_ev+day_pond_ev)*1000.0;
                RDleach[jday]=day_leach_water * 1000.0;				
                RDirri[jday]=(day_irri+precipitation)*1000.0;
#endif

#ifdef HYDRO//=== Record daily result at region scale;dengjia===
               RDmuslerunoff[jday]=day_runoff;
               RDmuslesedyld[jday]=sedyld;
               RDmuslesoc[jday]=socmusle;
               RDmusleson[jday]=sonmusle;
               RDmuslenh4[jday]=nh4musle;
               RDmusleno3[jday]=no3musle;
               RDmuslenh3[jday]=nh3musle;
               RDmusleurea[jday]=ureamusle;
               RDmusleclayn[jday]=claynmusle;
#endif
            }
        }
    }

#ifdef HYDRO
    scs_runoff=0;
    sedyld=0;
#endif	

#ifdef DEGUI
#else
    //if(Monitor==0) paint(pLinkInfo, scale, ViewMode, cropping_system);//++
#endif

    // Annual accumulation 
    if(Option>100&&Option<100000) //Monte Carlo
    {
        if(jday>=MC_StartDay&&jday<=MC_EndDay) MC_annu_accumu(); 
    }
    else annu_accumu();
    
    //season_N_stress += N_stress[cwc];

// changed liujm
#ifdef DEGUI
#else
//    CWnd* pDC = pLinkInfo->pWnd;
//    CDC& dc = *(pLinkInfo->pDC);
    
    MSG message;
    if (::PeekMessage(&message,NULL,0,0,PM_REMOVE)) {
        ::TranslateMessage(&message);
        ::DispatchMessage(&message);
    }
#endif
    
} // daily loop end
fclose( fday );

if(WaterControl==2) fclose(fWT);
//if(mm_ProductionOption==1) fclose(fLV);

if(DroughtID==1)
{
    if(year==2&&(drc>=1&&drc<=3)&&IrriType==0)
    {
        fclose( fs );
        fclose( fcrop );
    }
}
else
{
    if (scale == 0 && day_file == 1)
    {

    fclose( fclim );
    fclose( fw );
    fclose( fs );
    fclose( fc );
    fclose( fn );
    fclose( fg );
    fclose( fcrop );
    fclose( fma );
    fclose( fsp );
    fclose( fmb );

#ifdef ISOTOPE
    fclose( fis );
#endif

#ifdef BALANCE
    fclose( fCB );
    fclose( fNB );
#endif

#ifdef HOURLY
    fclose( HSM );
#endif

#ifdef HYDRO
   fclose(fmusle);
#endif
    }
}

if(fer_num==-1)
{
    fclose(fertg);
}
    // SOC distribution in profile 
    soc_distribution(CRB1, CRB2, newh );
    // Write soil pools on day 365 
    write_out_soil_parm(scale, SoilYear, SimuYears);//+
    // Write annual reports
    //if(GlobeData==0) 
    if(DroughtID==0)
    {
        if(scale==0)
        {
            write_out_annual_report(scale, ha, Batch, livestock, fout, fout2, FarmField, FarmCropArea, FarmCropManureFraction, drc, Pass, PassC1, PassL1);
        }
    }
#ifdef DEGUI
#else
    if(MonteCarlo==1)
    {
        write_MonteCarlo(sample, CountyIDX, cropping_system);
    }

    if(Batch==1)
    {
        write_BatchSummary(sample, site, PassC1);
    }
#endif

if(scale==0 || SimuYears>1)
{
    // Write crop conditions on day 365 
    err_no = write_out_cropini(scale );//+
    if ( err_no ) return( err_no );
}
if(scale==1)
{
    if(SoilYear==999)
    {
        //static int sample = 0;
        //write_MonteCarlo(sample, CountyIDX, cropTYPE);
        //sample++;
    }
    else
    {
        if(DroughtID==0)
        {
            write_globe_summary(SoilYear, ha, cropping_system, province, county, CountyIDX, 
                r_Country, r_Scenario, LandOrder, year, StartYear, r_Province, 1, 
                Batch, GlobeData, RRRRR, IrriType, country_ID, livestock, DroughtID, drc, 
                f_region, DroughtYield, SoilDataUse, FFN);

        }
        else
        {
            if(year==2)
            {
                write_globe_summary(SoilYear, ha, cropping_system, province, county, CountyIDX, 
                    r_Country, r_Scenario, LandOrder, year, StartYear, r_Province, 1, 
                    Batch, GlobeData, RRRRR, IrriType, country_ID, livestock, DroughtID, drc, 
                    f_region, DroughtYield, SoilDataUse, FFN);

            }
        }

        if(DailyRecord==1) 
        {
            //DailyDataRead2(year, LandOrder, SoilYear, ha, IrriType);
        }

#ifdef HYDRO
        //MonthData(year, LandOrder, SoilYear, ha, IrriType);
#endif
    }
    
}
else
{
    //Write annual water budget
    err_no = write_out_wb(Batch, Pass, PassC1);
    if ( err_no ) return( err_no );

}

    MSG message;
    if (::PeekMessage(&message,NULL,0,0,PM_REMOVE)) 
    {
        ::TranslateMessage(&message);
        ::DispatchMessage(&message);
    }

//}//Manure farm cropfield loop

if(Batch==1)
{
    fclose(fout);
}
else
{
    if(DroughtID==0)
    {
        if(scale==0)
        {
            fclose(fout);
        }
    }
}

#ifdef MANURE
fclose(fout2);
fclose(ftf);
#endif


return(0);
}

int class_model::DNDC101go(MODEL_LINK_INFO* pLinkInfo, int MODE, int SoilYear, float ha, 
                           char* cropping_system, int province, int county, int CountyIDX, 
                           int LandOrder, int MonteCarlo, int scale, int S_ThisYear,
                           char* r_Province, int ZX, int Batch, int sample, int RRRRR, 
                           int IrriType, char* country_ID, int livestock, int Monitor, int drc,
                           int DroughtID, char f_region[6][150], char* DroughtYield, int FFN, int FarmField)
{
    FILE   *fc=NULL, *fn=NULL, *fw=NULL, *fcrop=NULL, *fEh=NULL, *fday=NULL,*fpp=NULL,*fWT=NULL;
    FILE   *fNB=NULL, *fCB=NULL, *fLAI=NULL, *fs=NULL, *fg=NULL, *fma=NULL, *HSM=NULL, *fsp=NULL, *fmb=NULL, *fis=NULL;
    FILE   *ftf1=NULL,*ftf2=NULL,*ftf3=NULL,*ftf4=NULL,*ftf5=NULL,*ftf6=NULL, *fclim=NULL, *ftf=NULL;
    float  wrcvl01, wrcl01, wrcr01, wsoc01, wn2o01, wno01, wn201, wdoc01, winertC01, wcrb01, wcrhl01, wcrhr01;
    float  whumus01, wno201, wno301, wnh401, wnh301, wurea01, day_clay_n01;
    float till_fact, w_no2, w_nh3, w_n2o, w_n2, ttoo[90];
    float w_no3, w_nh4, wurea, wpool_no, OverT, wpool_no2, day_clayn;
    float dby_flux, jf, hrPE0, S_CO2IncreaseRate, AddC1, AddC2, AddC3, litco22, litco23, FM_C[7], FM_N[7], FM_H2O[7];
    float FarmCropArea[10]={0.0,0.0}, FarmCropManureFraction[10]={0.0,0.0};
    
    int i, l, err_no, TTTFlag, tilq, junkd, ShallowFlood, SelectedCrops, SC[150], GlobeData, StartGridID;
    int TotalCells, SimulatedCells, TotalManureCrops, ManureCropOrder, CropIDID, GN=0, FN=0;
    int FarmCropID[10], ETM, YieldSite, YieldCrop, YieldYear, YieldToday, SoilDataUse;
    
    char DAYW[90], DAYC[90], DAYN[90], DAYP[90], DAYS[90], DAYM[90],DAYnBalance[90],DAYSP[90],DAYMB[90],DAYIS[90];
    char js[120],IE[80], F_CLIMD[300], F_WT[150], F_LV[15], Zz[200], DAYclim[90];
    char DAYcBalance[90], DAYG[90], Pass[100];//, BatchFileName[300];
    char HrSoilMoist[100], YieldCountyName[200], LB[200];
    
#ifdef MANURE
    
    //#ifdef FACILITY
    ///////////// Single farm facility simulation
    
    int FSF=0;
    char SYF[300]
    FILE* sni;
    sprintf(SYF, "%s\\Inter\\SoilNewIni", OUTPUT);
    sni=fopen(SYF, "r");
    if(sni==NULL) note(0, SYF);
    fscanf(sni, "%d", &FSF);
    fclose(sni);
    
    if(FSF!=1)
    {
        if(FSF==3)//farm composit
        {
            FarmFacilityCompost(HSM);
            //exit(0);
        }
        else if(FSF==4)//farm lagoon
        {
            FarmFacilityLagoon();
            //goto EndFacility1;//exit(0);
        }
        else 
        {
            exit(0);
        }
    }
    //#endif
#endif
    
    AfxMessageBox("Facility simulation is done");
    
    return(0);
}

int Depth_convert_to_layer(float DepthO, float *hh, int q)
{
    int LayerN, Qflag=0;

    float Tk=0.0;
    for(int i=1;i<=1000;i++)
    {
        if(i<=q) Tk += hh[i];
        else Tk += hh[q];
        if(Tk>=DepthO)
        {
            LayerN = i-1;
            Qflag=1;
            break;
        }
    }

    if(Qflag==0) LayerN = q;
    return(LayerN);
}