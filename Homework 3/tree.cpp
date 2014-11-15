/*

#include <iostream>
#include <cassert>

using namespace std;

*/

	// Return the number of ways that all n2 elements of a2 appear
	// in the n1 element array a1 in the same order (though not
	// necessarily consecutively).  The empty sequence appears in a
	// sequence of length n1 in 1 way, even if n1 is 0.
	// For example, if a1 is the 7 element array
	//	10 50 40 20 50 40 30
	// then for this value of a2     the function must return
	//	10 20 40			1
	//	10 40 30			2
	//	20 10 40			0
	//	50 40 30			3
int countIncludes(const double a1[], int n1, const double a2[], int n2)
{
	if(n2<=0) //if n2's size becomes 0, that means all elements have been found in order (from back to front)
		return 1;
	else if(n1<=0) //if n1's size becomes 0 while n2's is positive, that means not all n2 elements were found
		return 0;
	
	if(a1[n1-1]==a2[n2-1])
		return countIncludes(a1, n1-1, a2, n2-1)+countIncludes(a1, n1-1, a2, n2); //if n2's element was found in n1, decrement both sizes
	else
		return countIncludes(a1, n1-1, a2, n2); //otherwise, decrement only n1's size, since n2's element hasn't been found yet
}

	// Exchange two doubles
void exchange(double& x, double& y)
{
	double t = x;
	x = y;
	y = t;
}

	// Rearrange the elements of the array so that all the elements
	// whose value is < splitter come before all the other elements,
	// and all the elements whose value is > splitter come after all
        // the other elements.  Upon return, firstNotLess is set to the index
        // of the first element in the rearranged array that is >= splitter,
	// or n if there is no such element, and firstGreater is set to
	// the index of the first element that is > splitter, or n if
	// there is no such element.
	// In other words, upon return from the function, the array is a
	// permutation of its original value such that
	//   * for 0 <= i < firstNotLess, a[i] < splitter
	//   * for firstNotLess <= i < firstGreater, a[i] == splitter
	//   * for firstGreater <= i < n, a[i] > splitter
	// All the elements < splitter end up in no particular order.
	// All the elements > splitter end up in no particular order.
void split(double a[], int n, double splitter,
	                                int& firstNotLess, int& firstGreater)
{
	if (n < 0)
	    n = 0;
	
	    // It will always be the case that just before evaluating the loop
	    // condition:
	    //  firstNotLess <= firstUnknown and firstUnknown <= firstGreater
	    //  Every element earlier than position firstNotLess is < splitter
	    //  Every element from position firstNotLess to firstUnknown-1 is
	    //    == splitter
	    //  Every element from firstUnknown to firstGreater-1 is not
	    //     known yet
	    //  Every element at position firstGreater or later is > splitter
	
	firstNotLess = 0;
	firstGreater = n;
	int firstUnknown = 0;
	while (firstUnknown < firstGreater)
	{
            if (a[firstUnknown] > splitter)
            {
                firstGreater--;
                exchange(a[firstUnknown], a[firstGreater]);
            }
            else
            {
                if (a[firstUnknown] < splitter)
                {
                    exchange(a[firstNotLess], a[firstUnknown]);
                    firstNotLess++;
                }
                firstUnknown++;
            }
	}
}

	// Rearrange the elements of the array so that
	// a[0] <= a[1] <= a[2] <= ... <= a[n-2] <= a[n-1]
	// If n <= 1, do nothing.
void order(double a[], int n)
{
	if(n<=1)
		return;
	
	int firstNotLess, firstGreater;
	split(a, n, a[n/2], firstNotLess, firstGreater);

	if(firstNotLess>=2 && firstNotLess<n) //if there are at least two more in the front that are unsorted, sort them
		order(a, firstNotLess);
	if(firstGreater<n-1)
		order(a+firstGreater, n-firstGreater);

	return;  // This is not always correct.
}

/*

int main()
{
	double none[1]={0};
	
	double a1[7]={10, 50, 40, 20, 50, 40, 30};
	double a21[3]={10, 20, 40};
	double a22[3]={10, 40, 30};
	double a23[3]={20, 10, 40};
	double a24[3]={50, 40, 30};

	assert(countIncludes(none, 0, none, 5)==0);
	assert(countIncludes(none, 0, none, 0)==1);

	cerr << countIncludes(a1, 7, a21, 3) << endl; //1
	cerr << countIncludes(a1, 7, a22, 3) << endl; //2
	cerr << countIncludes(a1, 7, a23, 3) << endl; //0
	cerr << countIncludes(a1, 7, a24, 3) << endl; //3

	assert(countIncludes(a1, 7, a21, 3)==1);
	assert(countIncludes(a1, 7, a22, 3)==2);
	assert(countIncludes(a1, 7, a23, 3)==0);
	assert(countIncludes(a1, 7, a24, 3)==3);

	cerr << "Passed basic countIncludes tests." << endl;

	double sortMe1[7]={10, 50, 40, 20, 50, 40, 30};
	order(sortMe1, 7);

	cerr << "SortMe1-----" << endl;
	for(int i=0; i<7; i++)
		cerr << sortMe1[i] << endl;
	cerr << "SortMe1-----" << endl;

	double sortMe2[10]={5, 3, 8, 9, 1, 7, 2, 4, 6, 0};
	order(sortMe2, 10);

	cerr << "SortMe2-----" << endl;
	for(int i=0; i<10; i++)
		cerr << sortMe2[i] << endl;
	cerr << "SortMe2-----" << endl;

	double sortMe3[5]={3, 3, 3, 3, 3};
	order(sortMe3, 5);

	cerr << "SortMe3-----" << endl;
	for(int i=0; i<5; i++)
		cerr << sortMe3[i] << endl;
	cerr << "SortMe3-----" << endl;

	order(none, 0);
		cerr << none[0] << endl;
	order(none, 1);
		cerr << none[0] << endl;

	cerr << "Passed basic order tests." << endl;

}

*/