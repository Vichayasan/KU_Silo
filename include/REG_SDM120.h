 
#define ID_SDM        1 
#define ID_PowerMeter 2
#define ID_Temp1      3
#define ID_Temp2      4
#define ID_Temp3      5
#define ID_Temp4      6
   

///////////////////////////////////////////
// Register Power Meter
 
#define v_A_B     3019
#define v_B_C     3021
#define v_C_A     3023
#define v_LL_Avg  3025
#define v_A_N      3027
#define v_B_N      3029
#define v_C_N      3031
#define v_LN_Avg   3035

#define v_Freq     3109

#define ap_Total    3059

/////
 

#define vu_AB 3037
#define vu_BC 3039
#define vu_CA 3041
#define vu_LLW 3043 // Voltage Unbalance L-L Worst

#define vu_AN 3045
#define vu_BN 3047
#define vu_CN 3049
#define vu_LNW 3051 // Voltage Unbalance L-N Worst

#define ap_A 3053
#define ap_B 3055
#define ap_C 3057
#define ap_T 3059 // Active Power Total
#define rp_A 3061
#define rp_B 3063
#define rp_C 3065
#define rp_T 3067 // Reactive Power Total
#define app_A 3069
#define app_B 3061
#define app_C 3073
#define app_T 3075 // Apparent Power Total
////

 

///////////////////////////////////////////
// Register Temp
#define pvTemp  4096



#define Reg_Volt                  0x0000      //  0.
#define Reg_Current               0x0001      //  1.
#define Reg_ActivePower           0x0002      //  2.
#define Reg_ApparentPower         0x0003      //  3.
#define Reg_ReactivePower         0x0004      //  4.
#define Reg_PowerFactor           0x0005      //  5.
#define Reg_Frequency             0x0006      //  6.
#define Reg_TotalActiveEnergy     0x0007      //  7.
#define Reg_TotalReactiveEnergy   0x0008      //  8.
#define Reg_MaxTotalPowerDemand   0x0009      //  9.
#define Reg_MaxImportPowerDemand  0x0010      //  10.
#define Reg_MaxExportPowerDemand  0x0011      //  11.
#define Reg_MaxCurDemand          0x0012      //  12.

char const Reg_ID[6] = {
  ID_SDM,
  ID_PowerMeter,
  ID_Temp1,
  ID_Temp2,
  ID_Temp3,
  ID_Temp4
  
};

uint16_t const Reg_addr[13] = {
  Reg_Volt,
  Reg_Current,
  Reg_ActivePower,
  Reg_ApparentPower,
  Reg_ReactivePower,
  Reg_PowerFactor,
  Reg_Frequency,
  Reg_TotalActiveEnergy,
  Reg_TotalReactiveEnergy,
  Reg_MaxTotalPowerDemand,
  Reg_MaxImportPowerDemand,
  Reg_MaxExportPowerDemand,
  Reg_MaxCurDemand
};

 
