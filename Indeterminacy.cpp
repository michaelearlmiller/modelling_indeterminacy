
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <iostream>
#include<fstream>
#include<string>
//run these in the fourth year folder 
//./g_11 > g_11.txt
// g++ Indeterminacy.cpp -o g_10
// ./g_10
// for i in {1..50} ; do ./g_10 ; done (run multiple times)
//cat ~/file01 ~/file02 ~/file03 ~/fileA ~/fileB ~/fileC > merged-file
//for i in {1..100} ; do g++ Indeterminacy.cpp -o g_7 ; ./g_7 ; ./g_7 >> test.txt ; done

double indeterminacy(double number, int d_w){

	srand(time(NULL));
	double a = (1./pow(10.,d_w));
	double deviate = rand()/(double)RAND_MAX;
	double stepper = pow(10.0, d_w);
    return (trunc(stepper * number) / stepper) + a*deviate;

}


class Body
{
 
	

public:
	double mass,px,py,vx,vy;
	std::vector<double> attraction(Body other){
	double G = 6.67428e-11;
	//double g = indeterminacy(G,7);
    double dx = (other.px-px);
    double dy = (other.py-py);
    double d = sqrt(dx*dx + dy*dy);
	
	double  f =  G * mass * other.mass / (d*d);

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

std::vector<double> loop(std::vector<Body> & bodies){
	double timestep =  3600 ; // one hour
	double step = 1;
	double AU = (149.6e6 * 1000.);

	double number_of_steps = 24000; // one thousand days
	std::vector<double> rlist(number_of_steps);
	std::vector<double> xlist(number_of_steps);
	std::vector<double> ylist(number_of_steps);
	while(step < number_of_steps){


		double x = bodies[0].px/AU;
    	double y = bodies[0].py/AU;
    	double r = sqrt(x*x + y*y);

		rlist[step] = r;
		xlist[step] = x;
		ylist[step] = y;
      
    
        step += 1;
        
        std::vector<double> force = bodies[1].attraction(bodies[0]);
        
        bodies[0].vx += force[0] / bodies[0].mass * timestep;
        bodies[0].vy += force[1] / bodies[0].mass * timestep;

        bodies[0].px +=  bodies[0].vx * timestep;
        bodies[0].py +=  bodies[0].vy * timestep;

      


	}

 return(rlist);
 return(xlist);
 return(ylist);
        
}




int main(int argc, char const *argv[])
{

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

    FILE * myfile;

    myfile = fopen("g_mp_test.txt","w");

    std::vector<double> rlist = loop(bodies);
    std::vector<double> xlist = loop(bodies);
    std::vector<double> ylist = loop(bodies);
    	for(int i = 0; i < rlist.size(); i++){
            fprintf(myfile, "%d %.16f %.16f %.16f\n" ,i, rlist[i],xlist[i],ylist[i]);
    		//printf("%d %.16f\n" ,i, rlist[i]);
    	}
    fclose(myfile);
	return 0;
}

