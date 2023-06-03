#include <stdio.h>
#include "stdio.h"
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xstatus.h"
#include "sleep.h"

float cof_lowpassA[4]= {2,1,-1.79396184525177,0.886283112007014};
float cof_lowpassB[4]= {2,1,-1.62340569764100,0.706949765682682};
float cof_lowpassC[4]= {2,1,-1.51329076583890,0.591168074568205};
float cof_lowpassD[4]= {2,1,-1.45970625437687,0.534825984961611};

float cof_bandpassA[4]= {-1.93187813428560,1,-1.92271809793349,0.992520964225918};
float cof_bandpassB[4]= {-1.93187813428560,1,-1.92673324854103,0.992724132428440};

float cof_highpassA[4]= {-1,-2,-1.99928185537009,0.999285212575927};
float cof_highpassB[4]= {-1,-2,-1.99796244598246,0.997965800972741};
float cof_highpassC[4]= {-1,-2,-1.99695378965628,0.996957142952821};
float cof_highpassD[4]= {-1,-2,-1.99640833339892,0.996411685779522};

/*
 * @param float* x : pointer to array of input of filter data
 * @param float p_fCoef[] : array of filter coefficients. might be more corrrect to pass struct of filter instead
 * @param float* y : pointer to array of output of filter data
 */
void SecondOrderFilter(float *x, float p_fCoef[],float *y)
{
	short n=2;
	(y[n])= -(p_fCoef[0]) * (y[n-1]) - (p_fCoef[1])* (y[n-2])+(x[n])+ (p_fCoef[2])* (x[n-1]) + (p_fCoef[3])* (x[n-2]);
}

void Shiftleftdata(float *data_In, int size)
{
	for (int i = 1; i < size; i++)
			{
				data_In[i-1] = data_In[i];
			}
}

float Uselowpassfilter(float *Lowpass_In, float*Lowpass_outA,float*Lowpass_outB,float*Lowpass_outC,float*Lowpass_outD,float*Lowpass_out)
{
	SecondOrderFilter(Lowpass_In,cof_lowpassA,Lowpass_outA);
	SecondOrderFilter(Lowpass_outA,cof_lowpassB,Lowpass_outB);
	SecondOrderFilter(Lowpass_outB,cof_lowpassC,Lowpass_outC);
	SecondOrderFilter(Lowpass_outC,cof_lowpassD,Lowpass_outD);
}
