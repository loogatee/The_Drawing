
#define	LOWER_X		2
#define UPPER_X		8120
#define	LOWER_Y		2
#define	UPPER_Y		10150


#define	LOWER_X1	2
#define UPPER_X1	10830
#define	LOWER_Y1	2
#define	UPPER_Y1	8260

main(argc,argv)
int	argc;
char	*argv[];
{

	/*
	 *  Page 1.  Enters HPGL via Esc%1B
	 *
	 *           Uses current PCL CAP
	 */
	printf("%%1B\n");
	printf("IN;\nPA;\nSP 1;\n");

	/*
	 * generate the outside perimeter frame
	 */
	printf("PU %d %d;\n",LOWER_X,UPPER_Y);
	printf("PD %d %d;\n",UPPER_X,UPPER_Y);
	printf("PD %d %d;\n",UPPER_X,LOWER_Y);
	printf("PD %d %d;\n",LOWER_X,LOWER_Y);
	printf("PD %d %d;\n",LOWER_X,UPPER_Y);

	printf("%%1A\n\n");


	/*
	 *  Page 2.  Enters HPGL via Stand-alone plotter mode
	 */
	printf("%%-1B\n");
	printf("IN;\nPA;\nSP 1;\n");

	/*
	 * generate the outside perimeter frame
	 */
	printf("PU %d %d;\n",LOWER_X1,UPPER_Y1);
	printf("PD %d %d;\n",UPPER_X1,UPPER_Y1);
	printf("PD %d %d;\n",UPPER_X1,LOWER_Y1);
	printf("PD %d %d;\n",LOWER_X1,LOWER_Y1);
	printf("PD %d %d;\n",LOWER_X1,UPPER_Y1);

	printf("E\n");
}

/*

Here is the output from this program

%1B
IN;
PA;
SP 1;
PU 2 10150;
PD 8120 10150;
PD 8120 2;
PD 2 2;
PD 2 10150;
%1A

%-1B
IN;
PA;
SP 1;
PU 2 8260;
PD 10830 8260;
PD 10830 2;
PD 2 2;
PD 2 8260;
E


*/
