/*This program will allow a user to enter a password and check whether or not it is valid.
*/

#include <iostream>
#include <cstring> // for std::strlen
#include <cctype>
using namespace std;

//fuction proto to check pass
bool testPass(char []); // don't need the 2nd parameter

int credmain()
{
    char *password; //dynamically allocating an array
    int length; //assure requested length and pass length are the same
    int numCharacters; //hold number of characters for password

//get the password length from the user
    cout << "Please enter how many characters you would like your\npassword to be.";
    cout << " Your password must be at least 6 characters long." << endl;
    cin >> numCharacters;

//validate
    while (numCharacters < 6)
    {
        cout << "Please enter a password length of at least 6 characters." << endl;
        cin >> numCharacters;
    }

//dynamically allocate the array for the password
    password = new char[numCharacters+1]; /// every cstring must end with a '\0' else -> crash


    cout << "Please enter a password that contains at least one uppercase letter, ";
    cout << "one\nlowercase letter, and at least one digit." << endl;

//get users password
    cin >> password;

//convert pointer/array length to interger
    length = strlen(password);


//check pointer/array length against user requested pointer/array size
//to ensure consistent data
    while (length != numCharacters)
    {
        cout << "Your password is not the size you requested. ";
        cout << "Please re-enter your password." << endl;
        cin >> password;
        length = strlen(password);
    }

    if (testPass(password))
        cout << "Your password is valid." << endl;
    else
    {
        cout << "Your password is not valid. ";
        cout << "Please refer to the above warning message." << endl;
    }

    delete[] password ; // hey!! don't forget to free your allocated memory!!!
    return 0;
}

/*This function will check each input and ensure that the password
contains a uppercase, lowercase, and digit.*/

bool testPass(char pass[]) // a beautiful hack (:
{
	// flags
	bool aUpper = false,
		 aLower = false,
		 aDigit = false ;
	for ( int i = 0 ; pass[i] ; ++i )
		if ( isupper(pass[i]) )
			aUpper = true ;
		else if ( islower(pass[i]) )
			aLower = true ;
		else if ( isdigit(pass[i]) )
			aDigit = true ;
	if ( aUpper && aLower && aDigit )
		return true;
	else
		return false ;
}