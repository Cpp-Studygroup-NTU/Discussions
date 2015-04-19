#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <functional>
using namespace std;

void example1(const vector<int>& v){
	cout<<"Print a list of even number:"<<endl;
	for_each(v.begin(),v.end(),[](int x){
		if( x % 2 == 0 )
			cout<<x<<" ";
		else
			cout<<"_ ";
	});
}
void example2(const vector<int>& v){
	vector<double> dv;

	dv.resize(v.size());
	transform(v.begin(),v.end(),dv.begin(),[](int x) -> double /*return type*/{
		if( x % 2 == 0)
			return x / 2.0;
		else
			return x ;
	});
	cout<<"Transform to double:"<<endl;
	for_each(dv.begin(), dv.end(),[](double x){
		cout<<x<<" ";
	});
}
void example3(const vector<int>& v){
	auto vv(v);
	int min = 3, max = 5;
	cout<<"Stripping from "<<min<<" to "<<max<<endl;
	/*
		 include 'min' and 'max' 
		 they're of type 'const int'. (copy by value)
	*/
	vv.erase(remove_if(vv.begin(), vv.end(),[min,max](int x){
		/* Cannot modify x or y here */
		return min < x && x < max;
	}),vv.end());
	/* 
		remove_if returns an iterator to the first element to be
		removed. (Those elements are moved to the 'end' of the vector)
		Thus, to remove them all, do 'v.erase(startIterator,endIterator)'
}	*/
	/* 
		Internally, the lambda expression expand to:

		class LambdaFunctor {
			public:
			    LambdaFunctor(int a, int b) : _a(a), _b(b) { }
			 
			    bool operator()(int n) const { return _a < n && n < _b; }
			 
			private:
			    int _a;
			    int _b;
		};

		v.erase(remove_if(v.begin(), v.end(), LambdaFunctor(x, y)), v.end());

	*/
	for_each(vv.begin(), vv.end(),[](int x){
		cout<<x<<" ";
	});
}
void example4(const vector<int>& v){
	string even_mesg = "This is even number: ";
	string odd_mesg = "This is odd number: ";
	// [=] := capture everything by value
	// When compiler sees 'odd_mesg' and 'even_mesg' within lambda, it captures it.
	for_each(v.begin(), v.end(), [=](int x){
		if( x % 2 )
			cout<< odd_mesg << x << endl;
		else
			cout<< even_mesg << x << endl;
	});
}
void example5(const vector<int>& v){
	int r = 1;
	cout<<"Multiply each element by r"<<endl;
	for_each(v.begin(), v.end(),[=](int x) mutable {
		// r is a non-const data member of the expanded functor class.
		cout<< r*x << " ";
		r++;
	});
	cout<<endl<<"r value after for_each: "<<r;

}
void example6(const vector<int>& v){
	int r = 1;
	cout<<"Multiply each element by r"<<endl;
	// Capture everything by reference
	for_each(v.begin(), v.end(),[&](int x){
		cout<< r*x << " ";
		r++;
	});
	cout<<endl<<"r value after for_each: "<<r;
	/*  Internally expands to:

		class LambdaFunctor {
			public:
			    LambdaFunctor(int& r) : _r(r) { }
			 	// The data member can't be modified
			 	// But they can be used to modify what they refer to
			    void operator()(int x) const {
			        cout<< _r*x << " ";
					_r++;
			    }
			 
			private:
			    int& _r;

		};
	*/
}
void example7(const vector<int>& v){
	string even_mesg = "This is even number: ";
	string odd_mesg = "This is odd number: ";
	int sum = 0, product = 1;
	for_each(v.begin(), v.end(), [=,&sum,&product](int x){
		if( x % 2 )
			cout<< odd_mesg << x << endl;
		else
			cout<< even_mesg << x << endl;
		sum += x; product *= x;
	});
	cout<<endl<<"Sum: "<<sum<<", Product: "<<product;
}
void example8(const vector<int>& v){
	class Hello {
	public:
		Hello(string _mesg): mesg(_mesg){}
		void sayHello(const vector<int>& v) const {
			// Capture 'this' so you can use its data memeber
			// Optionally, [=] will implicitly include 'this', too
			for_each(v.begin(), v.end(), [this](int x){
				if( x == 3 )
					cout<<"Say '"<<mesg<<"' to "<<x;
			});
		} 
	private:
		string mesg;
	};

    auto hi = new Hello("hi");
    hi->sayHello(v);
    delete hi;
}
void example9(const vector<int>& v){
	cout<<"This is valid C++0x code"<<endl;
	[](){}();
    []{}();
}
void example10(const vector<int>& v){
	auto vv(v);
	auto isPrime = [](const int n) -> bool {
		if( n < 2 )
			return false;
		for (int i = 2; i <= sqrt(n); ++i)
			if( n % i == 0 )
				return false;
		return true;
	};
	vv.erase(remove_if(vv.begin(), vv.end(),[=](int x){ return !isPrime(x); }),vv.end());
    cout<<"Print a list of primes"<<endl;
	for_each(vv.begin(), vv.end(),[](int prime){
		cout<<prime<<" ";
	});
}
using FilterContainer = vector<function<bool(int)>>;
FilterContainer filters;
int globalVariable = 0;
void exampleFromBook1_make_filter(const vector<int>& v){
	int min = 2, max = 4, cnt = 0;  
	// these local variables will die at the end of function, yet it is passed by reference into lambda!
	filters.emplace_back(
		[&,globalVariable](int x){ 
			cout<<"This is the "<<cnt++<<"th comparison"<<endl;  // Terrible consequence!
			cout<<"Global variable: "<<globalVariable<<endl;
			return min <= x && x <= max; 
		} 
	);
}
void exampleFromBook1(const vector<int>& v){
	exampleFromBook1_make_filter(v);
	auto f = filters.front();
	for_each(v.begin(),v.end(),[f](int x){ 
		if(f(x))
			cout<<x<<" ";
	});
	globalVariable++;
	for_each(v.begin(),v.end(),[f](int x){ 
		if(f(x))
			cout<<x<<" ";
	});
}
int main(int argc, char const *argv[])
{
	vector<int> v { 1,2,3,4,5,6,7,8 };
	/*
		examples is a vector of type 'function<void(const vector<int>&)>'
		where 'void' is the return type and 'const vector<int>&)' is the
		type of parameters.  In general, it takes the form 
		'function< RETURN_TYPE ( P1_Type, P2_Type, ...) >'
	*/
    typedef function<void(const vector<int>&)> Example;
	vector<Example> examples {
		example1,
		example2,
		example3,
		example4,
		example5,
		example6,
		example7,
		example8,
		example9,
		example10
	};
    // Couldn't just use 'auto example'
    // It's only supported in c++14
	for_each(examples.begin(),examples.end(),[&v](Example example) {
		static int i = 0;
		cout<<"-----------Examples "<<setw(3)<<++i<<"-------------"<<endl;
		example(v); 
		cout<<endl<<endl;
	});
	return 0;
}
