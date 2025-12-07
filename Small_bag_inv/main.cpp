
#include <iostream>
#include "bag.h"


using namespace std;

int main() {

	bag myBag;

	bool running = true;
	
	do {

		cout << "Welcome to my BAG! wanna see some shit?" << endl;
		cout << "1. add item" << endl;
		cout << "2. remove an item" << endl;
		cout << "3. view all items" << endl;
		cout << "4.quit bitch" << endl;

		int input1;
		cin >> input1;
		switch (input1) {
		case 1:
			
			myBag.AddItem();
			break;
		case 2:
			myBag.RemoveItem();
			
			break;
		case 3:
			cout << "VIEW SHIT" << endl;
			myBag.ViewItems();
			
			break;
		case 4:
			cout << "WELL THEN GOODBYE BITCH" << endl;
			running = false;

		default:
			cout << "invalid options fuck face get it RIGHT!" << endl;
			break;
		}
		
		
	} while (running);
	
}