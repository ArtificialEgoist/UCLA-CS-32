/*

#include <iostream>
#include <cassert>
#include <cmath>

using namespace std;

*/

// Return true if any of the array elements is negative, false otherwise.
bool anyNegative(const double a[], int n)
{
	if(n<=0)
		return false;

	if(a[0]<0)
		return true;
	else
		return anyNegative(a+1, n-1);
}

// Return the number of negative elements in the array.
int countNegatives(const double a[], int n)
{
	if(n<=0)
		return 0;

	if(a[0]<0)
		return 1+countNegatives(a+1, n-1);
	else
		return countNegatives(a+1, n-1);
}

// Return the subscript of the first negative element in the array.
// If no element is negative, return -1.
int firstNegative(const double a[], int n)
{
	if(n<=0)
		return -1;

	if(a[n-1]<0) //if this last element is negative
		if(firstNegative(a, n-2)==-1) //check to see if the one before is negative (if it is, this isn't the first!)
			return n-1; //otherwise, return that index

	return firstNegative(a, n-1); //if this one's not negative, try the ones before
}



// Return the subscript of the smallest element in the array.  If
// more than one element has the same smallest value, return the
// smallest index of such an element.  If the array is empty,
// return -1.
int indexOfMin(const double a[], int n)
{
	if(n==1)
		return 0; //if there is only one element, that index holds the min

	if(n<=0)
		return -1;

	if(a[n-1]<a[indexOfMin(a, n-1)]) //if the last element is smaller then the smallest one before it, return its index
		return n-1;
	
	return indexOfMin(a, n-1); //if it's not smaller, simply return the last smallest index
}

// If all n2 elements of a2 appear in the n1 element array a1, in
// the same order (though not necessarily consecutively), then
// return true; otherwise (i.e., if the array a1 does not include
// a2 as a not-necessarily-contiguous subsequence), return false.
// (Of course, if a2 is empty (i.e., n2 is 0), return true.)
// For example, if a1 is the 7 element array
//    10 50 40 20 50 40 30
// then the function should return true if a2 is
//    50 20 30
// or
//    50 40 40
// and it should return false if a2 is
//    50 30 20
// or
//    10 20 20
bool includes(const double a1[], int n1, const double a2[], int n2)
{
	if(n2<=0) //if n2's size becomes 0, that means all elements have been found in order (from back to front)
		return true;
	else if(n1<=0) //if n1's size becomes 0 while n2's is positive, that means not all n2 elements were found
		return false;

	
	if(a1[n1-1]==a2[n2-1])
		return includes(a1, n1-1, a2, n2-1); //if n2's element was found in n1, decrement both sizes
	else
		return includes(a1, n1-1, a2, n2); //otherwise, decrement only n1's size, since n2's element hasn't been found yet
}



/*

int main()
{
	double testNegatives[5]={1, 3, -2, 5, 7};
	double testNoNegatives[10]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	assert(anyNegative(testNegatives, 5)==true);
	assert(anyNegative(testNegatives, 3)==true);
	assert(anyNegative(testNegatives, 2)==false);
	assert(anyNegative(testNoNegatives, 10)==false);
	assert(anyNegative(testNoNegatives, 1)==false);
	assert(anyNegative(testNoNegatives, 0)==false);

	cerr << "Passed anyNegative tests." << endl;

	double testCountNegatives[5]={1, -2, -3, -4, 5};
	double testCountSomeNegatives[10]={-5, 0, 28, 5, 1, 23, -5, 1, 2, 14};
	double testCountAllNegatives[5]={-7, -3, -9, -1, -15};

	assert(countNegatives(testCountNegatives, 5)==3);
	assert(countNegatives(testCountNegatives, 4)==3);
	assert(countNegatives(testCountNegatives, 2)==1);
	assert(countNegatives(testCountNegatives, 1)==0);
	assert(countNegatives(testCountNegatives, 0)==0);

	assert(countNegatives(testCountSomeNegatives, 10)==2);
	assert(countNegatives(testCountSomeNegatives, 7)==2);
	assert(countNegatives(testCountSomeNegatives+1, 9)==1);
	assert(countNegatives(testCountSomeNegatives, 0)==0);
	assert(countNegatives(testCountSomeNegatives, 1)==1);

	assert(countNegatives(testCountAllNegatives, 5)==5);
	assert(countNegatives(testCountAllNegatives, 3)==3);
	assert(countNegatives(testCountAllNegatives+4, 1)==1);
	assert(countNegatives(testCountAllNegatives, 0)==0);

	cerr << "Passed countNegatives tests." << endl;
	
	
	
	double testFindNegative[5]={1, 3, -2, 5, 7};
	double testFindNegativeLong[10]={-5, 0, 28, 5, 1, 23, -5, 1, 2, 14};
	double testFindNoNegatives[10]={0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

	assert(firstNegative(testFindNegative, 5)==2);
	assert(firstNegative(testFindNegative, 3)==2);
	assert(firstNegative(testFindNegative, 2)==-1);
	assert(firstNegative(testFindNegative+2, 3)==0);
	assert(firstNegative(testFindNegative+3, 2)==-1);

	assert(firstNegative(testFindNegativeLong, 10)==0);
	assert(firstNegative(testFindNegativeLong, 2)==0);
	assert(firstNegative(testFindNegativeLong+1, 9)==5);
	assert(firstNegative(testFindNegativeLong+2, 8)==4);

	assert(firstNegative(testFindNoNegatives, 10)==-1);

	cerr << "Passed firstNegative tests." << endl;

	double testMinIndex[5]={1, 3, -2, 5, 7};
	double testMinNoNeg[5]={0, 1, 2, 3, 4};
	double testGettingSmaller[4]={5, 3, 1, -9};
	double testGettingBigger[4]={-9, 1, 3, 5};

	assert(indexOfMin(testMinIndex, 5)==2);
	assert(indexOfMin(testMinIndex, 3)==2);
	assert(indexOfMin(testMinIndex+3, 2)==0);
	assert(indexOfMin(testMinIndex, 2)==0);

	assert(indexOfMin(testMinNoNeg, 5)==0);
	assert(indexOfMin(testMinNoNeg+2, 3)==0);
	assert(indexOfMin(testMinNoNeg+4, 1)==0);

	assert(indexOfMin(testGettingSmaller, 4)==3);
	assert(indexOfMin(testGettingBigger, 4)==0);

	double testMoreMin[5]={19, 11, 13, 15, 17};

	assert(indexOfMin(testMoreMin, 5)==1);
	assert(indexOfMin(testMoreMin, 2)==1);
	assert(indexOfMin(testMoreMin, 1)==0);
	assert(indexOfMin(testMoreMin+2, 3)==0);

	cerr << "Passed indexOfMin tests." << endl;

	double a1[7]={10, 50, 40, 20, 50, 40, 30};
	double a2[3]={50, 20, 30};
	double a3[3]={50, 40, 40};
	double a4[3]={10, 20, 20};
	double a5[3]={50, 30, 20};

	assert(includes(a1, 7, a2, 3)==true);
	assert(includes(a1, 7, a3, 3)==true);
	assert(includes(a1, 7, a4, 3)==false);
	assert(includes(a1, 7, a5, 3)==false);

	cerr << "Passed includes tests." << endl;


}

*/