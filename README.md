# LiSK

**LiSK** is a lightweight C++ library for the numerical evaluation of classical polylogarithms Li(n,x) and the special function Li22(x,y) for arbitrary complex arguments. The evaluation is possible in double and in arbitrary precision arithmetics. The implementation is based on the algorithms presented in arXiv:1601.02649 (or click [here](http://arxiv.org/abs/1601.02649)).

If you need more information than provided in this file, contact the author:
Sebastian Kirchner (sebastian.t.kirchnerREMOVETHIS@gmail.com)

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
	
in the **example** directory. If **CLN** is not found automatically the _CLN\_INCLUDE\_DIR_ and _CLN\_LIB_ path can easily be set via

	ccmake .

---

##Usage

In the following **T** denotes one of the two currently supported types 
`std::complex<double>` or `cln::cl_N`. The first action should be to create a **LiSK** object of type `T` and precision `p` via
	
	LiSK::LiSK<T> lisk(n,p);
	
During object creation **LiSK** is initialised and all constants required for the computation of the `Li(n,x)` and `Li22(x,y)` are pre-computed. This reflects the main idea of **LiSK**; prepare and save all needed constants during its initialisation phase and use them during the actual computation. The constructor  `LiSK(n=4,prec=34)` features two optional arguments.

The first argument `n` defines the weight of the `Li(n,x)` up to which the constants are computed during **LiSK**s initialisation phase. It is not mandatory but advised to set `n` to a value which resembles the highest expected weights. If higher weights are encountered during the computation the constants will be adapted dynamically. This, obviously, leads to longer evaluation times for the polylogarithms for which the higher weights have been encountered. This situation should be avoided as much as possible.

The second argument of the constructor sets the desired precision if `T=cln::cl_N` is chosen. This argument is superfluous in the double precision case. E.g. set `p=34` to obtain results with 34 digit precision. Internally all floating point values are set to this precision. This is also true for the initial complex arguments x and y supplied by the user. The user has to ensure that the supplied input values match the requested precision.

Calling the public wrapper functions for the computation of `Li(m,x)` for positive integer weights `m` and `Li22(x,y)` at given points x and y is given by

	lisk.Li(m,x);
	lisk.Li22(x,y);
	
where `m` should be smaller than `n`. One might also call

	LiSK::LiSK<T>(n,p).Li(m,x);
	LiSK::LiSK<T>(n,p).Li22(x,y);

but this is strongly **not** recommended due to the above mentioned reasons. However, special wrapper functions exist for the classical polylogarithms with weights `n<=4`

	lisk.Li1(x);
	lisk.Li2(x);
	lisk.Li3(x);
	lisk.Li4(x);
	
In case some error is encountered **LiSK** will throw a `std::runtime_error`. Hence, it is advised to put all calls to **LiSK** into a `try`-block like

	try{
		/* some code */
	}
	catch(std::runtime_error &e){
		std::cout << e.what() << std::endl;
	}
	
Last but not least it must be ensured that all expressions, initial and intermediate, are well defined. To this end a small positive imaginary part is added to the initial arguments x and y of `Li(n,x)` and `Li22(x,y)`, i.e. `x->x-iep`.
The value of `ep` is set to `10^(-(p-_offset))` during initialisation. Hereby defines `p` the requested precision in the constructor (`p=17` in double precision mode). The default value of `_offset` is 2. The user can change this value at the top of the **LiSK** header.

## Miscellaneous

**LiSK** has been tested under **SUSE Linux** and is known to work with the **g++-4.8.3**, **clang++-3.5** and **icc 14.0.2** compiler versions using the GNU C++ standard library. At the moment some problem in **CLN** prevent using the compiler optimisation on **OS X** with the LLVM C++ standard library (OK without optimisation).