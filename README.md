# bigint
C++ Library for representing large numbers and computing on them

To use, include bigint.h in your source file and have bigint.cpp (which contains all the implementations) available for linking.

The class makes use of a vector container to store the digits of the number, thus the largest number that can be stored is limited only by the number of elements a vector can hold, which is typically 1073741823 digits on most system implementations.

The C++ arithmetic (except division), relational, increment/decrement, and assignment operators have been overloaded for this class. Also, some C++ standard library functions like abs() and to_string() have been overloaded for this class.

This project was intended to acquaint myself with basic object-oriented programming and operator overloading in C++.

Issues:
<ul>
<li>factorial function sometimes gives the factorial of the number 1 less than the number passed as argument. (Eg. factorial(34) sometimes gives the correct factorial of 34, other times it gives factorial of 33.) Cause of this bug is unknown.</li>
</ul>

To-do:
<ul>
<li>Rewrite the multiplication function to use the Karatsuba algorithm, which is more efficient than the simple digit-by-digit multiplication algorithm presently used.</li>
<li>Add support for division.</li>
</ul>