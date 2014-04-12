#include "stdafx.h"
#include "Dndcgo.h"
#include "Source_main.h"
#include "Dndc_tool.h"

float ANGOT;

void class_model::snowfall_snowpack()
{
 	//rainfall = precipitation;

	SnowMelt = 0.0;

    if ( precipitation > 0.0 && air_temp <= 0.0 )//air_temp
    {
		snowfall = precipitation;
		rainfall = 0.0;  //m		
		TTT = 0.0;

		//if(st[1]>=12)
		//{
		//	if(sslope>0.0) snowfall /= (1.0+sslope);
		//	else snowfall *= 1.0;
		//}
		snow_pack += snowfall; //m water
    }
    else
    {
		snowfall = 0.0;
    }

    if ( snow_pack > 0.0 && min_temp > 0.0 )//max_temp
    {	
		SnowMelt = 0.001 * min_temp;//0.004
		if ( SnowMelt > snow_pack )
		{
			SnowMelt = snow_pack; //m water
			snow_pack = 0.0;
		}
		else
		{
			snow_pack -= SnowMelt;
		}

		//rainfall += (0.5*SnowMelt);
		//TTT = rainfall / 0.005;
		runoff_pool += SnowMelt; 
    }
    else
    {
		//melt_flag = 0.0;
    }

}

void class_model::water_temperatue(void)
{
	
	int i;
	float Td, Txn, Tmn, X, AJT, Tt, Tlai=(lai[1]+lai[2]+lai[3]), MaxLAI=10.0;

	Td = (max_temp - min_temp)*0.01;
	Txn = max_temp - Td;
	Tmn = min_temp + Td;

	Tt = SatuDays/(25.0 - Tlai);

	X = (Txn - Tmn) * (MaxLAI - Tt) / MaxLAI;

	AJT = min_temp + X - 2.0;

	for(i=0; i<=1; i++) SWT[i] = SWT[i+1];
	SWT[2] = AJT;
	
	SurfaceWaterT = (SWT[0]+SWT[1]+SWT[2])/3.0;//+SWT[3]+SWT[4]+SWT[5]+SWT[6]+SWT[7]+SWT[8]+SWT[9]+SWT[10])/11.0;

	if(day_WT>0.0&&AJT<0.0) 
	{
		SurfaceWaterT = 4.0;
	}


	//SurfaceWaterT = (0.6625 * air_temp + 1.157);// / (1.0 + (lai[1]+lai[2]+lai[3]) * 0.1);
}

void class_model::soil_temperature()
{
  int l, n, qq;
  float K[120], Z[120], C[120], outQ[120], Kave, dQ;
  float Org, Min, Vwater, Vmin, Vorg, Vsoil, TM, Csoil, Ksoil, a,dQQ;
  float Mleaf, bcv1, bcv2;
 
  qq = q;//(int)(0.3/h);
  
  Mleaf = (Grain_Wt[1] + Leaf_Wt[1] + Stem_Wt[1])/.58/1000.0;//ton try matter/ha

  bcv1 = Mleaf / (Mleaf + (float)exp(7.563 - .0001297 * Mleaf));
  bcv2 = (snow_pack * 100.0 / (snow_pack * 100.0 + (float)exp(2.303 - .2197 * snow_pack * 100.0)));
  
  albedo = max(bcv1, bcv2);

  if ( jday == 1 ) OldT = air_temp;

  float adjT = (air_temp+max_temp)*0.5;
  //float adjT = air_temp;
  
  float fsl, dTT;

  //adjT = adjT / (float)pow((1.0+lai),0.2); //albedo * OldT + (1.0 - albedo) * adjT;
  //adjT = albedo * OldT + (1.0 - albedo) * adjT;

  if(snow_pack==0.0)//&&surface_litter<=2000.0)
  {
	  if(SatuDays==0)//flood_flag==0)
	  {	
		  fsl = surface_litter / 1000.0;//-0.1097*(float)log(surface_litter+0.0000001) + 1.3143;
		  fsl = min(0.5, max(0.0, fsl));

		  Surf_Temp = OldT * fsl + adjT * (1.0 - fsl);
	  }
	  else
	  {
		  fsl = 0.8;
		  
		  Surf_Temp = SurfaceWaterT;
	  }
	  

	  //if(day_WT>0.0) 
	//	  Surf_Temp = SurfaceWaterT;
	  //else
	//	  Surf_Temp = OldT * fsl + adjT * (1.0 - fsl);//min_temp * (1.0 - fsl);//

	  //Surf_Temp = min_temp;//OldT * fsl + min_temp * (1.0 - fsl);
  }
  else
  {
	  float SnowDepth = snow_pack*2000.0; //cm, snow pack thicknes
	  //Surf_Temp = 0.0 + adjT / (10.0 + SnowDepth);//
	  //Surf_Temp = max(0.0, Surf_Temp);
	  Surf_Temp = adjT / (1.0 + SnowDepth);//
	  
  } 

  float dHHH = 0.0;
  if(plastic_flag==1) 
  {
	  float TCfilm=0.25; //film thermal conductivity, 0.25 J/(s*m*K) 
	  float Dfilm=0.001; //film thickness, 0.001 m
	  float dt = 86400; //time, second
	  float SHsoil = 0.2; //soil heat capacity, 0.2 calorie/g/degree C
	  //1 calorie = 4.184 J

	  dHHH = (adjT - Surf_Temp) * Dfilm * TCfilm * dt;//J
	  float dTTT = (dHHH / 4.184) / (SHsoil * mmm[1]*1000.0);//degree C
	  Surf_Temp += dTTT;
  }
  else
  {
	  dHHH = 0.0;
  }

   
	  OldT = Surf_Temp;

  //Volumatric heat capacity, J/(layer*K), and conductivity
  for (l=1; l<=qq; l++)
  {
    /*
    if(temp[l]<=0.0 && air_temp> 0.0 && rainfall>0.0 )
    {
       int rain_layer = (int)(rainfall/0.005);
       if(l<=rain_layer) temp[l] = air_temp;
    }
    */
    
	Org = rcvl[l] + rcl[l] + rcr[l] + CRB1[l] + CRB2[l] + crhl[l]+crhr[l] + dphum[l]; //kg C/layer/ha
	if(Org<1000.0) Org = 1000.0;

    Min = mmm[l];
    Vwater = water[1][l] * 10000.0; //m water/layer -> m3 water/layer/ha
    
    Vmin = Min / (1000 * sd[l]); //volume of mineral matter, m3/layer/ha
    Vorg = Org / 0.58 / (1000 * 0.4); //volume of organic matter, m3/layer/ha
    Vsoil = Vmin + Vorg; //volume of soil, m3/layer/ha

    TM = Org + Min;

    Csoil = 4.0 * (3450000.0 * Vorg / Vsoil + 2350000.0 * Vmin / Vsoil); //J/(m3*K)
	//if(st[1]>=12) Csoil *= 2.0;

    C[l] = 0.5 * (Vsoil * Csoil + Vwater * 4180000.0); // J/(layer*K)

    //layer_Q[l] = (temp[l] + 273) * C[l];

    //Thermal conductivity, J/(cm*s*K)
	if(st[l]>=12)
		Ksoil = (0.0025 * Org / TM + 0.029 * Min / TM) * 10.0;
	else
		Ksoil = (0.0025 * Org / TM + 0.029 * Min / TM) * 10.0;
	
    K[l] = 86400.0 * ((1.0 - sts[l]) * Ksoil + sts[l] * day_wfps[l] * 0.0057);//
	   //J/(cm*day*K)

    //Depth of layer, cm
    //Z[l] = 100.0 * l * h[l]; // cm

	float Zz = 0.022;
	if(l==1) Z[l] = Zz * 100.0;
	else Z[l] = Z[l-1] + Zz * 100.0;
 
  }

  //Heat flux from layer L-1 down to L
  int WV = 200;
  
  for (n=1; n<=WV; n++)
  {
	   for (l=1; l<=qq; l++)
	   {
		   /*a = 5000.0; //5000
			if(l==1)//top layer
			{
			  float F_snow = 100.0 * snow_pack;
			  if (F_snow < 1.0) F_snow = 1.0;

			  //heat transfer between air and first layer

			  if(snow_pack > 0.0)
				outQ[0] = a * K[l] * (Surf_Temp - temp[l]) / Z[l] / F_snow;//J/(cm2*day)
			  else
				outQ[0] = a * K[l] * (Surf_Temp - temp[l]) / Z[l];
			}
			else if(l==qq)//bottom layer
			{
			   //heat from bottom layer to deeper soil
			   outQ[qq] = a * K[l] * (temp[l] - yr_avetj) / (200.0 - Z[l]);
			}
			else
			{
			   	Kave = (K[l] + K[l+1]) / 2.0;
			   //heat from layer l to l+1
			   outQ[l] = a * Kave * (temp[l] - temp[l+1]) / (Z[l+1] - Z[l]);		   
			}

			dQ = outQ[l-1] - outQ[l];

			float dT = dQ / C[l];
 
			temp[l] += dT;*/

		   a = 5000.0; //5000
			if (l==1)
			{
			  float F_snow = 100.0 * snow_pack;
			  if (F_snow < 1.0) F_snow = 1.0;

			  //heat transfer between air and first layer

			  if(snow_pack > 0.0)
				outQ[l-1] = a * K[l] * (Surf_Temp - temp[l]) / Z[l] / F_snow;//J/(cm2*day)
			  else
				outQ[l-1] = a * K[l] * (Surf_Temp - temp[l]) / Z[l];
			}

			if (l<qq)
			{
			   Kave = (K[l] + K[l+1]) / 2.0;
			   //heat from layer l to l+1
			   outQ[l] = 2000.0 * Kave * (temp[l] - temp[l+1]) / (Z[l+1] - Z[l]);
			}
			else
			{
			   //heat from bottom layer to deeper soil
			   outQ[l] = a * K[l] * (temp[l] - yr_avetj) / (200.0 - Z[l]);			   
			}

			dQ = outQ[l-1] - outQ[l];

			float dT = dQ / C[l];
 
			temp[l] += dT;

			if(Surf_Temp>=yr_avetj) temp[l] = min(Surf_Temp, max(yr_avetj, temp[l]));
			else temp[l] = min(yr_avetj, max(Surf_Temp, temp[l]));


	   } // l loop
 
  } // n

  for(l=1; l<=qq; l++)
  {
    float pQ1, pQ2, d_ice=0;
	float ps = h[l]*sts[l];
	float max_water = ps * 10000.0;//ps

    Vwater = water[1][l] * 10000.0;  // m water -> m3 water/layer/ha	

    if (temp[l]<=0.0) //freeze
    {
		if(Vwater>0.0&&ice[l]/10000.0<ps)
		{
		   pQ1 = -temp[l] * C[l]; //J/layer, heat release when T->0
		   pQ2 = Vwater * (4100000.0 - 2100000.0); //J/layer, water heat capacity

		   if (pQ1 > pQ2) //totally freeze
		   {
				dQQ = pQ2; //J/layer/ha
				temp[l] += (dQQ / C[l]);
				d_ice = Vwater; //m3 water/lay/ha
				Vwater = 0.0;

				ice[l] += d_ice;//m3 water/lay/ha
				water[1][l] = 0.0; //Vwater / 10000.0; // m water/layer

				//day_ice += d_ice/10000.0;
				//if(day_ice<0.0) day_ice = 0.0;

		   }
		   else //partially freeze
		   {
				dQQ = pQ1;
				temp[l] = 0.0;
				d_ice = dQQ / (4100000.0 - 2100000.0);

				if(d_ice > Vwater) 
					d_ice = Vwater;//m3 water/lay/ha
				else 
					d_ice = d_ice;

				Vwater -= d_ice;

				ice[l] += d_ice; //m3 water/lay/ha	
				
				water[1][l] -= (d_ice / 10000.0); // m water/layer
				//day_ice += d_ice/10000.0;// m water/layer
				//if(day_ice<0.0) day_ice = 0.0;
		   }

		   if(ice[l]/10000.0 > ps) ice[l] = ps * 10000.0;
		}
		else
		{
			d_ice = 0.0;
		}

    }
	else  //thawing
    {
		if (ice[l]>0.0&&temp[l]>0.0)
		{
		   pQ1 = temp[l] * C[l] * 0.2;//0.1;//0.05;
		   //pQ2 = 2.0 * ice[l] * (4100000.0 - 2100000.0);
		   pQ2 = ice[l] * (4100000.0 - 2100000.0);

		   if (pQ1 > pQ2) //totally thawing
		   {
			dQQ = pQ2;
			temp[l] -= dQQ / C[l];
			d_ice = ice[l];
			
			Vwater += d_ice;
			ice[l] = 0.0;
			water[1][l] += d_ice / 10000.0; // m water/layer

			//day_ice -= d_ice/10000.0;
			//if(day_ice<0.0) day_ice = 0.0;
		   }
		   else //partially thawing
		   {
			dQQ = pQ1;
			temp[l] = 0.0;
			//d_ice = -0.5 * pQ1 / (4100000.0 - 2100000.0);
			d_ice = pQ1 / (4100000.0 - 2100000.0);
			if(d_ice > ice[l]) d_ice = ice[l];
			Vwater += d_ice;
			ice[l] -= d_ice;// m3 water/layer/ha

			water[1][l] += d_ice / 10000.0; // m water/layer
			//day_ice -= d_ice/10000.0;
			//if(day_ice<0.0) day_ice = 0.0;
		   }
		   if(ice[l]>max_water) 
			   ice[l] = max_water;
		}
		else
		{ 
			d_ice = 0.0;
		}

    }	
    
	if (ice[l] < 0.0) ice[l] = 0.0;

    if (water[1][l] < 0.0) water[1][l] = 0.0;

    total_ice += (ice[l]/10000.0); //m water for profile

  }//layer loop



}

float class_model::potential_et_manure()
{	
	// Thornthwaite equation for daily PET:
	float thorni, A, TTMP, x, Fws;

    x = yr_avetj / 14.0;
    if (x < 0.0) x = 0.0;
    thorni = 30.0 * (1.0 + (float)pow(x, 2.0));
    if (thorni < 0.0) thorni = 0.0;
    A = (.49 + .0179 * thorni - .0000771 * (float)pow(thorni, 2.0) + .000000675 * (float)pow(thorni, 3.0));

    if ( air_temp >= 0.0 ) TTMP = air_temp;
    else TTMP = 0.0;
    
    x = 10.0 * TTMP / thorni;
    if (x< 0.0) x =0.0;    
    
	//Fws = 1.0 + wind_speed / 5.0;  
	//DayPET = (.015 * (1.6 * (float)pow(x, A)) / 30.0) * Fws - 0.0025;
	Fws = 1.0;// + wind_speed / 20.0; 
	if(Fws>3.0) Fws=3.0;

	DayPET = (0.015 * (1.6 * (float)pow(x, A)) / 30.0) * Fws;//0.015

	/*if(wind_speed>0.0)
	{
		float Fws = 1.0 + wind_speed / 20.0; 
		DayPET *= Fws;
	}*/

	return(DayPET);
}

float class_model::potential_et(int ETM)
{	
	//Penman-Monteith method for daily PET
	//Input data from measuremant 
		float RHmean = humidity; //measured daily mean relative humidity (%)
		float Tmean = air_temp; //measured mean daily air temperature (degree C)
		float Tmax = max_temp; //measured mean daily air temperature (degree C)
		float Tmin = min_temp; //measured mean daily air temperature (degree C)
		float u2 = wind_speed; //measured wind speed (m/second)	
		float AtmoP = 101.325; //atmospheric pressure (kPa), = 760 mmHg
		float N = Day_Hrs; //calculated maximum possible duration of sunshine or dalight hours (hours)
		float n = 0.58 * N; //actual duration of sunshine (hours)
		float z = 50.0; //elevation above sea level (m)

	//Input data from DNDC-modeled results
		float Ra = PAR; //extraterrestrial radiation, (MJ/m2/day)
		float Albedo = albedo; //measured or calculated albedo

	//Constants
		float G = 0.0; //soil heat flux density (MJ/m2/day)
		float As = 0.25; //refression constant, expressing the fraction of extraterestrial radiation reaching the earth on overcast days (n=0)
		float Bs = 0.50; //(As+Bs) = refression constant, expressing the fraction of extraterestrial radiation reaching the earth on clear days (n=N)
		double mu = 4.903 * pow(10.0,-9.0); //Stefan-Boltzmann constant (MJ/K^4/m^2/day)
		double gama = 0.000664742 * AtmoP; //psychrometric constatnt (kPa/degree C)	
		
		float delta = (4098.0 * (0.6108 * exp(17.27*Tmean/(Tmean+237.3)))) / ((Tmean+237.3)*(Tmean+237.3)); //slope vapour pressure curve (kPa/degree C)
		float E0_max = 0.6108 * exp(17.27*Tmax/(Tmax+237.3));
		float E0_min = 0.6108 * exp(17.27*Tmin/(Tmin+237.3));
		float E0_mean = 0.6108 * exp(17.27*Tmean/(Tmean+237.3));
		float Es = (E0_max + E0_min) * 0.5; //saturation vapour pressure (kPa)
		float Ea = E0_mean * RHmean / 100.0; //actual vapour pressure (kPa)
		// (Es - Ea) = saturation vapour pressure deficit (kPa)

		float KTmax = Tmax + 273.16;
		float KTmin = Tmin + 273.16;	
		
		float Rs = (As + Bs * n / N) * Ra; //measured or calculated solar or shortwave radiation (MJ/m2/day)
		float Rso = (0.75 + 0.00002 * z) * Ra;//calculated clear-sky radiation (MJ/m2/day)
		float Rns = (1.0 - Albedo) * Rs; //net solar or shortwave radiation (MJ/m2/day)
		float Rnl = mu * (pow(KTmax,4.0)+pow(KTmin,4.0))/2.0 * (0.34 - 0.14*pow(Ea,0.5)) * (1.35 * Rs/Rso - 0.35); //net outgoing longwave radiation (MJ/m2/day)
		float Rn = Rns - Rnl; //net radiation at the crop surface (MJ/m2/day)	

		float DPET = (0.408 * delta * (Rn - G) + gama * 900.0 / (Tmean + 273) * u2 * (Es-Ea)) / (delta + gama * (1.0 + 0.34 * u2));//mm/day
		
		DayPET = DPET / 1000.0; //m water/day

	if(DayPET<0.0) DayPET=0.0;
	
	if(flood_flag==1) DayPET *= 1.3;

    yr_PET += DayPET;

    
/*	/////////////////////////////////////////////////////////////////////////////////////

	//Priestley-Taylor equation for daily PET:
	//This subroutine calculates reference evapotranspiration     
	//     in a manner similar to Priestley and Taylor (1972).         
	//     To obtain crop evapotranspiration, multiplication with a    
	//     crop factor should be done.                                 
	//Refs.  : Kraalingen, D.W.G. van, W. Stol, 1997. Evapotranspiration   
	//         modules for crop growth simulation. Quantitative Approaches 
	//         in Systems Analysis No. 11. DLO Research Institute for      
	//         Agrobiology and Soil Fertility (AB-DLO), The C.T. de Wit    
	//         graduate school for Production Ecology (PE). Wageningen.    
	//         The Netherlands. 

	float LHVAP, PSCH, SIGMA, PTFAC, DATMTR;//ANGOT
    float RDLOI, RDLO, RDLII, RDLI, RDN;
    float DUMR1, DUMR2, DUMR3, DUMR4, DUMR5, DUMR6, DUMR;
	float RDD;//Daily short-wave radiation (J.m-2.d) 

	RDD = PAR * 1000000.0;

    LHVAP = 2454000.0;
	PSCH = 0.067;
    SIGMA = 0.00000005668;
	PTFAC = 1.42;

	if(RDD > 500000.0)
	{
        CString note;
		note.Format("WARNING from Potential_ET: Low short-wave radiation = %f MJ/m2/d", RDD/1000000.0);
		AfxMessageBox(note);
		//exit(0);
	} 
	
	//Vapor presure deficit
	//This subroutine calculates saturated vapour pressure and    
	//     slope of saturated vapour pressure. Parameters of the       
	//     formula were fitted on the Goff-Gratch formula used in the  
	//     Smithsonian Handbook of Meteorological Tables. The          
	//     saturated vapour following the Goff-Gratch formula is also  
	//     available as a subroutine. (Note that 1kPa = 10 mbar)       
	     
	float VPS;//Saturated vapour pressure, kPa
	float VPSL;//Slope of VPS at air_temp, kPa/C  

	if(air_temp< -20.0) air_temp = -20.0;
	if(air_temp > 50.0) air_temp = 50.0;
    VPS = 0.1 * 6.10588 * (float)exp(17.32491 * air_temp / (air_temp + 238.102));
    VPSL = 238.102 * 17.32491 * VPS / (float)pow((air_temp + 238.102),2.0);

	//Longwave radiation (J/m2/s and J/m2/d) and net radiation according to Swinbank

	SASTRO();

	DATMTR = max(0.0, min(1.0, RDD / ANGOT));

      RDLOI = SIGMA * (float)pow((air_temp + 273.16), 4.0);
      RDLII = DATMTR * (0.000000000000531 * (float)pow((air_temp + 273.16),6.0) - RDLOI) / 0.7 + RDLOI;
      RDLO = 86400.0 * RDLOI;
      RDLI = 86400.0 * RDLII;
      RDN = (1.0 - RF) * RDD + RDLI - RDLO;//RF: Reflection (=albedo) of surface (-) 

	//Priestley and Taylor reference evapotranspiration
      DayPET = PTFAC * (RDN * (VPSL / (VPSL + PSCH))) / LHVAP;
*/
/*	if(ETM!=5)//input climate data don't include relative humidity
	{
		// Thornthwaite equation
		float thorni, A, TTMP, x, Fws;

		x = yr_avetj / 14.0;
		if (x < 0.0) x = 0.0;
		thorni = 30.0 * (1.0 + (float)pow(x, 2.0));
		if (thorni < 0.0) thorni = 0.0;
		A = (.49 + .0179 * thorni - .0000771 * (float)pow(thorni, 2.0) + .000000675 * (float)pow(thorni, 3.0));

		if ( Surf_Temp >= 0.0 ) TTMP = Surf_Temp ;
		else TTMP = 0.0;
    
		x = 10.0 * TTMP / thorni;
		if (x< 0.0) x =0.0;    
    
		Fws = 1.0 + wind_speed / 5.0; 
		if(Fws>3.0) Fws=3.0;

		DayPET = (0.015 * (1.6 * (float)pow(x, A)) / 30.0) * Fws;//m water/day

	}
	else //input climate data include measured wind speed (m/s), radiation (MJ/m2/day) and relative humidity (%), 
*/

    return( DayPET );
}

void class_model::hour_loop(float hrPE0, float hrPT0, float* DayFlux, float* dby_flux, int DroughtID, 
							int YieldToday, int drc, int scale, float ha, int livestock)
{
    int   hr, l, i;
    float hrPE1, hrTrans, hrPE2, hrsnow_ev, hrPE3, dfIR;//, day_IrriWater;
    float Throufall=0.0, hrsoil_ev, dWW, Intercept, hrPonding_ev=0.0;

    dw = 0.0;
    
	day_IrriWater = IrriWater;
    for ( hr = 1; hr <= 24;  hr++ )
    {	
		//irrigation
		if(IrriWater>0.0)
		{
			if(IrriMethod==0)//furrow irrigation
			{
				dfIR = day_IrriWater / 5.0;
				surf_water += dfIR;				
			}
			else if(IrriMethod==1) //sprinkler
			{
				dfIR = day_IrriWater / 2.0;
				water[hr][5] += dfIR;							
			}
			else //drip
			{
				dfIR = day_IrriWater / 20.0;
				int DripD = int(0.05/h[1]);
				water[hr][DripD] += dfIR;
			}

			IrriWater -= dfIR;
			if(IrriWater<0.0) IrriWater = 0.0;
		}

		// Evaporation of water on leaves ///////////////////////////////////////////
		if(leaf_water > 0.0)
		{
			if ( hrPE0 * .5 < leaf_water )
			{
				leaf_water -= hrPE0 * 0.5;
				hrfol_ev = hrPE0 * 0.5;
				hrPE1 = hrPE0 * 0.5;
			}
			else
			{
				hrfol_ev = leaf_water;
				hrPE1 = hrPE0 - leaf_water;
				leaf_water = 0.0;
			}
		}
		else
		{
			hrfol_ev = 0.0;
			hrPE1 = hrPE0;
		}

		dfol_ev += hrfol_ev;

		
		// Evaporation of snow ////////////////////////////////////////////////////
		hrPE2 = hrPE1;

		evaporation_of_snow(hrPE2, &hrsnow_ev, &hrPE3);
		day_snow_ev += hrsnow_ev;

		// Evaporation of water from ponding water//////////////////////////////////
		if(day_WT >= 0.0) //WT is above ground
		{
			surf_water = 0.1;//day_WT; //m 
				
			for(int sa=1; sa<=q; sa++) 
			{
				float ps = h[sa]*sts[sa];
				water[hr][sa] = ps; //m/layer
				day_wfps[sa] = 1.0;
			}

			//flood_flag=1;
		}
		//else
		//{
			if(surf_water>0.0) 
			{
				float Ftt;
				if(air_temp>0.0) Ftt = 1.0 * air_temp/40.0;//0.1
				else Ftt = 0.0;

				Ftt = max(0.0, min(1.0, Ftt));

				if(surf_water>=hrPE3) 
				{
					hrPonding_ev = Ftt * hrPE3;					
				}
				else 
				{
					hrPonding_ev = Ftt * surf_water;					
				}
			}
			else
				hrPonding_ev = 0.0;

			hrPE3 -= hrPonding_ev;
			surf_water -= hrPonding_ev;
			
			//flood_flag=0;
		//}
		
		day_pond_ev += hrPonding_ev;
	

		//Rainfall interception by foliage and throughfall ////////////////////////
		rain_intercept(hr, &Throufall, &Intercept );

		day_intercept += Intercept;
		
		// Ponding water and bypass flux (m/hr) ///////////////////////////////////
		pond_water_bypass_flux(Throufall, scale, ha, livestock);

		//water move downward and evaporation from soil profile////////////////////////////////////////////	

		if(WaterControl==1||crop[1]==30)//||WaterControl==2
		{
			wetland_water_delivery(hr, &hrsoil_ev, DayFlux, hrPE3);
			soil_EV(hr, &hrsoil_ev, DayFlux, hrPE3);
		}
		else
		{
			if(IrriRice_flag==1||(wetland_flag==1&&flood_flag==1))
			{
				paddy_water_delivery(hr, &hrsoil_ev, DayFlux, hrPE3);
				soil_EV(hr, &hrsoil_ev, DayFlux, hrPE3);
			}
			else //if(flood_flag == 0)
			{				
					if(total_ice==0.0)
					{
						water_delivery(hr, &hrsoil_ev, DayFlux, hrPE3);
							
						soil_EV(hr, &hrsoil_ev, DayFlux, hrPE3);
					}
					else
					{
					}
				
			}			
		}
	//}
		if(hr<24) 
		{
			for ( l = 1; l <= q; l++ ) water[hr+1][l] = water[hr][l];				
		}

    } //end of hr loop

	/* Summary/conversion at end of a day */

	for ( l = 1; l <= q; l++ )
	{
		day_soil_ev += day_layer_soil_ev[l];
		day_layer_soil_ev[l] = 0.0;
		
		//float fldcapw = h[l] * sts[l] * fldcap[l];
		//if(st[1]>=12) water[24][l] = fldcapw;

		float ps = h[l]*sts[l];
		day_wfps[l] = (water[24][l] / ps);
		day_wfps[l] = min(1.0, max(0.00000001, day_wfps[l]));	
	}
 
	 if(runoff_pool>0.0)
	 {
		 float dRF;
		 if(SCSuse==0) dRF = runoff_pool * 0.5;
		 else dRF = runoff_pool * (1.0 - ManningRoughOverflow) * 0.1;

		 day_runoff += dRF;
		 runoff_pool -= dRF;
	 }
}

void class_model::hourly_transp_and_water_stress(int hr, float hrPT0, int DroughtID, int YieldToday, int drc)
{


}

void class_model::evaporation_of_snow(float hrPE2, float* hrsnow_ev, float* hrPE3 )
{

   if ( snow_pack > 0.0 && hrPE2 > 0.0 )
    {
		if ( snow_pack >= hrPE2 )
		{
			*hrsnow_ev = hrPE2;
			snow_pack -= hrPE2;
			*hrPE3 = 0.0;
		}
		else
		{
			*hrsnow_ev = snow_pack;
			*hrPE3 = hrPE2 - snow_pack;
			snow_pack = 0.0;
		}
    }
    else
    {
        *hrPE3 = hrPE2;
		*hrsnow_ev = 0.0;
    }		
}

void class_model::rain_intercept(int hr, float* Throufall, float* Intercept )
{
    float RIRI, max_leaf_water;
	float Tlai = lai[1]+lai[2]+lai[3];

    max_leaf_water = Tlai * 0.0001; //m water

    f_TTT = 1.0;

    if ( TTT > 0.0 && hr <= (int)(TTT + 1) )
    {
		RIRI = (float)(TTT * RI / ((int)(TTT + 1)));	

		*Throufall = RIRI;
		*Intercept = 0.0;
		
/*		if ( lai > 0.0 )
		{

			if ( leaf_water < max_leaf_water )
			{
				*Intercept = (0.1 * lai * RIRI);
				if ( lai > 10.0 ) *Intercept = RIRI;
				if ( (*Intercept) > max_leaf_water - leaf_water )
					*Intercept = max_leaf_water - leaf_water;

				leaf_water += *Intercept;
				*Throufall = RIRI - (*Intercept);
				f_TTT = 1.0 - leaf_water / (TTT * RI);
			}
			else
			{
				*Throufall = RIRI;
				*Intercept = 0.0;
			}

		}
		else
		{
			*Throufall = RIRI;
			*Intercept = 0.0;
		}*/
    }
    else
    {
		RIRI = 0.0;
		*Intercept = 0.0;
		*Throufall = 0.0;
    }
}

float class_model::pond_water_bypass_flux(float Throufall, int scale, float ha, int livestock)
{
	if(st[1]<12)
	{
		if(SCSuse==0)
		{
			if (Throufall>0.0&&sslope>0.0) 
			{
				//float VegeIndex = (1.0 - PresentBiomass[1]/8000.0);
				//float MoistIndex = day_wfps[1] / fldcap[1];
				float aslope = sslope / 90.0;//degree -> fraction	
				if(total_ice > 0.0) aslope = 0.9;
				if(aslope>1.0) aslope=1.0;
				
				runoff_pool += (aslope * Throufall);//m water/day
				Throufall *= (1.0 - aslope);
			}
		}
		else
		{
			if(scs_runoff>0.0)
			{
				float hr_runoff;
				
				if(scs_runoff>=Throufall) 
				{
					hr_runoff = Throufall;
					scs_runoff -= hr_runoff;
					Throufall = 0.0;
				}
				else
				{
					hr_runoff = scs_runoff;
					scs_runoff = 0.0;
					Throufall -= scs_runoff;
				}

				runoff_pool += hr_runoff;//m water/day
			}
		}
	}

    surf_water += Throufall;

	Throufall = 0.0;

    if ( surf_water > 0.0 && by_passf > 0.0 && total_ice == 0.0 && WRL>q)
    {
		//byflow = surf_water * by_passf * 8.0;
		byflow = surf_water * by_passf;
		surf_water -= byflow;
    }
    else
    {
		byflow = 0.0;
    }
    return(0 );
}

float class_model::water_delivery(int hr, float* hrsoil_ev, float* DayFlux, float hrPE3)
{
    int    l;
    float  W01, DeliWater, AvaWater=0.0; 
	float  lost_ev_water = 0.0;
	float  OutFlow = 0.0;

	if(total_ice == 0.0)
	{
		*hrsoil_ev = (float)0.0;

		for ( l = 1; l <= q; l++ )
		{	
			float ps = h[l]*sts[l];//m, porosity
			if(byflow>0.0)//with bypass flow
			{		
				W01 = (fldcap[l] - water[hr][l] / ps) / (fldcap[l] - wiltpt[l]);

				if ( W01 >= 1.0 ) DeliWater = byflow;
				if ( W01 <= 0.0 ) DeliWater = (float)0.0;
				if ( W01 > 0.0 && W01 < 1.0 ) DeliWater = 0.01 * byflow * W01;

				if(DeliWater > byflow) DeliWater = byflow;

				water[hr][l] += DeliWater;
				byflow -= DeliWater;
				if(byflow < 0.0) byflow = 0.0;

				day_bypass_influx += DeliWater;
				DeliWater = 0.0;
			}
			else
			{
				DeliWater = 0.0;
			}	  

			/* Water available for and entering in each layer */

			//afps = 1.0 - water[hr][l] / ps;//air-filled porosity
			//pswc = ps * afps; // air depth, m

			if ( l == 1 )
			{
				AvaWater = surf_water;
				surf_water = 0.0;
				day_water_in += AvaWater;

				/*if(day_water_in>rainfall) 
				{
					day_water_in=rainfall;
					AvaWater = 0.0;
					surf_water = 0.0;
				}*/
			}
			else
			{
				AvaWater = OutWater[l-1][hr];//old_lh_out_water;
			}


			water[hr][l] += AvaWater;
			AvaWater = 0.0;

			//water movement when layer is over-saturated 
			float TW = ps;//porosity, m/layer
			if (water[hr][l] > TW)
			{
				float sw = (water[hr][l] - TW);//
				float Vdid = 0.3;//0.3

				water[hr][l] = TW;

				if(q<WRL) 
				{
					if(l<q) 
						water[hr][l+1] += sw;
					else
					{
						day_leach_water += (Vdid * sw);
						WaterPool += ((1.0-Vdid) * sw);
					}
				}
				else
				{
					if(l<=WRL)
					{
						if(water[hr][l+1]<TW)//if not saturated 
							water[hr][l+1] += sw;
						else
						{
							if(l==1)
								surf_water += sw;
							else
								water[hr][l-1] += sw;
						}
					}
				}				
			}
		

			//water movement when layer water > field capacity			
			float LRA = 0.1 * (0.1439 * (float)log(sks[l]) + 1.0879);//0.9
			LRA = min(0.9, max(0.001, LRA));

			float wiltptw = h[l] * sts[l] * wiltpt[l];
			float fldcapw = h[l] * sts[l] * fldcap[l];

			if (water[hr][l]>fldcapw && WRL>l)
			{
				if(WTL>l)
				{
					if(st[1]<12)
						OutWater[l][hr] = LRA * (water[hr][l] - fldcapw);//original 0.9	
					else
						OutWater[l][hr] = 0.0;//0.01 * LRA * (water[hr][l] - fldcapw);
				}
				else//saturated layer
				{
					//OutWater[l][hr] = 0.002 * (water[hr][l] - fldcapw);
					OutWater[l][hr] = 0.0;
					water[hr][l] = ps;
				}

				if(OutWater[l][hr]<0.0) OutWater[l][hr]=0.0;		
			}
			else //if ( water[hr][l] <= fldcapw)
			{
				OutWater[l][hr] = 0.005 * (water[hr][l] - 0.1 * wiltptw);
				//OutWater[l][hr] = 0.0;
			}
		
			water[hr][l] -= OutWater[l][hr];
			if(water[hr][l]<0.0) water[hr][l] = 0.0;
			//water[hr][l+1] += OutWater[l][hr];

			//water move upward
/*			if(l>1&&(water[hr][l]>water[hr][l-1])&&(water[hr][l]>fldcapw))
			{
				float dLWT = (water[hr][l]-water[hr][l-1]) * 0.25;
				water[hr][l-1] += dLWT;
				water[hr][l] -= dLWT;
			}
*/
			//float ps = h[l]*sts[l];
			day_wfps[l] = (water[hr][l] / ps);
			day_wfps[l] = min(1.0, max(0.00000001, day_wfps[l]));

			////////////////////////////////////////////////////////////water_leaching
	
			float dDID = 5.0 * (-0.1375 *(float)log(clay[1]) + 0.0188);//0.4
			dDID = max(0.0, min(1.0, dDID));	
			float dDVD = 0.0;//0.00001

			if(flood_flag==1) 
			{
				dDID *= WaterLeakRate;
				dDVD *= WaterLeakRate;
			}

			DayFlux[l] += (OutWater[l][hr]);

			if(l==q)
			{
				if(OutWater[l][hr] > 0.0)
				{
					//float logW = OutWater[l][hr] * (1.0 - DrainEfficiency);
					//day_WT += logW;
				
					day_leach_water += (dDID * OutWater[l][hr]);
					WaterPool += ((1.0-dDID)*OutWater[l][hr]);
					if(WaterPool>WaterPoolMax)
					{
						day_leach_water += (WaterPool - WaterPoolMax);
						WaterPool=WaterPoolMax;
					}
				}

				/*if(day_WT>-WaterRetentionDepth)
				{
					float releaseW = (day_WT + WaterRetentionDepth) * DrainEfficiency * 0.00;
					day_WT -= releaseW;
				
					day_leach_water += (dDID * releaseW);
					WaterPool += ((1.0-dDID)*releaseW);
					if(WaterPool>WaterPoolMax)
					{
						day_leach_water += (WaterPool - WaterPoolMax);
						WaterPool=WaterPoolMax;
					}
				}*/
					
				if(WaterPool > 0.0001 && hr==24)
				{
					day_leach_water += (dDVD * WaterPool);
					WaterPool -= (dDVD * WaterPool);
				}
									
				if(byflow>0.0)
				{
					day_leach_water += byflow;
					byflow = 0.0;
				}

				if(day_WT>0.1) 
				{
					day_leach_water += (day_WT - 0.1);
					day_WT = 0.1;
				}
			}
				
		}//end of layer loop, l
	}//if ice = 0;

	return(0);
}

float class_model::paddy_water_delivery(int hr, float* hrsoil_ev,
		float* DayFlux, float hrPE3)
{
	float FLF;
    
	//if(flood_flag==1) FLF = 0.03 / (float)pow((1.0 + clay[1]), 4.0);
	//else FLF = 0.01 / (float)pow((1.0 + clay[1]), 4.0);
	
	float leak_water = WaterLeakRate / 24.0;

	float ps = h[1]*sts[1];
	
	if(flood_flag==1) water[hr][1] = ps;

	day_leach_water += leak_water; //paddy leachinf

    /*for ( l = 1; l <= q; l++ )
    {
	
		if(l==1) 
			day_water_in += surf_water;
	   
		OutWater[l][hr] = surf_water;

		if(flood_flag==1) 
			water[hr][l] = ps;

		DayFlux[l] += (OutWater[l][hr]);

		if (l == q) 
		{
			day_leach_water += OutWater[l][hr];
		}
			
		if (water[hr][l]>ps) water[hr][l]=ps;
    }*/

	return(0);
}

void class_model::annu_water_budget(float dby_flux)
{

	if(day_leach_water>0.0&&snow_pack>0.0)
	{
		runoff_pool += day_leach_water;
		day_leach_water = 0.0;
	}

    day_transp = act_DayAT;
    day_evapor = day_snow_ev + dfol_ev + day_pond_ev + day_soil_ev;
	//day_leach_water = day_pwater + dby_flux; //leached water, not available for plants
	day_ET = day_snow_ev + dfol_ev + day_pond_ev + day_transp + day_soil_ev;
    
	yr_prec_water += precipitation;

	//yr_water_in += day_water_in; //m water
	//day_water_in = 0.0;

	yr_bypass_in +=	day_bypass_influx;  
	day_bypass_influx = 0.0;
	
	yr_snow_ev += day_snow_ev;
    yr_fol_ev += dfol_ev;
	yr_pond_ev += day_pond_ev;

	//yr_soil_ev += day_soil_ev;
	//day_soil_ev = 0.0;

	//yr_trans += day_transp;  
	//day_transp = 0.0;

	yr_ET += day_ET;   
	yr_intercept += day_intercept;
	
}

void class_model::water_diffusion(int hr)
// Diffusion of water(Upward as positive) in unsaturated and thawing zone
// Its slower, so calculated independently layer by layer.
{
	int L, L1, Bottom_Type, First, Next_Layer;
	float A, SW[100], Water_Diff[100], THET1, THET2, DBAR, Depth2;
	float Water_Flux[100], Bottom_Depth=1.0;

/*  float SW[MaxL], Water_Diff[MaxL],
	THET1, THET2, DBAR, Depth2, A, F;
  int L, L1, Next_Layer, First, Last, Bottom_Type;*/

//Bottom_Type =1 for frozen layer, =2 for water table, 3: neither
  Bottom_Type=3;
  L1 = q-1;
  A=0.0;
  for(L=0; L<q; L++)  
  {
     A += h[L];//??? h[0]
     /*if(temp[L]<0.0) 
	 {
		Bottom_Type=1;
		L1=L-1;
		L=q;   //exit loop
     }
     else if (L>WRL) 
	 {
		Bottom_Type=2;
		L1=L;
		if(L1>q-1) L1=q-1;
		L=q;   //exit loop
     }*/
  }

  /*if(Bottom_Type==3)  
  {
     L1=q-1;
	 F=(water[hr][L1]-fldcapw)*h;
     water[hr][L1]=fldcapw;
     Water_Table-=F/(Porosity[L1]-Field_Capacity[L1]);
     if(Water_Table>Bottom_Depth) Water_Table=Bottom_Depth;
  }*/

  for(L=1; L<=q; L++) 
  {
     Water_Diff[L]=0.0;
     SW[L]=water[hr][L];
  }

// estimating diffusivity DBAR.
  First =1;
  for(L=First; L<q; L++)  
  {
	 float wiltptw = h[L] * sts[L] * wiltpt[L];
	 float fldcapw = h[L] * sts[L] * fldcap[L];
     Next_Layer=L+1;
     THET1=water[hr][L]-wiltptw;
     if (THET1 < 0.0)  THET1=0.0;
     THET2=water[hr][Next_Layer]-wiltptw;
     if (THET2 < 0.0)  THET2=0.0;
     //DBAR=0.88*exp(35.4*(THET1+THET2)*0.5);
	 DBAR=0.00001*exp(35.4*(THET1+THET2)*0.5);
     if (DBAR >100.0)  DBAR=100.0;
     Depth2 = 2.0 * h[L];
     Water_Diff[L]=DBAR*(THET2-THET1)/(Depth2/2.0);
// Duing diffusion, the water content in the receiving layer
// cannot be higher than the average of the two layers.
   /*  A=(water[hr][L]*h + water[hr][Next_Layer]*h)/Depth2;
     if(Water_Diff[L] > 0.0) 
	 {
       F = (A-water[hr][L]) *h;
       Water_Diff[L] = min(Water_Diff[L], F);
     }
     else 
	 {
       F = (water[hr][Next_Layer]-A) *h;
       Water_Diff[L] = max(Water_Diff[L], F);
     }*/

     SW[L] += Water_Diff[L]/h[L];
     SW[Next_Layer] -= Water_Diff[L]/h[L];
  }  //end for loop

 /* if(Bottom_Type==2) 
  {    //water table in CN layers
     Drain_W +=(SW[L1]-fldcapw)*h;
     if(SW[L1]<fldcapw)  
	 {
       F=fldcapw-SW[L1];
       SW[L1]=fldcapw;
       Water_Table +=F/(sts*h-fldcapw);
     }
     else  
	 {
       Water_Table=L1*h;
       F=(SW[L1]-fldcapw)/(sts*h-fldcapw)*h;
       Water_Table-=F;
     }
  }   //end bottom_type=2
  else if(Bottom_Type==3) 
  {
       F=(SW[L1]-fldcapw)*h;
       Water_Table -= F/(sts*h-fldcapw);
       if(Water_Table>Bottom_Depth) Water_Table=Bottom_Depth;
       Drain_W += F;
       SW[L1]=fldcapw;
  }*/

  for(L=0; L<=L1; L++)  water[hr][L] = SW[L];

  for(L=First; L<L1; L++)  
  {
     A = - Water_Diff[L];
     Water_Flux[L] += A;
     Next_Layer = L + 1;
     //N_Mass_Flow(Next_Layer, A);
  }

}


float class_model::wetland_water_delivery(int hr, float* hrsoil_ev,	float* DayFlux, float hrPE3)
{
    int    l;
    float  W01, DeliWater, AvaWater=0.0; //OutWater
	float  lost_ev_water = 0.0;
	float  OutFlow = 0.0;
	float TravelT;

	//WTlayer = (int)(-day_WT/h);
   
	if(total_ice == 0.0)
	{    
		for ( l = 1; l <= q; l++ )
		{
			float ps = h[l]*sts[l];			
			//bypass flow
			if(byflow>0.0&&l<WTL)
			{		
				W01 = (fldcap[l] - water[hr][l] / ps) / (fldcap[l] - wiltpt[l]);
				if ( W01 >= 1.0 ) DeliWater = byflow;
				if ( W01 <= 0.0 ) DeliWater = (float)0.0;
				if ( W01 > 0.0 && W01 < 1.0 )

				DeliWater = .01 * byflow * W01;

				if(DeliWater > byflow) DeliWater = byflow;

				water[hr][l] += DeliWater;
				byflow -= DeliWater;
				if(byflow < 0.0) byflow = 0.0;

				day_bypass_influx += DeliWater;
				DeliWater = 0.0;
			}
			else
			{
				DeliWater = 0.0;
			}	  

			if(l<WTL)
			{
				if ( l == 1 )
				{
					AvaWater = surf_water;
					surf_water = 0.0;
					day_water_in += AvaWater;
				}
				else
				{
					AvaWater = OutWater[l-1][hr];//old_lh_out_water;
				}

				water[hr][l] += AvaWater;

				AvaWater = 0.0;

				if (water[hr][l] > ps)
				{

				   float sw = (water[hr][l] - ps);			   
				   water[hr][l] = ps;
				   surf_water += sw;			   
				   if(l==1) day_water_in -= sw;			  
				   sw = 0.0;

				}

				float travelt = 20.0 * (h[l] * sts[l] - h[l] * sts[l] * fldcap[l]) / sks[l];	// hr/layer
				TravelT = 0.5 * pow(clay[l],1.9188)  * travelt;

				float wiltptw = h[l] * sts[l] * wiltpt[l];
				float fldcapw = h[l] * sts[l] * fldcap[l];

				if (water[hr][l] > fldcapw)
				{					
					OutWater[l][hr] = 0.1 * (water[hr][l] - fldcapw) * (1.0 - .9 * (float)exp(-1.0 / TravelT));
						 
					water[hr][l] -= OutWater[l][hr]; 
					//water[hr][l+1] += OutWater[l][hr]; 
					//OutWater[l][hr] = 0.0;
				}
				else 
				{
					OutWater[l][hr] = 0.0;
				}
			}
			else//l>=WTL
			{
				water[hr][l] = ps;
			}		
		}//end of layer loop
	}//if ice>0
		
	
	{
		day_leach_water += (WaterLeakRate / 24.0);//wetland leaching
	}
			
	return(0);
}

float class_model::soil_EV(int hr, float* hrsoil_ev, float* DayFlux, float hrPE3)
{
	float limit1, limit2, sev, asev, fsl, residue_volume;

	int EVL=(int)(0.2/h[1]);
	*hrsoil_ev = 0.0;

	if(EVL>WTL) EVL=WTL;

	fsl = 1.0;

	surface_litter = rcvl[1]+rcl[1]+rcr[1]+stub1+stub2+stub3;//kg C/ha
	residue_volume = surface_litter / 100.0 /10000.0; //cubic m/square m
	if(surface_litter>0.0) 
	{
		fsl = -0.99 - 0.236 * (float)log(residue_volume);// based on J.L. Steiner, 1989, SSSAJ 53:911-916
		if(fsl>1.0) fsl=1.0;
	}
	else
		fsl = 1.0;

	float  tev, zz;

	for(int l=1; l<=EVL; l++)
	{
		sev = hrPE3 * (float)pow(0.5, l);

/*		if ( l<= (int)(EVL / h) )
		{
			if ( jday==1 && l==1 ) old_tev=0.0;

			zz = l * h;       
			if ( l == 1 ) old_tev = (float)0.0;

			tev = fsl*(hrPE3 * ((zz / EVL) / (zz / EVL+ (float)exp(-2.92 - 1.43 * zz /EVL))));

			sev = tev - old_tev;
			old_tev = tev;

		}
		else
		{
			sev = 0.0;
		}
*/
		if(FilmCoverFraction>0.0) //plastic film mulching
		{
			limit1 = (1.0 - FilmCoverFraction);
		}
		else if(irri_flag==1&&IrriMethod==0) 
			limit1 = 2.0;
		else 
			limit1 = 1.0;

		float wiltptw = h[l] * sts[l] * wiltpt[l];
		float fldcapw = h[l] * sts[l] * fldcap[l];

		if ( water[hr][l] < fldcapw )
		{
			limit2 = ((water[hr][l] - 0.0) / (fldcapw - 0.0));
			if (limit2 < 0.0) limit2 = 0.0;
			limit2 = (float)pow(limit2, qstar);
		}
		else
		{
			limit2 = 1.0;
		}

		float limit = limit1 * limit2 * fsl;
		limit = min(1.0, max(0.0, limit));

		if(st[1]>=12&&day_WT>0.0) limit *= 0.01;

		asev = sev * limit;

		if ( asev < 0.0 ) asev = 0.0;

		if (by_passf > 0.0)
		{
		   asev += (0.0001 * water[hr][l]);
		}

		//float wiltptw = h[l] * sts[l] * wiltpt[l];
		//float fldcapw = h[l] * sts[l] * fldcap[l];

		if (asev <= 0.0005 * wiltptw) asev = 0.0;

		day_layer_soil_ev[l] += asev;

		if(water[hr][l]>asev)
			water[hr][l] -= asev;
		else
		{
			asev = water[hr][l];
			water[hr][l] = 0.0;
		}

		*hrsoil_ev += asev;

		//float fldcapw = h[l] * sts[l] * fldcap[l];
		if(st[1]>=12&&day_WT>0.0) water[hr][l] = fldcapw;
	}

	
	return(0);
}




