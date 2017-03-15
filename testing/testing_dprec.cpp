#include <iostream>
#include <memory>
#include "gtest/gtest.h"
#include "lisk.hpp"
#include <ginac/ginac.h>

using namespace std;


/**
 * Testing the public high-level functions of LiSK using Google Test.
 * The results are compared to GiNaC results, i.e. a working GiNaC 
 * installation must be provided and is not provided within this test suit. 
 * GiNaC can be downloaded from http://www.ginac.de
 * 
 * The Google Test framework will be downloaded from its git repository
 * when cmake is executed.
 */

// Setting up the testing environment
const unsigned TEST_PREC  = 17;	// Double-precision in GiNaC, should not be changed
const unsigned TEST_ERROR = 13;	// Acceptable error in comparison
const unsigned MAX_WEIGHT = 10; // max weight in LiSK init, should at least be equal to highest weight in TEST_VALUES
const auto TEST_VALUES = ::testing::Values(1,2,3,4,5,6,10/*,15,20*/);	// Specify which weights n of Li_n to test

/* NO CHANGES FROM HERE */

// Define function as long as not all C++14 features are supported
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


// LiSK test class
class LiSKTest : public ::testing::TestWithParam<unsigned> {
	
protected:
	//	Local instance of LiSK
	std::unique_ptr<LiSK::LiSK<std::complex<double>>> _lisk = make_unique<LiSK::LiSK<std::complex<double>>>(MAX_WEIGHT, TEST_PREC);

};


// Test classical polylogarithms
// Due to GiNaC's square root function we include the phase space points in the test
TEST_P(LiSKTest, ClassicalLogarithms){
	
	using namespace GiNaC;
	
	// Set precision
	Digits = TEST_PREC;
	cln::cl_F error = cln::cl_F(("1.e-"+std::to_string(TEST_ERROR)+"_"+std::to_string(TEST_PREC)).c_str());
	
	// Test points for each region
	const vector<numeric> x = {
		numeric(1,4)+I*numeric(1,4),					// Region: Re(x)<1/2 && |x|<1
		numeric(3,4)+I*numeric(1,4),					// Region: Re(x)>1/2 && |x-1|<1
		numeric(2)+I*numeric(4),						// Region: All other values...
		numeric(1,100)+I*sqrt(GiNaC::numeric(9899))/100	// Region: |x|~1 ...
	};
	
	// Get weights
	const unsigned &w = GetParam();
	
	// Run all points for given weight
	for(int i=0; i<x.size(); ++i){
	
		auto Gres = ex_to<numeric>(GiNaC::Li(w,x[i]).evalf()).to_cl_N();
		auto Lres = this->_lisk->Li(w, std::complex<double>(x[i].to_double(), imag(x[i]).to_double()));
	
		EXPECT_NEAR(cln::double_approx(cln::realpart(Gres)),
			Lres.real(),cln::double_approx(error));
		EXPECT_NEAR(cln::double_approx(cln::imagpart(Gres)),
			Lres.imag(),cln::double_approx(error));
	}
}


// Set weights for classical polylogarithms
INSTANTIATE_TEST_CASE_P(DoublePrec, LiSKTest, TEST_VALUES);


// Test Li_22
// Due to GiNaC's square root function we include the phase space points in the test
TEST_F(LiSKTest, Li22){
	
	using namespace GiNaC;
	
	// Set precision
	Digits = TEST_PREC;
	cln::cl_F error = cln::cl_F(("1.e-"+std::to_string(TEST_ERROR)+"_"+std::to_string(TEST_PREC)).c_str());
	
	// Test points (x,y) for each region
	const vector<std::pair<numeric, numeric>> x = {
		std::make_pair(numeric(1,8)+I*numeric(1,8), numeric(1,8)+I*numeric(1,8)),						// |x|<1 && |xy|<1
		std::make_pair(numeric(2)+I*numeric(1,8), numeric(1,8)+I*numeric(1,8)),							// |x|>1 && |xy|<1
		std::make_pair(numeric(2)+I*numeric(1,8), numeric(2)+I*numeric(1,8)),							// |x|>1 && |xy|>1
		std::make_pair(numeric(1,2)+I*numeric(1,2), numeric(2)+I*numeric(2)),							// |x|<1 && |xy|>1
		std::make_pair(numeric(2,10)+I*numeric(-9,10), numeric(2,10)+I*numeric(-9,10)),					// |x|~1 && |xy|~1
		std::make_pair(numeric(1,100)+I*sqrt(numeric(11))/100, numeric(1,100)+I*sqrt(numeric(11))/100),	// |x|~0 && |xy|~0
		std::make_pair(numeric(2), numeric(1,2)),														// Special point: x=2 and y=1/2
		std::make_pair(numeric(1,2), numeric(2))														// Special point: x=1/2 and y=2			
	};
	
	// Run all points
	for(int i=0; i<x.size(); ++i){

		auto Gres = ex_to<numeric>(GiNaC::Li(GiNaC::lst(2,2),GiNaC::lst(x[i].first, x[i].second)).evalf()).to_cl_N();
		auto Lres = this->_lisk->Li22(std::complex<double>(x[i].first.to_double(), imag(x[i].first).to_double()), 
				std::complex<double>(x[i].second.to_double(), imag(x[i].second).to_double()));
				
		EXPECT_NEAR(cln::double_approx(cln::realpart(Gres)),
			Lres.real(), cln::double_approx(error));
		EXPECT_NEAR(cln::double_approx(cln::imagpart(Gres)),
			Lres.imag(), cln::double_approx(error));
	}
}