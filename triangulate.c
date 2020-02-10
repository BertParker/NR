#include <stdio.h>
#include <math.h>

//Globals
float F[2];
float Jac[2][2];
float Inverse[2][2];
float Result[2];

void getFunction(float x, float y, float r1, float r2, float xa, float ya, float xb, float yb, float xc, float yc){
	F[0] = sqrt(pow(x-xb, 2) + pow(x-yb,2)) - sqrt(pow(x-xa, 2) + pow(x-ya,2)) - r1;
	F[1] = sqrt(pow(x-xc, 2) + pow(x-yc,2)) - sqrt(pow(x-xa, 2) + pow(x-ya,2)) - r2;
}

void getJacobian(float x, float y, float xa, float ya, float xb, float yb, float xc, float yc){
	//Calculate derivative 
	Jac[0][0] = (x-xb)*(pow(pow(x-xb, 2) + pow(y-yb, 2), -0.5)) - (x-xa)*(pow(pow(x-xa,2) + pow(y-ya,2),-0.5));
	Jac[0][1] = (y-yb)*(pow(pow(x-xb, 2) + pow(y-yb, 2), -0.5)) - (y-ya)*(pow(pow(x-xa,2) + pow(y-ya,2),-0.5));
	Jac[1][0] = (x-xc)*(pow(pow(x-xc, 2) + pow(y-yc, 2), -0.5)) - (x-xa)*(pow(pow(x-xa,2) + pow(y-ya,2),-0.5));
	Jac[1][1] = (y-yc)*(pow(pow(x-xc, 2) + pow(y-yc, 2), -0.5)) - (y-ya)*(pow(pow(x-xa,2) + pow(y-ya,2),-0.5));
}

void getInverse2x2(){
	//Get a,b,c,d for ease of use
	float a = Jac[0][0];
	float b = Jac[0][1];
	float c = Jac[1][0];
	float d = Jac[1][1];

	//Calculate determinant
	float det = 1/(a*d - b*c);

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

float calculateError(float xOld, float yOld, float x, float y){
	float temp = pow(x-xOld, 2);
	temp += pow(y-yOld, 2);

	return temp;
}

int getAngle(float x, float y){
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
	float rad2Deg = 180/3.14f;
	return (int)(atan(fabs(x)/fabs(y))*rad2Deg + quad);
}

//maybe put a thermistor here
float getTemperature(){
	return 25;
}

float getSpeedOfSound(){
	return (331 + 0.6*getTemperature());
}

int triangulate(float xa, float ya, float xb, float yb, float xc, float yc, int ta, int tb, int tc){

	//Variables
	float x,y = 1.0f;
	float err = 0;
	float rAB = getSpeedOfSound()*(tb - ta);
	float rAC = getSpeedOfSound()*(tc - ta);


	while(err < 0.1){
		//Calculate function into F
		//F =
		getFunction(x, y, rAB, rAC, xa, ya, xb, yb, xc, yc);

		//Calculate derivatives into Jac
		//Jac =
		getJacobian(x, y, xa, ya, xb, yb, xc, yc);

		//Place inverse of Jac into Inverse
		//Inverse =
		getInverse2x2();

		//Multiply Inverse and F into Result
		//Result = 
		mult2x2Matrix();

		//Calculate error between new and old values
		err = calculateError(x, y, Result[0], Result[1]);

		//Update values
		x = Result[0];
		y = Result[1];
	}
	//Note: this returns the angle from the perspective of the Mic which received the sound first
	return getAngle(x,y);
}

int main(){

}