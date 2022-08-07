//  libs **************************************************************************
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <NTL/ZZ.h>

//  work spaces *******************************************************************

using namespace std;
using namespace NTL;

//  modular functionality ********************************************************* 


ZZ mod(ZZ number, ZZ module)
{
	ZZ numb_by_module;

	if (number < conv<ZZ>(0))
	{
		if (number / module != conv<ZZ>(0))
		{
			numb_by_module = module - (abs(number) - module * (abs(number) / module));
		}
		else
		{
			numb_by_module = module - abs(number);
		}
	}
	else if (number / module != conv<ZZ>(0))
	{
		numb_by_module = number - module * (number / module);
	}
	else
	{
		numb_by_module = number;
	}

	return numb_by_module;
}

ZZ  inverted_element(ZZ number, ZZ moduls)
{
	number = mod(number, moduls);

	if (__gcd(number, moduls) ==conv<ZZ>(1))
	{
		ZZ b0 = moduls, t, q;
		ZZ x0 = conv<ZZ>(0), x1 = conv<ZZ>(1);
		if (moduls == conv<ZZ>(1)) return conv<ZZ>(1);
		while (number > conv<ZZ>(1)) {
			q = number / moduls;
			t = moduls, moduls = number % moduls, number = t;
			t = x0, x0 = x1 - q * x0, x1 = t;
		}
		if (x1 < conv<ZZ>(0)) x1 += b0;

		return x1;
	}
	if (__gcd(number, moduls) > conv<ZZ>(1))
	{ 
		return conv<ZZ>(-1); //code number if roots more or does`t exist
	}
}

ZZ chines_remainder_theorem(vector <ZZ> &a,vector<ZZ> &m, ZZ &general_module)
{
    ZZ M = conv<ZZ>(1), n, x = conv<ZZ>(0), u;

    for (int i = 0; i < m.size(); ++i)
    {
         M *= m[i];
    }
       
    for (int i = 0; i < m.size(); ++i)
    {
        n = M / m[i];
        u = inverted_element(n % m[i], m[i]);
        
        x += n *u * a[i];
    }

    x %= M;

    if (x < conv<ZZ>(0))
    {
        x += M;
    }
       
    general_module = M;
    
        
    return x;
}

ZZ baby_step_giant_step(ZZ alpha,ZZ beta,ZZ order,ZZ moduls)
{
    ZZ u,v;

    ZZ m=conv<ZZ>(floor(sqrt(conv<int>(order)))+1);

    ZZ b=PowerMod(alpha,m,moduls);

    vector <ZZ> first;     vector <ZZ> second;

    for(int i=0;i<=m;i++)
    {
        first.push_back(power(b,i)%moduls);  second.push_back((beta*power(alpha,i))%moduls);
       
    }

    bool triger=true;
    
    for(int i=0;i<=first.size();i++)
    {
        for(int j=0;j<=second.size();j++)
        {
            if(first[i]==second[j])
            {
                u=i;     v=j;

                triger=false;

                break;
            }
        }

        if(triger==false)
        {
            break;
        }
    }

    return m*u-v;
}

void split_on_even_and_prime_parts(ZZ number, ZZ &prime_part, ZZ &degree_of_two)
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

vector<ZZ> factorization(ZZ number)
{
    
    ZZ degree_of_two=conv<ZZ>(0), prime_part=conv<ZZ>(0);

    split_on_even_and_prime_parts(number, prime_part, degree_of_two);

    //cout<<endl<<"\t"<<degree_of_two<<"\t"<<prime_part<<"\t"<<degree_of_two*prime_part<<endl;

    ZZ temp=SqrRoot(number);

    //cout<<endl<<"\t"<<temp<<endl;

    int size=conv<int>(temp);

    vector<ZZ> degree(size+1,conv<ZZ>(0)); //[1],[2],[3],[4],[5],[6].....

    degree[2]=degree_of_two;

    for(int i=3;i<size+1;i++)
    {
        if(prime_part%i==0)
        {
            prime_part=prime_part/conv<ZZ>(i);
            degree[i]+=1;
            i--;;
        }

    }

    return degree;
}

void convert_to_binary_modify(ZZ number, vector<int> &binary_representation)
{

    for (unsigned long long int i = 0; number > 0; i++)
    {
        binary_representation.push_back(conv<int>(conv<ZZ>(number) & conv<ZZ>(1)));
        number = number >> 1;
    }
}

ZZ horner_method(ZZ number, ZZ degree, ZZ module)
{

    vector<int> binary_representation;

    convert_to_binary_modify(degree, binary_representation);

    ZZ result = conv<ZZ>(1);

    for (long long int i = (long long int)(binary_representation.size()) - 1; i >= 0; i--)
    {
        result = (result * result) % module;

        if (binary_representation[i] == 1)
        {
            result = (result * number) % module;
        }
    }

    return result;
}

//  pohlig_hellman_algorithm ****************************************************** 


ZZ pohlig_hellman_algorithm_simple(ZZ alpha,ZZ beta,ZZ order,ZZ moduls)
{
    vector <ZZ> a,m; 

    vector<ZZ> factorizations;      factorizations=factorization(order);  

    for(int i=0;i<factorizations.size();i++)
    {
        if(factorizations[i]!=0)
        {
            a.push_back(baby_step_giant_step(alpha,beta,order,moduls));
            m.push_back(PowerMod(conv<ZZ>(i),factorizations[i],moduls));
        }

    }

    ZZ general_module;
   
    return chines_remainder_theorem(a,m,general_module);

}


ZZ pohlig_hellman_algorithm_modify(ZZ alpha,ZZ beta,ZZ order,ZZ moduls)
{

    vector<ZZ>  a,m, factorizations;      
    
    factorizations=factorization(order);  

    for(int i=0;i<factorizations.size();i++)
    {
        
        if(factorizations[i]!=0)
        {
            vector<ZZ> x;

            for(int j=0;j<factorizations[i];j++)
            {
                ZZ degree=conv<ZZ>(0);
                    
                for(int r=0;r<j;r++) 
                {
                    degree+=x[r]*power(conv<ZZ>(i),r);
                }

                ZZ temp=horner_method(beta*inverted_element(horner_method(alpha,degree,moduls),moduls),order/power(conv<ZZ>(i),j+1),moduls);

                for(int r=0;r<i;r++)
                {   
                    if(horner_method(alpha,order*conv<ZZ>(r)/conv<ZZ>(i),moduls)==temp)
                    {
                        x.push_back(conv<ZZ>(r));
                    }
                }

            }

            ZZ sum=conv<ZZ>(0);

            for(int j=0;j<x.size();j++)
            {
                sum+=x[j]*power(conv<ZZ>(i),j);
            }

            
            a.push_back(sum%power(conv<ZZ>(i),conv<int>(factorizations[i])));
            m.push_back(horner_method(conv<ZZ>(i),factorizations[i],moduls));
        }

    }

    ZZ general_module=conv<ZZ>(0);

    return chines_remainder_theorem(a,m,general_module);
}


int main()
{
	
    cout<<"Result simple: "<<pohlig_hellman_algorithm_simple(conv<ZZ>(5),conv<ZZ>(11),conv<ZZ>(96),conv<ZZ>(97))<<endl;

    cout<<"Result modify: "<<pohlig_hellman_algorithm_modify(conv<ZZ>(5),conv<ZZ>(11),conv<ZZ>(96),conv<ZZ>(97))<<endl;


	return 0;
}
