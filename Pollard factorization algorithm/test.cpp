

#include <iostream>
#include <algorithm>
#include <vector>
#include <numeric>
#include <cmath>

using namespace std;


long int pollard_function(long int number)
{
   return (pow(number,2)+1);
}



void pollard_factorization_method_classical(unsigned int number,int a,int b)
{


   vector<unsigned  int> roots_of_polynoms;    


    
   cout<<endl<<"Root:"<<endl;

   roots_of_polynoms.push_back(1); 

   cout<<endl<<"\t Roots:"<<roots_of_polynoms[0]<<"\t"<<"gcd["<<roots_of_polynoms[0]<<","<<number<<"]="<<1<<endl;




   unsigned  int index=0;

   bool check=false;


   do
   {


      roots_of_polynoms.push_back(pollard_function(roots_of_polynoms[index]));

      index++;

      cout<<endl<<"Roots:"<<endl;

      for(unsigned int i=0;i<roots_of_polynoms.size()-1;i++)
      {
              
               cout<<endl<<"\t Root:"<<roots_of_polynoms[index]<<"\t"<<"gcd["<<roots_of_polynoms[index]<<"-"<<roots_of_polynoms[i]<<","<<number<<"]="<<__gcd((roots_of_polynoms[index]%number-roots_of_polynoms[i]%number),number)<<endl;
               
               if(__gcd((roots_of_polynoms[index]%number-roots_of_polynoms[i]%number),number)!=1||__gcd((roots_of_polynoms[index]%number-roots_of_polynoms[i]%number),number)==number)
               {
                  check=true;

                  break;
               }
      }

   }
   while(!check);

}


void pollard_factorization_method_first_modification(unsigned int number,int a,int b)
{

   cout<<endl<<"------------------------------------------------------------"<<endl;

   cout<<endl<<"Pollard factorization method (modification):"<<endl;


   vector<int> roots_of_polynoms;     


   bool check=false;   int index=0;

 
   while(!check)
   {
      if(index==0)
      {
         roots_of_polynoms.push_back(1); 
      }
      else
      {
         for(int i=2*(index-1);i<2*index;i++)
         {
            roots_of_polynoms.push_back(pollard_function(roots_of_polynoms[i])%number);
         }
      }
     
      if(roots_of_polynoms[2*index]%number-roots_of_polynoms[index]%number>0)
      {
         cout<<endl<<"\t"<<"gcd["<<roots_of_polynoms[2*index]<<"-"<<roots_of_polynoms[index]<<","<<number<<"]="<<__gcd((roots_of_polynoms[2*index]%number-roots_of_polynoms[index]%number),number)<<endl;

         if((__gcd((roots_of_polynoms[2*index]%number-roots_of_polynoms[index]%number),number)!=1||__gcd((roots_of_polynoms[2*index]%number-roots_of_polynoms[index]%number),number)==number))
         {
               check=true;    
               break;
         }
      }
      
      index++;

   }

   cout<<endl<<"------------------------------------------------------------"<<endl;
}  

   

void pollard_factorization_method_second_modification(int number,int a,int b)
{
   cout<<endl<<"------------------------------------------------------------"<<endl;

   cout<<endl<<"Pollard factorization method (modification):"<<endl;


   vector<int> roots_of_polynoms;  

   roots_of_polynoms.push_back(1);
   roots_of_polynoms.push_back(pollard_function(roots_of_polynoms[0]));

   int i=0;   int j=1;  int h=0; bool check=false;


   for(int l=0;l<number;l++)
   {
      while(!(pow(2,h)<=j&&j<pow(2,h+1)))
      {
         h++;
      }
      
      i=pow(2,h)-1;  h=0;

      roots_of_polynoms.push_back(pollard_function(roots_of_polynoms[j])%number);
            

      if((roots_of_polynoms[j]%number-roots_of_polynoms[i]%number)>0)
      {
         cout<<endl<<"\t"<<"gcd["<<roots_of_polynoms[j]<<"-"<<roots_of_polynoms[i]<<","<<number<<"]="<<__gcd((roots_of_polynoms[j]%number-roots_of_polynoms[i]%number),number)<<endl;

         if(__gcd((roots_of_polynoms[j]%number-roots_of_polynoms[i]%number),number)!=1)
         {
         
             break;
         }
      }
      
      j++;
   }

   cout<<endl<<"------------------------------------------------------------"<<endl;


}    

 int main()
 {

   int a=0,b=0;

   pollard_factorization_method_classical(8051,a,b);

   pollard_factorization_method_first_modification(8051,a,b);

   pollard_factorization_method_second_modification(8051,a,b);

   return 0;

 }