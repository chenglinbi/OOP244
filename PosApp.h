#ifndef POSAPP_H
#define POSAPP_H
#include <cstring>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
//add all .h files
#include "Item.h"
#include "NonPerishable.h"
#include "Perishable.h"
#include "Date.h"
#include "Error.h"
#include "POS.h"
#include "PosIO.h"
namespace ict {
	class PosApp {
	private:
		std::fstream data;
		//Private member variables (attributes)
		char m_filename[128];
		char m_billfname[128];
		Item* m_items[MAX_NO_ITEMS];
		int m_noOfItems;
		//Private member functions (methods)
		int menu();
		//Data management member functions (methods)
		void deallocateItems();
		void loadRecs();
		void saveRecs();
		int searchItems(const char* sku)const;
		void updateQty();
		void addItem(bool isPerishable);
		void listItems()const;
		//Point Of Sale member functions(methods)
		void truncateBillFile();
		void showBill();
		void addToBill(Item& I);
		void POS();

	
	public:
		//constructor
		PosApp(const char* filename, const char* billfname);
		void run();

	};
}
 
#endif