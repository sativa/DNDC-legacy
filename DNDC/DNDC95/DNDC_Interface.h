#pragma once

#include <memory>

#if defined( DNDC95_DLL )
    #define DNDC_EXPORTS __declspec( dllexport )
#else
    #define DNDC_EXPORTS __declspec( dllimport )
#endif

//DNDC_EXPORTS
 class DNDC_EXPORTS
 Site_BaseInformation
{
public:

    Site_BaseInformation();
    ~Site_BaseInformation();

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

};

DNDC_EXPORTS
std::shared_ptr< Site_BaseInformation > GetNewSitePtr();

DNDC_EXPORTS
void InitSiteData( Site_BaseInformation* siteData );

DNDC_EXPORTS
void DeleteSitePtr( Site_BaseInformation* siteData );

DNDC_EXPORTS
void SaveDatatoFile(
    const char* outputPath,
    int mode,
    Site_BaseInformation siteData );

DNDC_EXPORTS
void SetRootDirPaths(
    const char* rootPath,
    const char* outputPath,
    const char* interPath,
    const char* inputsPath,
    const char* intermanPath );

DNDC_EXPORTS
void SetLibPath(
    const char* libPath );

DNDC_EXPORTS
void WriteInputFiles(
    const char* dndFileName,
    char *BatchPass );

DNDC_EXPORTS
void WriteInputFiles(
    Site_BaseInformation* siteData,
    char* BatchPass );

DNDC_EXPORTS
int GetSimYearsFromDNDFile( const char* dndFileName );

DNDC_EXPORTS
int Model_link(
    int scale, char* cropping_system, int S_SoilYear, int S_ThisYear,
    int S_SimuYears, char* DatabaseName, char* r_Province, char* ScenarioName,
    double ha, int ProvinceOrder, int S_county, int CountyIDX, int landuse,
    int StartYear, int ifdaily, int MonteCarlo, int ZXH, int Batch,
    int GlobeData, char* DirRoot, int sample, int RRRRR, int IrriType,
    char* country_ID, int livestock, int Monitor, int DroughtID,
    char f_region[6][150], char* DroughtYield, int FFN, char *BatchPass );
