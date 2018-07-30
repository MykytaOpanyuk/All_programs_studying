#define BOOST_TEST_MODULE TEST
#define BOOST_TEST_DYN_LINK

#include "sema.h"
#include <boost/test/unit_test.hpp>

using namespace boost::unit_test;

BOOST_AUTO_TEST_CASE(my_test)
{
    	modified_semaphore t(2, 2, 600);
		
	cout << "Num of pushes :" << all_pushes << "; and num of pops: " << all_pops << endl; 
    	BOOST_CHECK(all_pushes == all_pops);
}
