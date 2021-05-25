/*
	Csv parser for C++ for reading and writeing
	Version 2.0
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
	const void uppercase_headers(void){
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

	const void open(std::string Filename){

		fstream csv_file;
		std::string row;
		bool Firstline = false;
		//Filename to open
		csv_file.open(Filename, std::ios::in);
		//Test if the filename was opened
		is_good = csv_file.is_open();
		//Check if the file was opened.
		if (is_good){
			headers.clear();
			rows.clear();
			while (std::getline(csv_file, row)){
				//check if the file is ok
				if (csv_file.bad() || csv_file.fail()){
					is_good = false;
					break;
				}
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

	bool save(const std::string file) const{
		fstream csv_file;
		std::string fisrtline;

		//Open the file for writeing.
		csv_file.open(file, std::ios::out);
		
		if (!csv_file.good()){
			return false;
		}
		else{
			//Get the headers as the first line
			for (size_t t = 0; t < headers.size(); t++){
				if (t < headers.size() - 1){
					fisrtline += headers[t];
					fisrtline.push_back(seperator);
				}
				else{
					fisrtline += headers[t];
				}
			}
			//Add the headers to the data vector.
			csv_file << fisrtline << std::endl;
			//Add the records
			for (size_t t = 0; t < rows.size(); t++){
				csv_file << rows[t] << std::endl;
			}
		}
		//Close the file.
		csv_file.close();
		return true;
	}

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

	template <typename T>
	void set_field_value(size_t row, std::string key, T data){
		std::string fname = key;
		std::string s_row;
		std::vector<string>vals;
		std::stringstream ss;
		T Ret;
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
			throw Error("Key Not Found.");
		}

		if (row < rows.size()){
			std::string s_rec;
			s_rec = rows[row];
			//
			vals = get_row_fields(s_rec);
			//Set the vals data
			ss << data;
			ss >> vals[f_idx];

			//Clear row data
			s_row.clear();
			//Build the new record from the vals vector.
			for (int t = 0; t < vals.size(); t++){
				if (t < vals.size() - 1) {
					s_row += vals[t];
					s_row.push_back(seperator);
				}
				else{
					s_row += vals[t];
				}
			}
			//Check for empry row
			if (!s_row.empty()){
				rows[row] = s_row;
			}
			else{
				throw Error("Record Is Empty");
			}
		}
		vals.clear();
		ss.clear();
	}

	const void add_record(std::vector<string>&colnums){
		std::string s_rec;

		if (colnums.empty()){
			throw Error("No Colunms To Add.");
		}
		if (colnums.size() > headers.size()){
			throw Error("Colunms Out Of Range.");
		}
		//
		for (int x = 0; x < colnums.size();x++){
			if (x < colnums.size() - 1){
				s_rec += colnums[x];
				s_rec.push_back(seperator);
			}
			else{
				s_rec += colnums[x];
			}
		}
		//Add the new record.0.

		rows.push_back(s_rec);
		//Clear the colunms ater each reocrd add.
		colnums.clear();
	}

	const void delete_record(size_t rec){
		if (rec < rows.size()){
			rows.erase(rows.begin() + rec);
		}
		else{
			throw Error("No Record To Delete");
		}
	}

	size_t record_count(void) const{
		//Return the number of rows in the csv file.
		return this->rows.size();
	}
	size_t field_count(void) const{
		//Return the number of fields in a single record same as headers count
		return this->headers.size();
	}

	size_t header_count(void) const{
		//Return the number of headers in the csv file.
		return this->headers.size();
	}

	const std::string get_header(const size_t index) const{
		if (index >= headers.size()){
			throw Error("Header Index Out Of Range.");
		}
		else{
			//Return header name
			return headers.at(index);
		}
	}

	//Add headers
	const void new_headers(const std::vector<string>colnums){
		std::string s_rec;

		if (colnums.empty()){
			throw Error("No Headers To Add.");
		}
		//Add to headers vector.
		headers = colnums;
		uppercase_headers();
	}

	const void set_header(size_t index, const std::string name){
		std::string fname = name;
		//Uppercase the header name.
		std::transform(fname.begin(), fname.end(), fname.begin(), ::toupper);

		if (index >= headers.size()){
			throw Error("Header Index Out Of Range.");
		}
		else{
			//Set header name
			headers[index] = fname;
		}
	}

	const void delete_header(size_t index){
		std::vector<string>temp;
		std::vector<string>temp_rows;
		std::string s_rec;
		//Delete header

		if (index >= headers.size()){
			throw Error("Header Index Out Of Range.");
		}
		else{
			//Next we need to delete the record field.
			for (size_t t = 0; t < rows.size(); t++){
				temp = get_row_fields(rows[t]);
				//Delete the item from temp at position index.
				temp.erase(temp.begin() + index);
				
				//Re-build the records
				for (size_t n = 0; n < temp.size();n++){
					if (n < temp.size() - 1){
						s_rec += temp[n];
						s_rec.push_back(seperator);
					}
					else{
						s_rec += temp[n];
					}
				}
				//Add to temp record vector
				temp_rows.push_back(s_rec);
				//Clear buffer
				s_rec.clear();
			}
			//
		}
		//Delete the header
		headers.erase(headers.begin() + index);
		//Set the new rows
		rows = temp_rows;

		//Clear up
		temp.clear();
		temp_rows.clear();
	}

	const void set_seperator(const unsigned char sep){
		this->seperator = sep;
	}

	const void clear(void){
		headers.clear();
		rows.clear();
	}

	~CDevCsv(){
		rows.clear();
		headers.clear();
	}
};