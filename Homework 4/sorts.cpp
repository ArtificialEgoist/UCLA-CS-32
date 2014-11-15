#include <iostream>
#include <algorithm>
#include <numeric>  // for std::accumulate
#include <vector>
#include <string>
#include <cstdlib>  // for std::rand
#include <cassert>

using namespace std;

//========================================================================
//  Set this to false to skip the insertion sort tests; you'd do this if
//  you're sorting so many items that insertion_sort would take more time
//  than you're willing to wait.

const bool TEST_INSERTION_SORT = true;
//========================================================================

//========================================================================
// TimerType            - a type to hold a timer reading
// TimerType getTimer() - get the current timer reading
// double interval(TimerType start, TimerType end) - milliseconds between
//                                                   two readings
//========================================================================

#ifdef _MSC_VER  // If we're compiling for Windows

#include <windows.h>

typedef LARGE_INTEGER TimerType;
inline TimerType getTimer()
{
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);
    return t;
}

inline double interval(TimerType start, TimerType end)
{
    LARGE_INTEGER ticksPerSecond;
    QueryPerformanceFrequency(&ticksPerSecond);
    return (1000.0 * (end.QuadPart - start.QuadPart)) / ticksPerSecond.QuadPart;
}

#else // If we're not compiling for Windows, use Standard C

#include <ctime>

typedef clock_t TimerType;
inline TimerType getTimer() { return clock(); }
inline double interval(TimerType start, TimerType end)
{
    return (1000.0 * (end - start)) / CLOCKS_PER_SEC;
}

#endif  // ifdef _MSC_VER

//========================================================================

// Here's a class that is not cheap to copy -- the vector holds a pointer
// to dynamic memory, so vector's assignment operator and copy constructor
// have to allocate storage.

// We'll simplify writing our timing tests by declaring everything public
// in this class.  (We wouldn't make data public in a class intended for
// wider use.)

typedef int IdType;

struct Student
{
    IdType id;
    double gpa;
    vector<double> grades;
    Student(IdType i) : id(i)
    {
          // create ten random grades (from 0 to 4)
        for (size_t k = 0; k < 10; k++)
            grades.push_back(rand() % 5);
          // (accumulate computes 0.0 + grades[0] + grades[1] + ...)
        gpa = accumulate(grades.begin(), grades.end(), 0.0) / grades.size();
    }
};

inline
bool compareStudent(const Student& lhs, const Student& rhs)
{
      // The Student with the higher GPA should come first.  If they have
      // the same GPA, then the Student with the smaller id number should
      // come first.  Return true iff lhs should come first.  Notice that
      // this means that a false return means EITHER that rhs should come
      // first, or there's a tie, so we don't care which comes first,

    if (lhs.gpa > rhs.gpa)
        return true;
    if (lhs.gpa < rhs.gpa)
        return false;
    return lhs.id < rhs.id;
}

inline
bool compareStudentPtr(const Student* lhs, const Student* rhs)
{
    // TODO: You implement this.  Using the same criteria as compareStudent,
    //       compare two Students POINTED TO by lhs and rhs.  Think about
    //	     how you can do it in one line by calling compareStudent.

	return compareStudent(*lhs, *rhs);
}

bool isSorted(const vector<Student>& s)
{
      // Return true iff the vector is sorted according to the ordering
      // relationship compareStudent

    for (size_t k = 1; k < s.size(); k++)
    {
        if (compareStudent(s[k],s[k-1]))
            return false;
    }
    return true;
}

void insertion_sort(vector<Student>& s, bool comp(const Student&, const Student&))
{
    // TODO: Using the insertion sort algorithm (pp. 463-465), sort s
    //       according to the ordering relationship passed in as the
    //       parameter comp.

    // Just to show you how to use the second parameter, we'll write code
    // that sorts only a vector of 2 elements.  (This is *not* the
    // insertion sort algorithm.)

    // Note that if comp(x,y) is true, it means x must end up before y in the
    // final ordering.
  
	//if (s.size() == 2  &&  comp(s[1], s[0]))
        //swap(s[0], s[1]);

	for(int i=1; i<s.size(); i++)
	{
		int j=i;
		int k=i-1;

		while(k>=0 && comp(s[j], s[k]))
		{
			swap(s[k], s[j]);
			j--;
			k--;
		}
	}

}

  // Report the results of a timing test

void report(string caption, double t, const vector<Student>& s)
{
    cout << t << " milliseconds; " << caption
             << "; first few students are\n\t";
    size_t n = s.size();
    if (n > 5)
        n = 5;
    for (size_t k = 0; k < n; k++)
        cout << " (" << s[k].id << ", " << s[k].gpa << ")";
    cout << endl;
}

int main()
{
    size_t nstudents;
    cout << "Enter number of students to sort: ";
    cin >> nstudents;
    if ( !cin  ||  nstudents <= 0)
    {
        cout << "You must enter a positive number.  Goodbye." << endl;
        return 1;
    }

      // Create a random ordering of id numbers 0 through nstudents-1
    vector<IdType> ids;
    for (size_t j = 0; j < nstudents; j++)
        ids.push_back(IdType(j));
    random_shuffle(ids.begin(), ids.end());  // from <algorithm>
    
      // Create a bunch of Students
    vector<Student> unorderedStuds;
    for (size_t k = 0; k < ids.size(); k++)
        unorderedStuds.push_back(Student(ids[k]));

    TimerType startSort;
    TimerType endSort;

      // Sort the Students using the STL sort algorithm.  It uses a variant
      // of quicksort called introsort.

    vector<Student> studs(unorderedStuds);
    startSort = getTimer();
    sort(studs.begin(), studs.end(), compareStudent);
    endSort = getTimer();
    report("STL sort", interval(startSort, endSort), studs);
    assert(isSorted(studs));

      // Sort the already sorted array using the STL sort.  This should be
      // fast.

    startSort = getTimer();
    sort(studs.begin(), studs.end(), compareStudent);
    endSort = getTimer();
    report("STL sort if already sorted", interval(startSort, endSort), studs);
    assert(isSorted(studs));

    if (TEST_INSERTION_SORT)
    {
          // Sort the original unsorted array using insertion sort.  This
          // should be really slow.  If you have to wait more than a minute,
          // try rerunning the program with a smaller number of Students.

        studs = unorderedStuds;
        startSort = getTimer();
        insertion_sort(studs, compareStudent);
        endSort = getTimer();
        report("insertion sort if not already sorted",
                                         interval(startSort, endSort), studs);
        assert(isSorted(studs));

          // Sort the already sorted array using insertion sort.  This should
          // be fast.

        startSort = getTimer();
        insertion_sort(studs, compareStudent);
        endSort = getTimer();
        report("insertion sort if already sorted",
                                         interval(startSort, endSort), studs);
        assert(isSorted(studs));
    }

      // Since Students are expensive to copy, and since the STL's sort copies
      // Students O(N log N) times, let's sort POINTERS to the Students, then
      // make one final pass to rearrange the Students according to the
      // reordered pointers.  We'll write some code; you write the rest.

      // Set studs to the original unsorted sequence
    studs = unorderedStuds;

      // Start the timing
    startSort = getTimer();

      // Create an auxiliary copy of studs, to faciliate the later reordering.
      // We create it in a local scope, so we also account for the destruction
      // time.
    {
     vector<Student> auxStuds(studs);

      // TODO:  Create a vector of Student pointers, and set each pointer
      //        to point to the corresponding Student in auxStuds.
	 vector<Student*> studPointers;
	 for(int i=0; i<studs.size(); i++)
	 {
		//Student* temp = new Student(auxStuds.at(i));
		 Student* temp = &(auxStuds.at(i));
		studPointers.push_back(temp);
	 }
    
      // TODO:  Sort the vector of pointers using the STL sort algorithm
      //        with compareStudentPtr as the ordering relationship.

    sort(studPointers.begin(), studPointers.end(), compareStudentPtr);

      // TODO:  Using the now-sorted vector of pointers, replace each Student
      //	in studs with the Students from auxStuds in the correct order.

	for(int i=0; i<studs.size(); i++)
	{
		studs.at(i)=*studPointers.at(i);
		//studs.at(i)=auxStuds.at(i);
	}


    } // auxStuds will be destroyed here

      // End the timing, report, and verify the sort worked
    endSort = getTimer();
    report("STL sort of pointers", interval(startSort, endSort), studs);
    assert(isSorted(studs));
}
