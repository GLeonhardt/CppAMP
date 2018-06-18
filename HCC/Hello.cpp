#include <iostream> 
#include <hc.hpp> 
using namespace hc; 

int main() 
{ 
  int v[11] = {'G', 'd', 'k', 'k', 'n', 31, 'v', 'n', 'q', 'k', 'c'};

  array_view<int> av(11, v); 
  parallel_for_each(av.extent, [=](index<1> idx) [[hc]]
  { 
    av[idx] += 1; 
  });

  for(int i = 0; i < 11; i++) 
    std::cout << static_cast<char>(av[i]); 
}
