#include "listA.cpp"
#include <iostream>

using namespace std;

int main() {
	int size = 7;
	char c;	
	List l(size), copy;
	if(l.gotoBeginning()) {
		cout << "Error in gotoBeginning" << endl;
	}
	if(l.gotoEnd()) {
		cout << "Error in gotoEnd" << endl;
	}
	if(l.gotoNext()) {
		cout << "Error in gotoNext" << endl;
	}
	if(l.gotoPrior()) {
		cout << "Error in gotoPrior" << endl;
	}
	if(!l.empty()) {
		cout << "Error in empty" << endl;
	}
	for(int i = 0; i < size; i++) {
		if(!l.insertAfter(i + '0')) {
			cout << "inserting " << (char)(i + '0');			
			cout << "Error in insertAfter" << endl;
		}
	}
	if(l.insertAfter('a')) {
		cout << "Error detecting overflow" << endl;
	}
	cout << "Begin <<: "<< endl << l << "end <<" << endl;
	for(int i = 0; i < size / 2; i++) {
		if(!l.gotoPrior()) {
			cout << "Error in gotoPrior" << endl;
		}
	}
	if(!l.getCursor(c)) {
		cout << "Error in getCursor" << endl;
	}
	cout << "getCursor (expected to see 3) saw " << c << endl;
	cout << "remove 3" << endl;		
	if(!l.remove(c)) {
		cout << "Error in remove" << endl;
	}
	cout << "removed " << c << endl;
	cout << "Begin <<: "<< endl << l << "end <<" << endl;
	cout << "insertBefore a" << endl;
	if(!l.insertBefore('a')) {
		cout << "Error in insertBefore" << endl;
	}
	cout << "Begin <<: "<< endl << l << "end <<" << endl;
	cout << "Replace a with 3" << endl;
	if(!l.replace('3')) {
		cout << "Error in replace" << endl;
	}
	cout << "Begin <<: "<< endl << l << "end <<" << endl;
	copy = l;
	cout << "Begin copied <<: "<< endl << copy << "end copied <<" << endl;
	cout << "Checking equality" << endl;
	if(!(copy == l)) {
		cout << "Error in == (Says not equal when they are equal" << endl;
	}	
	cout << "clearing copied" << endl;
	if(!copy.clear()) {
		cout << "Error in clearing" << endl;
	}
	if(!copy.empty()) {
		cout << "Error in emtpy (Says not empty when it is empty" << endl;
	}
	cout << "Begin copied <<: "<< endl << copy << "end copied <<" << endl;
	if(!l.full()) {
		cout << "Error in full (says not full when it is full)" << endl;
	}
	if(!l.gotoBeginning()) {
		cout << "Error in gotoBeginning" << endl;
	}
	if(!l.gotoNext()) {
		cout << "Error in gotoNext" << endl;
	}
	if(!l.gotoNext()) {
		cout << "Error in gotoNext" << endl;
	}
	if(!l.remove(c)) {
		cout << "Error in remove" << endl;
	}
	cout << "remove (expected 2) saw " << c << endl;
	if(!l.gotoEnd()) {
		cout << "Error in gotoEnd" << endl;
	}
	if(!l.gotoPrior()) {
		cout << "Error in gotoPrior" << endl;
	}
	if(!l.gotoPrior()) {
		cout << "Error in gotoPrior" << endl;
	}
	if(!l.remove(c)) {
		cout << "Error in remove" << endl;
	}
	cout << "remove (expected 4) saw " << c << endl;
	cout << "Begin copied <<: "<< endl << copy << "end copied <<" << endl;
}
