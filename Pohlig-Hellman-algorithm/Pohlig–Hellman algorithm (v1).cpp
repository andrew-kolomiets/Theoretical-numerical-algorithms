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
        x += n * u * a[i];
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


//  pohlig_hellman_algorithm ****************************************************** 

ZZ pohlig_hellman_algorithm(ZZ alpha,ZZ beta,ZZ order,ZZ moduls)
{
    vector <ZZ> a,m; // лишки та модулі для розв'язку системи з КТЛ +

    vector<ZZ> factorizations;      factorizations=factorization(order);  // факторизація на прості розклад +

   
    for(int i=0;i<factorizations.size();i++)//пробігаємося по всіх простих+
    {
        if(factorizations[i]!=conv<ZZ>(0))// зупиняємося на тих простих числах степені котрих не ненульові+
        {
            vector<ZZ> x, first_coefitient, second_coefitient; // x=x0+x1*p+... +  and псевдо альфа та бета псевдо +

            for(int j=0;j<factorizations[i]-1;j++) //пробігаємося по степенях простого числа для формування псевдо бета //+
            {
                if(j==0)
                {
                    first_coefitient.push_back(PowMod(beta,order/i,module)); //+
                }
                else
                {
                    first_coefitient.push_back(PowMod(beta,(order/conv<ZZ>(pow(i,j+1))),module)); //+
                }
            }

            for(int j=0;j<factorizations[i]-1;j++) //пробігаємося по степенях простого числа  числа для формування псевдо альфа
            {

                if(j==0)
                {
                    second_coefitient.push_back(conv<ZZ>(0)); //+
                }
                else
                {
                    ZZ temp=conv<ZZ>(0);
                    
                    for(int r=0;r<j+1;r++) // псевдо альфа степінь //+
                    {
                        temp+=x[r]*power(i,r);
                    } 

                    second_coefitient.push_back(inverted_element(PowMod(alpha,temp,module),module)); //+
                    
                }
            }

            for(int j=0;j<factorizations[i];j++)
            {
                ZZ prodact;

                if(j==0)
                {
                    prodact=first_coefitient[0];
                }
                else
                {
                    prodact=(first_coefitient[j]*second_coefitient[j])%module;
                }

                for(int h=0;h<i;h++)
                {   
                    if(PowMod(alpha,order*h/i,module)==prodact)
                    {
                        x.push_back(h);
                    }
                }
                
            }

            ZZ temp=conv<ZZ>(0);

            for(int j=0;j<x.size();j++) //x=x0+x1*p+...
            {
                temp=(temp+x[j]*PowMod(i,factorizations[i],module))%PowMod(i,factorizations[i],module);
            }
            
            a.push_back(temp);

        }
        
            
    }

    for(int i=0;i<factorizations.size();i++)
    {
        if(factorizations[i]!=conv<ZZ>(0))
        {
            m.push_back(PowerMod(conv<ZZ>(i),factorizations[i],order));
        }
       
    }

    ZZ general_module;
   
    chines_remainder_theorem(a,m,general_module);

}

//  main engine *******************************************************************

int main()
{
	
    cout<<"Result: "<<pohlig_hellman_algorithm(ZZ alpha,ZZ beta,ZZ order,ZZ moduls)<<endl;

	return 0;
}
