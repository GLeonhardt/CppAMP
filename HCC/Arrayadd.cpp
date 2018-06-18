#include <hc.hpp>  
#include <iostream>  
#include <ctime>
using namespace hc;  
  
const int size = 500;  
  
void main() {  
    int aCPP[size];  
    int bCPP[size];  
    int sumCPP[size];  
  
  	srand((unsigned)time(NULL));
	for (int i = 1; i < size; i++)
	{
		aCPP[i] = 1+ rand() % 101;
		bCPP[i] = 1+ rand() % 101;
	}
	
    // Create C++ AMP objects.  
    array_view<const int, 1> a(size, aCPP);  
    array_view<const int, 1> b(size, bCPP);  
    array_view<int, 1> sum(size, sumCPP);  
    sum.discard_data();  
  
    parallel_for_each(   
        // Define the compute domain, which is the set of threads that are created.  
        sum.extent,   
        // Define the code to run on each thread on the accelerator.  
 [=](index<1> idx) [[hc]]  
    {  
        sum[idx] = a[idx] + b[idx];  
    }  
    );  
  
    // Print the results. The expected output is "7, 9, 11, 13, 15".  
    for (int i = 0; i < size; i++) {  
        std::cout << sum[i] << "\n";  
    }  
}  
