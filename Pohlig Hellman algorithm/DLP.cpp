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

struct prime
{
    ZZ base;
    ZZ degree;
};

vector<prime> factorization(ZZ number)
{
    
    ZZ degree_of_two=conv<ZZ>(0), prime_part=conv<ZZ>(0);

    split_on_even_and_prime_parts(number, prime_part, degree_of_two);

    //cout<<endl<<"\t"<<degree_of_two<<"\t"<<prime_part<<"\t"<<degree_of_two*prime_part<<endl;

    ZZ temp=SqrRoot(number);

    //cout<<endl<<"\t"<<temp<<endl;

    int size=conv<int>(temp);

    vector<prime> _factorization; //[1],[2],[3],[4],[5],[6].....

    prime two; two.base=conv<ZZ>(2); two.degree=degree_of_two;

    _factorization.push_back(two);


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


ZZ pohlig_hellman_algorithm_modify(ZZ alpha,ZZ beta,ZZ order,ZZ moduls)
{
    vector<ZZ>  a,m;      
    
    vector<prime> factorizations;   factorizations=factorization(order); 


    cout<<endl<<"Factorization number: "<<endl;

    cout<<endl<<"Prime"<<"\t"<<"Degree"<<endl;

    for(int i=0;i< factorizations.size();i++)
    {
        cout<<endl<<factorizations[i].base<<"\t"<<factorizations[i].degree<<endl;
    }

    cout<<endl;


    cout<<endl<<"Result of discrete logarithm: "<<endl;

    for(int i=0;i<factorizations.size();i++)
    {
        vector<ZZ> x;

        for(int j=0;j<factorizations[i].degree;j++)
        {
            ZZ degree=conv<ZZ>(0);
            
            if(j!=0)
            {
                for(int r=0;r<j;r++) 
                {
                    degree+=x[r]*power(factorizations[i].base,r);
                }
            }

            ZZ temp;
            
            if(j==0)
            {
                temp=horner_method(beta,order/power(factorizations[i].base,j+1),moduls);
            }
            else
            {
                temp=horner_method(beta*inverted_element(horner_method(alpha,degree,moduls),moduls),order/power(factorizations[i].base,j+1),moduls);
            }

            for(int r=0;r<factorizations[i].base;r++)
            {   
                if(horner_method(alpha,(order*conv<ZZ>(r))/factorizations[i].base,moduls)==temp)
                {
                    x.push_back(conv<ZZ>(r));
                    break;
                }
                if(horner_method(alpha,(order*conv<ZZ>(r))/factorizations[i].base,moduls)!=temp&&r==factorizations[i].base-1)
                {
                    cout<<endl<<" No such element in base."<<endl;
                }
            }

        }

        ZZ sum=conv<ZZ>(0);
    
        for(int j=0;j<x.size();j++)
        {
            sum+=x[j]*power(factorizations[i].base,j);
        }

        a.push_back(sum%power(factorizations[i].base,conv<int>(factorizations[i].degree)));
        m.push_back(horner_method(factorizations[i].base,factorizations[i].degree,moduls));
    
    }

    cout<<endl<<"- chines remainder theorem: "<<endl;

    for(int i=0;i<a.size();i++)
    {
        cout<<endl<<"\t"<<"x = "<<a[i]<<" mod "<<m[i]<<endl;
    }

    ZZ general_module=conv<ZZ>(0);

    ZZ result=chines_remainder_theorem(a,m,general_module);

    return result;
}


int main()
{
    cout<<endl<<pohlig_hellman_algorithm_modify(conv<ZZ>(3),conv<ZZ>(844),conv<ZZ>(2098),conv<ZZ>(2099));
    cout<<endl<<pohlig_hellman_algorithm_modify(conv<ZZ>(3),conv<ZZ>(46894),conv<ZZ>(24076),conv<ZZ>(24077));
    cout<<endl<<pohlig_hellman_algorithm_modify(conv<ZZ>(6),conv<ZZ>(7531),conv<ZZ>(8100),conv<ZZ>(8101));
    cout<<endl<<pohlig_hellman_algorithm_modify(conv<ZZ>(5),conv<ZZ>(7531),conv<ZZ>(1222),conv<ZZ>(1223));
    cout<<endl<<pohlig_hellman_algorithm_modify(conv<ZZ>(6),conv<ZZ>(4511),conv<ZZ>(3256),conv<ZZ>(3257));
    cout<<endl<<pohlig_hellman_algorithm_modify(conv<ZZ>(6),conv<ZZ>(52334),conv<ZZ>(14946),conv<ZZ>(14947));
    cout<<endl<<pohlig_hellman_algorithm_modify(conv<ZZ>(2),conv<ZZ>(21549),conv<ZZ>(51858),conv<ZZ>(51859));


    
	return 0;
}






