# LiSK

If you need more information than provided here, contact the author:
Sebastian Kirchner (sebastian.t.kirchnerREMOVETHIS@gmail.com)

**LiSK** is a lightweight C++ library for the numerical evaluation of classical polylogarithms Li(n,x) and the special function Li22(x,y) for arbitrary complex arguments. The evaluation is possible in double and in arbitrary precision arithmetics. The implementation is based on the algorithms presented in arXiv:1601.02649 (or click [here](http://arxiv.org/abs/1601.02649)).

---

##Installation

**LiSK** is a complete _header-only_ library, i.e. the library does not have to be built separately but only the header file must be included in the user's code via the usual

	#include "lisk.hpp"

Note: The implementation has been put into the _lisk.cpp_ file due to readability. This source file must be present in the same directory as the header file.

**LiSK** is completely encapsulated in the namespace _LiSK_ and relies on several C++-11 features. A central part is the **CLN** library by Bruno Haible and has to be linked to all programs using **LiSK**. **CLN** can be downloaded from [here](http://www.ginac.de/CLN/).
A simple example of how to use **LiSK** is given in **example/example.cpp**, which can be compiled via

	g++ -O2 -std=c++11 example.cpp -o example -I/cln/inlude/path -I../LiSK -L/cln/library/path -lcln

Alternatively, if **cmake** is available, simply by running

	cmake .
	
in the example directory. If **CLN** is not found automatically the _CLN\_INCLUDE\_DIR_ and _CLN\_LIB_ path can easily be set via

	ccmake .

---

##Usage

In the following **T** denotes one of the two currently supported types 
`std::complex<double>` or `cln::cl_N`. An example for the computation of `Li(4,x)` and `Li22(x,y)` at given points x and y could look like
	
	LiSK::LiSK<T> lisk(n,p);
	lisk.Li(4,x);
	lisk.Li22(x,y);
	
The first line constructs a **LiSK** object of type T`.` The constructor  `LiSK(n=4,prec=34)` features two optional arguments. 

The first argument `n` defines the weigth of the `Li(n,x)` up to which the constants shall be pre-computed during **LiSK**s initialisation phase. It is not mandatory but advised to set `n` to a value which resembles the highest expected weight, e.g. here `n <= 4`. If higher weigths are encountered during the computation the constants will be adapted dynamically. This reflects the main idea of **LiSK**; prepare and save all needed constants during its initialisation phase in look-up tables and use them during the actual computation. Therefore, if any polylogarithms of higher weight are encountered new constants might have to be computed and stored. This, obviously, leads to longer evaluation times for the polylogarithms where the higher weights have been encountered. This situation should be avoided as much as possible. One might also compute the `Li(4,x)` and `Li22(x,y)` directly as

	LiSK::LiSK<T>(n,p).Li(4,x);
	LiSK::LiSK<T>(n,p).Li22(x,y);
	
but it is strongly not recommended due to the same reasons as mentioned above.

The second argument of the constructor sets the desired precison if `T=cln::cl_N` is chosen. This argument is superfluous in the double precision case. E.g. set `p=34` to obtain results with 34 digit precision. Internally all floating point values are set to this precision. This is also true for the initial complex arguments x and y supplied by the user. The user has to ensure that the supplied input values match the requested precision.

The second and third line of the constructor show the calls to the public wrappers for the classical polylogarithms of arbitrary weight `Li(n,x)` (here n=4`)` and the special function `Li22(x,y)`. The first four polylogarithms can also directly be addressed via
	
	lisk.Li1(x);
	lisk.Li2(x);
	lisk.Li3(x);
	lisk.Li4(x);
	
**LiSK** will throw a `std::runtime_error` if some error occurs. Hence, it is advised to put all calls to **LiSK** into a `try`-block like

	try{
		/* some code */
	}
	catch(std::runtime_error &e){
		std::cout << e.what() << std::endl;
	}
	
Last but not least it must be ensured that all expressions, initial and intermediate, are well defined. To this end a small positive imaginary part is added to the initial arguments x and y of `Li(n,x)` and `Li22(x,y)`, i.e. `x->x-iep`.
The value of `ep` is set to `10^(-(p-_offset))` during initialisation. Hereby defines `p` the requested precision in the constructor (`p=17` in double precision mode). The default value of `_offset` is 2. The user can change this value at the top of the **LiSK** header.

