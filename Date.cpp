//improved the read and write && >> << functions based on Prof Fardad and Robert Voronov's suggestions
#define _CRT_SECURE_NO_WARNINGS
#include <iomanip>
#include <iostream>
#include <ctime>
using namespace std;
#include "Date.h"
#include "POS.h"


namespace ict{
  void Date::set(){
    time_t t = time(NULL);
    tm lt = *localtime(&t);
    m_day = lt.tm_mday;
    m_mon = lt.tm_mon + 1;
    m_year = lt.tm_year + 1900;
    if (dateOnly()){
      m_hour = m_min = 0;
    }
    else{
      m_hour = lt.tm_hour;
      m_min = lt.tm_min;
    }
  }

  int Date::value()const{
    return m_year * 535680 + m_mon * 44640 + m_day * 1440 + m_hour * 60 + m_min;
  }

  int Date::mdays()const{
    int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, -1 };
    int mon = m_mon >= 1 && m_mon <= 12 ? m_mon : 13;
    mon--;
    return days[mon] + int((mon == 1)*((m_year % 4 == 0) && (m_year % 100 != 0)) || (m_year % 400 == 0));
  }
  void Date::errCode(int errorCode) {
	  m_readErrorCode = errorCode;
  }
  void Date::set(int year, int mon, int day, int hour, int min) {
	  m_year = year;
	  m_mon = mon;
	  m_day = day;
	  m_hour = hour;
	  m_min = min;
	  m_readErrorCode = NO_ERROR;
  }
  //constructors
  Date::Date() {
	  m_dateOnly = false;
	  set();
  }
  Date::Date(int year, int mon, int day) {
	  m_dateOnly = true;
	  m_year = year;
	  m_mon = mon;
	  m_day = day;
	  m_hour = 0;
	  m_min = 0;
	  m_readErrorCode = NO_ERROR;
  }
  Date::Date(int year, int mon, int day, int hour, int min) {
	  m_dateOnly = false;
	  m_year = year;
	  m_mon = mon;
	  m_day = day;
	  m_hour = hour;
	  m_min = min;
	  m_readErrorCode = NO_ERROR;
  }
  bool Date::operator==(const Date& D)const {
	  return (value() == D.value());
  }

  bool Date::operator!=(const Date& D)const {
	  return (value() != D.value());
  }

  bool Date::operator<(const Date& D)const {
	  return (value() < D.value());
  }

  bool Date::operator>(const Date& D)const {
	  return (value() > D.value());
  }

  bool Date::operator<=(const Date& D)const {
	  return (value() <= D.value());
  }

  bool Date::operator>=(const Date& D)const {
	  return (value() >= D.value());
  }
  int Date::errCode()const {
	  return m_readErrorCode;
  }
  bool Date::bad()const {
	  return (errCode() != 0);
  }
  bool Date::dateOnly()const {
	  return m_dateOnly;
  }
  void Date::dateOnly(bool value) {
	  m_dateOnly = value;
	  if (value == true) {
		  m_hour = 0;
		  m_min = 0;
	  }
  }
  //used Robert's vernov's read function
  istream& Date::read(std::istream& is) {
	  {
		  char slash, comma, colon;

		  if (this->dateOnly())
		  {
			  is >> this->m_year >> slash >> this->m_mon >> slash >> this->m_day;
		  }
		  else
		  {
			  is >> this->m_year >> slash >> this->m_mon >> slash >> this->m_day >> comma >> this->m_hour >> colon >> this->m_min;
		  }

		  if (is.fail())
		  {
			  this->errCode(CIN_FAILED);
		  }
		  else if (MIN_YEAR > this->m_year || this->m_year > MAX_YEAR)
		  {
			  this->errCode(YEAR_ERROR);
		  }
		  else if (1 > this->m_mon || this->m_mon > 12)
		  {
			  this->errCode(MON_ERROR);
		  }
		  else if (this->m_day > this->mdays() || this->m_day < 1)
		  {
			  this->errCode(DAY_ERROR);
		  }
		  else if (0 > this->m_hour || this->m_hour > 23)
		  {
			  this->errCode(HOUR_ERROR);
		  }
		  else if (0 > this->m_min || this->m_min > 59)
		  {
			  this->errCode(MIN_ERROR);
		  }
		  else {
			  this->errCode(NO_ERROR);
		  }

		  return is;
	  }
  }

  std::ostream& Date::write(std::ostream& ostr)const {
	  if (m_dateOnly) {
		  ostr << this->m_year << "/" << std::setfill('0') << std::setw(2) << this->m_mon << "/" << std::setfill('0') << std::setw(2) << this->m_day;
	  }
	  else {
		  ostr << this->m_year << "/" << std::setfill('0') << std::setw(2) << this->m_mon << "/" << std::setfill('0') << std::setw(2) << this->m_day << ", " << std::setfill('0') << std::setw(2);
		  ostr << this->m_hour << ":" << std::setfill('0') << std::setw(2) << this->m_min;
	  }
	  return ostr;
  }

  std::istream& operator >> (std::istream& is, Date& d) {
	  d.read(is);
	  return is;
  }

  std::ostream& operator << (std::ostream& os, const Date& d) {
	  d.write(os);
	  return os;
  }

}
