#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cmath>
#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;


ZZ baby_step_giant_step(ZZ alpha,ZZ beta,ZZ order,ZZ moduls)
{
    ZZ u,v;

    ZZ m=conv<ZZ>(floor(sqrt(conv<int>(order)))+1);

    cout<<endl<<"m="<<m<<endl;

    ZZ b=PowerMod(alpha,m,moduls);

    cout<<endl<<"b="<<b<<endl;

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

    cout<<endl<<"\t"<<"#"<<"\t"<<"b^u"<<"\t"<<"beta*alpha^v"<<endl;

    cout<<endl;

    for(int i=0;i<=m;i++)
    {  
        cout<<"\t"<<i<<"\t"<<first[i]<<"\t"<<second[i]<<endl;
        
    }

    cout<<endl<<"u="<<u<<"\t"<<"v="<<v<<endl;
    cout<<endl<< "Result: "<< m*u-v<<endl;

    return m*u-v;

    
}

int main()
{
    
    baby_step_giant_step(conv<ZZ>(3),conv<ZZ>(57),conv<ZZ>(112),conv<ZZ>(113));


    return 0;
}