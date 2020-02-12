#include <stdio.h>
#include <math.h>

//Globals
double F[2];
double Jac[2][2];
double Inverse[2][2];
double Result[2];
int angle;

void getFunction(double x, double y, double r1, double r2, double xa, double ya, double xb, double yb, double xc, double yc){
	F[0] = sqrt(pow(x-xb, 2) + pow(y-yb,2)) - sqrt(pow(x-xa, 2) + pow(y-ya,2)) - r1;
	F[1] = sqrt(pow(x-xc, 2) + pow(y-yc,2)) - sqrt(pow(x-xa, 2) + pow(y-ya,2)) - r2;
	printf("%f", r1);
}

void getJacobian(double x, double y, double xa, double ya, double xb, double yb, double xc, double yc){
	//Calculate derivative 
	Jac[0][0] = (x-xb)*(pow(pow(x-xb, 2) + pow(y-yb, 2), -0.5)) - (x-xa)*(pow(pow(x-xa,2) + pow(y-ya,2),-0.5));
	Jac[0][1] = (y-yb)*(pow(pow(x-xb, 2) + pow(y-yb, 2), -0.5)) - (y-ya)*(pow(pow(x-xa,2) + pow(y-ya,2),-0.5));
	Jac[1][0] = (x-xc)*(pow(pow(x-xc, 2) + pow(y-yc, 2), -0.5)) - (x-xa)*(pow(pow(x-xa,2) + pow(y-ya,2),-0.5));
	Jac[1][1] = (y-yc)*(pow(pow(x-xc, 2) + pow(y-yc, 2), -0.5)) - (y-ya)*(pow(pow(x-xa,2) + pow(y-ya,2),-0.5));
}

void getInverse2x2(){
	//Get a,b,c,d for ease of use
	double a = Jac[0][0];
	double b = Jac[0][1];
	double c = Jac[1][0];
	double d = Jac[1][1];

	//Calculate determinant
	double det = 1/(a*d - b*c);

	//Invert it
	Inverse[0][0] = d*det;
	Inverse[0][1] = -1*b*det;
	Inverse[1][0] = -1*c*det;
	Inverse[1][1] = a*det;
}

void mult2x2Matrix(){
	//Compute Inverse[2][2]*F[2]
	Result[0] = Inverse[0][0]*F[0] + Inverse[0][1]*F[1];
	Result[1] = Inverse[1][0]*F[0] + Inverse[1][1]*F[1];
}

double calculateError(double xOld, double yOld, double x, double y){
	double temp = pow(x-xOld, 2);
	temp += pow(y-yOld, 2);

	return temp;
}

int getAngle(double x, double y){
	int quad;

	//Get quadrant of coordinates
	if (x > 0 && y > 0){
		quad = 0;
	}
	else if (x < 0 && y > 0){
		quad = 90;
	}
	else if (x < 0 && y < 0){
		quad = 180;
	}
	else{
		quad = 270;
	}

	//Compute angle in degrees
	double rad2Deg = 180.0/3.14;
	return (int)(angle + quad);
}

//maybe put a thermistor here
double getTemperature(){
	return 25;
}

double getSpeedOfSound(){
	return 343.0;
}

int triangulate(double xa, double ya, double xb, double yb, double xc, double yc, double ta, double tb, double tc){

	//Variables
	double x = 1.0;
	double y = 1.0;
	double xOld = x;
	double yOld = y;
	double err = 1;
	double rAB = getSpeedOfSound()*(tb - ta);
	printf("%f", rAB);
	double rAC = getSpeedOfSound()*(tc - ta);


	while(err > 0.000001){
		//Calculate function into F
		//F =
		getFunction(x, y, rAB, rAC, xa, ya, xb, yb, xc, yc);

		//Calculate derivatives into Jac
		//Jac =
		getJacobian(x, y, xa, ya, xb, yb, xc, yc);

		printf("[%f, %f]\n", F[0], F[1]);
		printf("[%f, %f, %f, %f]\n", Jac[0][0], Jac[0][1], Jac[1][0], Jac[1][1]);

		//Place inverse of Jac into Inverse
		//Inverse =
		getInverse2x2();

		//Multiply Inverse and F into Result
		//Result = 
		mult2x2Matrix();

		x = x - Result[0];
		y = y - Result[1];

		//Calculate error between new and old values
		err = calculateError(xOld, yOld, x, y);

		printf("err: %f\n", err);

		//Update values
		xOld = x;
		yOld = y;
	}
	printf("X: %f Y: %f\n", x, y);
	//Note: this returns the angle from the perspective of the Mic which received the sound first
	angle = getAngle(x,y);
	printf("angle: %d", angle);
}

int main(){
	//triangulate(-0.10, -0.0866, 0.010, -0.0866, 0.0, 0.0866, 0.000023, 0.000039, 0.000045);
	triangulate(0.0, -2.0, -3.0, 2.0, 3.0, 2.0, 0.00583, 0.01505, 0.01505);
	printf("angle: %d", angle);
}