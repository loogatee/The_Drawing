#include <stdio.h>
#include <math.h>

/*
 *    These defines are for various parameters associated with
 *    the building of the ellipse figure.  I interpolated about
 *    20 times in order to come up with these figures.  See the
 *    code comments for how these constants are used.
 */
#define EVAR1        .835
#define EVAR2        .575
#define EVAR3        .238
#define EVAR4        .972
#define EVAR5        .504

/*
 *    This number needs to be a multiple of 3.  This is the number
 *    of segments to calculate for the ellipse.  The ellipse is
 *    built with HPGL    BZ commands, with each BZ command having 3
 *    coordinate pairs as parameters.  Thus the requirement for
 *    having this define be a multiple of 3.
 */
#define    NUM_POINTS    201

/*
 *    These numbers represent the outermost HPGL coordinates 
 *    of a page.  That is, the figure uses these defines as
 *    the X,Y range for the page:
 *
 *        lower left  = LOWER_X,LOWER_Y
 *        lower right = UPPER_X,LOWER_Y
 *        upper left  = LOWER_X,UPPER_Y
 *        upper right = UPPER_X,UPPER_Y
 */
#define    LOWER_X        105
#define    UPPER_X        8023
#define    LOWER_Y        170
#define    UPPER_Y        10050

/*
 *    Define this if you want the figure to be colored.
 *    Comment it out if you want the figure to be b&w.
 */
#define DO_COLOR

/*
 *    This function is taken from the following source:
 *        Manual:  Programming on HP-UX
 *        Chapter: 10, Standard Math Routines
 *        Section: Trigonometric Functions
 *        Name:    triangle.c
 *
 *    Its purpose is to resolve the dimensions and angles of a right
 *    triangle.
 *
 *    Return values:
 *        0 = OK
 *        1 = ERROR
 *
 *
 *                /|
 *               / |
 *              /  |
 *             / a |
 *            /    |
 *          C/     |B
 *          /      |
 *         /       |
 *        /        |
 *       / b     c_|
 *      /________|_|
 *            A
 */
int
resolve_triangle(xsideA,xsideB,xsideC,xanga,xangb)
double    *xsideA;
double    *xsideB;
double    *xsideC;
double    *xanga;
double    *xangb;
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

/*
 *    The basic algorithm for this function is taken from the
 *    following source:
 *
 *        Title:   A Programmer's Geometry
 *        Authors: Adrian Bowyer, John Woodwark
 *        Section: 1.5
 *        Name:    Intersection of Two lines
 *
 *    Given the X,Y coordinate pairs for 2 lines (2 coordinate
 *    pairs define each line), this function returns the X,Y
 *    coordinate pair of the intersection of the 2 lines.
 */
int
get_intersect(x1,y1,x2,y2,xx1,yy1,xx2,yy2,ret_x,ret_y)
double    x1, y1;
double    x2, y2;
double    xx1, yy1;
double    xx2, yy2;
double    *ret_x, *ret_y;
{
    double    xlk, ylk;
    double    rsq, rinv;
    double    A1, B1, C1;
    double    A2, B2, C2;
    double    det, dinv;

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
int    argc;
char    *argv[];
{
    int    length_y;
    int    midpoint_y;
    int    my_sf;
    int    first;

    double sideA, sideB, sideC, anga, angb, tempC;

    double    total_rect_len;
    double    dx;
    double    ang_x0, ang_x1, ang_x2;
    double    adjust;
    double    pen_width_1;
    double    pen_width_2;

    double    LSQ0_x, LSQ0_y;
    double    LSQ1_x, LSQ1_y;
    double    LSQ2_x, LSQ2_y;
    double    LSQ3_x, LSQ3_y;

    double    SQ_x1, SQ_x2, SQ_y1, SQ_y2, len_SQ;
    double    SQs_x1, SQs_x2, SQs_y1, SQs_y2, len_SQs;
    double    SQC_x, SQC_y;
    double    SQsC_x, SQsC_y;
    double    try1;
    double    increment;
    double    JR1_x, JR1_y;
    double    JR2_x, JR2_y;
    double    JR3_x, JR3_y;
    double    JR4_x, JR4_y;
    double    JR5_x, JR5_y;
    double    JR6_x, JR6_y;
    double    JR7_x, JR7_y;
    double    JR8_x, JR8_y;
    double    JR9_x, JR9_y;
    double    JRA_x, JRA_y;
    double    JRB_x, JRB_y;
    double    JRC_x, JRC_y;
    double    JRD_x, JRD_y;
    double    JRE_x, JRE_y;
    double    JRF_x, JRF_y;
    double    tmp1_x, tmp1_y;
    double    tmp2_x, tmp2_y;
    double    PE1_x, PE1_y;
    double    PE2_x, PE2_y;
    double    PE3_x, PE3_y;

    double    edist_long;
    double    eangle;

    double    H;
    double    K;
    double    A;
    double    B;
    double    I;
    int       N,M,i;
    double    X[NUM_POINTS];
    double    Y[NUM_POINTS];

    double    I1, C1, S1, C2, S2, C3, S3, T1;
    double    P,X1,Y1;


    first = 0;

    /*
     *    If no command line argument, then set to full scale.
     *    my_sf = 1 is the largest picture and my_sf = 100 is
     *    the smallest picture.
     */
    if (argc == 1)
        my_sf = 1;
    /*
     *    One command line parameter is allowed, and that is the
     *    scaling factor.  The parm must be converted to an int
     *    using atoi.  Also, checking is done to limit the scaling
     *    range to between 1 and 100.
     */
    else if (argc == 2)
    {
        my_sf = atoi(argv[1]);
        if (my_sf > 150 || my_sf < 1)
        {
            fprintf(stderr,"ERROR: Scaling parm is out of range\n");
            fprintf(stderr,"Allowable range is (1..100)\n");
            exit(1);
        }
    }
    else
    {
        fprintf(stderr,"ERROR: Too many command line parameters\n");
        fprintf(stderr,"usage:  build [sc_factor (1..100)]\n");
        exit(1);
    }

    /*
     *    Adjust the pen widths based on the scaling factor
     */
    if (my_sf >= 1 && my_sf <= 10)
    {
        pen_width_1 = .12;
        pen_width_2 = 1.05;
    }
    else if (my_sf >= 11 && my_sf <= 20)
    {
        pen_width_1 = .12;
        pen_width_2 = 1.00;
    }
    else if (my_sf >= 21 && my_sf <= 30)
    {
        pen_width_1 = .11;
        pen_width_2 = .95;
    }
    else if (my_sf >= 31 && my_sf <= 40)
    {
        pen_width_1 = .11;
        pen_width_2 = .90;
    }
    else if (my_sf >= 41 && my_sf <= 50)
    {
        pen_width_1 = .10;
        pen_width_2 = .85;
    }
    else if (my_sf >= 51 && my_sf <= 60)
    {
        pen_width_1 = .10;
        pen_width_2 = .80;
    }
    else if (my_sf >= 61 && my_sf <= 70)
    {
        pen_width_1 = .09;
        pen_width_2 = .75;
    }
    else if (my_sf >= 71 && my_sf <= 80)
    {
        pen_width_1 = .09;
        pen_width_2 = .70;
    }
    else if (my_sf >= 81 && my_sf <= 90)
    {
        pen_width_1 = .08;
        pen_width_2 = .65;
    }
    else if (my_sf >= 91 && my_sf <= 100)
    {
        pen_width_1 = .08;
        pen_width_2 = .60;
    }
    else
    {
        /*
         *    The purpose of this "else" is purely for
         *    experimentation of scaling factors greater
         *    than 100.  To achieve this, you must muck
         *    with the code that limits the scaling factor
         *    range.
         */
        pen_width_1 = .05;
        pen_width_2 = .25;
    }

    /*
     *    Basic layout, X and Y axis of the figure is as follows:
     *
     *
     *   Y /|\
     *      |            This is how the figure is built.
     *      |
     *      |-------> X
     *
     *
     *      |-------> Y
     *      |
     *      |           This is how the figure is meant to be viewed.
     *   X \|/
     *
     */

    /*
     *    Calculate the total length of the Y axis.
     *    Calculate the mid point of the Y axis.
     */
    length_y   = UPPER_Y - LOWER_Y;
    midpoint_y = LOWER_Y + (length_y / 2);

    /*
     *    sideA = 1/5th the length of the Y-axis
     *    sideB = length of the X axis
     *    Calculate:
     *        length of the hypotenuse
     *        angles for a and b
     */
    sideA = (double)length_y / 5.0;
    sideB = (double)UPPER_X - (double)LOWER_X;
    sideC = 0;
    anga  = 0;
    angb  = 0;
    resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

    /*
     *    Divide the hypotenuse by 2, then divide the remaining
     *    length of the hypotenuse by 200. (Total 400)
     *
     *    Try to picture a right triangle with the following properties:
     *        - sideB length = half the length of X axis
     *        - sideB starting at midpoint of X axis
     *        - sideB ending at UPPER_X
     *        - sideA starting at midpoint of X axis
     *        - sideA extending to point on sideC
     *        - sideC consisting of 200 equal-length segments
     *
     *    The following resolve_triangle is used to size the figure.
     *    Here is the strategy:
     *        - Provide for 100 allowable figure sizes
     *        - Make sure that the largest figure is noticeably
     *          smaller than the picture frame
     *        - Make sure that the smallest figure is still
     *          recognizable.
     *
     *    200 points on the line segment allows for 100 points in the
     *    middle, and ample padding on the "outside".  The number
     *    my_sf+55 positions the figure well within the outside picture
     *    frame.  Solve for sideB, which is the distance from the
     *    upper (or lower) X frame to the figure.
     */
    sideA = 0;
    sideB = 0;
    sideC = (sideC / 400.0) * (double)(my_sf+55);
    resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

    /*
     *    Store the value of the 1st coordinates for the square.
     *    Store both the upper and lower bound for the X coordinates.
     *    Also store the length of a square.  (Not the whole length
     *    of the figure.
     */
    SQ_x1  = LOWER_X + sideB;
    SQ_x2  = UPPER_X - sideB;
    len_SQ = SQ_x2 - SQ_x1;

    sideA = 0;
    sideB = 0;
    sideC = len_SQ / 2.0;
    anga  = 45;
    angb  = 45;

    resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);


    total_rect_len = len_SQ + len_SQ - (sideA / 2.0) -
                ((len_SQ / 2.0) - sideA);


    SQ_y1  = midpoint_y - (total_rect_len / 2.0);
    SQ_y2  = SQ_y1;

    printf("%%1B\n");
    printf("IN;\nPA;\nSP 1;\n\n");

    /*
     * this code generates the outside perimeter frame
     */
/*   printf("PU %d %d;\n",LOWER_X,UPPER_Y);
     printf("PD %d %d;\n",UPPER_X,UPPER_Y);
     printf("PD %d %d;\n",UPPER_X,LOWER_Y);
     printf("PD %d %d;\n",LOWER_X,LOWER_Y);
     printf("PD %d %d;\n",LOWER_X,UPPER_Y); */


    printf("PW %f;\n",pen_width_1);
    printf("NP 8;\n");        /* number of pens = 8 */
    printf("TR 1;\n");        /* transparency mode = on */
    printf("MC 1;\n");        /* merge control = on */

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

    /*
     *    Segment #1
     */
    printf("PU %f %f;\n",SQs_x1,SQs_y1);
    printf("PD %f %f;\n",SQs_x1 - (len_SQs / 4.0),SQs_y1 + (len_SQs / 4.0));
    printf("PD %f %f;\n",SQC_x,SQC_y + (len_SQs / 2.0));

    /*
     *    Segment #2
     */
    printf("PU %f %f;\n",SQs_x1 - (len_SQs / 4.0),SQs_y1 + (len_SQs / 4.0));
    printf("PD %f %f;\n",SQs_x1,SQs_y1 + (len_SQs / 4.0));


    printf("LA 2 6;\n");


    printf("PU %f %f;\n",(LSQ0_x = SQs_x1),
        (LSQ0_y = SQs_y1 + (len_SQs / 4.0)));

    printf("PD %f %f;\n",(LSQ1_x = SQs_x1 + (len_SQs / 4.0)),
        (LSQ1_y = SQs_y1));

    printf("PD %f %f;\n",(LSQ2_x = SQs_x1 + (len_SQs / 2.0)),
        (LSQ2_y = SQs_y1 + (len_SQs / 4.0)));

    printf("PD %f %f;\n",(LSQ3_x = SQs_x1 + (len_SQs / 4.0)),
        (LSQ3_y = SQs_y1 + (len_SQs / 2.0)));

    printf("PD %f %f;\n",SQs_x1,SQs_y1 + (len_SQs / 4.0));


    printf("LA 2 1;\n");



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

    sideC = 0;
    sideA = 0;
    sideB = try1 - SQ_y1;
    anga  = ang_x0;
    angb  = ang_x1;

    resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

    JR1_x = SQC_x + sideA;
    JR1_y = SQC_y + increment;

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

    sideC = 0;
    sideA = 0;
    sideB = SQC_y + (len_SQs / 2.0) - JR1_y;
    anga  = 45;
    angb  = 45;

    resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

    JR3_x = SQC_x + (len_SQs / 2.0) - sideA;
    JR3_y = JR1_y;

    sideC = 0;
    sideA = SQC_y - JR2_y;
    sideB = 0;
    anga  = 45;
    angb  = 45;

    resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

    JR4_x = SQC_x + sideB;
    JR4_y = JR2_y;

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

    if ((JR4_x - JR5_x) < 0.05)
    {
        adjust = adjust + .01;
        goto incr;
    }

    printf("PU %f %f;\n",JR1_x,JR1_y);
    printf("PD %f %f;\n",JR3_x,JR3_y);

    printf("PU %f %f;\n",JR2_x,JR2_y);
    printf("PD %f %f;\n",JR5_x,JR5_y);
    printf("PD %f %f;\n",SQC_x,SQC_y + (len_SQs / 2.0));


    printf("LA 2 6;\n");
    printf("PD %f %f;\n",JR2_x,JR2_y);
    printf("PD %f %f;\n",SQC_x,SQ_y1);
    printf("LA 2 1;\n");

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

    printf("PU %f %f;\n",JR7_x,JR7_y);
    printf("PD %f %f;\n",JR9_x,JR9_y);
    printf("PD %f %f;\n",JR3_x,JR3_y);
    printf("PU %f %f;\n",JR8_x,JR8_y);
    printf("PD %f %f;\n",JR9_x,JR9_y);

    get_intersect(SQC_x,SQC_y,JR2_x,SQC_y + (len_SQs/2.0),
        SQC_x,SQC_y+(len_SQs/2.0),JR2_x,JR2_y, &JRA_x,&JRA_y);

    JRE_x = ((len_SQs / 2.0) * .55) + SQC_x;
    JRE_y = SQC_y + (len_SQs / 2.0);

    printf("PU %f %f;\n",JRA_x,JRA_y);
    printf("PD %f %f;\n",JRE_x,JRE_y);

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
    printf("BZ %f %f %f %f %f %f;\n",JRD_x, JRD_y, JRC_x,JRC_y,JRB_x,JRB_y);

    JRF_x = SQs_x2;
    JRF_y = SQs_y1 + len_SQs;

    sideC = 0;
    sideA = JRA_y - JR2_y;
    sideB = JR2_x - JRA_x;
    angb  = 0;
    anga  = 0;

    resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

    /*
     *    PE1 is the left-most point (approx) of the ellipse which
     *    is on the segment JRA-JR2.  Decreasing EVAR1 moves PE1
     *    closer to JR2.  Increasing EVAR1 moves PE1 closing to JRA.
     *    EVAR1 should be less than 1, and it should be quite a bit
     *    bigger than .50.
     */
    sideC = 0;
    sideA = sideA * EVAR1;
    sideB = 0;

    resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

    PE1_x = JR2_x - sideB;
    PE1_y = JR2_y + sideA;

    /*
     *    PE2 is the right-most point (approx) of the ellipse which
     *    is on the segment JRE-JRF.  Decreasing EVAR2 moves PE2 closer
     *    to JRE.  Increasing EVAR2 moves PE2 closer to JRF.  EVAR2
     *    should be somewhere around .50.
     */
    PE2_x = JRE_x + ((JRF_x - JRE_x) * EVAR2);
    PE2_y = JRF_y;

    /*
     *    Resolve for the midpoint of the line connecting PE1 and
     *    PE2.  To do this, draw a perpendicular from the segment
     *    JRE-JRF to PE1.  This creates a right triangle with the
     *    hypotenuse PE2-PE1.
     */
    sideC = 0;
    sideA = PE2_x - PE1_x;
    sideB = PE2_y - PE1_y;
    angb  = 0;
    anga  = 0;

    resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

    edist_long = sideC;    /* length of segment PE2-PE1 */
    eangle     = angb;    /* angle PE1-PE2-JRE */

    /*
     *    Cut the segment in half (roughly) and resolve in
     *    order to find the midpoint.  Decreasing EVAR5 moves
     *    the midpoint more towards PE1.  Increasing EVAR5 moves
     *    the midpoint more towards PE2.
     */
    sideC = sideC * EVAR5;
    sideA = 0;
    sideB = 0;

    resolve_triangle(&sideA,&sideB,&sideC,&anga,&angb);

    PE3_x = PE1_x + sideA;
    PE3_y = PE1_y + sideB;

    /*
     *    Now begins the algorithm to calculate the ellipse.  This
     *    algorithm was taken from a book I found in the HP library.
     *
     *        Title:   Mathematical Elements for Computer Graphics
     *        Authors: David Rogers, J. Alan Adams
     *        Section: Appendix C-20
     *        Name:    Parametric Ellipse Algorithm
     */
    H = PE3_x;            /* center point - X */
    K = PE3_y;            /* center point - Y */
    A = (edist_long / 2) * EVAR4;    /* length (long axis) of ellipse */
    B = edist_long * EVAR3;        /* length (short axis) of ellipse */
    I = eangle;            /* angle of inclination for ellipse */
    N = NUM_POINTS;

    P  = 2 * 3.141592654 / (double)(N-1.0);
    I1 = I / 57.2957795;
    C1 = cos(I1);
    S1 = sin(I1);
    C2 = cos(P);
    S2 = sin(P);
    C3 = 1.0;
    S3 = 0.0;

    for (M=0; M < N; ++M)
    {
        X1   = A * C3;
        Y1   = B * S3;
        X[M] = H + X1 * C1 - Y1 * S1;
        Y[M] = K + X1 * S1 + Y1 * C1;
        T1   = C3 * C2 - S3 * S2;
        S3   = S3 * C2 + C3 * S2;
        C3   = T1;
    }

    /*
     *    Move the pen to the 1st element of the ellipse, then
     *    move the pen down.
     */
    printf("PU %f %f;\n",X[0],Y[0]);
    printf("PD;\n");

    /*
     *    Generate the ellipse in HP-GL/2 using the BZ command.
     *    Each BZ command has 6 arguments (3 X,Y coordinate pairs)
     */
    for (i=0; i < NUM_POINTS;)
    {
        printf("BZ %f,%f,%f,%f,%f,%f;\n",X[i],Y[i],X[i+1],Y[i+1],
                                X[i+2],Y[i+2]);
        i = i + 3;
    }

    printf("PU;\n");

#ifdef DO_COLOR
    /*
     *    Add color to the thick-lined square figure on the right.
     *    The mechanism for adding color to basic figures is this:
     *        - PU to a point somewhere on the polygon
     *        - Start Polygon Mode with the "PM 0" command
     *        - Trace the figure with PU commands
     *        - Close the polygon with the "PM 2" command
     *        - Select a pen to fill with
     *        - Fill with Polygon with the "FP" command
     *        - Select the standard pen "SP 1" when done.
     *
     *    Also, it is important to note that the coloring of the
     *    figures needs to be "transparent" to the black pen-lines.
     *    To achieve this, tranparency modes needs to be on ("TR 1"),
     *    and merge control needs to be on ("MC 1");
     */
    if (first == 1)
    {
        /*
         *    Add color to the top portion of the inner
         *    "perspective" box.  The box is colored red.
         */
        printf("PU %f %f;\n",JR5_x,JR5_y);
        printf("PM 0;\n");
        printf("PU %f %f;\n",JR5_x,JR5_y);
        printf("PU %f %f;\n",JR7_x,JR7_y);
        printf("PU %f %f;\n",JR9_x,JR9_y);
        printf("PU %f %f;\n",JR3_x,JR3_y);
        printf("PU %f %f;\n",JR5_x,JR5_y);
        printf("PM 2;\n");
        printf("SP 2;\n");
        printf("FP;\n");
        printf("SP 1;\n");

        /*
         *    Add color to the left-sided inscribed square.
         *    The square is colored yellow.
         */
        printf("PU %f %f;\n",LSQ1_x,LSQ1_y);
        printf("PM 0;\n");
        printf("PU %f %f;\n",LSQ1_x,LSQ1_y);
        printf("PU %f %f;\n",LSQ2_x,LSQ2_y);
        printf("PU %f %f;\n",LSQ3_x,LSQ3_y);
        printf("PU %f %f;\n",LSQ0_x,LSQ0_y);
        printf("PU %f %f;\n",LSQ1_x,LSQ1_y);
        printf("PM 2;\n");
        printf("SP 4;\n");
        printf("FP;\n");
        printf("SP 1;\n");

        /*
         *    Add color to the ellipse figure.  The
         *    ellipse is colored red.
         */
        printf("PU %f %f;\n",X[0],Y[0]);
        printf("PM 0;\n");
        for (i=0; i < NUM_POINTS;)
        {
            printf("BZ %f,%f,%f,%f,%f,%f;\n",X[i],Y[i],X[i+1],
                            Y[i+1],X[i+2],Y[i+2]);
            i = i + 3;
        }
        printf("PM 2;\n");
        printf("SP 2;\n");
        printf("FP;\n");
        printf("SP 1;\n");

        /*
         *    Add color to one of the many triangles in the
         *    figure.  The triangle is colored green.
         */
        printf("PU %f %f;\n",JR6_x,JR6_y);
        printf("PM 0;\n");
        printf("PU %f %f;\n",JR6_x,JR6_y);
        printf("PU %f %f;\n",JR2_x,JR2_y);
        printf("PU %f %f;\n",JR2_x,SQs_y1);
        printf("PU %f %f;\n",JR6_x,JR6_y);
        printf("PM 2;\n");
        printf("SP 3;\n");
        printf("FP;\n");
        printf("SP 1;\n");
    }
#endif

    /*
     *    The 1st square has been generated.  Now generate the
     *    2nd one.  Change the base coordinates, change the
     *    Pen-Width, and go get 'em again.
     */
    if (first++ == 0)
    {
        SQ_x1  = SQ_x1;
        SQ_x2  = SQ_x2;
        SQ_y1  = SQsC_y;
        SQ_y2  = SQ_y1;
        len_SQ = len_SQ;
        printf("PW %f;\n",pen_width_2);
        goto again;
    }

    /*
     *    Done generating figure.  Add on command to switch
     *    back to PCL (Esc%1A), and command to eject the page
     *    (Ctrl-L).
     */
    printf("%%1A\n\n");
}
