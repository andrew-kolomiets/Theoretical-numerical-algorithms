#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

struct prime // exelent
{
	ZZ base;
	ZZ degree;
};

ZZ gcd(ZZ first_number, ZZ second_number) // exelent
{
	ZZ result;

	if (first_number < second_number)
	{
		ZZ shift = second_number;

		second_number = first_number;

		first_number = shift;
	}

	while (first_number % second_number != 0)
	{
		ZZ shift = second_number;

		second_number = first_number % second_number;

		first_number = shift;
	}

	if (first_number % second_number == 0)
	{
		result = second_number;
	}
	else
	{
		cout << endl
			 << "Error function gcd." << endl;
	}

	return result;
}

void split_on_even_and_prime_parts(ZZ number, ZZ &prime_part, ZZ &degree_of_two) // exelent
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

vector<prime> factorization(ZZ number) // exelent
{
	bool triger = false;

	if (number < ZZ(0))
	{
		triger = true;
		number = abs(number);
	}

	if (number == ZZ(1) && triger == true)
	{
		vector<prime> _factorization;
		prime var;
		var.base = ZZ(-1);
		var.degree = conv<ZZ>(1);
		_factorization.push_back(var);
		return _factorization;
	}

	if (number == ZZ(0))
	{
		vector<prime> _factorization;
		prime var;
		var.base = conv<ZZ>(1);
		var.degree = conv<ZZ>(0);
		_factorization.push_back(var);
		return _factorization;
	}

	if (number == ZZ(1))
	{
		vector<prime> _factorization;
		prime var;
		var.base = conv<ZZ>(1);
		var.degree = conv<ZZ>(1);
		_factorization.push_back(var);
		return _factorization;
	}

	ZZ degree_of_two = ZZ(0), prime_part = ZZ(0);
	split_on_even_and_prime_parts(number, prime_part, degree_of_two);

	ZZ temp = SqrRoot(number) + ZZ(1);
	int size = conv<int>(temp);

	vector<prime> _factorization;

	if (triger == true)
	{
		prime minus_one;
		minus_one.base = ZZ(-1);
		minus_one.degree = ZZ(1);
		_factorization.push_back(minus_one);
	}

	if (degree_of_two != ZZ(0))
	{
		prime two;
		two.base = conv<ZZ>(2);
		two.degree = degree_of_two;
		_factorization.push_back(two);
	}

	if (prime_part > ZZ(1))
	{
		for (int i = 3; i < size; i++)
		{

			if (_factorization.size() == 0)
			{
				if (prime_part % conv<ZZ>(i) == 0)
				{
					prime var;
					var.base = conv<ZZ>(i);
					var.degree = conv<ZZ>(1);
					_factorization.push_back(var);

					prime_part = prime_part / conv<ZZ>(i);

					i--;
				}

				if (prime_part % conv<ZZ>(i) == ZZ(0) && _factorization[_factorization.size() - 1].base == conv<ZZ>(i))
				{
					_factorization[_factorization.size() - 1].degree++;
					prime_part = prime_part / conv<ZZ>(i);
					i--;
				}
			}
			if (_factorization.size() >= 1)
			{
				if (prime_part % conv<ZZ>(i) == 0 && _factorization[_factorization.size() - 1].base != conv<ZZ>(i))
				{
					prime var;
					var.base = conv<ZZ>(i);
					var.degree = conv<ZZ>(1);
					_factorization.push_back(var);

					prime_part = prime_part / conv<ZZ>(i);

					i--;
				}

				if (prime_part % conv<ZZ>(i) == ZZ(0) && _factorization[_factorization.size() - 1].base == conv<ZZ>(i))
				{
					_factorization[_factorization.size() - 1].degree++;
					prime_part = prime_part / conv<ZZ>(i);
					i--;
				}
			}
		}

		if (prime_part != ZZ(1))
		{
			prime var;
			var.base = prime_part;
			var.degree = conv<ZZ>(1);
			_factorization.push_back(var);
		}
	}

	return _factorization;
}

vector<ZZ> sum_vector(vector<ZZ> first, vector<ZZ> second) // exelent
{
	vector<ZZ> sum;

	int size = min(first.size(), second.size());

	for (int i = 0; i < size; i++)
	{
		sum.push_back((first[i] + second[i]) % conv<ZZ>(2));
	}

	return sum;
}

vector<int> convert_to_binary_modify(ZZ number) // exelent
{
	vector<int> binary_representation;

	for (unsigned long long int i = 0; number > 0; i++)
	{
		binary_representation.push_back(conv<int>(conv<ZZ>(number) & conv<ZZ>(1)));
		number = number >> 1;
	}

	return binary_representation;
}

ZZ check_on_negative_numbers_and_conversation(ZZ number_which_cheak, ZZ number_which_factor) // exelent
{
	if (number_which_factor / ZZ(2) > number_which_cheak)
	{
		return number_which_cheak;
	}
	else
	{
		return (number_which_cheak - number_which_factor);
	}
}

void computations(ZZ &alpha_current, ZZ &u_current, ZZ &v_current, ZZ &a_current, ZZ &D) // exelent
{
	ZZ next_alpha, next_u, next_v, next_a;

	next_v = (D - power(u_current, 2)) / v_current;

	next_alpha = (SqrRoot(D) + u_current) / next_v;

	next_a = next_alpha;
	next_u = next_a * next_v - u_current;

	alpha_current = next_alpha;
	u_current = next_u;
	v_current = next_v;
	a_current = next_a;
}

vector<ZZ> table_smooth(ZZ number, int size) // exelent
{
	vector<ZZ> S(size + 1, ZZ(0));
	S[0] = ZZ(-1);

	vector<ZZ> a(size + 1, ZZ(0));
	a[0] = ZZ(0);
	vector<ZZ> b_mod_n(size + 1, ZZ(0));
	b_mod_n[0] = ZZ(1);
	vector<ZZ> pow_b_mod_n(size + 1, ZZ(0));
	pow_b_mod_n[0] = ZZ(1);

	for (int i = 0; i < size + 1; i++)
	{
		S[i] = ZZ(i) - ZZ(1);
	}

	ZZ alpha_current = SqrRoot(number), u_current = alpha_current, v_current = ZZ(1), a_current = alpha_current, D = number;

	a[1] = a_current;
	b_mod_n[1] = a_current;

	for (int i = 2; i < size + 1; i++)
	{
		computations(alpha_current, u_current, v_current, a_current, D);

		a[i] = a_current;
		b_mod_n[i] = (b_mod_n[i - 1] * a_current + b_mod_n[i - 2]) % number;
	}

	for (int i = 0; i < size + 1; i++)
	{
		pow_b_mod_n[i] = check_on_negative_numbers_and_conversation(power(b_mod_n[i], 2) % number, number);
	}

	cout << endl
		 << "-start output table of number:" << endl;

	{
		for (int i = 0; i < size + 1; i++)
		{
			cout << "\t" << S[i];
		}

		cout << endl;

		for (int i = 0; i < size + 1; i++)
		{
			cout << "\t" << a[i];
		}

		cout << endl;

		for (int i = 0; i < size + 1; i++)
		{
			cout << "\t" << b_mod_n[i];
		}

		cout << endl;

		for (int i = 0; i < size + 1; i++)
		{
			cout << "\t" << pow_b_mod_n[i];
		}
	}

	cout << endl
		 << "-end output table of number" << endl;

	vector<ZZ> result;

	for (int i = 1; i < b_mod_n.size(); i++)
	{
		result.push_back(b_mod_n[i]);
	}

	return b_mod_n;
}

bool isSubset(vector<ZZ> a, vector<ZZ> b)
{
	for (int i = 0; i < a.size(); i++)
	{
		bool found = false;

		for (int j = 0; j < b.size(); j++)
		{
			if (a[i] == b[j])
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			return false;
		}
	}

	return true;
}

bool compare_with_base(vector<ZZ> base, vector<prime> number) // exelent
{
	vector<ZZ> base_of_current_number(number.size(), ZZ(0));

	for (int i = 0; i < number.size(); i++)
	{
		base_of_current_number[i] = number[i].base;
	}

	if (isSubset(base_of_current_number, base))
	{
		return true;
	}
	else
	{
		return false;
	}
}

vector<ZZ> get_factors(ZZ number, vector<ZZ> numbers) // exelent
{
	// cout<<endl;
	// for (int i = 0; i < numbers.size(); i++)
	// {
	// 	cout<<numbers[i]<<" ";
	// }
	// cout<<endl;
	
	ZZ x = conv<ZZ>(1);

	for (int i = 0; i < numbers.size(); i++)
	{
		x = x * numbers[i] % number;
	}

	ZZ y = conv<ZZ>(1);

	for (int i = 0; i < numbers.size(); i++)
	{
		y = y * ((numbers[i] * numbers[i]) % number);
	}
	y = (SqrRoot(y)) % number;

	y=check_on_negative_numbers_and_conversation(y,number);

	vector<ZZ> result;

	result.push_back(gcd(x - y, number));
	result.push_back(gcd(x + y, number));

	return result;
}

vector<ZZ> pading(vector<ZZ> base, ZZ number, ZZ module) // exelent
{
	vector<ZZ> result(base.size(), conv<ZZ>(0));

	vector<prime> factors = factorization(check_on_negative_numbers_and_conversation((number * number) % module, module));

	for (int i = 0; i < factors.size(); i++)
	{
		for (int j = 0; j < base.size(); j++)
		{
			if (factors[i].base == base[j])
			{
				result[j] = factors[i].degree;
			}
		}
	}

	return result;
}

vector<ZZ> combination_of_vectors_and_finding_roots(vector<ZZ> base, ZZ number, vector<ZZ> all_number) // okey
{
	vector<ZZ> factors(2, ZZ(0));

	for (int i = 1; i < pow(2, all_number.size()); i++)
	{
		vector<int> binary_representation = convert_to_binary_modify(conv<ZZ>(i));

		vector<ZZ> temp(base.size());
		for (int j = 0; j < base.size(); j++)
		{
			temp[j] = conv<ZZ>(0);
		}

		for (int j = 0; j < binary_representation.size(); j++)
		{
			if (binary_representation[j] == 1)
			{
				temp = sum_vector(temp, pading(base, all_number[j], number));
			}
		}

		bool triger = true;

		for (int j = 0; j < binary_representation.size(); j++)
		{
			if (temp[j] % ZZ(2) == ZZ(0))
			{
				continue;
			}
			else
			{
				triger = false;
			}
		}

		if (triger)
		{
			vector<ZZ> root;

			for (int j = 0; j < binary_representation.size(); j++)
			{
				if (binary_representation[j] == 1)
				{
					root.push_back(all_number[j]);
				}
			}

			factors = get_factors(number, root);

			if (factors[0] == ZZ(1) || factors[0] == number || factors[1] == ZZ(1) || factors[1] == number)
			{
				factors[0] = ZZ(0);
				factors[1] = ZZ(0);

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

vector<ZZ> CFRAC(ZZ number) // exelent
{

	ZZ size_base = SqrRoot(number) + conv<ZZ>(1);

	vector<ZZ> base;
	base.push_back(conv<ZZ>(-1));
	base.push_back(conv<ZZ>(2));
	for (int i = 1; i < size_base; i++)
	{
		base.push_back(conv<ZZ>(2 * i + 1));
	}

	bool triger = true;

	for (int i = 3; triger = true; i++)
	{
		vector<ZZ> smooth_number, candidate_in_smooth_number = table_smooth(number, i);

		for (int j = 0; j < candidate_in_smooth_number.size(); j++)
		{
			vector<prime> temp = factorization(check_on_negative_numbers_and_conversation((candidate_in_smooth_number[j] * candidate_in_smooth_number[j]) % number, number));

			if (candidate_in_smooth_number[j] > conv<ZZ>(1) && compare_with_base(base, temp)&&temp.size()>1)
			{
				smooth_number.push_back(candidate_in_smooth_number[j]);
			}
		}

		vector<ZZ> factors = combination_of_vectors_and_finding_roots(base, number, smooth_number);

		if (factors[0] != ZZ(0) && factors[0] != ZZ(0))
		{
			triger = false;
			return factors;
		}
	}
}

int main()
{

	vector<ZZ> factors;

	factors = CFRAC(ZZ(9353));

	cout << endl<< "Roots: " << factors[0] << " " << factors[1] << endl;
	

	factors = CFRAC(ZZ(25511));

	cout << endl<< "Roots: " << factors[0] << " " << factors[1] << endl;

	factors = CFRAC(ZZ(56789));

	cout << endl<< "Roots: " << factors[0] << " " << factors[1] << endl;

	factors = CFRAC(ZZ(233503));

	cout << endl<< "Roots: " << factors[0] << " " << factors[1] << endl;

	factors = CFRAC(ZZ(45679));

	cout << endl<< "Roots: " << factors[0] << " " << factors[1] << endl;

	factors = CFRAC(ZZ(34037));

	cout << endl<< "Roots: " << factors[0] << " " << factors[1] << endl;

	factors = CFRAC(ZZ(77173));

	cout << endl<< "Roots: " << factors[0] << " " << factors[1] << endl;

	factors = CFRAC(ZZ(77173));

	cout << endl<< "Roots: " << factors[0] << " " << factors[1] << endl;

	factors = CFRAC(ZZ(188777));

	cout << endl<< "Roots: " << factors[0] << " " << factors[1] << endl;

	return 0;
}

// cout << endl
// 	 << "Input number:" << endl;
// int temp_number; cin>>temp_number;
// ZZ number=conv<ZZ>(temp_number);
// vector<ZZ> root = dixson(conv<ZZ>(4633));
// cout << endl
// 	 << "Result: " << root[0] << " " << root[1] << endl;

// table_smooth(ZZ(25511),10);

// vector <prime> factors=factorization(ZZ(-1246));

// for (int  i = 0; i < factors.size(); i++)
// {
// 	cout<<endl<<factors[i].base<<" "<<factors[i].degree<<endl;
// }

// vector <prime> factors=factorization(ZZ(-1246));

// for (int  i = 0; i < factors.size(); i++)
// {
// 	cout<<endl<<factors[i].base<<" "<<factors[i].degree<<endl;
// }

// cout<<"_________________________"<<endl;

//  factors=factorization(ZZ(-4568));

// for (int  i = 0; i < factors.size(); i++)
// {
// 	cout<<endl<<factors[i].base<<" "<<factors[i].degree<<endl;
// }

// cout<<"_________________________"<<endl;

//  factors=factorization(ZZ(-5229));

// for (int  i = 0; i < factors.size(); i++)
// {
// 	cout<<endl<<factors[i].base<<" "<<factors[i].degree<<endl;
// }

// cout<<endl<<"- candidates in smooth numbers: "<<endl;

// 		for (int  j = 0; j < candidate_in_smooth_number.size(); j++)
// 		{
// 	 		cout<<candidate_in_smooth_number[j]<<"  ";
// 		}

// cout<<endl<<"- smooth numbers: "<<endl;

// 	for (int  j = 0; j < smooth_number.size(); j++)
// 	{
//  		cout<<smooth_number[j]<<"  ";
// 	}

// 	cout<<endl;

// cout<<endl<<"- factorisation number "<<candidate_in_smooth_number[j]<<" :"<<endl;

// 		for (int j = 0; j<temp.size(); j++)
// 		{
// 			cout<<endl<<temp[j].base<<" "<<temp[j].degree<<endl;
// 		}

// 	cout<<endl;

// for (int i = 0; i < base.size(); i++)
// {
// 	cout<<base[i]<<" ";
// }

// cout<<endl;