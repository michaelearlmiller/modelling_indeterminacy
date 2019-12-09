
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <iostream>
#include<fstream>
#include<string>
//run these in the fourth year folder 
//./g_if 86400 10000 9 > g9_day_tenthdays_if1.txt
// g++ if_indeterminacy.cpp -o g_if
// ./g_if 86400 10001 11
// for i in {1..1} ; do ./g_if 86400 200000000 12 ; ./g_if 86400 200000000 12 >> g12_day_2hmildays_if1000.txt ; done (run multiple times)
//cat ~/file01 ~/file02 ~/file03 ~/fileA ~/fileB ~/fileC > merged-file
//for i in {1..100} ; do g++ Indeterminacy_passport.cpp -o g_7 ; ./g_7 3600 24000 16; ./g_7 >> g16_hour_1000days_100runs.txt ; done

double indeterminacy(double number, int d_w){

	double a = 0;
	a = (1./pow(10.,d_w));
	double deviate = rand()/(double)RAND_MAX;
	double stepper = pow(10.0, d_w);
    return (trunc(stepper * number) / stepper) + a*deviate;


}


class Body
{
 
	

public:
	double mass,px,py,vx,vy;
	std::vector<double> attraction(double gi, Body other){
    double dx = (other.px-px);
    double dy = (other.py-py);
    double d = sqrt(dx*dx + dy*dy);
	
	double  f =  gi * mass * other.mass / (d*d);

	if (d == 0){
		exit(1);
	}

	double theta = atan2(dy, dx);
    double fx =cos(theta) * f;
    double fy = sin(theta) * f;
    std::vector<double> farr(2);
    farr[0] = -fx; 
    farr[1] = -fy;
    return farr;
        
}
	
};

std::vector<std::vector<double> > loop(std::vector<Body> & bodies, double timestep, int number_of_steps, int d_w){
	//double timestep =  3600 ; // one hour
	int step = 1;
	double AU = (149.6e6 * 1000.);
	double G = 6.67428e-11;
    
	//int number_of_steps = 24000; // one thousand days
	std::vector<double> rlist(number_of_steps/1000);
	std::vector<double> xlist(number_of_steps/1000);
	std::vector<double> ylist(number_of_steps/1000);
	std::vector<double> glist(number_of_steps/1000);
	while(step < number_of_steps){
        

		double g = 0;
		g = indeterminacy(G, d_w);
		double x = bodies[0].px/AU;
    	double y = bodies[0].py/AU;
    	double r = sqrt(x*x + y*y);



        if(step % 1000 == 0){

		rlist[step/1000] = r;
		xlist[step/1000] = x;
		ylist[step/1000] = y;
		glist[step/1000] = g;

    }
      
    
        step += 1;
        
        std::vector<double> force = bodies[1].attraction(g,bodies[0]);
        
        bodies[0].vx += force[0] / bodies[0].mass * timestep;
        bodies[0].vy += force[1] / bodies[0].mass * timestep;

        bodies[0].px +=  bodies[0].vx * timestep;
        bodies[0].py +=  bodies[0].vy * timestep;

      


	} 

 std::vector<std::vector<double> > data;
 data.push_back(rlist);
 data.push_back(xlist);
 data.push_back(ylist);
 data.push_back(glist);

 return(data);        
}


int main(int argc, char const *argv[])
{
	srand(time(NULL));

	double timestep = atof(argv[1]);
	int number_of_steps = atoi(argv[2]);
	int d_w = atoi(argv[3]);


	Body sun;
    sun.mass = 1.98892 *  pow(10.,30.);
    sun.px = 0. ;
    sun.py = 0. ;
    sun.vx = 0. ;
    sun.vy = 0. ;

    double AU = (149.6e6 * 1000);
   	Body earth;
    earth.mass = 1.98892 * pow(10.,30.);
    earth.px = -1 * AU;
    earth.py = 0.;
    earth.vx = 0.; 
    earth.vy = 29.783 * 1000.;

    std::vector<Body> bodies;
    bodies.push_back(earth);
    bodies.push_back(sun);


//write output to textfile

    //FILE * myfile;

    //myfile = fopen("g_mp_test1.txt","w");

    std::vector<std::vector<double> > data = loop(bodies, timestep, number_of_steps, d_w);
    std::vector<double> rlist = data[0];
    std::vector<double> xlist = data[1];
    std::vector<double> ylist = data[2];
    std::vector<double> glist = data[3];
    	for (int i = 0; i < rlist.size(); i++) {
            //if (i % 1000 == 0) { 
               printf("%d %.16f %.16f %.16f %.20f\n" ,i, rlist[i], xlist[i], ylist[i], glist[i]);
            //fprintf(myfile, "%d %.16f %.16f %.16f\n" ,i, rlist[i], xlist[i], ylist[i]);
    		
    	 } 
    //fclose(myfile);

	return 0;
}
