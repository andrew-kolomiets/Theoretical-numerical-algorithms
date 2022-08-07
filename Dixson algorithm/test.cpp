#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <NTL/ZZ.h>


using namespace std;
using namespace NTL;

struct prime //exelent
{
    ZZ base;
    ZZ degree;
};

ZZ gcd(ZZ  first_number, ZZ second_number) //exelent
{
    ZZ  result;

    if(first_number<second_number)
    {
        ZZ shift=second_number;

        second_number=first_number;

        first_number=shift;
    }

    while (first_number % second_number != 0)
    {
        ZZ shift = second_number;

        second_number = first_number % second_number;

        first_number = shift;
    }

    if (first_number % second_number == 0)
    {
        result=second_number;
    }
    else
    {
        cout<<endl<<"Error function gcd."<<endl;
    }

    return result;
}

void split_on_even_and_prime_parts(ZZ number, ZZ &prime_part, ZZ &degree_of_two) //exelent
{

    ZZ temp = number;  

    prime_part = conv<ZZ>(0);  

    degree_of_two = conv<ZZ>(0);
   
    while (!((temp & conv<ZZ>(1)) == conv<ZZ>(1)))
    {
        temp = temp >> 1;
        degree_of_two++;
    }

    prime_part = temp;
}

vector<prime> factorization(ZZ number)//exelent
{
    
    ZZ degree_of_two=conv<ZZ>(0), prime_part=conv<ZZ>(0);

    split_on_even_and_prime_parts(number, prime_part, degree_of_two);

    ZZ temp=SqrRoot(number);

    int size=conv<int>(temp);

    vector<prime> _factorization;	prime two; two.base=conv<ZZ>(2); two.degree=degree_of_two;	 _factorization.push_back(two);


    for(int i=3;i<size;i++)
    {
        if(prime_part%conv<ZZ>(i)==0 && _factorization[_factorization.size()-1].base!=conv<ZZ>(i))
        {
            
            prime var; var.base=conv<ZZ>(i); var.degree=conv<ZZ>(1);
            
            _factorization.push_back(var);

            prime_part=prime_part/conv<ZZ>(i);

            i--;

        }
        if(prime_part%conv<ZZ>(i)==0 && _factorization[_factorization.size()-1].base==conv<ZZ>(i))
        {
            _factorization[_factorization.size()-1].degree++;
            prime_part=prime_part/conv<ZZ>(i);
            i--;
        }
    }

    if(prime_part!=1)
    {
        prime var; var.base=prime_part; var.degree=conv<ZZ>(1);

       _factorization.push_back(var);
    }
    

    
    return _factorization;
}

bool compare_with_base(vector<ZZ> base,vector<prime> number)//exelent
{
	for(int i=0;i<number.size();i++)
	{
		for(int j=0;j<base.size();j++)
		{
			if(number[i].base==base[j]&&number[i].degree!=conv<ZZ>(0))
			{
				continue;
			}
			if(number[i].base!=base[j] && j==base.size()-1&&number[i].degree!=conv<ZZ>(0))
			{
				return false;
			}
		}
	}

	return true;
}

vector <ZZ> sum_vector(vector<ZZ> first,vector<ZZ> second) //exelent
{
	vector <ZZ> sum;

	int size=min(first.size(),second.size());

	for(int i=0;i<size;i++)
	{
		sum.push_back((first[i]+second[i])%conv<ZZ>(2));
	}
	
	return sum;
}

vector<int> convert_to_binary_modify(ZZ number) //exelent
{
	vector<int> binary_representation;

    for (unsigned long long int i = 0; number > 0; i++)
    {
        binary_representation.push_back(conv<int>(conv<ZZ>(number) & conv<ZZ>(1)));
        number = number >> 1;
    }

	return binary_representation;
}

vector<ZZ> get_factors(ZZ number, vector<ZZ> numbers)// exelent
{
	ZZ x=conv<ZZ>(1);	for(int i=0;i<numbers.size();i++) { 	x=x*numbers[i]%number; }

	ZZ y=conv<ZZ>(1);	for(int i=0;i<numbers.size();i++)	{ y= y*((numbers[i]*numbers[i])%number); }		y=(SqrRoot(y))%number;

	vector<ZZ> result;	result.push_back(gcd(x-y, number));	 result.push_back(gcd(x+y, number));
	
	return result;
}

vector<ZZ> pading(vector<ZZ> base,ZZ number,ZZ module)//exelent
{
	vector<ZZ> result(base.size(),conv<ZZ>(0));     vector<prime> factors=factorization((number*number)%module);

	for(int i=0;i<factors.size();i++)
	{
		for(int j=0;j<base.size();j++)
		{
			if(factors[i].base==base[j])
			{
				result[j]=factors[i].degree;
			}
		}
	}

	return result;
}

vector<ZZ> combination_of_vectors_and_finding_roots(vector<ZZ> base,ZZ number, vector<ZZ> all_number)//exelent
{
	vector<ZZ> factors(2,conv<ZZ>(0)); 

	for(int i=1;i<pow(2,all_number.size());i++)
	{
		vector<int> binary_representation=convert_to_binary_modify(conv<ZZ>(i));

		vector<ZZ> temp(base.size());	for(int j=0;j<base.size();j++){ temp[j]=conv<ZZ>(0);}

		for(int j=0;j<binary_representation.size();j++)
		{
			if(binary_representation[j]==1)
			{
				temp=sum_vector(temp,pading(base,all_number[j],number));
			}

		}

		bool triger=true;

		for(int j=0;j<binary_representation.size();j++)
		{
			if(temp[j]%conv<ZZ>(2)==conv<ZZ>(0))
			{
				continue;
			}
			else
			{
				triger=false;
			}
		}

		if(triger)
		{	
			vector <ZZ> root;

			for(int j=0;j<binary_representation.size();j++)
			{
				if(binary_representation[j]==1)
				{
					root.push_back(all_number[j]);
				}
			}

		
			factors=get_factors(number, root);

			if(factors[0]==conv<ZZ>(1)||factors[0]==number||factors[1]==conv<ZZ>(1)||factors[1]==number)
			{
				factors[0]=conv<ZZ>(0);	factors[1]=conv<ZZ>(0);
                
				continue;
			}
			else
			{
				return factors;
			}


		}
		
	}

	return factors;
}

vector<ZZ> dixson(ZZ number)//exelent
{
	
	bool triger=true;
	

	ZZ start_search_number=SqrRoot(number)+conv<ZZ>(1);


	for(int size_base=2;triger==true;size_base++)
	{
		vector<ZZ> base; 	
        
        base.push_back(conv<ZZ>(2)); for(int i=1;i<size_base;i++)
        {	
            base.push_back(conv<ZZ>(2*i+1));  
        }


		vector<ZZ> temp;

		for(int j=0;j<base.size()+1;j++)
		{
			if(compare_with_base(base,factorization((start_search_number*start_search_number)%number))) 
            {
                temp.push_back(start_search_number);   
            }
            
			start_search_number=start_search_number+conv<ZZ>(1);
		}


		vector<ZZ> factors=combination_of_vectors_and_finding_roots(base,number,temp);

		if(factors[0]!=conv<ZZ>(0)&&factors[0]!=conv<ZZ>(0))
        {  
            triger=false;
            return factors; 
        }
	}

	cout<<endl<<"Problem dixson algorithm."<<endl;

}


int main()
{   
   
	cout<<endl<<"Input number:"<<endl;


	vector<ZZ> root=dixson(conv<ZZ>(4633));

	cout<<endl<<"Result: "<<root[0]<<" "<<root[1]<<endl;


	return 0;
}
