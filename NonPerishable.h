//mine
#ifndef ICT_NONPERISHABLE_H__
#define ICT_NONPERISHABLE_H__
#include "Item.h"
#include "PosIO.h"
#include "Error.h"
#include <iostream>
#include <cstring>
#include <string>

using namespace std;
namespace ict {
	class NonPerishable : public Item
	{
	private:
		Error m_err;
	protected:
		bool ok() const;
		void error(const char* message);
		virtual char signature()const;
	public:
		fstream& save(fstream& file)const;
		fstream& load(std::fstream& file);
		ostream& write(ostream& ostr, bool linear)const;
		istream& read(istream& is);
		//istr.setstate(ios::failbit);

	};
}







#endif