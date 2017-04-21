#define _CRT_SECURE_NO_WARNINGS
// lib includes
#include <iomanip>
#include "NonPerishable.h"


using namespace std;
namespace ict {
	bool NonPerishable::ok() const {
		return (m_err.isClear());
	}
	void NonPerishable::error(const char* message) {
		m_err.message(message);
	}
	char NonPerishable::signature()const {
		return 'N';
	}
	std::fstream& NonPerishable::save(std::fstream& file)const {
		file << this->signature() << "," << this->sku() << "," << this->name() << "," << this->price() << "," << this->taxed() << "," << this->quantity();
		return file;
	}
	//used c_str() based on Robert Voronov's suggestion
	std::fstream& NonPerishable::load(std::fstream& file) {
		string sku, name;
		double price;
		bool taxed;
		int quantity;
		char comma;

		if (!(this->ok())) {
			cout << this->m_err;
		}
		else {
			getline(file, sku, ',');
			getline(file, name, ',');
			file >> price >> comma >> taxed >> comma >> quantity;
			this->sku(sku.c_str());
			this->name(name.c_str());
			this->price(price);
			this->taxed(taxed);
			this->quantity(quantity);
		}
		return file;
	}
	//v1.3 fixed a bug where empty spaces would be filled with 0s by adding setfill(' ')
	std::ostream& NonPerishable::write(std::ostream& ostr, bool linear)const {
		if (!ok()) {
			ostr << this->m_err;
		}
		else if (ok()) {
			if (linear) {
				ostr << setw(MAX_SKU_LEN) << left << setfill(' ') << this->sku();
				ostr << "|";
				ostr << setw(20) << left << setfill(' ') << this->name();
				ostr << "|";
				ostr << setw(7) << right << fixed << setprecision(2) << setfill(' ') << this->price();
				ostr << "|";
				if (this->taxed()) {
					ostr << setw(2) << right << "T" << setfill(' ') << this->signature();
				} if (!this->taxed()) {
					ostr << setw(3) << setfill(' ') << this->signature();
				}
				ostr << "|";
				ostr << setw(4) << right << setfill(' ') << this->quantity();
				ostr << "|";
				ostr << setw(9) << right << fixed << setprecision(2) << setfill(' ') << (this->quantity() * this->cost());
				ostr << "|" << endl;
			}
			else if (!linear && this->taxed()){
				ostr << "Name:" << endl;
				ostr << this->name() << endl;
				ostr << "Sku: " << this->sku() << endl;
				ostr << "Price: " << fixed << setprecision(2) << this->price() << endl;
				ostr << "Price after tax: " << fixed << setprecision(2) << (this->price() * (1 + TAX)) << endl;
				ostr << "Quantity: " << this->quantity() << endl;
				ostr << "Total Cost: " << fixed << setprecision(2) << (this->quantity() * this->cost()) << endl;
			}
			else if (!linear && !this->taxed()) {
				ostr << "Name:" << endl;
				ostr << this->name() << endl;
				ostr << "Sku: " << this->sku() << endl;
				ostr << "Price: " << fixed << setprecision(2) << this->price() << endl;
				ostr << "Price after tax: N/A" << endl;
				ostr << "Quantity: " << this->quantity() << endl;
				ostr << "Total Cost: " << fixed << setprecision(2) << (this->quantity() * this->cost()) << endl;
			}
		}
		return ostr;
	}
	// used the read function from Robert Voronov
	std::istream& NonPerishable::read(std::istream& is) {
		char m_sku[MAX_SKU_LEN + 1];
		string m_name;
		double m_price;
		char m_taxed;
		int m_quantity;
		this->m_err.clear();

		while (!is.fail()) {
			cout << "Item Entry:" << endl;
			cout << "Sku: ";
			is >> m_sku;
			cout << "Name:" << endl;
			is.ignore();
			getline(is, m_name, '\n');
			cout << "Price: ";
			is >> m_price;
			if (is.fail()) {
				this->m_err = "Invalid Price Entry";
				break;
			}
			cout << "Taxed: ";
			is >> m_taxed;
			if (is.fail() || (m_taxed != 'y' && m_taxed != 'n')) {
				this->m_err = "Invalid Taxed Entry, (y)es or (n)o";
				is.setstate(ios::failbit);
				break;
			}
			cout << "Quantity: ";
			is >> m_quantity;
			if (is.fail()) {
				this->m_err = "Invalid Quantity Entry";
				break;
			}

			this->sku(m_sku);
			this->name(m_name.c_str());
			this->price(m_price);
			if (m_taxed == 'y') {
				this->taxed(true);
			}
			else {
				this->taxed(false);
			}
			this->quantity(m_quantity);

			break;
		}
		return is;
	}
}

