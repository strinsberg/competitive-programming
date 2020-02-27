///////////////////////////////////////////////////////////////////////
// CPSC 3200
// Assignment 2
// UVA 11947
//
// Steven Deutekom
// Jan 9 2020
//
// For each dateset I extract the day month year. Then build a date class and
// add the appropriate number of days for gestation. Then using the new
// month and day find the appropriate zodiac sign from a lookup table.
// Since each month can give one of two signs we store the cuttoff and
// both signs in the table indexed by month - 1. Then we can get the
// month's info and use the cutoff and current day to determine wich
// sign to print.
//
// My code starts at line 204
///////////////////////////////////////////////////////////////////////
#include <bits/stdc++.h>
using namespace std;
using namespace std::rel_ops;

// Date Class from code binder
struct Date {

  int yyyy;
  int mm;
  int dd;

  // no dates before 1753
  static int const BASE_YEAR = 1753;

  // Enumerated type for names of the days of the week
  enum dayName {SUN,MON,TUE,WED,THU,FRI,SAT};

  // Is a date valid
  static bool validDate(int yr, int mon, int day)
  {
    return yr >= BASE_YEAR && mon >= 1 && mon <= 12 &&
      day > 0 && day <= daysIn(mon, yr);
  }    

  bool isValid() const
  {
    return validDate(yyyy, mm, dd);
  }
  
  // Constructor to create a specific date.  If the date is invalid,
  // the behaviour is undefined
  Date(int yr = 1970, int mon = 1, int day = 1)
  {
    yyyy = yr;
    mm = mon;
    dd = day;
  }
  
  // Returns the day of the week for this
  dayName dayOfWeek() const
  {
    int a = (14 - mm) / 12;
    int y = yyyy - a;
    int m = mm + 12 * a - 2;
    return (dayName)((dd + y + y/4 - y/100 + y/400 + 31 * m / 12) % 7);
  }
  
  // comparison operators
  bool operator==(const Date &d) const
  {
    return dd == d.dd && mm == d.mm && yyyy == d.yyyy;
  }

  bool operator<(const Date &d) const
  {
    return yyyy < d.yyyy || (yyyy == d.yyyy && mm < d.mm) ||
      (yyyy == d.yyyy && mm == d.mm && dd < d.dd);
  }

  // Returns true if yr is a leap year
  static bool leapYear(int y)
  {
    return (y % 400 ==0 || (y % 4 == 0 && y % 100 != 0));
  }

  // number of days in this month
  static int daysIn(int m, int y)
  {
    switch (m) {
    case 4  :
    case 6  :
    case 9  :
    case 11 :
      return 30;
    case 2  :
      if (leapYear(y)) {
	return 29;
      }
      else {
	return 28;
      }
    default :
      return 31;
    }
  }
  
  // increment by day, month, or year
  //
  // Use negative argument to decrement
  //
  // If adding a month/year results in a date before BASE_YEAR, the result
  // is undefined.
  //
  // If adding a month/year results in an invalid date (Feb 29 on a non-leap
  // year, Feb 31, Jun 31, etc.), the results are automatically "rounded down"
  // to the last valid date

  // add n days to the date: complexity is about n/30 iterations
  void addDay(int n = 1)
  {
    dd += n;
    while (dd > daysIn(mm,yyyy)) {
      dd -= daysIn(mm,yyyy);
      if (++mm > 12) {
	mm = 1;
	yyyy++;
      }
    }
    
    while (dd < 1) {
      if (--mm < 1) {
	mm = 12;
	yyyy--;
      }
      dd += daysIn(mm,yyyy); 
    }
  }

  // add n months to the date: complexity is about n/12 iterations
  void addMonth(int n = 1)
  {
    mm += n;
    while (mm > 12) {
      mm -= 12;
      yyyy++;
    }
    
    while (mm < 1)  {
      mm += 12;
      yyyy--;
    }
    
    if (dd > daysIn(mm,yyyy)) {
      dd = daysIn(mm,yyyy);
    }
  }

  // add n years to the date
  void addYear(int n = 1)
  {
    yyyy += n;
    if (!leapYear(yyyy) && mm == 2 && dd == 29) {
      dd = 28;
    }
  }

  // number of days since 1753/01/01, including the current date
  int daysFromStart() const
  {
    int c = 0;
    Date d(BASE_YEAR, 1, 1);
    Date d2(d);

    d2.addYear(1);
    while (d2 < *this) {
      c += leapYear(d.yyyy) ? 366 : 365;
      d = d2;
      d2.addYear(1);
    }

    d2 = d;
    d2.addMonth(1);
    while (d2 < *this) {
      c += daysIn(d.mm, d.yyyy);
      d = d2;
      d2.addMonth(1);
    }
    while (d <= *this) {
      d.addDay();
      c++;
    }
    return c;
  }
};

// Reads a date in yyyy/mm/dd format, assumes date is valid and in the
// right format
istream& operator>>(istream &is, Date &d)
{
  char c;
  return is >> d.yyyy >> c >> d.mm >> c >> d.dd;
}

// print date in yyyy/mm/dd format
ostream& operator<< (ostream &os, const Date &d) {
  char t = os.fill('0');
  os << d.yyyy << '/' << setw(2) << d.mm << '/' << setw(2) << d.dd;
  os.fill(t);
  return os;
}

// My Code //////////////////////////////////////////////////////////////////

// A struct to hold a months sign info
struct SignInfo {
  SignInfo(int c, string b, string a) : cap(c), before(b), after(a) {}
  int cap;        // The cap for being the before sign
  string before;  // The sign for the first part of the month
  string after;   // The sign for the second part of the month
};

// Store the sign info for each month. Indexed with month - 1.
vector<SignInfo> signs{
  {20, "capricorn", "aquarius"},
  {19, "aquarius", "pisces"},
  {20, "pisces", "aries"},
  {20, "aries", "taurus"},
  {21, "taurus", "gemini"},
  {21, "gemini", "cancer"},
  {22, "cancer", "leo"},
  {21, "leo", "virgo"},
  {23, "virgo", "libra"},
  {23, "libra", "scorpio"},
  {22, "scorpio", "sagittarius"},
  {22, "sagittarius", "capricorn"} 
};

// Main solve the problem ;)
int main() {  
  int n, c = 1;
  cin >> n;

  string d;
  while (cin >> d) {
    int M = stoi(d.substr(0,2));
    int D = stoi(d.substr(2,2));
    int Y = stoi(d.substr(4,4));

    // Build and add to the date
    Date date(Y,M,D);
    date.addDay(7*40);

    // Output the new date and sign
    cout << c++ << " ";
    if (date.mm < 10)
      cout << "0";
    cout << date.mm << "/";
    if (date.dd < 10)
      cout << "0";
    cout << date.dd << "/" << date.yyyy << " ";

    // If the day is less or equal to the months cap print
    // the before sign, otherwise print the after sign.
    SignInfo si = signs[date.mm - 1];
    cout << (date.dd <= si.cap ? si.before : si.after) << endl;
  }

  return 0;
}

