#ifndef ICT_ITEM_H__
#define ICT_ITEM_H__
// inlcude PosIO and POS header files
#include "POS.h"
#include "PosIO.h"

namespace ict{
  // class Item
	class Item: public PosIO
	{
	public:
		//constructor
		Item();
		Item(const char* sku,const char* name, double price, bool tax = true);
		//copy constructor
		Item(const Item&);
		//assignment operator
		Item& operator=(const Item&);
		//destructor
		virtual ~Item();
		//setters
		void sku(const char* sku);
		void price(double price);
		void name(const char* name);
		void taxed(bool taxed);
		void quantity(int quantity);
		//getters
		const char* sku() const;
		double price() const;
		char* name() const;
		bool taxed() const;
		int quantity() const;
		double cost() const;
		bool isEmpty() const;
		//operator overload
		bool operator==(const char* sku);
		int operator+=(int value);
		int operator-=(int value);

	private:
		char m_sku[MAX_SKU_LEN + 1];
		char* m_name;
		double m_price;
		bool m_taxed;
		int m_quantity;
	};

	  // end class Item
	  // operator += 
	  double operator+=(double& d, const Item& I);
	  // operator << and operator >>
	  std::istream& operator >> (std::istream& is, Item& i);
	  std::ostream& operator << (std::ostream& os, const Item& i);
}


#endif