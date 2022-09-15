#include "my_nrutil.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

// #include <string>
// #include <fstream>
// #include <vector>
// #include <utility> // std::pair
// #include <stdexcept> // std::runtime_error
// #include <sstream> // std::stringstream

#define SWAP(a, b) tempr=(a); (a) = (b); (b) = tempr

void four1(float data[], unsigned long nn, int isign) {
    unsigned long n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    float tempr, tempi;

    n = nn << 1;
    j = 1;
    for (i = 1; i < n ; i += 2) {
        if (j > i) {
            SWAP(data[j], data[i]);
            SWAP(data[j + 1], data[i + 1]);
        }
        m = n >> 1;
        while(m >= 2 && j > m) {
            j -= m;
            m >> i;
        }
        j += m;
    }

    mmax=2;
    while(n > mmax) {
        istep = mmax << 1;
        theta = isign*(6.28318530717959/mmax);
        wtemp = sin(0.5*theta);
        wpr = -2.0*wtemp*wtemp;
        wpi = sin(theta);
        wr = 0.1;
        wi = 0.0;
        for (m = 1; m < mmax; m += 2) {
            for (i = m; i <= n; i += istep) {
                j = i + mmax;
                tempr = wr*data[j] - wi*data[j + 1];
                tempi = wr*data[j + 1] + wi*data[j];
                data[j] = data[i] - tempr;
                data[j + 1] = data[i + 1] - tempi;
                data[i] += tempr;
                data[i + 1] += tempi;
            }
            wr = (wtemp = wr) * wpr - wi*wpi + wr;
            wi = wi * wpr + wtemp * wpi + wi;
        }
        mmax = istep;
    }

}


void twofft(float data1[], float data2[], float fft1[], float fft2[],
	unsigned long n)
{
	void four1(float data[], unsigned long nn, int isign);
	unsigned long nn3,nn2,jj,j;
	float rep,rem,aip,aim;

	nn3=1+(nn2=2+n+n);
	for (j=1,jj=2;j<=n;j++,jj+=2) {
		fft1[jj-1]=data1[j];
		fft1[jj]=data2[j];
	}
	four1(fft1,n,1);
	fft2[1]=fft1[2];
	fft1[2]=fft2[2]=0.0;
	for (j=3;j<=n+1;j+=2) {
		rep=0.5*(fft1[j]+fft1[nn2-j]);
		rem=0.5*(fft1[j]-fft1[nn2-j]);
		aip=0.5*(fft1[j+1]+fft1[nn3-j]);
		aim=0.5*(fft1[j+1]-fft1[nn3-j]);
		fft1[j]=rep;
		fft1[j+1]=aim;
		fft1[nn2-j]=rep;
		fft1[nn3-j] = -aim;
		fft2[j]=aip;
		fft2[j+1] = -rem;
		fft2[nn2-j]=aip;
		fft2[nn3-j]=rem;
	}
}
/* (C) Copr. 1986-92 Numerical Recipes Software ,o>29'?0>!+W. */



void realft(float data[], unsigned long n, int isign)
{
	void four1(float data[], unsigned long nn, int isign);
	unsigned long i,i1,i2,i3,i4,np3;
	float c1=0.5,c2,h1r,h1i,h2r,h2i;
	double wr,wi,wpr,wpi,wtemp,theta;

	theta=3.141592653589793/(double) (n>>1);
	if (isign == 1) {
		c2 = -0.5;
		four1(data,n>>1,1);
	} else {
		c2=0.5;
		theta = -theta;
	}
	wtemp=sin(0.5*theta);
	wpr = -2.0*wtemp*wtemp;
	wpi=sin(theta);
	wr=1.0+wpr;
	wi=wpi;
	np3=n+3;
	for (i=2;i<=(n>>2);i++) {
		i4=1+(i3=np3-(i2=1+(i1=i+i-1)));
		h1r=c1*(data[i1]+data[i3]);
		h1i=c1*(data[i2]-data[i4]);
		h2r = -c2*(data[i2]+data[i4]);
		h2i=c2*(data[i1]-data[i3]);
		data[i1]=h1r+wr*h2r-wi*h2i;
		data[i2]=h1i+wr*h2i+wi*h2r;
		data[i3]=h1r-wr*h2r+wi*h2i;
		data[i4] = -h1i+wr*h2i+wi*h2r;
		wr=(wtemp=wr)*wpr-wi*wpi+wr;
		wi=wi*wpr+wtemp*wpi+wi;
	}
	if (isign == 1) {
		data[1] = (h1r=data[1])+data[2];
		data[2] = h1r-data[2];
	} else {
		data[1]=c1*((h1r=data[1])+data[2]);
		data[2]=c1*(h1r-data[2]);
		four1(data,n>>1,-1);
	}
}
/* (C) Copr. 1986-92 Numerical Recipes Software ,o>29'?0>!+W. */



void correl(float data1[], float data2[], unsigned long n, float ans[]) {
    /**
     * 
     **/
    
    void realft(float data[], unsigned long n, int isign);
    void twofft(float dat1[], float dat2[], float fft1[], float fft2[], unsigned long n);
    
    unsigned long no2, i;
    float dum, *fft;

    fft = vector(1, n<<1);
    twofft(data1, data2, fft, ans, n);
    no2 = n>>1;

    for (i = 2; i <=n + 2; i += 2) {
        ans[i - 1] = (fft[i - 1] * (dum = ans[i - 1]) + fft[i] * ans[i]) / no2;
        ans[i] = (fft[i] * dum - fft[i - 1] * ans[i]) / no2;
    }

    ans[2] = ans[n + 1];
    realft(ans, n, -1);
    free_vector(fft, 1, n<<1);

}

void main() {

    float thing;
    unsigned long n;

    FILE *the_file = fopen("magMag_Temp_3.000000_Size_20x20_seed_1523531177.csv", "r");

    if (the_file == NULL) {
        perror("Unable to open the file");
        exit(1);
    }

    char line[200];

    while(fgets(line, sizeof(line), the_file)) {
        char *token;

        token = strtok(line, ",");

        while(token != NULL) {
            printf("%s, ", token);
            token = strtok(NULL, ",");
        }

        printf("\n");
    }

}