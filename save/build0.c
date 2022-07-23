#include <stdio.h>
#include <math.h>


#define	LOWER_X		105
#define UPPER_X		8023
#define	LOWER_Y		170
#define	UPPER_Y		10050


/*
 *	Return values:
 *
 *		0 = OK
 *		1 = ERROR
 */
int
resolve_triangle(xsideA,xsideB,xsideC,xanga,xangb)
double	*xsideA;
double	*xsideB;
double	*xsideC;
double	*xanga;
double	*xangb;
{
        double sideA, sideB, sideC, anga, angb, tempC;
        double pi;
        double torads;
        double todegs;
        double angc;

        pi     = fabs(acos(-1.));
        torads = pi/180.;
        todegs = 180./pi;
        angc   = 90.;

	sideA  = *xsideA;
	sideB  = *xsideB;
	sideC  = *xsideC;
	anga   = *xanga;
	angb   = *xangb;


        if(sideA && sideB && sideC) {
             tempC = sqrt(pow(sideA, 2.) + pow(sideB, 2.));
             if(fabs(sideC - tempC) > 0.001) {
                  printf("Sides invalid.\n");
                  return(1);
             }
             anga = acos(sideB/sideC) * todegs;
             angb = 90. - anga;
        } else if(sideA && sideB) {
             sideC = sqrt(pow(sideA, 2.) + pow(sideB, 2.));
             anga = acos(sideB/sideC) * todegs;
             angb = 90. - anga;
        } else if(sideB && sideC) {
             sideA = sqrt(pow(sideC, 2.) - pow(sideB, 2.));
             anga = acos(sideB/sideC) * todegs;
             angb = 90. - anga;
        } else if(sideA && sideC) {
             sideB = sqrt(pow(sideC, 2.) - pow(sideA, 2.));
             anga = acos(sideB/sideC) * todegs;
             angb = 90. - anga;
        } else if(sideA) {
             if(anga && angb) {
                  sideC = sideA/cos(angb*torads);
                  sideB = sqrt(pow(sideC, 2.) - pow(sideA, 2.));
             } else if(anga) {
                  sideC = sideA/sin(anga*torads);
                  sideB = sqrt(pow(sideC, 2.) - pow(sideA, 2.));
                  angb = 90. - anga;
             } else if(angb) {
                  sideC = sideA/cos(angb*torads);
                  sideB = sqrt(pow(sideC, 2.) - pow(sideA, 2.));
                  anga = 90. - angb;
             } else {
                  printf("Insufficient information.\n");
                  return(1);
             }
        } else if(sideB) {
             if(anga && angb) {
                  sideC = sideB/sin(angb*torads);
                  sideA = sqrt(pow(sideC, 2.) - pow(sideB, 2.));
             } else if(anga) {
                  sideC = sideB/cos(anga*torads);
                  sideA = sqrt(pow(sideC, 2.) - pow(sideB, 2.));
                  angb = 90. - anga;
             } else if(angb) {
                  sideC = sideB/sin(angb*torads);
                  sideA = sqrt(pow(sideC, 2.) - pow(sideB, 2.));
                  anga = 90. - angb;
             } else {
                  printf("Insufficient information.\n");
                  return(1);
             }
        } else if(sideC) {
             if(anga && angb) {
                  sideA = sideC * cos(angb*torads);
                  sideB = sideC * sin(angb*torads);
             } else if(anga) {
                  sideA = sideC * sin(anga*torads);
                  sideB = sideC * cos(anga*torads);
                  angb = 90. - anga;
             } else if(angb) {
                  sideA = sideC * cos(angb*torads);
                  sideB = sideC * sin(angb*torads);
                  anga = 90. - angb;
             } else {
                  printf("Insufficient information.\n");
                  return(1);
             }
        } else {
             printf("Insufficient information.\n");
             return(1);
        }

	*xsideA  = sideA;
	*xsideB  = sideB;
	*xsideC  = sideC;
	*xanga   = anga;
	*xangb   = angb;

	return(0);
}



main(argc,argv)
int	argc;
char	*argv[];
{
	int	length_y;
	int	half_length_y;
	int	midpoint_y;
	int	length_x;
	int	half_length_x;
	int	midpoint_x;
	int	my_sf;

        double sideA, sideB, sideC, anga, angb, tempC;

	double	store_anga;
	double	length_seg;
	double	mx;

	double	SQ_x1, SQ_x2, SQ_y1, SQ_y2, len_SQ;
	double	SQs_x1, SQs_x2, SQs_y1, SQs_y2, len_SQs;
	double	SQC_x, SQC_y;

	if (argc == 1)
		my_sf = 1;
	else if (argc == 2)
	{
		my_sf = atoi(argv[1]);
		if (my_sf > 5 || my_sf < 1)
		{
			fprintf(stderr,"ERROR: Scaling parm is out of range\n");
			fprintf(stderr,"Allowable range is (1..5)\n");
			exit(1);
		}
	}
	else
	{
		fprintf(stderr,"ERROR: Too many command line parameters\n");
		fprintf(stderr,"usage:  build [sc_factor (1..5)]\n");
		exit(1);
	}

/*	fprintf(stderr,"my_sf = %d\n",my_sf); */


	length_y      = UPPER_Y - LOWER_Y;
	half_length_y = length_y / 2;
	midpoint_y    = LOWER_Y + half_length_y;

	length_x      = UPPER_X - LOWER_X;
	half_length_x = length_x / 2;
	midpoint_x    = LOWER_X + half_length_x;

/*
	printf("length_y      = %d\n",length_y);
	printf("half_length_y = %d\n",half_length_y);
	printf("midpoint_y    = %d\n",midpoint_y);
	printf("length_x      = %d\n",length_x);
	printf("half_length_x = %d\n",half_length_x);
	printf("midpoint_x    = %d\n",midpoint_x);
*/

/*	sideA = (double)half_length_y; */
	sideA = (double)length_y / 5.0;
	sideB = (double)length_x;
	sideC = 0;
	anga  = 0;
	angb  = 0;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

 /*	printf("Angle a = %.2f degrees\n", anga); */

	store_anga = anga;

	sideA = 0;
	sideB = (double)half_length_x;
	sideC = 0;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);
	mx = sideA;
/*	printf("sideA = %.2f\n",sideA);
	printf("sideC = %.2f\n",sideC); */


	length_seg = sideC / 8.0;

	sideA = 0;
	sideB = 0;
	sideC = length_seg * (double)(my_sf+1);
	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);
	SQ_x1  = LOWER_X + sideB;
	SQ_x2  = UPPER_X - sideB;
	SQ_y1  = LOWER_Y + sideA;
	SQ_y2  = SQ_y1;
	len_SQ = SQ_x2 - SQ_x1;

	printf("%%1B\n");
	printf("IN;\nPA;\nSP 1;\n\n");

	printf("PU %d %d;\n",LOWER_X,UPPER_Y);
	printf("PD %d %d;\n",UPPER_X,UPPER_Y);
	printf("PD %d %d;\n",UPPER_X,LOWER_Y);
	printf("PD %d %d;\n",LOWER_X,LOWER_Y);
	printf("PD %d %d;\n",LOWER_X,UPPER_Y);

/*
	printf("PU %d %d;\n",LOWER_X,LOWER_Y);
	printf("PD %d %f;\n",midpoint_x,mx+(double)LOWER_Y);
	printf("PU %d %d;\n",UPPER_X,LOWER_Y);
	printf("PD %d %f;\n",midpoint_x,mx+(double)LOWER_Y);
*/

	printf("PU %f %f;\n",SQ_x1,SQ_y1);
	printf("PD %f %f;\n",SQ_x2,SQ_y2);
	printf("PD %f %f;\n",SQ_x2,SQ_y2+len_SQ);
	printf("PD %f %f;\n",SQ_x1,SQ_y2+len_SQ);
	printf("PD %f %f;\n",SQ_x1,SQ_y1);

	printf("PD %f %f;\n",SQ_x2,SQ_y2+len_SQ);
	printf("PU %f %f;\n",SQ_x1,SQ_y2+len_SQ);
	printf("PD %f %f;\n",SQ_x2,SQ_y2);

	SQC_x = (len_SQ / 2.0) + SQ_x1;
	SQC_y = (len_SQ / 2.0) + SQ_y1;

	printf("PU %f %f;\n",SQC_x,SQC_y);
	printf("CI %f;\n",len_SQ / 2.0);

	printf("PU %f %f;\n",SQ_x1,SQC_y);
	printf("PD %f %f;\n",SQ_x2,SQC_y);

	printf("PU %f %f;\n",SQC_x,SQ_y2);
	printf("PD %f %f;\n",SQC_x,SQ_y2+len_SQ);

	sideA = len_SQ / 2.0;
	sideB = sideA;
	sideC = 0;
	anga  = 45;
	angb  = 45;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	sideC = sideC - (len_SQ / 2.0);
	sideA = 0;
	sideB = 0;
	anga  = 45;
	angb  = 45;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	SQs_x1  = SQ_x1 + sideA;
	SQs_x2  = SQ_x2 - sideA;
	SQs_y1  = SQ_y1 + sideA;
	len_SQs = SQs_x2 - SQs_x1;

	printf("PU %f %f;\n",SQs_x1,SQs_y1);
	printf("PD %f %f;\n",SQs_x2,SQs_y1);
	printf("PD %f %f;\n",SQs_x2,SQs_y1+len_SQs);
	printf("PD %f %f;\n",SQs_x1,SQs_y1+len_SQs);
	printf("PD %f %f;\n",SQs_x1,SQs_y1);


	printf("%%1A\n\n");
}
