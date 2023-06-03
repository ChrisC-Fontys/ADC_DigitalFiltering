/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "stdio.h"
#include "platform.h"
#include "xil_printf.h"
#include "xadcps.h"
#include "xparameters.h"
#include "xstatus.h"
#include "sleep.h"
#include "ADCcode.h"
#include "xgpio.h"
#include "Filters.h"
#include "stdlib.h"
/************************** Constant Definitions ****************************/

#define XADC_DEVICE_ID 		XPAR_XADCPS_0_DEVICE_ID
#define XGPIO_DEVICE_ID		XPAR_XGPIO_0_DEVICE_ID

#define SAMPLE_FREQUENCY 1200 //sample frequency is in Hz

/***************** Macros (Inline Functions) Definitions ********************/

#define printf xil_printf /* Small foot-print printf function */

/************************** Function Prototypes *****************************/

XGpio dac;

/************************** Variable Definitions ****************************/
//u16 RawADCdata;

/****************************************************************************/
/**************************        MAIN         *****************************/
/****************************************************************************/

//int arrayA[]={2,3,4,1,3};
//int arrayB[]={4,5,1,2,2};
//int result[5];
int sum;

int main()
{
	// first we make all the array's for the filters and set them to zero
	float *outputlowpassA, *outputlowpassB,*outputlowpassC,*outputlowpassD,*resultlowpass;

	outputlowpassA = (float*)calloc(3,sizeof(float));
	outputlowpassB = (float*)calloc(3,sizeof(float));
	outputlowpassC = (float*)calloc(3,sizeof(float));
	outputlowpassD = (float*)calloc(3,sizeof(float));
	resultlowpass = (float*)calloc(3,sizeof(float));

	//array of all filter coefficients
	float cof_lowpassA[4]= {2,1,-1.79396184525177,0.886283112007014};
	//float *outputnotchA, *outputnotchB,*resultnotch = (float*)calloc(3,sizeof(float));
	//float *outputhighpassA, *outputhighpassB,*outputhighpassC,*outputhighpassD,*resulthighpass = (float*)calloc(3,sizeof(float));
	// them we make a array for the ECG
	float arrayECG[3];
	init_platform();
	print("Starting program...\n\r");

    // configure the XADC
	int Status;
    Status = XAdcConfig(XADC_DEVICE_ID);
    if (Status != XST_SUCCESS) {
    	print("configuring XADC unsuccessful\n\r");
    	return XST_FAILURE;
    }

    //XGpio_Initialize(&dac, XPAR_AXI_GPIO_0_DEVICE_ID);
    //XGpio_SetDataDirection(&dac, 1, 0x0);

    while(1){
    	//shift the ECG data before putting new data into it
    	Shiftleftdata(arrayECG, 3);
    	// get data from the XADC depending on the sampling frequency
    	arrayECG[2] = XAdcGeTSampledValue(SAMPLE_FREQUENCY);

    	// print the voltage
    	//printf("%0d.%03d Volts.\n\r", (int)(Voltagedata), XAdcFractionToInt(Voltagedata));


    	// now we filter the data of the ECG

    	// first we apply a 8th order lowpass filter with a steeper cutoff at ...Hz
    	// Cascade the 8th order lowpass filter into four 2nd order lowpass filters
    	//outputlowpassA[n]= -2*outputlowpassA[n-1] - outputlowpassA[n-2] + arrayECG[n] - 1.79396184525177*arrayECG[n-1] + 0.886283112007014*arrayECG[n-2];
    	//outputlowpassB[n]= -2*outputlowpassB[n-1] - outputlowpassB[n-2] + arrayECG[n] - 1.62340569764100*arrayECG[n-1] + 0.706949765682682*arrayECG[n-2];
    	//outputlowpassC[n]= -2*outputlowpassC[n-1] - outputlowpassC[n-2] + arrayECG[n] - 1.51329076583890*arrayECG[n-1] + 0.591168074568205*arrayECG[n-2];
    	//outputlowpassD[n]= -2*outputlowpassD[n-1] - outputlowpassD[n-2] + arrayECG[n] - 1.45970625437687*arrayECG[n-1] + 0.534825984961611*arrayECG[n-2];

    	//calculate the result
    	//resultlowpass[2] = (outputlowpassA[2]) * (outputlowpassB[2]) * (outputlowpassC[2]) * (outputlowpassD[2]);

    	// get the output data into their respective array

    	SecondOrderFilter(arrayECG, cof_lowpassA, outputlowpassA);
    	Shiftleftdata(outputlowpassA,3);
    	//Shiftleftdata(outputlowpassB,3);
    	//Shiftleftdata(outputlowpassC,3);
    	//Shiftleftdata(outputlowpassD,3);

    	//Secondly we apply a 4th order notch filter from 49 to 51 Hz to suppress 50Hz
    	// Cascade the 4th order notch filter into two 2nd order notch filters
    	//outputnotchA[n]= 1.93187813428560*outputnotchA[n-1] - outputnotchA[n-2] +arrayECG[n]-1.92271809793349*arrayECG[n-1] +0.992520964225918*arrayECG[n-2];
    	//outputnotchB[n]= 1.93187813428560*outputnotchB[n-1] - outputnotchB[n-2] +arrayECG[n]-1.92673324854103*arrayECG[n-1] +0.992724132428440*arrayECG[n-2];

    	// calculate the result
    	//resultnotch[2] = outputnotchA[2] *outputnotchB[2];

    	//get the output data into their respective array
    	//Shiftleftdata(outputnotchA,3);
    	//Shiftleftdata(outputnotchB,3);

    	// shift the data in resultlowpass
    	Shiftleftdata(resultlowpass,3);

    	// Lastly we apply a high pass filter at ...Hz to suppress small muscle movements
    	// Cascade the 8th order Highpass filter into four 2nd order lowpass filters
    	//outputhighpassA[n]= outputhighpassA[n-1] + 2*outputhighpassA[n-2] + arrayECG[n] - 1.99928185537009*arrayECG[n-1] + 0.999285212575927*arrayECG[n-2];
    	//outputhighpassB[n]= outputhighpassB[n-1] + 2*outputhighpassB[n-2] + arrayECG[n] - 1.99796244598246*arrayECG[n-1] + 0.997965800972741*arrayECG[n-2];
    	//outputhighpassC[n]= outputhighpassC[n-1] + 2*outputhighpassC[n-2] + arrayECG[n] - 1.99695378965628*arrayECG[n-1] + 0.996957142952821*arrayECG[n-2];
    	//outputhighpassD[n]= outputhighpassD[n-1] + 2*outputhighpassD[n-2] + arrayECG[n] - 1.99640833339892*arrayECG[n-1] + 0.996411685779522*arrayECG[n-2];

    	//calculate the result
    	//resulthighpass[2] = outputhighpassA[2] * outputhighpassB[2] * outputhighpassC[2] *outputhighpassD[2];

    	// get the output data into their respective array
    	//Shiftleftdata(outputhighpassA,3);
    	//Shiftleftdata(outputhighpassB,3);
    	//Shiftleftdata(outputhighpassC,3);
    	//Shiftleftdata(outputhighpassD,3);

    	// shift the data in resultnotch & resulthighpass
    	//Shiftleftdata(resultnotch,3);
    	//Shiftleftdata(resulthighpass,3);

    	// after filtering, we can calculate BPM

    	//if (n == 2){n=0};
	//RawADCdata = rawdata(VCCPintData_Sampled);

    //XGpio_DiscreteWrite(&dac, 1, RawADCdata<<4);

    // apply filters
    	/*
    		- steps to apply the filters:
    		Step 1: get values from adc
    		step 2: save the last 2 input values in a array (kind of a static moving array)
    		step 3: calculate the output of the filter
    		step 4: save the last 2 output values from filter in a array
    	*/

    	//8th order lowpass filter cascade 2nd order part A
    	//y[n]= -2*y[n-1] - y[n-2] +x[n] -1.79396184525177*x[n-1] +0.886283112007014*x[n-2];
    	//cascade 2nd order part B
    	//y[n]= -2*y[n-1] - y[n-2] +x[n] -1.62340569764100*x[n-1] +0.706949765682682*x[n-2];
    	//cascade 2nd order part C
    	//y[n]= -2*y[n-1] - y[n-2] +x[n] -1.51329076583890*x[n-1] +0.591168074568205*x[n-2];
    	//cascade 2nd order part D
    	//y[n]= -2*y[n-1] - y[n-2] +x[n] -1.45970625437687*x[n-1] +0.534825984961611*x[n-2];

    	//4th order notch filter cascade 2nd order part A
    	//y[n]= 1.93187813428560*y[n-1] - y[n-2] +x[n]-1.92271809793349*x[n-1] +0.992520964225918*x[n-2];
    	//cascade 2nd order part B
    	//y[n]= 1.93187813428560*y[n-1] - y[n-2] +x[n]-1.92673324854103*x[n-1] +0.992724132428440*x[n-2];

    	//8th order High pass filter cascade 2nd order part A
    	//y[n]= y[n-1] 2*y[n-2] +x[n] -1.99928185537009*x[n-1] +0.999285212575927*x[n-2];
   	   	//cascade 2nd order part B
   	   	//y[n]= y[n-1] 2*y[n-2] +x[n] -1.99796244598246*x[n-1] +0.997965800972741*x[n-2];
   	   	//cascade 2nd order part C
   	   	//y[n]= y[n-1] 2*y[n-2] +x[n] -1.99695378965628*x[n-1] +0.996957142952821*x[n-2];
       	//cascade 2nd order part D
       	//y[n]= y[n-1] 2*y[n-2] +x[n] -1.99640833339892*x[n-1] +0.996411685779522*x[n-2]];

    //		print("Starting program...\n\r");
    //		for (int i = 0; i<=4; i++)
    //		{
    //			result[i] = arrayA[i] * arrayB[i];
    //			sum = result[i]+sum;
    //		}
    //		printf("%d\r\n",sum);
    //		sum =0;

    // add periodic detection

    // add piek detection (BPM calculation)

	}

    print("Program finished \n\r");
    free(outputlowpassA);
    free(outputlowpassB);
    free(outputlowpassC);
    free(outputlowpassD);
    free(resultlowpass);
    cleanup_platform();
    return XST_SUCCESS;
}
