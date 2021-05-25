/*
	Csv parser for C++ for reading and writeing
	Version 2.0
	By Ben a.k.a DreamVB
*/

#include <iostream>
#include <string>
#include "devcsv.h"

using namespace std;

int main(int argc, char *argv[]){
	CDevCsv csv;
	vector<string>Cols;
	vector<string>headers;

	csv.set_seperator(',');

	//Setup header names
	headers.push_back("name");
	headers.push_back("gender");
	headers.push_back("age");
	//Add the new headers
	csv.new_headers(headers);

	//Add new record
	Cols.push_back("Jenny");
	Cols.push_back("Female");
	Cols.push_back("32");
	//Add new record
	csv.add_record(Cols);

	//Add next record
	Cols.push_back("Mat");
	Cols.push_back("male");
	Cols.push_back("44");
	//Add the new record
	csv.add_record(Cols);

	//Save file file name.
	if (!csv.save("persons.csv")){
		std::cout << "Error creating csv file." << std::endl;
		exit(EXIT_FAILURE);
	}

	csv.clear();
	//Load csv file

	csv.open("persons.csv");

	if (!csv.is_good){
		std::cout << "Faild the open csv file." << std::endl;
		exit(EXIT_FAILURE);
	}
	//Change the second records age.
	csv.set_field_value<int>(1, "age", 65);

	//Display the records we just opened
	for (int x = 0; x < csv.record_count(); x++){
		std::cout << csv.get_field_value<string>(x, "name") << "\t"
			<< csv.get_field_value<string>(x, "gender") << "\t"
			<< csv.get_field_value<string>(x,"age") << std::endl;
	}


	//Clear up
	csv.clear();

	system("pause");
	return EXIT_SUCCESS;
}