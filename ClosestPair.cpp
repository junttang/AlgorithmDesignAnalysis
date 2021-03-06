#include <math.h>
#include <float.h>

void exchange(unsigned *x, unsigned *y) {		// swap func in quick/selectionsort
	unsigned temp;

	temp = *x;
	*x = *y;
	*y = temp;
}

void partition(									 // conquer algorithm for quicksort
	double *X, unsigned *Xid, unsigned low, 
	unsigned high, unsigned *pivotpoint
) {
	unsigned i = low;
	unsigned j = high;
	*pivotpoint = Xid[low + (high - low) / 2];	 // pivot is the elem in the middle

	do {
		while (X[Xid[i]] < X[*pivotpoint])
			i++;
		while (X[Xid[j]] > X[*pivotpoint])
			j--;
		if (i < j) {						   // when leftside elem is bigger than
			exchange(&Xid[i], &Xid[j]);			// and rightside elem is lower than
			i++; j--;							// pivotitem, then swap these elems
		}
	} while (i < j);

	*pivotpoint = j;							// update the pivotpoint as j index
}

void quicksort(double *X, unsigned *Xid, unsigned low, unsigned high) {
	unsigned pivotpoint;

	if (high > low) {
		partition(X, Xid, low, high, &pivotpoint);
		quicksort(X, Xid, low, pivotpoint);
		quicksort(X, Xid, pivotpoint + 1, high);
	}
}

void sortXid(double *X, unsigned *Xid, unsigned *TMP, unsigned N) {
	// typical quick-sort with low and high index, recursive form, and median pivot
	quicksort(X, Xid, 0, N - 1);
}

void merge(				   // merge two 'Yid' subarrays with O(n) merging algorithm
	unsigned M, unsigned L, unsigned R, double *Y,
	unsigned *Yid1, unsigned *Yid2,		// Yid, Yid1, Yid2 take same argument 'Yid'
	unsigned *Yid, unsigned *TMP	   // distinct identifiers just for readability
) {
	unsigned i = L, j = M + 1, k = L;			// Yid1 and Yid2 are same Yid array
												 // different only in indexing area
	while (i <= M && j <= R) {				
		if (Y[Yid1[i]] <= Y[Yid2[j]])			// typical linear merging algorithm
			TMP[k++] = Yid1[i++];
		else
			TMP[k++] = Yid2[j++];
	}

	if (i > M) {
		while (j <= R) 
			TMP[k++] = Yid2[j++];
	}
	else {
		while (i <= M) 
			TMP[k++] = Yid1[i++];
	}

	for (int i = L; i <= R; i++)
		Yid[i] = TMP[i];
}

void selectionsort(unsigned L, unsigned R, double *Y, unsigned *Yid) {
	unsigned min_index; // algorithm used when constructing Yid array if 'n <= THR'
	
	for (unsigned i = L; i < R; i++) {
		min_index = i;

		for (unsigned j = i + 1; j <= R; j++) {
			if (Y[Yid[j]] < Y[Yid[min_index]])
				min_index = j;
		}

		exchange(&(Yid[i]), &(Yid[min_index]));			 // swap with smallest elem
	}
}

double bruteforce(	 // search the closest pair in the given range with nested loop
	unsigned start, unsigned end,
	unsigned *pt1, unsigned *pt2, unsigned *ARR,   // ARR could be Yid or TMP array
	double temp, double *X, double *Y						// temp takes dLR or -1
) {
	double dist, dmin = temp * temp;
	if (temp == -1) dmin = DBL_MAX;				 // if this is used when 'n <= THR'

	for (unsigned i = start; i < end - 1; i++) {
		for (unsigned j = i + 1; j < end; j++) {
			if (Y[ARR[j]] - Y[ARR[i]] > dmin) break;		// this guarantees O(n)
			dist = (X[ARR[i]] - X[ARR[j]])*(X[ARR[i]] - X[ARR[j]])
				+ (Y[ARR[i]] - Y[ARR[j]])*(Y[ARR[i]] - Y[ARR[j]]);

			if (dist < dmin) {
				dmin = dist;
				*pt1 = ARR[i]; *pt2 = ARR[j];			 // update the closest pair
			}
		}
	}

	return sqrt(dmin);		   // return the euclidean distance of the closest pair 
}

double combine(	 // search the closest pair in center area if it's smaller than dLR
	unsigned M, unsigned L, unsigned R, 
	unsigned *pt1, unsigned *pt2, 	
	double dLR, double *X, double *Y,
	unsigned *Xid, unsigned *Yid, unsigned *TMP
) {
	double Xmid = (X[Xid[M]] + X[Xid[M + 1]]) / 2;	  // middle(center) point value
	double dmin = dLR;								  // dmin is initialized as dLR
	unsigned k = 1;										 // k is index of TMP array

	for (unsigned i = L; i <= R; i++) {		  // elems in center area are extracted
		if (X[Yid[i]] >= (Xmid - dLR) && X[Yid[i]] <= (Xmid + dLR))
			TMP[k++] = Yid[i];
	}  // TMP contains all candidate points in nondecreasing order by y coordinates

	dmin = bruteforce(1, k, pt1, pt2, TMP, dmin, X, Y);				   // search CP
	 
	return dmin;		 // distance of closest pair in center area or original dLR
}

double closestPairDC(
	unsigned L, unsigned R,				  // current leftmost and rightmost indices
	unsigned *pt1, unsigned *pt2,					 // closest pair points indices
	double *X, double *Y,							// (x,y) positions array(input)
	unsigned *Xid,					  // point index array(sorted by x coordinates)
	unsigned *Yid,			// point index array(gradually sorted by y coordinates)
	unsigned *TMP,											// temporal index array
	unsigned THR												 // threshold value
) {
	unsigned M;
	unsigned Lpt1, Lpt2, Rpt1, Rpt2;
	double dist, dL, dR, dLR, dmin;
	dL = dR = dLR = dmin = DBL_MAX;

	if (R - L + 1 <= THR) {							// if n is lower than threshold
		for (unsigned i = L; i <= R; i++)			 // copy Xid array to Yid array
			Yid[i] = Xid[i];

		selectionsort(L, R, Y, Yid);				   // sort Yid by y coordinates

		if (R == L) return dmin;						// if THR is 1, just return

		dmin = bruteforce(L, R + 1, pt1, pt2, Yid, -1, X, Y);		   // search CP

		return dmin;
	}

	M = (L + R) / 2;
	dL = closestPairDC(L, M, &Lpt1, &Lpt2, X, Y, Xid, Yid, TMP, THR);
	dR = closestPairDC(M + 1, R, &Rpt1, &Rpt2, X, Y, Xid, Yid, TMP, THR);
	merge(M, L, R, Y, Yid, Yid, Yid, TMP);
	if (dL < dR) { dLR = dL; *pt1 = Lpt1; *pt2 = Lpt2; }
	else { dLR = dR; *pt1 = Rpt1; *pt2 = Rpt2; }
	return combine(M, L, R, pt1, pt2, dLR, X, Y, Xid, Yid, TMP);
}

									/* [ Descriptions of Instructions Right Above ]
													 M means the middle point index
					 dL means the distance of closest pair in the left points group
					dR means the distance of closest pair in the right points group
		 'merge' is the procedure that is merging the sorted left Yid and right Yid
		the if-else syntax is determining the minimum dist and pair among 'dL'&'dR'
	   'combine' is the prodecure that is searching the closest pair in the center-
	   -area(call it dC), and comparing dC with original dLR, and finally determing
							   the closest pair in the range currently considered*/