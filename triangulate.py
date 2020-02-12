#!python3
import numpy as np 
import math

"""
tauAB, tauAC are constants
where A, B, C are time stamps of when mic receives sound
where 'A' is first mic received
and TauAB = B - A
and TauAC = C - A
"""

def lorenzSysNR(X, C, tauAB, tauAC, xa, ya, xb, yb, xc, yc):
	x = X[0]
	y = X[1]

	#equations for fval
	eq1 = math.sqrt((x - xb)**2 + (y - yb)**2) - math.sqrt((x - xa)**2 + (y - ya)**2) - C*tauAB
	eq2 = math.sqrt((x - xc)**2 + (y - yc)**2) - math.sqrt((x - xa)**2 + (y - ya)**2) - C*tauAC
	print(C*tauAB)
	#equations for jacobian
	der1_1 = (x - xb)*(((x - xb)**2 + (y - yb)**2)**(-0.5)) - (x - xa)*(((x - xa)**2 + (y - ya)**2)**(-0.5)) 
	der1_2 = (y - yb)*(((x - xb)**2 + (y - yb)**2)**(-0.5)) - (y - ya)*(((x - xa)**2 + (y - ya)**2)**(-0.5))
	der2_1 = (x - xc)*(((x - xc)**2 + (y - yc)**2)**(-0.5)) - (x - xa)*(((x - xa)**2 + (y - ya)**2)**(-0.5))
	der2_2 = (y - yc)*(((x - xc)**2 + (y - yc)**2)**(-0.5)) - (y - ya)*(((x - xa)**2 + (y - ya)**2)**(-0.5))

	fval = np.array([eq1, eq2])
	fval = fval.reshape((2,1))

	jac = np.array([[der1_1[0],der1_2[0]],[der2_1[0],der2_2[0]]])

	fval = fval.astype(float)
	jac = jac.astype(float)

	print(fval)
	print(jac)

	return fval, jac




def multiVarNR(xa, ya, xb, yb, xc, yc, ta, tb, tc):
	X0 = np.array([[1.0], [1.0]])

	maxIter = 50
	tolX = 1e-6

	tauAB = tb - ta
	tauAC = tc - ta
	C = 343

	X = X0
	Xold = X0
	err = np.zeros(2)

	for i in range(maxIter):
		f, j = lorenzSysNR(X, C, tauAB, tauAC, xa, ya, xb, yb, xc, yc)

		X = X - np.matmul(np.linalg.inv(j), f)

		err = abs(X - Xold)
		print("err)")
		print(err)

		Xold = X

		if (err[0] < tolX and err[1] < tolX):
			break		

	return X




def main():
	#X = multiVarNR(0.0, -0.0866, -0.10, 0.0866, 0.1, 0.0866, 0.000023, 0.000039, 0.000045)
	X = multiVarNR(0.0, -2.0, -3.0, 2.0, 3.0, 2.0, 0.00583, 0.01505, 0.01505)
	print(X)


if __name__ == '__main__':
	main()






