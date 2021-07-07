This library allows you to do operations on arbitrarilly large integers, similar to 
python ints.

You can assign a bigint to a normal int:

	bigint x=37;

If your integer is to large, you can use the string constuctor:

	bigint x=(string)"37873892982891988917116";

For large negative integers:

	bigint x=(string)"-36736637238289191918272";

The following binary operators are supported:
1. Arithmetic: +, -, \*, /, %
1. Mutating: +=, -=, \*=, /=, %=
1. Comparison: >, <, >=, <=, ==, !=

You can print a bigint in base 10 using:
	
	std::cout<<x;

The end line is included.

The data stucture is very simple, it contatins a `std::vector<uint8_t>` to store
the number, and a `bool` which is true for positive numbers and false for negatives.

The complexity comes form the implementation of addition, subtraction, 
multiplication, and divison in bigint.cpp. 

Use `make` to build, or `make run` to build and run tests.

