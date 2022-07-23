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

int
get_intersect(x1,y1,x2,y2,xx1,yy1,xx2,yy2,ret_x,ret_y)
double	x1;
double	y1;
double	x2;
double	y2;
double	xx1;
double	yy1;
double	xx2;
double	yy2;
double	*ret_x;
double	*ret_y;
{
	double	xlk;
	double	ylk;
	double	rsq;
	double	rinv;
	double	A1, B1, C1;
	double	A2, B2, C2;
	double	det;
	double	dinv;

	xlk  = x2 - x1;
	ylk  = y2 - y1;
	rsq  = (xlk * xlk) + (ylk * ylk);
	rinv = 1.0 / (sqrt(rsq));

	A1 = -(ylk) * rinv;
	B1 = xlk * rinv;
	C1 = ((x1 * y2) - (x2 * y1)) * rinv;

	xlk  = xx2 - xx1;
	ylk  = yy2 - yy1;
	rsq  = (xlk * xlk) + (ylk * ylk);
	rinv = 1.0 / (sqrt(rsq));

	A2 = -(ylk) * rinv;
	B2 = xlk * rinv;
	C2 = ((xx1 * yy2) - (xx2 * yy1)) * rinv;

	det  = (A1*B2) - (A2*B1);
	dinv = 1.0 / det;

	*ret_x = ((B1*C2) - (B2*C1)) * dinv;
	*ret_y = ((A2*C1) - (A1*C2)) * dinv;
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
	int	first;

        double sideA, sideB, sideC, anga, angb, tempC;

	double	store_anga;
	double	length_seg;
	double	mx;
	double	dx;
	double	ang_x0, ang_x1, ang_x2;
	double	adjust;

	double	SQ_x1, SQ_x2, SQ_y1, SQ_y2, len_SQ;
	double	SQs_x1, SQs_x2, SQs_y1, SQs_y2, len_SQs;
	double	SQC_x, SQC_y;
	double	SQsC_x, SQsC_y;
	double	try1;
	double	increment;
	double	JR1_x, JR1_y;
	double	JR2_x, JR2_y;
	double	JR3_x, JR3_y;
	double	JR4_x, JR4_y;
	double	JR5_x, JR5_y;
	double	JR6_x, JR6_y;
	double	JR7_x, JR7_y;
	double	JR8_x, JR8_y;
	double	JR9_x, JR9_y;
	double	JRA_x, JRA_y;
	double	JRB_x, JRB_y;
	double	JRC_x, JRC_y;
	double	JRD_x, JRD_y;
	double	tmp1_x, tmp1_y;
	double	tmp2_x, tmp2_y;

	first = 0;

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


	length_seg = sideC / 10.0;

	sideA = 0;
	sideB = 0;
	sideC = length_seg * (double)(my_sf+2);
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

	printf("PW .10;\n");

again:

	printf("PU %f %f;\n",SQ_x1,SQ_y1);
	printf("PD %f %f;\n",SQ_x2,SQ_y2);
	printf("PD %f %f;\n",SQ_x2,SQ_y2+len_SQ);
	printf("PD %f %f;\n",SQ_x1,SQ_y2+len_SQ);
	printf("PD %f %f;\n",SQ_x1,SQ_y1);

	printf("LA 3 2;\n");

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

	sideC = len_SQ / 4.0;
	sideA = 0;
	sideB = 0;
	anga  = 45;
	angb  = 45;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	SQsC_x = SQC_x - sideA;
	SQsC_y = SQC_y + sideA;

	printf("PU %f %f;\n",SQsC_x,SQsC_y);
	printf("CI %f;\n",sideA);

	printf("PU %f %f;\n",SQs_x1,SQs_y1);
	printf("PD %f %f;\n",SQs_x1 - (len_SQs / 4.0),SQs_y1 + (len_SQs / 4.0));
	printf("PD %f %f;\n",SQs_x1,SQs_y1 + (len_SQs / 4.0));
	printf("PD %f %f;\n",SQs_x1 + (len_SQs / 4.0),SQs_y1);
	printf("PD %f %f;\n",SQs_x1 + (len_SQs / 2.0),SQs_y1 + (len_SQs / 4.0));
	printf("PD %f %f;\n",SQs_x1 + (len_SQs / 4.0),SQs_y1 + (len_SQs / 2.0));
	printf("PD %f %f;\n",SQs_x1,SQs_y1 + (len_SQs / 4.0));
	printf("PU %f %f;\n",SQs_x1 - (len_SQs / 4.0),SQs_y1 + (len_SQs / 4.0));
	printf("PD %f %f;\n",SQC_x,SQC_y + (len_SQs / 2.0));

	printf("PU %f %f;\n",SQs_x1 + (len_SQs / 2.0),SQs_y1 + (len_SQs / 4.0));
	printf("PD %f %f;\n",SQs_x1 + (len_SQs / 4.0),SQs_y1 + (len_SQs / 4.0));
	printf("PD %f %f;\n",SQs_x1,SQs_y1 + (len_SQs / 2.0));

	dx = (len_SQs / 4.0) + ((len_SQs / 4.0) * .35);

	printf("PU %f %f;\n",SQC_x - dx,SQC_y);
	printf("PD %f %f;\n",SQC_x - dx,SQC_y+dx);
	printf("PD %f %f;\n",SQC_x,SQC_y+dx);


	JRB_x = SQC_x - dx;
	JRB_y = SQC_y + dx;


	printf("PU %f %f;\n",SQC_x,SQ_y1);
	printf("PD %f %f;\n",SQs_x2,SQs_y1+len_SQs);

	sideC = 0;
	sideA = len_SQs / 2.0;
	sideB = SQs_y1-SQ_y1 + len_SQs;
	anga  = 0;
	angb  = 0;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	ang_x0 = anga;
	ang_x1 = angb;

	adjust = 0.0;

incr:

	increment = len_SQs / 8.0 - adjust;

	try1 = (len_SQs / 2.0) + SQs_y1 + increment;

/*
	printf("PU %f %f;\n",SQC_x,try1);
	printf("PD %f %f;\n",SQC_x + (len_SQs / 2.0),try1);
*/

	sideC = 0;
	sideA = 0;
	sideB = try1 - SQ_y1;
	anga  = ang_x0;
	angb  = ang_x1;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	JR1_x = SQC_x + sideA;
	JR1_y = SQC_y + increment;

/*
	printf("PU %f %f;\n",JR1_x,JR1_y);
	printf("PU %f %f;\n",JR1_x,JR1_y);
	printf("CI %f;\n",50.0);
*/

	sideC = 0;
	sideA = (SQC_y + (len_SQs / 2.0)) - JR1_y;
	sideB = JR1_x - SQC_x;
	anga  = 0;
	angb  = 0;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	sideC = 0;
	sideA = 0;
	sideB = len_SQs / 2.0;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	JR2_x = SQC_x + (len_SQs / 2.0);
	JR2_y = SQC_y + (len_SQs / 2.0) - sideA;
/*
	printf("PU %f %f;\n",JR2_x,JR2_y);
	printf("CI %f;\n",50.0);
	printf("PU %f %f;\n",JR2_x,JR2_y);
	printf("PD %f %f;\n",SQC_x,JR2_y);
*/

	sideC = 0;
	sideA = 0;
	sideB = SQC_y + (len_SQs / 2.0) - JR1_y;
	anga  = 45;
	angb  = 45;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	JR3_x = SQC_x + (len_SQs / 2.0) - sideA;
	JR3_y = JR1_y;
/*
	printf("PU %f %f;\n",JR3_x,JR3_y);
	printf("CI %f;\n",50.0);
*/

	sideC = 0;
	sideA = SQC_y - JR2_y;
	sideB = 0;
	anga  = 45;
	angb  = 45;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	JR4_x = SQC_x + sideB;
	JR4_y = JR2_y;
/*
	printf("PU %f %f;\n",JR4_x,JR4_y);
	printf("CI %f;\n",50.0);
*/

	sideC = 0;
	sideA = (len_SQs / 2.0) - increment;
	sideB = JR3_x - SQC_x;
	anga  = 0;
	angb  = 0;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	ang_x2 = anga;

	sideC = 0;
	sideA = increment;
	sideB = increment;
	anga  = 45;
	angb  = 45;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	sideA = sideC;
	sideC = 0;
	sideB = 0;
	angb  = 180.0 - 45.0 - ang_x2;
	anga  = 180.0 - 90.0 - angb;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	sideC = sideB;
	sideA = 0;
	sideB = 0;
	angb  = 45;
	anga  = 45;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	JR5_x = SQC_x + sideA;
	JR5_y = SQC_y - sideB;

/*
	printf("PU %f %f;\n",JR5_x,JR5_y);
	printf("CI %f;\n",50.0);

	printf("PU %f %f;\n",JR5_x,JR5_y);
	printf("PD %f %f;\n",SQC_x,SQC_y + (len_SQs / 2.0));
	printf("PD %f %f;\n",JR2_x,JR2_y);

	printf("PU %f %f;\n",JR4_x,JR4_y);
	printf("PU %f %f;\n",JR5_x,JR5_y);
*/
	if ((JR4_x - JR5_x) < 1.0)
	{
		adjust = adjust + .1;
		goto incr;
	}

	printf("PU %f %f;\n",JR1_x,JR1_y);
	printf("PD %f %f;\n",JR3_x,JR3_y);

	printf("PU %f %f;\n",JR2_x,JR2_y);
	printf("PD %f %f;\n",JR5_x,JR5_y);
	printf("PD %f %f;\n",SQC_x,SQC_y + (len_SQs / 2.0));
	printf("PD %f %f;\n",JR2_x,JR2_y);
	printf("PD %f %f;\n",SQC_x,SQ_y1);
	printf("PD %f %f;\n",JR5_x,JR5_y);


	get_intersect(SQ_x2,SQ_y1,SQC_x,SQC_y,SQC_x,SQ_y1,JR2_x,JR2_y,
					&JR6_x,&JR6_y);

	get_intersect(SQC_x,SQ_y1,JR5_x,JR5_y,SQC_x,JR6_y,JR6_x,JR6_y,
					&JR7_x,&JR7_y);

	printf("PU %f %f;\n",JR6_x,JR6_y);
	printf("PD %f %f;\n",JR7_x,JR7_y);

	get_intersect(SQC_x,SQ_y1,JR2_x,SQC_y + (len_SQs/2.0),
		JR2_x,JR2_y,JR5_x,JR5_y, &tmp1_x,&tmp1_y);

	get_intersect(SQC_x,SQ_y1,JR2_x,SQC_y + (len_SQs/2.0),
		SQC_x,SQC_y,JR2_x,SQC_y, &tmp2_x,&tmp2_y);


	sideC = 0;
	sideA = tmp2_x - tmp1_x;
	sideB = tmp2_y - tmp1_y;
	angb  = 0;
	anga  = 0;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	sideC = sideC / 4.0;
	sideA = 0;
	sideB = 0;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	JR8_x = tmp1_x + sideA;
	JR8_y = tmp1_y + sideB;

	printf("PU %f %f;\n",JR6_x,JR6_y);
	printf("PD %f %f;\n",JR8_x,JR8_y);

	sideC = 0;
	sideA = SQC_y - JR8_y;
	sideB = 0;
	angb  = 45;
	anga  = 45;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	JR9_x = SQC_x + (sideB / 2.0);
	JR9_y = JR8_y;

	printf("PD %f %f;\n",JR9_x,JR9_y);
	printf("PD %f %f;\n",JR7_x,JR7_y);
	printf("PU %f %f;\n",JR9_x,JR9_y);
	printf("PD %f %f;\n",JR3_x,JR3_y);

	get_intersect(SQC_x,SQC_y,JR2_x,SQC_y + (len_SQs/2.0),
		SQC_x,SQC_y+(len_SQs/2.0),JR2_x,JR2_y, &JRA_x,&JRA_y);

	tmp1_x = ((len_SQs / 2.0) * .55) + SQC_x;
	tmp1_y = SQC_y + (len_SQs / 2.0);

	printf("PU %f %f;\n",JRA_x,JRA_y);
	printf("PD %f %f;\n",tmp1_x,tmp1_y);

/*	printf("PU %f %f;\n",JRB_x,JRB_y);
	printf("PD %f %f;\n",SQC_x,SQC_y + (len_SQs / 2.0)); */

	sideC = 0;
	sideA = SQC_x - JRB_x;
	sideB = SQC_y + (len_SQs / 2.0) - JRB_y;
	angb  = 0;
	anga  = 0;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	sideC = 0;
	sideA = sideA * .4;
	sideB = 0;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	JRC_x = JRB_x + sideA;
	JRC_y = JRB_y + sideB + (sideB * .5);

/*	printf("PU %f %f;\n",JRC_x,JRC_y);
	printf("PD %f %f;\n",JRC_x,JRB_y); */


	sideC = 0;
	sideA = SQC_x - JRB_x;
	sideB = SQC_y + (len_SQs / 2.0) - JRB_y;
	angb  = 0;
	anga  = 0;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	sideC = 0;
	sideA = sideA * .6;
	sideB = 0;

	resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

	JRD_x = JRB_x + sideA;
	JRD_y = JRB_y + sideB + (sideB * .4);


	printf("PU %f %f;\n",SQC_x,SQC_y + (len_SQs / 2.0));
	printf("PD;\n");
/*
	printf("PD %f %f;\n",JRD_x,JRD_y);
	printf("PD %f %f;\n",JRC_x,JRC_y);
	printf("PD %f %f;\n",JRB_x,JRB_y);
*/

	printf("BZ %f %f %f %f %f %f;\n",JRD_x, JRD_y, JRC_x,JRC_y,JRB_x,JRB_y);


	if (first++ == 0)
	{
		SQ_x1  = SQ_x1;
		SQ_x2  = SQ_x2;
		SQ_y1  = SQsC_y;
		SQ_y2  = SQ_y1;
		len_SQ = len_SQ;
		printf("PW 1.0;\n");
		goto again;
	}

	printf("%%1A\n\n");
}
