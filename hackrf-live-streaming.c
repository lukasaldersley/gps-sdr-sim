#if 0
gcc gpssim.c -D LIVE -lm -O3 -o gps-sdr-sim
gcc $0 -o hackrf-live-streaming
./hackrf-live-streaming
exit
#endif

/*ATTENTION: this wil ONLY work on Linux!!!
This is basically a shell script, but I didn't want to deal with the Day of Year calculations in bash.

you can compile this program by simply running this file (as if is was a script).
The preprocessor instructions at the very top of the file will handle compilation and the first execution for you.
This works because of the different comment schemes in bash and c


The check if the return value of the system() call was 256 at cmd3, cmd5, cmd9, cmdB and cmdC is because the error code 256 means,
the file wasn't there in te first place or has already been deleted, therefore the program can continue*/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>



int day_of_year(int a, int b, int c);

int main(){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	int mt=tm.tm_mon+1;
	int dy=tm.tm_mday;
	int yr=tm.tm_year+1900;
	int doy=0;
	int mth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if ( (yr % 4) || (yr % 100) && (yr % 400) ){
		mth[3]++;
	}
	for (int i = 0; i < mt; i++)
	{
		doy += mth[i];
	}
	doy+=dy;

	char cmd0[]="wget ftp://cddis.nasa.gov/gnss/data/hourly/----/---/hour---0.--n.Z";
	//char cmd1[]="wget https://celestrak.com/GPS/almanac/SEM/almanac.sem.txt";
	char cmd2[]="uncompress hour---0.--n.Z -f";
	char cmd3[]="rm hour---0.--n.Z";
	//char cmd4[]="mv almanac.sem.txt current.al3";
	char cmd5[]="rm gpsstream";
	char cmd6[]="mkfifo gpsstream";
	char cmd7[]="hackrf_transfer -t ./gpsstream -f 1575420000 -s 2600000 -a 1 -x 0 &";
	char cmd8[]="./gps-sdr-sim -e hour---0.--n -l 48.195952, 11.564827,503 -b 8 -t $(date -u +%Y/%m/%d,%H:%M:%S) -i -o gpsstream";
	char cmd9[]="rm gpsstream";
	char cmdA[]="sleep 1";
	char cmdB[]="rm hour---0.--n";
	char cmdC[]="rm current.al3";

	char doy1=((char)((doy%10)+48));
	char doy10=((doy<10)?'0':((char)((((doy%100)-((doy%100)%10))/10)+48)));
	char doy100=((doy<100)?'0':((char)(((doy-(doy%100))/100)+48)));
	char y1=((char)((yr%10)+48));
	char y10=((char)((((yr-yr%10)/10)%100)+48));
	char y100=((char)((((yr-yr%100)/100)%10)+48));
	char y1000=((char)((((yr-yr%1000)/1000))+48));

	cmd0[43]=y1000;
	cmd0[44]=y100;
	cmd0[45]=cmd0[61]=cmd2[20]=cmd3[12]=cmd8[26]=cmdB[12]=y10;
	cmd0[46]=cmd0[62]=cmd2[21]=cmd3[13]=cmd8[27]=cmdB[13]=y1;
	cmd0[48]=cmd0[56]=cmd2[15]=cmd3[7]=cmd8[21]=cmdB[7]=doy100;
	cmd0[49]=cmd0[57]=cmd2[16]=cmd3[8]=cmd8[22]=cmdB[8]=doy10;
	cmd0[50]=cmd0[58]=cmd2[17]=cmd3[9]=cmd8[23]=cmdB[9]=doy1;

	//printf("following commands will be executed:\r\n\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n\r\n",cmd0,cmd1,cmd2,cmd3,cmd4,cmd5,cmd6,cmd7,cmd8,cmd9,cmdA,cmdB,cmdC);
	printf("\r\nfollowing commands will be executed:\r\n\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n%s\r\n\r\n",cmd0,cmd2,cmd3,cmd5,cmd6,cmd7,cmd8,cmd9,cmdA,cmdB,cmdC);

	int rv=0;
	if(rv=system(cmd0)){
		printf("CMD0 failed with code %d, ABORTING\r\n",rv);
		return rv;
	}
	/*if(rv=system(cmd1)){
		printf("CMD1 failed with code %d, ABORTING\r\n",rv);
		return rv;
	}*/
	if(rv=system(cmd2)){
		printf("CMD2 failed with code %d, ABORTING\r\n",rv);
		return rv;
	}
	if(rv=system(cmd3)){
		printf("CMD3 failed with code %d,",rv);
		if(rv!=256)
		{
			printf("ABORTING\r\n");
			return rv;
		}
		printf("ACCEPTING AND CONTINUING\r\n");
	}
	/*if(rv=system(cmd4)){
		printf("CMD4 failed with code %d, ABORTING\r\n",rv);
		return rv;
	}*/
	for(int i=0;i<2;i++){
		if(rv=system(cmd5)){
			printf("CMD5 failed with code %d,",rv);
			if(rv!=256)
			{
				printf("ABORTING\r\n");
				return rv;
			}
			printf("ACCEPTING AND CONTINUING\r\n");
		}
		if(rv=system(cmd6)){
			printf("CMD6 failed with code %d, ABORTING\r\n",rv);
			return rv;
		}
		if(rv=system(cmd7)){
			printf("CMD7 failed with code %d, ABORTING\r\n",rv);
			return rv;
		}
		if(rv=system(cmd8)){
			printf("CMD8 failed with code %d, ABORTING\r\n",rv);
			return rv;
		}
		if(rv=system(cmd9)){
			printf("CMD9 failed with code %d,",rv);
			if(rv!=256)
			{
				printf("ABORTING\r\n");
				return rv;
			}
			printf("ACCEPTING AND CONTINUING\r\n");
		}
		if(rv=system(cmdA)){
			printf("CMDA failed with code %d, IGNORING\r\n",rv);
		}
	}
	if(rv=system(cmdB)){
		printf("CMDB failed with code %d,",rv);
		if(rv!=256)
		{
			printf("ABORTING\r\n");
			return rv;
		}
		printf("ACCEPTING AND CONTINUING\r\n");
	}
	if(rv=system(cmdC)){
		printf("CMDB failed with code %d,",rv);
		if(rv!=256)
		{
			printf("ABORTING\r\n");
			return rv;
		}
		printf("ACCEPTING AND CONTINUING\r\n");
	}
	return 0;
}
