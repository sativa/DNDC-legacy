#include <string>
#include "Source_main.h"
#include "Dndc_tool.h"
//#include "SitePara.h"
#include <iostream>
#include "main.h"
#include <iostream>
#include <fstream>
#include <vector>
#include "MY_GDAL.h"
#include "version.h"
#include "Python_plug.h"
#include "Dndc_RunModel.h"

#include "SQLiteWrapper.h"
#include "libsqlitewrapped.h"

using std::string;


/// Change log
/// 2012-4-3, change the max length of path to 3072 bytes.
/// Still have long path bugs in the program, need to fix soon

//operator string(int i){char temp[40];return string(itoa(i,temp,20));}

//using std::string;


//using namespace std;

///TODO:




/// if wanted to compile exe file, just comment this sentence.

//#define DLLEXPORT extern "C" __declspec(dllexport)




int main(int argc, char *argv[])
{
    char *DRD_file;
    string Result_Path;
    char *DNDC_path;
    char *DND_file;
    string Drought_file = "";
    char *Log_file ="";
    char *Region_json_file;
    string InConfig;

    Site_BaseInformation mytestdata ;

//    mytestdata = callPython();

//    std::string input;
//
//    input = std::cin.get();
    //cout << "mytestdata " << mytestdata.latitude << endl;

//      sqline = "CREATE TABLE t_test4 (a INTEGER PRIMARY KEY , ";
//
//      for (int i=1 ;i<365;i++)
//      {
//        string tmp;
//        tmp = "Day" + int2str(i) + " int , ";
//        sqline += tmp;
//
//      }
//
//      sqline = sqline + " Day365 int)";
//      //sprintf(sqline,"%s, 365 int )",sqline);
//
//      cout << sqline << endl;
//
//      OperateDB(dbpath,sqline);
//
//    while (cmd != "END")
//    {
////        cin >> cmd;
//        cout << "Input SQL line " << endl;
//        getline(cin,sqline);
//
//        cout << sqline << endl;
//        OperateDB(dbpath,sqline);
//    }


//    string major =  string(AutoVersion::MAJOR);
//    string minor =  string(AutoVersion::MINOR);
//    string compiletime = string(AutoVersion::BUILDS_COUNT);
//    string version = major + minor + compiletime;

    string version = AutoVersion::FULLVERSION_STRING;



    int json = 0;
    int drought = 0;
    int site = 0;
    int region = 0;
    int simu_log = 0;


    for( int i = 1; i < argc; i++ )
    {
        if( EQUAL(argv[i],"-json"))
        {
            json = 1;
        }
        if( EQUAL(argv[i],"-dews"))
        {
            drought = 1;
            Drought_file = argv[i+1];
        }
        if( EQUAL(argv[i],"-r")&&site==0)
        {
            region = 1;
            if (i+1 < argc) InConfig=argv[i+1];
        }
        if( EQUAL(argv[i],"-s")&&region==0)
        {
            site = 1;
            if (i+1 < argc) InConfig=argv[i+1];
        }
        if( EQUAL(argv[i],"-log"))
        {
            simu_log = 1;
        }
        if( EQUAL(argv[i],"-root"))
        {
            if (i+1 < argc) DNDC_path = argv[i+1];
        }
        if( EQUAL(argv[i],"-out"))
        {
            if (i+1 < argc) Result_Path = argv[i+1];
        }
    }

//    if (site == region)
//    {
//        cout << "Have to chose BETWEEN <Regional Simulation> -r  OR <Point Simulation> -s " <<endl;
//        exit(-1);
//    }
//    else if (len(argv) < 4)
//    {
//        cout << "Usage: Site Model: Console -s <site.json> -out <Result Path> -root <Excutable Path> -json" << endl;
//        cout << "Usage: Region Model: Console -r <Region.json> -out <Result Path> -root <Excutable Path> -json" << endl;
//        cout << "Current Version "<< version << endl;
//        cout << "For Site:"
//        cout << "Drought_DNDC.exe -s c:\test\DNDC.json -out c:\test\mytest  -root d:\drought_DND -json"
//        cout << "For Regional Simulation:"
//        cout << "Drought_DNDC.exe -r c:\test\dndcgeojson.geojson -out d:\mydndctest  -root d:\drought_DNDC -json" <<endl;
//
//
//    }




    if (region == 1 && json == 1 && drought == 0 && site == 0)
    {
        Initial_Dirs(Result_Path);
        OpenRegion(DNDC_path,InConfig,Result_Path,Drought_file,simu_log);

    }
    else if (site ==1 && json == 1 && region == 0)
    {
        Initial_Dirs(Result_Path);
        OpenSite(DNDC_path,InConfig,Result_Path);

    }
    else
    {
        cout << "Usage: Site Model: Console -s <site.json> -out <Result Path> -root <Excutable Path> -json" << endl;
        cout << "Usage: Region Model: Console -r <Region.json> -out <Result Path> -root <Excutable Path> -json" << endl;
        cout << "Current Version "<< version << endl;
        cout << "For Site:" << endl;
        cout << "Drought_DNDC.exe -s c:\\test\\DNDC.json -out c:\\test\\mytest  -root d:\\drought_DND -json" << endl;
        cout << "For Regional Simulation:" << endl;
        cout << "Drought_DNDC.exe -r c:\\test\\dndcgeojson.geojson -out d:\\mydndctest  -root d:\\drought_DNDC -json" <<endl;



//        cout << "       Drought_DNDC.exe C:\\DNDC\\Changhai.dnd C:\\DNDC\\Result C:\\DNDC\\" << endl;
//        cout << "       Drought_DNDC.exe C:\\DNDC\\Changhai.xml C:\\DNDC\\Result C:\\DNDC\\  -xml" << endl;
//        cout << "       Regional Model : Console <drd Path> <Result Path> <Database Path> <Drought Configure Path > <Log File Name>" << endl;
//        cout << "       Drought_DNDC.exe C:\\DNDC\\Liaoning.drd C:\\DNDC\\Result C:\\DNDC  C:\\DNDC\\Drought.txt Logging.txt" << endl;
        exit(1);
    }
//
//    if (argc == 6)
//    {
//        // run regional model
//        DRD_file = argv[1];
//        Result_Path= argv[2];
//        DNDC_path= argv[3];
//        Drought_file = argv[4];
//        Log_file     = argv[5];
//        cout << "DNDC Path " << DNDC_path <<endl;
//
//#ifndef LINUX
//
//        int tmppoint = Result_Path.find_first_of("/",0);
//        while(tmppoint != string::npos)
//        {
//            Result_Path.replace(tmppoint,1,"\\");
//            tmppoint = Result_Path.find_first_of("/",tmppoint+1);
//        }
//#endif
//        Initial_Dirs(string(Result_Path));
//
//
//        char *p = new char[Result_Path.length()+1]();
//        strcpy(p, Result_Path.c_str());
//
//        //OpenRegion(DRD_file,p,DNDC_path,Drought_file,Log_file);
//
//        OpenRegion(DNDC_path,DRD_file,Result_Path);
//        //RunRegion("c:\\DNDC/debug.drd","C:\\DNDC/Result", "C:\\DNDC" );
//    }
//    else if (argc == 4 || (argc == 5 && xml == 1))
//    {
//        // Run site model
//        cout << "Run Site Model" << endl;
//        DND_file = argv[1];
//        Result_Path= argv[2];
//        DNDC_path= argv[3];
//
//#ifndef LINUX
//        int tmppoint = Result_Path.find_first_of("/",0);
//        while(tmppoint != string::npos)
//        {
//            Result_Path.replace(tmppoint,1,"\\");
//            tmppoint = Result_Path.find_first_of("/",tmppoint+1);
//        }
//
//#endif
//
//        Initial_Dirs(string(Result_Path));
//
//        if (json ==1)
//        {
//            string xmlfile = argv[1];
//            OpenSite(DNDC_path,xmlfile,Result_Path);
//        }
//        else
//        {
//            OpenSite(DNDC_path,DND_file,Result_Path);
//        }
//
//    }
//    else
//    {
//        cout << "Usage: Site Model: Console <dnd Path> <Result Path> <Database Path>" << endl;
//        cout << "Usage: Site Model XML Input : Console <XML Config Path> <Result Path> <Database Path> -xml" << endl;
//        cout << "Current Version "<< version << endl;
//        cout << "       Drought_DNDC.exe C:\\DNDC\\Changhai.dnd C:\\DNDC\\Result C:\\DNDC\\" << endl;
//        cout << "       Drought_DNDC.exe C:\\DNDC\\Changhai.xml C:\\DNDC\\Result C:\\DNDC\\  -xml" << endl;
//        cout << "       Regional Model : Console <drd Path> <Result Path> <Database Path> <Drought Configure Path > <Log File Name>" << endl;
//        cout << "       Drought_DNDC.exe C:\\DNDC\\Liaoning.drd C:\\DNDC\\Result C:\\DNDC  C:\\DNDC\\Drought.txt Logging.txt" << endl;
//        exit(1);
//    }
//


}


void Initial_Dirs(string out_path)
{
    /** \brief Create all directories...
     *
     * \param
     * \param
     * \return
     *
     */


#ifdef LINUX

    cout << "LINUX VERSION" <<endl;

    system(("mkdir " + out_path + " -m 777").c_str());
    system(("mkdir " + out_path + "/Record -m 777").c_str());
    system(("mkdir " + out_path + "/Record/Site -m 777").c_str());
    system(("mkdir " + out_path + "/Record/Region -m 777").c_str());
    system(("mkdir " + out_path + "/Inter -m 777").c_str());
//    system(("mkdir " + out_path + "/Inter/Management_1 -m 777").c_str());
//    system(("mkdir " + out_path + "/Inter/RegionFiles -m 777").c_str());
    system(("mkdir " + out_path + "/Inputs -m 777").c_str());

#else
    /// Replace all "/" to the "\\"
    /// Only for windows

    int tmppoint = out_path.find_first_of("/",0);
    while(tmppoint != string::npos)
    {
        out_path.replace(tmppoint,1,"\\");
        tmppoint = out_path.find_first_of("/",tmppoint+1);
    }

    int tmpcount = 0;
    size_t found= out_path.find("\\");
    while(found!=string::npos)
    {
        tmpcount++;
        found= out_path.find("\\",found+1);
    }


    if (tmpcount < 2)
    {
        mkpath(out_path.c_str(),777);

    }
    else
    {
        int point = out_path.find_first_of("\\",0);
        while(point != string::npos)
        {

            mkpath((out_path.substr(0,point)).c_str(),777);
            point = out_path.find_first_of("\\",point+1);
        }
    }

#endif

    mkpath((out_path + "/Record").c_str(),777);
    mkpath((out_path + "/Record/Site").c_str(),777);
    mkpath((out_path + "/Record/Region").c_str(),777);
    mkpath((out_path + "/Inter").c_str(),777);
//    mkpath((out_path + "/Inter/Management_1").c_str(),777);
//    mkpath((out_path + "/Inter/RegionFiles").c_str(),777);
    mkpath((out_path + "/Inputs").c_str(),777);


}


/// if need compile exe file, just comment this sentence and uncomment the old code.
//DLLEXPORT int start(char* argv)
//{
//
//    string dndc_path;
//    dndc_path = argv;
//    cout << "Current Workspace is \n" << dndc_path.c_str() << endl;
//    // cout << dndc_path.c_str() << endl;
//    class_model dndcobj;
//    // Run regional model
//    dndcobj.zf_Shangrila(argv,1);
//    return 0;
//
//  }


/// In original DNDC, this function name is zf_Shangrila() or Shangrila(). In this new version, I changed the name to Region_Simulator().
//DLLEXPORT void Region_Simulator(char* argv)
void Region_Simulator(char* DNDC_path,char* Result_Path,char *in_shape,char *Log_File)
{
    // A test function, to invoke the regional DNDC function.
    /// Initialize the Region model simulator

    printf("Starting Regional Modelling....\n");
    //string path;
    //path = argv;
    printf("Current Workspace is %s \n", DNDC_path);

    /// DNDC Logging ......
    //string LogName = string(Result_Path) + "/DNDC_" + GetSysTimeFormat() + ".status";
    /// Use Predefined Logging Name
    string LogName = string(Result_Path) + "/" + string(Log_File);

    FILE *logout;
    logout = fopen(LogName.c_str(), "w");
    fprintf(logout, "Starting Regional Modelling....\n");
    fclose(logout);

//    string dndc_path;
//    dndc_path = argv;

    class_model dndcobj(DNDC_path,Result_Path);

    // initail daily results database
    char DB_Path[255];
    sprintf(DB_Path,"%s/Region_Daily_Result.db",Result_Path);
    Initial_DNDC_DB(DB_Path);
    // Run regional model
    // Default Startcell is 0
    dndcobj.LogName = LogName;
    int ret = dndcobj.zf_Shangrila(DNDC_path,Result_Path,DB_Path,1,0,in_shape);

    if (ret == 0)
        cout << "Success" << endl;
    else
        cout << "False" << endl;



}





/// In original DNDC, this function name is Model_link. In this new version, I changed the name to Site_Simulator().


void OpenSite(char* root_path, string InConfig_Path,string Results_Path)
{
/// \brief Initialize the site model AND Data Initialize for site model
/// \brief this version was revised for the new json input format.
///
/// \param root_path Location of excuable file
/// \param InConfig_Path  Config file, Use json
/// \param RESULT_Path Results Path
/// \return Void
///
///



    //sprintf(result_path,"%s/Result",root_path);


    //sprintf(DailyResult,"%s/Site_Daily_Result.db",result_path);

    /// Initialization form the JSON saved file

//     ReadInputDatafromDND("c:\\DNDC/debug.dnd","C:\\DNDC/Result","C:\\DNDC");



    char result_path[3027];

    strcpy(result_path,Results_Path.c_str());


    // initialization of the input files

    string xmlpath = InConfig_Path;// "C:\\test\\DNDC_INPUT.xml";

    class_model *DNDC = new class_model(root_path,result_path);
//    ReadInputDatafromDND(dnd_path,result_path,root_path,&IData);
    DNDC->ReadInputDatafromXML(xmlpath,result_path);

    int S_SimuYears = DNDC->IData->years;

    cout << "Simulation Years " << S_SimuYears << endl;

    for(int ThisYear=1; ThisYear<=S_SimuYears; ThisYear++)
    {

        int drc = 1;
        int DroughtID= 0;
        int scale = 0;
        //char f_region[5][150]= {"",""};
        char f_region[6][3072]= {""};
        char* DroughtYield = "0";
        char* DroughtDayProfile = "0";
        float ha = 0.0;
        int CountyIDX = 0;
        int landuse = 0;
        string cropping_system = "Corn";
        char* DatabaseName = "None";
        char* ScenarioName = "None";
        string DailyResult="";
        int DailyRecord = DNDC->IData->ifdaily;
        cout << "Year: " << ThisYear << endl;
        cout << "Total Years " <<S_SimuYears << endl;
        DNDC->DNDCgo_New(scale,cropping_system,
                         CountyIDX, landuse, DailyRecord,
                         ThisYear,DatabaseName,ScenarioName,drc,DroughtID,
                         DroughtYield,DroughtDayProfile,DailyResult);

    }

    cout << "The simulation is accomplished " << endl;

}


void OpenRegion(char* root_path, string InConfig_Path,string Result_Path,string dewsfile,int simulog)
{
    // initialization of the input files

    char result_path[3072];
    char in_config[3072];
    strcpy(result_path,Result_Path.c_str());
    strcpy(in_config,InConfig_Path.c_str());
//    string xmlpath = InConfig_Path;// "C:\\test\\DNDC_INPUT.xml";
    int Drought_Simulation = 0;

    if (dewsfile !="")
    {
        Drought_Simulation = 1;
    }

//    RunModel DNDC(root_path,result_path);
    RunModel *DNDC = new RunModel();
    string dailyfile = "";
    DNDC->RegionProcess(root_path,Result_Path,0,0,in_config);

    delete DNDC;


}


void OpenSite(char* root_path, char *dnd_path,string RESULT_Path)
//int Site_Simulator(char* out_root_path)
/*
int scale, char* cropping_system, int S_SoilYear, int S_ThisYear,
    int S_SimuYears, char* DatabaseName, char* r_Province, char* ScenarioName,
    float ha, int ProvinceOrder, int S_county, int CountyIDX, int landuse,
    int StartYear, int ifdaily, int MonteCarlo, int ZXH, int Batch,
    int GlobeData, char* DirRoot, int sample, int Start_Cell, int IrriType,
    int country_ID)
*/
{
/// \brief Initialize the site model AND Data Initialize for site model
///
/// \param root_path Location of excuable file
/// \param dnd_path  Config file, Use Original DND, used for Backwards compatible
/// \param RESULT_Path Results Path
/// \return Void
///
///




    int scale = 0;
    int livestock = 0;
    char* cropping_system = "0";
    int S_SoilYear = 1;
    int S_ThisYear = 0;
    char* DatabaseName = "None";
    char* r_Province = "None";
    char* ScenarioName = "None";
    float ha = 0.0;
    int ProvinceOrder = 0;
    int S_county = 0;
    int CountyIDX = 0;
    int landuse = 0;
    int StartYear = 0;
    //int ifdaily = 1;
    int MonteCarlo = 0;
    int ZXH = 0;
    int Batch = 0;
    int GlobeData = 1;
    char* DirRoot = "None";
    int sample= 0;
    int Start_Cell = 0;
    int IrriType = 0;
    int country_ID = 0;
    int Monitor = 0;

    char result_path[3072];
    char DailyResult[3072];

    //sprintf(result_path,"%s/Result",root_path);

    strcpy(result_path,RESULT_Path.c_str());
    sprintf(DailyResult,"%s/Site_Daily_Result.db",result_path);

    /// Initialization form the DND saved file

//     ReadInputDatafromDND("c:\\DNDC/debug.dnd","C:\\DNDC/Result","C:\\DNDC");

    Site_BaseInformation IData;

    // initialization of the input files

    class_model *myDNDC = new class_model(root_path,result_path);
    ReadInputDatafromDND(dnd_path,result_path,root_path,&IData);
//    DNDC.ReadInputDatafromXML(xmlpath,result_path,root_path,&IData);

    int S_SimuYears = IData.years;

    for(int ThisYear=1; ThisYear<=S_SimuYears; ThisYear++)
    {

        int drc = 1;
        int DroughtID= 0;
        //char f_region[5][150]= {"",""};
        char f_region[6][3072]= {""};
        char* DroughtYield = "0";
        char* DroughtDayProfile = "0";

        cout << "Year: " << ThisYear << endl;

        myDNDC->DNDCgo_zf(scale, S_SoilYear, ha, cropping_system, ProvinceOrder,
                          S_county, CountyIDX, landuse, MonteCarlo, IData.ifdaily, ThisYear,
                          r_Province, Batch, sample, Start_Cell, IrriType, country_ID, livestock,
                          Monitor,drc,DroughtID, f_region, DroughtYield,DroughtDayProfile,DailyResult,0,f_region);

    }

    delete myDNDC;
    myDNDC = NULL;
}


void Savesite(char* out_name,char* out_path)
{
    /// Save site configure files

    char cst[3072];

    char *fname;
    int l, rrr, yyy;
    fname = out_name;
    strcmp(cst,fname);
    Site_BaseInformation IData;



    char DB[3072];
    FILE *db;

    sprintf(DB,"%s/Inter/CropRotation.txt", out_path);
    db=fopen(DB, "r");
    if(db==NULL)
    {
        cout << "Can not open file " << DB << endl;
        exit(0);
    }
    fscanf(db, "%d", &IData.db_Rotation_Number);
    fclose(db);

    for(rrr=1; rrr<=IData.db_Rotation_Number; rrr++)
    {
        sprintf(DB,"%s/Inter/CropRotation_%d.txt", out_path, rrr);
        db=fopen(DB, "r");
        fscanf(db, "%d %d %d", &IData.db_RotationID[rrr], &IData.db_RotateSpan[rrr], &IData.db_CycleSpan[rrr]);
        fclose(db);

        for(yyy=1; yyy<=IData.db_CycleSpan[rrr]; yyy++)
        {
            sprintf(DB,"%s/Inter/CropRotation_%d_%d.txt", out_path, rrr, yyy);
            db=fopen(DB, "r");
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
                fscanf(db, "%f", &IData.db_IniBiomass[rrr][yyy][ccc]);
                fscanf(db, "%d", &IData.db_CoverCrop[rrr][yyy][ccc]);
                fscanf(db, "%d", &IData.db_PerennialCrop[rrr][yyy][ccc]);

                fscanf(db, "%f %f %f", &IData.db_GrainFraction[rrr][yyy][ccc], &IData.db_ShootFraction[rrr][yyy][ccc], &IData.db_RootFraction[rrr][yyy][ccc]);
                fscanf(db, "%f %f %f", &IData.db_GrainCN[rrr][yyy][ccc], &IData.db_ShootCN[rrr][yyy][ccc], &IData.db_RootCN[rrr][yyy][ccc]);
                fscanf(db, "%f %f %f %f %f", &IData.db_TDD[rrr][yyy][ccc], &IData.db_Water[rrr][yyy][ccc], &IData.db_LAI[rrr][yyy][ccc], &IData.db_Nfix[rrr][yyy][ccc], &IData.db_Vascularity[rrr][yyy][ccc]);
            }
            fclose(db);
        }
    }

    SaveDatatoFile(cst, 1,out_path);
}

void SaveDatatoFile(char *fname, int mode,char *out_path)
{

    /// save site configure file
    /// Only for site
    int i, j, k, n, CropModel, rrr;
    FILE *fpo;
    char Oname[3072];

    int NNN, TillID[300], TillMonth[300], TillDay[300], TillMethod[300];
    int FertMonth[300], FertDay[300], FertMethod[300];
    float FertNitrate[300], FertAbi[300], FertUrea[300], FertAnh[300], FertAmNi[300], NH42SO4[300], NH42HPO4[300], FertDepth[300];
    int ManuMonth[300], ManuDay[300], ManuType[300], manu_method[300];
    float ManuAmount[300], ManuCN[300], ManuN[300];
    int WeedMonth1[300], WeedDay1[300],WeedMonth2[300], WeedDay2[300];
    int FlooMonth1[300], FlooDay1[300], FlooMonth2[300], FlooDay2[300];
    float FlooN[300];
    int ShallowFlood[300];
    int IrrMode, IrriMonth[300], IrriDay[300];
    float IrrIndex, IrriAmount[300];
    int IrriMethod[300];
    int GrazMonth1[300], GrazDay1[300], GrazMonth2[300], GrazDay2[300];
    float GrazHour[300], Dairy[300], Beef[300], Pig[300], Horse[300], Sheep[300];
    char FertigationFile[200], m_ProfileName[250];;
    int CutMonth[300], CutDay[300], CutPart[300], FertOption;
    float CutAmount[300], WaterGetherIndex, WaterLeakRate;
    float DayRelease[300], NIefficiency[300], NIduration[300];
    float UreaseEfficiency[300], UreaseDuration[300];
    float m_IniWT, m_LWTceasingSurfFlow, m_LWTceasingGroungFlow, m_WatershedIndex, m_SurfOutflowIntensity, m_GroundOutflowIntensity;
    char WT_file[200];
    int FdLeak, WaterControl;
    Site_BaseInformation IData;
    StringArray ClimateFilename;
    char INTERMANAGE[3072];

    sprintf(INTERMANAGE,"%s/Result/Inter/Management",out_path);

    char CM[3072];
    FILE *cm;
    sprintf(CM,"%s/Inter/CropModel", out_path);
    cm=fopen(CM, "r");
    if(cm==NULL)
    {
        cout << "Can not open file " << CM << endl;
        exit(0);
    }
    else
    {
        fscanf(cm, "%d", &CropModel);
        fclose(cm);
        IData.CropModel = CropModel;
    }

    if(IData.Sks<0.008) IData.Sks = 0.008;

    sprintf(Oname,"%s.fn",fname);

    if ((fpo=sopen(fname,0))==NULL) return;//write

    // Save site
    fprintf(fpo,"Input_Parameters:\n");
    fprintf(fpo,"----------------------------------------\n");
    fprintf(fpo,"Site_data:                  %s\n",IData.sitename);
    fprintf(fpo,"Simulated_Year:             %d\n",IData.years);
    fprintf(fpo,"Latitude:                   %6.3f\n",IData.latitude);
    fprintf(fpo,"Daily_Record:               %6d\n",IData.ifdaily);
    //fprintf(fpo,"Climate_file_mode           %6d\n",IData.if1File);

    // Climate data
    fprintf(fpo,"----------------------------------------\n");
    fprintf(fpo,"Climate_data:\n");
    fprintf(fpo,"Climate_Data_Type:          %d\n",IData.ClimateFileType);
    fprintf(fpo,"NO3NH4_in_Rainfall          %8.4f\n",IData.NO3NH4_Rainfall);
    fprintf(fpo,"NO3_of_Atmosphere           %8.4f\n",IData.NH3_Atmosphere);
    fprintf(fpo,"BaseCO2_of_Atmosphere       %8.4f\n",IData.BaseCO2);
    fprintf(fpo,"Climate_file_count=         %d\n",IData.ClimateFileNumber);
    for (i = 0; i<IData.ClimateFileNumber; i++)
    {
        fprintf(fpo,"    %d   %s\n",i + 1, ClimateFilename.get(i + 1));;
    }
    fprintf(fpo,"Climate_file_mode           %6d\n",IData.if1File);//new
    fprintf(fpo,"CO2_increase_rate           %f\n",IData.CO2IncreaseRate);//new

    // Soil parameter
    fprintf(fpo,"----------------------------------------\n");
    fprintf(fpo,"Soil_data:\n");
    fprintf(fpo,"   Soil_Texture             %2d\n",IData.Soil_Texture);
    fprintf(fpo,"   Landuse_Type             %2d\n",IData.Soil_landuse);
    fprintf(fpo,"   Density                  %8.5f\n",IData.Soil_Density);
    fprintf(fpo,"   Soil_pH                  %8.5f\n",IData.Soil_pH);
    fprintf(fpo,"   SOC_at_Surface           %8.5f\n",IData.Soil_SOCatSurface);
    fprintf(fpo,"   Clay_fraction            %8.5f\n",IData.Soil_Clay);
    fprintf(fpo,"   BypassFlow               %8.5f\n",IData.Soil_BypassFlow);
    fprintf(fpo,"   Litter_SOC               %8.5f\n",IData.Soil_Litter);
    fprintf(fpo,"   Humads_SOC               %8.5f\n",IData.Soil_humads);
    fprintf(fpo,"   Humus_SOC                %8.5f\n",IData.Soil_humus);
    fprintf(fpo,"   Soil_NO3(-)(mgN/kg)      %8.5f\n",IData.Soil_NO3);
    fprintf(fpo,"   Soil_NH4(+)(mgN/kg)      %8.5f\n",IData.Soil_NH4);
    fprintf(fpo,"   Moisture                 %8.5f\n",IData.Soil_moiture);
    fprintf(fpo,"   Lateral_influx_index     %8.5f\n",IData.LateralInflux);


    fprintf(fpo,"Field_capacity              %f\n",IData.Fldcap);
    fprintf(fpo,"Wilting_point               %f\n",IData.Wiltpt);
    fprintf(fpo,"Hydro_conductivity          %f\n",IData.Sks);
    fprintf(fpo,"Soil_porosity               %f\n",IData.Soil_Porosity);
    fprintf(fpo,"SOC_profile_A               %f\n",IData.SOCa);
    fprintf(fpo,"SOC_profile_B               %f\n",IData.SOCb);
    fprintf(fpo,"DC_litter_factor            %f\n",IData.DClitter);
    fprintf(fpo,"DC_humads_factor            %f\n",IData.DChumads);
    fprintf(fpo,"DC_humus_factor             %f\n",IData.DChumus);
    fprintf(fpo,"Humad_CN                    %f\n",IData.HumadCN);
    fprintf(fpo,"Humus_CN                    %f\n",IData.HumusCN);
    fprintf(fpo,"Soil_PassiveC               %f\n",IData.PassiveC);
    fprintf(fpo,"Soil_microbial_index        %f\n",IData.Soil_MicrobioIndex);
    fprintf(fpo,"Highest_WT_depth               %f\n",IData.HighestWT);
    fprintf(fpo,"Depth_WRL_m                 %f\n",IData.Soil_WaterObstructLayer);
    fprintf(fpo,"Slope                       %f\n",IData.slope);
    fprintf(fpo,"Salinity                    %f\n",IData.Soil_Quality);
    fprintf(fpo,"Profile_type                %d\n",IData.ProfileType);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    char FCT30[3072];
    int TotalManureCrops;

#ifdef MANURE
    char XFF[300];
    FILE *xff;
    sprintf(XFF,"%s\\ManureCropNumber", INTER);
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
#ifdef MANURE
        sprintf(FCT30,"%s_%d", INTERMANAGE, CSL);
#else
        sprintf(FCT30,"%s_1", INTERMANAGE);
#endif

        int yyy;

        char DB[3072];
        FILE *db;

        sprintf(DB,"%s\\CropRotation.txt", FCT30);
        db=fopen(DB, "r");
        if(db==NULL) note(0,DB);
        fscanf(db, "%d", &IData.db_Rotation_Number);
        fclose(db);

        for(rrr=1; rrr<=IData.db_Rotation_Number; rrr++)
        {
            //DB.Format("%s\\inter\\CropRotation_%d.txt", OUTPUT, rrr);
            sprintf(DB,"%s\\CropRotation_%d.txt", FCT30, rrr);
            db=fopen(DB, "r");
            if(db==NULL) note(0,DB);
            fscanf(db, "%d %d %d", &IData.db_RotationID[rrr], &IData.db_RotateSpan[rrr], &IData.db_CycleSpan[rrr]);
            fclose(db);

            for(yyy=1; yyy<=IData.db_CycleSpan[rrr]; yyy++)
            {
                //DB.Format("%s\\inter\\CropRotation_%d_%d.txt", OUTPUT, rrr, yyy);
                sprintf(DB,"%s\\CropRotation_%d_%d.txt", FCT30, rrr, yyy);
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
                    fscanf(db, "%f", &IData.db_IniBiomass[rrr][yyy][ccc]);
                    fscanf(db, "%d", &IData.db_CoverCrop[rrr][yyy][ccc]);
                    fscanf(db, "%d", &IData.db_PerennialCrop[rrr][yyy][ccc]);

                    fscanf(db, "%f %f %f", &IData.db_GrainFraction[rrr][yyy][ccc], &IData.db_ShootFraction[rrr][yyy][ccc], &IData.db_RootFraction[rrr][yyy][ccc]);
                    fscanf(db, "%f %f %f", &IData.db_GrainCN[rrr][yyy][ccc], &IData.db_ShootCN[rrr][yyy][ccc], &IData.db_RootCN[rrr][yyy][ccc]);
                    fscanf(db, "%f %f %f %f %f", &IData.db_TDD[rrr][yyy][ccc], &IData.db_Water[rrr][yyy][ccc], &IData.db_LAI[rrr][yyy][ccc], &IData.db_Nfix[rrr][yyy][ccc], &IData.db_Vascularity[rrr][yyy][ccc]);
                }
                fclose(db);
            }
        }


        fprintf(fpo,"----------------------------------------\n");
        fprintf(fpo,"Crop_data:\n");
        fprintf(fpo,"Rotation_Number=            %d\n",IData.db_Rotation_Number);
        for (rrr = 1; rrr<=IData.db_Rotation_Number; rrr++)
        {
            fprintf(fpo,"  Rotation_ID=              %d\n",rrr);
            fprintf(fpo,"  Totalyear=                %d\n",	IData.db_RotateSpan[rrr]);
            fprintf(fpo,"  Years_Of_A_Cycle=         %d\n", IData.db_CycleSpan[rrr]);
            for (j = 1; j<=IData.db_CycleSpan[rrr]; j++)
            {
                fprintf(fpo,"    YearID_of_a_cycle=      %d\n",j);

                //n = IData.db_YrCrops[i][j];

                fprintf(fpo,"    Crop_total_Number=      %d\n",IData.db_YrCrops[rrr][j]);
                for (k = 1; k<= IData.db_YrCrops[rrr][j]; k++)
                {
                    fprintf(fpo,"      Crop_ID=              %d\n",k);
                    fprintf(fpo,"        Crop_Type=          %d\n",IData.db_YrCropType[rrr][j][k]);
                    fprintf(fpo,"        Plant_time=         %d %d\n", IData.db_PMonth[rrr][j][k], IData.db_PDay[rrr][j][k]);
                    fprintf(fpo,"        Harvest_time=       %d %d\n",IData.db_HMonth[rrr][j][k], IData.db_HDay[rrr][j][k]);
                    fprintf(fpo,"        Year_of_harvest=    %d\n",IData.db_HYr[rrr][j][k]);
                    fprintf(fpo,"        Ground_Residue=     %f\n",IData.db_Residue[rrr][j][k]);
                    fprintf(fpo,"        Yield=              %f\n",IData.db_Yield[rrr][j][k]);

                    fprintf(fpo,"        Rate_reproductive=  %f\n",IData.db_RepRate[rrr][j][k]);
                    fprintf(fpo,"        Rate_vegetative=    %f\n",IData.db_VegRate[rrr][j][k]);
                    fprintf(fpo,"        Psn_efficiency=     %f\n",IData.db_PsnEff[rrr][j][k]);
                    fprintf(fpo,"        Psn_maximum=        %f\n",IData.db_PsnMax[rrr][j][k]);
                    fprintf(fpo,"        Initial_biomass=    %f\n",IData.db_IniBiomass[rrr][j][k]);
                    fprintf(fpo,"        Cover_crop=         %d\n",IData.db_CoverCrop[rrr][j][k]);
                    fprintf(fpo,"        Perennial_crop=     %d\n",IData.db_PerennialCrop[rrr][j][k]);

                    fprintf(fpo,"        Grain_fraction=  %f\n",IData.db_GrainFraction[rrr][j][k]);
                    fprintf(fpo,"        Shoot_fraction=  %f\n",IData.db_ShootFraction[rrr][j][k]);
                    fprintf(fpo,"        Root_fraction=  %f\n",IData.db_RootFraction[rrr][j][k]);
                    fprintf(fpo,"        Grain_CN=  %f\n",IData.db_GrainCN[rrr][j][k]);
                    fprintf(fpo,"        Shoot_CN=  %f\n",IData.db_ShootCN[rrr][j][k]);
                    fprintf(fpo,"        Root_CN=  %f\n",IData.db_RootCN[rrr][j][k]);
                    fprintf(fpo,"        TDD=  %f\n",IData.db_TDD[rrr][j][k]);
                    fprintf(fpo,"        Water_requirement=  %f\n",IData.db_Water[rrr][j][k]);
                    fprintf(fpo,"        Max_root_depth=  %f\n",IData.db_LAI[rrr][j][k]);
                    fprintf(fpo,"        N_fixation=  %f\n",IData.db_Nfix[rrr][j][k]);
                    fprintf(fpo,"        Vascularity=  %f\n",IData.db_Vascularity[rrr][j][k]);
                }


                sprintf(DB,"%s/Inter/CropTill_%d_%d.txt", out_path, rrr, j);
                db=fopen(DB, "r");
                if(db==NULL) note(0,DB);
                fscanf(db, "%d", &NNN);
                for(k=1; k<=NNN; k++)
                {
                    TillID[k] = k;
                    fscanf(db, "%d %d %d", &TillMonth[k], &TillDay[k], &TillMethod[k]);
                }
                fclose(db);

                fprintf(fpo,"    Tillage_number=         %d\n",NNN);
                for (k = 1; k<= NNN; k++)
                {
                    fprintf(fpo,"      Tillage_ID=           %d\n",k);
                    fprintf(fpo,"        Month/Day/method=   %d %d %d\n",TillMonth[k], TillDay[k], TillMethod[k]);
                }

                sprintf(DB,"%s/Inter/CropFert_%d_%d.txt", out_path, rrr, j);
                db=fopen(DB, "r");
                if(db==NULL) note(0,DB);
                fscanf(db, "%d", &n);
                if(n==-1)
                {
                    fscanf(db, "%s", FertigationFile);
                }
                else
                {
                    for(i=1; i<=n; i++)
                    {
                        fscanf(db, "%d %d %d", &FertMonth[i], &FertDay[i], &FertMethod[i]);
                        fscanf(db, "%f %f %f %f %f %f %f %f", &FertNitrate[i], &FertAbi[i], &FertUrea[i], &FertAnh[i],
                               &FertAmNi[i], &NH42SO4[i], &NH42HPO4[i], &FertDepth[i]);
                        fscanf(db, "%f %f %f %f %f", &DayRelease[i], &NIefficiency[i], &NIduration[i],
                               &UreaseEfficiency[i], &UreaseDuration[i]);
                    }
                    fscanf(db, "%d", &FertOption);
                }
                fclose(db);

                fprintf(fpo,"    Fertil_number=          %d\n",n);
                if(n==-1)
                {
                    fprintf(fpo,"      fertigation_file=     %s\n",FertigationFile);
                }
                else
                {
                    for (k = 1; k<=n; k++)
                    {
                        fprintf(fpo,"      fertilization_ID=     %d\n",k);
                        fprintf(fpo,"        Month/Day/method=   %d %d %d\n",FertMonth[k], FertDay[k], FertMethod[k]);
                        fprintf(fpo,"        Depth=              %f\n",FertDepth[k]);
                        fprintf(fpo,"        Nitrate=            %f\n",FertNitrate[k]);
                        fprintf(fpo,"        AmmBic=             %f\n",FertAbi[k]);
                        fprintf(fpo,"        Urea=               %f\n",FertUrea[k]);
                        fprintf(fpo,"        Anh=                %f\n",FertAnh[k]);
                        fprintf(fpo,"        NH4NO3=             %f\n",FertAmNi[k]);
                        fprintf(fpo,"        NH42SO4=            %f\n",NH42SO4[k]);
                        fprintf(fpo,"        NH4HPO4=            %f\n",NH42HPO4[k]);
                        fprintf(fpo,"      Release_rate=         %f\n",DayRelease[k]);
                        fprintf(fpo,"      Inhibitor_efficiency= %f\n",NIefficiency[k]);
                        fprintf(fpo,"      Inhibitor_duration=   %f\n",NIduration[k]);
                        fprintf(fpo,"      Urease_efficiency= %f\n",UreaseEfficiency[k]);
                        fprintf(fpo,"      Urease_duration=   %f\n",UreaseDuration[k]);

                    }
                    fprintf(fpo,"      FertilizationOption=  %d\n",FertOption);

                }

                sprintf(DB,"%s/Inter/CropManu_%d_%d.txt", out_path, rrr, j);
                db=fopen(DB, "r");
                if(db==NULL) note(0,DB);
                fscanf(db, "%d", &n);
                for(i=1; i<=n; i++)
                {
                    fscanf(db, "%d %d", &ManuMonth[i], &ManuDay[i]);
                    fscanf(db, "%f %f %d %d", &ManuAmount[i], &ManuCN[i], &ManuType[i], &manu_method[i]);
                }
                fclose(db);

                fprintf(fpo,"    Manure_number=          %d\n",n);
                for (k = 1; k<= n; k++)
                {
                    fprintf(fpo,"      Manure_ID=            %d\n",k);
                    fprintf(fpo,"        Month/Day=          %d %d\n",ManuMonth[k], ManuDay[k]);
                    fprintf(fpo,"        Amount/C|N_ratio=   %f %f\n",ManuAmount[k], ManuCN[k]);
                    fprintf(fpo,"        Type=               %d\n", ManuType[k]);
                    fprintf(fpo,"        Method=             %d\n", manu_method[k]);
                }

                int WeedOption, PlasticM1, PlasticD1,PlasticM2, PlasticD2;
                sprintf(DB,"%s/Inter/CropPlastic_%d_%d.txt", out_path, rrr, j);
                db=fopen(DB, "r");
                if(db==NULL) note(0,DB);
                fscanf(db, "%d %d", &n, &WeedOption);
                for(i=1; i<=n; i++)
                {
                    fscanf(db, "%d %d", &WeedMonth1[i], &WeedDay1[i]);
                    fscanf(db, "%d %d", &WeedMonth2[i], &WeedDay2[i]);

                    //WeedMonth1[i] = JulianDay(PlasticM1, PlasticD1);
                    //WeedDay1[i] = JulianDay(PlasticM2, PlasticD2);
                }
                fclose(db);

                fprintf(fpo,"    Plastic_applications=            %d\n",n);
                fprintf(fpo,"    Ventilation=                     %d\n",WeedOption);
                for (k = 1; k<= n; k++)
                {
                    fprintf(fpo,"      Plastic_ID=           %d\n",k);
                    fprintf(fpo,"      Start/End=          %d %d\n",WeedMonth1[k], WeedDay1[k]);
                    fprintf(fpo,"      Start/End=          %d %d\n",WeedMonth2[k], WeedDay2[k]);
                }


                sprintf(DB,"%s/Inter/CropFloo_%d_%d.txt", out_path, rrr, j);
                db=fopen(DB, "r");
                if(db==NULL) note(0,DB);
                fscanf(db, "%d %d %d %f", &n, &FdLeak, &WaterControl, &WaterLeakRate);
                for(i=1; i<=n; i++)
                {
                    fscanf(db, "%d %d", &FlooMonth1[i], &FlooDay1[i]);
                    fscanf(db, "%d %d", &FlooMonth2[i], &FlooDay2[i]);
                    fscanf(db, "%f", &FlooN[i]);
                    fscanf(db, "%d", &ShallowFlood[i]);
                }

                fscanf(db, "%f", &WaterGetherIndex);
                fscanf(db, "%s", WT_file);
                fscanf(db, "%f %f %f %f %f %f", &m_IniWT, &m_LWTceasingSurfFlow, &m_LWTceasingGroungFlow, &m_WatershedIndex,
                       &m_SurfOutflowIntensity, &m_GroundOutflowIntensity);
                fclose(db);

                fprintf(fpo,"    Flood_number=           %d\n",n);
                fprintf(fpo,"    Leak_type=              %d\n",FdLeak);
                fprintf(fpo,"    Water_control=          %d\n",WaterControl);
                fprintf(fpo,"    Leak_rate=              %f\n",WaterLeakRate);
                for (k = 1; k<= n; k++)
                {
                    fprintf(fpo,"      Flooding_ID=          %d\n",k);
                    fprintf(fpo,"        Flood_Month/Day=    %d %d\n",FlooMonth1[k], FlooDay1[k]);
                    fprintf(fpo,"        Drain_Month/Day=    %d %d\n",FlooMonth2[k], FlooDay2[k]);
                    fprintf(fpo,"        Water_N=            %f\n",FlooN[k]);
                    fprintf(fpo,"        Shallow_flood=       %d\n",ShallowFlood[k]);
                }
                fprintf(fpo,"        Water_gather=   %f\n", WaterGetherIndex);
                fprintf(fpo,"        WT_file=        %s\n", WT_file);
                fprintf(fpo,"        Empirical_parameters= %f %f %f %f %f %f\n", m_IniWT, m_LWTceasingSurfFlow, m_LWTceasingGroungFlow, m_WatershedIndex,
                        m_SurfOutflowIntensity, m_GroundOutflowIntensity);



                sprintf(DB,"%s/Inter/CropIrri_%d_%d.txt", out_path, rrr, j);
                db=fopen(DB, "r");
                if(db==NULL) note(0,DB);
                fscanf(db, "%d %d %f", &n, &IrrMode, &IrrIndex);
                for(i=1; i<=n; i++)
                {
                    fscanf(db, "%d %d", &IrriMonth[i], &IrriDay[i]);
                    fscanf(db, "%f %d", &IrriAmount[i], &IrriMethod[i]);
                }
                fclose(db);

                fprintf(fpo,"    Irrigation_number=      %d\n",n);
                fprintf(fpo,"    Irrigation_type=        %d\n",IrrMode);
                fprintf(fpo,"    Irrigation_Index=       %f\n",IrrIndex);
                for (k = 1; k<= n; k++)
                {
                    fprintf(fpo,"      Irrigation_ID=        %d\n",k);
                    fprintf(fpo,"        Irr_Month/Day=      %d %d\n",IrriMonth[k], IrriDay[k]);
                    fprintf(fpo,"        Water_amount/Method=    %f %d\n",IrriAmount[k], IrriMethod[k]);
                }

                sprintf(DB,"%s/Inter/CropGraz_%d_%d.txt", out_path, rrr, j);
                db=fopen(DB, "r");
                if(db==NULL) note(0,DB);
                fscanf(db, "%d", &n);
                for(i=1; i<=n; i++)
                {
                    fscanf(db, "%d %d", &GrazMonth1[i], &GrazDay1[i]);
                    fscanf(db, "%d %d", &GrazMonth2[i], &GrazDay2[i]);
                    fscanf(db, "%f", &GrazHour[i]);
                    fscanf(db, "%f %f %f %f %f", &Dairy[i], &Beef[i], &Pig[i], &Sheep[i], &Horse[i]);
                }
                fclose(db);

                fprintf(fpo,"    Grazing_number=           %d\n",n);
                for (k = 1; k<= n; k++)
                {
                    fprintf(fpo,"      Grazing_ID=           %d\n",k);
                    fprintf(fpo,"        Start_Month/Day=    %d %d\n",GrazMonth1[k], GrazDay1[k]);
                    fprintf(fpo,"        End_Month/Day=      %d %d\n",GrazMonth2[k], GrazDay2[k]);
                    fprintf(fpo,"        Dairy_Head_per_ha= %f\n",Dairy[k]);
                    fprintf(fpo,"        Beef_Head_per_ha= %f\n",Beef[k]);
                    fprintf(fpo,"        Pig_Head_per_ha= %f\n",Pig[k]);
                    fprintf(fpo,"        Sheep_Head_per_ha_= %f\n",Sheep[k]);
                    fprintf(fpo,"        Horse_Head_per_ha_= %f\n",Horse[k]);
                    fprintf(fpo,"        Graz_Hours_Per_day= %f\n",GrazHour[k]);
                }

                sprintf(DB,"%s/Inter/GrassCut_%d_%d.txt", out_path, rrr, j);
                db=fopen(DB, "r");
                if(db==NULL) note(0,DB);
                fscanf(db, "%d", &n);
                for(i=1; i<=n; i++)
                {
                    fscanf(db, "%d %d", &CutMonth[i], &CutDay[i]);
                    fscanf(db, "%f", &CutAmount[i]);
                    fscanf(db, "%d", &CutPart[i]);
                }
                fclose(db);

                fprintf(fpo,"    Cut_number=           %d\n",n);
                for (k = 1; k<= n; k++)
                {
                    fprintf(fpo,"      Cut_ID=           %d\n",k);
                    fprintf(fpo,"        Cut_Month/Day=    %d %d\n",CutMonth[k], CutDay[k]);
                    fprintf(fpo,"        Cut_amount= %f\n",CutAmount[k]);
                    fprintf(fpo,"        Cut_part= %d\n",CutPart[k]);
                }

            }
        }

        fprintf(fpo,"Crop_model_approach         %d\n\n",IData.CropModel);

#ifdef MANURE

        char ManureHouseName[300], ManurePenName[300];
        int AnimalType, m_ProductionOption, m_GroundSurface, m_Ventilation, m_Bedding, InputFormat, AnimalTypes;
        int m_BedType, BedMonth[366], BedDay[366];
        float m_BedApplications, m_BedCN, m_BedAmount, m_VentRate, m_AnimalHeads, m_FeedRate, m_FeedProtein, m_GroundArea;

        int PenAnimalType, PenGroundSurface, PenInputFormat, PenAnimalTypes;
        int PenBedType, PenBedApplications, PenBedMonth[366], PenBedDay[366];
        float PenAnimalHeads, PenBedCN, PenBedAmount, PenFeedRate, PenFeedProtein, PenGroundArea;

        string ManureFieldName;
        int FieldInputFormat, FieldAnimalType, FieldGrazeTimes, IniMonth[366], IniDay[366], EndMonth[366], EndDay[366];
        float FieldHeads, FieldHrs, FieldArea;

        float LagoonCapacity, LagoonSurfaceArea, LagoonDays, LagoonToField, LagoonToMarket, LagoonToRemain;
        int LagoonCover, LagoonRain;

        float m_CompostDensity, m_CompostDays, m_LitterAmount, m_LitterCN, m_ToField, m_ToMarket, m_Remain;
        float m_DigesterCH4, m_DigesterDays, m_DigesterToField, m_DigesterToMarket, m_DigesterRemain;
        int m_Temp;

        float m_Frequency, m_UC, m_UL, m_UD, m_UR, m_FC, m_FL, m_FD, m_FR;
        float m_Frequencyp, m_UCp, m_ULp, m_UDp, m_URp, m_FCp, m_FLp, m_FDp, m_FRp;
        float m_Frequencyf, m_USf, m_URf, m_FSf, m_FRf, m_FieldArea;

        CM.Format("%s/Inter/ManureHouse.txt", OUTPUT);
        cm=fopen(CM, "r");
        if(cm==NULL) note(0, CM);

        fscanf(cm, "%d", &InputFormat);
        fprintf(fpo,"HouseLivestockInputFormat				%d\n",InputFormat);
        if(InputFormat==1)
        {
            //Open a file to read ferd, feed data
            fscanf(cm, "%s", ManureHouseName);
            fprintf(fpo,"HouseInputFileName				%s\n",ManureHouseName);
        }
        else
        {
            fscanf(cm, "%d", &AnimalTypes);
            fprintf(fpo,"HouseAnimalTypes				%d\n",AnimalTypes);

            fscanf(cm, "%d %f %f %f", &AnimalType, &m_AnimalHeads, &m_FeedRate, &m_FeedProtein);
            fprintf(fpo,"Herd_Feed	%d %f %f %f\n",AnimalType, m_AnimalHeads, m_FeedRate, m_FeedProtein);

            fscanf(cm, "%f %d", &m_GroundArea, &m_GroundSurface);//0 slatted floor; 1 conrete floor
            fscanf(cm, "%d  %f  %f %f", &m_BedType, &m_BedCN, &m_BedApplications, &m_BedAmount);//0 None, 1 straw, 2 sawdust, 3 dry manure solids, 4 sand
            fprintf(fpo,"HouseSurface	%f %d %d %f %f %f\n", m_GroundArea, m_GroundSurface, m_BedType, m_BedCN, m_BedApplications, m_BedAmount);

            fscanf(cm, "%d  %f", &m_Ventilation, &m_VentRate);// 0 natural, 1 fan
            fprintf(fpo,"Ventilation	%d %f\n", m_Ventilation, m_VentRate);

            fscanf(cm, "%f", &m_Frequency);
            fscanf(cm, "%f %f %f %f", &m_UC, &m_UL, &m_UD, &m_UR);
            fscanf(cm, "%f %f %f %f", &m_FC, &m_FL, &m_FD, &m_FR);

            fprintf(fpo,"HouseManureOut		%f\n", m_Frequency);
            fprintf(fpo,"UrineGo		%f %f %f %f\n", m_UC, m_UL, m_UD, m_UR);
            fprintf(fpo,"FecesGo		%f %f %f %f\n\n",  m_FC, m_FL, m_FD, m_FR);
        }
        fclose(cm);

        sprintf(CM,"%s/Inter/ManurePen.txt", out_path);
        cm=fopen(CM, "r");
        if(cm==NULL) note(0, CM);

        fscanf(cm, "%d", &PenInputFormat);
        fprintf(fpo,"PenLivestockInputFormat	%d\n", PenInputFormat);
        if(PenInputFormat==1)
        {
            //Open a file to read ferd, feed data
            fscanf(cm, "%s", ManurePenName);
            fprintf(fpo,"PenFileName	%s\n", ManurePenName);
        }
        else
        {
            fscanf(cm, "%d", &PenAnimalTypes);
            fprintf(fpo,"PenAnimalTypes		%d\n", PenAnimalTypes);
            fscanf(cm, "%d %f %f %f", &PenAnimalType, &PenAnimalHeads, &PenFeedRate, &PenFeedProtein);
            fprintf(fpo,"Herd_Feed		%d %f %f %f\n", PenAnimalType, PenAnimalHeads, PenFeedRate, PenFeedProtein);
            fscanf(cm, "%f %d", &PenGroundArea, &PenGroundSurface);//0 slatted floor; 1 conrete floor
            fscanf(cm, "%d  %f  %f %f", &PenBedType, &PenBedCN, &PenBedApplications, &PenBedAmount);//0 None, 1 straw, 2 sawdust, 3 dry manure solids, 4 sand
            fscanf(cm, "%f", &m_Frequencyp);
            fscanf(cm, "%f %f %f %f", &m_UCp, &m_ULp, &m_UDp, &m_URp);
            fscanf(cm, "%f %f %f %f", &m_FCp, &m_FLp, &m_FDp, &m_FRp);
            fprintf(fpo,"PenSurface		%f %d %d %f %f %f\n", PenGroundArea, PenGroundSurface, PenBedType, PenBedCN, PenBedApplications, PenBedAmount);
            fprintf(fpo,"PenManureOut		%f\n", m_Frequencyp);
            fprintf(fpo,"UrineGo		%f %f %f %f\n", m_UCp, m_ULp, m_UDp, m_URp);
            fprintf(fpo,"FecesGo		%f %f %f %f\n\n", m_FCp, m_FLp, m_FDp, m_FRp);
        }
        fclose(cm);

        CM.Format("%s/Inter/ManureGraze.txt", OUTPUT);
        cm=fopen(CM, "r");
        if(cm==NULL) note(0, CM);

        fscanf(cm, "%d", &FieldInputFormat);
        fprintf(fpo,"GrazeLivestockInputFormat		%d\n", FieldInputFormat);
        if(FieldInputFormat==1)
        {
            fscanf(cm, "%s", ManureFieldName);
            fprintf(fpo,"GrazeFileName		%s\n", ManureFieldName);
        }
        else
        {
            fscanf(cm, "%d %d", &FieldAnimalType, &FieldGrazeTimes);
            fprintf(fpo,"GrazeAnimalType		%d\n", FieldAnimalType);
            fprintf(fpo,"GrazeGrazeTimes		%d\n", FieldGrazeTimes);
            for(i=1; i<=FieldGrazeTimes; i++)
            {
                fscanf(cm, "%d %d %d %d %f %f", &IniMonth[i], &IniDay[i], &EndMonth[i], &EndDay[i], &FieldHeads, &FieldHrs);
                fprintf(fpo,"Grazing		%d %d %d %d %f %f\n", IniMonth[i], IniDay[i], EndMonth[i], EndDay[i], FieldHeads, FieldHrs);
            }
            fscanf(cm, "%f", &FieldArea);
            fprintf(fpo,"GrazeArea		%f\n\n", FieldArea);
        }
        fclose(cm);

        CM.Format("%s/Inter/ManureLagoon.txt", OUTPUT);
        cm=fopen(CM, "r");
        if(cm==NULL) note(0, CM);

        fscanf(cm, "%f %f %d %d %f", &LagoonCapacity, &LagoonSurfaceArea, &LagoonCover,	&LagoonRain, &LagoonDays);
        fscanf(cm, "%f %f %f", &LagoonToField, &LagoonToMarket, &LagoonToRemain);
        fprintf(fpo,"Lagoon		%f %f %d %d %f\n", LagoonCapacity, LagoonSurfaceArea, LagoonCover, LagoonRain, LagoonDays);
        fprintf(fpo,"SlurryPartition		%f %f %f\n\n", LagoonToField, LagoonToMarket, LagoonToRemain);
        fclose(cm);

        string SF;
        FILE* sf;
        SF.Format("%s/Inter/ManureCompost.txt", OUTPUT);
        sf=fopen(SF, "r");
        if(sf==NULL) note(0, SF);
        fscanf(sf, "%f  %f  %f  %f", &m_CompostDensity, &m_CompostDays, &m_LitterAmount, &m_LitterCN);
        fscanf(sf, "%f  %f  %f", &m_ToField, &m_ToMarket, &m_Remain);
        fprintf(fpo,"Compost		%f %f %f %f\n", m_CompostDensity, m_CompostDays, m_LitterAmount, m_LitterCN);
        fprintf(fpo,"CompostPartition		%f %f %f\n\n", m_ToField, m_ToMarket, m_Remain);
        fclose(sf);

        SF.Format("%s/Inter/ManureDigester.txt", OUTPUT);
        sf=fopen(SF, "r");
        if(sf==NULL) note(0,SF);
        fscanf(sf, "%d  %f  %f", &m_Temp, &m_DigesterCH4, &m_DigesterDays);
        fscanf(sf, "%f  %f  %f", &m_ToField, &m_ToMarket, &m_Remain);

        fprintf(fpo,"Digester		%d %f %f\n", m_Temp, m_DigesterCH4, m_DigesterDays);
        fprintf(fpo,"ResiduePartition		%f %f %f\n\n", m_ToField, m_ToMarket, m_Remain);
        fclose(sf);

        sprintf(CM,"%s/Inter/ManureField.txt", out_path);
        sf=fopen(CM, "r");
        if(sf==NULL) note(0,CM);
        fscanf(sf, "%f", &m_FieldArea);
        fclose(sf);

        if(m_FieldArea<=0.0) m_FieldArea = 1.0;

        fprintf(fpo,"Land_area		%f\n", m_FieldArea);

#endif
        if(IData.ProfileType==1)
        {
            float m_SPth[11],m_SPd[11], m_SPsoc[11], m_SPph[11], m_SPfc[11], m_SPwp[11], m_SPpo[11], m_SPhy[11], m_SPc[11];
            float m_SPthickness;
            int m_SPso[11], layer[11],m_SPlayers, i;
            char NN[300], m_ProfileName[250];
            FILE* ef;
            sprintf(NN, "%s\\Inter\\SoilProfileData.spf", out_path);
            ef=fopen(NN, "r");
            if(ef==NULL) note(0, NN);

            fscanf(ef, "%f %d", &m_SPthickness, &m_SPlayers);
            for(i=1; i<=10; i++)
            {
                fscanf(ef, "%d %f %f %f %d %f %f %f %f %f %f",
                       &layer[i], &m_SPth[i], &m_SPd[i], &m_SPsoc[i], &m_SPso[i], &m_SPph[i], &m_SPfc[i], &m_SPwp[i], &m_SPpo[i], &m_SPhy[i], &m_SPc[i]);
            }
            fscanf(ef, "%s", m_ProfileName);
            fclose(ef);

            ////////////////////////////////////////////////////////////////

            fprintf(fpo, "%f %d\n", m_SPthickness, m_SPlayers);
            for(i=1; i<=10; i++)
            {
                fprintf(fpo, "%d %f %f %f %d %f %f %f %f %f %f\n",
                        i, m_SPth[i], m_SPd[i], m_SPsoc[i], m_SPso[i], m_SPph[i], m_SPfc[i], m_SPwp[i], m_SPpo[i], m_SPhy[i], m_SPc[i]);
            }
            fprintf(fpo, "%s\n", m_ProfileName);
        }

        fclose (fpo);
    }

}
void OpenRegion(char* region_name,char *out_path, char *root_path,char* Drought_file,char* Log_file)
{
    /// Open Region configure file
    /// debug parameters: c:/DNDC/debug.drd C:/DNDC/Result C:/DNDC

    cout << "Load Region Configure file" << endl;

    int i, RegionCode, CountryCode, StateCode, DBType, MajorConcern, DailyRecord, ViewMode, FarmFile, GlobeData, years;
    float r_CO2, r_AirTemp, r_YieldIncrease, jf;
    char RegionName[300],RegionName2[300],ScenarioName[300];
    float y_AtmoCO2, y_AirTemp, y_YieldIncrease, y_RainChange, y_MicroIndex;
    float y_LitterBackField, y_AnimalManure, y_FertilizerRate, y_IrriIndex;
    int y_Tillage, Drainage, Autofert, y_ShallowFlood, y_UplandRice, y_FertSO4, y_SlowReleaseDays;
    int m_SimuYears = 0, m_StartYear,m_StartGridID,m_TotalCells, m_SimulatedCells, m_SoilDataUse;
    char m_RegionName[300],m_ScenarioName[300];

    int m_DataFormat = 0;
    int m_Drought = 0;

    char ConName[3072], FileName[3072], Root1[3072], RootName[3072], WholeName[3072];
    //void www(int year);
    int ManageFlag=0, SoilDataUse=0;


    char cst[3072];
    strcpy(cst,region_name);
    if (strlen(cst)==0) return;

    //char [3072],
    char RegionFile[3072];
    FILE *db, *rf;

    sprintf(RegionFile,"%s", cst);

    rf = fopen(RegionFile, "r");
    if(rf==NULL) error_print(0,RegionFile);

    fscanf(rf, "%s", RegionName);
    fscanf(rf, "%s", WholeName);
    fscanf(rf, "%s", ScenarioName);
    fscanf(rf, "%d", &m_DataFormat);
    fscanf(rf, "%d %d %d %d", &MajorConcern, &DailyRecord, &m_SimuYears, &m_StartYear);
    fscanf(rf, "%d", &m_StartGridID);
    fscanf(rf, "%d %d %d", &m_TotalCells, &m_SimulatedCells, &m_SoilDataUse);
    fscanf(rf, "%d", &m_Drought);

    fscanf(rf, "%d", &years);

//    if (m_Drought == 1) years = 2;
    if (m_SimuYears != years)
    {
        cout << "Regional Configure file error: Simulation years not equal to real years" << endl;
        exit(0);
    }
    for(int i=1;  i<=years; i++)
    {
        fscanf(rf, "%f %f %f", &y_AtmoCO2, &y_AirTemp, &y_YieldIncrease);
        fscanf(rf, "%f %f %f", &y_LitterBackField, &y_AnimalManure, &jf);
        fscanf(rf, "%f %f", &y_FertilizerRate, &y_IrriIndex);
        fscanf(rf, "%d", &y_Tillage);
        fscanf(rf, "%d", &Drainage);
        fscanf(rf, "%d", &y_ShallowFlood);
        fscanf(rf, "%d", &y_UplandRice);
        fscanf(rf, "%d", &Autofert);
        fscanf(rf, "%d", &y_SlowReleaseDays);
        fscanf(rf, "%f", &y_RainChange);
        fscanf(rf, "%f", &y_MicroIndex);

        char RegYr[3072];
        FILE* regyr;
        sprintf(RegYr,"%s/Inter/Region_yr%d", out_path, i);
        regyr = fopen(RegYr, "w");
        if(regyr==NULL)
        {
            cout << "Can not create file " << RegYr << endl;
            exit(0);
        }

        fprintf(regyr, "%f %f %f %f\n", y_AtmoCO2, y_AirTemp, y_YieldIncrease, y_LitterBackField);
        fprintf(regyr, "%f %f %f %f\n", y_AnimalManure, jf, y_FertilizerRate, y_IrriIndex);
        fprintf(regyr, "%d %d %d %d %d %d\n", y_Tillage, Drainage, y_ShallowFlood, y_UplandRice, Autofert, y_SlowReleaseDays);
        fprintf(regyr, "%f %f\n", y_RainChange, y_MicroIndex);
        fclose(regyr);
    }

    fclose(rf);

    sprintf(m_RegionName,"%s", RegionName);
    sprintf(m_ScenarioName,"%s", ScenarioName);

    char RegionPath[3072];
    sprintf(RegionPath,"%s/Record/Region/%s", out_path, RegionName);
    mkpath(RegionPath,777);
    sprintf(RegionPath,"%s/Record/Region/%s/%s", out_path, RegionName, ScenarioName);
    mkpath(RegionPath,777);
    sprintf(RegionPath,"%s/Record/Region/%s/%s/publish", out_path, RegionName, ScenarioName);
    mkpath(RegionPath,777);


    char CP[200];

    int L1, T=0, G[20]= {0,0};

    int LL=strlen(WholeName);
    for(i=LL; i>=0; i--)
    {
        if(WholeName[i]=='_')
        {
            L1=i-1;
            break;
        }
    }

    for(i=0; i<=LL; i++)
    {
        if(WholeName[i]=='\\')
        {
            T++;
            G[T]=i;
        }
    }


    //strcpy(ConName ,(string(WholeName).substr(G[T]+1, L1)).c_str());
    strcpy(ConName ,(string(WholeName).substr(G[T]+1, abs(G[T] - L1))).c_str());
    // RootName is GIS dir path, such as: C:\DNDC\Database\liaoning\GIS_CountyBased
    strcpy(RootName, (string(WholeName).substr(0, G[T])).c_str());

    strcpy(Root1,(string(WholeName).substr(0, G[T-1])).c_str());

    cout << "Test WholeName---->" << endl;
    cout << WholeName << endl;

    cout << "Test ConName---->" << endl;
    cout << ConName << endl;

    cout << "Test RootName ---->" << endl;
    cout << RootName << endl;

//    cout << "Test Root1 ---->" << endl;
//    cout << Root1 << endl;


/// Initial some variables   ////////////////////////
    char RegYr[3072];
    FILE* regyr;
    sprintf(RegYr,"%s/Inter/RegionInputFileFlag", out_path);
    regyr = fopen(RegYr, "w");
    if(regyr==NULL)
    {
        cout << "Can not create file " << RegYr << endl;
        exit(0);
    }
    fprintf(regyr, "%d\n", 1);
    fclose(regyr);

//	char DM[255];
//	FILE *dm;
//	int DroughtMode=0;
//	sprintf(DM,"%s/Inter/DroughtMode", out_path);
//	dm=fopen(DM, "r");
//	fscanf(dm, "%d", &DroughtMode);
//	fclose(dm);
//
//	m_Drought = DroughtMode;

/////////////////////////////////////////////////////

    RegionCode = 0;

    char CYW[3072];
    FILE* cyw;
    sprintf(CYW,"%s/Inter/TargetYr", out_path);
    cyw=fopen(CYW, "w");
    if(cyw==NULL) note(1, CYW);
    fprintf(cyw, "%d", m_StartYear);
    fclose(cyw);

    ManageFlag=1;



///////////////////////////////////////////////////////////


    /// TODO: Define the simulated Drought day
    /// m_YieldMonth and m_YieldToday are drought simulation start day
    /// m_YieldCrop is simulated crop type,m_YieldYear
    /// id is the simulate cell day
    /// TODO, Add a new configure file




    /// Default Drought sneario
    string CellID = "210000";
    string CropType = "1";
    string m_YieldMonth = "5";
    string m_YieldToday = "1";
    string m_YieldCrop = "1";
    string m_YieldYear = "2000";

    string shapefile;

//if (m_Drought == 1)
//{



    ifstream config_in(Drought_file,ios::in);

    if (config_in.fail())
    {
        cout << "Configuration file " << Drought_file << "  cannot found" << endl;;
        return;
    }

    char line[500];

    //read the Drought information
    std::getline(config_in,CellID);
    std::getline(config_in, m_YieldCrop);// >> m_YieldCrop;
    std::getline(config_in,m_YieldYear);
    std::getline(config_in, m_YieldMonth);
    std::getline(config_in,m_YieldToday);
    std::getline(config_in,shapefile);
    //config_in.getline(line, CellID);// >> CellID;
//    config_in.getline(line, m_YieldCrop);// >> m_YieldCrop;
//    config_in.getline(line, 500) >> m_YieldYear;
//    config_in.getline(line, 500) >> m_YieldMonth;
//    config_in.getline(line, 500) >> m_YieldToday;

//    config_in.getline(line,500) >> shapefile;


    // Set the scenario year to the last year of the simulation.
    int simu_yr = m_StartYear + m_SimuYears -1;
    if (atoi(m_YieldYear.c_str()) != simu_yr)
    {
        cout << "scenario year should same as the last simulation year. " << endl;
        m_YieldYear == int2str(simu_yr);
    }

#ifdef DEBUG
    PR(shapefile);
    PR(CellID);
    PR(m_YieldCrop);
    PR(m_YieldMonth);
    PR(m_YieldToday);
#endif
    int Jday;
    Jday = JulianDay(atoi(m_YieldMonth.c_str()), atoi(m_YieldToday.c_str()));

    PR(Jday);

    //int site = m_YieldLocation;

    char fname[3072], str[3072];
    int GridNum, id;

    FILE *fp;
    sprintf(fname,"%s/Liaoning_GridList.txt", RootName);
    PR(RootName);
    fp=fopen(fname,"r");

    fscanf(fp,"%d",&GridNum);
    for (i=0; i<GridNum; i++)
    {
        //if (fscanf(fp,"%d %s",&id, str)!=2) break;

        fscanf(fp,"%d %s",&id,&str);
        PR(id);
        PR(str);
        if (id = atoi(CellID.c_str()))
            break;
    }

    fclose(fp);

    char YF[3072];
    FILE* yf;
    sprintf(YF,"%s/Inter/YieldPrediction", out_path);
    yf=fopen(YF, "w");
    if(yf==NULL) note(1,YF);

    fprintf(yf, "%s  %s  %s  %d  %s  %s  %s", CellID.c_str(), m_YieldCrop.c_str(), m_YieldYear.c_str(), Jday, str, m_YieldMonth.c_str(), m_YieldToday.c_str());
    fclose(yf);

//}

///////////////////////////////////////////////////////////

    char SCF[3072];
    FILE *scf;
    sprintf(SCF,"%s/Inter/RegionSelectCrops.txt", out_path);
    scf=fopen(SCF, "w");
    if(scf==NULL)
    {
        cout << "Can not create file " << SCF << endl;
        exit(0);
    }
    fprintf(scf, "%d\n", 0);
    fclose(scf);

    char Fregion[3072];
    FILE *RegionState;

    sprintf(Fregion,"%s/Inter/CountryState", out_path);
    RegionState = fopen(Fregion, "w");

    if (RegionState==NULL)
    {
        cout << "Can not create file " << Fregion << endl;
        exit(0);
    }

    fprintf(RegionState, "%d %d\n", -1, 0);
    fprintf(RegionState, "%s\n", CP);
    fprintf(RegionState, "%s\n", Root1);
    fprintf(RegionState, "%s\n", RootName);
    fprintf(RegionState, "%s\n", WholeName);
    fclose (RegionState);

    PR(RootName);



///////////////////////////////////////////// Write Region File /////////////////////////////////////

    //m_SimulatedCells = m_TotalCells - m_StartGridID + 1;

    char r_Country[3072], r_Province[3072];
    char r_Scenario[3072], r_DatabaseName[3072];
    float CO2, AirTemp, YieldIncrease;
    int r_ViewMode;
    int region_code=0, state_code, country_code, SelectedCrops, SC[150];

//    char states[120];
//    FILE *cs;
//    sprintf(states, "%s/Inter/CountryState", out_path);
//    cs = fopen(states, "r");
//    if(cs==NULL)
//    {
//        note(1, states);
//    }
//    fscanf(cs, "%d %d", &country_code, &state_code);
//    fscanf(cs, "%s", r_Province);
//    fscanf(cs, "%s", Root1);
//    fscanf(cs, "%s", RootName);
//    fscanf(cs, "%s", WholeName);
//    fclose (cs);
    PR(RootName);
    //char SCF[255];
    //FILE *scf;
    sprintf(SCF,"%s/Inter/RegionSelectCrops.txt", out_path);
    scf=fopen(SCF, "r");
    if(scf==NULL)
    {
        note(1, SCF);
    }

    fscanf(scf, "%d", &SelectedCrops);
    if(SelectedCrops>0)
    {
        for(i=1; i<=SelectedCrops; i++)
            fscanf(scf, "%d", &SC[i]);
    }
    fclose(scf);


    if(m_SimuYears==0)
    {
        cout << "Please define number of simulated years" << endl;
        return;
    }

    if(m_StartYear==0)
    {
        cout << "Please define the start year" << endl;
        return;
    }

    /*if(m_FarmFile==0)
    	FarmFile=1;
    else
    	FarmFile=0;*/

    FarmFile=1;

    char XZY[3072];
    sprintf(XZY, "%s",  m_RegionName);
    LL=strlen(XZY);

    if(LL>=12)
    {
        Glean_String(XZY, 0, 11, XZY);
    }
    else
    {
        int RLL=12-LL;
        for(i=1; i<=RLL; i++) sprintf(XZY, "%s_", XZY);
    }

    sprintf(r_DatabaseName, "%s", XZY);

    sprintf(r_Country, "%s", m_RegionName);
    sprintf(r_Scenario, "%s", m_ScenarioName);

    CO2 = 350.0;//m_AtmoCO2;
    AirTemp = 0.0;//m_AirTemp;
    YieldIncrease = 1.0;//m_YieldIncrease;

    r_ViewMode = 0;
    sprintf(r_DatabaseName, "%s", r_DatabaseName);

    FILE *Region;
    sprintf(Fregion, "%s/Inter/Region", out_path);
    Region = fopen(Fregion, "w");

    fprintf(Region, "%d %d %d\n", region_code, country_code, state_code);
    fprintf(Region, "%s\n", r_Country);
    fprintf(Region, "%s\n", Root1);
    fprintf(Region, "%s\n", WholeName);//r_DatabaseName);
    fprintf(Region, "%s\n", r_Scenario);
    fprintf(Region, "%d\n", 0);
    fprintf(Region, "%f %f %f\n", CO2, AirTemp, YieldIncrease);
    fprintf(Region, "%d\n", MajorConcern);
    fprintf(Region, "%d\n", DailyRecord);
    fprintf(Region, "%d\n", m_SimuYears);
    fprintf(Region, "%d\n", m_StartYear);
    fprintf(Region, "%d\n", r_ViewMode);
    fprintf(Region, "%d\n", FarmFile);
    fprintf(Region, "%d\n", m_DataFormat);
    fprintf(Region, "%d\n", m_StartGridID);
    fprintf(Region, "%d\n", m_TotalCells);
    fprintf(Region, "%d\n", m_SimulatedCells);
    fprintf(Region, "%d\n", m_SoilDataUse);
    fprintf(Region, "%d\n", m_Drought);
    fclose(Region);



///////////////////////////////////////////////// END ////////////////////////////////////////////////

    cout << "Prepare Regional Data" << endl;
//    cout << "Root Name " << endl;
//    cout << RootName << endl;
    PR(RootName);
    RegionDataConversion(out_path,ConName,RootName, WholeName,m_TotalCells,m_RegionName, m_DataFormat,m_Drought);
//    RegionDataBase(out_path,ConName,RootName, WholeName,m_TotalCells,m_RegionName, m_DataFormat);

//    string DB_Path = "c:/test";
//    PR(DB_Path);
    char tmp[3072];
    strcpy(tmp,shapefile.c_str());

    Region_Simulator(root_path,out_path,tmp,Log_file);
//    sprintf(DB_Path,"%s/Record/Region/%s/%s/Daily%s_%s_Yr%d_%s.txt",  OUTPUT,
//                        CountryName, ScenarioName, DI[di].c_str(), r_Province, Yy, SSYY[si].c_str());




}


void Saveregion(char* out_name, char* out_path)
{
    /// Save Region config file
    char cst[3072];



    int RegionCode, CountryCode, StateCode, DBType, MajorConcern, DailyRecord, SimuYears, StartYear, ViewMode;
    int FarmFile, GlobeData, StartGridID, TotalCells, SimulatedCells;
    char r_Country[3072], js[3072], mDatabaseName[3072], r_Scenario[3072];
    float r_CO2, r_AirTemp, r_YieldIncrease, jf;
    float y_AtmoCO2, y_AirTemp, y_YieldIncrease, y_LitterBackField, y_AnimalManure, y_FertilizerRate, y_IrriIndex, y_RainChange, y_MicroIndex;
    int y_Tillage, Drainage, Autofert, y_ShallowFlood, y_UplandRice, y_FertSO4, y_SlowReleaseDays;

    char DB[3072], RegionFile[3072],fname[3072];
    int m_SimuYears, m_StartYear,m_StartGridID,m_TotalCells, m_SimulatedCells, m_SoilDataUse,SoilDataUse,years;
    char m_RegionName[3072],m_ScenarioName[3072];
    char ConName[3072] ,RootName[3072];
    FILE *db, *rf;

    char wholename[3072];
    int m_DataFormat;
    int	m_Drought;


    strcmp(fname,out_name);
    sprintf(RegionFile,"%s", fname);

    rf = fopen(RegionFile, "w");
    if(rf==NULL)
    {
        cout << "Can not create file " << RegionFile << endl;
        exit(0);

    }


//    sprintf(DB,"%s/Inter/Region", out_path);
//    db=fopen(DB, "r");
//    if(db==NULL)
//    {
//        cout << "Can not read file " << DB << endl;
//        exit(0);
//
//    }
//
//    fscanf(db, "%d %d %d", &RegionCode, &CountryCode, &StateCode);
//    fscanf(db, "%s", r_Country);
//    fscanf(db, "%s", js);
//    fscanf(db, "%s", mDatabaseName);
//    fscanf(db, "%s", r_Scenario);
//    fscanf(db, "%d", &DBType);
//    fscanf(db, "%f %f %f", &r_CO2, &r_AirTemp, &r_YieldIncrease);
//    fscanf(db, "%d", &MajorConcern);
//    fscanf(db, "%d", &DailyRecord);
//    fscanf(db, "%d", &SimuYears);
//    fscanf(db, "%d", &StartYear);
//    fscanf(db, "%d", &ViewMode);
//    fscanf(db, "%d", &FarmFile);
//    fscanf(db, "%d", &GlobeData);
//    fscanf(db, "%d", &StartGridID);
//    fscanf(db, "%d", &TotalCells);
//    fscanf(db, "%d", &SimulatedCells);
//    fscanf(db, "%d", &SoilDataUse);
//    fclose(db);





    fprintf(rf, "%s\n", m_RegionName);
    fprintf(rf, "%s\n", wholename);
    fprintf(rf, "%s\n", m_ScenarioName);
    fprintf(rf, "%d\n", m_DataFormat);
    fprintf(rf, "%d %d %d %d\n", MajorConcern, DailyRecord, m_SimuYears, m_StartYear);
    fprintf(rf, "%d\n", m_StartGridID);
    fprintf(rf, "%d %d %d\n", m_TotalCells, m_SimulatedCells, m_SoilDataUse);
    fprintf(rf, "%d\n\n", m_Drought);
//
//
//    char Fregion[120];
//    FILE *Region;
//    sprintf(Fregion, "%s/Inter/RegionYear", out_path);
//    Region = fopen(Fregion, "r");
//    if(Region==NULL)
//    {
//        cout << "Can not open file " << Fregion << endl;
//        exit(0);
//
//    }
//    fscanf(Region, "%d", &years);
//    fclose(Region);

    years = m_SimuYears;

    fprintf(rf, "%d\n\n", years);

    for(int i=1; i<=years; i++)
    {
        char RegionYr[3072];
        FILE* ry;

        sprintf(RegionYr, "%s/Inter/Region_yr%d", out_path, i);
        ry = fopen(RegionYr, "r");
        if(ry==NULL)
        {
            cout << "Can not open file " << RegionYr << endl;
            exit(0);
        }

        fscanf(ry, "%f %f %f", &y_AtmoCO2, &y_AirTemp, &y_YieldIncrease);
        fscanf(ry, "%f %f %f", &y_LitterBackField, &y_AnimalManure, &jf);
        fscanf(ry, "%f %f", &y_FertilizerRate, &y_IrriIndex);
        fscanf(ry, "%d", &y_Tillage);
        fscanf(ry, "%d", &Drainage);
        fscanf(ry, "%d", &y_ShallowFlood);
        fscanf(ry, "%d", &y_UplandRice);
        fscanf(ry, "%d", &Autofert);
        fscanf(ry, "%d", &y_SlowReleaseDays);
        fscanf(ry, "%f", &y_RainChange);
        fscanf(ry, "%f", &y_MicroIndex);

        fclose(ry);

        fprintf(rf, "%f %f %f\n", y_AtmoCO2, y_AirTemp, y_YieldIncrease);
        fprintf(rf, "%f %f %f\n", y_LitterBackField, y_AnimalManure, 0.0);
        fprintf(rf, "%f %f\n", y_FertilizerRate, y_IrriIndex);
        fprintf(rf, "%d\n", y_Tillage);
        fprintf(rf, "%d\n", Drainage);
        fprintf(rf, "%d\n", y_ShallowFlood);
        fprintf(rf, "%d\n", y_UplandRice);
        fprintf(rf, "%d\n", Autofert);
        fprintf(rf, "%d\n", y_SlowReleaseDays);
        fprintf(rf, "%f\n", y_RainChange);
        fprintf(rf, "%f\n\n", y_MicroIndex);
    }

    fclose(rf);

}



//void ReadInputDatafromDND(char *fname, char *out_path, char *root,Site_BaseInformation *IData)
//{
//    /// Load DND files,which are site configure..
//
//    cout << "Load DND file" << endl;
//    cout << "New Drought Warning System Version 9.4" << endl;
//
//    //char root_path[20] = "C:\\DNDC";
//    //char root_path[20] = root;
//    char root_path[20];
//    strcpy(root_path,root);
//
//
//
//
//
//    int  i, j, k, cropn, MissFlag=0;
//    int floon, FdLeak, ShallowFlood, WaterControl, FlooMonth1, FlooDay1, FlooMonth2, FlooDay2;
//    float FlooN, WaterGetherIndex, WaterLeakRate;
//    float m_IniWT, m_LWTceasingSurfFlow, m_LWTceasingGroungFlow, m_WatershedIndex,m_SurfOutflowIntensity, m_GroundOutflowIntensity;
//    char note[255],tmp[255];
//    FILE *fp;
//    StringArray ClimateFilename;
//    int DaymetFlag = 0;
//    int mode = 0;
//
//    // Initial IData class
////    Site_BaseInformation IData;
//
////    float nSoca[6]={0.0,0.2,0.2,0.08,0.05,0.2};
////    float nSocb[6]={10.0,2.0,2.0,1.4,4.0,2.0};
//
//
//    char txt[200];
//
//    if ((fp=sopen(fname,1))==NULL)
//    {
//        cout << "Can not open " <<  fname << endl;
//        return;
//    }
//
//    int Line=1;
//    //char vvv[200];
//
//    // Read site
//    fscanf(fp,"%s",note);
//    if(strcmp(note, "Input_Parameters:")!=0) Toend("Input_Parameters:", fname, Line);
//    Line++;
//    XX(fp);
//
//    //Site data:
//    fscanf(fp,"%s",note);//"--------------------"
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %s",note,IData->sitename);
//    if(strcmp(note, "Site_data:")!=0) Toend("Site_data:", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %d",note,&IData->years);
//    if(strcmp(note, "Simulated_Year:")!=0) Toend("Simulated_Year:", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %f", note,&IData.latitude);// "  "
//    if(strcmp(note, "Latitude:")!=0) Toend("Latitude:", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %d", note,&IData.ifdaily);	//"  "
//    if(strcmp(note, "Daily_Record:")!=0) Toend("Daily_Record:", fname, Line);
//    Line++;
//    XX(fp);
//
//    //climate data
//    fscanf(fp,"%s",note);//"--------------------"
//    Line++;
//    XX(fp);
//    fscanf(fp,"%s",note);//"Climate data:"
//    Line++;
//    XX(fp);
//    fscanf(fp,"%s %d", note,&IData.ClimateFileType);
//    if(strcmp(note, "Climate_Data_Type:")!=0) Toend("Climate_Data_Type:", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %f", note,&IData.NO3NH4_Rainfall);	//"
//    if(strcmp(note, "NO3NH4_in_Rainfall")!=0) Toend("NO3NH4_in_Rainfall", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %f", note,&IData.NH3_Atmosphere);	//"
//    if(strcmp(note, "NO3_of_Atmosphere")!=0) Toend("NO3_of_Atmosphere:", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %f", note,&IData.BaseCO2);		//"
//    if(strcmp(note, "BaseCO2_of_Atmosphere")!=0) Toend("BaseCO2_of_Atmosphere:", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %d", note,&IData.ClimateFileNumber);
//    if(strcmp(note, "Climate_file_count=")!=0) Toend("Climate_file_count:", fname, Line);
//    Line++;
//    XX(fp);
//
//    k=IData.ClimateFileType;
//    int nn=IData.ClimateFileNumber;
//    //IData.MaxCycle = IData.years;
//    //read climate file name
//
//    ClimateFilename.clear();
//    for (i=1; i<=IData.ClimateFileNumber; i++)
//    {
//        fscanf(fp,"%d",&j);
//        Line++;
//        XX(fp);
//        fgets(tmp,230,fp);
//        EraseFBSpace(tmp);
//        ClimateFilename.insert(200,tmp);
//    }
//
//    fscanf(fp,"%s", note);
//    if(strcmp(note, "Climate_file_mode")!=0)
//    {
//        IData.if1File = 0;
//        IData.CO2IncreaseRate = 0.0;
//        MissFlag=1;
//    }
//    else
//    {
//        fscanf(fp,"%d", IData->if1File);
//        fscanf(fp,"%s %f", note,&IData.CO2IncreaseRate);
//        MissFlag=0;
//    }
//
//    // Read Soil parameter
//    if(MissFlag==0) fscanf(fp,"%s",note);//"--------------------"
//    MissFlag=0;
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s",note);//"Soil data:"
//    Line++;
//    XX(fp);
//    fscanf(fp,"%s %d", note,&IData.Soil_Texture);// "  "
//    if(strcmp(note, "Soil_Texture")!=0) Toend("Soil_Texture:", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %d", note,&IData.Soil_landuse);// "  "
//    if(strcmp(note, "Landuse_Type")!=0) Toend("Landuse_Type:", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %f", note,&IData.Soil_Density);// "  Soil_"
//    if(strcmp(note, "Density")!=0) Toend("Density:", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %f", note,&IData.Soil_pH);	 // "  Soil_pH"
//    if(strcmp(note, "Soil_pH")!=0) Toend("Soil_pH:", fname, Line);
//    Line++;
//    XX(fp);
//
//    //fscanf(fp,"%s %f", note,&IData.Flood_pH);
//    fscanf(fp,"%s %f", note,&IData.Soil_SOCatSurface);// "  "
//    if(strcmp(note, "SOC_at_Surface")!=0) Toend("SOC_at_Surface:", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %f", note,&IData.Soil_Clay);	// "  Soil_Clay_Content"
//    if(strcmp(note, "Clay_fraction")!=0) Toend("Clay_fraction:", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %f", note,&IData.Soil_BypassFlow);	// "  "
//    if(strcmp(note, "BypassFlow")!=0) Toend("BypassFlow:", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %f", note,&IData.Soil_Litter);	// "  Litter"
//    if(strcmp(note, "Litter_SOC")!=0) Toend("Litter_SOC:", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %f", note,&IData.Soil_humads);	// "  "
//    if(strcmp(note, "Humads_SOC")!=0) Toend("Humads_SOC:", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %f", note,&IData.Soil_humus);	// "  humus"
//    if(strcmp(note, "Humus_SOC")!=0) Toend("Humus_SOC:", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %f", note,&IData.Soil_NO3);	// "  "
//    if(strcmp(note, "Soil_NO3(-)(mgN/kg)")!=0) Toend("Soil_NO3(-)(mgN/kg):", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %f", note,&IData.Soil_NH4);	// "  Soil_NH4"
//    if(strcmp(note, "Soil_NH4(+)(mgN/kg)")!=0) Toend("Soil_NH4(-)(mgN/kg):", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %f", note,&IData.Soil_moiture);// "  Soil_moisture"
//    if(strcmp(note, "Moisture")!=0) Toend("Moisture:", fname, Line);
//    Line++;
//    XX(fp);
//
//    fscanf(fp,"%s %f", note,&IData.LateralInflux);
//    if(strcmp(note, "Temperature")==0) IData.LateralInflux = 0.0;
//    if(IData.LateralInflux<0.0) IData.LateralInflux = 0.0;
//
////    fscanf(fp,"%s %f", note,&IData.Soil_T);		// "  Soil_ "
////    if(strcmp(note, "Temperature")!=0) Toend("Temperature:", fname, Line);
////    Line++;
////    XX(fp);
//
//    fscanf(fp,"%s", note);
//    if(strcmp(note, "Field_capacity")!=0)
//    {
//        IData.Fldcap = 0.0;
//        IData.Wiltpt = 0.0;
//        IData.Sks = 0.0;
//        IData.Soil_Porosity = 0.0;
//        IData.SOCa = nSoca[IData.Soil_landuse];
//        IData.SOCb = nSocb[IData.Soil_landuse];
//        IData.DClitter = 1.0;
//        IData.DChumads = 1.0;
//        IData.DChumus = 1.0;
//        IData.HumadCN = 10.0;
//        IData.HumusCN = 10.0;
//        IData.PassiveC = 0.0;
//        IData.Soil_MicrobioIndex = 1.0;
//        IData.HighestWT = 9.99;
//        IData.Soil_WaterObstructLayer = 9.99;
//        IData.slope = 0.0;
//        IData.UseION = 0;
//        IData.ProfileType = 0;
//
//        MissFlag=1;
//    }
//    else
//    {
//        fscanf(fp,"%f", IData->Fldcap);
//        fscanf(fp,"%s %f", note,&IData.Wiltpt);
//        fscanf(fp,"%s %f", note,&IData.Sks);
//        fscanf(fp,"%s %f", note,&IData.Soil_Porosity);
//        fscanf(fp,"%s %f", note,&IData.SOCa);
//        fscanf(fp,"%s %f", note,&IData.SOCb);
//        fscanf(fp,"%s %f", note,&IData.DClitter);
//        fscanf(fp,"%s %f", note,&IData.DChumads);
//        fscanf(fp,"%s %f", note,&IData.DChumus);
//        fscanf(fp,"%s %f", note,&IData.HumadCN);
//        fscanf(fp,"%s %f", note,&IData.HumusCN);
//        fscanf(fp,"%s %f", note,&IData.PassiveC);
//        fscanf(fp,"%s %f", note,&IData.Soil_MicrobioIndex);
//        fscanf(fp,"%s %f", note,&IData.HighestWT);
//        fscanf(fp,"%s %f", note,&IData.Soil_WaterObstructLayer);
//        fscanf(fp,"%s %f", note,&IData.slope);
//        fscanf(fp,"%s %d", note,&IData.ProfileType);
//
//        if(IData.Sks<0.015) IData.Sks = 0.015;
//        MissFlag=0;
//    }
//
//
//    // Read crop parameter
//    int ri, temp, yc, db_CycleYear[20], FCTT=0;
//    int db_Rotation_Number, FarmMode=0, FarmCropNumber;
//
//    char DB[200];
//    FILE *db;
//
//    char CropDB[200];
//    FILE *cropdb;
//
//    if(MissFlag==0) fscanf(fp,"%s",note);//"--------------------"
//    MissFlag=0;
//    Line++;
//    XX(fp);
//    fscanf(fp,"%s",note);//"Crop data:"
//    Line++;
//    XX(fp);
//    fscanf(fp,"%s %d", note,&db_Rotation_Number);// "  Rotation_Number"
//    if(strcmp(note, "Rotation_Number=")!=0) Toend("Rotation_Number=", fname);
//
//    Line++;
//    XX(fp);
//
//    sprintf(DB,"%s/Inter/CropRotation.txt", out_path);//###
//
//    db=fopen(DB, "w");
//    fprintf(db, "%d\n", db_Rotation_Number);
//    fclose(db);
//
//    //IData.Rotation_Number=db_Rotation_Number;
//    // Set CroPracticeArray
//    //rotation.dim(IData.Rotation_Number);
//    //cycle.dim(IData.Rotation_Number, IData.years);
//
//    for (i=1; i<=db_Rotation_Number; i++)
//    {
//        int db_TotalYear;
//        int tilln, TillMonth, TillDay, TillMethod;
//        int manun, ManuMonth, ManuDay, ManuType, ManuMethod;
//        float ManuAmount, ManuCN, ManuN;
//
//        fscanf(fp,"%s %d", note,&ri);// "---->"
//        if(strcmp(note, "Rotation_ID=")!=0) Toend("Rotation_ID=", fname);
//
//
//        //ri=i;
//
//        //normal crop
//        fscanf(fp,"%s %d", note,&db_TotalYear);
//        if(strcmp(note, "Totalyear=")!=0) Toend("Totalyear=", fname, Line);
//        Line++;
//        XX(fp);
//
//        fscanf(fp,"%s %d", note,&db_CycleYear[i]);
//        if(strcmp(note, "Years_Of_A_Cycle=")!=0) Toend("Years_of_a_cycle=", fname, Line);
//        Line++;
//        XX(fp);
//
//        sprintf(DB,"%s/Inter/CropRotation_%d.txt",out_path, i);
//        db=fopen(DB, "w");
//        fprintf(db, "%d\n", i);
//        fprintf(db, "%d\n", db_TotalYear);
//        fprintf(db, "%d\n", db_CycleYear[i]);
//        fclose(db);
//
//        //rrr = i;
//        for (j=1; j<=db_CycleYear[i]; j++)
//        {
//            char WT_file[200];
//
//            sprintf(CropDB,"%s/Inter/CropRotation_%d_%d.txt", out_path, i, j);
//            cropdb=fopen(CropDB, "w");
//            if(cropdb==NULL)
//            {
//                cout << "Can not create file " << CropDB << endl;
//                exit(0);
//            }
//
//            if(MissFlag==0) fscanf(fp,"%s",note);
//            fscanf(fp,"%d",&yc);
//            MissFlag=0;
//
//            //fscanf(fp,"%s %d", note,&yc);
//            if(strcmp(note, "YearID_of_a_cycle=")!=0) Toend("YearID_of_a_cycle=", fname, Line);
//            Line++;
//            XX(fp);
//
//            //yc = j;
//
//            fscanf(fp,"%s %d", note,&cropn);//CCPP.M_YrCrops[i][j]);
//            if(strcmp(note, "Crop_total_Number=")!=0) Toend("Crop_total_Number=", fname, Line);
//            Line++;
//            XX(fp);
//
//            fprintf(cropdb, "%d\n", cropn);
//            //cropn=CCPP.M_YrCrops[i][j];
//
//            //cycle.get(i,j)->CropNumber = cropn;
//            //yyy = j;
//            //SetArrayRange(1, cropn);
//            int db_Type, db_Pmonth, db_Pday, db_Hmonth , db_Hday, db_Hyear, db_CoverCrop, db_PerennialCrop;
//            float db_Yield, db_Residue;
//            float db_GrowthReproductive, db_GrowthVegetative, db_PsnEfficiency, db_PsnMax, db_InitialBiomass;
//            float db_GrainFraction, db_ShootFraction, db_RootFraction;
//            float db_GrainCN, db_ShootCN, db_RootCN;
//            float db_TDD, db_Water, db_LAI, db_Nfix, db_Vascularity;
//
//            for (k=1; k<=cropn; k++)
//            {
//                if(MissFlag==0) fscanf(fp,"%s",note);
//                fscanf(fp,"%d",&temp);
//                MissFlag=0;
//                if(strcmp(note, "Crop_ID=")!=0) Toend("Crop_ID=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %d",note,&db_Type);
//                if(strcmp(note, "Crop_Type=")!=0) Toend("Crop_Type=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %d %d",note, &db_Pmonth , &db_Pday);
//                if(strcmp(note, "Plant_time=")!=0) Toend("Plant_time=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %d %d",note,&db_Hmonth , &db_Hday);
//                if(strcmp(note, "Harvest_time=")!=0) Toend("Harvest_time=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %d",note,&db_Hyear);
//                if(strcmp(note, "Year_of_harvest=")!=0) Toend("Year_of_harvest=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %f",note,&db_Residue);
//                if(strcmp(note, "Ground_Residue=")!=0) Toend("Ground_Residue=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %f",note,&db_Yield);// "
//                if(strcmp(note, "Yield=")!=0) Toend("Yield=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %f",note,&db_GrowthReproductive);
//                if(strcmp(note, "Rate_reproductive=")!=0) Toend("Rate_reproductive=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %f",note,&db_GrowthVegetative);
//                if(strcmp(note, "Rate_vegetative=")!=0) Toend("Rate_vegetative=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %f",note,&db_PsnEfficiency);
//                if(strcmp(note, "Psn_efficiency=")!=0) Toend("Psn_efficiency=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %f",note,&db_PsnMax);
//                if(strcmp(note, "Psn_maximum=")!=0) Toend("Psn_maximum=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %f",note,&db_InitialBiomass);
//                if(strcmp(note, "Initial_biomass=")!=0) Toend("Initial_biomass=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %d",note,&db_CoverCrop);
//                if(strcmp(note, "Cover_crop=")!=0) Toend("Cover_crop=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s", note);
//                if(strcmp(note, "Perennial_crop=")!=0)
//                {
//                    db_PerennialCrop = 0;
//
//                    FILE *fpp;
//                    char  VEGE[80], note[50];
//                    int jd;
//                    float jf;
//                    sprintf(VEGE, "%s/Library/Lib_crop/crop_%d", root, db_Type);
//                    fpp = fopen(VEGE, "r");
//                    if ( fpp == NULL )
//                    {
//                        cout << "Can not open file " << VEGE << endl;
//                        exit(0);
//                    }
//
//                    fscanf(fpp, "%d %s", &jd, note);
//                    fscanf(fpp, "%s %s", note, note);
//                    fscanf(fpp, "%s %s", note, note);
//                    fscanf(fpp, "%f %s", &jf, note);
//                    fscanf(fpp, "%f %s", &db_GrainFraction, note);
//                    fscanf(fpp, "%f %s", &db_ShootFraction, note);
//                    fscanf(fpp, "%f %s", &db_RootFraction, note);
//                    fscanf(fpp, "%f %s", &jf, note);
//                    fscanf(fpp, "%f %s", &db_GrainCN, note);
//                    fscanf(fpp, "%f %s", &db_RootCN, note);
//                    fscanf(fpp, "%f %s", &db_ShootCN, note);
//                    fscanf(fpp, "%f %s", &db_Water, note);
//                    fscanf(fpp, "%f %s", &db_LAI, note);
//                    fscanf(fpp, "%f %s", &jf, note);
//                    fscanf(fpp, "%f %s", &db_TDD, note);
//                    fscanf(fpp, "%f %s", &db_Nfix, note);
//                    fscanf(fpp, "%f %s", &db_Vascularity, note);
//                    fclose(fpp);
//
//                    MissFlag=1;
//                }
//                else
//                {
//                    fscanf(fp,"%d",&db_PerennialCrop);
//                    fscanf(fp,"%s %f",note,&db_GrainFraction);
//                    fscanf(fp,"%s %f",note,&db_ShootFraction);
//                    fscanf(fp,"%s %f",note,&db_RootFraction);
//                    fscanf(fp,"%s %f",note,&db_GrainCN);
//                    fscanf(fp,"%s %f",note,&db_ShootCN);
//                    fscanf(fp,"%s %f",note,&db_RootCN);
//                    fscanf(fp,"%s %f",note,&db_TDD);
//                    fscanf(fp,"%s %f",note,&db_Water);
//                    fscanf(fp,"%s %f",note,&db_LAI);
//                    fscanf(fp,"%s %f",note,&db_Nfix);
//                    MissFlag=0;
//
//                    fscanf(fp,"%s",note);
//                    if(strcmp(note, "Vascularity=")!=0)
//                    {
//                        db_Vascularity = 0.0;
//                        MissFlag=1;
//                    }
//                    else
//                        fscanf(fp,"%f",&db_Vascularity);
//
//                }
//
//                fprintf(cropdb, "%d\n", k);
//                fprintf(cropdb, "%d\n", db_Type);
//                fprintf(cropdb, "%d  %d\n", db_Pmonth , db_Pday);
//                fprintf(cropdb, "%d  %d\n", db_Hmonth , db_Hday);
//                fprintf(cropdb, "%d\n", db_Hyear);
//                fprintf(cropdb, "%f\n", db_Residue);
//                fprintf(cropdb, "%f\n", db_Yield);
//                fprintf(cropdb, "%f %f %f %f %f\n", db_GrowthReproductive, db_GrowthVegetative,
//                        db_PsnEfficiency, db_PsnMax, db_InitialBiomass);
//                fprintf(cropdb, "%d\n", db_CoverCrop);
//                fprintf(cropdb, "%d\n", db_PerennialCrop);
//
//                fprintf(cropdb, "%f %f %f\n", db_GrainFraction, db_ShootFraction, db_RootFraction);
//                fprintf(cropdb, "%f %f %f\n", db_GrainCN, db_ShootCN, db_RootCN);
//                fprintf(cropdb, "%f %f %f %f %f\n", db_TDD, db_Water, db_LAI, db_Nfix, db_Vascularity);
//
//            }
//
//            fclose(cropdb);
//
//            //Tillage
//            sprintf(DB,"%s/Inter/CropTill_%d_%d.txt", out_path, i, j);
//            db=fopen(DB, "w");
//            if(db==NULL)
//            {
//                cout << "Can not create file " << DB << endl;
//                exit(0);
//            }
//
//            if(MissFlag==0) fscanf(fp,"%s", note);// "---"
//            fscanf(fp,"%d", &tilln);
//            MissFlag=0;
//            if(strcmp(note, "Tillage_number=")!=0) Toend("Tillage_number=", fname, Line);
//            Line++;
//            XX(fp);
//
//            fprintf(db, "%d\n", tilln);
//
//            //cycle.get(ri,yc)->TillNumber = tilln;
//            //SetArrayRange(2, tilln);
//            for (k=1; k<=tilln; k++)
//            {
//                fscanf(fp,"%s %d", note,&temp);// "th Tillage:"   //  Month,Day,method"
//                if(strcmp(note, "Tillage_ID=")!=0) Toend("Tillage_ID=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %d %d %d", note,&TillMonth, &TillDay, &TillMethod);
//                if(strcmp(note, "Month/Day/method=")!=0) Toend("Tillage_Month/Day/method=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fprintf(db, "%d  %d  %d\n", TillMonth, TillDay, TillMethod);
//            }
//            fclose(db);
//
//            //Fertilization
//            sprintf(DB,"%s/Inter/CropFert_%d_%d.txt", out_path, i, j);
//            db=fopen(DB, "w");
//            if(db==NULL)
//            {
//                cout << "Can not create file " << DB << endl;
//                exit(0);
//            }
//
//            int fertn, FertMonth, FertDay, FertMethod, FertOption;
//            float FertNitrate, FertAbi, FertUrea, FertAnh, FertNH4NO3, FertNH42SO4, FertNH42HPO4, FertDepth;
//            float DayRelease, NIefficiency, NIduration, UreaseEfficiency, UreaseDuration;
//
//
//            fscanf(fp,"%s %d", note,&fertn);// "Fertilization_number---"
//            if(strcmp(note, "Fertil_number=")!=0) Toend("Fertil_number=", fname, Line);
//            Line++;
//            XX(fp);
//
//            fprintf(db, "%d\n", fertn);
//
//            if(fertn!=-1)
//            {
//                for (k=1; k<=fertn; k++)
//                {
//                    if(MissFlag==0) fscanf(fp,"%s %d", note,&temp);
//                    else fscanf(fp,"%d", &temp);
//                    MissFlag=0;
//
//                    /*fscanf(fp,"%s %d", note,&temp);
//                    if(strcmp(note, "fertilization_ID=")!=0) Toend("fertilization_ID=", fname, Line);
//                    Line++; XX(fp);*/
//
//                    fscanf(fp,"%s %d %d %d", note,&FertMonth, &FertDay, &FertMethod);
//                    if(strcmp(note, "Month/Day/method=")!=0) Toend("Month/Day/method=", fname, Line);
//                    Line++;
//                    XX(fp);
//
//                    fscanf(fp,"%s %f", note,&FertDepth);
//                    if(strcmp(note, "Depth=")!=0) Toend("Depth=", fname, Line);
//                    Line++;
//                    XX(fp);
//
//                    fscanf(fp,"%s %f", note,&FertNitrate);
//                    if(strcmp(note, "Nitrate=")!=0) Toend("Nitrate=", fname, Line);
//                    Line++;
//                    XX(fp);
//
//                    fscanf(fp,"%s %f", note,&FertAbi);
//                    if(strcmp(note, "AmmBic=")!=0) Toend("AmmBic=", fname, Line);
//                    Line++;
//                    XX(fp);
//
//                    fscanf(fp,"%s %f", note,&FertUrea);
//                    if(strcmp(note, "Urea=")!=0) Toend("Urea=", fname, Line);
//                    Line++;
//                    XX(fp);
//
//                    fscanf(fp,"%s %f", note,&FertAnh);
//                    if(strcmp(note, "Anh=")!=0) Toend("Anh=", fname, Line);
//                    Line++;
//                    XX(fp);
//
//                    fscanf(fp,"%s %f", note,&FertNH4NO3);
//                    if(strcmp(note, "NH4NO3=")!=0) Toend("NH4NO3=", fname, Line);
//                    Line++;
//                    XX(fp);
//
//                    fscanf(fp,"%s %f", note,&FertNH42SO4);
//                    if(strcmp(note, "NH42SO4=")!=0) Toend("NH42SO4=", fname, Line);
//                    Line++;
//                    XX(fp);
//
//                    fscanf(fp,"%s %f", note,&FertNH42HPO4);
//                    if(strcmp(note, "NH4HPO4=")!=0) Toend("NH4HPO4=", fname, Line);
//                    Line++;
//                    XX(fp);
//
//                    fprintf(db, "%d %d %d\n", FertMonth, FertDay, FertMethod);
//                    fprintf(db, "%f %f %f %f %f %f %f %f\n", FertNitrate, FertAbi, FertUrea, FertAnh, FertNH4NO3,
//                            FertNH42SO4, FertNH42HPO4, FertDepth);
//
//                    fscanf(fp,"%s %f", note,&DayRelease);
//                    if(strcmp(note, "Release_rate=")!=0) Toend("Release_rate=", fname, Line);
//                    Line++;
//                    XX(fp);
//
//                    fscanf(fp,"%s %f", note,&NIefficiency);
//                    if(strcmp(note, "Inhibitor_efficiency=")!=0) Toend("Inhibitor_efficiency=", fname, Line);
//                    Line++;
//                    XX(fp);
//
//                    fscanf(fp,"%s %f", note,&NIduration);
//                    if(strcmp(note, "Inhibitor_duration=")!=0) Toend("Inhibitor_duration=", fname, Line);
//                    Line++;
//                    XX(fp);
//
//                    fscanf(fp,"%s", note);
//                    if(strcmp(note, "Urease_efficiency=")!=0)
//                    {
//                        UreaseEfficiency=0.0;
//                        UreaseDuration=0.0;
//                        MissFlag=1;
//                    }
//                    else
//                    {
//                        fscanf(fp,"%f", &UreaseEfficiency);
//                        if(strcmp(note, "Urease_efficiency=")!=0) Toend("Urease_efficiency=", fname);
//                        fscanf(fp,"%s %f", note, &UreaseDuration);
//                        if(strcmp(note, "Urease_duration=")!=0) Toend("Urease_duration=", fname);
//                    }
//
//
//
//                    fprintf(db, "%f %f %f %f %f\n", DayRelease, NIefficiency, NIduration,
//                            UreaseEfficiency, UreaseDuration);
//                }
//
//                if(MissFlag==0)
//                    fscanf(fp,"%s",note);
//                else
//                {
//                }
//                if(strcmp(note, "FertilizationOption=")!=0)
//                {
//                    FertOption = 0;
//                    MissFlag=1;
//                }
//                else
//                {
//                    fscanf(fp,"%d",&FertOption);
//                    MissFlag=0;
//                }
//
//                fprintf(db, "%d\n", FertOption);
//            }
//            else
//            {
//                char Fertigation[200];
//
//                fscanf(fp,"%s %s", note, Fertigation);
//                fprintf(db, "%s\n", Fertigation);
//            }
//
//            fclose(db);
//
//            //Manure
//            sprintf(DB,"%s/Inter/CropManu_%d_%d.txt", out_path, i, j);
//            db=fopen(DB, "w");
//            if(db==NULL)
//            {
//                cout << "Can not create file " << DB << endl;
//                exit(0);
//            }
//
//            if(MissFlag==0) fscanf(fp,"%s %d", note,&manun);
//            else fscanf(fp,"%d", &manun);
//            MissFlag=0;
//
//            if(strcmp(note, "Manure_number=")!=0) Toend("Manure_number=", fname, Line);
//            Line++;
//            XX(fp);
//
//            fprintf(db, "%d\n", manun);
//
//            //cycle.get(ri,yc)->ManNumber = manun;
//            //SetArrayRange(4, manun);
//            for (k=1; k<=manun; k++)
//            {
//                if(MissFlag==0) fscanf(fp,"%s %d", note,&temp);
//                else fscanf(fp,"%d", &temp);
//                MissFlag=0;
//
//                if(strcmp(note, "Manure_ID=")!=0) Toend("Manure_ID=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %d %d", note,&ManuMonth, &ManuDay);
//                if(strcmp(note, "Month/Day=")!=0) Toend("Month/Day=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %f %f", note,&ManuAmount, &ManuCN);
//                if(strcmp(note, "Amount/C|N_ratio=")!=0) Toend("Amount/C|N_ratio=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %d", note, &ManuType);
//                if(strcmp(note, "Type=")!=0) Toend("Type=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s", note);
//                if(strcmp(note, "Method=")!=0)
//                {
//                    ManuMethod = 0;
//                    MissFlag=1;
//                }
//                else
//                    fscanf(fp,"%d",&ManuMethod);
//
//                fprintf(db, "%d %d\n", ManuMonth, ManuDay);
//                fprintf(db, "%f %f %d %d\n", ManuAmount, ManuCN, ManuType, ManuMethod);
//
//
//            }
//            fclose(db);
//
//            //Plastic Filming
//            sprintf(DB,"%s/Inter/CropPlastic_%d_%d.txt", out_path, i, j);
//            db=fopen(DB, "w");
//            if(db==NULL)
//            {
//                cout << "Can not create file " << DB << endl;
//                exit(0);
//            }
//
//            int weedn, WeedOption, WeedMonth1, WeedDay1, WeedMonth2, WeedDay2;
//            if(MissFlag==0) fscanf(fp,"%s %d", note,&weedn);// "Weed_number---"
//            else fscanf(fp,"%d", &weedn);
//            MissFlag=0;
//
//            //if(strcmp(note, "Plastic_applications=")!=0) Toend("Plastic_applications=", fname, Line);
//            //Line++; XX(fp);
//
//            //cycle.get(ri,yc)->WeedNumber = weedn;
//            //SetArrayRange(5, weedn);
//            fscanf(fp,"%s %d", note,&WeedOption);// "Weed_Problem"
//            //if(strcmp(note, "Ventilation=")!=0) Toend("Ventilation=", fname, Line);
//            Line++;
//            XX(fp);
//
//            fprintf(db, "%d %d\n", weedn, WeedOption);
//
//            //WeedParameter weed1;
//            for (k=1; k<=weedn; k++)
//            {
//                fscanf(fp,"%s %d", note,&temp);// "th Weeding:"			//Month,Day,method"
//                //if(strcmp(note, "Plastic_ID=")!=0) Toend("Plastic_ID=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %d %d", note,&WeedMonth1, &WeedDay1);
//                fscanf(fp,"%s %d %d", note,&WeedMonth2, &WeedDay2);
//                //if(strcmp(note, "Start/End=")!=0) Toend("Start/End=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fprintf(db, "%d %d\n", WeedMonth1, WeedDay1);
//                fprintf(db, "%d %d\n", WeedMonth2, WeedDay2);
//                //weed.get(ri,yc,k)->month=weed1.month;
//                //weed.get(ri,yc,k)->day =weed1.day;
//                //fscanf(fp,"%d %d %s",&weed.get(ri,yc,k)->month, &weed.get(ri,yc,k)->day, note);
//            }
//            fclose(db);
//
//            //Flooding
//            sprintf(DB,"%s/Inter/CropFloo_%d_%d.txt", out_path, i, j);
//            db=fopen(DB, "w");
//            if(db==NULL)
//            {
//                cout << "Can not create file " << DB << endl;
//                exit(0);
//            }
//
//            fscanf(fp,"%s %d", note,&floon);// "Flood_number---"
//            if(strcmp(note, "Flood_number=")!=0) Toend("Flood_number=", fname, Line);
//            Line++;
//            XX(fp);
//
//            fscanf(fp,"%s %d", note,&FdLeak);// Leak type"
//            if(strcmp(note, "Leak_type=")!=0) Toend("Leak_type=", fname, Line);
//            Line++;
//            XX(fp);
//
//            fscanf(fp,"%s %d", note,&WaterControl);//
//            if(strcmp(note, "Water_control=")!=0) Toend("Water_control=", fname, Line);
//            Line++;
//            XX(fp);
//
//            fscanf(fp,"%s %f", note, &WaterLeakRate);
//            if(strcmp(note, "Leak_rate=")!=0) Toend("Leak_rate=", fname, Line);
//            Line++;
//            XX(fp);
//
//            fprintf(db, "%d %d %d %f\n", floon, FdLeak, WaterControl, WaterLeakRate);
//
//            for (k=1; k<=floon; k++)
//            {
//                fscanf(fp,"%s %d", note,&temp);// "th Flooding:"
//                if(strcmp(note, "Flooding_ID=")!=0) Toend("Flooding_ID=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %d %d", note,&FlooMonth1, &FlooDay1);
//                if(strcmp(note, "Flood_Month/Day=")!=0) Toend("Flood_Month/Day=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %d %d", note,&FlooMonth2, &FlooDay2);
//                if(strcmp(note, "Drain_Month/Day=")!=0) Toend("Drain_Month/Day=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %f", note,&FlooN);
//                //if(strcmp(note, "Water_N=")!=0) Toend("Water_N=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %d", note,&ShallowFlood);
//                if(strcmp(note, "Shallow_flood=")!=0) Toend("Shallow_flood=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fprintf(db, "%d %d\n", FlooMonth1, FlooDay1);
//                fprintf(db, "%d %d\n", FlooMonth2, FlooDay2);
//                fprintf(db, "%f\n", FlooN);
//                fprintf(db, "%d\n", ShallowFlood);
//            }
//
//            fscanf(fp,"%s", note);
//            if(strcmp(note, "Water_gather=")!=0)
//            {
//                WaterGetherIndex = 1.0;
//                MissFlag=1;
//            }
//            else
//                fscanf(fp,"%f", &WaterGetherIndex);
//
//            fprintf(db, "%f\n", WaterGetherIndex);
//
//            if(MissFlag==1)
//            {
//                if(strcmp(note, "WT_file=")!=0)
//                {
//                    sprintf(WT_file, "None");
//                }
//                else
//                {
//                    fscanf(fp,"%s", WT_file);
//                    MissFlag=0;
//                }
//            }
//            else
//            {
//                fscanf(fp,"%s", note);
//                if(strcmp(note, "WT_file=")!=0)
//                {
//                    sprintf(WT_file, "None");
//                    MissFlag=1;
//                }
//                else
//                {
//                    fscanf(fp,"%s", WT_file);
//                }
//            }
//
//            fprintf(db, "%s\n", WT_file);
//
//
//            if(MissFlag==1)
//            {
//                if(strcmp(note, "Empirical_parameters=")!=0)
//                {
//                    m_IniWT=0.0;
//                    m_LWTceasingSurfFlow=0.0;
//                    m_LWTceasingGroungFlow=0.0;
//                    m_WatershedIndex=0.0;
//                    m_SurfOutflowIntensity=0.0;
//                    m_GroundOutflowIntensity=0.0;
//
//                    MissFlag=1;
//                }
//                else
//                {
//                    fscanf(fp,"%f %f %f %f %f %f", &m_IniWT, &m_LWTceasingSurfFlow, &m_LWTceasingGroungFlow, &m_WatershedIndex,
//                           &m_SurfOutflowIntensity, &m_GroundOutflowIntensity);
//                    MissFlag=0;
//                }
//            }
//            else
//            {
//                fscanf(fp,"%s", note);
//                if(strcmp(note, "Empirical_parameters=")!=0)
//                {
//                    m_IniWT=0.0;
//                    m_LWTceasingSurfFlow=0.0;
//                    m_LWTceasingGroungFlow=0.0;
//                    m_WatershedIndex=0.0;
//                    m_SurfOutflowIntensity=0.0;
//                    m_GroundOutflowIntensity=0.0;
//
//                    MissFlag=1;
//                }
//                else
//                {
//                    fscanf(fp,"%f %f %f %f %f %f", &m_IniWT, &m_LWTceasingSurfFlow, &m_LWTceasingGroungFlow, &m_WatershedIndex,
//                           &m_SurfOutflowIntensity, &m_GroundOutflowIntensity);
//                    MissFlag=0;
//                }
//            }
//
//            fprintf(db, "%f %f %f %f %f %f\n", m_IniWT, m_LWTceasingSurfFlow, m_LWTceasingGroungFlow, m_WatershedIndex,
//                    m_SurfOutflowIntensity, m_GroundOutflowIntensity);
//
//            fclose(db);
//
//            //Irrigation
//            sprintf(DB,"%s/Inter/CropIrri_%d_%d.txt", out_path, i, j);
//            db=fopen(DB, "w");
//            if(db==NULL)
//            {
//                cout << "Can not create file " << DB << endl;
//                exit(0);
//            }
//
//            int irrin, IrriMode, IrriMonth, IrriDay, IrriMethod;
//            float IrriIndex, IrriAmount;
//
//            if(MissFlag==0) fscanf(fp,"%s", note);
//            fscanf(fp,"%d", &irrin);// "Irrigation_number---"
//            MissFlag=0;
//
//            if(strcmp(note, "Irrigation_number=")!=0) Toend("Irrigation_number=", fname, Line);
//            Line++;
//            XX(fp);
//
//            fscanf(fp,"%s %d", note,&IrriMode);// Irrigation type"
//            if(strcmp(note, "Irrigation_type=")!=0) Toend("Irrigation_type=", fname, Line);
//            Line++;
//            XX(fp);
//
//            fscanf(fp,"%s %f", note,&IrriIndex);// Irrigation type"
//            if(strcmp(note, "Irrigation_Index=")!=0) Toend("Irrigation_Index=", fname, Line);
//            Line++;
//            XX(fp);
//
//            fprintf(db, "%d %d %f\n", irrin, IrriMode, IrriIndex);
//
//            //cycle.get(ri,yc)->IrrNumber = irrin;
//            //SetArrayRange(7, irrin);
//            for (k=1; k<=irrin; k++)
//            {
//                fscanf(fp,"%s %d", note,&temp);
//                if(strcmp(note, "Irrigation_ID=")!=0) Toend("Irrigation_ID=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %d %d", note,&IrriMonth, &IrriDay);
//                if(strcmp(note, "Irr_Month/Day=")!=0) Toend("Irr_Month/Day=", fname, Line);
//                Line++;
//                XX(fp);
//
//                int jd;
//                fscanf(fp,"%s %f %d", note,&IrriAmount, &jd);
//                if(jd!=0&&jd!=1&&jd!=2) IrriMethod=0;
//                else IrriMethod=jd;
//                //if(strcmp(note, "Water_amount/Method=")!=0) Toend("Water_amount/Method=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fprintf(db, "%d %d\n", IrriMonth, IrriDay);
//                fprintf(db, "%f %d\n", IrriAmount, IrriMethod);
//            }
//            fclose(db);
//
//            //Grazing
//            sprintf(DB,"%s/Inter/CropGraz_%d_%d.txt", out_path, i, j);
//            db=fopen(DB, "w");
//            if(db==NULL)
//            {
//                cout << "Can not create file " << DB << endl;
//                exit(0);
//            }
//
//            int grazn, GrazMonth1, GrazDay1, GrazMonth2, GrazDay2;
//            float GrazHour, Dairy, Beef, Pig, Horse, Sheep;
//
//            fscanf(fp,"%s %d", note,&grazn);// "Grazing_number---"
//            if(strcmp(note, "Grazing_number=")!=0) Toend("Grazing_number= ", fname, Line);
//            Line++;
//            XX(fp);
//
//            fprintf(db, "%d\n", grazn);
//
//            //cycle.get(ri,yc)->GrazingNumber = grazn;
//            //SetArrayRange(8, grazn);
//            for (k=1; k<=grazn; k++)
//            {
//                fscanf(fp,"%s %d", note,&temp);
//                if(strcmp(note, "Grazing_ID=")!=0) Toend("Grazing_ID=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %d %d", note,&GrazMonth1, &GrazDay1);
//                if(strcmp(note, "Start_Month/Day=")!=0) Toend("Start_Month/Day=", fname, Line);
//                Line++;
//                XX(fp);
//
//                fscanf(fp,"%s %d %d", note,&GrazMonth2, &GrazDay2);
//                if(strcmp(note, "End_Month/Day=")!=0) Toend("End_Month/Day=", fname, Line);
//                Line++;
//                XX(fp);
//                fscanf(fp,"%s", note);
//                if(strcmp(note, "Dairy_Head_per_ha=")!=0)//old version
//                {
//                    //MissFlag=1;
//                    fscanf(fp,"%f", &Beef);
//                    fscanf(fp,"%s %f", note,&Horse);
//                    fscanf(fp,"%s %f", note,&Sheep);
//                    Pig = 0.0;
//                    Dairy = 0.0;
//                }
//                else//new version
//                {
//                    fscanf(fp,"%f", &Dairy);
//                    fscanf(fp,"%s %f", note,&Beef);
//                    fscanf(fp,"%s %f", note,&Pig);
//                    fscanf(fp,"%s %f", note,&Sheep);
//                    fscanf(fp,"%s %f", note,&Horse);
//                }
//
//                fscanf(fp,"%s %f", note,&GrazHour);
//                if(strcmp(note, "Graz_Hours_Per_day=")!=0) Toend("Graz_Hours_Per_day=", fname);
//
//                fprintf(db, "%d %d\n", GrazMonth1, GrazDay1);
//                fprintf(db, "%d %d\n", GrazMonth2, GrazDay2);
//                fprintf(db, "%f\n", GrazHour);
//                fprintf(db, "%f %f %f %f %f\n", Dairy, Beef, Pig, Sheep, Horse);
//
//            }
//            fclose(db);
//
//            //Grass cutting
//            sprintf(DB,"%s/Inter/GrassCut_%d_%d.txt", out_path, i, j);
//            db=fopen(DB, "w");
//            if(db==NULL)
//            {
//                cout << "Can not create file " << DB << endl;
//                exit(0);
//            }
//
//            int cutn, CutMonth, CutDay, CutPart;
//            float CutAmount;
//
//            fscanf(fp,"%s %d", note,&cutn);// "Cutting_number---"
//            Line++;
//            XX(fp);
//
//            if(strcmp(note, "Cut_number=")==0)
//            {
//                fprintf(db, "%d\n", cutn);
//
//                for (k=1; k<=cutn; k++)
//                {
//                    if(MissFlag==0) fscanf(fp,"%s %d", note,&temp);
//                    else
//                    {
//                        fscanf(fp,"%d", &temp);
//                        MissFlag = 0;
//                    }
//                    if(strcmp(note, "Cut_ID=")!=0) Toend("Cut_ID=", fname, Line);
//                    Line++;
//                    XX(fp);
//
//                    fscanf(fp,"%s %d %d", note,&CutMonth, &CutDay);
//                    if(strcmp(note, "Cut_Month/Day=")!=0) Toend("Cut_Month/Day=", fname, Line);
//                    Line++;
//                    XX(fp);
//
//                    fscanf(fp,"%s %f", note,&CutAmount);
//                    if(strcmp(note, "Cut_amount=")!=0) Toend("Cut_amount=", fname, Line);
//                    Line++;
//                    XX(fp);
//
//                    fscanf(fp,"%s", note);
//                    if(strcmp(note, "Cut_part=")!=0)
//                    {
//                        MissFlag=1;
//                        CutPart=0;
//                    }
//                    else
//                    {
//                        fscanf(fp,"%d", &CutPart);
//                        Line++;
//                        XX(fp);
//                    }
//
//                    fprintf(db, "%d %d\n", CutMonth, CutDay);
//                    fprintf(db, "%f %d\n", CutAmount, CutPart);
//                }
//            }
//            fclose(db);
//        }
//    }
//    fscanf(fp,"%s", note);
//    if(strcmp(note, "----------------------------------------")==0) FarmMode = 1;
//    if(strcmp(note, "Crop_model_approach")==0)
//    {
//        FarmMode = 0;
//        MissFlag = 1;
//    }
//
//
//
//    if(MissFlag==0) fscanf(fp,"%s %d", note,&IData.CropModel);
//    else
//    {
//        fscanf(fp,"%d", IData->CropModel);
//        MissFlag=0;
//    }
//    if(strcmp(note, "Crop_model_approach")!=0)
//    {
//        //Toend("Crop_model_approach", fname, Line);
//        //Line++; XX(fp);
//        IData.CropModel = 0;
//    }
//
//#ifdef MANURE
//
//    int HouseInputFormat, AnimalTypes, AnimalType[10];
//    char HouseInputFileName[200];
//    float AnimalHeads[10],FeedRate[10],FeedProtein[10];
//    float m_FloorArea,m_BedCN, m_VentRate, m_BedRate, m_BedFrequency,PenBedFrequency;
//    int m_FloorSurface, m_BedType, m_Ventilation;
//    int PenInputFormat, PenAnimalTypes, PenAnimalType[15],PenSurface,PenBedMaterial;
//    char PenInputFileName[200];
//    float PenAnimalHeads[15],PenFeedRate[15],PenFeedProtein[15],PenArea,PenBedCN,PenBedAmount;
//    int GrazeInputFormat, GrazeAnimalType, GrazeTimes, IniMonth[366],IniDay[366],EndMonth[366],EndDay[366];
//    char GrazeInputFileName[200];
//    float GrazeAnimalHeads[366],GrazeAnimalHrs[366], GrazeArea;
//    float m_LagoonCapacity, m_LagoonSurfaceArea, m_LagoonDays, LagoonToField, LagoonToMarket, LagoonRemain;
//    int m_LagoonCover, m_LagoonRain;
//    float m_CompostDensity, m_CompostDays, m_LitterAmount, m_LitterCN, CompostToField, CompostToMarket, CompostRemain;
//    float m_DigesterCapacity,m_DigesterCH4,m_DigesterDays, DigesterToField;
//    int m_Temp;
//    float DigesterToMarket, DigesterRemain;
//    float HouseManureOutDays,m_UC, m_UL, m_UD, m_UR, m_FC, m_FL, m_FD, m_FR;
//    float PenManureOutDays, PenUC, PenUL,PenUD,PenUR,PenFC, PenFL,PenFD,PenFR;
//    float GrazeManureOutDays, GrazeUF, GrazeUR, GrazeFF, GrazeFR, FieldArea;
//
//    string MP;
//    FILE *mp;
//    string SF;
//    FILE* sf;
//
//    if(fscanf(fp,"%s %d",note,&HouseInputFormat)!=2)
//    {
//        cout << "No livostock data" << endl;
//    }
//    else
//    {
//        if(HouseInputFormat==1)
//            fscanf(fp,"%s %s",note,&HouseInputFileName);
//        else
//        {
//            fscanf(fp,"%s %d",note,&AnimalTypes);
//            fscanf(fp,"%s %d %f %f %f",note,&AnimalType[1],&AnimalHeads[1],&FeedRate[1],&FeedProtein[1]);
//        }
//        fscanf(fp,"%s %f %d %d %f %f %f",note,&m_FloorArea,&m_FloorSurface,&m_BedType,&m_BedCN,&m_BedFrequency,&m_BedRate);
//        fscanf(fp,"%s %d %f",note,&m_Ventilation,&m_VentRate);
//        fscanf(fp,"%s %f", note, &HouseManureOutDays);
//        fscanf(fp,"%s %f %f %f %f", note, &m_UC, &m_UL, &m_UD, &m_UR);
//        fscanf(fp,"%s %f %f %f %f", note, &m_FC, &m_FL, &m_FD, &m_FR);
//
//        //////////////////
//        if(AnimalHeads[1]>0)
//        {
//            SF.Format("%s/Inter/ManureHouse.txt", OUTPUT);
//            sf=fopen(SF, "w");
//            fprintf(sf, "%d\n", HouseInputFormat);
//            if(HouseInputFormat==1)								//use a input file name
//                fprintf(sf, "%s\n", HouseInputFileName);
//            else										//use average input data
//            {
//                //Type: 1 dairy, 2 beef, 3 veal, 4 swine, 5 sheep, 6 goat, 7 horse, 8 layer, 9 brolier, 10 turkey, 11 duck
//                fprintf(sf, "%d\n", AnimalTypes);
//                for(int j=1; j<=AnimalTypes; j++)
//                    fprintf(sf, "%3d  %8.2f  %8.2f  %8.4f\n", AnimalType[j], AnimalHeads[j], FeedRate[j], FeedProtein[j]);
//            }
//
//            fprintf(sf, "%f  %d\n", m_FloorArea, m_FloorSurface);//0 slatted floor; 1 conrete floor
//            fprintf(sf, "%d  %f  %f  %f\n", m_BedType, m_BedCN, m_BedFrequency, m_BedRate);//0 None, 1 straw, 2 sawdust, 3 dry manure solids, 4 sand
//            fprintf(sf, "%d  %f\n", m_Ventilation, m_VentRate);// 0 natural, 1 fan
//            fprintf(sf, "%f\n", HouseManureOutDays);
//            fprintf(sf, "%f %f %f %f\n", m_UC, m_UL, m_UD, m_UR);
//            fprintf(sf, "%f %f %f %f\n", m_FC, m_FL, m_FD, m_FR);
//            fclose(sf);
//        }
//        ////////////////////////////////
//
//        fscanf(fp,"%s %d",note,&PenInputFormat);
//        if(PenInputFormat==1)
//            fscanf(fp,"%s %s",note,&PenInputFileName);
//        else
//        {
//            fscanf(fp,"%s %d",note,&PenAnimalTypes);
//            fscanf(fp,"%s %d %f %f %f",note,&PenAnimalType[1],&PenAnimalHeads[1],&PenFeedRate[1],&PenFeedProtein[1]);
//        }
//        fscanf(fp,"%s %f %d %d %f %f %f",note,&PenArea,&PenSurface,&PenBedMaterial,&PenBedCN,&PenBedFrequency,&PenBedAmount);
//
//        fscanf(fp,"%s %f", note, &PenManureOutDays);
//        fscanf(fp,"%s %f %f %f %f", note, &PenUC, &PenUL,&PenUD,&PenUR);
//        fscanf(fp,"%s %f %f %f %f", note, &PenFC, &PenFL,&PenFD,&PenFR);
//
//        ///////////////////////
//        if(PenAnimalHeads[1]>0)
//        {
//            SF.Format("%s/Inter/ManurePen.txt", OUTPUT);
//            sf=fopen(SF, "w");
//            fprintf(sf, "%d\n", PenInputFormat);
//            if(PenInputFormat==1)								//use a input file name
//                fprintf(sf, "%s\n", PenInputFileName);
//            else										//use average input data
//            {
//                fprintf(sf, "%d\n", PenAnimalTypes);
//                for(int j=1; j<=PenAnimalTypes; j++)
//                    fprintf(sf, "%3d  %8.2f  %8.2f  %8.4f\n", PenAnimalType[j], PenAnimalHeads[j],PenFeedRate[j],PenFeedProtein[j]);
//            }
//
//            fprintf(sf, "%f  %d\n", PenArea, PenSurface);//0 mineral soil; 1 conrete
//            fprintf(sf, "%d  %f  %f  %f\n", PenBedMaterial, PenBedCN, PenBedFrequency, PenBedAmount);//0 None, 1 straw, 2 sawdust, 3 dry manure solids, 4 sand
//            fprintf(sf, "%f\n", PenManureOutDays);
//            fprintf(sf, "%f %f %f %f\n", PenUC, PenUL,PenUD,PenUR);
//            fprintf(sf, "%f %f %f %f\n", PenFC, PenFL,PenFD,PenFR);
//            fclose(sf);
//        }
//        /////////////////////////////
//
//        fscanf(fp,"%s %d",note,&GrazeInputFormat);
//        if(GrazeInputFormat==1)
//            fscanf(fp,"%s %s",note,&GrazeInputFileName);
//        else
//        {
//            fscanf(fp,"%s %d",note,&GrazeAnimalType);
//            fscanf(fp,"%s %d",note,&GrazeTimes);
//            for(i=1; i<=GrazeTimes; i++)
//            {
//                fscanf(fp,"%s %d %d %d %d %f %f",note,&IniMonth[i],&IniDay[i],&EndMonth[i],&EndDay[i],
//                       &GrazeAnimalHeads[i],&GrazeAnimalHrs[i]);
//            }
//        }
//        fscanf(fp,"%s %f",note,&GrazeArea);
//
//        ///////////////////////
//        if(GrazeTimes>0)
//        {
//            SF.Format("%s/Inter/ManureGraze.txt", OUTPUT);
//            sf=fopen(SF, "w");
//            fprintf(sf, "%d\n", GrazeInputFormat);
//            if(GrazeInputFormat==1)								//use a input file name
//                fprintf(sf, "%s\n", GrazeInputFileName);
//            else										//use average input data
//            {
//                fprintf(sf, "%d  %d\n", GrazeAnimalType, GrazeTimes);//Type: 1 dairy, 2 beef, 3 veal, 4 swine, 5 sheep, 6 goat, 7 horse,
//                for(int i=1; i<=GrazeTimes; i++)
//                {
//                    fprintf(sf, "%d  %d  %d  %d  %f  %f\n", IniMonth[i], IniDay[i], EndMonth[i], EndDay[i], GrazeAnimalHeads[i], GrazeAnimalHrs[i]);
//                }
//            }
//            fprintf(sf, "%f\n", GrazeArea);
//            fclose(sf);
//        }
//        ///////////////////////////////
//
//        fscanf(fp,"%s %f %f %d %d %f",note, &m_LagoonCapacity, &m_LagoonSurfaceArea,&m_LagoonCover, &m_LagoonRain, &m_LagoonDays);
//        fscanf(fp,"%s %f %f %f",note, &LagoonToField,  &LagoonToMarket, &LagoonRemain);
//
//        if(m_LagoonCapacity>0.0)
//        {
//            SF.Format("\%s/Inter/ManureLagoon.txt", OUTPUT);
//            sf=fopen(SF, "w");
//            fprintf(sf, "%f  %f  %d  %d  %f\n", m_LagoonCapacity, m_LagoonSurfaceArea, m_LagoonCover, m_LagoonRain, m_LagoonDays);
//            fprintf(sf, "%f  %f  %f\n", LagoonToField, LagoonToMarket, LagoonRemain);
//            fclose(sf);
//        }
//
//        ///////////////////////////////
//        fscanf(fp,"%s %f %f %f %f",note,&m_CompostDensity,&m_CompostDays,&m_LitterAmount,&m_LitterCN);
//        fscanf(fp,"%s %f %f %f",note, &CompostToField,  &CompostToMarket, &CompostRemain);
//
//        if(m_CompostDensity>0.0)
//        {
//            sprintf(SF,"%s/Inter/ManureCompost.txt", OUTPUT);
//            sf=fopen(SF, "w");
//            fprintf(sf, "%f  %f  %f  %f\n", m_CompostDensity, m_CompostDays, m_LitterAmount, m_LitterCN);
//            fprintf(sf, "%f  %f  %f\n", CompostToField, CompostToMarket, CompostRemain);
//            fclose(sf);
//        }
//        ///////
//        fscanf(fp,"%s %d %f %f",note, &m_Temp,&m_DigesterCH4, &m_DigesterDays);
//        fscanf(fp,"%s %f %f %f",note, &DigesterToField,  &DigesterToMarket, &DigesterRemain);
//
//        if(m_DigesterDays>0.0)
//        {
//            sprintf(SF,"%s/Inter/ManureDigester.txt", OUTPUT);
//            sf=fopen(SF, "w");
//            fprintf(sf, "%d  %f  %f\n", m_Temp,m_DigesterCH4,m_DigesterDays);
//            fprintf(sf, "%f  %f  %f\n", DigesterToField,  DigesterToMarket, DigesterRemain);
//            fclose(sf);
//        }
//
//        fscanf(fp,"%s %f",note, &FieldArea);
//
//        if(FieldArea>0.0)
//        {
//            sprintf(MP,"%s/Inter/ManureField.txt", OUTPUT);
//            mp=fopen(MP, "w");
//            fprintf(mp, "%f\n", FieldArea);
//            fclose(mp);
//        }
//
//    }
//#endif
//
//    fclose(fp);
//
//
//    char CM[200];
//    FILE *cm;
//    sprintf(CM,"%s/Inter/CropModel", out_path);
//    cm=fopen(CM, "w");
//    if(cm==NULL)
//    {
//        cout << "Can not create file " << CM << endl;
//        exit(0);
//    }
//    else
//    {
//        fprintf(cm, "%d\n", IData.CropModel);
//        fclose(cm);
//    }
//
//////////////////////////////////////////////// GetAMT //////////////////////////////////////
//
//
///////////////////////////////////////////////  END    //////////////////////////////////////
//
//    /// Initial the simulation data
//
//
//////////////////////////////////////////////// SaveSite /////////////////////////////////////
//
//    FILE *fpo;
//    char fout[255];
//    cout << "debug" << endl;
//    cout << fname << endl;
//
//    sprintf(fout,"%s/Inputs/site",out_path);
//    fpo=sopen(fout,0);
//    if (fpo==NULL) return;
//
//    if (strlen(IData.sitename)==0)
//    {
//        cout << "No Site name information!" << "Input Error:" << endl;
//        return ;
//    }
//    fprintf(fpo,"%s\n",IData.sitename);
//    if (IData.years<=0)
//    {
//        cout << "No Simulated year information!" << "Input Error:" << endl;
//        fclose(fpo);
//        return ;
//    }
//    /*if (IData.latitude<=0) {
//    	MessageBox(NULL,"No latitude information!","Input Error:",0);
//    	fclose(fpo);
//    	return -1;
//    }*/
//
//    fprintf(fpo,"%d %d %f %ld %f\n", IData.years, IData.ifdaily,
//            IData.latitude, IData.totalday, IData.longitude);//, IData.if1File);
//    fprintf(fpo,"%d\n", IData.Soil_landuse);//IData.CropModel);
//    fclose (fpo);
//
//    sprintf(fout, "%s/Inputs/SPAN",out_path);
//    fpo=sopen(fout,0);
//    if (fpo==NULL) return ;
//    fprintf(fpo, "%d\n", 1);//Span
//    fclose(fpo);
////////////////////////////////////////// END   /////////////////////////////////////////////////////
//
//
//////////////////////////////////////////  RecordClimateFile  ///////////////////////////////////////
//
//    int yn, result=-1, TotalYear, FirstYear;
//    float longitute, latitude;
//
//#ifdef GLOBE_CLIM
//    IData.ClimateFileType= 4;
//#endif
//
//    if(DaymetFlag==1)
//    {
//        char DMF[255];
//        FILE *dmf;
//        sprintf(DMF,"%s/Inter/SiteLocation.txt", out_path);
//        dmf=fopen(DMF, "r");
//        if(dmf==NULL)
//        {
//            cout << "Can not open file %s" <<  DMF << endl;
//        }
//
//        fscanf(dmf, "%f %f %d %d", &longitute, &latitude, &TotalYear, &FirstYear);
//        fclose(dmf);
//
//        IData.ClimateFileType = 3;
//    }
//    else
//    {
//    }
//
//    i = ClimateFilename.n;
//    yn = IData.years;
//
//    if (i<IData.years && IData.if1File==0)
//    {
//        cout << "No enough Climate data files! Please re-input." << "Input Error:" << endl;
//        return;
//    }
//
//
//    char foutname[255];
//    char ch[255];//cn[250],
////	int j, k, jday,jday0,tn;
////	float maxT, minT, Rainfall, temp, solrad, Day_Hrs;
//    float MonAveT[15], YrAveT,YrRain;
//    //FILE *fpo;//*fpi,
//
//    sprintf(foutname,"%s/Inputs/CLIM.INP", out_path);
//    fpo=sopen(foutname,"w");
//
//
//    fprintf(fpo, "---------------------------------------------------------------\n");
//    fprintf(fpo, "Climate: \n");
//    fprintf(fpo, "    Year  File name   Temp (C)  Prec (cm)  N_in_rain(ppm)\n");
//
//    for (int i = 1; i<=IData.years; i++)
//    {
//        if(DaymetFlag==1)
//        {
//            sprintf(ch,"%s/Inter/daymet%d", out_path, FirstYear+i-1);
//        }
//        else
//        {
//            if(IData.if1File==0)
//                sprintf(ch,"%s",ClimateFilename.get(i));
//            else
//                sprintf(ch,"%s",ClimateFilename.get(1));
//        }
//
//
//        SaveClimateFileOfAYear(i,ch,MonAveT,&YrAveT,&YrRain,IData.latitude,
//                               IData.ClimateFileType,IData.NO3NH4_Rainfall,
//                               IData.NH3_Atmosphere,IData.BaseCO2, out_path ,mode, IData.WaterYrMonth,
//                               IData.WaterYrDay, IData.CO2IncreaseRate);
//
//        fprintf(fpo, "    %3d   %s          %.2f      %.2f       %.2f\n",
//                1, ch, YrAveT, YrRain, IData.NO3NH4_Rainfall );
//
//        //if (mode==0) return 0;
//
//    }
//
//    fclose(fpo);
//
/////////////////////////////////////////////////// END ///////////////////////////////////////////
//
/////////////////////////////////////////////////  ReadSoilFeature  //////////////////////////////
//
//    FILE *fpi;
//    char f_in_name[255];
//
//    float DClay, DFldcap, DWiltpt, DSks, DPorosity;
//
//    //IData.Soil_Texture = Soiltexture.ListIndex + 1
//    if (IData.Soil_Texture<1 && IData.Soil_Texture>13) return ;
//
//    cout << "test1" << endl;
//    cout << IData.Soil_Texture << endl;
//    sprintf(f_in_name, "%s/Library/Lib_soil/soil_%d", root_path, IData.Soil_Texture);
//    cout << "test2" << endl;
//
//    //sprintf(note,"%d",IData.Soil_Texture);
//    //strcat(fname,note);
//    if ((fpi=sopen(f_in_name, "r")) == NULL)
//    {
//        cout << "You may have the DNDC model installed in an incorrect place. Please move the entire DNDC folder to the root directory of any drive in your computer!" << endl;
//        cout << " Can not find Library Folder" << endl;
//        exit(0);
//    }
//
//    fscanf(fpi,"%s %s", IData.SoilName,note);//IData.NameNote);
//    fscanf(fpi,"%f %s", &DClay,note);
//    fscanf(fpi,"%f %s", &DPorosity, note);//IData.StsNote);
//    fscanf(fpi,"%f %s", &DSks, note);//IData.SksNote);
//    fscanf(fpi,"%f %s", &DFldcap, note);//IData.FldcapNote);
//    fscanf(fpi,"%f %s", &DWiltpt, note);//IData.WiltptNote);
//    //fscanf(fpi,"%f %s", IData->Soilhtcp, note);//IData.SoilhtcpNote);
//    //fscanf(fpi,"%f %s", &IData.Psisat, note);//IData.PsisatNote);
//    //fscanf(fpi,"%f %s", &IData.Beta, note);//IData.BetaNote);
//    fclose (fpi);
//
//    DSks *= 0.6;//cm/min ->m/hr
//
//    if(IData.Soil_Clay==0.0) IData.Soil_Clay = DClay;
//    if(IData.Fldcap==0.0) IData.Fldcap = DFldcap;
//    if(IData.Wiltpt==0.0) IData.Wiltpt = DWiltpt;
//    if(IData.Sks==0.0) IData.Sks = DSks;
//    if(IData.Soil_Porosity==0.0) IData.Soil_Porosity = DPorosity;
//
//    cout << IData.SoilName << endl;
//
//
////////////////////////////////////////////////// END ///////////////////////////////////////////
//
////////////////////////////////////////////////// ReadClimateHead  //////////////////////////////
//
//    cout << "ReadClimateHead" << endl;
//    ReadClimateHead(1,out_path,&IData);
////////////////////////////////////////////////// END ///////////////////////////////////////////
//
//
////////////////////////////////////////////////// SetSoilH /////////////////////////////////////
//
//    IData.AcWater   = 0;
//    IData.SurfWater = 0;
//    IData.SnowPack  = 0;
//
//    //IData.Idc = 1# + IIf(soilbypass(0).Value = True, 0, 1) 'Soil_BypassFlow
//    IData.Idc = 1 + (int)IData.Soil_BypassFlow;
//
//    //if (IData.Soil_Texture != 12) { //'If Soiltexture.ListIndex <> 11 Then
//    //     IData.Sts = float(1 - IData.Soil_Density * (IData.Soil_SOCatSurface / 1.3 + (1 - IData.Soil_SOCatSurface) / 2.6));
//    //
//    //if (IData.Sts<=0) //safety check
//    /*{
//    	MessageBox(NULL,"Soil-Density or Soil Organic may be wrong! Please input a smaller one.","Inputed Error:",NULL);
//    	IData.Sts = 1;
//    	return -1;
//    }*/
//
//    //if (IData.Soil_Texture < 10) IData.Sks = IData.RawSKS * (float)0.6; //cm/min -> m/hr
//    IData.Qstar = (float)(6.45 * (IData.Soil_Clay - 0.01) + 1);
//    IData.BypassF = IData.Soil_BypassFlow;//(float)0.1 * (IData.Idc - 1);
//
//    //float T01 = 1;
//    //if (IData.BypassF > 0.0) T01 = IData.BypassF * 10.0 + 1.0;
//    //IData.Sks = IData.Sks * T01;
//
//    float dt;
//    dt = (2 * 0.005 / IData.Soil_Porosity);
//    if (dt < IData.Sks)
//    {
//        IData.H = dt;
//    }
//    else
//    {
//        IData.H = IData.Sks;
//    }
//    if (IData.H <= 0)
//    {
//        cout << "There is a parameter error of Soil_H=0 ! Please check system file and your data. " "Input Error :" << endl;
//        return ;
//    }
//
//    int total_layer=MAXSOILLAYER-10;
//    IData.TH = SOILDEPTH;//0.9;//otal depth of simulated soil profile, m
//    if (IData.H*total_layer < IData.TH )
//        IData.H = IData.TH / total_layer;//total layer number is <=50
//    IData.Q = (int)(IData.TH / IData.H);
//
//    if (IData.Q > total_layer)
//    {
//        IData.Q = total_layer;
//        IData.H = IData.TH / total_layer;
//    }
////////////////////////////////////////////////// END ///////////////////////////////////////////
//
//////////////////////////////////////////////////  SaveInterFarmParas ///////////////////////////
//
//    SaveInterFarmParas(out_path);
/////////////////////////////////////////////////  END //////////////////////////////////////////
//
////////////////////////////////////////////////// SaveSoilParas /////////////////////////////////
//
//    SaveSoilParas(1,IData,out_path);
////////////////////////////////////////////////// END ///////////////////////////////////////////
//
////////////////////////////////////////////////// SaveCropParas  ////////////////////////////////
//
//    SaveCropParas(out_path,root);
////////////////////////////////////////////////// END ///////////////////////////////////////////
//
//
//
//    cout << "DND Loaded successuly" << endl;
//
//    //Site_Simulator("c:\\dndc");
//    //start("c:\\dndc");
//
//}
//




void ReadInputDatafromDND(char *fname, char *out_path, char *root,Site_BaseInformation *IData)
{
    /// Load DND files,which are site configure..

    cout << "Load DND file" << endl;
    cout << "New Drought Warning System Version 9.4" << endl;

    //char root_path[20] = "C:\\DNDC";
    //char root_path[20] = root;
    char root_path[3072];
    strcpy(root_path,root);





    int  i, j, k, cropn, MissFlag=0;
    int floon, FdLeak, ShallowFlood, WaterControl, FlooMonth1, FlooDay1, FlooMonth2, FlooDay2;
    float FlooN, WaterGetherIndex, WaterLeakRate;
    float m_IniWT, m_LWTceasingSurfFlow, m_LWTceasingGroungFlow, m_WatershedIndex,m_SurfOutflowIntensity, m_GroundOutflowIntensity;
    char note[255],tmp[255];
    FILE *fp;
    StringArray ClimateFilename;
    int DaymetFlag = 0;
    int mode = 0;


    // Initial IData class
//    Site_BaseInformation IData;

//    float nSoca[6]={0.0,0.2,0.2,0.08,0.05,0.2};
//    float nSocb[6]={10.0,2.0,2.0,1.4,4.0,2.0};


    char txt[200];

    if ((fp=sopen(fname,1))==NULL)
    {
        cout << "Can not open " <<  fname << endl;
        return;
    }

    int Line=1;
    //char vvv[200];

    // Read site ReadInputDatafromDND() function
    fscanf(fp,"%s",note);
    if(strcmp(note, "Input_Parameters:")!=0) Toend("Input_Parameters:", fname, Line);
    Line++;
    XX(fp);

    //Site data:
    fscanf(fp,"%s",note);//"--------------------"
    Line++;
    XX(fp);

    fscanf(fp,"%s %s",note,&IData->sitename);
    if(strcmp(note, "Site_data:")!=0) Toend("Site_data:", fname, Line);
    Line++;
    XX(fp);

    fscanf(fp,"%s %d",note,&IData->years);
    if(strcmp(note, "Simulated_Year:")!=0) Toend("Simulated_Year:", fname, Line);
    Line++;
    XX(fp);

    fscanf(fp,"%s %f", note,&IData->latitude);// "  "
    if(strcmp(note, "Latitude:")!=0) Toend("Latitude:", fname, Line);
    Line++;
    XX(fp);

    fscanf(fp,"%s %d", note,&IData->ifdaily);	//"  "
    if(strcmp(note, "Daily_Record:")!=0) Toend("Daily_Record:", fname, Line);
    Line++;
    XX(fp);

    //climate data
    fscanf(fp,"%s",note);//"--------------------"
    Line++;
    XX(fp);
    fscanf(fp,"%s",note);//"Climate data:"
    Line++;
    XX(fp);
    fscanf(fp,"%s %d", note,&IData->ClimateFileType);
    if(strcmp(note, "Climate_Data_Type:")!=0) Toend("Climate_Data_Type:", fname, Line);
    Line++;
    XX(fp);

    fscanf(fp,"%s %f", note,&IData->NO3NH4_Rainfall);	//"
    if(strcmp(note, "NO3NH4_in_Rainfall")!=0) Toend("NO3NH4_in_Rainfall", fname, Line);
    Line++;
    XX(fp);

    fscanf(fp,"%s %f", note,&IData->NH3_Atmosphere);	//"
    if(strcmp(note, "NO3_of_Atmosphere")!=0) Toend("NO3_of_Atmosphere:", fname, Line);
    Line++;
    XX(fp);

    fscanf(fp,"%s %f", note,&IData->BaseCO2);		//"
    if(strcmp(note, "BaseCO2_of_Atmosphere")!=0) Toend("BaseCO2_of_Atmosphere:", fname, Line);
    Line++;
    XX(fp);

    fscanf(fp,"%s %d", note,&IData->ClimateFileNumber);
    if(strcmp(note, "Climate_file_count=")!=0) Toend("Climate_file_count:", fname, Line);
    Line++;
    XX(fp);

    k=IData->ClimateFileType;
    int nn=IData->ClimateFileNumber;
    //&IData->MaxCycle = &IData->years;
    //read climate file name
    ClimateFilename.clear();
    for (i=1; i<=IData->ClimateFileNumber; i++)
    {
        fscanf(fp,"%d",&j);
        Line++;
        XX(fp);
        fgets(tmp,230,fp);
        EraseFBSpace(tmp);
        ClimateFilename.insert(200,tmp);
    }

    fscanf(fp,"%s", note);
    if(strcmp(note, "Climate_file_mode")!=0)
    {
        IData->if1File = 0;
        IData->CO2IncreaseRate = 0.0;
        MissFlag=1;
    }
    else
    {
        fscanf(fp,"%d", &IData->if1File);
        fscanf(fp,"%s %f", note,&IData->CO2IncreaseRate);
        MissFlag=0;
    }

    // Read Soil parameter
    if(MissFlag==0) fscanf(fp,"%s",note);//"--------------------"
    MissFlag=0;
    Line++;
    XX(fp);

    fscanf(fp,"%s",note);//"Soil data:"
//    Line++;
//    XX(fp);
    fscanf(fp,"%s %d", note,&IData->Soil_Texture);// "  "
    if(strcmp(note, "Soil_Texture")!=0) Toend("Soil_Texture:", fname,note);
//    Line++;
//    XX(fp);

    fscanf(fp,"%s %d", note,&IData->Soil_landuse);// "  "
    if(strcmp(note, "Landuse_Type")!=0) Toend("Landuse_Type:", fname,note);
    Line++;
    XX(fp);

    fscanf(fp,"%s %f", note,&IData->Soil_Density);// "  Soil_"
    if(strcmp(note, "Density")!=0) Toend("Density:", fname, Line);
    Line++;
    XX(fp);

    fscanf(fp,"%s %f", note,&IData->Soil_pH);	 // "  Soil_pH"
    if(strcmp(note, "Soil_pH")!=0) Toend("Soil_pH:", fname, Line);
    Line++;
    XX(fp);

    //fscanf(fp,"%s %f", note,&IData->Flood_pH);
    fscanf(fp,"%s %f", note,&IData->Soil_SOCatSurface);// "  "
    if(strcmp(note, "SOC_at_Surface")!=0) Toend("SOC_at_Surface:", fname, Line);
    Line++;
    XX(fp);

    fscanf(fp,"%s %f", note,&IData->Soil_Clay);	// "  Soil_Clay_Content"
    if(strcmp(note, "Clay_fraction")!=0) Toend("Clay_fraction:", fname, Line);
    Line++;
    XX(fp);

    fscanf(fp,"%s %f", note,&IData->Soil_BypassFlow);	// "  "
    if(strcmp(note, "BypassFlow")!=0) Toend("BypassFlow:", fname, Line);
    Line++;
    XX(fp);

    fscanf(fp,"%s %f", note,&IData->Soil_Litter);	// "  Litter"
    if(strcmp(note, "Litter_SOC")!=0) Toend("Litter_SOC:", fname, Line);
    Line++;
    XX(fp);

    fscanf(fp,"%s %f", note,&IData->Soil_humads);	// "  "
    if(strcmp(note, "Humads_SOC")!=0) Toend("Humads_SOC:", fname, Line);
    Line++;
    XX(fp);

    fscanf(fp,"%s %f", note,&IData->Soil_humus);	// "  humus"
    if(strcmp(note, "Humus_SOC")!=0) Toend("Humus_SOC:", fname, Line);
    Line++;
    XX(fp);

    fscanf(fp,"%s %f", note,&IData->Soil_NO3);	// "  "
    if(strcmp(note, "Soil_NO3(-)(mgN/kg)")!=0) Toend("Soil_NO3(-)(mgN/kg):", fname, Line);
    Line++;
    XX(fp);

    fscanf(fp,"%s %f", note,&IData->Soil_NH4);	// "  Soil_NH4"
    if(strcmp(note, "Soil_NH4(+)(mgN/kg)")!=0) Toend("Soil_NH4(-)(mgN/kg):", fname, Line);
    Line++;
    XX(fp);

    fscanf(fp,"%s %f", note,&IData->Soil_moiture);// "  Soil_moisture"
    if(strcmp(note, "Moisture")!=0) Toend("Moisture:", fname, Line);
    Line++;
    XX(fp);

    fscanf(fp,"%s %f", note,&IData->LateralInflux);
    if(strcmp(note, "Temperature")==0) IData->LateralInflux = 0.0;
    if(IData->LateralInflux<0.0) IData->LateralInflux = 0.0;

//    fscanf(fp,"%s %f", note,&IData->Soil_T);		// "  Soil_ "
//    if(strcmp(note, "Temperature")!=0) Toend("Temperature:", fname, Line);
//    Line++;
//    XX(fp);

    fscanf(fp,"%s", note);
    if(strcmp(note, "Field_capacity")!=0)
    {
        IData->Fldcap = 0.0;
        IData->Wiltpt = 0.0;
        IData->Sks = 0.0;
        IData->Soil_Porosity = 0.0;
        IData->SOCa = nSoca[IData->Soil_landuse];
        IData->SOCb = nSocb[IData->Soil_landuse];
        IData->DClitter = 1.0;
        IData->DChumads = 1.0;
        IData->DChumus = 1.0;
        IData->HumadCN = 10.0;
        IData->HumusCN = 10.0;
        IData->PassiveC = 0.0;
        IData->Soil_MicrobioIndex = 1.0;
        IData->HighestWT = 9.99;
        IData->Soil_WaterObstructLayer = 9.99;
        IData->slope = 0.0;
        IData->UseION = 0;
        IData->ProfileType = 0;
        IData->Soil_Quality = 0.0;

        MissFlag=1;
    }
    else
    {
        fscanf(fp,"%f", &IData->Fldcap);
        fscanf(fp,"%s %f", note,&IData->Wiltpt);
        fscanf(fp,"%s %f", note,&IData->Sks);
        fscanf(fp,"%s %f", note,&IData->Soil_Porosity);
        fscanf(fp,"%s %f", note,&IData->SOCa);
        fscanf(fp,"%s %f", note,&IData->SOCb);
        fscanf(fp,"%s %f", note,&IData->DClitter);
        fscanf(fp,"%s %f", note,&IData->DChumads);
        fscanf(fp,"%s %f", note,&IData->DChumus);
        fscanf(fp,"%s %f", note,&IData->HumadCN);
        fscanf(fp,"%s %f", note,&IData->HumusCN);
        fscanf(fp,"%s %f", note,&IData->PassiveC);
        fscanf(fp,"%s %f", note,&IData->Soil_MicrobioIndex);
        fscanf(fp,"%s %f", note,&IData->HighestWT);
        fscanf(fp,"%s %f", note,&IData->Soil_WaterObstructLayer);
        fscanf(fp,"%s %f", note,&IData->slope);
        fscanf(fp,"%s", note);


        if (IData->Soil_landuse == 2)
        {
            IData->Soil_WaterObstructLayer = 0.30; //meter
        }
        else
        {
            IData->Soil_WaterObstructLayer = 9.99; //meter
        }

        if(strcmp(note, "Salinity")!=0)
        {
            IData->Soil_Quality = 0.0;
            fscanf(fp,"%d", &IData->ProfileType);
        }
        else
        {
            fscanf(fp,"%f", &IData->Soil_Quality);
            fscanf(fp,"%s %d", note,&IData->ProfileType);
        }


        if(IData->Sks<0.015) IData->Sks = 0.015;
        MissFlag=0;
    }


    // Read crop parameter
    int ri, temp, yc, db_CycleYear[20], FCTT=0;
    int db_Rotation_Number, FarmMode=0, FarmCropNumber;

    char DB[200];
    FILE *db;

    char CropDB[200];
    FILE *cropdb;

    if(MissFlag==0) fscanf(fp,"%s",note);//"--------------------"
    MissFlag=0;
//    Line++;
//    XX(fp);


NewFarmCrop:
    fscanf(fp,"%s",note);//"Crop data:"
//    Line++;
//    XX(fp);
    fscanf(fp,"%s %d", note,&db_Rotation_Number);// "  Rotation_Number"
    if(strcmp(note, "Rotation_Number=")!=0) Toend("Rotation_Number=", fname,note);

//    Line++;
//    XX(fp);

    sprintf(DB,"%s/Inter/CropRotation.txt", out_path);//###

    db=fopen(DB, "w");
    fprintf(db, "%d\n", db_Rotation_Number);
    fclose(db);

    //&IData->Rotation_Number=db_Rotation_Number;
    // Set CroPracticeArray
    //rotation.dim(&IData->Rotation_Number);
    //cycle.dim(&IData->Rotation_Number, &IData->years);

    for (i=1; i<=db_Rotation_Number; i++)
    {
        int db_TotalYear;
        int tilln, TillMonth, TillDay, TillMethod;
        int manun, ManuMonth, ManuDay, ManuType, ManuMethod;
        float ManuAmount, ManuCN, ManuN;

        fscanf(fp,"%s %d", note,&ri);// "---->"
        if(strcmp(note, "Rotation_ID=")!=0) Toend("Rotation_ID=", fname,note);


        //ri=i;

        //normal crop
        fscanf(fp,"%s %d", note,&db_TotalYear);
        if(strcmp(note, "Totalyear=")!=0) Toend("Totalyear=", fname, Line);
        Line++;
        XX(fp);

        fscanf(fp,"%s %d", note,&db_CycleYear[i]);
        if(strcmp(note, "Years_Of_A_Cycle=")!=0) Toend("Years_of_a_cycle=", fname,note);




        //DB.Format("%s\\CropRotation_%d.txt", FCT60, i);

        sprintf(DB,"%s/Inter/CropRotation_%d.txt",out_path, i);
        db=fopen(DB, "w");
        fprintf(db, "%d\n", i);
        fprintf(db, "%d\n", db_TotalYear);
        fprintf(db, "%d\n", db_CycleYear[i]);
        fclose(db);

        //rrr = i;
        for (j=1; j<=db_CycleYear[i]; j++)
        {
            char WT_file[200];

            sprintf(CropDB,"%s/Inter/CropRotation_%d_%d.txt", out_path, i, j);
            cropdb=fopen(CropDB, "w");
            if(cropdb==NULL)
            {
                cout << "Can not create file " << CropDB << endl;
                exit(0);
            }

            if(MissFlag==0) fscanf(fp,"%s",note);
            fscanf(fp,"%d",&yc);
            MissFlag=0;

            //fscanf(fp,"%s %d", note,&yc);
            if(strcmp(note, "YearID_of_a_cycle=")!=0) Toend("YearID_of_a_cycle=", fname, Line);


            //yc = j;

            fscanf(fp,"%s %d", note,&cropn);//CCPP.M_YrCrops[i][j]);
            if(strcmp(note, "Crop_total_Number=")!=0) Toend("Crop_total_Number=", fname, Line);


            fprintf(cropdb, "%d\n", cropn);
            //cropn=CCPP.M_YrCrops[i][j];

            //cycle.get(i,j)->CropNumber = cropn;
            //yyy = j;
            //SetArrayRange(1, cropn);
            int db_Type, db_Pmonth, db_Pday, db_Hmonth , db_Hday, db_Hyear, db_CoverCrop, db_PerennialCrop;
            float db_Yield, db_Residue;
            float db_GrowthReproductive, db_GrowthVegetative, db_PsnEfficiency, db_PsnMax, db_InitialBiomass;
            float db_GrainFraction, db_ShootFraction, db_RootFraction;
            float db_GrainCN, db_ShootCN, db_RootCN;
            float db_TDD, db_Water, db_LAI, db_Nfix, db_Vascularity;

            for (k=1; k<=cropn; k++)
            {
                if(MissFlag==0) fscanf(fp,"%s",note);
                fscanf(fp,"%d",&temp);
                MissFlag=0;
                if(strcmp(note, "Crop_ID=")!=0) Toend("Crop_ID=", fname,note);
//                Line++;
//                XX(fp);

                fscanf(fp,"%s %d",note,&db_Type);
                if(strcmp(note, "Crop_Type=")!=0) Toend("Crop_Type=", fname,note);
//                Line++;
//                XX(fp);

                fscanf(fp,"%s %d %d",note, &db_Pmonth , &db_Pday);
                if(strcmp(note, "Plant_time=")!=0) Toend("Plant_time=", fname,note);
//                Line++;
//                XX(fp);

                fscanf(fp,"%s %d %d",note,&db_Hmonth , &db_Hday);
                if(strcmp(note, "Harvest_time=")!=0) Toend("Harvest_time=", fname,note);
//                Line++;
//                XX(fp);

                fscanf(fp,"%s %d",note,&db_Hyear);
                if(strcmp(note, "Year_of_harvest=")!=0) Toend("Year_of_harvest=", fname,note);
//                Line++;
//                XX(fp);

                fscanf(fp,"%s %f",note,&db_Residue);
                if(strcmp(note, "Ground_Residue=")!=0) Toend("Ground_Residue=", fname,note);
//                Line++;
//                XX(fp);

                fscanf(fp,"%s %f",note,&db_Yield);// "
                if(strcmp(note, "Yield=")!=0) Toend("Yield=", fname,note);
//                Line++;
//                XX(fp);

                fscanf(fp,"%s %f",note,&db_GrowthReproductive);
                if(strcmp(note, "Rate_reproductive=")!=0) Toend("Rate_reproductive=", fname,note);
//                Line++;
//                XX(fp);

                fscanf(fp,"%s %f",note,&db_GrowthVegetative);
                if(strcmp(note, "Rate_vegetative=")!=0) Toend("Rate_vegetative=", fname,note);
//                Line++;
//                XX(fp);

                fscanf(fp,"%s %f",note,&db_PsnEfficiency);
                if(strcmp(note, "Psn_efficiency=")!=0) Toend("Psn_efficiency=", fname,note);
//                Line++;
//                XX(fp);

                fscanf(fp,"%s %f",note,&db_PsnMax);
                if(strcmp(note, "Psn_maximum=")!=0) Toend("Psn_maximum=", fname,note);
//                Line++;
//                XX(fp);

                fscanf(fp,"%s %f",note,&db_InitialBiomass);
                if(strcmp(note, "Initial_biomass=")!=0) Toend("Initial_biomass=", fname,note);
//                Line++;
//                XX(fp);

                fscanf(fp,"%s %d",note,&db_CoverCrop);
                if(strcmp(note, "Cover_crop=")!=0) Toend("Cover_crop=", fname,note);
//                Line++;
//                XX(fp);

                fscanf(fp,"%s", note);
                if(strcmp(note, "Perennial_crop=")!=0)
                {
                    db_PerennialCrop = 0;

                    FILE *fpp;
                    char  VEGE[80], note[50];
                    int jd;
                    float jf;
                    sprintf(VEGE, "%s/Library/Lib_crop/crop_%d", root, db_Type);
                    fpp = fopen(VEGE, "r");
                    if ( fpp == NULL )
                    {
                        cout << "Can not open file " << VEGE << endl;
                        exit(0);
                    }

                    fscanf(fpp, "%d %s", &jd, note);
                    fscanf(fpp, "%s %s", note, note);
                    fscanf(fpp, "%s %s", note, note);
                    fscanf(fpp, "%f %s", &jf, note);
                    fscanf(fpp, "%f %s", &db_GrainFraction, note);
                    fscanf(fpp, "%f %s", &db_ShootFraction, note);
                    fscanf(fpp, "%f %s", &db_RootFraction, note);
                    fscanf(fpp, "%f %s", &jf, note);
                    fscanf(fpp, "%f %s", &db_GrainCN, note);
                    fscanf(fpp, "%f %s", &db_RootCN, note);
                    fscanf(fpp, "%f %s", &db_ShootCN, note);
                    fscanf(fpp, "%f %s", &db_Water, note);
                    fscanf(fpp, "%f %s", &db_LAI, note);
                    fscanf(fpp, "%f %s", &jf, note);
                    fscanf(fpp, "%f %s", &db_TDD, note);
                    fscanf(fpp, "%f %s", &db_Nfix, note);
                    fscanf(fpp, "%f %s", &db_Vascularity, note);
                    fclose(fpp);

                    MissFlag=1;
                }
                else
                {
                    fscanf(fp,"%d",&db_PerennialCrop);
                    fscanf(fp,"%s %f",note,&db_GrainFraction);
                    fscanf(fp,"%s %f",note,&db_ShootFraction);
                    fscanf(fp,"%s %f",note,&db_RootFraction);
                    fscanf(fp,"%s %f",note,&db_GrainCN);
                    fscanf(fp,"%s %f",note,&db_ShootCN);
                    fscanf(fp,"%s %f",note,&db_RootCN);
                    fscanf(fp,"%s %f",note,&db_TDD);
                    fscanf(fp,"%s %f",note,&db_Water);
                    fscanf(fp,"%s %f",note,&db_LAI);
                    fscanf(fp,"%s %f",note,&db_Nfix);
                    MissFlag=0;

                    fscanf(fp,"%s",note);
                    if(strcmp(note, "Vascularity=")!=0)
                    {
                        db_Vascularity = 0.0;
                        MissFlag=1;
                    }
                    else
                        fscanf(fp,"%f",&db_Vascularity);

                }

                fprintf(cropdb, "%d\n", k);
                fprintf(cropdb, "%d\n", db_Type);
                fprintf(cropdb, "%d  %d\n", db_Pmonth , db_Pday);
                fprintf(cropdb, "%d  %d\n", db_Hmonth , db_Hday);
                fprintf(cropdb, "%d\n", db_Hyear);
                fprintf(cropdb, "%f\n", db_Residue);
                fprintf(cropdb, "%f\n", db_Yield);
                fprintf(cropdb, "%f %f %f %f %f\n", db_GrowthReproductive, db_GrowthVegetative,
                        db_PsnEfficiency, db_PsnMax, db_InitialBiomass);
                fprintf(cropdb, "%d\n", db_CoverCrop);
                fprintf(cropdb, "%d\n", db_PerennialCrop);

                fprintf(cropdb, "%f %f %f\n", db_GrainFraction, db_ShootFraction, db_RootFraction);
                fprintf(cropdb, "%f %f %f\n", db_GrainCN, db_ShootCN, db_RootCN);
                fprintf(cropdb, "%f %f %f %f %f\n", db_TDD, db_Water, db_LAI, db_Nfix, db_Vascularity);

            }

            fclose(cropdb);

            //Tillage
            sprintf(DB,"%s/Inter/CropTill_%d_%d.txt", out_path, i, j);
            db=fopen(DB, "w");
            if(db==NULL)
            {
                cout << "Can not create file " << DB << endl;
                exit(0);
            }

            if(MissFlag==0) fscanf(fp,"%s", note);// "---"
            fscanf(fp,"%d", &tilln);
            MissFlag=0;
            if(strcmp(note, "Tillage_number=")!=0) Toend("Tillage_number=", fname, Line);
            Line++;
            XX(fp);

            fprintf(db, "%d\n", tilln);

            //cycle.get(ri,yc)->TillNumber = tilln;
            //SetArrayRange(2, tilln);
            for (k=1; k<=tilln; k++)
            {
                fscanf(fp,"%s %d", note,&temp);// "th Tillage:"   //  Month,Day,method"
                if(strcmp(note, "Tillage_ID=")!=0) Toend("Tillage_ID=", fname, Line);
                Line++;
                XX(fp);

                fscanf(fp,"%s %d %d %d", note,&TillMonth, &TillDay, &TillMethod);
                if(strcmp(note, "Month/Day/method=")!=0) Toend("Tillage_Month/Day/method=", fname, Line);
                Line++;
                XX(fp);

                fprintf(db, "%d  %d  %d\n", TillMonth, TillDay, TillMethod);
            }
            fclose(db);

            //Fertilization
            sprintf(DB,"%s/Inter/CropFert_%d_%d.txt", out_path, i, j);
            db=fopen(DB, "w");
            if(db==NULL)
            {
                cout << "Can not create file " << DB << endl;
                exit(0);
            }

            int fertn, FertMonth, FertDay, FertMethod, FertOption;
            float FertNitrate, FertAbi, FertUrea, FertAnh, FertNH4NO3, FertNH42SO4, FertNH42HPO4, FertDepth;
            float DayRelease, NIefficiency, NIduration, UreaseEfficiency, UreaseDuration;


            fscanf(fp,"%s %d", note,&fertn);// "Fertilization_number---"
            if(strcmp(note, "Fertil_number=")!=0) Toend("Fertil_number=", fname, Line);
            Line++;
            XX(fp);

            fprintf(db, "%d\n", fertn);

            if(fertn!=-1)
            {
                for (k=1; k<=fertn; k++)
                {
                    if(MissFlag==0) fscanf(fp,"%s %d", note,&temp);
                    else fscanf(fp,"%d", &temp);
                    MissFlag=0;

                    /*fscanf(fp,"%s %d", note,&temp);
                    if(strcmp(note, "fertilization_ID=")!=0) Toend("fertilization_ID=", fname, Line);
                    Line++; XX(fp);*/

                    fscanf(fp,"%s %d %d %d", note,&FertMonth, &FertDay, &FertMethod);
                    if(strcmp(note, "Month/Day/method=")!=0) Toend("Month/Day/method=", fname, Line);
                    Line++;
                    XX(fp);

                    fscanf(fp,"%s %f", note,&FertDepth);
                    if(strcmp(note, "Depth=")!=0) Toend("Depth=", fname, Line);
                    Line++;
                    XX(fp);

                    fscanf(fp,"%s %f", note,&FertNitrate);
                    if(strcmp(note, "Nitrate=")!=0) Toend("Nitrate=", fname, Line);
                    Line++;
                    XX(fp);

                    fscanf(fp,"%s %f", note,&FertAbi);
                    if(strcmp(note, "AmmBic=")!=0) Toend("AmmBic=", fname, Line);
                    Line++;
                    XX(fp);

                    fscanf(fp,"%s %f", note,&FertUrea);
                    if(strcmp(note, "Urea=")!=0) Toend("Urea=", fname, Line);
                    Line++;
                    XX(fp);

                    fscanf(fp,"%s %f", note,&FertAnh);
                    if(strcmp(note, "Anh=")!=0) Toend("Anh=", fname, Line);
                    Line++;
                    XX(fp);

                    fscanf(fp,"%s %f", note,&FertNH4NO3);
                    if(strcmp(note, "NH4NO3=")!=0) Toend("NH4NO3=", fname, Line);
                    Line++;
                    XX(fp);

                    fscanf(fp,"%s %f", note,&FertNH42SO4);
                    if(strcmp(note, "NH42SO4=")!=0) Toend("NH42SO4=", fname, Line);
                    Line++;
                    XX(fp);

                    fscanf(fp,"%s %f", note,&FertNH42HPO4);
                    if(strcmp(note, "NH4HPO4=")!=0) Toend("NH4HPO4=", fname, Line);
                    Line++;
                    XX(fp);

                    fprintf(db, "%d %d %d\n", FertMonth, FertDay, FertMethod);
                    fprintf(db, "%f %f %f %f %f %f %f %f\n", FertNitrate, FertAbi, FertUrea, FertAnh, FertNH4NO3,
                            FertNH42SO4, FertNH42HPO4, FertDepth);

                    fscanf(fp,"%s %f", note,&DayRelease);
                    if(strcmp(note, "Release_rate=")!=0) Toend("Release_rate=", fname, Line);
                    Line++;
                    XX(fp);

                    fscanf(fp,"%s %f", note,&NIefficiency);
                    if(strcmp(note, "Inhibitor_efficiency=")!=0) Toend("Inhibitor_efficiency=", fname, Line);
                    Line++;
                    XX(fp);

                    fscanf(fp,"%s %f", note,&NIduration);
                    if(strcmp(note, "Inhibitor_duration=")!=0) Toend("Inhibitor_duration=", fname, Line);
                    Line++;
                    XX(fp);

                    fscanf(fp,"%s", note);
                    if(strcmp(note, "Urease_efficiency=")!=0)
                    {
                        UreaseEfficiency=0.0;
                        UreaseDuration=0.0;
                        MissFlag=1;
                    }
                    else
                    {
                        fscanf(fp,"%f", &UreaseEfficiency);
                        if(strcmp(note, "Urease_efficiency=")!=0) Toend("Urease_efficiency=", fname);
                        fscanf(fp,"%s %f", note, &UreaseDuration);
                        if(strcmp(note, "Urease_duration=")!=0) Toend("Urease_duration=", fname);
                    }



                    fprintf(db, "%f %f %f %f %f\n", DayRelease, NIefficiency, NIduration,
                            UreaseEfficiency, UreaseDuration);
                }

                if(MissFlag==0)
                    fscanf(fp,"%s",note);
                else
                {
                }
                if(strcmp(note, "FertilizationOption=")!=0)
                {
                    FertOption = 0;
                    MissFlag=1;
                }
                else
                {
                    fscanf(fp,"%d",&FertOption);
                    MissFlag=0;
                }

                fprintf(db, "%d\n", FertOption);
            }
            else
            {
                char Fertigation[200];

                fscanf(fp,"%s %s", note, Fertigation);
                fprintf(db, "%s\n", Fertigation);
            }

            fclose(db);

            //Manure
            sprintf(DB,"%s/Inter/CropManu_%d_%d.txt", out_path, i, j);
            db=fopen(DB, "w");
            if(db==NULL)
            {
                cout << "Can not create file " << DB << endl;
                exit(0);
            }

            if(MissFlag==0) fscanf(fp,"%s %d", note,&manun);
            else fscanf(fp,"%d", &manun);
            MissFlag=0;

            if(strcmp(note, "Manure_number=")!=0) Toend("Manure_number=", fname, Line);
            Line++;
            XX(fp);

            fprintf(db, "%d\n", manun);

            //cycle.get(ri,yc)->ManNumber = manun;
            //SetArrayRange(4, manun);
            for (k=1; k<=manun; k++)
            {
                if(MissFlag==0) fscanf(fp,"%s %d", note,&temp);
                else fscanf(fp,"%d", &temp);
                MissFlag=0;

                if(strcmp(note, "Manure_ID=")!=0) Toend("Manure_ID=", fname, Line);
                Line++;
                XX(fp);

                fscanf(fp,"%s %d %d", note,&ManuMonth, &ManuDay);
                if(strcmp(note, "Month/Day=")!=0) Toend("Month/Day=", fname, Line);
                Line++;
                XX(fp);

                fscanf(fp,"%s %f %f", note,&ManuAmount, &ManuCN);
                if(strcmp(note, "Amount/C|N_ratio=")!=0) Toend("Amount/C|N_ratio=", fname, Line);
                Line++;
                XX(fp);

                fscanf(fp,"%s %d", note, &ManuType);
                if(strcmp(note, "Type=")!=0) Toend("Type=", fname, Line);
                Line++;
                XX(fp);

                fscanf(fp,"%s", note);
                if(strcmp(note, "Method=")!=0)
                {
                    ManuMethod = 0;
                    MissFlag=1;
                }
                else
                    fscanf(fp,"%d",&ManuMethod);

                fprintf(db, "%d %d\n", ManuMonth, ManuDay);
                fprintf(db, "%f %f %d %d\n", ManuAmount, ManuCN, ManuType, ManuMethod);


            }
            fclose(db);

            //Plastic Filming
            sprintf(DB,"%s/Inter/CropPlastic_%d_%d.txt", out_path, i, j);
            db=fopen(DB, "w");
            if(db==NULL)
            {
                cout << "Can not create file " << DB << endl;
                exit(0);
            }

            int weedn, WeedOption, WeedMonth1, WeedDay1, WeedMonth2, WeedDay2;
            if(MissFlag==0) fscanf(fp,"%s %d", note,&weedn);// "Weed_number---"
            else fscanf(fp,"%d", &weedn);
            MissFlag=0;

            //if(strcmp(note, "Plastic_applications=")!=0) Toend("Plastic_applications=", fname, Line);
            //Line++; XX(fp);

            //cycle.get(ri,yc)->WeedNumber = weedn;
            //SetArrayRange(5, weedn);
            fscanf(fp,"%s %d", note,&WeedOption);// "Weed_Problem"
            //if(strcmp(note, "Ventilation=")!=0) Toend("Ventilation=", fname, Line);
            Line++;
            XX(fp);

            fprintf(db, "%d %d\n", weedn, WeedOption);

            //WeedParameter weed1;
            for (k=1; k<=weedn; k++)
            {
                fscanf(fp,"%s %d", note,&temp);// "th Weeding:"			//Month,Day,method"
                //if(strcmp(note, "Plastic_ID=")!=0) Toend("Plastic_ID=", fname, Line);
                Line++;
                XX(fp);

                fscanf(fp,"%s %d %d", note,&WeedMonth1, &WeedDay1);
                fscanf(fp,"%s %d %d", note,&WeedMonth2, &WeedDay2);
                //if(strcmp(note, "Start/End=")!=0) Toend("Start/End=", fname, Line);
                Line++;
                XX(fp);

                fprintf(db, "%d %d\n", WeedMonth1, WeedDay1);
                fprintf(db, "%d %d\n", WeedMonth2, WeedDay2);
                //weed.get(ri,yc,k)->month=weed1.month;
                //weed.get(ri,yc,k)->day =weed1.day;
                //fscanf(fp,"%d %d %s",&weed.get(ri,yc,k)->month, &weed.get(ri,yc,k)->day, note);
            }
            fclose(db);

            //Flooding
            sprintf(DB,"%s/Inter/CropFloo_%d_%d.txt", out_path, i, j);
            db=fopen(DB, "w");
            if(db==NULL)
            {
                cout << "Can not create file " << DB << endl;
                exit(0);
            }

            fscanf(fp,"%s %d", note,&floon);// "Flood_number---"
            if(strcmp(note, "Flood_number=")!=0) Toend("Flood_number=", fname, Line);
            Line++;
            XX(fp);

            fscanf(fp,"%s %d", note,&FdLeak);// Leak type"
            if(strcmp(note, "Leak_type=")!=0) Toend("Leak_type=", fname, Line);
            Line++;
            XX(fp);

            fscanf(fp,"%s %d", note,&WaterControl);//
            if(strcmp(note, "Water_control=")!=0) Toend("Water_control=", fname, Line);
            Line++;
            XX(fp);

            fscanf(fp,"%s %f", note, &WaterLeakRate);
            if(strcmp(note, "Leak_rate=")!=0) Toend("Leak_rate=", fname, Line);
            Line++;
            XX(fp);

            fprintf(db, "%d %d %d %f\n", floon, FdLeak, WaterControl, WaterLeakRate);

            for (k=1; k<=floon; k++)
            {
                fscanf(fp,"%s %d", note,&temp);// "th Flooding:"
                if(strcmp(note, "Flooding_ID=")!=0) Toend("Flooding_ID=", fname, Line);
                Line++;
                XX(fp);

                fscanf(fp,"%s %d %d", note,&FlooMonth1, &FlooDay1);
                if(strcmp(note, "Flood_Month/Day=")!=0) Toend("Flood_Month/Day=", fname, Line);
                Line++;
                XX(fp);

                fscanf(fp,"%s %d %d", note,&FlooMonth2, &FlooDay2);
                if(strcmp(note, "Drain_Month/Day=")!=0) Toend("Drain_Month/Day=", fname, Line);
                Line++;
                XX(fp);

                fscanf(fp,"%s %f", note,&FlooN);
                //if(strcmp(note, "Water_N=")!=0) Toend("Water_N=", fname, Line);
                Line++;
                XX(fp);

                fscanf(fp,"%s %d", note,&ShallowFlood);
                if(strcmp(note, "Shallow_flood=")!=0) Toend("Shallow_flood=", fname, Line);
                Line++;
                XX(fp);

                fprintf(db, "%d %d\n", FlooMonth1, FlooDay1);
                fprintf(db, "%d %d\n", FlooMonth2, FlooDay2);
                fprintf(db, "%f\n", FlooN);
                fprintf(db, "%d\n", ShallowFlood);
            }

            fscanf(fp,"%s", note);
            if(strcmp(note, "Water_gather=")!=0)
            {
                WaterGetherIndex = 1.0;
                MissFlag=1;
            }
            else
                fscanf(fp,"%f", &WaterGetherIndex);

            fprintf(db, "%f\n", WaterGetherIndex);

            if(MissFlag==1)
            {
                if(strcmp(note, "WT_file=")!=0)
                {
                    sprintf(WT_file, "None");
                }
                else
                {
                    fscanf(fp,"%s", WT_file);
                    MissFlag=0;
                }
            }
            else
            {
                fscanf(fp,"%s", note);
                if(strcmp(note, "WT_file=")!=0)
                {
                    sprintf(WT_file, "None");
                    MissFlag=1;
                }
                else
                {
                    fscanf(fp,"%s", WT_file);
                }
            }

            fprintf(db, "%s\n", WT_file);


            if(MissFlag==1)
            {
                if(strcmp(note, "Empirical_parameters=")!=0)
                {
                    m_IniWT=0.0;
                    m_LWTceasingSurfFlow=0.0;
                    m_LWTceasingGroungFlow=0.0;
                    m_WatershedIndex=0.0;
                    m_SurfOutflowIntensity=0.0;
                    m_GroundOutflowIntensity=0.0;

                    MissFlag=1;
                }
                else
                {
                    fscanf(fp,"%f %f %f %f %f %f", &m_IniWT, &m_LWTceasingSurfFlow, &m_LWTceasingGroungFlow, &m_WatershedIndex,
                           &m_SurfOutflowIntensity, &m_GroundOutflowIntensity);
                    MissFlag=0;
                }
            }
            else
            {
                fscanf(fp,"%s", note);
                if(strcmp(note, "Empirical_parameters=")!=0)
                {
                    m_IniWT=0.0;
                    m_LWTceasingSurfFlow=0.0;
                    m_LWTceasingGroungFlow=0.0;
                    m_WatershedIndex=0.0;
                    m_SurfOutflowIntensity=0.0;
                    m_GroundOutflowIntensity=0.0;

                    MissFlag=1;
                }
                else
                {
                    fscanf(fp,"%f %f %f %f %f %f", &m_IniWT, &m_LWTceasingSurfFlow, &m_LWTceasingGroungFlow, &m_WatershedIndex,
                           &m_SurfOutflowIntensity, &m_GroundOutflowIntensity);
                    MissFlag=0;
                }
            }

            fprintf(db, "%f %f %f %f %f %f\n", m_IniWT, m_LWTceasingSurfFlow, m_LWTceasingGroungFlow, m_WatershedIndex,
                    m_SurfOutflowIntensity, m_GroundOutflowIntensity);

            fclose(db);

            //Irrigation
            sprintf(DB,"%s/Inter/CropIrri_%d_%d.txt", out_path, i, j);
            db=fopen(DB, "w");
            if(db==NULL)
            {
                cout << "Can not create file " << DB << endl;
                exit(0);
            }

            int irrin, IrriMode, IrriMonth, IrriDay, IrriMethod;
            float IrriIndex, IrriAmount;

            if(MissFlag==0) fscanf(fp,"%s", note);
            fscanf(fp,"%d", &irrin);// "Irrigation_number---"
            MissFlag=0;

            if(strcmp(note, "Irrigation_number=")!=0) Toend("Irrigation_number=", fname, Line);
//            Line++;
//            XX(fp);

            fscanf(fp,"%s %d", note,&IrriMode);// Irrigation type"
            if(strcmp(note, "Irrigation_type=")!=0) Toend("Irrigation_type=", fname, Line);
//            Line++;
//            XX(fp);

            fscanf(fp,"%s %f", note,&IrriIndex);// Irrigation type"
            if(strcmp(note, "Irrigation_Index=")!=0) Toend("Irrigation_Index=", fname, Line);
//            Line++;
//            XX(fp);

            fprintf(db, "%d %d %f\n", irrin, IrriMode, IrriIndex);

            //cycle.get(ri,yc)->IrrNumber = irrin;
            //SetArrayRange(7, irrin);
            for (k=1; k<=irrin; k++)
            {
                fscanf(fp,"%s %d", note,&temp);
                if(strcmp(note, "Irrigation_ID=")!=0) Toend("Irrigation_ID=", fname, Line);
//                Line++;
//                XX(fp);

                fscanf(fp,"%s %d %d", note,&IrriMonth, &IrriDay);
                if(strcmp(note, "Irr_Month/Day=")!=0) Toend("Irr_Month/Day=", fname, Line);
//                Line++;
//                XX(fp);

                int jd;
                fscanf(fp,"%s %f %d", note,&IrriAmount, &jd);
                if(jd!=0&&jd!=1&&jd!=2) IrriMethod=0;
                else IrriMethod=jd;
                //if(strcmp(note, "Water_amount/Method=")!=0) Toend("Water_amount/Method=", fname, Line);
//                Line++;
//                XX(fp);

                fprintf(db, "%d %d\n", IrriMonth, IrriDay);
                fprintf(db, "%f %d\n", IrriAmount, IrriMethod);
            }
            fclose(db);

            //Grazing
            sprintf(DB,"%s/Inter/CropGraz_%d_%d.txt", out_path, i, j);
            db=fopen(DB, "w");
            if(db==NULL)
            {
                cout << "Can not create file " << DB << endl;
                exit(0);
            }

            int grazn, GrazMonth1, GrazDay1, GrazMonth2, GrazDay2;
            float GrazHour, Dairy, Beef, Pig, Horse, Sheep;

            fscanf(fp,"%s %d", note,&grazn);// "Grazing_number---"
            if(strcmp(note, "Grazing_number=")!=0) Toend("Grazing_number= ", fname, Line);
//            Line++;
//            XX(fp);

            fprintf(db, "%d\n", grazn);

            //cycle.get(ri,yc)->GrazingNumber = grazn;
            //SetArrayRange(8, grazn);
            for (k=1; k<=grazn; k++)
            {
                fscanf(fp,"%s %d", note,&temp);
                if(strcmp(note, "Grazing_ID=")!=0) Toend("Grazing_ID=", fname, note);
                Line++;
                XX(fp);

                fscanf(fp,"%s %d %d", note,&GrazMonth1, &GrazDay1);
                if(strcmp(note, "Start_Month/Day=")!=0) Toend("Start_Month/Day=", fname, note);
                Line++;
                XX(fp);

                fscanf(fp,"%s %d %d", note,&GrazMonth2, &GrazDay2);
                if(strcmp(note, "End_Month/Day=")!=0) Toend("End_Month/Day=", fname, note);
                Line++;
                XX(fp);
                fscanf(fp,"%s", note);
                if(strcmp(note, "Dairy_Head_per_ha=")!=0)//old version
                {
                    //MissFlag=1;
                    fscanf(fp,"%f", &Beef);
                    fscanf(fp,"%s %f", note,&Horse);
                    fscanf(fp,"%s %f", note,&Sheep);
                    Pig = 0.0;
                    Dairy = 0.0;
                }
                else//new version
                {
                    fscanf(fp,"%f", &Dairy);
                    fscanf(fp,"%s %f", note,&Beef);
                    fscanf(fp,"%s %f", note,&Pig);
                    fscanf(fp,"%s %f", note,&Sheep);
                    fscanf(fp,"%s %f", note,&Horse);
                }

                fscanf(fp,"%s %f", note,&GrazHour);
                if(strcmp(note, "Graz_Hours_Per_day=")!=0) Toend("Graz_Hours_Per_day=", fname,note);

                fprintf(db, "%d %d\n", GrazMonth1, GrazDay1);
                fprintf(db, "%d %d\n", GrazMonth2, GrazDay2);
                fprintf(db, "%f\n", GrazHour);
                fprintf(db, "%f %f %f %f %f\n", Dairy, Beef, Pig, Sheep, Horse);

            }
            fclose(db);

            //Grass cutting
            sprintf(DB,"%s/Inter/GrassCut_%d_%d.txt", out_path, i, j);
            db=fopen(DB, "w");
            if(db==NULL)
            {
                cout << "Can not create file " << DB << endl;
                exit(0);
            }

            int cutn, CutMonth, CutDay, CutPart;
            float CutAmount;

            fscanf(fp,"%s %d", note,&cutn);// "Cutting_number---"
            Line++;
            XX(fp);

            if(strcmp(note, "Cut_number=")==0)
            {
                fprintf(db, "%d\n", cutn);

                for (k=1; k<=cutn; k++)
                {
                    if(MissFlag==0) fscanf(fp,"%s %d", note,&temp);
                    else
                    {
                        fscanf(fp,"%d", &temp);
                        MissFlag = 0;
                    }
                    if(strcmp(note, "Cut_ID=")!=0) Toend("Cut_ID=", fname, Line);
                    Line++;
                    XX(fp);

                    fscanf(fp,"%s %d %d", note,&CutMonth, &CutDay);
                    if(strcmp(note, "Cut_Month/Day=")!=0) Toend("Cut_Month/Day=", fname, Line);
                    Line++;
                    XX(fp);

                    fscanf(fp,"%s %f", note,&CutAmount);
                    if(strcmp(note, "Cut_amount=")!=0) Toend("Cut_amount=", fname, Line);
                    Line++;
                    XX(fp);

                    fscanf(fp,"%s", note);
                    if(strcmp(note, "Cut_part=")!=0)
                    {
                        MissFlag=1;
                        CutPart=0;
                    }
                    else
                    {
                        fscanf(fp,"%d", &CutPart);
                    }

                    fprintf(db, "%d %d\n", CutMonth, CutDay);
                    fprintf(db, "%f %d\n", CutAmount, CutPart);
                }
            }
            fclose(db);
        }
    }
    fscanf(fp,"%s", note);
    if(strcmp(note, "----------------------------------------")==0) FarmMode = 1;
    if(strcmp(note, "Crop_model_approach")==0)
    {
        FarmMode = 0;
        MissFlag = 1;
    }

    if(FarmMode==1)
        goto NewFarmCrop;

    if(MissFlag==0) fscanf(fp,"%s %d", note,&IData->CropModel);
    else
    {
        fscanf(fp,"%d", &IData->CropModel);
        MissFlag=0;
    }
    if(strcmp(note, "Crop_model_approach")!=0)
    {
        //Toend("Crop_model_approach", fname, Line);
        //Line++; XX(fp);
        IData->CropModel = 0;
    }


    if(IData->ProfileType==1)
    {
        char SPF[255];
        FILE *spf;
        sprintf(SPF,"%s/Inter/SoilProfileData.spf", out_path);
        spf=fopen(SPF, "w");
        if(spf==NULL)
        {
            cout << "Cannot create file SoilProfileData.spf" << endl;
        }

        int i;
        fscanf(fp, "%f %d", &IData->m_SPthickness, &IData->m_SPlayers);
        fprintf(spf, "%f %d\n", IData->m_SPthickness, IData->m_SPlayers);
        for(i=1; i<=10; i++)
        {
            fscanf(fp, "%d %f %f %f %d %f %f %f %f %f %f",
                   &IData->layer[i], &IData->m_SPth[i], &IData->m_SPd[i], &IData->m_SPsoc[i], &IData->m_SPso[i], &IData->m_SPph[i], &IData->m_SPfc[i], &IData->m_SPwp[i], &IData->m_SPpo[i], &IData->m_SPhy[i], &IData->m_SPc[i]);

            fprintf(spf, "%d %f %f %f %d %f %f %f %f %f %f\n", IData->layer[i], IData->m_SPth[i], IData->m_SPd[i], IData->m_SPsoc[i], IData->m_SPso[i], IData->m_SPph[i], IData->m_SPfc[i], IData->m_SPwp[i], IData->m_SPpo[i], IData->m_SPhy[i], IData->m_SPc[i]);
        }
        fscanf(fp, "%s", IData->m_ProfileName);
        fprintf(spf, "%s\n", IData->m_ProfileName);
        fclose(spf);
    }

    fclose(fp);


    char CM[200];
    FILE *cm;
    sprintf(CM,"%s/Inter/CropModel", out_path);
    cm=fopen(CM, "w");
    if(cm==NULL)
    {
        cout << "Can not create file " << CM << endl;
        exit(0);
    }
    else
    {
        fprintf(cm, "%d\n", IData->CropModel);
        fclose(cm);
    }

////////////////////////////////////////////// GetAMT //////////////////////////////////////


/////////////////////////////////////////////  END    //////////////////////////////////////

    /// Initial the simulation data


////////////////////////////////////////////// SaveSite /////////////////////////////////////

    FILE *fpo;
    char fout[255];
    cout << "debug" << endl;
    cout << fname << endl;

    sprintf(fout,"%s/Inputs/site",out_path);
    fpo=sopen(fout,0);
    if (fpo==NULL) return;

    if (strlen(IData->sitename)==0)
    {
        cout << "No Site name information!" << "Input Error:" << endl;
        return ;
    }
    fprintf(fpo,"%s\n",IData->sitename);
    if (IData->years<=0)
    {
        cout << "No Simulated year information!" << "Input Error:" << endl;
        fclose(fpo);
        return ;
    }
    /*if (&IData->latitude<=0) {
    	MessageBox(NULL,"No latitude information!","Input Error:",0);
    	fclose(fpo);
    	return -1;
    }*/

    fprintf(fpo,"%d %d %f %ld %f\n", IData->years, IData->ifdaily,
            IData->latitude, IData->totalday, IData->longitude);//, &IData->if1File);
    fprintf(fpo,"%d\n", IData->Soil_landuse);//&IData->CropModel);
    fclose (fpo);

    sprintf(fout, "%s/Inputs/SPAN",out_path);
    fpo=sopen(fout,0);
    if (fpo==NULL) return ;
    fprintf(fpo, "%d\n", 1);//Span
    fclose(fpo);
//////////////////////////////////////// END   /////////////////////////////////////////////////////


////////////////////////////////////////  RecordClimateFile  ///////////////////////////////////////

    int yn, result=-1, TotalYear, FirstYear;
    float longitute, latitude;

#ifdef GLOBE_CLIM
    &IData->ClimateFileType= 4;
#endif

    if(DaymetFlag==1)
    {
        char DMF[255];
        FILE *dmf;
        sprintf(DMF,"%s/Inter/SiteLocation.txt", out_path);
        dmf=fopen(DMF, "r");
        if(dmf==NULL)
        {
            cout << "Can not open file %s" <<  DMF << endl;
        }

        fscanf(dmf, "%f %f %d %d", &longitute, &latitude, &TotalYear, &FirstYear);
        fclose(dmf);

        IData->ClimateFileType = 3;
    }
    else
    {
    }

    i = ClimateFilename.n;
    yn = IData->years;

    if (i<IData->years && IData->if1File==0)
    {
        cout << "No enough Climate data files! Please re-input." << "Input Error:" << endl;
        return;
    }


///  CLIM.INP file

    char foutname[255];
    char ch[255];//cn[250],
//	int j, k, jday,jday0,tn;
//	float maxT, minT, Rainfall, temp, solrad, Day_Hrs;
    float MonAveT[15], YrAveT,YrRain;
    //FILE *fpo;//*fpi,

//    sprintf(foutname,"%s/Inputs/CLIM.INP", out_path);
//    fpo=sopen(foutname,"w");
//
//
//    fprintf(fpo, "---------------------------------------------------------------\n");
//    fprintf(fpo, "Climate: \n");
//    fprintf(fpo, "    Year  File name   Temp (C)  Prec (cm)  N_in_rain(ppm)\n");

/// mode=0:Get initial Soil T   mode=1:ReSave climate data for simulation
    for (int i = 1; i<=IData->years; i++)
    {
        if(DaymetFlag==1)
        {
            sprintf(ch,"%s/Inter/daymet%d", out_path, FirstYear+i-1);
        }
        else
        {
            if(IData->if1File==0)
            {
                sprintf(ch,"%s",ClimateFilename.get(i));
            }
            else
                sprintf(ch,"%s",ClimateFilename.get(1));
        }


        SaveClimateFileOfAYear(i,ch,MonAveT,&YrAveT,&YrRain,IData->latitude,
                               IData->ClimateFileType,IData->NO3NH4_Rainfall,
                               IData->NH3_Atmosphere,IData->BaseCO2, out_path ,mode, IData->WaterYrMonth,
                               IData->WaterYrDay, IData->CO2IncreaseRate,IData,0,0);

//        fprintf(fpo, "    %3d   %s          %.2f      %.2f       %.2f\n",
//                1, ch, YrAveT, YrRain, IData->NO3NH4_Rainfall );

        //if (mode==0) return 0;

    }

//    fclose(fpo);


///////////////////////////////////////////////// END ///////////////////////////////////////////

///////////////////////////////////////////////  ReadSoilFeature  //////////////////////////////

    FILE *fpi;
    char f_in_name[255];

    float DClay, DFldcap, DWiltpt, DSks, DPorosity;

    //&IData->Soil_Texture = Soiltexture.ListIndex + 1
    if (IData->Soil_Texture<1 && IData->Soil_Texture>13) return ;

    cout << "test1" << endl;
    cout << &IData->Soil_Texture << endl;
    sprintf(f_in_name, "%s/Library/Lib_soil/soil_%d", root_path, IData->Soil_Texture);
    cout << "test2" << endl;

    //sprintf(note,"%d",&IData->Soil_Texture);
    //strcat(fname,note);
    if ((fpi=sopen(f_in_name, "r")) == NULL)
    {
        cout << "You may have the DNDC model installed in an incorrect place. Please move the entire DNDC folder to the root directory of any drive in your computer!" << endl;
        cout << " Can not find Library Folder" << endl;
        exit(0);
    }

    fscanf(fpi,"%s %s", &IData->SoilName,note);//&IData->NameNote);
    fscanf(fpi,"%f %s", &DClay,note);
    fscanf(fpi,"%f %s", &DPorosity, note);//&IData->StsNote);
    fscanf(fpi,"%f %s", &DSks, note);//&IData->SksNote);
    fscanf(fpi,"%f %s", &DFldcap, note);//&IData->FldcapNote);
    fscanf(fpi,"%f %s", &DWiltpt, note);//&IData->WiltptNote);
    //fscanf(fpi,"%f %s", &IData->Soilhtcp, note);//&IData->SoilhtcpNote);
    //fscanf(fpi,"%f %s", &IData->Psisat, note);//&IData->PsisatNote);
    //fscanf(fpi,"%f %s", &IData->Beta, note);//&IData->BetaNote);
    fclose (fpi);

    DSks *= 0.6;//cm/min ->m/hr

    if(IData->Soil_Clay==0.0) IData->Soil_Clay = DClay;
    if(IData->Fldcap==0.0) IData->Fldcap = DFldcap;
    if(IData->Wiltpt==0.0) IData->Wiltpt = DWiltpt;
    if(IData->Sks==0.0) IData->Sks = DSks;
    if(IData->Soil_Porosity==0.0) IData->Soil_Porosity = DPorosity;

    cout << IData->SoilName << endl;


//////////////////////////////////////////////// END ///////////////////////////////////////////

//////////////////////////////////////////////// ReadClimateHead  //////////////////////////////

    cout << "ReadClimateHead" << endl;
    ReadClimateHead(1,out_path,IData);
//////////////////////////////////////////////// END ///////////////////////////////////////////


//////////////////////////////////////////////// SetSoilH /////////////////////////////////////

    IData->AcWater   = 0;
    IData->SurfWater = 0;
    IData->SnowPack  = 0;

    //&IData->Idc = 1# + IIf(soilbypass(0).Value = True, 0, 1) 'Soil_BypassFlow
    IData->Idc = 1 + (int)IData->Soil_BypassFlow;

    //if (&IData->Soil_Texture != 12) { //'If Soiltexture.ListIndex <> 11 Then
    //     &IData->Sts = float(1 - &IData->Soil_Density * (&IData->Soil_SOCatSurface / 1.3 + (1 - &IData->Soil_SOCatSurface) / 2.6));
    //
    //if (&IData->Sts<=0) //safety check
    /*{
    	MessageBox(NULL,"Soil-Density or Soil Organic may be wrong! Please input a smaller one.","Inputed Error:",NULL);
    	&IData->Sts = 1;
    	return -1;
    }*/

    //if (&IData->Soil_Texture < 10) &IData->Sks = &IData->RawSKS * (float)0.6; //cm/min -> m/hr
    IData->Qstar = (float)(6.45 * (IData->Soil_Clay - 0.01) + 1);
    IData->BypassF = IData->Soil_BypassFlow;//(float)0.1 * (&IData->Idc - 1);

    //float T01 = 1;
    //if (&IData->BypassF > 0.0) T01 = &IData->BypassF * 10.0 + 1.0;
    //&IData->Sks = &IData->Sks * T01;

    float dt;
    dt = (2 * 0.005 / IData->Soil_Porosity);
    if (dt < IData->Sks)
    {
        IData->H = dt;
    }
    else
    {
        IData->H = IData->Sks;
    }
    if (IData->H <= 0)
    {
        cout << "There is a parameter error of Soil_H=0 ! Please check system file and your data. " "Input Error :" << endl;
        return ;
    }

    int total_layer=MAXSOILLAYER-10;
    IData->TH = SOILDEPTH;//0.9;//otal depth of simulated soil profile, m
    if (IData->H*total_layer < IData->TH )
        IData->H = IData->TH / total_layer;//total layer number is <=50
    IData->Q = (int)(IData->TH / IData->H);

    if (IData->Q > total_layer)
    {
        IData->Q = total_layer;
        IData->H = IData->TH / total_layer;
    }
//////////////////////////////////////////////// END ///////////////////////////////////////////

////////////////////////////////////////////////  SaveInterFarmParas ///////////////////////////

    SaveInterFarmParas(out_path);
///////////////////////////////////////////////  END //////////////////////////////////////////

//////////////////////////////////////////////// SaveSoilParas /////////////////////////////////

    SaveSoilParas(1,*IData,out_path);
//////////////////////////////////////////////// END ///////////////////////////////////////////

//////////////////////////////////////////////// SaveCropParas  ////////////////////////////////

    SaveCropParas(out_path,root);
//////////////////////////////////////////////// END ///////////////////////////////////////////



    cout << "DND Loaded successuly" << endl;

    //Site_Simulator("c:\\dndc");
    //start("c:\\dndc");

}




void class_model::ReadInputDatafromXML(string xmlname, char *out_path)
{
    /// Load JSON files,which are site configure..

    cout << "Load info form JSON file" << endl;
    cout << "New Drought Warning System Version 9.4" << endl;

    //char root_path[20] = "C:\\DNDC";
    //char root_path[20] = root;
//    char root_path[3072];
//    strcpy(root_path,root);


    int  i, j, k, cropn, MissFlag=0;
    int floon, FdLeak, ShallowFlood, WaterControl, FlooMonth1, FlooDay1, FlooMonth2, FlooDay2;
    float FlooN, WaterGetherIndex, WaterLeakRate;
    float m_IniWT, m_LWTceasingSurfFlow, m_LWTceasingGroungFlow, m_WatershedIndex,m_SurfOutflowIntensity, m_GroundOutflowIntensity;
    char tmp[255],note[255];
    FILE *fp;
    StringArray ClimateFilename;
    int DaymetFlag = 0;
    int mode = 0;

//    Py_Initialize();

    string argvarr[1];
    argvarr[0] = xmlname;

//    PyRun_SimpleString("import gc");
//    PyRun_SimpleString("print len(gc.get_objects())");
//
//
    PyObject* inxml = CallPythonFunction("read_input_xml","Read_SiteConfig", 1,argvarr);
//
//
//    PyObject *pFunc,*pArgs, *pValue, *inxml;

//    PyObject* sysPath = PySys_GetObject((char*)"path");
//    PyObject* curDir = PyString_FromString(".");

//    PyRun_SimpleString("import os\n");
//    PyRun_SimpleString("import sys\n");
//    PyRun_SimpleString("print sys.version");


    //PyEval_CallObject(strfunc, strargs);
//
//    if (pModule != NULL)
//    {
//        pFunc = PyObject_GetAttrString(pModule, "Read_SiteConfig");
//        /* pFunc is a new reference */
//        pArgs = PyTuple_New(1);
//        if (pFunc && PyCallable_Check(pFunc))
//        {
//            // prepare arguments for program.
//
//            pValue = PyString_FromString(xmlname.c_str());
//            /* pValue reference stolen here: */
//            PyTuple_SetItem(pArgs, 0, pValue);
//        }
//        inxml = PyObject_CallObject(pFunc, pArgs);
//        Py_DECREF(pArgs);
//        Py_DECREF(pFunc);
//        Py_DECREF(pValue);
//
//    }
//    else
//    {
//        cout << "Read Plug error, exit " << endl;
//        exit(-1);
//    }



    // Initial IData class
//    Site_BaseInformation IData;

//    float nSoca[6]={0.0,0.2,0.2,0.08,0.05,0.2};
//    float nSocb[6]={10.0,2.0,2.0,1.4,4.0,2.0};


    char txt[200];

    char tmpname[300];
    strcpy(tmpname,xmlname.c_str());
    if ((fp=sopen(tmpname,1))==NULL)
    {
        cout << "Can not open " <<  xmlname << endl;
        return;
    }

    int Line=1;

    //Site data:
    PyObject *pSitename = PyString_FromString("site");
    PyObject *pSite = PyObject_GetAttr(inxml,pSitename);

    strcpy(IData->sitename,PyString_AsString(PyObject_GetAttrString(pSite,"Site_Name")));
    IData->years=atoi(PyString_AsString(PyObject_GetAttrString(pSite,"Simulated_Year")));

    cout << "reading  total years   " << IData->years << endl;
    IData->startyear = atoi(PyString_AsString(PyObject_GetAttrString(pSite,"Start_Year")));
    IData->latitude = atof(PyString_AsString(PyObject_GetAttrString(pSite,"Latitude")));
    IData->ifdaily = atoi(PyString_AsString(PyObject_GetAttrString(pSite,"Daily_Record")));
    IData->longitude = atof(PyString_AsString(PyObject_GetAttrString(pSite,"Longitude")));
    IData->continent = atoi(PyString_AsString(PyObject_GetAttrString(pSite,"Continent")));
    IData->elevation = atof(PyString_AsString(PyObject_GetAttrString(pSite,"Elevation")));

    Py_DECREF(pSitename);
    Py_DECREF(pSite);


    //climate data
    PyObject *pClimatename = PyString_FromString("climate");
    PyObject *pClimate = PyObject_GetAttr(inxml,pClimatename);

    IData->ClimateFileType = atoi(PyString_AsString(PyObject_GetAttrString(pClimate,"Climate_Data_Type")));
    IData->NO3NH4_Rainfall = atof(PyString_AsString(PyObject_GetAttrString(pClimate,"NO3NH4_in_Rainfall")));
    IData->NH3_Atmosphere = atof(PyString_AsString(PyObject_GetAttrString(pClimate,"NO3_of_Atmosphere")));
    IData->BaseCO2 = atof(PyString_AsString(PyObject_GetAttrString(pClimate,"BaseCO2_of_Atmosphere")));
    IData->ClimateFileNumber = atoi(PyString_AsString(PyObject_GetAttrString(pClimate,"Climate_file_count")));
    //If use same climate file for all years
    IData->if1File = atoi(PyString_AsString(PyObject_GetAttrString(pClimate,"Climate_file_mode")));
    IData->CO2IncreaseRate = atof(PyString_AsString(PyObject_GetAttrString(pClimate,"CO2_increase_rate")));

    string climatenames =  PyString_AsString(PyObject_GetAttrString(pClimate,"Climate_Name"));
    string climatepath =  PyString_AsString(PyObject_GetAttrString(pClimate,"Data_Path"));
    IData->ClimateFileName = climatenames;
    IData->ClimatePath = climatepath;
    cout << "Climat Name " << climatenames << endl;
    cout << "Climate Path " << climatepath << endl;


    Py_DECREF(pClimatename);
    Py_DECREF(pClimate);


    // Read Soil parameter
    PyObject *pSoilname = PyString_FromString("soil");
    PyObject *pSoil = PyObject_GetAttr(inxml,pSoilname);

    IData->Soil_Texture=atoi(PyString_AsString(PyObject_GetAttrString(pSoil,"Soil_Texture")));
    IData->Soil_landuse = atoi(PyString_AsString(PyObject_GetAttrString(pSoil,"Landuse_Type")));
    IData->Soil_Density = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Density")));
    IData->Soil_pH = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Soil_pH")));
    IData->Soil_SOCatSurface = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"SOC_at_Surface")));
    IData->Soil_Clay = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Clay_fraction")));
    IData->Soil_BypassFlow = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"BypassFlow")));
    IData->Soil_Litter=atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Litter_SOC")));
    IData->Soil_humads = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Humads_SOC")));
    IData->Soil_humus = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Humus_SOC")));
    IData->Soil_NO3 = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Soil_NO3")));
    IData->Soil_NH4 = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Soil_NH4")));
    IData->Soil_moiture = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Moisture")));
    IData->LateralInflux = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Lateral_influx_index")));
    IData->Fldcap = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Field_capacity")));
    IData->Wiltpt = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Wilting_point")));
    IData->Sks = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Hydro_conductivity")));
    IData->Soil_Porosity = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Soil_porosity")));
    IData->SOCa = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"SOC_profile_A")));
    IData->SOCb = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"SOC_profile_B")));
    IData->DClitter = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"DC_litter_factor")));
    IData->DChumads = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"DC_humads_factor")));
    IData->DChumus = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"DC_humus_factor")));
    IData->HumadCN = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Humad_CN")));
    IData->HumusCN = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Humus_CN")));
    IData->PassiveC=atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Soil_PassiveC")));
    IData->Soil_MicrobioIndex = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Soil_microbial_index")));
    IData->HighestWT = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Highest_WT_depth")));
    IData->Soil_WaterObstructLayer = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Depth_WRL_m")));
    IData->slope = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Slope")));
    IData->UseION = atoi(PyString_AsString(PyObject_GetAttrString(pSoil,"UseION")));
    IData->Soil_Quality = atof(PyString_AsString(PyObject_GetAttrString(pSoil,"Salinity")));
    IData->ProfileType = atoi(PyString_AsString(PyObject_GetAttrString(pSoil,"Profile_type")));



    if (IData->Soil_landuse == 2)
    {
        IData->Soil_WaterObstructLayer = 0.30; //meter
    }
    else
    {
        IData->Soil_WaterObstructLayer = 9.99; //meter
    }

    if(IData->Sks<0.015) IData->Sks = 0.015;


    Py_DECREF(pSoilname);
    Py_DECREF(pSoil);

    char fname[300];
    // Read crop parameter
    int ri, temp, yc, FCTT=0;


    PyObject *pManagement = PyObject_GetAttr(inxml,PyString_FromString("management"));
    PyObject* pManage_List_Item;

    int rotation_num = atoi(PyString_AsString(PyObject_GetAttrString(inxml,"Rotation_Number")));
//
    printf("Rotation num %i \n",rotation_num);

    IData->Rotation_Number = rotation_num;



    char DB[3072];
    FILE *db;
    sprintf(DB,"%s/Inter/CropRotation.txt", out_path);//###

    db=fopen(DB, "w");
    fprintf(db, "%d\n", IData->Rotation_Number);
    fclose(db);

    int CropNumbers;

    int yr = 0; // total simulation years
    int kkk;
    int db_RotateSpan[200];

    for (int i=0; i<IData->Rotation_Number; i++)
    {

        int db_TotalYear;
        int tilln, TillMonth, TillDay, TillMethod;
        int manun, ManuMonth, ManuDay, ManuType, ManuMethod;
        float ManuAmount, ManuCN, ManuN;

        pManage_List_Item = PyList_GetItem(pManagement,i);
        ri = atoi(PyString_AsString(PyObject_GetAttrString(pManage_List_Item,"Rotation_ID")));

        db_TotalYear = IData->years;

        db_RotateSpan[i] = atoi(PyString_AsString(PyObject_GetAttrString(pManage_List_Item,"Years_Of_This_Cycle")));

        char outname[255];
        kkk = 0;

        for (int j = 1; j<=db_RotateSpan[i]; j++) //total years within a rotation
        {
            yr++;
            cropn = atoi(PyString_AsString(PyObject_GetAttrString(pManage_List_Item,"Crop_Number")));

            PyObject *db_CropID =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Crop_ID"));
            PyObject *db_CropType =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Crop_Type"));
            PyObject *db_CropName =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Crop_Name"));
            PyObject  *db_PlantDay =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Plant_Day"));
            PyObject  *db_HarvestDay =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Harvest_Day"));
            PyObject  *db_Hyear=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Year_of_harvest"));
            PyObject  *db_Residue=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Ground_Residue"));
            PyObject  *db_Yield=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Yield"));
            PyObject  *db_GrowthReproductive=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Rate_reproductive"));
            PyObject  *db_GrowthVegetative=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Rate_vegetative"));
            PyObject  *db_PsnEfficiency=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Psn_efficiency"));
            PyObject  *db_PsnMax=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Psn_maximum"));
            PyObject  *db_InitialBiomass=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Initial_biomass"));
            PyObject  *db_CoverCrop=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Cover_crop"));
            PyObject  *db_PerennialCrop=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Perennial_crop"));
            PyObject  *db_GrainFraction=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Grain_fraction"));
            PyObject  *db_ShootFraction=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Shoot_fraction"));
            PyObject  *db_RootFraction=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Root_fraction"));
            PyObject  *db_GrainCN=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Grain_CN"));
            PyObject  *db_ShootCN=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Shoot_CN"));
            PyObject  *db_RootCN=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Root_CN"));
            PyObject  *db_TDD=PyObject_GetAttr(pManage_List_Item,PyString_FromString("TDD"));
            PyObject  *db_Water=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Water_requirement"));
            PyObject  *db_LAI=PyObject_GetAttr(pManage_List_Item,PyString_FromString("LAI"));
            PyObject  *db_Nfix=PyObject_GetAttr(pManage_List_Item,PyString_FromString("N_fixation"));
            PyObject  *db_Vascularity=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Vascularity"));
            PyObject  *db_MaxRoot=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Max_root_depth"));
            PyObject  *db_CropModel=PyObject_GetAttr(pManage_List_Item,PyString_FromString("Crop_model_approach"));


            // Open output crop file
            sprintf(outname,"%s/Inputs/crop-%d",out_path,yr);
            FILE *fcrop=sopen(outname, "w");

            fprintf(fcrop, "%d\n", cropn);
            for (int k = 1; k<=cropn; k++)
            {
                int GrowDays;
                int WinterCropFlag;
                int LeftDays;

                int WCFlag = 0;
                int Left = 0;
                int CropID = atoi(PyString_AsString(PyList_GetItem(db_CropID,k-1)));
                char* Crop_Name = PyString_AsString(PyList_GetItem(db_CropName,k-1));
                int SeedDay = atoi(PyString_AsString(PyList_GetItem(db_PlantDay,k-1)));
                int HarvDay = atoi(PyString_AsString(PyList_GetItem(db_HarvestDay,k-1)));
                int HarvYear = atoi(PyString_AsString(PyList_GetItem(db_Hyear,k-1)));
                float LeftPrec = atof(PyString_AsString(PyList_GetItem(db_Residue,k-1)));

                float MaxPlantN =0.0;

                int CropOrder=111;
                if(k==cropn) CropOrder = 999;

                float InitialBiomass = atof(PyString_AsString(PyList_GetItem(db_InitialBiomass,k-1)));
                float Crop_Yield = atof(PyString_AsString(PyList_GetItem(db_Yield,k-1)));
                float GrowthVegetative = atof(PyString_AsString(PyList_GetItem(db_GrowthVegetative,k-1)));
                float GrowthReproductive = atof(PyString_AsString(PyList_GetItem(db_GrowthReproductive,k-1)));
                float PsnEfficiency = atof(PyString_AsString(PyList_GetItem(db_PsnEfficiency,k-1)));
                float PsnMax = atof(PyString_AsString(PyList_GetItem(db_PsnMax,k-1)));
                float CoverCrop = atof(PyString_AsString(PyList_GetItem(db_CoverCrop,k-1)));
                int PerennialCrop = atoi(PyString_AsString(PyList_GetItem(db_PerennialCrop,k-1)));
                float Grain = atof(PyString_AsString(PyList_GetItem(db_GrainFraction,k-1)));
                float Straw = atof(PyString_AsString(PyList_GetItem(db_ShootFraction,k-1)));
                float CropRoot = atof(PyString_AsString(PyList_GetItem(db_RootFraction,k-1)));
                float GrainCN = atof(PyString_AsString(PyList_GetItem(db_GrainCN,k-1)));
                float ShootCN = atof(PyString_AsString(PyList_GetItem(db_ShootCN,k-1)));
                float RootCN = atof(PyString_AsString(PyList_GetItem(db_RootCN,k-1)));
                float CropTDD = atof(PyString_AsString(PyList_GetItem(db_TDD,k-1)));
                float WRuq = atof(PyString_AsString(PyList_GetItem(db_Water,k-1)));
                float MXLAI = atof(PyString_AsString(PyList_GetItem(db_LAI,k-1)));
                float CropNFix = atof(PyString_AsString(PyList_GetItem(db_Nfix,k-1)));
                float Vascularity = atof(PyString_AsString(PyList_GetItem(db_Vascularity,k-1)));


                float Straw_Yield = Crop_Yield/Grain * Straw;
                float Root_Yield = Crop_Yield/Grain * CropRoot;
                if(CropID==0) HarvYear=1;
                if(HarvYear==1)//SeedDay<HarvDay)
                {
                    GrowDays = HarvDay - SeedDay;
                    WinterCropFlag = 0;
                    LeftDays = 0;
                }
                else
                {
                    GrowDays = 365 - SeedDay + HarvDay;
                    WinterCropFlag =  1;//yyy-1;
                    LeftDays = GrowDays;
                }

                if(CropID==0) MaxPlantN = 0.0;
                else MaxPlantN = Crop_Yield/GrainCN + Straw_Yield/ShootCN + Root_Yield/RootCN;



                fprintf(fcrop, "%d  %s  %d  %d  %f  %d\n",CropID,Crop_Name,WCFlag,Left,Crop_Yield,HarvYear);
                fprintf(fcrop, "%d  %d  %f  %f  %d\n", SeedDay,GrowDays,LeftPrec,MaxPlantN,CropOrder);

                fprintf(fcrop, "%f  %f  %f  %f  %f\n",
                        InitialBiomass, GrowthVegetative, GrowthReproductive,
                        PsnEfficiency, PsnMax);
                fprintf(fcrop, "%d\n", CoverCrop);
                fprintf(fcrop, "%d\n", PerennialCrop);
                fprintf(fcrop, "%f %f %f\n", Grain,Straw, CropRoot);
                fprintf(fcrop, "%f %f %f\n", GrainCN, ShootCN, RootCN);
                fprintf(fcrop, "%f %f %f %f %f\n", CropTDD, WRuq, MXLAI, CropNFix, Vascularity);



                //write file cropini

                char F_INI[200];
                FILE *fcropini;
                sprintf(F_INI, "%s/Inputs/cropini-1", out_path);
                fcropini = fopen(F_INI, "w");
                if ( fcropini == NULL )
                {

                    error_print(1,F_INI);
                }

                if(WinterCropFlag == 1)
                {
                    /*fprintf(fcropini, "1 78 16.10 15.7 0.16\n");
                    fprintf(fcropini, "92.17 27.72 4.95 47.71 416.2\n");
                    fprintf(fcropini, "0.45 49.23 157.57 1233.67 400.35 833.32 20.81 0.38\n");
                    fprintf(fcropini, "1405.38 2053.77 0.00 -0.35 -0.18 -0.16\n");
                    fprintf(fcropini, "7.78 2.61 4.44 0.24 12.22\n");
                    fprintf(fcropini, "6.15 0.00 0.00 6.15\n");
                    fprintf(fcropini, "12.39 2.49 0.96 0.24\n");
                    fprintf(fcropini, "0.00 0.00 10.16 0.00 0.00 0.00 4.91 3.32\n");
                    fprintf(fcropini, "1.00 0.72 0.14 4.92 2.0 0.00 0.00 0.00\n");
                    fprintf(fcropini, "-0.45 0.033 0.36 0.19 0.00 0.00 1.00\n");
                    fprintf(fcropini, "0.41 0.43 0.43 0.48 15.53 0.000045 0.90 0.006\n");
                    fprintf(fcropini, "0.0\n");*/
                    fprintf(fcropini, "1 78 98.782234 141.296143 3.370028\n");
                    fprintf(fcropini, "396.762939 791.093994 272.596741 60.061760 928.299866\n");
                    fprintf(fcropini, "0.3 260.431610 1222.437012 10054.463867 5001.835449 5052.628906 593.914429 0.456428\n");
                    fprintf(fcropini, "388.150146 425.000000 45.241417 16.600092 54.701843 0.000000\n");
                    fprintf(fcropini, "134.906097 59.428238 0.000000 57.709061 134.906097\n");
                    fprintf(fcropini, "6.000000 0.000000 0.000000 6.000000\n");
                    fprintf(fcropini, "41.017162 51.416466 1.178264 0.0\n");

                }
                else
                {
                    fprintf(fcropini, "0 0 0.0 0.0 0.0\n");
                    fprintf(fcropini, "0.0 0.0 0.0 0.0 0.0\n");
                    fprintf(fcropini, "0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0\n");
                    fprintf(fcropini, "0.0 0.0 0.0 0.0 0.0 0.0\n");
                    fprintf(fcropini, "0.0 0.0 0.0 0.0 0.0\n");
                    fprintf(fcropini, "0.0 0.0 0.0 0.0\n");
                    fprintf(fcropini, "0.0 0.0 0.0 0.0\n");
                    fprintf(fcropini, "0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0\n");
                    fprintf(fcropini, "0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0\n");
                    fprintf(fcropini, "0.0 0.0 0.0 0.0 0.0 0.0 0.0\n");
                    fprintf(fcropini, "0.0 0.0 0.0 0.0 0.0 0.0 0.0 0.0\n");
                    fprintf(fcropini, "0.0\n");

                }
                fclose(fcropini);


            }


            /// TODO about Year ID
            /// Need OR Not


            Py_XDECREF(db_CropID);
            Py_XDECREF(db_CropType);
            Py_XDECREF(db_CropName);
            Py_XDECREF(db_PlantDay);
            Py_XDECREF(db_HarvestDay);
            Py_XDECREF(db_Hyear);
            Py_XDECREF(db_Residue);
            Py_XDECREF(db_Yield);
            Py_XDECREF(db_GrowthReproductive);
            Py_XDECREF(db_GrowthVegetative);
            Py_XDECREF(db_PsnEfficiency);
            Py_XDECREF(db_PsnMax);
            Py_XDECREF(db_InitialBiomass);
            Py_XDECREF(db_CoverCrop);
            Py_XDECREF(db_PerennialCrop);
            Py_XDECREF(db_GrainFraction);
            Py_XDECREF(db_ShootFraction);
            Py_XDECREF(db_RootFraction);
            Py_XDECREF(db_GrainCN);
            Py_XDECREF(db_ShootCN);
            Py_XDECREF(db_RootCN);
            Py_XDECREF(db_TDD);
            Py_XDECREF(db_Water);
            Py_XDECREF(db_LAI);
            Py_XDECREF(db_Nfix);
            Py_XDECREF(db_Vascularity);
            Py_XDECREF(db_MaxRoot);
            Py_XDECREF(db_CropModel);

            fclose(fcrop);

            /// Tillage
            sprintf(DB,"%s/Inputs/till-%d",out_path,yr);
            db=fopen(DB, "w");
            if(db==NULL)
            {
                cout << "Can not create file " << DB << endl;
                exit(0);
            }


            tilln = atoi(PyString_AsString(PyObject_GetAttrString(pManage_List_Item,"Tillage_Number")));

            PyObject *db_TillMethod =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Tillage_Type"));
            PyObject *db_TillDay =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Tillage_Day"));
            PyObject *db_Tillage_ID =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Tillage_ID"));

            fprintf(db, "%d\n", tilln);

            for (int k=0; k<tilln; k++)
            {

                int TillDay =  atoi(PyString_AsString(PyList_GetItem(db_TillDay,k)));
                int TillMethod = atoi(PyString_AsString(PyList_GetItem(db_TillMethod,k)));
                int Tillage_ID = atoi(PyString_AsString(PyList_GetItem(db_Tillage_ID,k)));

                fprintf(db,"%d %d\n",TillDay,TillMethod);

            }
            fclose(db);


            Py_XDECREF(db_TillMethod);
            Py_XDECREF(db_TillDay);
            Py_XDECREF(db_Tillage_ID);


            /// Fertilization

            sprintf(DB,"%s/Inputs/fert-%d",out_path,yr);
            fp=fopen(DB, "w");
            if(fp==NULL)
            {
                cout << "Can not create file " << DB << endl;
                exit(0);
            }


            int fertn = atoi(PyString_AsString(PyObject_GetAttrString(pManage_List_Item,"Fertilization_Number")));

            PyObject *db_Fert_ID =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertilization_ID"));
            PyObject *db_Fertigation =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertigation"));
            PyObject *db_Fert_Day =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertilization_Day"));
            PyObject *db_Fert_Type =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertilization_Type"));
            PyObject *db_Fert_Method =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertilization_Method"));
            PyObject *db_Fert_Option =PyObject_GetAttr(pManage_List_Item,PyString_FromString("FertilizationOption"));
            PyObject *db_Nitrate_Amount =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertilization_Nitrate_Amount"));
            PyObject *db_Abi_Amount =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertilization_Abi_Amount"));
            PyObject *db_Urea_Amount =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertilization_Urea_Amount"));
            PyObject *db_Anh_Amount =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertilization_Anh_Amount"));
            PyObject *db_AmNi_Amount =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertilization_AmNi_Amount"));
            PyObject *db_NH42SO4_Amount =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertilization_NH42SO4_Amount"));
            PyObject *db_NH42HPO4_Amount =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertilization_NH42HPO4_Amount"));
            PyObject *db_Fert_Depth =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertilization_Depth"));
            PyObject *db_Fert_Release_rate =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertilization_Release_rate"));
            PyObject *db_Fert_Inhibitor_efficiency =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertilization_Inhibitor_efficiency"));
            PyObject *db_Fert_Inhibitor_duration =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertilization_Inhibitor_duration"));
            PyObject *db_Fert_Urease_efficiency =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertilization_Urease_efficiency"));
            PyObject *db_Fert_Urease_duration =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Fertilization_Urease_duration"));

            fprintf(fp,"%d\n", fertn);

            for (int k = 0; k<fertn; k++)
            {
                int FertDay = atoi(PyString_AsString(PyList_GetItem(db_Fert_Day,k)));
                int FertID = atoi(PyString_AsString(PyList_GetItem(db_Fert_ID,k)));
                int FertMethod = atoi(PyString_AsString(PyList_GetItem(db_Fert_Method,k)));

                int FertType = atoi(PyString_AsString(PyList_GetItem(db_Fert_Type,k)));

                float FertDepth = atof(PyString_AsString(PyList_GetItem(db_Fert_Depth,k)));
                float FertNitrate = atof(PyString_AsString(PyList_GetItem(db_Nitrate_Amount,k)));
                float FertAbi = atof(PyString_AsString(PyList_GetItem(db_Abi_Amount,k)));
                float FertUrea = atof(PyString_AsString(PyList_GetItem(db_Urea_Amount,k)));
                float FertAnh = atof(PyString_AsString(PyList_GetItem(db_Anh_Amount,k)));
                float FertAmNi = atof(PyString_AsString(PyList_GetItem(db_AmNi_Amount,k)));
                float NH42SO4 = atof(PyString_AsString(PyList_GetItem(db_NH42SO4_Amount,k)));
                float NH42HPO4 = atof(PyString_AsString(PyList_GetItem(db_NH42HPO4_Amount,k)));
                float DayRelease = atof(PyString_AsString(PyList_GetItem(db_Fert_Release_rate,k)));
                float NIefficiency = atof(PyString_AsString(PyList_GetItem(db_Fert_Inhibitor_efficiency,k)));
                float NIduration = atof(PyString_AsString(PyList_GetItem(db_Fert_Inhibitor_duration,k)));
                float UreaseEfficiency = atof(PyString_AsString(PyList_GetItem(db_Fert_Urease_efficiency,k)));
                float UreaseDuration = atof(PyString_AsString(PyList_GetItem(db_Fert_Urease_duration,k)));
                FertOption = atoi(PyString_AsString(PyList_GetItem(db_Fert_Option,k)));



                fprintf(fp,"%d %d %f %f %f %f %f %f %f %f\n",
                        FertDay,
                        FertMethod,
                        FertDepth*0.01,
                        FertNitrate,
                        FertAbi,
                        FertUrea,
                        FertAnh,
                        FertAmNi,
                        NH42SO4,
                        NH42HPO4);
                fprintf(fp,"%f %f %f %f %f\n", DayRelease, NIefficiency, NIduration,
                        UreaseEfficiency, UreaseDuration);

            }
            ///TODO: About this fertOption, need to modify the whole the input config file
            fprintf(fp,"%d\n", FertOption);
            fclose (fp);

            Py_XDECREF(db_Fert_ID);
            Py_XDECREF(db_Fertigation);
            Py_XDECREF(db_Fert_Day);
            Py_XDECREF(db_Fert_Type);
            Py_XDECREF(db_Fert_Method);
            Py_XDECREF(db_Fert_Option);
            Py_XDECREF(db_Nitrate_Amount);
            Py_XDECREF(db_Abi_Amount);
            Py_XDECREF(db_Urea_Amount);
            Py_XDECREF(db_Anh_Amount);
            Py_XDECREF(db_AmNi_Amount);
            Py_XDECREF(db_NH42SO4_Amount);
            Py_XDECREF(db_NH42HPO4_Amount);
            Py_XDECREF(db_Fert_Depth);
            Py_XDECREF(db_Fert_Release_rate);
            Py_XDECREF(db_Fert_Inhibitor_efficiency);
            Py_XDECREF(db_Fert_Inhibitor_duration);
            Py_XDECREF(db_Fert_Urease_efficiency);
            Py_XDECREF(db_Fert_Urease_duration);

            /// Manure
            sprintf(DB,"%s/Inputs/manu-%d",out_path,yr);
            fp=fopen(DB, "w");
            if(fp==NULL)
            {
                cout << "Can not create file " << DB << endl;
                exit(0);
            }

            int manun = atoi(PyString_AsString(PyObject_GetAttrString(pManage_List_Item,"Manure_Number")));

            PyObject *db_Manure_ID =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Manure_ID"));
            PyObject *db_Manure_Day =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Manure_Day"));
            PyObject *db_Manure_Amount =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Manure_Amount"));
            PyObject *db_Manure_C_N_Ratio =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Manure_C_N_Ratio"));
            PyObject *db_Manure_Type =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Manure_Type"));
            PyObject *db_Manure_Method =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Manure_Method"));

            fprintf(fp, "%d\n",manun);
            for (k=0; k<manun; k++)
            {

                int ManuDay = atoi(PyString_AsString(PyList_GetItem(db_Manure_Day,k)));
                float ManuAmount = atof(PyString_AsString(PyList_GetItem(db_Manure_Amount,k)));
                float ManuCN = atof(PyString_AsString(PyList_GetItem(db_Manure_C_N_Ratio,k)));
                int ManuType = atoi(PyString_AsString(PyList_GetItem(db_Manure_Type,k)));
                int ManuMethod = atoi(PyString_AsString(PyList_GetItem(db_Manure_Method,k)));

                fprintf(fp,"%d %f %f %d %d\n",
                        ManuDay,
                        ManuAmount,
                        ManuCN,
                        ManuType,
                        ManuMethod);

            }
            fclose(fp);


            Py_XDECREF(db_Manure_ID);
            Py_XDECREF(db_Manure_Day);
            Py_XDECREF(db_Manure_Amount);
            Py_XDECREF(db_Manure_C_N_Ratio);
            Py_XDECREF(db_Manure_Type);
            Py_XDECREF(db_Manure_Method);


            /// Plastic Filming


            sprintf(DB,"%s/Inputs/plastic-%d",out_path,yr);
            fp=fopen(DB, "w");
            if(fp==NULL)
            {
                cout << "Can not create file " << DB << endl;
                exit(0);
            }

            int plsticnum = atoi(PyString_AsString(PyObject_GetAttrString(pManage_List_Item,"Plasticultural_Number")));

            PyObject *db_Plastic_ID =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Plastic_ID"));
            PyObject *db_Plastic_StartDay =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Plastic_StartDay"));
            PyObject *db_Plastic_EndDay =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Plastic_EndDay"));
            PyObject *db_Plastic_Ventilation =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Plastic_Ventilation"));
            PyObject *db_Plastic_Type =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Plastic_Type"));
            PyObject *db_Plastic_Method =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Plastic_Method"));


            fprintf(fp, "%d\n",plsticnum);
            for (k=0; k<plsticnum; k++)
            {

                int StartDay = atoi(PyString_AsString(PyList_GetItem(db_Plastic_StartDay,k)));
                int EndDay = atoi(PyString_AsString(PyList_GetItem(db_Plastic_EndDay,k)));
                float Plastic_Ventilation = atof(PyString_AsString(PyList_GetItem(db_Plastic_Ventilation,k)));
                int PlasticType = atoi(PyString_AsString(PyList_GetItem(db_Plastic_Type,k)));
                int PlasticMethod = atoi(PyString_AsString(PyList_GetItem(db_Plastic_Method,k)));

                fprintf(fp,"%d %d %d %d %d\n",StartDay, EndDay,Plastic_Ventilation,PlasticType,PlasticMethod);

            }
            fclose(db);

            Py_XDECREF(db_Plastic_ID);
            Py_XDECREF(db_Plastic_StartDay);
            Py_XDECREF(db_Plastic_EndDay);
            Py_XDECREF(db_Plastic_Ventilation);
            Py_XDECREF(db_Plastic_Type);
            Py_XDECREF(db_Plastic_Method);



            /// Flooding


            sprintf(DB,"%s/Inputs/flood-%d",out_path,yr);
            fp=fopen(DB, "w");
            if(fp==NULL)
            {
                cout << "Can not create file " << DB << endl;
                exit(0);
            }



            int floodn = atoi(PyString_AsString(PyObject_GetAttrString(pManage_List_Item,"Flooding_Number")));

            PyObject *db_Flooding_ID =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Flooding_ID"));
            PyObject *db_Leak_type =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Leak_type"));
            PyObject *db_Water_control =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Water_control"));
            PyObject *db_Flood_Day =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Flood_Day"));
            PyObject *db_Drain_Day =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Drain_Day"));
            PyObject *db_Leak_rate =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Leak_rate"));
            PyObject *db_WT_file =PyObject_GetAttr(pManage_List_Item,PyString_FromString("WT_file"));
            PyObject *db_Water_N =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Water_N"));
            PyObject *db_Shallow_flood =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Shallow_flood"));
            PyObject *db_Water_gather =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Water_gather"));
            PyObject *db_FEmpirical_parameters =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Empirical_parameters"));
            PyObject *db_Initial_WT =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Initial_WT"));
            PyObject *db_WatershedIndex =PyObject_GetAttr(pManage_List_Item,PyString_FromString("inflow_PrecFrac"));
            PyObject *db_LWTceasingGroungFlow =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Lowest_ground_outflow"));
            PyObject *db_LWTceasingSurfFlow =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Lowest_surface_outflow"));
            PyObject *db_SurfOutflowIntensity =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Intensity_surface_outflow"));
            PyObject *db_GroundOutflowIntensity =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Intensity_ground_outflow"));


            /// add some new features in this flooding option. Here have some difference with the original flooding file.
            /// in here, all options are involved into the loop
            /// Currently, Flooding only achive same results with the original one, when flooding equal one.



            fprintf(fp,"%d\n",floodn);
            for (k = 0; k<floodn; k++)
            {

                char *WT_file = NULL;
                int Flood_Day = atoi(PyString_AsString(PyList_GetItem(db_Flood_Day,k)));
                int Drain_Day = atoi(PyString_AsString(PyList_GetItem(db_Drain_Day,k)));
                float FlooN = atof(PyString_AsString(PyList_GetItem(db_Water_N,k)));
                int ShallowFlood = atoi(PyString_AsString(PyList_GetItem(db_Shallow_flood,k)));
                int FdLeak = atoi(PyString_AsString(PyList_GetItem(db_Leak_type,k)));
                int WaterControl = atoi(PyString_AsString(PyList_GetItem(db_Water_control,k)));
                float WaterLeakRate = atof(PyString_AsString(PyList_GetItem(db_Leak_rate,k)));
                float WaterGetherIndex = atof(PyString_AsString(PyList_GetItem(db_Water_gather,k)));
                WT_file = PyString_AsString(PyList_GetItem(db_WT_file,k));
                float m_IniWT = atof(PyString_AsString(PyList_GetItem(db_Initial_WT,k)));
                float m_LWTceasingSurfFlow = atof(PyString_AsString(PyList_GetItem(db_LWTceasingSurfFlow,k)));
                float m_LWTceasingGroungFlow = atof(PyString_AsString(PyList_GetItem(db_LWTceasingGroungFlow,k)));
                float m_WatershedIndex = atof(PyString_AsString(PyList_GetItem(db_WatershedIndex,k)));
                float m_SurfOutflowIntensity = atof(PyString_AsString(PyList_GetItem(db_SurfOutflowIntensity,k)));
                float m_GroundOutflowIntensity = atof(PyString_AsString(PyList_GetItem(db_GroundOutflowIntensity,k)));

                fprintf(fp, "%d ",Flood_Day);
                fprintf(fp, "%d\n",Drain_Day);
                fprintf(fp, "%f\n", FlooN);
                fprintf(fp, "%d\n", ShallowFlood);
                fprintf(fp, "%d %d %f\n",FdLeak, WaterControl, WaterLeakRate);
                fprintf(fp, "%f\n",WaterGetherIndex);
                fprintf(fp, "%s\n",WT_file);
                fprintf(fp, "%f %f %f %f %f %f\n",m_IniWT, m_LWTceasingSurfFlow, m_LWTceasingGroungFlow, m_WatershedIndex, m_SurfOutflowIntensity, m_GroundOutflowIntensity);

            }

            fclose (fp);

            Py_XDECREF(db_Flooding_ID);
            Py_XDECREF(db_Leak_type);
            Py_XDECREF(db_Water_control);
            Py_XDECREF(db_Flood_Day);
            Py_XDECREF(db_Drain_Day);
            Py_XDECREF(db_Leak_rate);
            Py_XDECREF(db_WT_file);
            Py_XDECREF(db_Water_N);
            Py_XDECREF(db_Shallow_flood);
            Py_XDECREF(db_Water_gather);
            Py_XDECREF(db_FEmpirical_parameters);
            Py_XDECREF(db_Initial_WT);
            Py_XDECREF(db_WatershedIndex);
            Py_XDECREF(db_LWTceasingGroungFlow);
            Py_XDECREF(db_LWTceasingSurfFlow);
            Py_XDECREF(db_SurfOutflowIntensity);
            Py_XDECREF(db_GroundOutflowIntensity);


            /// Irrigation
//

            sprintf(DB,"%s/Inputs/irri-%d",out_path,yr);
            db=fopen(DB, "w");
            if(db==NULL)
            {
                cout << "Can not create file " << DB << endl;
                exit(0);
            }


            int irrin=  atoi(PyString_AsString(PyObject_GetAttrString(pManage_List_Item,"Irrigation_Number")));
            int IrrIndex = atoi(PyString_AsString(PyObject_GetAttrString(pManage_List_Item,"Irrigation_Index")));

            PyObject *db_Irrigation_ID =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Irrigation_ID"));
            PyObject *db_Irrigation_Day =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Irrigation_Day"));
            PyObject *db_Irr_Water_amount =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Irrigation_Water_amount"));
            PyObject *db_Irrigation_Method =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Irrigation_Method"));

            fprintf(fp,"%d %f\n", irrin, IrrIndex);

            for (k=0; k<irrin; k++)
            {

                int Irr_Day = atoi(PyString_AsString(PyList_GetItem(db_Irrigation_Day,k)));
                float IrriAmount = atof(PyString_AsString(PyList_GetItem(db_Irr_Water_amount,k)));
                int IrriMethod = atoi(PyString_AsString(PyList_GetItem(db_Irrigation_Method,k)));


                fprintf(fp,"%d %f %d\n", Irr_Day, IrriAmount, IrriMethod);

            }
            fclose(fp);

            Py_XDECREF(db_Irrigation_ID);
            Py_XDECREF(db_Irrigation_Day);
            Py_XDECREF(db_Irr_Water_amount);
            Py_XDECREF(db_Irrigation_Method);


            ///Grazing

            sprintf(DB,"%s/Inputs/graz-%d",out_path,yr);
            fp=fopen(DB, "w");
            if(fp==NULL)
            {
                cout << "Can not create file " << DB << endl;
                exit(0);
            }

            int grazn=  atoi(PyString_AsString(PyObject_GetAttrString(pManage_List_Item,"Grazing_Number")));


            PyObject *db_Grazing_ID =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Grazing_ID"));
            PyObject *db_Graz_Start_Day =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Graz_Start_Day"));
            PyObject *db_Graz_End_Day =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Graz_End_Day"));
            PyObject *db_Graz_Hours_Per_day =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Graz_Hours_Per_day"));
            PyObject *db_Beef_Head_per_ha =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Beef_Head_per_ha"));
            PyObject *db_Sheep_Head_per_ha =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Sheep_Head_per_ha"));
            PyObject *db_Horse_Head_per_ha =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Horse_Head_per_ha"));
            PyObject *db_Pig_Head_per_ha =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Pig_Head_per_ha"));
            PyObject *db_Dairy_Head_per_ha =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Dairy_Head_per_ha"));


            fprintf(fp, "%d\n", grazn);

            for (k=0; k<grazn; k++)
            {

                int GrazStartDay = atoi(PyString_AsString(PyList_GetItem(db_Graz_Start_Day,k)));
                int GrazEndDay = atoi(PyString_AsString(PyList_GetItem(db_Graz_End_Day,k)));
                float Cattle = atof(PyString_AsString(PyList_GetItem(db_Beef_Head_per_ha,k)));
                float Horse = atof(PyString_AsString(PyList_GetItem(db_Horse_Head_per_ha,k)));
                float Sheep = atof(PyString_AsString(PyList_GetItem(db_Sheep_Head_per_ha,k)));
                float GrazHour = atof(PyString_AsString(PyList_GetItem(db_Graz_Hours_Per_day,k)));

                fprintf(fp, "%d %d %f %f %f %f\n",GrazStartDay,GrazEndDay,Cattle,
                        Horse,Sheep,GrazHour);

            }
            fclose(fp);

            Py_XDECREF(db_Grazing_ID);
            Py_XDECREF(db_Graz_Start_Day);
            Py_XDECREF(db_Graz_End_Day);
            Py_XDECREF(db_Graz_Hours_Per_day);
            Py_XDECREF(db_Beef_Head_per_ha);
            Py_XDECREF(db_Sheep_Head_per_ha);
            Py_XDECREF(db_Horse_Head_per_ha);
            Py_XDECREF(db_Pig_Head_per_ha);
            Py_XDECREF(db_Dairy_Head_per_ha);


            /// Grass cutting


            sprintf(DB,"%s/Inputs/cut-%d",out_path,yr);
            fp=fopen(DB, "w");
            if(fp==NULL)
            {
                cout << "Can not create file " << DB << endl;
                exit(0);
            }



            int cutn = atoi(PyString_AsString(PyObject_GetAttrString(pManage_List_Item,"Cutting_Number")));

            PyObject *db_Cut_ID =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Cut_ID"));
            PyObject *db_Cut_Day =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Cut_Day"));
            PyObject *db_Cut_Fraction =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Cut_Fraction"));
            PyObject *db_Cut_Part =PyObject_GetAttr(pManage_List_Item,PyString_FromString("Cut_Part"));


            fprintf(fp, "%d\n", cutn);
            for (k=0; k<cutn; k++)
            {
                int CutDay = atoi(PyString_AsString(PyList_GetItem(db_Cut_Day,k)));
                float CutAmount = atof(PyString_AsString(PyList_GetItem(db_Cut_Fraction,k)));
                int CutPart = atoi(PyString_AsString(PyList_GetItem(db_Cut_Part,k)));

                fprintf(fp, "%d %f\n",CutDay, CutAmount);
                fprintf(fp, "%d\n", CutPart);
            }
            fclose(fp);

            Py_XDECREF(db_Cut_ID);
            Py_XDECREF(db_Cut_Day);
            Py_XDECREF(db_Cut_Fraction);
            Py_XDECREF(db_Cut_Part);

        }



    }



    Py_XDECREF(pManagement);
//    PyRun_SimpleString("import gc");
//    PyRun_SimpleString("print len(gc.get_objects())");
    pManage_List_Item = NULL;
    Py_XDECREF(pManage_List_Item);
    delete pManage_List_Item;
    inxml = NULL;
    Py_XDECREF(inxml);
    delete inxml;
//    PyRun_SimpleString("gc.collect()");

    Py_Finalize();

    if (yr!=IData->years)
    {
        cout << "Error: !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
        cout << "Error: Managements not equal to total simulation years,toatal simulation years was set to Management years " << endl;
        IData->years = yr;
    }






////////////////////////////////////////////// GetAMT //////////////////////////////////////


/////////////////////////////////////////////  END    //////////////////////////////////////

/// Initial the simulation data


////////////////////////////////////////////// SaveSite /////////////////////////////////////

    FILE *fpo;
    char fout[255];
    cout << "debug" << endl;
    cout << fname << endl;

    sprintf(fout,"%s/Inputs/site",out_path);
    fpo=sopen(fout,"w");
    if (fpo==NULL) return;

    if (strlen(IData->sitename)==0)
    {
        cout << "No Site name information!" << "Input Error:" << endl;
        return ;
    }
    fprintf(fpo,"%s\n",IData->sitename);

    if (IData->years<=0)
    {
        cout << "No Simulated year information!" << "Input Error:" << endl;
        fclose(fpo);
        return ;
    }
    /*if (&IData->latitude<=0) {
    	MessageBox(NULL,"No latitude information!","Input Error:",0);
    	fclose(fpo);
    	return -1;
    }*/

    fprintf(fpo,"%d %d %f %ld %f\n", IData->years, IData->ifdaily,
            IData->latitude, IData->totalday, IData->longitude);//, &IData->if1File);
    fprintf(fpo,"%d\n", IData->Soil_landuse);//&IData->CropModel);
    fclose (fpo);

    sprintf(fout, "%s/Inputs/SPAN",out_path);
    fpo=sopen(fout,0);
    if (fpo==NULL) return ;
    fprintf(fpo, "%d\n", 1);//Span
    fclose(fpo);
//////////////////////////////////////// END   /////////////////////////////////////////////////////


////////////////////////////////////////  RecordClimateFile  ///////////////////////////////////////

    int yn, result=-1, TotalYear, FirstYear;

    char foutname[255];
    char Climate_File[255];//cn[250],
    float MonAveT[15], YrAveT,YrRain;

    string inclimatefile;

    int YieldSite, YieldYear, YieldToday, YieldCrop,YieldMonth, YieldDay;
    int DEW = 0;
    int UseID=0;
    int IfCheck=0;
    int DryYear, MidYear, WetYear;
    char DDname[3027], DYname[3027];
    int currentyear;
    char Pname[3072],ClimateName[3027];

    for(int i = 0; i<IData->years; i++)
    {
        currentyear = i+ IData->startyear;

        if(IData->if1File==0)
        {
//            cout << "climate path is:  " << IData->ClimatePath << endl;
            inclimatefile = IData->ClimatePath + "\\" + int2str(currentyear) + "\\"+ IData->ClimateFileName +"_"+  int2str(currentyear) + ".txt";
//            cout << "climate path is:  " << inclimatefile << endl;
        }
        else
        {
            inclimatefile = IData->ClimatePath + "\\" + int2str(IData->startyear) + "\\"+ IData->ClimateFileName +"_"+ int2str(IData->startyear) + ".txt"; // use same cliamte file for all years
        }


        strcpy(Pname,inclimatefile.c_str());
        strcpy(ClimateName,IData->ClimateFileName.c_str());


//        sprintf(DDname,"%d_%d-%d.day",ClimateName,currentyear , i+1);
//        sprintf(DYname,"%d_%d-%d.yr",ClimateName,currentyear, i+1);

        string tmpstr = "";
        tmpstr = IData->ClimateFileName+"_"+int2str(currentyear)+".txt-"+int2str(i+1)+".day";
        //char *DDname = (char*)tmpstr.c_str();
        strcpy(DDname,tmpstr.c_str());
        tmpstr = IData->ClimateFileName+"_"+int2str(currentyear)+".txt-"+int2str(i+1)+".yr";
        //char *DYname = (char*)tmpstr.c_str();
        strcpy(DYname,tmpstr.c_str());
        sprintf(fname,"%s/Inputs/clim-%d",out_path,i+1);
        if ((fpo=sopen(fname,"w"))==NULL) error_print(1, fname);

        fprintf(fpo,"%s\n",DDname);
        fprintf(fpo,"%s\n",DYname);
        fclose(fpo);

//        FILE *fpiid =fopen(inclimatefile.c_str(),"r");
//
////        cout << "climate path is:  " << inclimatefile << endl;
//
//        if (fpiid==NULL)
//        {
//            cout << "clim: Can not find file " << inclimatefile << endl;
//            cout << "Try old path " << endl;
//        }
//        fclose(fpiid);



//        SaveClimateFileOfAYear(i+1,inclimatefile,MonAveT,&YrAveT,&YrRain,IData->latitude,
//                               IData->ClimateFileType,IData->NO3NH4_Rainfall,
//                               IData->NH3_Atmosphere,IData->BaseCO2, out_path ,mode, IData->WaterYrMonth,
//                               IData->WaterYrDay, IData->CO2IncreaseRate,IData,0,0);


        string outpath = string(OUTPUT);
        MakeDailyClimateFile(Pname,DDname, DYname,outpath,UseID,IData->ClimateFileType,
                             IData->latitude,IfCheck,IData->NO3NH4_Rainfall,
                             IData->NH3_Atmosphere,IData->BaseCO2,IData->CO2IncreaseRate,
                             YieldSite, YieldYear, YieldToday,ClimateName,DEW,
                             DryYear, MidYear, WetYear);

    }

///////////////////////////////////////////////// END ///////////////////////////////////////////

///////////////////////////////////////////////  ReadSoilFeature  //////////////////////////////

    FILE *fpi;
    char f_in_name[255];

    float DClay, DFldcap, DWiltpt, DSks, DPorosity;


    int ave_ST = 0;
    int tmpxxx = 0;

    Soil_Texture_Lib *soillib = new Soil_Texture_Lib();
    soil_feature mysoil;


    if (tmpxxx >= .2) ave_ST = 12;
    else if (IData->Soil_Clay <= 0.03) mysoil = soillib->GetData(1);
    else if (IData->Soil_Clay <= 0.06 && IData->Soil_Clay > 0.03 ) mysoil = soillib->GetData(2);
    else if (IData->Soil_Clay <= 0.09 && IData->Soil_Clay > 0.06 ) mysoil = soillib->GetData(3);
    else if (IData->Soil_Clay <= 0.14 && IData->Soil_Clay > 0.09 ) mysoil = soillib->GetData(4);
    else if (IData->Soil_Clay <= 0.19 && IData->Soil_Clay > 0.14 ) mysoil = soillib->GetData(5);
    else if (IData->Soil_Clay <= 0.27 && IData->Soil_Clay > 0.19 ) mysoil = soillib->GetData(6);
    else if (IData->Soil_Clay <= 0.34 && IData->Soil_Clay > 0.27 ) mysoil = soillib->GetData(7);
    else if (IData->Soil_Clay <= 0.41 && IData->Soil_Clay > 0.34 ) mysoil = soillib->GetData(8);
    else if (IData->Soil_Clay <= 0.43 && IData->Soil_Clay > 0.41 ) mysoil = soillib->GetData(9);
    else if (IData->Soil_Clay <= 0.49 && IData->Soil_Clay > 0.43 ) mysoil = soillib->GetData(10);
    else mysoil = soillib->GetData(11);//if (ave_clay <= .63)


    delete soillib;



    if (IData->Soil_Texture<1 && IData->Soil_Texture>13) return ;

//    sprintf(f_in_name, "%s/Library/Lib_soil/soil_%d", root_path, IData->Soil_Texture);

//sprintf(note,"%d",&IData->Soil_Texture);
//strcat(fname,note);
//    if ((fpi=sopen(f_in_name, "r")) == NULL)
//    {
//        cout << "You may have the DNDC model installed in an incorrect place. Please move the entire DNDC folder to the root directory of any drive in your computer!" << endl;
//        cout << " Can not find Library Folder" << endl;
//        exit(0);
//    }


    strcpy(IData->SoilName,mysoil.Texture_Name.c_str());
    DClay = mysoil.clay_portion;
    DPorosity = mysoil.porosity;
    DSks = mysoil.satu_conductivity;
    DFldcap = mysoil.field_capasity;
    DWiltpt = mysoil.wilting_point;



    DSks *= 0.6;//cm/min ->m/hr

    if(IData->Soil_Clay==0.0) IData->Soil_Clay = DClay;
    if(IData->Fldcap==0.0) IData->Fldcap = DFldcap;
    if(IData->Wiltpt==0.0) IData->Wiltpt = DWiltpt;
    if(IData->Sks==0.0) IData->Sks = DSks;
    if(IData->Soil_Porosity==0.0) IData->Soil_Porosity = DPorosity;
    if(IData->RCNB==0.0) IData->RCNB = 10.0;
    if(IData->RCNH==0.0) IData->RCNH = 10.0;
    if(IData->RCNM==0.0) IData->RCNM = 10.0;
    if(IData->RCNRVL==0.0) IData->RCNRVL = 5.;
    if(IData->RCNRL==0.0) IData->RCNRL = 25.;
    if(IData->RCNRR==0.0) IData->RCNRR = 100.;

    // Define the initial soil wpfs as the average of Fldcap and Wiltpt
    if(IData->Soil_moiture==0.0) IData->Soil_moiture = (IData->Fldcap + IData->Wiltpt) * 0.5;
//////////////////////////////////////////////// END ///////////////////////////////////////////


//////////////////////////////////////////////// SetSoilH /////////////////////////////////////

    IData->AcWater   = 0;
    IData->SurfWater = 0;
    IData->SnowPack  = 0;

//&IData->Idc = 1# + IIf(soilbypass(0).Value = True, 0, 1) 'Soil_BypassFlow
    IData->Idc = 1 + (int)IData->Soil_BypassFlow;


    IData->Qstar = (float)(6.45 * (IData->Soil_Clay - 0.01) + 1);
    IData->BypassF = IData->Soil_BypassFlow;//(float)0.1 * (&IData->Idc - 1);


    float dt;
    dt = (2 * 0.005 / IData->Soil_Porosity);
    if (dt < IData->Sks)
    {
        IData->H = dt;
    }
    else
    {
        IData->H = IData->Sks;
    }
    if (IData->H <= 0)
    {
        cout << "There is a parameter error of Soil_H=0 ! Please check system file and your data. " "Input Error :" << endl;
        return ;
    }

    int total_layer=MAXSOILLAYER-10;
    IData->TH = SOILDEPTH;//0.9;//Total depth of simulated soil profile, m
    if (IData->H*total_layer < IData->TH )
        IData->H = IData->TH / total_layer;//total layer number is <=50
    IData->Q = (int)(IData->TH / IData->H);

    if (IData->Q > total_layer)
    {
        IData->Q = total_layer;
        IData->H = IData->TH / total_layer;
    }


    IData->CEC= (float)(IData->Soil_Clay * 46.0);
    IData->m  = (float)(IData->Soil_Density * 1000. * IData->H * 10000.0);
    IData->V  = (float)(IData->H * 10000. * IData->Soil_Porosity);	// m^3/layer/ha

    float PS		= IData->H * IData->Soil_Porosity;	// porosity  (m/layer)
    //ePS = PS * 0.8;
    float FLDCAPw = PS * IData->Fldcap;	// m water/layer IData.FieldCapacity
    float Wiltptw = PS * IData->Wiltpt;	// m water/layer
    float TravelTime = 20.0 * (PS - FLDCAPw) / IData->Sks;	// hr/layer
    // fraction of soil water available for plants, can be tuned


    int n;
    n = IData->Q;


    float ClayF1 = pow(10.0,-IData->Soil_Clay / 0.75 - 0.2);
    float ClayF2 = IData->Soil_Clay * 6.;
    float s_IniNH4[MAXSOILLAYER];





    IData->Tranr[1] = IData->Soil_SOCatSurface * IData->Soil_Litter * IData->m;
    IData->OCini[1] = IData->Soil_SOCatSurface * IData->Soil_humads * IData->m;
    IData->Dphum[1] = IData->Soil_SOCatSurface * IData->Soil_humus * IData->m;



    IData->s_NO3[1] = IData->Soil_NO3 * IData->m / 1000000.;
    s_IniNH4[1] = IData->Soil_NH4 * IData->m / 1000000.;

    IData->RCVL[1] = 0.0 * IData->Tranr[1];
    IData->RCL[1]  = 0.0 * IData->Tranr[1];
    IData->RCR[1]  = 1.0 * IData->Tranr[1];


    float Dbm = IData->Soil_Density;


    IData->s_NH4[1]   = s_IniNH4[1] * 0.7;
    IData->ClayNH4[1] = s_IniNH4[1] * 0.3;

    float F_soc = IData->Soil_SOCatSurface/0.058;
    if(F_soc>1.0) F_soc = 1.0;

    //ClayNH4[1] = F_soc * IData.CEC * 0.4;
    //ClayNH4[1] = IData.CEC * 0.4; //NewChange


    float stub1  = 0.0;
    float stub2  = 0.0;
    float stub3  = 0.0;
    //WeedN  = 0.0;


    float TCN=0.0;

//
    char outname[255];
    FILE *fpsoil;
    sprintf(outname,"%s/Inputs/soil-%d",out_path,(int)fabs(1));
    fpsoil=sopen(outname,"w");
    if (fpsoil==NULL) return;

    IData->IniFrostNO=0;
    IData->IniFrostN2O=0;
    IData->IniFrostN2=0;

    float ice=0,tt[120];
    int Retard=1;
    if (IData->SoilRetardation==1) Retard=3;





    char climf[3072],ClimFileName[3072], ClimYrName[3072];
    FILE *fClim;
    int jday;
    sprintf(climf, "%s/Inputs/clim-1", out_path);
    fClim = fopen(climf, "r");
    if(fClim==NULL) error_print(0,climf);
    else
    {
        fscanf(fClim, "%s", ClimFileName);
        fscanf(fClim, "%s", ClimYrName);
    }
    fclose(fClim);

    sprintf(climf, "%s/Inputs/%s", out_path, ClimFileName);
    fClim = fopen(climf, "r");
    if(fClim==NULL) error_print(0, climf);
    else
    {
        fscanf(fClim, "%d  %f", &jday, &IData->Soil_T);
    }
    fclose(fClim);

    sprintf(climf, "%s/Inputs/%s", out_path, ClimYrName);
    fClim = fopen(climf, "r");
    if(fClim==NULL) error_print(0,climf);
    else
    {
        for(i=1; i<=13; i++)
            fscanf(fClim, "%f", &IData->AnnualMeanT);
    }
    fclose(fClim);




    for (i=1; i<=IData->Q; i++)
    {
        tt[i] = IData->AnnualMeanT + (IData->Soil_T - IData->AnnualMeanT)/ (float)pow(i*IData->H*100, 0.5);
        if (IData->Soil_T<0.0 && tt[i] < IData->Soil_T) tt[i] = IData->Soil_T;

        /*tt[L] =  yr_avet + (s_temp - yr_avet)
        	/ (float)pow(L*H*100, 0.5);
        if (s_temp<0.0 && tt[L] < s_temp) tt[L] = s_temp;*/
    }



    fprintf(fpsoil,"%3d\n", IData->Soil_Texture);
    fprintf(fpsoil,"%s\n\n", IData->SoilName);

    fprintf(fpsoil,"%.4f  %.4f  %.4f  %.4f  %.4f  %.4f\n",
            IData->Soil_Clay, IData->Soil_Porosity, IData->Sks, IData->Fldcap,
            IData->Wiltpt, IData->LateralInflux);
    fprintf(fpsoil,"%f %f %f %f %d  %f\n",
            IData->Psisat, IData->Beta, Dbm, IData->Soil_pH,
            IData->Idc, IData->Soil_SOCatSurface);
    fprintf(fpsoil,"%.4f  %.4f  %.4f  %.4f  %.4f  %.4f\n",
            IData->Soil_NO3, IData->Soil_NH4, IData->Soil_Litter,
            IData->Soil_humads, IData->Soil_humus, 0.0);//Byp
    fprintf(fpsoil,"%.4f  %.4f  %.4f  %.4f  %.4f  %d\n",
            IData->H, IData->CEC, IData->TH, IData->m, IData->V, IData->Q);
    fprintf(fpsoil,"%.4f  %.4f\n", ClayF1, ClayF2);
    fprintf(fpsoil,"%.4f  %.4f  %d  %.4f  %.4f  %.4f  %.4f\n",
            IData->Qstar, IData->BypassF, Retard, IData->RCNB, IData->RCNH, IData->RCNM, IData->RCNRVL);
    fprintf(fpsoil,"%.4f  %.4f %.4f  %.4f  %.4f  %.4f\n",
            IData->RCNRL, IData->RCNRR, IData->AcWater, IData->SurfWater,
            IData->SnowPack, stub1);
    fprintf(fpsoil,"%.4f  %.4f\n", stub2, stub3);
    fprintf(fpsoil, "%.4f  %.4f  %.4f\n\n", IData->IniFrostNO, IData->IniFrostN2O, IData->IniFrostN2);

    fprintf(fpsoil, "%f\n", IData->Soil_MicrobioIndex);
    fprintf(fpsoil, "%f\n", IData->Soil_WaterObstructLayer);//m
    fprintf(fpsoil, "%f %f\n", IData->slope, IData->Soil_Quality);
    fprintf(fpsoil, "%f\n", IData->Fldcap);
    fprintf(fpsoil, "%f\n", IData->Wiltpt);
    fprintf(fpsoil, "%f %f\n", IData->SOCa, IData->SOCb);
    fprintf(fpsoil, "%f %f %f %f %f %f %f %f %f\n", IData->RCVL[1], IData->RCL[1], IData->RCR[1], IData->OCini[1], IData->Dphum[1],
            IData->s_NO2[1], IData->s_NO3[1], IData->s_NH4[1], IData->ClayNH4[1]);
    fprintf(fpsoil, "%f\n", tt[1]);
    fprintf(fpsoil, "%f\n", IData->HighestWT);
    fprintf(fpsoil, "%d\n", IData->ProfileType);
    fprintf(fpsoil, "%f\n", IData->PassiveC);
    fprintf(fpsoil, "%f\n", IData->DClitter);
    fprintf(fpsoil, "%f\n", IData->DChumads);
    fprintf(fpsoil, "%f\n", IData->DChumus);
    fprintf(fpsoil, "%f\n", IData->HumadCN);
    fprintf(fpsoil, "%f\n", IData->HumusCN);

    fclose (fpsoil);

//////////////////////////////////////////////// END ///////////////////////////////////////////

    cout << "DND XML Loaded successuly" << endl;
}




int SaveClimateFileOfAYear(int id,string incliamtefile, float *MonAveT,float *YrAveT,
                           float *YrRain, float lat, int ClimateFileType, float NO3NH4_Rainfall,
                           float NH3_Atmosphere, float BaseCO2,char *out_path,int mode, int WaterYrMonth,
                           int WaterYrDay, float CO2IncreaseRate,Site_BaseInformation *IData ,int UseID,int DMW)
{


    /** \brief
     *
     * \param
     * \UseID is drought option
     * \DMW   is drought conditions:1 is drought, 2 is medium, 3 is wet
     * \ch    is full path od climate files
     * \return
     *
     */


    //float Day_Hrs,temp=0.0,Rainfall=0.0,maxT=0.0,minT=0.0,solrad,wind_speed=2.0;
    //float AveYrT, AveMonthT[13], TotalYrRain;
    //int   jday=0,jday0=0,tn,j,k, Aday;
    char fname[3072];
    char Pname[3072],cn[3072];//, cjunk[200], junkcc[300];
    //	CString SiteName[4]={"","Changtu","Chaoyang","Fuxin"};
    //	CString CropName[4]={"","corn","s_wheat",""};
    string IfCheck = "Check";
    char ch[3072];
    strcpy(ch,incliamtefile.c_str());
    FILE *fpo,*fpi;

    sprintf(Pname,"%s",ch);
    sprintf(cn,"%s",ch);

    GetShortFileName(cn);

    //////////////////////////
    char Dname[3072], Yname[3072], LocationName[3072];
    char Dname1[3072],Dname2[3072],Dname3[3072],Yname1[3072],Yname2[3072],Yname3[3072];
    int Site, YieldYear, YieldToday, YieldCrop,YieldMonth, YieldDay;
    if(UseID==10)//drought option
    {
        //////////////////////////////////////////////////////////////////////////
        char YF[3072];
        FILE* yf;
        sprintf(YF,"%s/Inter/YieldPrediction", out_path);
        yf=fopen(YF, "r");
        if(yf==NULL) note(0,YF);

        fscanf(yf, "%d %d %d %d %s %d %d", &Site, &YieldCrop, &YieldYear, &YieldToday, LocationName,&YieldMonth, &YieldDay);
        fclose(yf);

        //sprintf(LocationName, "%s", SiteName[Site]);
        ///////////////////////////////////////////////////////////////////////////

        if(id==1)
        {
            sprintf(fname,"%s/Inputs/clim-%d",out_path,id);
            if ((fpo=sopen(fname,0))==NULL) error_print(1, fname);

            sprintf(Dname,"%s-%d.day",LocationName, id);
            sprintf(Yname,"%s-%d.yr",LocationName, id);
            fprintf(fpo,"%s\n",Dname);
            fprintf(fpo,"%s\n",Yname);
            fclose(fpo);
        }
        else
        {
            sprintf(Pname,"%s",LocationName);

            sprintf(fname,"%s/Inputs/clim-%d",out_path,id);
            if ((fpo=sopen(fname,0))==NULL) error_print(1, fname);
            sprintf(Dname1,"%s-%d-dry.day",LocationName, id);
            sprintf(Dname2,"%s-%d-mid.day",LocationName, id);
            sprintf(Dname3,"%s-%d-wet.day",LocationName, id);
            sprintf(Yname1,"%s-%d-dry.yr",LocationName, id);
            sprintf(Yname2,"%s-%d-mid.yr",LocationName, id);
            sprintf(Yname3,"%s-%d-wet.yr",LocationName, id);
            fprintf(fpo,"%s %s %s\n",Dname1,Dname2,Dname3);
            fprintf(fpo,"%s %s %s\n",Yname1,Yname2,Yname3);
            fclose(fpo);
        }
    }
    else
    {
        sprintf(fname,"%s/Inputs/clim-%d",out_path,id);
        if ((fpo=sopen(fname,"w"))==NULL) note(1, fname);

        sprintf(Dname,"%s-%d.day",cn, id);
        sprintf(Yname,"%s-%d.yr",cn, id);
        fprintf(fpo,"%s\n",Dname);
        fprintf(fpo,"%s\n",Yname);
        fclose(fpo);
    }

    //Create daily climate files
    if(UseID==10)//drought option
    {
        if(id==1)
        {
            CreateDailyClimateFile(Pname, Dname, Yname, UseID, ClimateFileType, lat, IfCheck,
                                   NO3NH4_Rainfall,NH3_Atmosphere,BaseCO2, CO2IncreaseRate,
                                   LocationName, YieldYear, YieldToday, 0,IData,out_path);
        }
        else
        {
            CreateDailyClimateFile(Pname, Dname1, Yname1, UseID, ClimateFileType, lat, IfCheck,
                                   NO3NH4_Rainfall,NH3_Atmosphere,BaseCO2, CO2IncreaseRate,
                                   LocationName, YieldYear, YieldToday, 1,IData,out_path);
            CreateDailyClimateFile(Pname, Dname2, Yname2, UseID, ClimateFileType, lat, IfCheck,
                                   NO3NH4_Rainfall,NH3_Atmosphere,BaseCO2, CO2IncreaseRate,
                                   LocationName, YieldYear, YieldToday, 2,IData,out_path);
            CreateDailyClimateFile(Pname, Dname3, Yname3, UseID, ClimateFileType, lat, IfCheck,
                                   NO3NH4_Rainfall,NH3_Atmosphere,BaseCO2, CO2IncreaseRate,
                                   LocationName, YieldYear, YieldToday, 3,IData,out_path);
        }
    }
    else
    {
        CreateDailyClimateFile(Pname, Dname, Yname, UseID, ClimateFileType, lat, IfCheck,
                               NO3NH4_Rainfall,NH3_Atmosphere,BaseCO2, CO2IncreaseRate,
                               "None", 0, 0, 0,IData,out_path);
    }


    return -1;
}




int SaveClimateFileOfAYear(int id,char* ch, float *MonAveT,float *YrAveT,
                           float *YrRain, float lat, int ClimateFileType, float NO3NH4_Rainfall,
                           float NH3_Atmosphere, float BaseCO2,char *out_path,int mode, int WaterYrMonth,
                           int WaterYrDay, float CO2IncreaseRate,Site_BaseInformation *IData ,int UseID,int DMW)
{


    /** \brief
     *
     * \param
     * \UseID is drought option
     * \DMW   is drought conditions:1 is drought, 2 is medium, 3 is wet
     * \ch    is full path od climate files
     * \return
     *
     */


    //float Day_Hrs,temp=0.0,Rainfall=0.0,maxT=0.0,minT=0.0,solrad,wind_speed=2.0;
    //float AveYrT, AveMonthT[13], TotalYrRain;
    //int   jday=0,jday0=0,tn,j,k, Aday;
    char fname[3072];
    char Pname[3072],cn[3072];//, cjunk[200], junkcc[300];
    //	CString SiteName[4]={"","Changtu","Chaoyang","Fuxin"};
    //	CString CropName[4]={"","corn","s_wheat",""};
    string IfCheck = "Check";

    FILE *fpo,*fpi;

    sprintf(Pname,"%s",ch);
    sprintf(cn,"%s",ch);

    GetShortFileName(cn);

    //////////////////////////
    char Dname[3072], Yname[3072], LocationName[3072];
    char Dname1[3072],Dname2[3072],Dname3[3072],Yname1[3072],Yname2[3072],Yname3[3072];
    int Site, YieldYear, YieldToday, YieldCrop,YieldMonth, YieldDay;
    if(UseID==10)//drought option
    {
        //////////////////////////////////////////////////////////////////////////
        char YF[3072];
        FILE* yf;
        sprintf(YF,"%s/Inter/YieldPrediction", out_path);
        yf=fopen(YF, "r");
        if(yf==NULL) note(0,YF);

        fscanf(yf, "%d %d %d %d %s %d %d", &Site, &YieldCrop, &YieldYear, &YieldToday, LocationName,&YieldMonth, &YieldDay);
        fclose(yf);

        //sprintf(LocationName, "%s", SiteName[Site]);
        ///////////////////////////////////////////////////////////////////////////

        if(id==1)
        {
            sprintf(fname,"%s/Inputs/clim-%d",out_path,id);
            if ((fpo=sopen(fname,0))==NULL) error_print(1, fname);

            sprintf(Dname,"%s-%d.day",LocationName, id);
            sprintf(Yname,"%s-%d.yr",LocationName, id);
            fprintf(fpo,"%s\n",Dname);
            fprintf(fpo,"%s\n",Yname);
            fclose(fpo);
        }
        else
        {
            sprintf(Pname,"%s",LocationName);

            sprintf(fname,"%s/Inputs/clim-%d",out_path,id);
            if ((fpo=sopen(fname,0))==NULL) error_print(1, fname);
            sprintf(Dname1,"%s-%d-dry.day",LocationName, id);
            sprintf(Dname2,"%s-%d-mid.day",LocationName, id);
            sprintf(Dname3,"%s-%d-wet.day",LocationName, id);
            sprintf(Yname1,"%s-%d-dry.yr",LocationName, id);
            sprintf(Yname2,"%s-%d-mid.yr",LocationName, id);
            sprintf(Yname3,"%s-%d-wet.yr",LocationName, id);
            fprintf(fpo,"%s %s %s\n",Dname1,Dname2,Dname3);
            fprintf(fpo,"%s %s %s\n",Yname1,Yname2,Yname3);
            fclose(fpo);
        }
    }
    else
    {
        sprintf(fname,"%s/Inputs/clim-%d",out_path,id);
        if ((fpo=sopen(fname,0))==NULL) note(1, fname);

        sprintf(Dname,"%s-%d.day",cn, id);
        sprintf(Yname,"%s-%d.yr",cn, id);
        fprintf(fpo,"%s\n",Dname);
        fprintf(fpo,"%s\n",Yname);
        fclose(fpo);
    }

    //Create daily climate files
    if(UseID==10)//drought option
    {
        if(id==1)
        {
            CreateDailyClimateFile(Pname, Dname, Yname, UseID, ClimateFileType, lat, IfCheck,
                                   NO3NH4_Rainfall,NH3_Atmosphere,BaseCO2, CO2IncreaseRate,
                                   LocationName, YieldYear, YieldToday, 0,IData,out_path);
        }
        else
        {
            CreateDailyClimateFile(Pname, Dname1, Yname1, UseID, ClimateFileType, lat, IfCheck,
                                   NO3NH4_Rainfall,NH3_Atmosphere,BaseCO2, CO2IncreaseRate,
                                   LocationName, YieldYear, YieldToday, 1,IData,out_path);
            CreateDailyClimateFile(Pname, Dname2, Yname2, UseID, ClimateFileType, lat, IfCheck,
                                   NO3NH4_Rainfall,NH3_Atmosphere,BaseCO2, CO2IncreaseRate,
                                   LocationName, YieldYear, YieldToday, 2,IData,out_path);
            CreateDailyClimateFile(Pname, Dname3, Yname3, UseID, ClimateFileType, lat, IfCheck,
                                   NO3NH4_Rainfall,NH3_Atmosphere,BaseCO2, CO2IncreaseRate,
                                   LocationName, YieldYear, YieldToday, 3,IData,out_path);
        }
    }
    else
    {
        CreateDailyClimateFile(Pname, Dname, Yname, UseID, ClimateFileType, lat, IfCheck,
                               NO3NH4_Rainfall,NH3_Atmosphere,BaseCO2, CO2IncreaseRate,
                               "None", 0, 0, 0,IData,out_path);
    }


    return -1;
}


void CreateDailyClimateFile(char *Pname, char *Dname, char *Yname, int UseID, int ClimateFileType, float lat,
                            string IfCheck, float NO3NH4_Rainfall,float NH3_Atmosphere,float BaseCO2,
                            float CO2IncreaseRate, char *LocaName, int YieldYear, int YieldToday,
                            int DMW,Site_BaseInformation *IData,char *out_path)
{
    int tn, j, k;
    float Day_Hrs,temp=0.0,Rainfall=0.0,maxT=0.0,minT=0.0,solrad,wind_speed=2.0;
    float AveMonthT[13],AveYrT, TotalYrRain, humidity=30.0, CloudIndex=1.0;
    int   jday=0,jday0=0,Aday;
    char fname[3072],cn[256], cjunk[200], junkcc[300], ch[256];
    char CurrentClim[300], junkc[200];

    FILE* fpo, *fpi, *fcc;
    sprintf(fname,"%s\\Inputs\\%s",out_path,Dname);
    fpo=sopen(fname,"w");
    if(fpo==NULL) note(1,fname);

    if(UseID==0)
    {
        fpi=sopen(Pname,"r");
        if (fpi==NULL)
        {
            char NCF[255];
            sprintf(NCF,"%s.txt", Pname);
            fpi=fopen(NCF,"r");
            if (fpi==NULL) error_print(0, Pname);
        }
        if (ClimateFileType!=4)
            fscanf(fpi,"%s", cjunk);	// read climate file dataname

        if(strcmp(cjunk, "1")==0)
        {
            fclose(fpi);
            fpi=sopen(Pname,"r");
        }
    }
    else
    {
//        if(DMW==0)//year 1
//        {
//            sprintf(Pname, "%s/Database/Liaoning/%s/Lib_clim/%s_%d", ROOTDIR, LocaName, LocaName, YieldYear-1);
//        }
//        else if(DMW==1)//dry year
//        {
//            sprintf(Pname, "%s/Database/DroughtPrediction/Liaoning/%s/%s_dry", ROOTDIR, LocaName, LocaName);
//        }
//        else if(DMW==2)//medium
//        {
//            sprintf(Pname, "%s/Database/DroughtPrediction/Liaoning/%s/%s_mid", ROOTDIR, LocaName, LocaName);
//        }
//        else
//        {
//            sprintf(Pname, "%s/Database/DroughtPrediction/Liaoning/%s/%s_wet", ROOTDIR, LocaName, LocaName);
//        }
//
//        fpi=sopen(Pname,"r");
//        if (fpi==NULL) note(0, Pname);
//
//        if (ClimateFileType!=4)
//        {
//            fscanf(fpi,"%s", cjunk);	// read climate file dataname
//            if(strcmp(cjunk, "1")==0)
//            {
//                fclose(fpi);
//                fpi=sopen(Pname,"r");
//            }
//        }
//
//        sprintf(CurrentClim, "%s/Database/roughtPrediction/Liaoning/%s/%s_%d", ROOTDIR, LocaName, LocaName, YieldYear);
//        fcc=fopen(CurrentClim, "r");
//        if(fcc==NULL) note(0, CurrentClim);
//        fscanf(fcc,"%s", cjunk);
    }

    tn = 0;
    AveYrT = 0.0;
    TotalYrRain = 0.0;

    //default data
    wind_speed = 2.0;//m/s
    humidity = 60.0;//%
    int djunk=0, dflag=0;
    char www[100];

    for (j = 1; j<=12; j++)
    {
        AveMonthT[j] = 0;
        for (k = 1; k<=Mday[j]; k++)
        {
            dflag=0;

            if(j==1&&k==1)
            {
                fscanf(fpi,"%d", &djunk);	// read climate file dataname
                if(djunk!=1)
                {
                    fgets(www, 300, fpi);
                    dflag=0;
                }
                else
                {
                    jday = djunk;
                    dflag = 1;
                }
            }

            tn++;
            Day_Hrs = 0.0;

            if(lat<0.0)
            {
                Aday = tn - 182;//for southern semisphere
                if(Aday<=0) Aday = 365 + Aday;

                solrad = day_par(Aday, -lat, &Day_Hrs);
            }
            else
                solrad = day_par(tn, lat, &Day_Hrs);//extre-terrestrial radiation, MJ/m2/day

            if(Day_Hrs<=0.0) Day_Hrs=0.01;

            if (ClimateFileType ==0) //aveT, prec
            {
                if(dflag==1) fscanf(fpi, "%f %f", &temp, &Rainfall);
                else fscanf(fpi, "%d %f %f", &jday, &temp, &Rainfall);
                fgets(junkcc, 200, fpi);
                maxT = temp + (float)4.5;
                minT = temp - (float)4.5;

            }
            else if (ClimateFileType ==1) //maxT, minT, prec
            {
                if(dflag==1) fscanf(fpi, "%f %f %f", &maxT, &minT, &Rainfall);
                else fscanf(fpi, "%d %f %f %f", &jday, &maxT, &minT, &Rainfall);

                if(UseID==10&&DMW>0&&YieldToday>=jday)//drought effect
                {
                    if(dflag==1) fscanf(fcc, "%f %f %f", &maxT, &minT, &Rainfall);
                    else fscanf(fcc, "%d %f %f %f", &jday, &maxT, &minT, &Rainfall);
                }

                fgets(junkcc, 200, fpi);
                temp = (maxT + minT) * 0.5;

                CloudIndex = 0.9693 * (float)exp(-1.3903 * Rainfall);
                if(CloudIndex<0.0001) CloudIndex = 0.0001;
                solrad *= CloudIndex;
            }
            else if (ClimateFileType ==2) //maxT, minT, prec, radiation
            {
                if(dflag==1) fscanf(fpi, "%f %f %f %f", &maxT, &minT, &Rainfall, &solrad);
                else fscanf(fpi, "%d %f %f %f %f", &jday, &maxT, &minT, &Rainfall, &solrad);
                fgets(junkcc, 200, fpi);
                temp =float( (maxT + minT) *0.5);
            }
            else if (ClimateFileType ==3) //maxT, minT, prec, wind speed (m/s)
            {
                if(dflag==1) fscanf(fpi, "%f %f %f %f", &maxT, &minT, &Rainfall, &wind_speed);//wind speed, m/second
                else fscanf(fpi, "%d %f %f %f %f", &jday, &maxT, &minT, &Rainfall, &wind_speed);//wind speed, m/second
                temp =float( (maxT + minT) *0.5);

                CloudIndex = 0.9693 * (float)exp(-1.3903 * Rainfall);
                if(CloudIndex<0.0001) CloudIndex = 0.0001;
                solrad *= CloudIndex;

            }
            if(ClimateFileType ==4)
            {
                int yr_ID, month_ID, day_ID;
                float radiation, ave_T;
                char polygon_ID[100];

                fscanf(fpi, "%s %d %d %d %f %f %f %f %f",
                       polygon_ID, &yr_ID, &month_ID, &day_ID, &radiation, &maxT, &minT, &ave_T, &Rainfall);
                fgets(junkc, 200, fpi);
                temp =float( (maxT + minT) *0.5);
                Rainfall *= 0.1;
                jday = tn;

                CloudIndex = 0.9693 * (float)exp(-1.3903 * Rainfall);
                if(CloudIndex<0.0001) CloudIndex = 0.0001;
                solrad *= CloudIndex;
            }
            else if(ClimateFileType==5) //maxT, minT, prec, wind speed, radiation, relative humidity
            {
                if(dflag==1) fscanf(fpi, "%f %f %f %f %f %f", &maxT, &minT, &Rainfall, &wind_speed, &solrad, &humidity);
                else fscanf(fpi, "%d %f %f %f %f %f %f", &jday, &maxT, &minT, &Rainfall, &wind_speed, &solrad, &humidity);//wind speed, m/second
                temp =float( (maxT + minT) *0.5);
            }
            else if(ClimateFileType==6) //maxT, minT, prec, wind speed, relative humidity
            {
                if(dflag==1) fscanf(fpi, "%f %f %f %f %f", &maxT, &minT, &Rainfall, &wind_speed, &humidity);
                else fscanf(fpi, "%d %f %f %f %f %f", &jday, &maxT, &minT, &Rainfall, &wind_speed, &humidity);
                temp =float( (maxT + minT) *0.5);

                CloudIndex = 0.9693 * (float)exp(-1.3903 * Rainfall);
                if(CloudIndex<0.0001) CloudIndex = 0.0001;
                solrad *= CloudIndex;
            }
            else if(ClimateFileType==7) //maxT, minT, prec, relative humidity
            {
                if(dflag==1) fscanf(fpi, "%f %f %f %f", &maxT, &minT, &Rainfall, &humidity);
                else fscanf(fpi, "%d %f %f %f %f", &jday, &maxT, &minT, &Rainfall, &humidity);

                temp =float( (maxT + minT) *0.5);

                CloudIndex = 0.9693 * (float)exp(-1.3903 * Rainfall);
                if(CloudIndex<0.0001) CloudIndex = 0.0001;
                solrad *= CloudIndex;
            }

            if(tn!=jday)
            {
                cout <<  "Error at day " << tn << " in climate data file " <<  Pname << endl;
            }

            if (IfCheck=="Check")
            {

                // Check whether or not if Julian day is continous
                if (tn > 1)
                {
                    if (jday0 != jday - 1)
                    {
                        sprintf(ch,"Julian day of %dth line of <%s>",tn,fname);
                        cout << ch <<  "  Climate Data Error "  << endl;
                        fclose (fpi);
                        fclose (fpo);
                        goto outerr;
                    }
                }

                // Check if rainfall is bigger than 0
                if (Rainfall < 0.0)
                {
                    sprintf(ch,"Rainfall in %dth line of <%s>",tn,fname);
                    cout << ch <<  "  Climate Data Error "  << endl;
                    fclose (fpi);
                    fclose (fpo);

                    goto outerr;
                }

                // check if  -50<T<70
                if (temp < -50.0 || temp > 70.0)
                {
                    sprintf(ch,"Temperature in %dth line of <%s>",tn,fname);
                    cout << ch <<  "  Climate Data Error "  << endl;
                    fclose (fpi);
                    fclose (fpo);

                    goto outerr;
                }
            }
            jday0 = jday;
            if (tn==1)//(mode==0)
            {
                IData->Soil_T = temp;
            }

            // Output climate data of 365 days

            fprintf(fpo, "%3d %5.2f  %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f\n",
                    jday, temp, maxT, minT, Rainfall, solrad, Day_Hrs, wind_speed, humidity);

            //if (ClimateFileType ==3)
            //{
            //	fprintf(fpo, "%3d %5.2f  %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f\n",
            //		jday, temp, maxT, minT, Rainfall, solrad, Day_Hrs, wind_speed);
            //}
            //else
            //{
            //	fprintf(fpo, "%3d %5.2f  %5.2f %5.2f %5.2f %5.2f %5.2f\n",
            //		jday, temp, maxT, minT, Rainfall, solrad, Day_Hrs);
            //}

            // Calculate the average T and rainfall for month, year
            AveMonthT[j] += temp / Mday[j];
            AveYrT += (temp / 365.0);
            TotalYrRain += Rainfall;
        }
    }
    fclose (fpi);
    fclose (fpo);
    if(DMW>0) fclose (fcc);

    sprintf(fname,"%s/Inputs/%s",out_path,Yname);
    fpo=sopen(fname,"w");
    if (fpo==NULL) note(1, fname);
    for (j=1; j<=12; j++)
    {
        fprintf(fpo,"%.2f	", AveMonthT[j]);
    }
    fprintf(fpo,"\n%.2f\n", AveYrT);
    fprintf(fpo,"%.2f\n", TotalYrRain);
    fprintf(fpo,"%.3f\n", NO3NH4_Rainfall);//IData.
    fprintf(fpo,"%.2f\n", NH3_Atmosphere);//IData.
    fprintf(fpo,"%.2f\n", BaseCO2);//IData.
    fprintf(fpo,"%.2f\n", CO2IncreaseRate);
    fprintf(fpo,"%d\n", ClimateFileType);
    fclose (fpo);
outerr:
    ;
}

void ReadClimateHead(int yr,char* out_path, Site_BaseInformation *IData)
{
    char fname[255],fn1[256],fn2[256];
    FILE *fpo;
    int  jday;
    float temp,maxT, minT, Rainfall, solrad, Day_Hrs;

    sprintf(fname,"%s/Inputs/clim-%d",out_path,yr);
    fpo=sopen(fname, "r");
    if (fpo==NULL)
    {
        cout << "Cannot find " << fname << endl;
        return;
    }
    fscanf(fpo,"%s",fn1);
    fscanf(fpo,"%s",fn2);
    fclose(fpo);

    sprintf(fname,"%s/Inputs/%s", out_path,fn1);
    fpo=sopen(fname,"r");
    if (fpo==NULL)
    {
        cout << "Cannot find " << fname << endl;
        return;
    }
    fscanf(fpo, "%d %f %f %f %f %f %f",
           &jday, &temp, &maxT, &minT, &Rainfall, &solrad, &Day_Hrs);
    IData->Soil_T=temp;
    fclose(fpo);

    sprintf(fname,"%s/Inputs/%s",out_path,fn2);
    IData->AnnualMeanT = GetAMT(fname);
    cout << "Get AnnualMeanT " << GetAMT(fname) << endl;
    return;
}



int SaveSoilParas(int id, Site_BaseInformation IData, char* out_path)//1
{

    IData.CEC= (float)(IData.Soil_Clay * 46.0);
    IData.m  = (float)(IData.Soil_Density * 1000. * IData.H * 10000.);
    IData.V  = (float)(IData.H * 10000. * IData.Soil_Porosity);	// m^3/layer/ha

    int n;
    n = IData.Q;
    float RCVL[MAXSOILLAYER], RCL[MAXSOILLAYER], RCR[MAXSOILLAYER];
    float Urea[MAXSOILLAYER], NH3[MAXSOILLAYER];
    float Tranr[MAXSOILLAYER], OCini[MAXSOILLAYER], Dphum[MAXSOILLAYER];
    float s_NO3[MAXSOILLAYER], s_NH4[MAXSOILLAYER], ClayNH4[MAXSOILLAYER];
    float s_NO[MAXSOILLAYER], s_N2O[MAXSOILLAYER];
    float s_N2[MAXSOILLAYER], s_NO2[MAXSOILLAYER], s_IniNH4[MAXSOILLAYER];

    float PS, FLDCAPw, Wiltptw, TravelTime, ClayF1, ClayF2;//, ePS;
    float RCNRVL, RCNRL, RCNRR, RCNB, RCNH, RCNM;
    float stub1, stub2, stub3, Dbm;

    PS		= IData.H * IData.Soil_Porosity;	// porosity  (m/layer)
    //ePS = PS * 0.8;
    FLDCAPw = PS * IData.Fldcap;	// m water/layer IData.FieldCapacity
    Wiltptw = PS * IData.Wiltpt;	// m water/layer
    TravelTime = 20.0 * (PS - FLDCAPw) / IData.Sks;	// hr/layer
    // fraction of soil water available for plants, can be tuned
    ClayF1 = pow(10.0,-IData.Soil_Clay / 0.75 - 0.2);
    ClayF2 = IData.Soil_Clay * 6.;
    RCNRVL = 5.;
    RCNRL  = 25.;
    RCNRR  = 100.;
    RCNB   = 10.0;
    RCNH   = 10.0;
    RCNM   = 10.0;

    int DFF;
    char SPP[200];
    FILE* spp;
    sprintf(SPP,"%s/Inter/SOCpartition.txt",out_path);
    spp=fopen(SPP, "r");
    if(spp==NULL)
    {

    }
    else
    {
        fscanf(spp, "%d", &DFF);
        if(DFF==1)
            fscanf(spp, "%f %f %f %f %f", &RCNRVL, &RCNRL, &RCNRR, &RCNH, &RCNM);
        fclose(spp);
    }

    //RCNB=RCNH;


    Tranr[1] = IData.Soil_SOCatSurface * IData.Soil_Litter * IData.m;
    OCini[1] = IData.Soil_SOCatSurface * IData.Soil_humads * IData.m;
    PR(IData.Soil_SOCatSurface);
    PR(IData.Soil_humads);
    PR(IData.m);
    Dphum[1] = IData.Soil_SOCatSurface * IData.Soil_humus * IData.m;

    s_NO3[1] = IData.Soil_NO3 * IData.m / 1000000.;
    s_IniNH4[1] = IData.Soil_NH4 * IData.m / 1000000.;

    RCVL[1] = 0.0 * Tranr[1];
    RCL[1]  = 0.0 * Tranr[1];
    RCR[1]  = 1.0 * Tranr[1];

    /*	if (IfForest()) //ffffffff
    	{
    		RCVL[1] = 0;
    		RCL[1]  = 0;
    		RCR[1]  = Tranr[1];
    		RCNRVL = 20.0;
    		RCNRL = 400.0 / 11.0;
    		RCNRR = 200.0;
    		IData.LitterWater = (float)(IData.Soil_moiture / IData.Fldcap * IData.FloorC /300000.0);

    		float Fo;
    		Fo = 2 * IData.Soil_SOCatSurface;
    		Dbm = float((1 - Fo) * 0.14 * IData.Soil_Density /
    			(.14 - IData.Soil_Density * Fo));
    	}
    	else*/

    Dbm = IData.Soil_Density;


    s_NH4[1]   = s_IniNH4[1] * 0.7;
    ClayNH4[1] = s_IniNH4[1] * 0.3;

    float F_soc = IData.Soil_SOCatSurface/0.058;
    if(F_soc>1.0) F_soc = 1.0;

    //ClayNH4[1] = F_soc * IData.CEC * 0.4;
    //ClayNH4[1] = IData.CEC * 0.4; //NewChange

    Urea[1]= 0.;
    NH3[1] = 0.;
    stub1  = 0.;
    stub2  = 0.;
    stub3  = 0.;
    //WeedN  = 0.0;

    int i;
    float TD1,TD2,TD3,TD4,TD5,TD6,TD7,TD8;
    float TCN=0.0;

    TD1 = RCVL[1];
    TD2 = RCL[1];
    TD3 = RCR[1];
    TD4 = OCini[1];
    TD5 = Dphum[1];
    TD6 = s_NO3[1];//NO3[1];
    TD7 = s_NH4[1];//NH4[1];
    TD8 = ClayNH4[1];


    /*for (i = 1;i<=IData.Q;i++)
    {
        if (i <= IData.Toph)
    	{
            RCVL[i]    = TD1 * IData.SOC12;
            RCL[i]	   = TD2 * IData.SOC12;
            RCR[i]	   = TD3 * IData.SOC12;
            OCini[i]   = TD4 * IData.SOC12;
            Dphum[i]   = TD5 * IData.SOC12;
            s_NO3[i]   = TD6 * IData.SOC12;
            s_NH4[i]   = TD7 * IData.SOC12;
            ClayNH4[i] = TD8 * IData.SOC12;
            Urea[i]    = Urea[1];
            NH3[i]     = NH3[1];
    		TCN += ClayNH4[i];
    	}
        else
    	{
            //dt = pow(IData.Topr, (i - IData.Toph) * IData.H / 0.1));
            dt         = pow(IData.Topr, (i - IData.Toph) * IData.H / 0.1);
            RCVL[i]    = TD1 / dt;
            RCL[i]     = TD2 / dt;
            RCR[i]     = TD3 / dt;
            OCini[i]   = TD4 / dt;
            Dphum[i]   = TD5 / dt;
            s_NO3[i]   = TD6 / dt;
            s_NH4[i]   = TD7 / dt;
            ClayNH4[i] = ClayNH4[(int)IData.Toph];
            Urea[i]    = Urea[1] / dt;
            NH3[i]     = NH3[1] / dt;
    		TCN += ClayNH4[i];
        }*/
    s_NO[1] = 0.;
    s_N2O[1]= 0.;
    s_N2[1] = 0.;
    s_NO2[1]= 0.;
    //}

    char fname[255];
    FILE *fpo;
    sprintf(fname,"%s/Inputs/soil-%d",out_path,(int)fabs(id));
    fpo=sopen(fname,"w");
    if (fpo==NULL) return -1;

    float IniFrostNO=0, IniFrostN2O=0, IniFrostN2=0,ice=0,tt[120];
    int Retard=1;
    if (IData.SoilRetardation==1) Retard=3;

    //check data
    if (IData.Soil_Texture<0)
    {
        cout << "No Soil_Texture information!" << "Input Error:";
        fclose(fpo);
        return -1;
    }
    if (IData.Soil_Clay<0)
    {
        cout << "No Soil_Clay information!" <<"Input Error:" << endl;
        fclose(fpo);
        return -1;
    }
    if (IData.Soil_Density<0)
    {
        cout << "No Soil_Density information!" << "Input Error:" << endl;
        fclose(fpo);
        return -1;
    }
    if (IData.Soil_SOCatSurface<0)
    {
        cout << "No Soil_Organic information!" << "Input Error:"<< endl;
        fclose(fpo);
        return -1;
    }
    if (IData.Soil_NO3<0)
    {
        cout << "No Soil_NO3 information! " << "Input Error:" << endl;
        fclose(fpo);
        return -1;
    }
    if (IData.Soil_NH4<0)
    {
        cout << "No Soil_NH4 information!" << "Input Error:" << endl;
        fclose(fpo);
        return -1;
    }
    if (IData.Soil_Litter<0 || IData.Soil_humads<0 || IData.Soil_humus<0)
    {
        cout << "Soil_Litter or humads, mumus information error!" << "Input Error:"<< endl;
        fclose(fpo);
        return -1;
    }

    for (i=1; i<=IData.Q; i++)
    {
        tt[i] = IData.AnnualMeanT + (IData.Soil_T - IData.AnnualMeanT)/ (float)pow(i*IData.H*100, 0.5);
        if (IData.Soil_T<0.0 && tt[i] < IData.Soil_T) tt[i] = IData.Soil_T;


        /*tt[L] =  yr_avet + (s_temp - yr_avet)
        	/ (float)pow(L*H*100, 0.5);
        if (s_temp<0.0 && tt[L] < s_temp) tt[L] = s_temp;*/
    }


    fprintf(fpo,"%3d\n", IData.Soil_Texture);
    fprintf(fpo,"%s\n\n", IData.SoilName);

    fprintf(fpo,"%.4f  %.4f  %.4f  %.4f  %.4f  %.4f\n",
            IData.Soil_Clay, IData.Soil_Porosity, IData.Sks, IData.Fldcap,
            IData.Wiltpt, IData.LateralInflux);
    fprintf(fpo,"%f %f %f %f %d  %f\n",
            IData.Psisat, IData.Beta, Dbm, IData.Soil_pH,
            IData.Idc, IData.Soil_SOCatSurface);
    fprintf(fpo,"%.4f  %.4f  %.4f  %.4f  %.4f  %.4f\n",
            IData.Soil_NO3, IData.Soil_NH4, IData.Soil_Litter,
            IData.Soil_humads, IData.Soil_humus, 0.0);//Byp
    fprintf(fpo,"%.4f  %.4f  %.4f  %.4f  %.4f  %d\n",
            IData.H, IData.CEC, IData.TH, IData.m, IData.V, IData.Q);
    fprintf(fpo,"%.4f  %.4f\n", ClayF1, ClayF2);
    fprintf(fpo,"%.4f  %.4f  %d  %.4f  %.4f  %.4f  %.4f\n",
            IData.Qstar, IData.BypassF, Retard, RCNB, RCNH, RCNM, RCNRVL);
    fprintf(fpo,"%.4f  %.4f %.4f  %.4f  %.4f  %.4f\n",
            RCNRL, RCNRR, IData.AcWater, IData.SurfWater,
            IData.SnowPack, stub1);
    fprintf(fpo,"%.4f  %.4f\n", stub2, stub3);
    fprintf(fpo, "%.4f  %.4f  %.4f\n\n", IniFrostNO, IniFrostN2O, IniFrostN2);

    int WRL = (int)(IData.Soil_WaterObstructLayer*0.01/IData.H);
    /*for (i = 1;i<=IData.Q;i++)
    {
    	//if(WRL>0&&i>=WRL) IData.Soil_moiture=1.0;
    	//else IData.Soil_moiture=IData.Fldcap;

    	IData.Soil_moiture=IData.Fldcap;

        fprintf(fpo,"%.4f  %.4f  %.4f  %.4f  %.4f\n",
    		RCVL[i], RCL[i], RCR[i], OCini[i], Dphum[i]);
        fprintf(fpo,"%.4f  %.4f  %.4f  %.4f  %.4f  %.4f\n",
    		s_NO2[i], s_NO3[i], s_NH4[i], ClayNH4[i], Urea[i], NH3[i]);
        fprintf(fpo,"%.4f  %.4f  %.4f  %.4f  %.4f  %.4f\n",
    		s_NO[i], s_N2O[i], s_N2[i], tt[i],
    		IData.Soil_moiture,ice);

    	fprintf(fpo,"\n");
    }*/

    fprintf(fpo, "%f\n", IData.Soil_MicrobioIndex);
    fprintf(fpo, "%f\n", IData.Soil_WaterObstructLayer);//m
    fprintf(fpo, "%f %f\n", IData.slope, IData.Soil_Quality);
    fprintf(fpo, "%f\n", IData.Fldcap);
    fprintf(fpo, "%f\n", IData.Wiltpt);
    fprintf(fpo, "%f %f\n", IData.SOCa, IData.SOCb);
    fprintf(fpo, "%f %f %f %f %f %f %f %f %f\n", RCVL[1], RCL[1], RCR[1], OCini[1], Dphum[1],
            s_NO2[1], s_NO3[1], s_NH4[1], ClayNH4[1]);
    fprintf(fpo, "%f\n", tt[1]);
    fprintf(fpo, "%f\n", IData.HighestWT);
    fprintf(fpo, "%d\n", IData.ProfileType);
    fprintf(fpo, "%f\n", IData.PassiveC);
    fprintf(fpo, "%f\n", IData.DClitter);
    fprintf(fpo, "%f\n", IData.DChumads);
    fprintf(fpo, "%f\n", IData.DChumus);
    fprintf(fpo, "%f\n", IData.HumadCN);
    fprintf(fpo, "%f\n", IData.HumusCN);


    fclose (fpo);

    if (id<0) return 0;

    /*  sprintf(fname,"%s/Inputs/soil_site.inp",OUTPUT);
    	fpo=sopen(fname,0);
    	if (fpo==NULL) return -1;
    	fprintf(fpo, "---------------------------------------------------------------\n");
    	fprintf(fpo, "Soil:\n");
    	fprintf(fpo, "      -land-use                          %d\n\n", IData.Soil_landuse);

    	fprintf(fpo, "      -profile thickness                 %.2f\n", IData.TH);
    	fprintf(fpo, "      -layer thickness (m)               %.3f\n", IData.H);
    	fprintf(fpo, "      -number of total soil layers       %d\n\n", IData.Q);

    	fprintf(fpo, "      -bulk density (g/cm^3)             %.2f\n", IData.Soil_Density);
    	fprintf(fpo, "      -soil mass (kg soil/layer)         %.2f\n", IData.m);
    	fprintf(fpo, "      -soil volume (m3/layer)            %.2f\n\n", IData.V);

    	fprintf(fpo, "      -texture                           %s (%d)\n", IData.SoilName, IData.Soil_Texture);
    	fprintf(fpo, "      -porosity                          %.2f\n", IData.Soil_Porosity);
    	fprintf(fpo, "      -PS                                %.2f\n", PS);
    	fprintf(fpo, "      -sat. hydro-conductivity (m/hr)    %.3f\n", IData.Sks);
    	fprintf(fpo, "      -field capasity (wfps)             %.2f\n", IData.Fldcap);
    	fprintf(fpo, "      -field capasity (m water/layer)    %.2f\n", FLDCAPw);
    	fprintf(fpo, "      -wilt point (wfps)                 %.2f\n", IData.Wiltpt);
    	fprintf(fpo, "      -wilt point (m water/layer)        %.2f\n", Wiltptw);
    	fprintf(fpo, "      -water travel time                 %.2f\n", TravelTime);
    	fprintf(fpo, "      -daturated Psi                     %.2f\n", IData.Psisat);
    	fprintf(fpo, "      -Beta                              %.2f\n", IData.Beta);
    	fprintf(fpo, "      -Idc                               %d\n", IData.Idc);
    	fprintf(fpo, "      -bypass flow                       %f\n", IData.Soil_BypassFlow);
    	fprintf(fpo, "      -bypass factor                     %f\n", IData.BypassF);
    	fprintf(fpo, "      -retardation for infiltration      %d\n", IData.SoilRetardation);
    	fprintf(fpo, "      -Retard                            %d\n", Retard);
    	fprintf(fpo, "      -heat capasity                     %.2f\n\n", IData.Soilhtcp);

    	fprintf(fpo, "      -initial SOC at surface (kg C/kg)  %.4f\n", IData.Soil_SOCatSurface);
    	fprintf(fpo, "      -fraction of passive SOC           %.2f\n", IData.Soil_humus);
    	fprintf(fpo, "                of litter  SOC           %.2f\n", IData.Soil_Litter);
    	fprintf(fpo, "                of humads  SOC           %.2f\n", IData.Soil_humads);
    	fprintf(fpo, "      -microbial C/N                     %.2f\n", RCNB);
    	fprintf(fpo, "      -humads C/N                        %.2f\n", RCNH);
    	fprintf(fpo, "      -humus C/N                         %.2f\n", RCNM);
    	fprintf(fpo, "      -very labile litter C/N            %.2f\n", RCNRVL);
    	fprintf(fpo, "      -labile litter C/N                 %.2f\n", RCNRL);
    	fprintf(fpo, "      -resistant litter C/N              %.2f\n\n", RCNRR);

    	fprintf(fpo, "      -clay fraction                     %.2f\n", IData.Soil_Clay);
    	fprintf(fpo, "      -cation exchange capasity          %.2f\n", IData.CEC);
    	fprintf(fpo, "      -Clay factor 1                     %.2f\n", ClayF1);
    	fprintf(fpo, "      -Clay factor 2                     %.2f\n", ClayF2);
    	fprintf(fpo, "      -Qstar                             %.2f\n", IData.Qstar);
    	fprintf(fpo, "      -Soil pH                           %.2f\n", IData.Soil_pH);
    	//fprintf(fpo, "      -Flood water pH                    %.2f\n\n", IData.Flood_pH);
    	fprintf(fpo, "      -initial NO3(-) (mg N/kg)          %.2f\n", IData.Soil_NO3);
    	fprintf(fpo, "      -initial NH4(+) (mg N/kg)          %.2f\n", IData.Soil_NH4);
    	fprintf(fpo, "      -initial soil moisture (wfps)      %.2f\n", IData.Soil_moiture);
    	fprintf(fpo, "      -initial soil temperature (C)      %.2f\n", IData.Soil_T);
    	fprintf(fpo, "      -water content                     %.2f\n", IData.AcWater);
    	fprintf(fpo, "      -ponding water                     %.2f\n", IData.SurfWater);
    	fprintf(fpo, "      -snow pack                         %.2f\n", IData.SnowPack);
    	fprintf(fpo, "      -stub                              %.2f\n", stub1+stub2+stub3);
    	//fprintf(fpo, "      -N in weeds                        %.2f\n", WeedN);
    	fprintf(fpo, "      -NO in frozen soil                 %.2f\n", IniFrostNO);
    	fprintf(fpo, "      -N2O in frozen soil                %.2f\n", IniFrostN2O);
    	fprintf(fpo, "      -N2 in frozen soil                 %.2f\n", IniFrostN2);

    	fclose(fpo);
    */
    return 0;

}


int SaveCropParas(char *out_path, char *Model_Path) //!!!!!!!!!!!
{
    char fname[255],outname[255];
    int  i, j, k, kkk, yr;//, hyr=0;
    FILE *fcrop, *fpi;

    int db_Rotation_Number;
    int db_RotationID[20], db_RotateSpan[20], db_CycleSpan[20],db_PerennialCrop[20][20][5];
    int db_YrCrops[20][20], db_YrCropOrder[20][20][5], db_YrCropType[20][20][5], db_CoverCrop[20][20][5];
    int db_PMonth[20][20][5], db_PDay[20][20][5], db_HMonth[20][20][5], db_HDay[20][20][5], db_HYr[20][20][5];
    float db_Residue[20][20][5], db_Yield[20][20][5], db_RepRate[20][20][5], db_VegRate[20][20][5];
    float db_PsnEff[20][20][5], db_PsnMax[20][20][5], db_IniBiomass[20][20][5];

    float db_GrainFraction[20][20][5], db_ShootFraction[20][20][5], db_RootFraction[20][20][5];
    float db_GrainCN[20][20][5], db_ShootCN[20][20][5], db_RootCN[20][20][5];
    float db_TDD[20][20][5], db_Water[20][20][5], db_LAI[20][20][5], db_Nfix[20][20][5], db_Vascularity[20][20][5];

    CropofEachYear CYr[2];



    char DB[255];
    FILE *db;

    sprintf(DB,"%s/Inter/CropRotation.txt", out_path);
    db=fopen(DB, "r");
    if(db==NULL)
    {
        cout << "Can not open file " << DB << endl;
    }
    fscanf(db, "%d", &db_Rotation_Number);
    fclose(db);

    int ccc;
    for(int rrr=1; rrr<=db_Rotation_Number; rrr++)
    {
        sprintf(DB,"%s/Inter/CropRotation_%d.txt", out_path, rrr);
        db=fopen(DB, "r");
        fscanf(db, "%d %d %d", &db_RotationID[rrr], &db_RotateSpan[rrr], &db_CycleSpan[rrr]);
        fclose(db);

        for(int yyy=1; yyy<=db_CycleSpan[rrr]; yyy++)
        {
            sprintf(DB,"%s/Inter/CropRotation_%d_%d.txt", out_path, rrr, yyy);
            db=fopen(DB, "r");
            fscanf(db, "%d", &db_YrCrops[rrr][yyy]);

            for(int ccc=1; ccc<=db_YrCrops[rrr][yyy]; ccc++)
            {
                fscanf(db, "%d %d", &db_YrCropOrder[rrr][yyy][ccc], &db_YrCropType[rrr][yyy][ccc]);
                fscanf(db, "%d %d", &db_PMonth[rrr][yyy][ccc], &db_PDay[rrr][yyy][ccc]);
                fscanf(db, "%d %d", &db_HMonth[rrr][yyy][ccc], &db_HDay[rrr][yyy][ccc]);
                fscanf(db, "%d", &db_HYr[rrr][yyy][ccc]);
                fscanf(db, "%f %f", &db_Residue[rrr][yyy][ccc], &db_Yield[rrr][yyy][ccc]);
                fscanf(db, "%f %f", &db_RepRate[rrr][yyy][ccc], &db_VegRate[rrr][yyy][ccc]);
                fscanf(db, "%f %f", &db_PsnEff[rrr][yyy][ccc], &db_PsnMax[rrr][yyy][ccc]);
                fscanf(db, "%f", &db_IniBiomass[rrr][yyy][ccc]);
                fscanf(db, "%d", &db_CoverCrop[rrr][yyy][ccc]);
                fscanf(db, "%d", &db_PerennialCrop[rrr][yyy][ccc]);

                fscanf(db, "%f %f %f", &db_GrainFraction[rrr][yyy][ccc], &db_ShootFraction[rrr][yyy][ccc], &db_RootFraction[rrr][yyy][ccc]);
                fscanf(db, "%f %f %f", &db_GrainCN[rrr][yyy][ccc], &db_ShootCN[rrr][yyy][ccc], &db_RootCN[rrr][yyy][ccc]);
                fscanf(db, "%f %f %f %f %f", &db_TDD[rrr][yyy][ccc], &db_Water[rrr][yyy][ccc], &db_LAI[rrr][yyy][ccc], &db_Nfix[rrr][yyy][ccc], &db_Vascularity[rrr][yyy][ccc]);

            }

            fclose(db);

            ///////////////////////////////////////////////////////////
            char fname[255];
            FILE *fp;
            int WinterCropFlag=0, LeftDays=0;

            sprintf(fname, "%s/Inter/cyc-%d_%d", out_path,rrr, yyy);
            fp=fopen(fname, "w");

            if (db_YrCrops[rrr][yyy]==0) //no crop
            {
                fprintf(fp, "%d\n", 0);
                fprintf(fp, "%d  %s  %d  %d  %f  %d\n", 0, "None", 0, 0, 0.0, 10);
                fprintf(fp, "%d  %d  %f  %f  %d\n", 0, 0, 0.0, 0.0, 999);
                fprintf(fp, "%f  %f  %f  %f  %f\n", 0.0, 0.0, 0.0, 0.0, 0.0);
                fprintf(fp, "%d\n", 0);
                //fprintf(fp, "%d\n", 0);

                // CropCode, WinterCropFlag, WinterCropLeftDays, OptimumYield, HarvestYear;
                // PalntingJday, GrowingDays, ResidueIncorporation, MaxNDemand, EndCropFlag;
                // IniCropBiomass, GRvege, GRreprod, PsnEfficiency, PsnMax
                //cover_crp
            }
            else
            {
                fprintf(fp, "%d\n", db_YrCrops[rrr][yyy]);
                for (i = 1; i<=db_YrCrops[rrr][yyy]; i++)
                {
                    int SeedDay = JulianDay(db_PMonth[rrr][yyy][i], db_PDay[rrr][yyy][i]);
                    int HarvDay = JulianDay(db_HMonth[rrr][yyy][i], db_HDay[rrr][yyy][i]);
                    int GrowDays;
                    if(db_HYr[rrr][yyy][i]==1)//SeedDay<HarvDay)
                    {
                        GrowDays = HarvDay - SeedDay;
                        WinterCropFlag = 0;
                        LeftDays = 0;
                    }
                    else
                    {
                        GrowDays = 365 - SeedDay + HarvDay + (db_HYr[rrr][yyy][i] - 2) * 365;
                        WinterCropFlag = db_HYr[rrr][yyy][i] - 1;//yyy-1;
                        if(WinterCropFlag==1) LeftDays = GrowDays;
                        else LeftDays = GrowDays - (365*(db_HYr[rrr][yyy][i] - 1) - SeedDay);
                    }

                    int wwww=111, jd;
                    if(i==db_YrCrops[rrr][yyy]) wwww = 999;

                    char VEGE[300], Crop_Name[100], js[100];
                    FILE* fpw;
                    sprintf(VEGE, "%s/Library/Lib_crop/crop_%d",Model_Path , db_YrCropType[rrr][yyy][i]);
                    fpw = fopen(VEGE, "r");
                    if (fpw==NULL) note(0, VEGE);

                    fscanf(fpw, "%d %s", &jd, js);
                    fscanf(fpw, "%s %s", js, js);
                    fscanf(fpw, "%s %s", Crop_Name, js);
                    fclose(fpw);

                    float PlantC = db_Yield[rrr][yyy][i] / db_GrainFraction[rrr][yyy][i];
                    float ShootC = PlantC * db_ShootFraction[rrr][yyy][i];
                    float RootC = PlantC * db_RootFraction[rrr][yyy][i];

                    float MaxPlantN;
                    if(db_YrCropType[rrr][yyy][i]==0) MaxPlantN = 0.0;
                    else MaxPlantN = db_Yield[rrr][yyy][i]/db_GrainCN[rrr][yyy][i] + ShootC/db_ShootCN[rrr][yyy][i] + RootC/db_RootCN[rrr][yyy][i];


                    fprintf(fp, "%d  %s  %d  %d  %f  %d\n", db_YrCropType[rrr][yyy][i], Crop_Name, WinterCropFlag, LeftDays, db_Yield[rrr][yyy][i], db_HYr[rrr][yyy][i]);
                    fprintf(fp, "%d  %d  %f  %f  %d\n", SeedDay, GrowDays, db_Residue[rrr][yyy][i], MaxPlantN, wwww);
                    fprintf(fp, "%f  %f  %f  %f  %f\n", db_IniBiomass[rrr][yyy][i], db_VegRate[rrr][yyy][i],
                            db_RepRate[rrr][yyy][i], db_PsnEff[rrr][yyy][i], db_PsnMax[rrr][yyy][i]);
                    fprintf(fp, "%d\n", db_CoverCrop[rrr][yyy][i]);
                    fprintf(fp, "%d\n", db_PerennialCrop[rrr][yyy][i]);
                    fprintf(fp, "%f %f %f\n", db_GrainFraction[rrr][yyy][i], db_ShootFraction[rrr][yyy][i], db_RootFraction[rrr][yyy][i]);
                    fprintf(fp, "%f %f %f\n", db_GrainCN[rrr][yyy][i], db_ShootCN[rrr][yyy][i], db_RootCN[rrr][yyy][i]);
                    fprintf(fp, "%f %f %f %f %f\n", db_TDD[rrr][yyy][i], db_Water[rrr][yyy][i], db_LAI[rrr][yyy][i], db_Nfix[rrr][yyy][i], db_Vascularity[rrr][yyy][i]);
                }
            }
            fclose (fp);
            ///////////////////////////////////////////////////////////
        }

    }

    int CropNumbers;
    char Crop_Name[100];
    yr = 0;
    for (int i = 1; i<=db_Rotation_Number; i++)
    {
        kkk = 0;
        for (int j = 1; j<=db_RotateSpan[i]; j++) //total years within a rotation
        {
            yr++;
            kkk++;

            // Open output crop file
            sprintf(outname,"%s/Inputs/crop-%d",out_path,yr);
            fcrop=sopen(outname, "w");
            if(fcrop==NULL) note(1, outname);

            sprintf(fname,"%s/Inter/cyc-%d_%d",out_path,i,kkk);
            fpi=sopen(fname, "r");
            if (fpi==NULL) note(0, fname);
            //{
            //	fclose(fcrop);
            //	return -1;
            //}

            fscanf(fpi, "%d", &CropNumbers);
            fprintf(fcrop, "%d\n", CropNumbers);

            for (int k = 1; k<=CropNumbers; k++)
            {
                fscanf(fpi, "%d  %s  %d  %d  %f  %d",
                       &CYr[1].CropID, Crop_Name, &CYr[1].WCFlag, &CYr[1].Left, &CYr[1].Yield,
                       &CYr[1].HarYr);
                fscanf(fpi, "%d  %d  %f  %f  %d",
                       &CYr[1].SeedDay, &CYr[1].GrowDays, &CYr[1].LeftPrec,
                       &CYr[1].MaxPlanN, &CYr[1].CropOrder);
                fscanf(fpi, "%f  %f  %f  %f  %f",
                       &CYr[1].InitialBiomass, &CYr[1].GrowthVegetative, &CYr[1].GrowthReproductive,
                       &CYr[1].PsnEfficiency, &CYr[1].PsnMax);
                fscanf(fpi, "%d", &CYr[1].CoverCrop);
                fscanf(fpi, "%d", &CYr[1].PerennialCrop);

                fscanf(fpi, "%f %f %f", &CYr[1].Grain, &CYr[1].Straw, &CYr[1].Root);
                fscanf(fpi, "%f %f %f", &CYr[1].GrainCN, &CYr[1].ShootCN, &CYr[1].RootCN);
                fscanf(fpi, "%f %f %f %f %f", &CYr[1].CropTDD, &CYr[1].WRuq, &CYr[1].MXLAI,
                       &CYr[1].CropNFix, &CYr[1].Vascularity);

                if(CYr[1].CropID==0) CYr[1].HarYr=1;

                //if(yr==1)
                //{
                CYr[1].WCFlag = 0;
                CYr[1].Left = 0;
                //}

                fprintf(fcrop, "%d  %s  %d  %d  %f  %d\n",
                        CYr[1].CropID, Crop_Name, CYr[1].WCFlag, CYr[1].Left, CYr[1].Yield, CYr[1].HarYr);
                fprintf(fcrop, "%d  %d  %f  %f  %d\n",
                        CYr[1].SeedDay, CYr[1].GrowDays, CYr[1].LeftPrec,
                        CYr[1].MaxPlanN, CYr[1].CropOrder);
                fprintf(fcrop, "%f  %f  %f  %f  %f\n",
                        CYr[1].InitialBiomass, CYr[1].GrowthVegetative, CYr[1].GrowthReproductive,
                        CYr[1].PsnEfficiency, CYr[1].PsnMax);
                fprintf(fcrop, "%d\n", CYr[1].CoverCrop);
                fprintf(fcrop, "%d\n", CYr[1].PerennialCrop);
                fprintf(fcrop, "%f %f %f\n", CYr[1].Grain, CYr[1].Straw, CYr[1].Root);
                fprintf(fcrop, "%f %f %f\n", CYr[1].GrainCN, CYr[1].ShootCN, CYr[1].RootCN);
                fprintf(fcrop, "%f %f %f %f %f\n", CYr[1].CropTDD, CYr[1].WRuq, CYr[1].MXLAI, CYr[1].CropNFix, CYr[1].Vascularity);

            }

            //fscanf(fpi, "%d", &CYr[1].PerennialCrop);
            //fprintf(fcrop, "%d\n", CYr[1].PerennialCrop);
            fclose (fpi);
            fclose (fcrop);


            // Output Crop farming parameters in YRth year

            // output Tillage data
            sprintf(fname,"%s/Inter/ti-%d_%d",out_path,i,kkk);
            sprintf(outname,"%s/Inputs/till-%d",out_path,yr);
            FileCopy(fname,outname);

            // out_path Fertilization data
            sprintf(fname,"%s/Inter/fe-%d_%d",out_path,i,kkk);
            sprintf(outname,"%s/Inputs/fert-%d",out_path,yr);
            FileCopy(fname,outname);

            // output Manure data
            sprintf(fname,"%s/Inter/ma-%d_%d",out_path,i,kkk);
            sprintf(outname,"%s/Inputs/manu-%d",out_path,yr);
            FileCopy(fname,outname);

            // output Flooding data
            sprintf(fname,"%s/Inter/fl-%d_%d",out_path,i,kkk);
            sprintf(outname,"%s/Inputs/flood-%d",out_path,yr);
            FileCopy(fname,outname);

            // output Irrigation data
            sprintf(fname,"%s/Inter/ir-%d_%d",out_path,i,kkk);
            sprintf(outname,"%s/Inputs/irri-%d",out_path,yr);
            FileCopy(fname,outname);

            // output Weeding data
            sprintf(fname,"%s/Inter/we-%d_%d",out_path,i,kkk);
            sprintf(outname,"%s/Inputs/plastic-%d",out_path,yr);
            FileCopy(fname,outname);

            // output Grazing data
            sprintf(fname,"%s/Inter/gr-%d_%d",out_path,i,kkk);
            sprintf(outname,"%s/Inputs/graz-%d",out_path,yr);
            FileCopy(fname,outname);

            // output grass cutting data
            sprintf(fname,"%s/Inter/cut-%d_%d",out_path,i,kkk);
            sprintf(outname,"%s/Inputs/cut-%d",out_path,yr);
            FileCopy(fname,outname);

            if (kkk == db_CycleSpan[i]) kkk = 0;

        }
    }

    char F_INI[200];
    FILE *fpp;
    sprintf(F_INI, "%s/Inputs/cropini-1", out_path);
    fpp = fopen(F_INI, "w");
    if (fpp == NULL) note(1, F_INI);
    fprintf(fpp, "%d\n", 0);
    fclose( fpp );

    return 0;

}

int SaveInterFarmParas(char * out_path)
{
    int rrr, yyy;
    int result = -1;
    char fname[255];
    FILE *fp;
    int i, n, day, db_Rotation_Number, db_RotationID[20], db_RotateSpan[20], db_CycleSpan[20];

    int TillID[300], TillMonth[300], TillDay[300], TillMethod[300];
    int FertMonth[300], FertDay[300], FertMethod[300];
    float FertNitrate[300], FertAbi[300], FertUrea[300], FertAnh[300], FertAmNi[300], NH42SO4[300], NH42HPO4[300], FertDepth[300];
    int ManuMonth[300], ManuDay[300], ManuType[300], ManuMethod[300], FertOption;
    float ManuAmount[300], ManuCN[300], ManuN[300];
    float DayRelease[300], NIefficiency[300], NIduration[300],UreaseEfficiency[300], UreaseDuration[300];
    int WeedMonth1[300], WeedDay1[300], WeedMonth2[300], WeedDay2[300];
    int FlooID[300], FlooMonth1[300], FlooDay1[300], FlooMonth2[300], FlooDay2[300];
    float FlooN[300];
    int ShallowFlood[300];
    int IrriMonth[300], IrriDay[300], IrriMethod[300];
    float IrriAmount[300];
    int GrazMonth1[300], GrazDay1[300], GrazMonth2[300], GrazDay2[300];
    float GrazHour[300], Cattle[300], Horse[300], Sheep[300];
    float Dairy[300], Beef[300], Pig[300];
    int CutMonth[300], CutDay[300], CutPart[300];
    float CutAmount[300];
    char Fertigation[200];
    float WaterGetherIndex, WaterLeakRate;
    float m_IniWT, m_LWTceasingSurfFlow, m_LWTceasingGroungFlow, m_WatershedIndex,m_SurfOutflowIntensity, m_GroundOutflowIntensity;


    char DB[255];
    FILE *db;

    sprintf(DB,"%s/Inter/CropRotation.txt", out_path);
    db=fopen(DB, "r");
    if(db==NULL)
    {
        note(1,DB);
    }
    fscanf(db, "%d", &db_Rotation_Number);
    fclose(db);

    for(rrr=1; rrr<=db_Rotation_Number; rrr++)
    {
        sprintf(DB,"%s/Inter/CropRotation_%d.txt", out_path, rrr);
        db=fopen(DB, "r");
        fscanf(db, "%d %d %d", &db_RotationID[rrr], &db_RotateSpan[rrr], &db_CycleSpan[rrr]);
        fclose(db);

        for(yyy=1; yyy<=db_CycleSpan[rrr]; yyy++)
        {



            // Save tillage
            sprintf(DB,"%s/Inter/CropTill_%d_%d.txt", out_path, rrr, yyy);
            db=fopen(DB, "r");
            fscanf(db, "%d", &n);
            for(i=1; i<=n; i++)
            {
                TillID[i] = i;
                fscanf(db, "%d %d %d", &TillMonth[i], &TillDay[i], &TillMethod[i]);
            }
            fclose(db);

            //	TillageParameter *tp;
            sprintf(fname,"%s/Inter/ti-%d_%d",out_path,rrr,yyy);
            fp=sopen(fname, "w");
            if (fp == NULL) note(1,fname);

            //rn = rrr;
            //cn = yyy;

            fprintf(fp,"%d\n",n);
            for (i = 1; i<=n; i++)
            {
                /*int   day;
                float mtd;

                day = JulianDay(TillMonth[i], TillDay[i]);

                if (TillMethod[i] == 1)
                	mtd = 0;
                else if (TillMethod[i] == 2)
                	mtd = (float)0.05;
                else if (TillMethod[i] == 3)
                	mtd = (float)0.1;
                else if (TillMethod[i] == 4)
                	mtd = (float)0.2;
                else if (TillMethod[i] == 5)
                	mtd = (float)0.3;
                else if (TillMethod[i] == 6)
                	mtd = (float)0.45;
                else {
                MessageBox(NULL,"1. Tillage method is wrong!","Input Error:",0);
                fclose(fp);
                return -1;
                }*/
                day = JulianDay(TillMonth[i], TillDay[i]);

                fprintf(fp,"%d %d\n",day,TillMethod[i]);
            }
            fclose (fp);

            // Save fertilization

            sprintf(DB,"%s/Inter/CropFert_%d_%d.txt", out_path, rrr, yyy);
            db=fopen(DB, "r");
            fscanf(db, "%d", &n);
            if(n==-1)
            {
                fscanf(db, "%s", Fertigation);
            }
            else
            {
                for(i=1; i<=n; i++)
                {
                    fscanf(db, "%d %d %d", &FertMonth[i], &FertDay[i], &FertMethod[i]);
                    fscanf(db, "%f %f %f %f %f %f %f %f", &FertNitrate[i], &FertAbi[i], &FertUrea[i], &FertAnh[i], &FertAmNi[i],
                           &NH42SO4[i], &NH42HPO4[i], &FertDepth[i]);
                    fprintf(db, "%f %f %f %f %f\n", DayRelease[i], NIefficiency[i], NIduration[i],
                            UreaseEfficiency[i], UreaseDuration[i]);
                }
                fscanf(db, "%d", &FertOption);

            }
            fclose(db);

            sprintf(fname,"%s/Inter/fe-%d_%d",out_path,rrr,yyy);
            fp=sopen(fname, "w");
            if (fp == NULL) return result;

            fprintf(fp,"%d\n", n);
            if(n==-1)
            {
                fprintf(fp,"%s\n", Fertigation);
            }
            else
            {
                for (i = 1; i<=n; i++)
                {
                    day = JulianDay(FertMonth[i], FertDay[i]);

                    fprintf(fp,"%d %d %f %f %f %f %f %f %f %f\n",
                            day,
                            FertMethod[i],
                            FertDepth[i]*0.01,
                            FertNitrate[i],
                            FertAbi[i],
                            FertUrea[i],
                            FertAnh[i],
                            FertAmNi[i],
                            NH42SO4[i],
                            NH42HPO4[i]);
                    fprintf(fp,"%f %f %f %f %f\n", DayRelease[i], NIefficiency[i], NIduration[i],
                            UreaseEfficiency[i], UreaseDuration[i]);
                }
                fprintf(fp,"%d\n", FertOption);

            }
            fclose (fp);

            // Save Manuring file
            sprintf(DB,"%s/Inter/CropManu_%d_%d.txt", out_path, rrr, yyy);
            db=fopen(DB, "r");
            fscanf(db, "%d", &n);
            for(i=1; i<=n; i++)
            {
                fscanf(db, "%d %d", &ManuMonth[i], &ManuDay[i]);
                fscanf(db, "%f %f %d %d", &ManuAmount[i], &ManuCN[i], &ManuType[i], &ManuMethod[i]);
            }
            fclose(db);

            sprintf(fname,"%s/Inter/ma-%d_%d",out_path,rrr,yyy);
            fp=sopen(fname, 0);
            if (fp == NULL) return result;

            fprintf(fp,"%d\n", n);
            for (i = 1; i<=n; i++)
            {
                day = JulianDay(ManuMonth[i], ManuDay[i]);

                fprintf(fp,"%d %f %f %d %d\n",
                        day,
                        ManuAmount[i],
                        ManuCN[i],
                        ManuType[i],
                        ManuMethod[i]);
            }
            fclose (fp);

            // Save plastic file
            int WeedOption;
            sprintf(DB,"%s/Inter/CropPlastic_%d_%d.txt", out_path, rrr, yyy);
            db=fopen(DB, "r");
            fscanf(db, "%d %d", &n, &WeedOption);
            for(i=1; i<=n; i++)
            {
                fscanf(db, "%d %d", &WeedMonth1[i], &WeedDay1[i]);
                fscanf(db, "%d %d", &WeedMonth2[i], &WeedDay2[i]);
            }
            fclose(db);

            sprintf(fname,"%s/Inter/we-%d_%d",out_path,rrr,yyy);
            fp=sopen(fname, 0);
            if (fp == NULL) return result;


            fprintf(fp, "%d %d\n",n, WeedOption);
            for (i = 1; i<=n; i++)
            {
                //int day1 = JulianDay(WeedMonth1[i], WeedDay1[i]);
                //fprintf(fp,"%d\n",day1);
                //int day2 = JulianDay(WeedMonth2[i], WeedDay2[i]);
                //fprintf(fp,"%d\n",day2);
//                fprintf(fp,"%d %d\n", WeedMonth1[i], WeedDay1[i]);
//                fprintf(fp,"%d %d\n", WeedMonth2[i], WeedDay2[i]);
                int day1 = JulianDay(WeedMonth1[i], WeedDay1[i]);
                int day2 = JulianDay(WeedMonth2[i], WeedDay2[i]);
                fprintf(fp,"%d %d\n",day1, day2);
            }
            fclose (fp);

            // Save Flooding file
            int FdLeak, WaterControl;
            char WT_file[255];
            sprintf(DB,"%s/Inter/CropFloo_%d_%d.txt", out_path, rrr, yyy);
            db=fopen(DB, "r");
            fscanf(db, "%d %d %d %f", &n, &FdLeak, &WaterControl, &WaterLeakRate);
            for(i=1; i<=n; i++)
            {
                FlooID[i] = i;
                fscanf(db, "%d %d", &FlooMonth1[i], &FlooDay1[i]);
                fscanf(db, "%d %d", &FlooMonth2[i], &FlooDay2[i]);
                fscanf(db, "%f", &FlooN[i]);
                fscanf(db, "%d", &ShallowFlood[i]);
            }

            //if(WaterControl==1)
            //{
            fscanf(db, "%f", &WaterGetherIndex);
            //}

            //if(WaterControl==2)
            //{
            fscanf(db, "%s", WT_file);

            //}
            fscanf(db, "%f %f %f %f %f %f", &m_IniWT, &m_LWTceasingSurfFlow, &m_LWTceasingGroungFlow, &m_WatershedIndex,
                   &m_SurfOutflowIntensity, &m_GroundOutflowIntensity);

            fclose(db);

            sprintf(fname,"%s/Inter/fl-%d_%d",out_path,rrr,yyy);
            fp=sopen(fname, 0);
            if (fp == NULL) return result;

            fprintf(fp,"%d\n", n);

            for (i = 1; i<=n; i++)
            {
                day = JulianDay(FlooMonth1[i], FlooDay1[i]);
                fprintf(fp, "%d ",day);

                day = JulianDay(FlooMonth2[i], FlooDay2[i])+1;
                fprintf(fp, "%d\n",day);
                fprintf(fp, "%f\n", FlooN[i]);
                fprintf(fp, "%d\n", ShallowFlood[i]);
            }

            fprintf(fp, "%d %d %f\n",FdLeak, WaterControl, WaterLeakRate);
            fprintf(fp, "%f\n",WaterGetherIndex);
            fprintf(fp, "%s\n",WT_file);
            fprintf(fp, "%f %f %f %f %f %f\n",m_IniWT, m_LWTceasingSurfFlow, m_LWTceasingGroungFlow, m_WatershedIndex, m_SurfOutflowIntensity, m_GroundOutflowIntensity);
            fclose (fp);


            // Save Irrigation file
            int IrrMode;
            float IrrIndex;
            sprintf(DB,"%s/Inter/CropIrri_%d_%d.txt", out_path, rrr, yyy);
            db=fopen(DB, "r");
            fscanf(db, "%d %d %f", &n, &IrrMode, &IrrIndex);
            for(i=1; i<=n; i++)
            {
                fscanf(db, "%d %d", &IrriMonth[i], &IrriDay[i]);
                fscanf(db, "%f %d", &IrriAmount[i], &IrriMethod[i]);
            }
            fclose(db);

            sprintf(fname,"%s/Inter/ir-%d_%d",out_path,rrr,yyy);
            fp=sopen(fname, 0);
            if (fp == NULL) return result;

            fprintf(fp,"%d %f\n", n, IrrIndex);

            for (i = 1; i<=n; i++)
            {
                day = JulianDay(IrriMonth[i], IrriDay[i]);

                fprintf(fp,"%d %f %d\n", day, IrriAmount[i], IrriMethod[i]);
            }
            //fprintf(fp," %d", cycle.get(rn, cn)->IrrIndex);
            fclose (fp);


            // Save Grazing file

            sprintf(DB,"%s/Inter/CropGraz_%d_%d.txt", out_path, rrr, yyy);
            db=fopen(DB, "r");
            fscanf(db, "%d", &n);
            for(i=1; i<=n; i++)
            {



                fscanf(db, "%d %d", &GrazMonth1[i], &GrazDay1[i]);
                fscanf(db, "%d %d", &GrazMonth2[i], &GrazDay2[i]);
                fscanf(db, "%f", &GrazHour[i]);
                fscanf(db, "%f %f %f %f %f",&Dairy[i],&Beef[i],&Pig[i],&Sheep[i],&Horse[i]);
            }
            fclose(db);

            sprintf(fname,"%s/Inter/gr-%d_%d",out_path,rrr,yyy);
            fp=sopen(fname, 0);
            if (fp == NULL) return result;

            fprintf(fp, "%d\n", n);
            int bt,et;
            for (i = 1; i<=n; i++)
            {
                bt=JulianDay(GrazMonth1[i], GrazDay1[i]);
                et=JulianDay(GrazMonth2[i], GrazDay2[i]);
                fprintf(fp, "%d %d %f %f %f %f\n",bt,et,Beef[i],
                        Horse[i],Sheep[i],
                        GrazHour[i]);

            }
            fclose(fp);

            // Save garss cutting file
            sprintf(DB,"%s/Inter/GrassCut_%d_%d.txt", out_path, rrr, yyy);
            db=fopen(DB, "r");
            fscanf(db, "%d", &n);
            for(i=1; i<=n; i++)
            {
                fscanf(db, "%d %d", &CutMonth[i], &CutDay[i]);
                fscanf(db, "%f", &CutAmount[i]);
                fscanf(db, "%d", &CutPart[i]);
            }
            fclose(db);

            sprintf(fname,"%s/Inter/cut-%d_%d",out_path,rrr,yyy);
            fp=sopen(fname, 0);
            if (fp == NULL) return result;

            fprintf(fp, "%d\n", n);
            for (i = 1; i<=n; i++)
            {
                bt=JulianDay(CutMonth[i], CutDay[i]);
                fprintf(fp, "%d %f\n",bt, CutAmount[i]);
                fprintf(fp, "%d\n", CutPart[i]);
            }
            fclose(fp);
        }
    }

    //result = 1;
    return (0);
}



int ReadSoilFeature(char *mode, Site_BaseInformation IData,char *Model_Path)
{
    int result = -1;
    FILE *fpi;
    char fname[255],note[255];

    float DClay, DFldcap, DWiltpt, DSks, DPorosity;

    //IData.Soil_Texture = Soiltexture.ListIndex + 1
    if (IData.Soil_Texture<1 && IData.Soil_Texture>13) return result;
    sprintf(fname, "%s/Library/Lib_soil/soil_%d", Model_Path, IData.Soil_Texture);
    //sprintf(note,"%d",IData.Soil_Texture);
    //strcat(fname,note);
    if ((fpi=sopen(fname, 1)) == NULL)
    {
        cout << "You may have the DNDC model installed in an incorrect place. Please move the entire DNDC folder to the root directory of any drive in your computer!" << endl;
        cout << " Can not find Library Folder" << endl;
        exit(0);
    }

    fscanf(fpi,"%s %s", IData.SoilName,note);//IData.NameNote);
    fscanf(fpi,"%f %s", &DClay,note);
    fscanf(fpi,"%f %s", &DPorosity, note);//IData.StsNote);
    fscanf(fpi,"%f %s", &DSks, note);//IData.SksNote);
    fscanf(fpi,"%f %s", &DFldcap, note);//IData.FldcapNote);
    fscanf(fpi,"%f %s", &DWiltpt, note);//IData.WiltptNote);
    //fscanf(fpi,"%f %s", &IData.Soilhtcp, note);//IData.SoilhtcpNote);
    //fscanf(fpi,"%f %s", &IData.Psisat, note);//IData.PsisatNote);
    //fscanf(fpi,"%f %s", &IData.Beta, note);//IData.BetaNote);
    fclose (fpi);

    DSks *= 0.6;//cm/min ->m/hr

    if(IData.Soil_Clay==0.0) IData.Soil_Clay = DClay;
    if(IData.Fldcap==0.0) IData.Fldcap = DFldcap;
    if(IData.Wiltpt==0.0) IData.Wiltpt = DWiltpt;
    if(IData.Sks==0.0) IData.Sks = DSks;
    if(IData.Soil_Porosity==0.0) IData.Soil_Porosity = DPorosity;

    if (mode=="Default")
    {
        IData.Soil_Clay=DClay;
        IData.Fldcap=DFldcap;
        IData.Wiltpt=DWiltpt;
        IData.Sks=DSks;
        IData.Soil_Porosity=DPorosity;
    }

    result = 1;

    cout << IData.SoilName << endl;
    return result;
}


void RegionDataConversion(char *out_path, char* ConName, char *RootName, char *WholeName,int  m_TotalCells,
                          char *m_RegionName, int m_DataFormat,int m_Drought)
{
    char RP[200];

    char Pname[500];
    int i, FarmFile, ClimItem, CropSystemNumber, S_CountryCode, StartCell=1;
    int cell_ID1, cell_ID2, cell_ID3, cell_ID4, cell_ID5, cell_ID6, cell_ID7, cell_ID8, cell_ID9;
    int cell_ID10, cell_ID11, cell_ID12, country_ID;
    char KK[500], CropSystemName[160][50], CropSystemID[160][30], county_name[80];
    char climfileID[120], CountryName[120], ScenarioName[120], js[50];
    int S_ViewMode;
    float S_longitude, S_latitude, S_RainN, S_crop_ha[160];
    float Kmax_SOC[5], Kmin_SOC[5],Kmax_clay[5], Kmin_clay[5];
    float Kmax_pH[5], Kmin_pH[5], Kmax_BD[5], Kmin_BD[5];
    float max_SOC, min_SOC,	max_clay, max_pH, min_pH, max_BD, min_BD;
    int S_OnlyRice, S_SoilYear;
    float FertRate, IrriRate, SCS[50];
    int landtype, GDB, jd;
    int TotalCells, SimulatedCells;
    float CountyHa, SnowPack, FertNormalizationIndex;
    int ifdaily, landuse, ii, qq, id, column, raw, continent_ID;
    int crop_type_number, DryYear, MidYear, WetYear;
    char File1[200],File2[200],File3[200],File4[200],File5[200],File6[200],File7[200],File8[200];
    char File9[200],File10[200],File11[200],File12[200],File40[200];
    float cattle, sheep, pig, poultry, people, SlopeIndex, SoilQuality;
    char PlantHarvestDates[100], ResidueFraction[100], Tillage[100], FertDates[100], ManureRate[100];
    char TillDate[100], Flooding[100], CropPara[100];
    char CropCode[200];
    FILE *f1, *f2, *f3, *f4, *f5, *f6, *f7, *f8, *f9, *f10, *f11, *f12, *CC, *f40;
    char Fregion[200];
    FILE *region;
    FILE *fout;
    char FileOut[255];

    //int m_Drought = 1;


//    size_t pos;
//    //char str1[200];
//    string str1;
//    cout << RootName << endl;
//    //strcpy(str1,RootName);
//    str1 = string(RootName);
////    string str2 = string(str1);
////    cout << str2 << endl;
//    pos=str1.find_last_of('\\');
//    cout << pos << endl;
//    if(pos != -1)
//    {
//        str1 = str1.substr(0,pos + 1);
//    }
//
//    strcpy(RootName,str1.c_str());

    cout << RootName << endl;
    cout << ConName  << endl;


    sprintf(Pname,"%s", RootName);
    sprintf(File1, "%s/%s_ClimateSoil.txt", RootName, ConName);//location, climate and soil
    sprintf(File2, "%s/%s_CropArea.txt", RootName, ConName);//crop ha
    sprintf(File3, "%s/%s_PlantingHarvestDates.txt", RootName, ConName);//planting and harvest dates
    sprintf(File4, "%s/%s_ResidueManagement.txt", RootName, ConName);//crop residue incorporation fraction
    sprintf(File5, "%s/%s_Tillage.txt", RootName, ConName);//tilling date and depth
    sprintf(File6, "%s/%s_Fertilization.txt", RootName, ConName);//fertilizer application date, type, rate
    sprintf(File7, "%s/%s_ManureAmendment.txt", RootName, ConName);//manure application date, N and C/N
    sprintf(File8, "%s/%s_Flooding.txt", RootName, ConName);//flooding dates and method
    sprintf(File9, "%s/%s_Irrigation.txt", RootName, ConName);//irrigated percentage



    f1=fopen(File1, "r");
    if (f1==NULL) error_print(0, File1);
    f2=fopen(File2, "r");
    if (f2==NULL) error_print(0, File2);
    f3=fopen(File3, "r");
    if (f3==NULL) error_print(0, File3);
    f4=fopen(File4, "r");
    if (f4==NULL) error_print(0, File4);
    f5=fopen(File5, "r");
    if (f5==NULL) error_print(0, File5);
    f6=fopen(File6, "r");
    if (f6==NULL) error_print(0, File6);
    f7=fopen(File7, "r");
    if (f7==NULL) error_print(0, File7);
    f8=fopen(File8, "r");
    if (f8==NULL) error_print(0, File8);
    f9=fopen(File9, "r");
    if (f9==NULL) error_print(0, File9);

    //if(m_Drought==1)
    //{
    sprintf(File12, "%s/%s_CropParameter.txt", Pname, ConName);//crop max yield_TDD_WaterDEmand
    f12=fopen(File12, "r");
    if (f12==NULL) note(0, File12);
    //}



    fgets(KK, 300, f1);//ClimateSoil
    fscanf(f1, "%d", &ClimItem);
    for(i=1; i<=ClimItem; i++) fscanf(f1, "%s", js);
    fscanf(f1, "%d", &ClimItem);
    for(i=1; i<=ClimItem; i++) fscanf(f1, "%s", js);

    fgets(KK, 500, f2);//crop area
    fscanf(f2, "%d", &CropSystemNumber);
    for(i=1; i<=CropSystemNumber; i++) fscanf(f2, "%s", CropSystemName[i]);
    fscanf(f2, "%s", js);
    for(i=1; i<=CropSystemNumber; i++)
        fscanf(f2, "%s", CropSystemID[i]);

    fgets(KK, 500, f3);//planting, harvest
    fscanf(f3, "%d", &jd);
    for(i=1; i<=CropSystemNumber; i++) fscanf(f3, "%s", js);
    fscanf(f3, "%s", js);
    for(i=1; i<=CropSystemNumber; i++) fscanf(f3, "%s", js);

    fgets(KK, 500, f4);//residue management
    fscanf(f4, "%d", &jd);
    for(i=1; i<=CropSystemNumber; i++) fscanf(f4, "%s", js);
    fscanf(f4, "%s", js);
    for(i=1; i<=CropSystemNumber; i++) fscanf(f4, "%s", js);

    fgets(KK, 500, f5);//tillage
    fscanf(f5, "%d", &jd);
    for(i=1; i<=CropSystemNumber; i++) fscanf(f5, "%s", js);
    fscanf(f5, "%s", js);
    for(i=1; i<=CropSystemNumber; i++) fscanf(f5, "%s", js);

    fgets(KK, 500, f6);//fertilization
    fscanf(f6, "%d", &jd);
    for(i=1; i<=CropSystemNumber; i++) fscanf(f6, "%s", js);
    fscanf(f6, "%s", js);
    for(i=1; i<=CropSystemNumber; i++) fscanf(f6, "%s", js);

    fgets(KK, 500, f7);//manure use
    fscanf(f7, "%d", &jd);
    for(i=1; i<=CropSystemNumber; i++) fscanf(f7, "%s", js);
    fscanf(f7, "%s", js);
    for(i=1; i<=CropSystemNumber; i++) fscanf(f7, "%s", js);

    fgets(KK, 500, f8);//flooding
    fscanf(f8, "%d", &jd);
    for(i=1; i<=CropSystemNumber; i++) fscanf(f8, "%s", js);
    fscanf(f8, "%s", js);
    for(i=1; i<=CropSystemNumber; i++) fscanf(f8, "%s", js);

    fgets(KK, 500, f9);//irrigation
    fscanf(f9, "%d", &jd);
    for(i=1; i<=CropSystemNumber; i++) fscanf(f9, "%s", js);
    fscanf(f9, "%s", js);
    for(i=1; i<=CropSystemNumber; i++) fscanf(f9, "%s", js);

    //if(m_Drought==1)
    //{
    fgets(KK, 500, f12);//Crop para
    fscanf(f12, "%d", &jd);
    for(i=1; i<=CropSystemNumber; i++) fscanf(f12, "%s", js);
    fscanf(f12, "%s", js);
    for(i=1; i<=CropSystemNumber; i++) fscanf(f12, "%s", js);
    //}

    for(ii=1; ii<=m_TotalCells; ii++)
    {
        char www[200];
        int BB=(int)(100.0*(float)ii/(float)m_TotalCells);

        sprintf(FileOut, "%s/Inter/RegionFiles/%s_%d", out_path, m_RegionName, ii);//new files
        fout=fopen(FileOut, "w");
        if (fout==NULL) note(1, FileOut);

        if(m_DataFormat==1)//global database format
        {
            fscanf(f1, "%d %d %s %f %f   %s %f %f    %f %f %f %f %f %f %f %f  %d %d %d",
                   &cell_ID1, &country_ID, CountryName, &S_longitude, &S_latitude,
                   climfileID, &S_RainN, &SnowPack,
                   &Kmax_SOC[0], &Kmin_SOC[0], &Kmax_clay[0], &Kmin_clay[0], &Kmax_pH[0],  &Kmin_pH[0], &Kmax_BD[0], &Kmin_BD[0],
                   &column, &raw, &continent_ID);
        }
        else//Shangrila database format
        {
            fscanf(f1, "%d %s %s %f %f %s %f %f %f %f %f %f %f %f %f %f %f",
                   &cell_ID1, CountryName, county_name, &S_longitude, &S_latitude,
                   climfileID, &S_RainN, &Kmax_SOC[0], &Kmin_SOC[0], &Kmax_clay[0], &Kmin_clay[0],
                   &Kmax_pH[0],  &Kmin_pH[0], &Kmax_BD[0], &Kmin_BD[0], &SlopeIndex, &SoilQuality);
            if(m_Drought==1)
            {
                fscanf(f1, "%d %d %d", &DryYear, &MidYear, &WetYear);
            }
            else
                fgets(www, 200, f1);

            country_ID = 0;
            SnowPack = 0.0;
            column=0;
            raw=0;
            continent_ID=0;
        }


        fprintf(fout, "%d %d %s %f %f   %s %f %f    %f %f %f %f %f %f %f %f  %d %d %d %f %f\n",
                cell_ID1, country_ID, CountryName, S_longitude, S_latitude,
                climfileID, S_RainN, SnowPack,
                Kmax_SOC[0], Kmin_SOC[0], Kmax_clay[0], Kmin_clay[0], Kmax_pH[0],  Kmin_pH[0], Kmax_BD[0], Kmin_BD[0],
                column, raw, continent_ID, SlopeIndex, SoilQuality);
        if(m_Drought==1)
        {
            fprintf(fout, "%d %d %d\n", DryYear, MidYear, WetYear);
        }


        fprintf(fout, "%d\n", CropSystemNumber);
        for(i=1; i<=CropSystemNumber; i++) fprintf(fout, "%s ", CropSystemName[i]);
        fprintf(fout, "\n");
        fprintf(fout, "%d\n", CropSystemNumber);
        for(i=1; i<=CropSystemNumber; i++) fprintf(fout, "%s ", CropSystemID[i]);
        fprintf(fout, "\n");

        fscanf(f2, "%d", &cell_ID2);//crop area

        if(cell_ID1!=cell_ID2)
        {
            cout << "1: GIS data error for grid cell "<< cell_ID1 << " in " << File1 << endl;
            exit(0);
        }

        CountyHa=0.0;
        for(i=1; i<=CropSystemNumber; i++)
        {
            fscanf(f2, "%f", &S_crop_ha[i]);
            fprintf(fout, "%f ", S_crop_ha[i]);
            CountyHa += S_crop_ha[i];
        }
        fprintf(fout, "\n");

        /////////////////


        sprintf(CropCode,"%s/Inter/RegionCropCode", out_path);
        CC = fopen(CropCode, "w");
        if (CC==NULL) error_print(1,CropCode);

        fprintf(CC, "%d\n", CropSystemNumber);
        for(i=1; i<=CropSystemNumber; i++) fprintf(CC, "%10s ", CropSystemName[i]);
        fprintf(CC, "\n");
        for(i=1; i<=CropSystemNumber; i++) fprintf(CC, "%10s ", CropSystemID[i]);
        fprintf(CC, "\n");
        fclose(CC);


        fscanf(f3, "%d", &cell_ID3);//planting/harvest dates

        if(cell_ID1!=cell_ID3)
        {
            cout << "3: GIS data error for grid cell "<< cell_ID3 << " in " << File3 << endl;
            exit(0);
        }

        for(i=1; i<=CropSystemNumber; i++)
        {
            fscanf(f3, "%s", PlantHarvestDates);
            fprintf(fout, "%s ", PlantHarvestDates);
        }
        fprintf(fout, "\n");

        fscanf(f4, "%d", &cell_ID4);//crop residue incorporation

        if(cell_ID1!=cell_ID4)
        {
            cout << "4: GIS data error for grid cell "<< cell_ID4 << " in " << File4 << endl;
            exit(0);
        }

        for(i=1; i<=CropSystemNumber; i++)
        {
            fscanf(f4, "%s", ResidueFraction);
            fprintf(fout, "%s ", ResidueFraction);
        }
        fprintf(fout, "\n");

        fscanf(f5, "%d", &cell_ID5);//tillage

        if(cell_ID1!=cell_ID5)
        {
            cout << "5: GIS data error for grid cell "<< cell_ID5 << " in " << File5 << endl;
            exit(0);
        }

        for(i=1; i<=CropSystemNumber; i++)
        {
            fscanf(f5, "%s", Tillage);
            fprintf(fout, "%s ", Tillage);
        }
        fprintf(fout, "\n");

        fscanf(f6, "%d", &cell_ID6);

        if(cell_ID1!=cell_ID6)
        {
            cout << "6: GIS data error for grid cell "<< cell_ID6 << " in " << File6 << endl;
            exit(0);
        }

        for(i=1; i<=CropSystemNumber; i++)
        {
            fscanf(f6, "%s", FertDates);
            fprintf(fout, "%s ", FertDates);
        }
        fprintf(fout, "\n");

        fscanf(f7, "%d", &cell_ID7);

        if(cell_ID1!=cell_ID7)
        {
            cout << "7: GIS data error for grid cell "<< cell_ID7 << " in " << File7 << endl;
            exit(0);
        }
        for(i=1; i<=CropSystemNumber; i++)
        {
            fscanf(f7, "%s", ManureRate);
            fprintf(fout, "%s ", ManureRate);
        }
        fprintf(fout, "\n");

        fscanf(f8, "%d", &cell_ID8);

        if(cell_ID1!=cell_ID8)
        {
            cout << "8: GIS data error for grid cell "<< cell_ID8 << " in " << File8 << endl;
            exit(0);
        }
        for(i=1; i<=CropSystemNumber; i++)
        {
            fscanf(f8, "%s", Flooding);
            fprintf(fout, "%s ", Flooding);
        }
        fprintf(fout, "\n");

//		if(m_Drought==1)
//		{
//			fscanf(f12, "%d", &cell_ID12);
//
//			if(cell_ID1!=cell_ID12)
//			{
//				sprintf(message, "12: GIS data error for grid cell %d in %s", cell_ID12, File12);
//				exit(0);
//			}
//			for(i=1; i<=CropSystemNumber; i++)
//			{
//				fscanf(f12, "%s", CropPara);
//				fprintf(f40, "%s ", CropPara);
//			}
//			fprintf(f40, "\n");
//		}


        //if(m_Drought==1)
        //{
        fscanf(f12, "%d", &cell_ID12);

        if(cell_ID1!=cell_ID12)
        {
            cout <<  "12: GIS data error for grid cell " << cell_ID2 << " in "<<  File12 << endl;
        }
        for(i=1; i<=CropSystemNumber; i++)
        {
            fscanf(f12, "%s", CropPara);
            fprintf(fout, "%s ", CropPara);
        }
        fprintf(fout, "\n");
        //}

        fclose(fout);
    }
    fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f5);
    fclose(f6);
    fclose(f7);
    fclose(f8);
    fclose(f9);
}






void RegionDataBase(char *out_path, char* ConName, char *RootName, char *WholeName,int  m_TotalCells,
                    char *m_RegionName, int m_DataFormat)
{
    /** \brief Read Regional model data form the database
     *
     * \param out_path
     * \param ConName
     * \param RootName
     * \param WholeName
     * \param m_TotalCells
     * \param m_RegionName
     * \param m_DataFormat
     * \return
     *
     */

    char RP[200], js[200];

    char Pname[500];
    int i, FarmFile, crop_number, S_RegionCode, S_CountryCode, S_StateCode, StartCell=1;
    int cell_ID1, cell_ID2, cell_ID3, cell_ID5, cell_ID6, cell_ID7, cell_ID8, cell_ID9, cell_ID10, country_ID;
    char KK[500], S_crop_name[160][50], S_crop_ID[160][30], state_name[80], county_name[80];
    char CropTypeName[80][50], FertDates[200], ManureRate[100];
    char climfileID[120], CountryName[120], ScenarioName[120];
    float S_BaseCO2;
    int S_ThisYear, S_ViewMode;
    float S_longitude, S_latitude, S_RainN, S_crop_ha[160];
    float Kmax_SOC[5], Kmin_SOC[5],Kmax_clay[5], Kmin_clay[5];
    float Kmax_pH[5], Kmin_pH[5], Kmax_BD[5], Kmin_BD[5];
    float max_SOC, min_SOC,	max_clay, min_clay, max_pH, min_pH, max_BD, min_BD;
    int S_InterDrain, S_OnlyRice, S_NoTill, S_SoilYear, S_SimuYears;
    float ManureN, FertRate, IrriRate, SCS[50];
    int landtype, GDB;
    int TotalCells, SimulatedCells;
    float CountyHa, SnowPack, CropFertRate[100], FertNormalizationIndex;
    int ifdaily, landuse, ii, qq, jd, id, column, raw, continent_ID;
    int crop_type_number, CropTypeCode[100];
    char File1[200],File2[200],File3[200],File4[200],File5[200],File6[200],File7[200],File8[200],File9[200],File10[200];
    float cattle, sheep, pig, poultry, people;
    char CropCode[200];
    char Fregion[200];
    FILE *region;
    char Region_name[255];
    FILE *f4,*CC;


    // Total need 9 tables.
    //ClimateSoil                        //location, climate and soil
    //CropArea                           //crop ha
    //PlantingHarvestDates               //planting and harvest dates
    //ResidueManagement                  //crop residue incorporation fraction
    //Tillage                            //tilling date and depth
    //Fertilization                      //fertilizer application date, type, rate
    //ManureAmendment                    //manure application date, N and C/N
    //Flooding                           //flooding dates and method
    //Irrigation                         //irrigated percentage


    SQLiteWrapper sqlite;
    sqlite.Open("c:/test/Input_database.db");


    //sqlite.Open(DailyResult);

    string SQLCMD = "select * from BaseInfo";
    SQLiteStatement* stmt = sqlite.Statement(SQLCMD);

    if (stmt == 0)
    {
        cout<< "No data found" << endl;
        return;
    }


    Database db( "c:/test/Input_database.db" );
    Query q(db);
    Query crop_area_Q(db);
    Query Fertilizer_Q(db);
    Query Planting_harvest_Q(db);
    Query Fertilizer_dates_Q(db);
    Query Manure_Q(db);

    // count total rows of the result table
    m_TotalCells =  q.get_count("select count(*) from BaseInfo where ID > 0");

    q.get_result("select * from BaseInfo");
    cout << "Total cols " << q.num_cols() << endl;

//	while (q.fetch_row())
//	{
//		long num = atol(q.getstr(0));
//		string name = q.getstr(3);
//
//		printf("User# %ld: %s\n", num, name.c_str() );
//
//	}
//	q.free_result();



    int CroppingSystemTotal;
    char CroppingSystemName[150][50], CroppingSystemCode[150][100];


    for(ii=1; ii<=m_TotalCells; ii++)
    {
        q.fetch_row();
        // Progress percetage
        int BB=(int)(100.0*(float)ii/(float)m_TotalCells);

//        cout << stmt->DataType   (0) << " - " << stmt->DataType   (1) << " | " <<
//             stmt->ValueString(0) << " - " << stmt->ValueString(1) << endl;

        sprintf(File4, "%s/Inter/RegionFiles/%s_%d", out_path, m_RegionName, ii);//new files
        f4=fopen(File4, "w");
        if (f4==NULL) note(1, File4);


        cell_ID1 = atoi(q.getstr(0));
        // Region
        strcpy(Region_name,q.getstr(1));
        // Polygon name
        strcpy(CountryName,q.getstr(1));
        S_longitude = atof(q.getstr(3));
        S_latitude = atof(q.getstr(4));
        strcpy(climfileID,q.getstr(5));
        S_RainN = atof(q.getstr(6));
        Kmax_SOC[0] = atof(q.getstr(7));
        Kmin_SOC[0] = atof(q.getstr(8));
        Kmax_clay[0] = atof(q.getstr(9));
        Kmin_clay[0] = atof(q.getstr(10));
        Kmax_pH[0] = atof(q.getstr(11));
        Kmin_pH[0] = atof(q.getstr(12));
        Kmax_BD[0] = atof(q.getstr(13));
        Kmin_BD[0] = atof(q.getstr(14));

        country_ID = 0;
        SnowPack = 0.0;
        column=0;
        raw=0;
        continent_ID=0;

        cout << Kmax_SOC[0] << Kmin_SOC[0] << Kmax_clay[0] << Kmin_clay[0] << Kmax_pH[0] << Kmin_pH[0] << Kmax_BD[0] << Kmin_BD[0] << endl;

        fprintf(f4, "%d %d %s %f %f   %s %f %f    %f %f %f %f %f %f %f %f  %d %d %d\n",
                cell_ID1, country_ID, CountryName, S_longitude, S_latitude,
                climfileID, S_RainN, SnowPack,
                Kmax_SOC[0], Kmin_SOC[0], Kmax_clay[0], Kmin_clay[0], Kmax_pH[0],  Kmin_pH[0], Kmax_BD[0], Kmin_BD[0],
                column, raw, continent_ID);


        string CROPAREA = "select * from Crop_area";
        crop_area_Q.get_result(CROPAREA);
        crop_area_Q.fetch_row();

        crop_number = atoi(crop_area_Q.getstr(0));

        for(i=1; i<=crop_number; i++) strcpy(S_crop_name[i],crop_area_Q.getstr(i));
        crop_area_Q.fetch_row();
        for(i=1; i<=crop_number; i++) strcpy(S_crop_ID[i],crop_area_Q.getstr(i));

        fprintf(f4, "%d\n", crop_number);
        for(i=1; i<=crop_number; i++) fprintf(f4, "%s ", S_crop_name[i]);
        fprintf(f4, "\n");
        fprintf(f4, "%d\n", crop_number);
        for(i=1; i<=crop_number; i++) fprintf(f4, "%s ", S_crop_ID[i]);
        fprintf(f4, "\n");
        crop_area_Q.free_result();

        CountyHa=0.0;

        CROPAREA = "select * from Crop_area where ID = ";
        CROPAREA += int2str(cell_ID1);
        crop_area_Q.get_result(CROPAREA);
        crop_area_Q.fetch_row();

        for(i=1; i<=crop_number; i++)
        {
            S_crop_ha[i] = atof(crop_area_Q.getstr(i));
            fprintf(f4, "%8.1f ", S_crop_ha[i]);
            CountyHa += S_crop_ha[i];
        }
        fprintf(f4, "\n");
        crop_area_Q.free_result();

        /// TODO : crop code part, need new method to process.

        sprintf(CropCode,"%s/Inter/RegionCropCode", out_path);
        CC = fopen(CropCode, "w");
        if (CC==NULL) error_print(1,CropCode);

        fprintf(CC, "%d\n", crop_number);
        for(i=1; i<=crop_number; i++) fprintf(CC, "%10s ", S_crop_name[i]);
        fprintf(CC, "\n");
        for(i=1; i<=crop_number; i++) fprintf(CC, "%10s ", S_crop_ID[i]);
        fprintf(CC, "\n");
        fclose(CC);
        ///////////////


        Fertilizer_Q.get_result("select * from Fertilizer");

        Fertilizer_Q.fetch_row();
        crop_type_number = atoi(Fertilizer_Q.getstr(0));

        for(i=1; i<=crop_type_number; i++)
        {
            strcpy(CropTypeName[i],Fertilizer_Q.getstr(i));
        }
        Fertilizer_Q.fetch_row();
        for(i=1; i<=crop_type_number; i++)
        {
            CropTypeCode[i] = atoi(Fertilizer_Q.getstr(i));
        }

        Fertilizer_Q.free_result();

        fprintf(f4, "%d\n", crop_type_number);
        for(i=1; i<=crop_type_number; i++) fprintf(f4, "%s ", CropTypeName[i]);
        fprintf(f4, "\n");
        fprintf(f4, "%d\n", crop_type_number);
        for(i=1; i<=crop_type_number; i++) fprintf(f4, "%d ", CropTypeCode[i]);
        fprintf(f4, "\n");

        string FERTILIZER = "select * from Fertilizer where ID = ";
        FERTILIZER += int2str(cell_ID1);
        Fertilizer_Q.get_result(FERTILIZER);
        Fertilizer_Q.fetch_row();

        for(i=1; i<=crop_type_number; i++)
        {
            CropFertRate[CropTypeCode[i]] = atof(Fertilizer_Q.getstr(i));
            fprintf(f4, "%f ", CropFertRate[CropTypeCode[i]]);
        }
        fprintf(f4, "\n");

        Fertilizer_Q.free_result();


        string Planting_HARVEST = "select * from Planting_harvest where ID = ";
        Planting_HARVEST += int2str(cell_ID1);
        Planting_harvest_Q.get_result(Planting_HARVEST);
        Planting_harvest_Q.fetch_row();

        /// New add, in all tables, the total number of crops should be same.
        CroppingSystemTotal = crop_number;

        char PlantHarvestDates[50];
        for(i=1; i<=CroppingSystemTotal; i++)
        {
            strcpy(PlantHarvestDates,Planting_harvest_Q.getstr(i));
            fprintf(f4, "%s ", PlantHarvestDates);
        }
        fprintf(f4, "\n");
        Planting_harvest_Q.free_result();

        string Fertilizer_DATES = "select * from Fertilizer_dates where ID = ";
        Fertilizer_DATES += int2str(cell_ID1);
        Fertilizer_dates_Q.get_result(Fertilizer_DATES);

        Fertilizer_dates_Q.fetch_row();
        for(i=1; i<=crop_number; i++)
        {
            strcpy(FertDates,Fertilizer_dates_Q.getstr(i));
            fprintf(f4, "%s ", FertDates);
        }
        fprintf(f4, "\n");
        Fertilizer_dates_Q.free_result();

        //////////////////////////////////////////////////////
        string MANURE = "select * from Manure where ID = ";
        MANURE += int2str(cell_ID1);
        Manure_Q.get_result(MANURE);
        Manure_Q.fetch_row();

        for(i=1; i<=crop_number; i++)
        {
            strcpy(ManureRate,Manure_Q.getstr(i));
            fprintf(f4, "%s ", ManureRate);
        }
        fprintf(f4, "\n");
        Manure_Q.free_result();

        fclose(f4);
    }

}



void Initial_DNDC_DB(char* dbpath)
{
    /** \brief Prepare bland Database for regional daily results.
     *
     * \param dbpath The path of the database.
     * \param
     * \return
     *
     */

    string WaterSQLine;
    string CSQLine;
    string NSQLine;
    string CropSQLine;
    string GrazingSQLine;
    string EhSQLine;

    WaterSQLine = "CREATE TABLE DailyWater (ID INTEGER PRIMARY KEY AUTOINCREMENT, \
    GridID,crop_system_ID,crop_system_name,Area,Year,Day,\
    IniSoilWater,EndSoilWater,FreeWater,SoilIce,IniDeepWater,EndDeepWater,Precipitation,Irrigation,\
    Ponding,SnowPack,Evaporation,Transpiration,SurfaceWaterEV,Leaching,dSoilWater,DayInFlow,DayOutFlow)";

    // carbon table
    CSQLine = "CREATE TABLE DailyCarbon (ID INTEGER PRIMARY KEY AUTOINCREMENT, \
    GridID,crop_system_ID,crop_system_name,Area,Year,Day,\
    Very_labile_litter,Labile_litter,Resistant_litter,Microbe,\
    Humads,Humus,CharkC,SOC,dSOC,DOC,Photosynthesis,Shoot_respiration, \
    Root_respiration,Soil_heterotrophic_respiration, Eco_respiration, \
    NEE,NPP,Stub,CH4_production,CH4_oxid,CH4_flux,CH4_pool,SOC_leach)";

    // nitrogen table
    NSQLine = "CREATE TABLE DailyNitrogen (ID INTEGER PRIMARY KEY AUTOINCREMENT, \
    GridID,crop_system_ID,crop_system_name,Area,Year,Day,\
    Crop,Urea,'NH4+(0-10)','NO3-(0-10)','exchengeable_NH4(0-10)','NH4+(10-20)','NO3-(10-20)','exchengeable_NH4(10-20)','NH4+(20-30)','NO3-(20-30)',\
    'exchengeable_NH4(20-30)','NH4+(30-40)','NO3-(30-40)','exchengeable_NH4(30-40)','NH4+(40-50)','NO3-(40-50)','exchengeable_NH4(40-50)','NH3(40-50)',\
    N2O_flux,NO_flux,N2_flux,NH3_flux,N_leach,Gross_N_mineralization,N_assimilation,\
    Ice_DOC,Ice_N,Ice_N2O,Ice_N2,nitrification,denitrification,N_fixation,Litter_N)";

    // crop table
    CropSQLine = "CREATE TABLE DailyCrop (ID INTEGER PRIMARY KEY AUTOINCREMENT, \
    GridID,crop_system_ID,crop_system_name,Area,Year,Day,\
    IniSoilWater,EndSoilWater,FreeWater,SoilIce,IniDeepWater,EndDeepWater,Precipitation,Irrigation,\
    Ponding,SnowPack,Evaporation,Transpiration,SurfaceWaterEV,Leaching,dSoilWater,DayInFlow,DayOutFlow)";

    // grazing table
    GrazingSQLine = "CREATE TABLE DailyGrazing (ID INTEGER PRIMARY KEY AUTOINCREMENT, \
    GridID,crop_system_ID,crop_system_name,Area,Year,Day,\
    Grass_shoot_C,Cattle,Horse,Sheep,Hours,Grazed_C,Grazed_N,\
    Dung_C,Dung_N,Urine_N,Food_deficit,Waste_NH3)";

    //Eh table
    EhSQLine = "CREATE TABLE DailyEh (ID INTEGER PRIMARY KEY AUTOINCREMENT, \
    GridID,crop_system_ID,crop_system_name,Area,Year,Day,\
    Eh,Decom,Nitri,Denit,CH4_prod,CH4_oxid,CH4,N2O,SOC,O2,soil_N2O)";



//      for (int i=1 ;i<365;i++)
//      {
//        string tmp;
//        tmp = "Day" + int2str(i) + " int , ";
//        sqline += tmp;
//
//      }
//
//      sqline = sqline + " Day365 int)";


    //cout << sqline << endl;

//        string dbpath;
//        dbpath = outpath + "/Daily_Result.db";

    // prepare Daily results.
    SQLiteWrapper sqlite;
    sqlite.Open(dbpath);
    sqlite.Begin();
    sqlite.DirectStatement("Drop table DailyWater");
    sqlite.DirectStatement("Drop table DailyCarbon");
    sqlite.DirectStatement("Drop table DailyNitrogen");
    sqlite.DirectStatement("Drop table DailyCrop");
    sqlite.DirectStatement("Drop table DailyGrazing");
    sqlite.DirectStatement("Drop table DailyEh");
    sqlite.DirectStatement(WaterSQLine);
    sqlite.DirectStatement(CropSQLine);
    sqlite.DirectStatement(CSQLine);
    sqlite.DirectStatement(NSQLine);
    sqlite.DirectStatement(GrazingSQLine);
    sqlite.DirectStatement(CropSQLine);
    sqlite.DirectStatement(EhSQLine);
    //OperateDB(dbpath,"Drop table dailyresults");
    //OperateDB(dbpath,sqline);
    //cout << "-------------------------------------" << endl;
    sqlite.DirectStatement("VACUUM");
    sqlite.Commit();


//        while (cmd != "END")
//        {
//            cout << "Input SQL line " << endl;
//            getline(cin,sqline);
//
//            cout << sqline << endl;
//            OperateDB(dbpath,sqline);
//        }


}

