#pragma once

#if defined( DNDC95_DLL )
    #define DNDC_EXPORTS __declspec( dllexport )
#else
    #define DNDC_EXPORTS __declspec( dllimport )
#endif

//DNDC_EXPORTS
 class DNDC_EXPORTS Site_BaseInformation
{
public:
    //input file data
    int Tillage_number[20];
    int Fert_number[20];
    int Flood_number[20];
    int Manure_number[20];
    int Irrigation_number[20];
    int Grazing_number[20];
    int Cut_number[20];
    int Rotation_number;
    int TillID[20][15], TillMonth[20][15], TillDay[20][15], TillMethod[20][15];
    int FertMonth[20][15], FertDay[20][15], FertMethod[20][15], FertID[20][15];
    double FertNitrate[20][15], FertAbi[20][15], FertUrea[20][15], FertAnh[20][15], FertAmNi[20][15], NH42SO4[20][15], NH42HPO4[20][15], FertDepth[20][15];
    int ManuMonth[20][15], ManuDay[20][15], ManuType[20][15], manu_method[20][15];
    double ManuAmount[20][15], ManuCN[20][15], ManuN[20][15];
    int WeedMonth1[20][15], WeedDay1[20][15],WeedMonth2[20][15], WeedDay2[20][15];
    double CoverFraction[20][15];
    int FlooMonth1[20][15], FlooDay1[20][15], FlooMonth2[20][15], FlooDay2[20][15];
    double FlooN[20][15];
    int ShallowFlood[20][15];
    int IrrMode[20], IrriMonth[20][15], IrriDay[20][15];
    double IrrIndex[20], IrriAmount[20][15];
    int IrriMethod[20][15];
    int GrazMonth1[20][15], GrazDay1[20][15], GrazMonth2[20][15], GrazDay2[20][15], Excreta[20][15];
    double GrazHour[20][15], Dairy[20][15], Beef[20][15], Pig[20][15], Horse[20][15], Sheep[20][15], AddFeed[20][15], FeedCN[20][15];
    const char* FertigationFile;
    int CutMonth[20][15], CutDay[20][15], CutPart[20][15], FertOption[20];
    double CutAmount[20][15], WaterGetherIndex[20], WaterLeakRate[20];
    double DayRelease[20][15], NIefficiency[20][15], NIduration[20][15];
    double UreaseEfficiency[20][15], UreaseDuration[20][15];
    double m_IniWT[20], m_LWTceasingSurfFlow[20], m_LWTceasingGroungFlow[20], m_WatershedIndex[20], m_SurfOutflowIntensity[20], m_GroundOutflowIntensity[20];
    const char* WT_file;
    double FloodWaterN[20];
    int WaterControl[20];
    int Units, WeedOption[20], plastic_film[20];
    // SCS data
    double m_CN;
    double m_RoughLand;
    double m_RoughChannel;
    double m_ChannelSlope;
    double m_ChannelLength; 
    double m_LandManage;
    // Site information
    char  sitename[255];
    char  Regionname[255];
    double latitude;
    double longitude;
    long  totalday;
    int   years;
    int   ifdaily, if1File;
    // Climate information
    const char* ClimateFilenames[300];
    int   ClimateFileType;
    int   ClimateFileNumber;
    double NO3NH4_Rainfall;
    double NH3_Atmosphere;//NO3_Atmosphere;
    double BaseCO2;
    double CO2IncreaseRate;
    double SOCa, SOCb;
    //double s_temp;
    double AnnualMeanT;//yr_avet;
    int WaterYr;
    // Soil information
    int   Soil_Texture;
    double Soil_Density;
    double Soil_pH;
    double Flood_pH;
    double Soil_OCatSurface;
    double Soil_Clay;
    int   Soil_landuse;
    //int   Soil_BypassFlow;
    double Soil_BypassFlow;
    double Soil_Litter;// Resoc
    double Soil_humads;// Ahsoc
    double Soil_humus; // Phsoc
    double PassiveC, PassiveCCN;
    //double RCNRVL, RCNRL, RCNRR, RCNB, RCNH, RCNM;
    double Soil_NO3;   // IniNO3
    double Soil_NH4;   // IniNH4
    double Soil_moiture;
    double Soil_T;
    int   SoilRetardation; //Soil retaration for water infiltration
    double LateralInflux;
    double Soil_MicrobioIndex;
    double Soil_WaterObstructLayer;
    double slope;
    double Soil_Quality;
    //int HighWT;
    double HighestWT;
    double Soil_Porosity;
    int UseION, PerennialCrop;
    double DClitter, DChumads, DChumus;
    double HumadCN, HumusCN;
    //double FieldCapacity;
    //double WiltPoint;
    double WaterPool;
    int   Idc;
    double H;
    double   Topr;
    double   Toph;
    double CEC;
    double TH;
    double m;
    double V;
    int   Q;//double 
    double Qstar;
    double AcWater;
    double SurfWater;
    double SnowPack;
    double BypassF;
    //double IrriWaterPH; 
    char  SoilName[30];
    char  NameNote[30];
    //Clay;
    //double Sts;
    //double RawSKS;
    double Sks;
    double Fldcap;
    double Wiltpt;
    double Psisat;
    double Beta;
    int SCSuse;
    double m_SPthickness;
    int m_SPlayers, m_SPso[11], layer[11];
    double m_SPth[11], m_SPd[11], m_SPsoc[11], m_SPph[11], m_SPfc[11], m_SPwp[11], m_SPpo[11], m_SPhy[11], m_SPc[11];
    char m_ProfileName[250];
    //    char  ClayNote[30];
    //    char  StsNote[30];
    //    char  SksNote[30];
    //    char  FldcapNote[30];
    //    char  WiltptNote[30];
    //    char  SoilhtcpNote[30];
    //    char  BetaNote[30];
    //    char  PsisatNote[30];
    //int   MaxCycle;
    int   MaxCrop;
    int   MaxTill;
    int   MaxFer;
    int   MaxManure;
    int   MaxWeed;
    int   MaxFlood;
    int   MaxIrr;
    int   MaxGrazing;
    int   MaxLime;
    int   CropModel;
    //forest parameter
    double FoliageMass, WoodMass, LitterMass;
    int   ForestAge, ForestType, SoilFertility;
    double SOC12, LitterpH;
    double FolCNR,FloorC,LitterWater, Vascularity;
    const char* ForestName;	
    int cropping_number;
    int db_Rotation_Number;
    int db_RotationID[20], db_RotateSpan[20], db_CycleSpan[20], db_PerennialCrop[20][20][5];
    int db_YrCrops[20][20], db_YrCropOrder[20][20][5], db_YrCropType[20][20][5], db_CoverCrop[20][20][5];
    int db_PMonth[20][20][5], db_PDay[20][20][5], db_HMonth[20][20][5], db_HDay[20][20][5], db_HYr[20][20][5];
    double db_Residue[20][20][5], db_Yield[20][20][5], db_RepRate[20][20][5], db_VegRate[20][20][5];
    double db_PsnEff[20][20][5], db_PsnMax[20][20][5], db_TreeAge[20][20][5];
    double db_GrainFraction[20][20][5], db_LeafFraction[20][20][5], db_ShootFraction[20][20][5], db_RootFraction[20][20][5];
    double db_GrainCN[20][20][5], db_LeafCN[20][20][5], db_ShootCN[20][20][5], db_RootCN[20][20][5];
    double db_TDD[20][20][5], db_Water[20][20][5], db_OptT[20][20][5], db_Nfix[20][20][5], db_Vascularity[20][20][5];

    Site_BaseInformation()
    {
        CropModel = 0;
        Rotation_number = 0;
        Tillage_number[0] = 0;
        Fert_number[0] = 0;
        Flood_number[0] = 0;
        Manure_number[0] = 0;
        Irrigation_number[0] = 0;
        Grazing_number[0] = 0;
        Cut_number[0] = 0;
        TillID[0][0] = 0; 
        TillMonth[0][0] = 0; 
        TillDay[0][0] = 0;
        TillMethod[0][0] = 0;
        FertID[0][0] = 0;
        FertMonth[0][0] = 0;
        FertDay[0][0] = 0;
        FertMethod[0][0] = 0;
        FertNitrate[0][0] = 0.0; 
        FertAbi[0][0] = 0.0;
        FertUrea[0][0] = 0.0;
        FertAnh[0][0] = 0.0;
        FertAmNi[0][0] = 0.0;
        NH42SO4[0][0] = 0.0;
        NH42HPO4[0][0] = 0.0;
        FertDepth[0][0] = 0.0;
        FertigationFile = "None0.00000";
        ManuMonth[0][0] = 0;
        ManuDay[0][0] = 0;
        ManuType[0][0] = 0;
        manu_method[0][0] = 0;
        ManuAmount[0][0] = 0.0;
        ManuCN[0][0] = 0.0; 
        ManuN[0][0] = 0.0;
        plastic_film[0] = 0;
        WeedMonth1[0][0] = 0;
        WeedDay1[0][0] = 0;
        WeedMonth2[0][0] = 0;
        WeedDay2[0][0] = 0;
        CoverFraction[0][0] = 0.0;
        WeedOption[0] = 0;
        FlooMonth1[0][0] = 0;
        FlooDay1[0][0] = 0;
        FlooMonth2[0][0] = 0;
        FlooDay2[0][0] = 0;
        FlooN[0][0] = 0.0;
        ShallowFlood[0][0] = 0;
        FloodWaterN[0] = 0.0;
        WaterGetherIndex[0] = 0.0;
        WaterLeakRate[0] = 0.0;
        WaterControl[0] = 0;
        IrrMode[0] = 0;
        IrriMonth[0][0] = 0;
        IrriDay[0][0] = 0;
        IrrIndex[0] = 0.0;
        IrriAmount[0][0] = 0.0;
        IrriMethod[0][0] = 0;
        GrazMonth1[0][0] = 0;
        GrazDay1[0][0] = 0;
        GrazMonth2[0][0] = 0;
        GrazDay2[0][0] = 0;
        Excreta[0][0] = 0;
        GrazHour[0][0] = 0.0;
        Dairy[0][0] = 0.0;
        Beef[0][0] = 0.0;
        Pig[0][0] = 0.0;
        Horse[0][0] = 0.0;
        Sheep[0][0] = 0.0;
        AddFeed[0][0] = 0.0;
        FeedCN[0][0] = 0.0;
        CutMonth[0][0] = 0;
        CutDay[0][0] = 0;
        CutPart[0][0] = 0;
        FertOption[0] = 0;
        CutAmount[0][0] = 0.0;
        DayRelease[0][0] = 0.0;
        NIefficiency[0][0] = 0.0;
        NIduration[0][0] = 0.0;
        UreaseEfficiency[0][0] = 0.0;
        UreaseDuration[0][0] = 0.0;
        m_IniWT[0] = 0.0;
        m_LWTceasingSurfFlow[0] = 0.0;
        m_LWTceasingGroungFlow[0] = 0.0;
        m_WatershedIndex[0] = 0.0;
        m_SurfOutflowIntensity[0] = 0.0;
        m_GroundOutflowIntensity[0] = 0.0;
        WT_file = "None0.00000";
        Units = 0;
        m_CN = 0.0;
        m_RoughLand = 0.0;
        m_RoughChannel = 0.0;
        m_ChannelSlope = 0.0;
        m_ChannelLength = 0.0; 
        m_LandManage = 0.0;
        latitude =0;
        longitude=120.0;
        totalday =0;
        years =0;
        ifdaily  =0;
        if1File  =0;
        Units = 0;
        db_Rotation_Number = 0;
        ClimateFileType  = 1;
        ClimateFileNumber= 0;
        NO3NH4_Rainfall  = 0;
        NH3_Atmosphere   = 0.06;
        BaseCO2= 350;
        WaterYr = 0;
        CO2IncreaseRate = 0.0;
        SOCa = 0.0;
        SOCb = 0.0;
        Soil_Texture = 0;
        Soil_Density = 0;
        Soil_pH = 0;
        Soil_OCatSurface = 0;
        Soil_Clay = 0;
        Soil_landuse = 0;
        Soil_BypassFlow  = 0;
        Soil_Litter  = 0;
        Soil_humads  = 0;
        Soil_humus   = 0;
        PassiveC = 0.0;
        Soil_NO3 = 0;// IniNO3
        Soil_NH4 = 0;// IniNH4
        Soil_moiture = 0;
        Soil_T = 0;
        SoilRetardation = 0;
        LateralInflux = 1.0;
        Soil_MicrobioIndex = 1.0;
        Soil_Quality = 0.0;
        Soil_WaterObstructLayer = 9.99;//m
        slope = 0.0;
        Fldcap = 0.0;
        Wiltpt = 0.0;
        HighestWT = 1.0;
        Soil_Porosity = 0.0;
        DClitter=1.0;
        DChumads=1.0;
        DChumus=1.0;
        HumadCN=10.0;
        HumusCN = 10.0;
        SCSuse = 0;
        Idc = 0;
        H = 0;
        Topr = 0;
        Toph = 0;
        CEC = 0;
        TH = 0;
        m = 0;
        V = 0;
        Q = 0;
        Qstar = 0;
        AcWater = 0;
        SurfWater = 0;
        SnowPack = 0;
        BypassF = 0;
        Sks = 0;
        LateralInflux= 1.0;
        Psisat= 0; 
        Beta = 0;
        MaxCrop = 0;
        MaxTill = 0;
        MaxFer = 0;
        MaxManure = 0;
        MaxWeed = 0;
        MaxFlood = 0;
        MaxIrr = 0;
        MaxGrazing = 0;
        MaxLime = 0;
        FoliageMass = 0;
        WoodMass = 0;
        LitterMass = 0;
        ForestAge = 0;
        ForestType = 0;
        SoilFertility = -1;
        SOC12 = 1;
        FolCNR = 0;
        FloorC = 0;
        LitterWater = 0;
        LitterpH = 0;
        ForestName = "";
    }

    ~Site_BaseInformation()
    {
        ;
    }

};

DNDC_EXPORTS Site_BaseInformation* GetNewSitePtr();

DNDC_EXPORTS void InitSiteData( Site_BaseInformation* siteData );

DNDC_EXPORTS void DeleteSitePtr( Site_BaseInformation* siteData );

DNDC_EXPORTS void SaveDatatoFile(
        const char* outputPath,
        int mode,
        Site_BaseInformation siteData );

DNDC_EXPORTS void SetRootDirPaths(
    const char* rootPath,
    const char* outputPath,
    const char* interPath,
    const char* inputsPath,
    const char* intermanPath );

DNDC_EXPORTS void SetLibPath(
    const char* libPath );

DNDC_EXPORTS void WriteInputFiles(
    const char* InputFileName,
    char *BatchPass );

DNDC_EXPORTS void WriteInputFiles(
    Site_BaseInformation* siteData,
    char* BatchPass );


DNDC_EXPORTS int Model_link(
    int scale, char* cropping_system, int S_SoilYear, int S_ThisYear,
    int S_SimuYears, char* DatabaseName, char* r_Province, char* ScenarioName,
    double ha, int ProvinceOrder, int S_county, int CountyIDX, int landuse,
    int StartYear, int ifdaily, int MonteCarlo, int ZXH, int Batch,
    int GlobeData, char* DirRoot, int sample, int RRRRR, int IrriType,
    char* country_ID, int livestock, int Monitor, int DroughtID,
    char f_region[6][150], char* DroughtYield, int FFN, char *BatchPass );
