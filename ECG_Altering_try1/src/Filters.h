#ifndef SRC_FILTERS_H_
#define SRC_FILTERS_H_



#endif /* SRC_FILTERS_H_ */


void SecondOrderFilter(float *x, float p_fCoef[],float *y);
void Shiftleftdata(float data_In[], int size);
/*
struct filterCoefficients
{
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
};*/
