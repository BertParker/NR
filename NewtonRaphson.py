import numpy as np

def lorenzSysNR(X):
	x = X[0]
	y = X[1]
	z = X[2]

	fval = np.array([x-y, 2*x - x*z - y,x*y - 3*z])
	jac = np.array([[1, -1, 0],[2-z,-1,-x],[y,x,-3]])

	return fval, jac


def multiVarNR():
	X0 = np.array([[1.0], [1.0], [1.0]])

	maxIter = 50
	tolX = 1e-6

	X = X0
	Xold = X0
	err = np.zeros(3)

	for i in range(50):
		f, j = lorenzSysNR(X)

		X = X - np.matmul(np.linalg.inv(j), f)

		err = abs(X-Xold)
		Xold = X
	
		if (err[0] < tolX and err[1] < tolX and err[2] < tolX):
			break

	return X



def main():
	print(multiVarNR())

if __name__ == '__main__':
	main()