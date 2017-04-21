#define _CRT_SECURE_NO_WARNINGS
// lib includes
#include "Perishable.h"
using namespace std;
namespace ict {
	char Perishable::signature()const {
		return 'P';
	}
	Perishable::Perishable() {
		m_expiry.dateOnly(true);
	}
	std::fstream& Perishable::save(std::fstream& file)const {
		NonPerishable::save(file);
		file << ',' << this->m_expiry;
		return file;
	}
	//used char comma instead of ignore based on Robert Veronvo's suggestion
	std::fstream& Perishable::load(std::fstream& file) {
		char comma;
		NonPerishable::load(file);
		file >> comma >> this->m_expiry;
		return file;
	}
	std::ostream& Perishable::write(ostream& os, bool linear)const {
		NonPerishable::write(os,linear);
		if (this->ok() && !linear) {
			os << "Expiry date: " << m_expiry << endl;
		}
		return os;
	}
	std::istream& Perishable::read(istream& istr) {
		/*cout << "Perishable ";
		NonPerishable::read(istr);
		cout << "Expiry date (YYYY/MM/DD): ";
		istr >> this->m_expiry;
		if (istr.fail()) {
			if (this->m_expiry.errCode() == CIN_FAILED) {
				NonPerishable::error("Invalid Date Entry");
			}
			else if (this->m_expiry.errCode() == YEAR_ERROR) {
				NonPerishable::error("Invalid Year in Date Entry");
			}
			else if (this->m_expiry.errCode() == MON_ERROR) {
				NonPerishable::error("Invalid Month in Date Entry");
			}
			else if (this->m_expiry.errCode() == DAY_ERROR) {
				NonPerishable::error("Invalid Day in Date Entry");
			}
			istr.setstate(ios::failbit);
		}
		return istr;*/
		cout << "Perishable ";
		NonPerishable::read(istr);
		if (!istr.fail()) {
			cout << "Expiry date (YYYY/MM/DD): ";
			istr >> this->m_expiry;
			if (istr.fail() || this->m_expiry.errCode() != 0) {
				if (this->m_expiry.errCode() == CIN_FAILED) {
					NonPerishable::error("Invalid Date Entry");
				}
				else if (this->m_expiry.errCode() == YEAR_ERROR) {
					NonPerishable::error("Invalid Year in Date Entry");
				}
				else if (this->m_expiry.errCode() == MON_ERROR) {
					NonPerishable::error("Invalid Month in Date Entry");
				}
				else if (this->m_expiry.errCode() == DAY_ERROR) {
					NonPerishable::error("Invalid Day in Date Entry");
				}
				istr.setstate(ios::failbit);
			}
		}
		return istr;
	}
}
