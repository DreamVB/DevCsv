/*
	Csv parser for C++ for reading only
	By Ben a.k.a DreamVB
*/

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

const unsigned char sQuote = '"';

class CDevCsv{
private:
	std::vector<string>rows;
	std::vector<string>headers;
	unsigned char seperator = ',';

	class Error : public std::runtime_error
	{

	public:
		Error(const std::string &msg) :
			std::runtime_error(std::string("CDevCsv : ").append(msg))
		{
		}
	};

	enum class CSVState{
		NoQuotes,
		QuoteField,
		QuoteQuote
	};

	// trim left side of string
	void ltrim(std::string &s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
			return !std::isspace(ch);
		}));
	}

	// trim right end of string
	void rtrim(std::string &s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
			return !std::isspace(ch);
		}).base(), s.end());
	}

	// trim both sides of a string
	void trim(std::string &s) {
		ltrim(s);
		rtrim(s);
	}
	//Convert all the strings in vector to uppercase
	const void uppercase_headers(){
		for (auto &s : headers){
			for (auto &c : s){
				c = toupper(c);
			}
		}
	}

	vector<string>get_row_fields(const std::string &row){
		//Small state machine to cut up the csv row string
		vector<std::string>tmp{ "" };
		CSVState state = CSVState::NoQuotes;
		size_t i = 0;

		for (char c : row){
			switch (state)
			{
			case CSVState::NoQuotes:

				if (c == seperator){
					tmp.push_back("");
					i++;
					break;
				}
				else if (c == sQuote){
					state = CSVState::QuoteField;
					break;
				}
				else{
					tmp[i].push_back(c);
					break;
				}

			case CSVState::QuoteField:
				if (c == sQuote){
					state = CSVState::QuoteQuote;
					break;
				}
				else{
					tmp[i].push_back(c);
					break;
				}
				break;

			case CSVState::QuoteQuote:

				if (c == seperator){
					tmp.push_back("");
					i++;
					state = CSVState::NoQuotes;
					break;
				}
				else if (c == sQuote){
					tmp[i].push_back(sQuote);
					state = CSVState::QuoteField;
					break;
				}
				else{
					state = CSVState::NoQuotes;
					break;
				}
				break;
			}
		}
		return tmp;
	}

public:
	//Used to test if the csv file opened.
	bool is_good = false;

	const void open(std::string Filename, 
		unsigned seperator = ','){

		fstream csv_file;
		std::string row;
		bool Firstline = false;
		this->seperator = seperator;
		//Filename to open
		csv_file.open(Filename, std::ios::in);
		//Test if the filename was opened
		is_good = csv_file.is_open();
		//Check if the file was opened.
		if (is_good){
			while (std::getline(csv_file, row)){
				//Trim the line of spaces
				trim(row);
				//See if the line is not empty
				if (!row.empty()){
					//This gets only the first line the headers
					if (!Firstline){
						//Get the headers
						headers = get_row_fields(row);
						//Uppercase the headers vector
						uppercase_headers();
						//Set first to true
						Firstline = true;
					}
					else{
						//Add the rows to the rows vector.
						rows.push_back(row);
					}
				}
			}
		}
		//Close file.
		csv_file.close();
	}
	//
	template <typename T>
	const T get_field_value(unsigned int row, unsigned int field){
		T ret;
		std::string s_row;
		std::vector<string>vals;
		std::stringstream ss;

		if (field < 0 || field > headers.size()){
			throw Error("Field Index Out Of Range");
		}

		if (row < rows.size()){
			//Get record
			s_row = rows.at(row);
			//Get fields into vector
			vals = get_row_fields(s_row);
			ss << vals[field];
			ss >> ret;
		}
		//Clear up
		ss.clear();
		s_row.clear();
		vals.clear();
		return ret;
	}
	
	template <typename T>
	const T get_field_value(unsigned int row, std::string field){
		T ret;
		std::string fname = field;
		std::string s_row;
		std::vector<string>vals;
		std::stringstream ss;
		size_t f_idx;

		//Uppercase fname
		std::transform(fname.begin(), fname.end(), fname.begin(), ::toupper);
		//Find string in vector
		auto it = find(headers.begin(), headers.end(), fname);
		//Check if string was found on vector
		if (it != headers.end()){
			//Work out the position of the string in the vector
			f_idx = it - headers.begin();
		}
		else{
			//Use default zero
			throw Error("Field Not Found.");
		}
		//Check that row is less than rows count
		if (row < rows.size()){
			//Get row
			s_row = rows.at(row);
			//Get fields into vector
			vals = get_row_fields(s_row);
			ss << vals[f_idx];
			ss >> ret;
		}
		//Clear up
		ss.clear();
		s_row.clear();
		vals.clear();
		return ret;
	}

	const size_t record_count(){
		//Return the number of rows in the csv file.
		return this->rows.size();
	}
	const size_t field_count(){
		//Return the number of fields in the csv file.
		return this->headers.size();
	}

	const std::string get_header(size_t index) const{
		if (index >= headers.size()){
			throw Error("Header Index Out Of Range.");
		}
		else{
			//Return header name
			return headers.at(index);
		}
	}

	~CDevCsv(){
		rows.clear();
		headers.clear();
	}
};