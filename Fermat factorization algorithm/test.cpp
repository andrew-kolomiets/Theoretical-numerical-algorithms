

#include <iostream>
#include <cmath>
 
using namespace std;
 

void fermat_factorization(int number,int a, int b)
{
   cout<<endl<<"------------------------------------------------------------"<<endl;

   cout<<endl<<"Fermat factorization method:"<<endl;

   int r;   int x=(int)sqrt(number);  int y=0;


   do
   {

      r=pow(x,2)-pow(y,2)-number;


      if(r==0)
      {
         cout<<endl<<"r="<<r<<"\t"<<"["<<x<<","<<y<<"]"<<"\t" <<"r=0"<<endl;

         a=x-y;  b=x+y;
          
         cout<<endl<<"Number factorized on next numbers:"<<endl;

         cout<<endl<<"a="<<a<<endl;
         cout<<endl<<"b="<<b<<endl;

         break;
      }

      if(r<0)
      {
         cout<<endl<<"r="<<r<<"\t"<<"["<<x<<","<<y<<"]"<<"\t" <<"r<0"<<endl;

         x++;
      }

      if (r>0)
      {
         cout<<endl<<"r="<<r<<"\t"<<"["<<x<<","<<y<<"]"<<"\t" <<"r>0"<<endl;

         y++;
      }

   }
   while(r!=0);

   cout<<endl<<"------------------------------------------------------------"<<endl;

}
                     
   
 int main()
 {
   
   int a;
   int b;     

   fermat_factorization(8051,a,b);

   return 0;
 }
