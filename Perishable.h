//mine
#ifndef ICT_PERISHABLE_H__
#define ICT_PERISHABLE_H__
#include <iostream>
#include "NonPerishable.h"
#include "Date.h"
#include "PosIO.h"
using namespace std;
namespace ict {
	class Perishable :public NonPerishable
	{
		private:
			Date m_expiry;
		protected:
			//constructor
			//methods
			char signature()const;
		public:
			Perishable();
			fstream& save(fstream& file)const;
			fstream& load(fstream& file);
			ostream& write(ostream& os, bool linear)const;
			istream& read(istream& istr);
			//istr.setstate(ios::failbit);
	};
}





#endif
