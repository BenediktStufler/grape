/*
 * precomputation of probability weights for separable permutations
 */

INT sepamax;
double sepaq[2];


void sepainit() {

/*
 * Weights for balls in boxes model
 * Obtained using the following mathematica code
 *
 *
$MaxExtraPrecision=1000
Q = z^2/(1-z)
Qs = Q / z
phi = 1 / (1-Qs)
phip = D[phi,z] // Simplify
Solve[phip*z == phi && z > 0, z]
tau = ( 2 - Sqrt[2] ) / 2
phitau = phi /. {z->tau} // Simplify
xi = (phi/phitau) /.{z-> z*tau}  // Simplify
up = 1000
taxi = Table[Coefficient[Series[xi,{z,0,up}],z,n], {n,0,up}]
Export["sepa_xi.txt", N[taxi,200], "CSV"]
taq = Table[N[taxi[[n+1]] / ( 1 - Sum[taxi[[i]], {i,1,n}]),200], {n,0,up}]
Export["sepa_q.txt", taq, "CSV"]

// for separable permutations, we have
// taq[[0]] == 2 - Sqrt[2]
// taq[[i]] == Sqrt[2] - 1 for all i >= 1

*/

	sepamax = 2;
	sepaq[0] = 0.58578643762690495119831127579030192143032812462305192682332026200926752153789296114961246567235842726498615376908770297507516394414926278735587850290006416858677733407249440724420004949884721793942853;
	sepaq[1] = 0.41421356237309504880168872420969807856967187537694807317667973799073247846210703885038753432764157273501384623091229702492483605585073721264412149709993583141322266592750559275579995050115278206057147;


}

