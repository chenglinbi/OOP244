#define _CRT_SECURE_NO_WARNINGS  
// Lib includes
#include <cstring>
// inlcude Item and POS header files
#include "POS.h"
#include "Item.h"
using namespace std;
namespace ict{
  // class Item implementaion
  //constructor
	Item::Item() {
		m_sku[0] = '\0';
		m_name = nullptr;
		m_price = 0;
		m_quantity = 0;
		m_taxed = 0;
	}
	Item::Item(const char* sku, const char* name, double price, bool tax) {
		strcpy(m_sku, sku);
		m_name = new char[strlen(name) + 1];
		strcpy(m_name, name);
		m_quantity = 0;
		m_taxed = tax;
		m_price = price;
	}
	//copy constructor
	Item::Item(const Item& src){
		strcpy(m_sku, src.m_sku);
		m_price = src.m_price;
		m_taxed = src.m_taxed;
		m_quantity = src.m_quantity;

		if (src.m_name != nullptr) {
			m_name = new char[strlen(src.m_name) + 1];
			strcpy(m_name, src.m_name);
		}
		else {
			m_name = nullptr;
		}
	}
	//assignment operator
	Item& Item::operator=(const Item& src) {
		if (src.m_name != nullptr) {
			strcpy(m_sku, src.m_sku);
			m_price = src.m_price;
			m_taxed = src.m_taxed;
			m_quantity = src.m_quantity;
			strcpy(m_name, src.m_name);
		}
		return *this;
	}
	//destructor
	Item::~Item() {
		delete[] m_name;
	}
	//setter
	void Item::sku(const char* sku) {
		strcpy(m_sku, sku);
	}
	void Item::price(double price) {
		m_price = price;
	}
	void Item::name(const char* name) {
		delete[] m_name;
		m_name = new char[strlen(name) + 1];
		strcpy(m_name, name);
	}
	void Item::taxed(bool taxed) {
		m_taxed = taxed;
	}
	void Item::quantity(int quantity) {
		m_quantity = quantity;
	}
	//getter
	const char* Item::sku() const {
		return m_sku;
	}
	double Item::price() const {
		return m_price;
	}
	char* Item::name() const {
		return m_name;
	}
	bool Item::taxed() const {
		return m_taxed;
	}
	int Item::quantity() const {
		return m_quantity;
	}
	double Item::cost() const {
		double cost = 0.0;
		if (taxed()) {
			cost = price() * (1 + TAX);
		}
		else {
			cost = price();
		}
		return cost;
	}
	bool Item::isEmpty() const {
		return (strcmp(m_sku, "") == 0 && m_name == nullptr && m_price == 0 && m_quantity == 0);
	}
	//operator overload
	bool Item::operator==(const char* sku) {
		bool check = false;
		if (strcmp(m_sku, sku) == 0) {
			check = true;
		}
		return check;
	}
	int Item::operator+=(int value) {
		return (m_quantity += value);
	}
	int Item::operator-=(int value) {
		return (m_quantity -= value);
	}
	double operator+=(double& d, const Item& I) {
		d += I.cost() * I.quantity();
		return d;
	}
	std::istream& operator >> (std::istream& is, Item& i) {
		i.read(is);
		return is;
	}
	std::ostream& operator << (std::ostream& os, const Item& i) {
		i.write(os, true);
		return os;
	}
}