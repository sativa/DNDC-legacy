#include "dndc_InputFile.h"
#include "RunPaths.h"
#include "DNDC_Interface.h"
#include <io.h>
#include <direct.h>
#include <math.h>
#include <vector>
#include <iostream>

double nSoca[6]={0.0,0.2,0.2,0.08,0.05,0.2};
double nSocb[6]={10.0,2.0,2.0,1.4,4.0,2.0};
int NutritionModelX;

void SaveDatatoFile(
    const char* outputPath,
    int mode,
    Site_BaseInformation IData )
{
    /*
    char* fname = new char[outputPath.size()+1];
    std::copy( outputPath.begin(), outputPath.end(), fname );
    fname[outputPath.size()] = '\0';
    */

    char* fname = new char[200];
    strcpy( fname, outputPath );
    //int i, j, k, n, CropModel, rrr, NNN;

    FILE *fpo;
    char Oname[255];

    /*
    const char* XFF;
    FILE *xff;

    const char* CM;
    CM = ( (std::string)OUTPUT + "\\Inter\\CropModel" ).c_str();
    FILE *cm;
    //CM.Format( "%s\\Inter\\CropModel", OUTPUT );
    cm=fopen(CM, "r");
    if(cm==NULL)
    {
        std::cout << "Cannot open file " << CM << std::endl;
        exit(0);
    }
    else
    {
        fscanf(cm, "%d", &CropModel);
        fclose(cm);
        IData.CropModel = CropModel;
    }
    */

    if(IData.Sks<0.008) IData.Sks = 0.008;
    
    sprintf(Oname,"%s.fn",fname);
    
    if ( (fpo=sopen(fname,0) )==NULL ) return;//write
    
        // Save site
    fprintf(fpo,"Input_Parameters:\n");
    fprintf(fpo,"----------------------------------------\n");
    fprintf(fpo,"Site_info:\n");
    fprintf(fpo,"	Site_name:                  %s\n", IData.sitename);
    fprintf(fpo,"	Simulated_Year:             %d\n",IData.years);
    fprintf(fpo,"	Latitude:                   %6.3f\n",IData.latitude);
    fprintf(fpo,"	Daily_Record:               %6d\n",IData.ifdaily); 
    //fprintf(fpo,"Climate_file_mode           %6d\n",IData.if1File); 

#ifdef MANURE

    char ManureHouseName[300], ManurePenName[300], IndoorClimateFile[300];
    int AnimalType, m_ProductionOption, m_GroundSurface, m_Ventilation, m_Bedding, InputFormat, AnimalTypes;
    int m_BedType, BedMonth[366], BedDay[366];
    float m_BedApplications, m_BedCN, m_BedAmount, m_VentRate, m_AnimalHeads, m_FeedRate, m_FeedProtein, m_FeedTP, m_GroundArea;

    int PenAnimalType, PenGroundSurface, PenInputFormat, PenAnimalTypes;
    int PenBedType, PenBedApplications, PenBedMonth[366], PenBedDay[366];
    float PenAnimalHeads, PenBedCN, PenBedAmount, PenFeedRate, PenFeedProtein, PenFeedTP, PenGroundArea;
    
    const char* ManureFieldName;
    int FieldInputFormat, FieldAnimalType, FieldGrazeTimes, IniMonth[366], IniDay[366], EndMonth[366], EndDay[366];
    float FieldHeads, FieldHrs, FieldArea;

    float LagoonCapacity, LagoonSurfaceArea, LagoonDays;
    float LagoonToField[10]={0.0,0.0}, LagoonToMarket[10]={0.0,0.0}, LagoonToRemain[10]={0.0,0.0};
    int LagoonCover, LagoonRain, LagoonDrainTimes, LagoonDrainMonth[10]={0,0}, LagoonDrainDay[10]={0,0}; 

    float m_CompostDensity, m_LitterAmount, m_LitterCN, m_ToField, m_ToMarket, m_Remain;
    float m_DigesterCH4, m_DigesterDays, m_DigesterToField, m_DigesterToMarket, m_DigesterRemain;
    int m_Temp, m_Flash, m_CompostCover, m_CompostRemoveTimes;
    int m_CompostRemoveMonth, m_CompostRemoveDay;

    float m_Frequency, m_UC, m_UL, m_UD, m_UF, m_UR, m_FC, m_FL, m_FD, m_FF, m_FR;
    float m_Frequencyp, m_UCp, m_ULp, m_UDp, m_URp, m_FCp, m_FLp, m_FDp, m_FRp;
    float m_Frequencyf, m_USf, m_URf, m_FSf, m_FRf, m_FieldArea;
    
    fprintf(fpo,"----------------------------------------\n");
    fprintf(fpo,"Livestock_data:\n");		
    fprintf(fpo,"SimulatedYears				%d\n",IData.years);

    for(int m_FeedlotYear=1; m_FeedlotYear<=IData.years; m_FeedlotYear++)
    {

        int m_FeedLots;
        const char* TotalFeedlots;
        FILE *TFL;
        TotalFeedlots.Format("%s\\Result\\inter\\TotalFeedLots_%d", ROOTDIR, m_FeedlotYear);
        TFL=fopen(TotalFeedlots, "r");
        if(TFL==NULL) note(0, TotalFeedlots);
        fscanf(TFL, "%d", &m_FeedLots);
        fclose(TFL);

        fprintf(fpo,"TotalFeedlots				%d\n",m_FeedLots);

        for(i=1; i<=m_FeedLots; i++)
        {
            fprintf(fpo,"Feedlot					%d\n", i);

            CM.Format("%s\\Inter\\ManureHouse_%d_%d.txt", OUTPUT, m_FeedlotYear, i);
            cm=fopen(CM, "r");
            if(cm==NULL) note(0, CM);
        
            fscanf(cm, "%d", &InputFormat);
            fprintf(fpo,"   FeedlotInputFormat			%d\n",InputFormat);
            if(InputFormat==1)
            {
                //Open a file to read ferd, feed data
                fscanf(cm, "%s", ManureHouseName);
                fprintf(fpo,"   HouseInputFileName			%s\n",ManureHouseName);
            }
            else if(InputFormat==2)
            {
                //Open a file to read ferd, feed data
                fscanf(cm, "%s", ManureHouseName);
                fprintf(fpo,"   HouseInputFileName			%s\n",ManureHouseName);
            }
            else if(InputFormat==3)
            {
                //Open a nutrition model
                fscanf(cm, "%d", &NutritionModelX);
                fprintf(fpo,"   HouseInputFileName			%d\n",NutritionModelX);
            }
            else
            {
                fscanf(cm, "%d", &AnimalTypes);
                fprintf(fpo,"   LivestockType			%d\n",AnimalTypes);
                
                fscanf(cm, "%d %f %f %f %f", &AnimalType, &m_AnimalHeads, &m_FeedRate, &m_FeedProtein, &m_FeedTP);
                fprintf(fpo,"   LivestockType_ID			%d\n",AnimalType);
                fprintf(fpo,"   Herd_heads				%f\n", m_AnimalHeads);
                fprintf(fpo,"   Feed:	\n");
                fprintf(fpo,"      Rate				%f\n", m_FeedRate);
                fprintf(fpo,"      CrudeProtein_percent		%f\n", m_FeedProtein);
                fprintf(fpo,"      Phosphorus_percent		%f\n", m_FeedTP);			
            }
            
            fscanf(cm, "%f %d", &m_GroundArea, &m_GroundSurface);//0 slatted floor; 1 conrete floor
            fscanf(cm, "%d  %f  %f %f", &m_BedType, &m_BedCN, &m_BedApplications, &m_BedAmount);//0 None, 1 straw, 2 sawdust, 3 dry manure solids, 4 sand
            fprintf(fpo,"   Floor:	\n");
            fprintf(fpo,"      SurfaceArea			%f\n", m_GroundArea);
            fprintf(fpo,"      SurfaceType			%d\n", m_GroundSurface);
            fprintf(fpo,"   Bedding:	\n");
            fprintf(fpo,"      MaterialType			%d\n", m_BedType);
            fprintf(fpo,"      Material_C/N			%f\n", m_BedCN);
            fprintf(fpo,"      ApplyFrequency			%f\n", m_BedApplications);
            fprintf(fpo,"      Amount				%f\n", m_BedAmount);			
                
            fscanf(cm, "%d  %f  %s", &m_Ventilation, &m_VentRate, IndoorClimateFile);// 0 natural, 1 fan, -1 IndoorClimateFile
            fprintf(fpo,"   Ventilation:	\n");
            fprintf(fpo,"      Type				%d\n", m_Ventilation);
            fprintf(fpo,"      Ven_rate				%f\n", m_VentRate);
            fprintf(fpo,"      IndoorClimateFile			%s\n", IndoorClimateFile);

            fscanf(cm, "%f", &m_Frequency);
            fscanf(cm, "%f %f %f %f %f", &m_UC, &m_UL, &m_UD, &m_UF, &m_UR);
            fscanf(cm, "%f %f %f %f %f", &m_FC, &m_FL, &m_FD, &m_FF, &m_FR);
            fscanf(cm, "%d", &m_Flash);

            fprintf(fpo,"   ManureRemoval:	\n");
            fprintf(fpo,"      Frequency				%f\n", m_Frequency);
            fprintf(fpo,"      Liquid_to_compost			%f\n", m_UC);
            fprintf(fpo,"      Liquid_to_lagoon			%f\n", m_UL);
            fprintf(fpo,"      Liquid_to_digester		%f\n", m_UD);
            fprintf(fpo,"      Liquid_to_field			%f\n", m_UF);
            fprintf(fpo,"      Liquid_remain			%f\n", m_UR);
            fprintf(fpo,"      Solid_to_compost			%f\n", m_FC);
            fprintf(fpo,"      Solid_to_lagoon			%f\n", m_FL);
            fprintf(fpo,"      Solid_to_digester			%f\n", m_FD);
            fprintf(fpo,"      Solid_to_field			%f\n", m_FF);
            fprintf(fpo,"      Solid_remain 			%f\n", m_FR);

            fprintf(fpo,"   Flushing:	\n");
            fprintf(fpo,"      FlushingLiquidType		%d\n",  m_Flash);
            
            fclose(cm);
        }

    }

    CM.Format("%s\\Inter\\ManureLagoon.txt", OUTPUT);
    cm=fopen(CM, "r");
    if(cm==NULL) note(0, CM);
    
    fscanf(cm, "%f %f %d %d %d", &LagoonCapacity, &LagoonSurfaceArea, &LagoonCover,	&LagoonRain, &LagoonDrainTimes);
    fprintf(fpo,"Lagoon				\n");
    fprintf(fpo,"   Capacity				%f\n", LagoonCapacity);
    fprintf(fpo,"   SurfaceArea				%f\n", LagoonSurfaceArea);
    fprintf(fpo,"   Coverage				%d\n", LagoonCover);
    fprintf(fpo,"   RainfallReceival		%d\n", LagoonRain);
    fprintf(fpo,"   SlurryRemovalTimes		%d\n", LagoonDrainTimes);

    for(i=1; i<=LagoonDrainTimes; i++)
    {
        fscanf(cm, "%d %d %f %f %f", &LagoonDrainMonth[i], &LagoonDrainDay[i], &LagoonToField[i], &LagoonToMarket[i], &LagoonToRemain[i]);
    
        fprintf(fpo,"   Slurry_drain_month		%d\n", LagoonDrainMonth[i]);
        fprintf(fpo,"   Slurry_drain_day		%d\n", LagoonDrainDay[i]);
        fprintf(fpo,"   Slurry_to_field			%f\n", LagoonToField[i]);
        fprintf(fpo,"   Slurry_to_digester		%f\n", LagoonToMarket[i]);
        fprintf(fpo,"   Slurry_remain			%f\n", LagoonToRemain[i]);
    }	
    
    fclose(cm);

    const char* SF;
    FILE* sf;
    SF.Format("%s\\Inter\\ManureCompost.txt", OUTPUT);
    sf=fopen(SF, "r");
    if(sf==NULL) note(0, SF);
    fscanf(sf, "%f  %d  %d  %f  %f", &m_CompostDensity, &m_CompostCover, &m_CompostRemoveTimes, &m_LitterAmount, &m_LitterCN);    
        
    fprintf(fpo,"Compost				\n");
    fprintf(fpo,"   Porosity				%f\n", m_CompostDensity); 
    fprintf(fpo,"   Coverage				%d\n", m_CompostCover);
    fprintf(fpo,"   RemovalTimes			%d\n", m_CompostRemoveTimes); 
    fprintf(fpo,"   AdditiveAmount			%f\n", m_LitterAmount); 
    fprintf(fpo,"   Additive_C/N				%f\n", m_LitterCN);
    
    for(i=1;i<=m_CompostRemoveTimes;i++)
    {
        fscanf(sf, "%d %d %f  %f  %f", &m_CompostRemoveMonth, &m_CompostRemoveDay, &m_ToField, &m_ToMarket, &m_Remain);
        fprintf(fpo,"   Remove_month			%d\n", m_CompostRemoveMonth);
        fprintf(fpo,"   Remove_Day				%d\n", m_CompostRemoveDay);		
        fprintf(fpo,"   Manure_to_field			%f\n", m_ToField);
        fprintf(fpo,"   Manure_to_market		%f\n", m_ToMarket);
        fprintf(fpo,"   Manure_remain			%f\n", m_Remain);
    }
    fclose(sf);
    
    SF.Format("%s\\Inter\\ManureDigester.txt", OUTPUT);
    sf=fopen(SF, "r");
    if(sf==NULL) note(0,SF);
    fscanf(sf, "%d  %f  %f", &m_Temp, &m_DigesterCH4, &m_DigesterDays);    
    fscanf(sf, "%f  %f  %f", &m_ToField, &m_ToMarket, &m_Remain);	
    
    fprintf(fpo,"AnaerobicDigester				\n");
    fprintf(fpo,"   Thermo_type				%d\n", m_Temp); 
    fprintf(fpo,"   MethaneProductivity			%f\n", m_DigesterCH4);
    fprintf(fpo,"   RetentionDays			%f\n", m_DigesterDays);
    fprintf(fpo,"   Manure_to_field			%f\n", m_ToField);
    fprintf(fpo,"   Manure_to_lagoon			%f\n", m_ToMarket);
    fprintf(fpo,"   Manure_remain			%f\n", m_Remain);
    fclose(sf);

    CM.Format("%s\\Inter\\ManureField.txt", OUTPUT);
    sf=fopen(CM, "r");
    if(sf==NULL) note(0,CM);
    fscanf(sf, "%f", &m_FieldArea);
    fclose(sf);

    if(m_FieldArea<=0.0) m_FieldArea = 1.0;
    
    fprintf(fpo,"LandApplication				\n");
    fprintf(fpo,"   Land_area				%f\n", m_FieldArea);

    int ManuCropType, mkk, jd;
    float ManuCropArea, ManuFraction;
    
    XFF.Format("%s\\inter\\ManureFieldCrops", OUTPUT);		
    xff=fopen(XFF, "r");
    if(xff==NULL) note(1,XFF);
    fscanf(xff, "%d", &mkk);
    fprintf(fpo,"   CroppingSystems			%d\n", mkk);
    for(i=1;i<=mkk;i++)
    {
        fscanf(xff, "%d  %d  %f  %f", &jd, &ManuCropType, &ManuCropArea, &ManuFraction);
        fprintf(fpo,"      CroppingSystem			%d\n", i);
        fprintf(fpo,"      CropType				%d\n", ManuCropType);
        fprintf(fpo,"      FieldArea				%f\n", ManuCropArea);
        fprintf(fpo,"      ManureShareFraction		%f\n", ManuFraction);
    }
    fclose(xff);
    
#endif
        // Climate data
    fprintf(fpo,"----------------------------------------\n");
    fprintf(fpo,"Climate_data:\n");
    fprintf(fpo,"	Climate_Data_Type:          %d\n",IData.ClimateFileType);
    fprintf(fpo,"	NO3NH4_in_Rainfall          %8.4f\n",IData.NO3NH4_Rainfall);
    fprintf(fpo,"	NH3_of_Atmosphere           %8.4f\n",IData.NH3_Atmosphere);
    fprintf(fpo,"	BaseCO2_of_Atmosphere       %8.4f\n",IData.BaseCO2);
    fprintf(fpo,"	Climate_file_count=         %d\n",IData.ClimateFileNumber);
    for (int i = 0; i<IData.ClimateFileNumber;i++) {
        fprintf(fpo,"    %d   %s\n",i + 1, IData.ClimateFilenames[i] );
    }
    fprintf(fpo,"	Climate_file_mode           %6d\n",IData.if1File);//new
    fprintf(fpo,"	CO2_increase_rate           %f\n",IData.CO2IncreaseRate);//new
    
        // Soil parameter
    fprintf(fpo,"----------------------------------------\n");
    fprintf(fpo,"Soil_data:\n");
    fprintf(fpo,"	Soil_Texture             %2d\n",IData.Soil_Texture);
    fprintf(fpo,"	Landuse_Type             %2d\n",IData.Soil_landuse);
    fprintf(fpo,"	Density                  %8.5f\n",IData.Soil_Density);
    fprintf(fpo,"	Soil_pH                  %8.5f\n",IData.Soil_pH);
    fprintf(fpo,"	SOC_at_Surface           %8.5f\n",IData.Soil_OCatSurface); 
    fprintf(fpo,"	Clay_fraction            %8.5f\n",IData.Soil_Clay);
    fprintf(fpo,"	BypassFlow               %8.5f\n",IData.Soil_BypassFlow);
    fprintf(fpo,"	Litter_SOC               %8.5f\n",IData.Soil_Litter);
    fprintf(fpo,"	Humads_SOC               %8.5f\n",IData.Soil_humads);
    fprintf(fpo,"	Humus_SOC                %8.5f\n",IData.Soil_humus);
    fprintf(fpo,"	Soil_NO3(-)(mgN/kg)      %8.5f\n",IData.Soil_NO3);
    fprintf(fpo,"	Soil_NH4(+)(mgN/kg)      %8.5f\n",IData.Soil_NH4);
    fprintf(fpo,"	Soil_PassiveCN           %8.5f\n",IData.PassiveCCN);
    fprintf(fpo,"	Lateral_influx_index     %8.5f\n",IData.LateralInflux);
    fprintf(fpo,"	Field_capacity           %f\n",IData.Fldcap); 
    fprintf(fpo,"	Wilting_point            %f\n",IData.Wiltpt); 	
    fprintf(fpo,"	Hydro_conductivity       %f\n",IData.Sks);
    fprintf(fpo,"	Soil_porosity            %f\n",IData.Soil_Porosity);	
    fprintf(fpo,"	SOC_profile_A            %f\n",IData.SOCa);
    fprintf(fpo,"	SOC_profile_B            %f\n",IData.SOCb);
    fprintf(fpo,"	DC_litter_factor         %f\n",IData.DClitter);
    fprintf(fpo,"	DC_humads_factor         %f\n",IData.DChumads);
    fprintf(fpo,"	DC_humus_factor          %f\n",IData.DChumus);
    fprintf(fpo,"	Humad_CN                 %f\n",IData.HumadCN);
    fprintf(fpo,"	Humus_CN                 %f\n",IData.HumusCN);
    fprintf(fpo,"	Soil_PassiveC            %f\n",IData.PassiveC);
    fprintf(fpo,"	Soil_microbial_index     %f\n",IData.Soil_MicrobioIndex);	
    fprintf(fpo,"	Highest_WT_depth         %f\n",IData.HighestWT);
    fprintf(fpo,"	Depth_WRL_m              %f\n",IData.Soil_WaterObstructLayer);
    fprintf(fpo,"	Slope                    %f\n",IData.slope);  
    fprintf(fpo,"	Salinity                    %f\n",IData.Soil_Quality);  
    fprintf(fpo,"	SCS_curve_use               %d\n",IData.SCSuse);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	const char* FCT30;
    int TotalManureCrops;
     
#ifdef MANURE	
            
    XFF.Format("%s\\ManureCropNumber", INTER);		
    xff=fopen(XFF, "r");
    if(xff==NULL) note(0,XFF);
    fscanf(xff, "%d", &TotalManureCrops);
    fclose(xff);

    //fprintf(fpo,"Farm_mode					%d %d\n",1, TotalManureCrops);
#else
    TotalManureCrops = 1;		
    //fprintf(fpo,"Farm_mode					%d %d\n",0, 1);
#endif


for(int CSL=1; CSL<=TotalManureCrops; CSL++)
{
    /*
#ifdef MANURE
    FCT30.Format("%s_%d", INTERMANAGE, CSL);
#else
    //FCT30.Format("%s_1", INTERMANAGE);
    FCT30 = ( (std::string)INTERMANAGE + "_1" ).c_str();
#endif

    int yyy;
    char DB[300];
    FILE *db;
    
    sprintf(DB, "%s\\CropRotation.txt", FCT30);
    db=fopen(DB, "r");
    if(db==NULL) note(0,DB);
    fscanf(db, "%d", &IData.db_Rotation_Number);
    fclose(db);
    
    for(rrr=1; rrr<=IData.db_Rotation_Number; rrr++)
    {
        
        sprintf(DB, "%s\\CropRotation_%d.txt", FCT30, rrr);//read: SaveDatatoFile
        db=fopen(DB, "r");
        if(db==NULL) note(0,DB);
        fscanf(db, "%d %d %d", &IData.db_RotationID[rrr], &IData.db_RotateSpan[rrr], &IData.db_CycleSpan[rrr]);
        fclose(db);
        
        for(yyy=1; yyy<=IData.db_CycleSpan[rrr]; yyy++)
        {
            //DB.Format("%s\\inter\\CropRotation_%d_%d.txt", OUTPUT, rrr, yyy);
            sprintf(DB, "%s\\CropRotation_%d_%d.txt", FCT30, rrr, yyy);
            db=fopen(DB, "r");
            if(db==NULL) note(0,DB);
            fscanf(db, "%d", &IData.db_YrCrops[rrr][yyy]);

            for(int ccc=1; ccc<=IData.db_YrCrops[rrr][yyy]; ccc++)
            {
                    fscanf(db, "%d %d", &IData.db_YrCropOrder[rrr][yyy][ccc], &IData.db_YrCropType[rrr][yyy][ccc]);
                    fscanf(db, "%d %d", &IData.db_PMonth[rrr][yyy][ccc], &IData.db_PDay[rrr][yyy][ccc]);
                    fscanf(db, "%d %d", &IData.db_HMonth[rrr][yyy][ccc], &IData.db_HDay[rrr][yyy][ccc]);
                    fscanf(db, "%d", &IData.db_HYr[rrr][yyy][ccc]);
                    fscanf(db, "%f %f", &IData.db_Residue[rrr][yyy][ccc], &IData.db_Yield[rrr][yyy][ccc]);
                    fscanf(db, "%f %f", &IData.db_RepRate[rrr][yyy][ccc], &IData.db_VegRate[rrr][yyy][ccc]);
                    fscanf(db, "%f %f", &IData.db_PsnEff[rrr][yyy][ccc], &IData.db_PsnMax[rrr][yyy][ccc]);
                    fscanf(db, "%f", &IData.db_TreeAge[rrr][yyy][ccc]);
                    fscanf(db, "%d", &IData.db_CoverCrop[rrr][yyy][ccc]);
                    fscanf(db, "%d", &IData.db_PerennialCrop[rrr][yyy][ccc]);

                    fscanf(db, "%f %f %f %f", &IData.db_GrainFraction[rrr][yyy][ccc], &IData.db_LeafFraction[rrr][yyy][ccc], &IData.db_ShootFraction[rrr][yyy][ccc], &IData.db_RootFraction[rrr][yyy][ccc]);
                    fscanf(db, "%f %f %f %f", &IData.db_GrainCN[rrr][yyy][ccc], &IData.db_LeafCN[rrr][yyy][ccc], &IData.db_ShootCN[rrr][yyy][ccc], &IData.db_RootCN[rrr][yyy][ccc]);
                    fscanf(db, "%f %f %f %f %f", &IData.db_TDD[rrr][yyy][ccc], &IData.db_Water[rrr][yyy][ccc], &IData.db_OptT[rrr][yyy][ccc], &IData.db_Nfix[rrr][yyy][ccc], &IData.db_Vascularity[rrr][yyy][ccc]);
            }
            fclose(db);
        }
    }
    */
    fprintf(fpo,"----------------------------------------\n");
    fprintf(fpo,"Crop_data:\n");
    fprintf(fpo,"Rotation_Number=            %d\n", IData.Rotation_number );
    for (int rrr = 0; rrr < IData.Rotation_number;rrr++) 
    {
        fprintf(fpo,"  Rotation_ID=              %d\n", IData.db_RotationID[rrr]);
        fprintf(fpo,"  Totalyear=                %d\n",	IData.db_RotateSpan[rrr]);
        fprintf(fpo,"  Years_Of_A_Cycle=         %d\n", IData.db_CycleSpan[rrr]);
        for (int j = 0; j<IData.db_CycleSpan[rrr];j++) 
        {
            fprintf(fpo,"    YearID_of_a_cycle=      %d\n",j+1);

            //n = IData.db_YrCrops[i][j];

            fprintf(fpo,"    Crop_total_Number=      %d\n",IData.db_YrCrops[rrr][j]); 
            for (int k = 0; k< IData.db_YrCrops[rrr][j];k++)
            {
                fprintf(fpo,"      Crop_ID=              %d\n",k+1);
                fprintf(fpo,"        Crop_Type=          %d\n",IData.db_YrCropType[rrr][j][k]);
                fprintf(fpo,"        Plant_time=         %d %d\n", IData.db_PMonth[rrr][j][k], IData.db_PDay[rrr][j][k]);
                fprintf(fpo,"        Harvest_time=       %d %d\n",IData.db_HMonth[rrr][j][k], IData.db_HDay[rrr][j][k]);
                fprintf(fpo,"        Year_of_harvest=    %d\n",IData.db_HYr[rrr][j][k]);
                fprintf(fpo,"        Ground_Residue=     %f\n",IData.db_Residue[rrr][j][k]); 
                fprintf(fpo,"        Yield=              %f\n",IData.db_Yield[rrr][j][k]);

                fprintf(fpo,"        Leaf_fraction=      %f\n",IData.db_LeafFraction[rrr][j][k]);
                fprintf(fpo,"        Leaf_CN=            %f\n",IData.db_LeafCN[rrr][j][k]);
                fprintf(fpo,"        Psn_efficiency=     %f\n",IData.db_PsnEff[rrr][j][k]);
                fprintf(fpo,"        Psn_maximum=        %f\n",IData.db_PsnMax[rrr][j][k]);
                fprintf(fpo,"        TreeAge=            %f\n",IData.db_TreeAge[rrr][j][k]);
                fprintf(fpo,"        Cover_crop=         %d\n",IData.db_CoverCrop[rrr][j][k]);
                fprintf(fpo,"        Perennial_crop=     %d\n",IData.db_PerennialCrop[rrr][j][k]);

                fprintf(fpo,"        Grain_fraction=     %f\n",IData.db_GrainFraction[rrr][j][k]);
                fprintf(fpo,"        Stem_fraction=      %f\n",IData.db_ShootFraction[rrr][j][k]);
                fprintf(fpo,"        Root_fraction=      %f\n",IData.db_RootFraction[rrr][j][k]);
                fprintf(fpo,"        Grain_CN=           %f\n",IData.db_GrainCN[rrr][j][k]);
                fprintf(fpo,"        Stem_CN=            %f\n",IData.db_ShootCN[rrr][j][k]);
                fprintf(fpo,"        Root_CN=            %f\n",IData.db_RootCN[rrr][j][k]);
                fprintf(fpo,"        TDD=                %f\n",IData.db_TDD[rrr][j][k]);
                fprintf(fpo,"        Water_requirement=  %f\n",IData.db_Water[rrr][j][k]);
                fprintf(fpo,"        Optimum_temp=       %f\n",IData.db_OptT[rrr][j][k]);
                fprintf(fpo,"        N_fixation=         %f\n",IData.db_Nfix[rrr][j][k]);
                fprintf(fpo,"        Vascularity=        %f\n",IData.db_Vascularity[rrr][j][k]);
            }
            
            /*
            //DB.Format("%s\\inter\\CropTill_%d_%d.txt", OUTPUT, rrr, j);
            sprintf(DB, "%s\\CropTill_%d_%d.txt", FCT30, rrr, j);
            db=fopen(DB, "r");
            if(db==NULL) note(0,DB);
            fscanf(db, "%d", &NNN);
            for(k=1; k<=NNN; k++)
            {
                IData.TillID[k] = k;
                fscanf(db, "%d %d %d", &IData.TillMonth[k], &IData.TillDay[k], &IData.TillMethod[k]);
            }
            fclose(db);
            */

      

            fprintf(fpo,"    Tillage_number=         %d\n", IData.Tillage_number[j] ); 
            for (int k = 0; k< IData.Tillage_number[j]; k++) 
            {
                fprintf(fpo,"      Tillage_ID=           %d\n", IData.TillID[j][k] ); 
                fprintf(fpo,"        Month/Day/method=   %d %d %d\n",IData.TillMonth[j][k], IData.TillDay[j][k], IData.TillMethod[j][k]); 
            }
            /*
            //DB.Format("%s\\inter\\CropFert_%d_%d.txt", OUTPUT, rrr, j);
            sprintf(DB, "%s\\CropFert_%d_%d.txt", FCT30, rrr, j);
            db=fopen(DB, "r");
            if(db==NULL) note(0,DB);
            fscanf(db, "%d", &n);
            if(n==-1)
            {
                fscanf(db, "%s", IData.FertigationFile);
            }
            else
            {
                for(i=1; i<=n; i++)
                {
                    fscanf(db, "%d %d %d", &IData.FertMonth[i], &IData.FertDay[i], &IData.FertMethod[i]);
                    fscanf(db, "%f %f %f %f %f %f %f %f", &IData.FertNitrate[i], &IData.FertAbi[i], &IData.FertUrea[i], &IData.FertAnh[i], 
                        &IData.FertAmNi[i], &IData.NH42SO4[i], &IData.NH42HPO4[i], &IData.FertDepth[i]);
                    fscanf(db, "%f %f %f %f %f", &IData.DayRelease[i], &IData.NIefficiency[i], &IData.NIduration[i],
                        &IData.UreaseEfficiency[i], &IData.UreaseDuration[i]);
                }
                fscanf(db, "%d", &IData.FertOption);
            }
            fclose(db);
            */
            fprintf(fpo,"    Fertil_number=          %d\n", IData.Fert_number[j] );
            if(IData.Fert_number[j] == -1)
            {
                fprintf(fpo,"      fertigation_file=     %s\n",IData.FertigationFile); 
            }
            else
            {
                for (int k = 0; k<IData.Fert_number[j];k++) 
                {
                    fprintf(fpo,"      fertilization_ID=     %d\n",IData.FertID[j][k]); 
                    fprintf(fpo,"        Month/Day/method=   %d %d %d\n",IData.FertMonth[j][k], IData.FertDay[j][k], IData.FertMethod[j][k]); 
                    fprintf(fpo,"        Depth=              %f\n",IData.FertDepth[j][k]); 
                    fprintf(fpo,"        Nitrate=            %f\n",IData.FertNitrate[j][k]); 
                    fprintf(fpo,"        AmmBic=             %f\n",IData.FertAbi[j][k]); 
                    fprintf(fpo,"        Urea=               %f\n",IData.FertUrea[j][k]); 
                    fprintf(fpo,"        Anh=                %f\n",IData.FertAnh[j][k]); 
                    fprintf(fpo,"        NH4NO3=             %f\n",IData.FertAmNi[j][k]);
                    fprintf(fpo,"        NH42SO4=            %f\n",IData.NH42SO4[j][k]); 
                    fprintf(fpo,"        NH4HPO4=            %f\n",IData.NH42HPO4[j][k]); 
                    fprintf(fpo,"      Release_rate=         %f\n",IData.DayRelease[j][k]); 
                    fprintf(fpo,"      Inhibitor_efficiency= %f\n",IData.NIefficiency[j][k]); 
                    fprintf(fpo,"      Inhibitor_duration=   %f\n",IData.NIduration[j][k]); 
                    fprintf(fpo,"      Urease_efficiency= %f\n",IData.UreaseEfficiency[j][k]); 
                    fprintf(fpo,"      Urease_duration=   %f\n",IData.UreaseDuration[j][k]); 

                    
                    
                }
                fprintf(fpo,"      FertilizationOption=  %d\n",IData.FertOption[j]); 
                
            }
            /*
            //DB.Format("%s\\inter\\CropManu_%d_%d.txt", OUTPUT, rrr, j);
            sprintf(DB, "%s\\CropManu_%d_%d.txt", FCT30, rrr, j);
            db=fopen(DB, "r");
            if(db==NULL) note(0,DB);
            fscanf(db, "%d", &n);
            for(i=1; i<=n; i++)
                {
                    fscanf(db, "%d %d", &IData.ManuMonth[i], &IData.ManuDay[i]);
                    fscanf(db, "%f %f %d %d", &IData.ManuAmount[i], &IData.ManuCN[i], &IData.ManuType[i], &IData.manu_method[i]);
                }
            fclose(db);
            */
            fprintf(fpo,"    Manure_number=          %d\n",IData.Manure_number[j] );
            for (int k = 0; k < IData.Manure_number[j];k++) {
                fprintf(fpo,"      Manure_ID=            %d\n",k+1); 
                fprintf(fpo,"        Month/Day=          %d %d\n",IData.ManuMonth[j][k], IData.ManuDay[j][k]);
                fprintf(fpo,"        Amount/C|N_ratio=   %f %f\n",IData.ManuAmount[j][k], IData.ManuCN[j][k]);
                fprintf(fpo,"        Type=               %d\n", IData.ManuType[j][k]);
                fprintf(fpo,"        Method=             %d\n", IData.manu_method[j][k]);

            }
            /*
            int PlasticM1, PlasticD1,PlasticM2, PlasticD2;
            
            sprintf(DB, "%s\\CropPlastic_%d_%d.txt", FCT30, rrr, j);
            db=fopen(DB, "r");
            if(db==NULL) note(0,DB);
            fscanf(db, "%d %d", &n, &IData.WeedOption);
            for(i=1; i<=n; i++)
                {
                    fscanf(db, "%d %d", &IData.WeedMonth1[i], &IData.WeedDay1[i]);
                    fscanf(db, "%d %d", &IData.WeedMonth2[i], &IData.WeedDay2[i]);
                    fscanf(db, "%f", &IData.CoverFraction[i]);

                    //WeedMonth1[i] = JulianDay(PlasticM1, PlasticD1);
                    //WeedDay1[i] = JulianDay(PlasticM2, PlasticD2);
                }
            fclose(db);
            */
            fprintf(fpo,"    Plastic_film=            %d\n", IData.plastic_film[j] );
            fprintf(fpo,"    Ventilation=             %d\n",IData.WeedOption[j]); 
            for (int k = 0; k < IData.plastic_film[j];k++) 
            {
                fprintf(fpo,"      Plastic_ID=           %d\n",k); 
                fprintf(fpo,"      Start/End=          %d %d\n",IData.WeedMonth1[j][k], IData.WeedDay1[k]); 
                fprintf(fpo,"      Start/End=          %d %d\n",IData.WeedMonth2[j][k], IData.WeedDay2[k]);
                fprintf(fpo,"      CoverFraction=      %f\n",IData.CoverFraction[j][k]);
            }
                
            
            //DB.Format("%s\\inter\\CropFloo_%d_%d.txt", OUTPUT, rrr, j);
            //float FloodWaterN;
            /*
            sprintf(DB, "%s\\CropFloo_%d_%d.txt", FCT30, rrr, j);
            db=fopen(DB, "r");
            if(db==NULL) note(0,DB);
            fscanf(db, "%d %f %d %f", &n, &IData.FloodWaterN, &IData.WaterControl, &IData.WaterLeakRate);
            for(i=1; i<=n; i++)
            {
                fscanf(db, "%d %d", &IData.FlooMonth1[i], &IData.FlooDay1[i]);
                fscanf(db, "%d %d", &IData.FlooMonth2[i], &IData.FlooDay2[i]);
                fscanf(db, "%f", &IData.FlooN[i]);
                fscanf(db, "%d", &IData.ShallowFlood[i]);
            }
            
            fscanf(db, "%f", &IData.WaterGetherIndex);				
            fscanf(db, "%s", IData.WT_file);
            fscanf(db, "%f %f %f %f %f %f", &IData.m_IniWT, &IData.m_LWTceasingSurfFlow, &IData.m_LWTceasingGroungFlow, &IData.m_WatershedIndex, 
                                                &IData.m_SurfOutflowIntensity, &IData.m_GroundOutflowIntensity);
            fclose(db);
            */
            fprintf(fpo,"    Flood_number=           %d\n", IData.Flood_number[j]); 
            fprintf(fpo,"    FloodWaterN=            %f\n",IData.FloodWaterN[j]);
            fprintf(fpo,"    Water_control=          %d\n",IData.WaterControl[j]);
            fprintf(fpo,"    Leak_rate=              %f\n",IData.WaterLeakRate[j]);
            for (int k = 0; k< IData.Flood_number[j];k++)
            {
                fprintf(fpo,"      Flooding_ID=          %d\n",k); 
                fprintf(fpo,"        Flood_Month/Day=    %d %d\n",IData.FlooMonth1[j][k], IData.FlooDay1[j][k]);
                fprintf(fpo,"        Drain_Month/Day=    %d %d\n",IData.FlooMonth2[j][k], IData.FlooDay2[j][k]);
                fprintf(fpo,"        Water_N=            %f\n",IData.FlooN[j][k]);
                fprintf(fpo,"        Shallow_flood=       %d\n",IData.ShallowFlood[j][k]);
            }
            fprintf(fpo,"        Water_gather=       %f\n", IData.WaterGetherIndex[j]);				
            fprintf(fpo,"        WT_file=            %s\n", IData.WT_file);
            fprintf(fpo,"        Empirical_parameters= %f %f %f %f %f %f\n", IData.m_IniWT[j], IData.m_LWTceasingSurfFlow[j], IData.m_LWTceasingGroungFlow[j], IData.m_WatershedIndex[j], 
                                                IData.m_SurfOutflowIntensity[j], IData.m_GroundOutflowIntensity[j]);
        /*
            sprintf(DB, "%s\\CropIrri_%d_%d.txt", FCT30, rrr, j);
            db=fopen(DB, "r");
            if(db==NULL) note(0,DB);
            fscanf(db, "%d %d %f %d", &n, &IData.IrrMode, &IData.IrrIndex, &IData.IrriMethod[0]);
            for(i=1; i<=n; i++)
                {
                    fscanf(db, "%d %d", &IData.IrriMonth[i], &IData.IrriDay[i]);
                    fscanf(db, "%f %d", &IData.IrriAmount[i], &IData.IrriMethod[i]);
                }
            fclose(db);
            */
            fprintf(fpo,"    Irrigation_number=      %d\n",IData.Irrigation_number[j]); 
            fprintf(fpo,"    Irrigation_type=        %d\n",IData.IrrMode[j]);
            fprintf(fpo,"    Irrigation_Index=       %f\n",IData.IrrIndex[j]); 
            fprintf(fpo,"    Irrigation_Method=       %d\n",IData.IrriMethod[j][0]);
            for (int k = 0; k < IData.Irrigation_number[j]; k++)
            {
                fprintf(fpo,"      Irrigation_ID=        %d\n",k); 
                fprintf(fpo,"        Irr_Month/Day=      %d %d\n",IData.IrriMonth[j][k], IData.IrriDay[j][k]); 
                fprintf(fpo,"        Water_amount/Method=    %f %d\n",IData.IrriAmount[j][k], IData.IrriMethod[j][k]); 
            }
            /*
            //DB.Format("%s\\inter\\CropGraz_%d_%d.txt", OUTPUT, rrr, j);
            sprintf(DB, "%s\\CropGraz_%d_%d.txt", FCT30, rrr, j);
            db=fopen(DB, "r");
            if(db==NULL) note(0,DB);
            fscanf(db, "%d", &n);
                for(i=1; i<=n; i++)
                {
                    fscanf(db, "%d %d", &IData.GrazMonth1[i], &IData.GrazDay1[i]);
                    fscanf(db, "%d %d", &IData.GrazMonth2[i], &IData.GrazDay2[i]);
                    fscanf(db, "%f", &IData.GrazHour[i]);
                    fscanf(db, "%f %f %f %f %f", &IData.Dairy[i], &IData.Beef[i], &IData.Pig[i], &IData.Sheep[i], &IData.Horse[i]);
                    fscanf(db, "%f %f %d", &IData.AddFeed[i], &IData.FeedCN[i], &IData.Excreta[i]);
                }
            fclose(db);
            */

            fprintf(fpo,"    Grazing_number=           %d\n",IData.Grazing_number[j]); 
            for (int k = 0; k < IData.Grazing_number[j];k++) {
                fprintf(fpo,"      Grazing_ID=           %d\n",IData.Grazing_number[j]); 
                fprintf(fpo,"        Start_Month/Day=    %d %d\n",IData.GrazMonth1[j][k], IData.GrazDay1[j][k]);
                fprintf(fpo,"        End_Month/Day=      %d %d\n",IData.GrazMonth2[j][k], IData.GrazDay2[j][k]);
                fprintf(fpo,"        Dairy_Head_per_ha= %f\n",IData.Dairy[j][k]);
                fprintf(fpo,"        Beef_Head_per_ha= %f\n",IData.Beef[j][k]);
                fprintf(fpo,"        Pig_Head_per_ha= %f\n",IData.Pig[j][k]);
                fprintf(fpo,"        Sheep_Head_per_ha_= %f\n",IData.Sheep[j][k]);
                fprintf(fpo,"        Horse_Head_per_ha_= %f\n",IData.Horse[j][k]);				
                fprintf(fpo,"        Graz_Hours_Per_day= %f\n",IData.GrazHour[j][k]);
                fprintf(fpo,"        Additional_feed= %f\n",IData.AddFeed[j][k]);
                fprintf(fpo,"        Feed_C/N= %f\n",IData.FeedCN[j][k]);
                fprintf(fpo,"        Excreta= %d\n",IData.Excreta[j][k]);
            }

            /*
            //DB.Format("%s\\inter\\GrassCut_%d_%d.txt", OUTPUT, rrr, j);
            sprintf(DB, "%s\\GrassCut_%d_%d.txt", FCT30, rrr, j);
            db=fopen(DB, "r");
            if(db==NULL) note(0,DB);
            fscanf(db, "%d", &n);
                for(i=1; i<=n; i++)
                {
                    fscanf(db, "%d %d", &IData.CutMonth[i], &IData.CutDay[i]);
                    fscanf(db, "%f", &IData.CutAmount[i]);
                    fscanf(db, "%d", &IData.CutPart[i]);
                }
            fclose(db);
           */
            fprintf(fpo,"    Cut_number=           %d\n", IData.Cut_number[j]); 
            for (int k = 0; k < IData.Cut_number[j];k++) 
            {
                fprintf(fpo,"      Cut_ID=           %d\n",k+1); 
                fprintf(fpo,"        Cut_Month/Day=    %d %d\n",IData.CutMonth[j][k], IData.CutDay[j][k]);
                fprintf(fpo,"        Cut_amount= %f\n",IData.CutAmount[j][k]);
                fprintf(fpo,"        Cut_part= %d\n",IData.CutPart[j][k]);
            }

        }
    }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    fprintf(fpo,"Crop_model_approach         %d\n",IData.CropModel);
    
    if(IData.SCSuse==1)
    {
        /*
        float m_CN, m_RoughLand, m_RoughChannel, m_ChannelSlope, m_ChannelLength, m_LandManage;

        char scs[120];
        FILE *scspara;
        sprintf(scs,"%s\\Inputs\\scspara", OUTPUT);
        scspara = fopen(scs, "r");
        if(scspara==NULL) note(0, scs);
        fscanf(scspara, "%f %f %f %f %f %f\n", &m_CN, &m_RoughLand, &m_RoughChannel, &m_ChannelSlope, &m_ChannelLength, &m_LandManage);
        fclose(scspara);
        */
        fprintf(fpo, "%f %f %f %f %f %f\n", IData.m_CN, IData.m_RoughLand, IData.m_RoughChannel, IData.m_ChannelSlope, IData.m_ChannelLength, IData.m_LandManage);	
    }

    
    char LB[200];
    FILE* ddb;
    int US;
    sprintf(LB,"%s\\Result\\inter\\UnitSystem", ROOTDIR);
    ddb=fopen(LB, "r");
    if(ddb==NULL) //note(0, LB);
        std::cout << "Unit system file is Null" << std::endl;
    fscanf(ddb, "%d", &US);
    fclose(ddb);

    fprintf(fpo, "Unit:			%d\n", IData.Units);

    delete[] fname;

    fclose (fpo);
}

FILE* sopen (char *fname,char *mode)
{
    if (strcmp(mode,"w")==0) return sopen(fname,0);
    if (strcmp(mode,"r")==0) return sopen(fname,1);
    if (strcmp(mode,"a")==0) return sopen(fname,2);
    if (strcmp(mode,"wb")==0) return sopen(fname,3);
    if (strcmp(mode,"rb")==0) return sopen(fname,4);
    return NULL;
}

FILE* sopen (char *fname, int mode)
{
    char err[255];//IE[255],
    FILE *fp;

    //sprintf(IE, fname);
    if (mode==0) 
        fp = fopen(fname, "w");
    else if (mode==1) 
        fp = fopen(fname, "r");
    else if (mode==2) 
        fp = fopen(fname, "a");
    else if (mode==3) 
        fp = fopen(fname, "wb");
    else if (mode==4) 
        fp = fopen(fname, "rb");

    if ( fp == NULL )
    {
        if (mode==0) 
            sprintf(err,"Cannot open file: %s (File system may have error!)", fname);
        else
            sprintf(err,"Cannot open file: %s (File may not exist!)", fname);

        return (NULL);
    }
    return (fp);
}

void ReadCropPara1(int *v_CropID, char v_CropName[95][100], float *v_max_biomass_C, float *v_grain_fraction, float *v_leaf_fraction,
                  float *v_leafstem_fraction, float *v_root_fraction, float *v_GrainCN, 
                  float *v_LeafCN, float *v_ShootCN, float *v_RootCN, float *v_WaterDemand, float *v_OptimumT, 
                  float *v_TDD, float *v_Nfixation, float *v_Vascularity, int *v_Perennial, float *v_TreeAge)
{
    char text[300];
    std::string CCrop;
    FILE *fCrop;
    //CCrop.Format("%s\\Library\\lib_crop\\Crop_parameters.txt", ROOTDIR);//, CropType);
    CCrop = ( (std::string)ROOTDIR + "\\Library\\lib_crop\\Crop_parameters.txt" );
    fCrop = fopen(CCrop.c_str(), "r");
    if(fCrop==NULL)
        std::cout << fCrop << " is Null" << std::endl;
    else
    {
        fgets(text, 300, fCrop);
        fgets(text, 300, fCrop);
        for(int i=0; i<=90; i++)
        {
            fscanf(fCrop, "%d %s", &v_CropID[i], v_CropName[i]);
            fscanf(fCrop, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f", &v_max_biomass_C[i], 
                &v_grain_fraction[i], &v_leaf_fraction[i], &v_leafstem_fraction[i], &v_root_fraction[i], 
                &v_GrainCN[i], &v_LeafCN[i], &v_ShootCN[i], &v_RootCN[i],
                &v_WaterDemand[i], &v_OptimumT[i], &v_TDD[i], &v_Nfixation[i], &v_Vascularity[i]);
            fscanf(fCrop, "%d", &v_Perennial[i]);//, &v_TreeAge[i]);
        }
        fclose(fCrop);

    }	
}

void CN_ratios(float *RCNRVL, float *RCNRL, float *RCNRR, float *RCNB, float *RCNH, float *RCNM, float *PassiveCCN)
{
    *RCNRVL = 5.;
    *RCNRL  = 25.;
    *RCNRR  = 100.;
    *RCNB   = 10.0;
    *RCNH   = 10.0;
    *RCNM   = 10.0;
    *PassiveCCN = 500.0;
}

int FileCopy(char *source, char *dest)
{
    FILE *fpi,*fpo;
    long flen,rt,num,adr;
    char *p;

    p=(char*)malloc(4096);
    if (p==NULL) {
        //MessageBox(NULL,"No enough memory!","Running Error:",0);
        return -1;
    }

    fpi=fopen(source,"rb");
    if (fpi==NULL) 
    {
        //note.Format("1. Source file does not exist: %s", source);
        //AfxMessageBox(note);
        exit(0);
    }
    fpo=fopen(dest,"wb");
    if (fpo==NULL) {
        //MessageBox(NULL,"Source file no exist!","Running Error:",0);
        return -1;
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