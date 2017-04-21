#define _CRT_SECURE_NO_WARNINGS
#include "PosApp.h"
//V1.4
//fixed minor output refinement
using namespace std;

namespace ict {
	//constructor
	PosApp::PosApp(const char* filename, const char* billfname) {
		strcpy(m_filename, filename);
		strcpy(m_billfname, billfname);
	}
	//Private member functions (methods)
	int PosApp::menu() {
		int value;
		cout << "The Sene-Store" << endl;
		cout << "1- List items" << endl;
		cout << "2- Add Perishable item" << endl;
		cout << "3- Add Non-Perishable item" << endl;
		cout << "4- Update item quantity" << endl;
		cout << "5- Show Item" << endl;
		cout << "6- POS" << endl;
		cout << "0- exit program" << endl;
		cout << "> ";
		cin >> value;
		if ((value < 0 || value > 6) || cin.fail()) {
			value = -1;
		}
		//added proper keyboard flushing,
		//instead of cin.ignore, used cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		//used #include <limits>
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return value;
	}
	//added m_item[i] = nullptr so each pointer in m_item isn't pointing to garbage
	void PosApp::deallocateItems() {
		for (int i = 0; i < m_noOfItems; i++) {
			delete m_items[i];
			m_items[i] = nullptr;
		}
		m_noOfItems = 0;
	}
	//collaborated with Robert Veronov
	//NEW VERSION
	void PosApp::loadRecs() {
		char comma, check;
		fstream file;
		deallocateItems();

		m_noOfItems = -1;
		file.open(m_filename, ios::in);
		if (file.is_open()) {
			while (file) {
				file >> check;
				file >> comma;

				if (check == 'P') {
					m_items[m_noOfItems + 1] = new Perishable();
					m_items[m_noOfItems + 1]->load(file);
					m_noOfItems++;
				}
				else if (check == 'N') {
					m_items[m_noOfItems + 1] = new NonPerishable();
					m_items[m_noOfItems + 1]->load(file);
					m_noOfItems++;
				}
				else {
					m_noOfItems--;
				}
			}
			file.close();
		}
		else {
			file.clear();
			file.close();
			file.open(m_filename, ios::out);
			file.close();
		}
	}
	void PosApp::saveRecs() {
		fstream file;
		file.open(m_filename, ios::out);
		for (int i = 0; i < m_noOfItems; i++) {
			if (m_items[i]->quantity() > 0) {
				m_items[i]->save(file);
			}
		}
		file.close();
		loadRecs();
	}
	int PosApp::searchItems(const char* sku)const {
		int index = -1;
		for (int i = 0; i < m_noOfItems; i++) {
			if (*m_items[i] == sku) {
				index = i;
			}
		}
		return index;
	}
	void PosApp::updateQty() {
		string sku;
		int ammountPurchased, indexOfItem;
		cout << "Please enter the SKU: ";
		cin >> sku;
		indexOfItem = searchItems(sku.c_str());
		if (indexOfItem == -1) {
			cout << "Not found!" << endl;
		}
		else if (indexOfItem != -1) {
			m_items[indexOfItem]->write(cout, false);
			cout << endl;
			cout << "Please enter the number of purchased items: ";
			cin >> ammountPurchased;
			*m_items[indexOfItem] += ammountPurchased;
			saveRecs();
			cout << "Updated!" << "\n" << endl;
		}
	}
	//fixed the bug where a gargabe item would always appear when adding a NPerishable or Perishable item
	//the garbage item would appear at the last element of m_items array.
	//used suggestions from classmates in OOP lab to fix the bug.
	//the code on the top is the old version
	void PosApp::addItem(bool isPerishable) {
		/*v1.1
		Item* item;
		if (isPerishable) {
			item = new Perishable();
		}
		else if (!isPerishable) {
			item = new NonPerishable();
		}
		cin >> *item;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << *item << "\n" << endl;
		}
		else {
			m_items[m_noOfItems] = item;
			m_noOfItems++;
			saveRecs();
			cout << "Item added." << endl << endl;
		}*/
		//v1.3
		loadRecs();
		if (isPerishable) {
			m_items[m_noOfItems] = new Perishable();
		}
		else {
			m_items[m_noOfItems] = new NonPerishable();
		}
		m_items[m_noOfItems]->read(cin);
		if (!cin.fail()) {
			m_noOfItems++;
			saveRecs();
			cout << "Item added." << endl << endl;
		}
		else if (cin.fail()){
			cin.clear();
			cout << *m_items[m_noOfItems] << endl << endl;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			delete m_items[m_noOfItems];
		}
	}
	//v1.3 fixed a bug where empty spaces would be filled with 0s by adding setfill(' ')
	void PosApp::listItems()const {
		double asset = 0.0;
		cout << " Row | SKU    | Item Name          | Price |TX |Qty |   Total |" << endl;
		cout << "-----|--------|--------------------|-------|---|----|---------|" << endl;
		for (int i = 0; i < m_noOfItems; i++) {
			cout << setw(4) << right << setfill(' ') << i + 1 << " | ";
			m_items[i]->write(cout, true);
			asset += m_items[i]->cost() * m_items[i]->quantity();
		}
		cout << "-----^--------^--------------------^-------^---^----^---------^" << endl;
		cout << "                               Total Asset: $  |" << setw(14) << right << fixed << setprecision(2) << asset << "|" << endl;
		cout << "-----------------------------------------------^--------------^" << '\n' << endl;
	}
	void PosApp::truncateBillFile() {
		fstream file;
		file.open(m_billfname, std::ofstream::out | std::ofstream::trunc);
		file.close();
	}
	//collaborated with Robert veronov,
	void PosApp::showBill() {
		double total = 0.0;
		char check, comma;
		Item* itemBuffer;
		fstream file(m_billfname, ios::in);

		cout << "v--------------------------------------------------------v" << endl;
		cout << "| " << Date() << setfill(' ') << setw(39) << "|" << endl;
		cout << "| SKU    | Item Name          | Price |TX |Qty |   Total |" << endl;
		cout << "|--------|--------------------|-------|---|----|---------|" << endl;

		while (!file.eof()) {
			file >> check;
			file >> comma;
			itemBuffer = nullptr;

			if (check == 'P') {
				itemBuffer = new Perishable();
			}
			else if (check == 'N') {
				itemBuffer = new NonPerishable();
			}
			if (check == 'P' || check == 'N') {
				itemBuffer->load(file);
				if (!(itemBuffer->isEmpty())) {
					cout << "| ";
					itemBuffer->write(cout, true);
					total += itemBuffer->cost();
				}
			}
		}
		cout << "^--------^--------------------^-------^---^----^---------^" << endl;
		cout << "|                               Total: $  |" << setw(14) << right << fixed << setprecision(2) << total << "|" << endl;
		cout << "^-----------------------------------------^--------------^" << endl << endl;
		file.close();
		truncateBillFile();
	}
	void PosApp::addToBill(Item& I) {
		fstream file;
		file.open(m_billfname, ios::out | ios::app);
		int quantity = I.quantity();
		I.quantity(1);
		I.save(file);
		I.quantity(quantity - 1);
		file.close();
		saveRecs();
	}
	//used getline(cin,sku) instead of cin >> sku to check if user entered <enter>
	void PosApp::POS() {
		string sku;
		bool go = true;
		int skuIndex;
		while (go) {
			cout << "Sku: ";
			//cin >> sku;
			getline(cin, sku);
			if (strcmp(sku.c_str(), "") == 0) {
				showBill();
				go = false;
			}
			else if (strcmp(sku.c_str(), "") != 0) {
				skuIndex = searchItems(sku.c_str());
				if (skuIndex != -1) {
					cout << "v------------------->" << endl;
					cout << "| " << m_items[skuIndex]->name() << endl;
					cout << "^------------------->" << endl;
					addToBill(*m_items[skuIndex]);
				}
				else if (skuIndex == -1) {
					cout << "Not found!" << endl;
				}
			}
		}
	}
	void PosApp::run(){
		string sku;
		loadRecs();
		int menuSelection;
		do {
			menuSelection = menu();
			cout << endl;
			if (menuSelection == -1) {
				cout << "===Invalid Selection, try again===" << endl << endl;
			}
			else if (menuSelection == 1) {
				listItems();
			}
			else if (menuSelection == 2) {
				addItem(true);
			}
			else if (menuSelection == 3) {
				addItem(false);
			}
			else if (menuSelection == 4) {
				updateQty();
			}
			else if (menuSelection == 5) {
				cout << "Please enter the SKU: ";
				cin >> sku;
				int skuIndex = searchItems(sku.c_str());
				if (skuIndex == -1) {
					cout << "Not found!" << endl << endl;
				}
				else {
					cout << "v-----------------------------------v" << endl;
					m_items[skuIndex]->write(cout, false);
					cout << "^-----------------------------------^" << endl << endl;
				}
			}
			else if (menuSelection == 6) {
				POS();
			}
		} while (menuSelection != 0);
		cout << "Goodbye!" << endl;
	}
}

/*
ouputs:
-------------------------------------
update:
Please enter the SKU: 1313
Name:
Paper Tissue
Sku: 1313
Price: 1.22
Price after tax: 1.38
Quantity: 194
Total Cost: 267.45

Please enter the number of purchased items: 12
Updated!

------------------------------
Please enter the SKU: 3434
Not found!

-------------------------------------
add item:
Perishable Item Entry:
Sku: 5656
Name:
Honey
Price: 12.99
Taxed: y
Quantity: 12
Expiry date (YYYY/MM/DD): 1200/10/12
Invalid Year in Date Entry

Perishable Item Entry:
Sku: 5656
Name:
Honey
Price: 12.99
Taxed: y
Quantity: 12
Expiry date (YYYY/MM/DD): 2017/5/15
Item added.

--------------------------------------------
list item:
 Row | SKU    | Item Name          | Price |TX |Qty |   Total |
-----|--------|--------------------|-------|---|----|---------|
   1 | 1234   |Milk                |   3.99|  P|   2|     7.98|
   2 | 3456   |Paper Cups          |   5.99| TN|  38|   257.21|
   3 | 4567   |Butter              |   4.56| TP|   9|    46.38|
   4 | 1212   |Salted Butter       |   5.99|  P| 111|   664.89|
   5 | 1313   |Paper Tissue        |   1.22| TN| 206|   283.99|
   6 | 5656   |Honey               |  12.99| TP|  12|   176.14|
-----^--------^--------------------^-------^---^----^---------^
                               Total Asset: $  |       1436.59|
-----------------------------------------------^--------------^


--------------------------------------
printbill:
v--------------------------------------------------------v
| 2017/04/02, 12:42                                      |
| SKU    | Item Name          | Price |TX |Qty |   Total |
|--------|--------------------|-------|---|----|---------|
| 1212   |Salted Butter       |   5.99|  P|   1|     5.99|
| 1313   |Paper Tissue        |   1.22| TN|   1|     1.38|
| 5656   |Honey               |  12.99| TP|   1|    14.68|
^--------^--------------------^-------^---^----^---------^
|                               Total: $  |         22.05|
^-----------------------------------------^--------------^

-------------------------------------------------------
POS:
Sku: 1212
v------------------->
| Salted Butter
^------------------->
Sku: 1212
v------------------->
| Salted Butter
^------------------->
Sku: 1313
v------------------->
| Paper Tissue
^------------------->
Sku: 1234
v------------------->
| Milk
^------------------->
Sku: 7654
Not found!
Sku: 5656
v------------------->
| Honey
^------------------->
Sku:
v--------------------------------------------------------v
| 2017/04/02, 12:58                                      |
| SKU    | Item Name          | Price |TX |Qty |   Total |
|--------|--------------------|-------|---|----|---------|
| 1212   |Salted Butter       |   5.99|  P|   1|     5.99|
| 1212   |Salted Butter       |   5.99|  P|   1|     5.99|
| 1313   |Paper Tissue        |   1.22| TN|   1|     1.38|
| 1234   |Milk                |   3.99|  P|   1|     3.99|
| 5656   |Honey               |  12.99| TP|   1|    14.68|
^--------^--------------------^-------^---^----^---------^
|                               Total: $  |         32.03|
^-----------------------------------------^--------------^

------------------------------------------------------
run:
The Sene-Store
1- List items
2- Add Perishable item
3- Add Non-Perishable item
4- Update item quantity
5- Show Item
6- POS
0- exit program
> 5

Please enter the SKU: 5656
v-----------------------------------v
Name:
Honey
Sku: 5656
Price: 12.99
Price after tax: 14.68
Quantity: 10
Total Cost: 146.79
Expiry date: 2017/05/15
^-----------------------------------^

The Sene-Store
1- List items
2- Add Perishable item
3- Add Non-Perishable item
4- Update item quantity
5- Show Item
6- POS
0- exit program
> 5

Please enter the SKU: 12345
Not found!

The Sene-Store
1- List items
2- Add Perishable item
3- Add Non-Perishable item
4- Update item quantity
5- Show Item
6- POS
0- exit program
> five

===Invalid Selection, try again===

The Sene-Store
1- List items
2- Add Perishable item
3- Add Non-Perishable item
4- Update item quantity
5- Show Item
6- POS
0- exit program
> 0

Goodbye!

*/