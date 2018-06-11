#include <iostream>
#include <amp.h>  
#include <iostream>
#include <ctime>

using namespace concurrency;  

void initialize_array(std::vector<int> &v_data, unsigned size)
{
    for(unsigned i=0; i<size; ++i)
    {
        v_data[i] = 1+ rand() % 101;
    }
}

void mxm_amp_simple(int M, int N, int W, const std::vector<int>& va, const std::vector<int>& vb, 
      std::vector<int>& vresult)
{
    if ((va.size() != M*N) || (vb.size() != N*W) || (vresult.size() != M*W))
        throw "Expected matrix dimension result(M*W) = a(MxN) * b(N*W)";

   extent<2> e_a(M, N), e_b(N, W), e_c(M, W);

   // Copy in
   array_view<const int, 2> av_a(e_a, va); 
   array_view<const int, 2> av_b(e_b, vb); 
   array_view<int, 2> av_c(e_c, vresult);
   av_c.discard_data();

   // Compute - outer 2 for loops of CPU is replaced by a parallel_for_each
   parallel_for_each(av_c.extent, [=](index<2> idx) restrict(amp)
   {
      int result = 0;
      for(int i = 0; i < av_a.extent[1]; ++i)
      {
         index<2> idx_a(idx[0], i);
         index<2> idx_b(i, idx[1]);

         result += av_a[idx_a] * av_b[idx_b];
      }

      av_c[idx] = result;
   });
   // explicitly about copying out data
   av_c.synchronize();
}
    

void main() {  
   accelerator default_device;
      std::wcout << L"Using device : " << default_device.get_description() << std::endl;

   srand(2012);

   const int M = 256;
   const int N = 256;
   const int W = 256;
    
   std::vector<int> v_a(M * N);
   std::vector<int> v_b(N * W);
   std::vector<int> v_c_simple(M * W);

   initialize_array(v_a, M * N);
   initialize_array(v_b, N * W);

   mxm_amp_simple(M, N, W, v_a, v_b, v_c_simple);

   for(int x=0;x<M*W;x++){
       cout<<v_c_simple[x];
       cout<<"\n";
    }
      
   }

}  
