/*
 * Filters.c
 *
 *  Created on: 2 jun. 2023
 *      Author: Chris
 */

float outputArray[3] = {0, 0, 0};

float SecondOrderFilter( float dataIn[3], float p_fCoef[2])
{
	int n = sizeof(dataIn);
	outputArray[n]= -2*outputArray[n-1] - outputArray[n-2] + dataIn[n] - p_fCoef[1]*dataIn[n-1] + p_fCoef[2]*dataIn[n-2];

	return outputArray[n];
}
//outputlowpassA[n]= -2*outputlowpassA[n-1] - outputlowpassA[n-2] + arrayECG[n] - p_fCoef[1]*arrayECG[n-1] + p_fCoef[2]*arrayECG[n-2];


void shiftArray(float arr[])
{
	int size = sizeof(arr);

	for (int i = 0; i < size-1; i++) {
		arr[i] = arr[i + 1];
	}
}
