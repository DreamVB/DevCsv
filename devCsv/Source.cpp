/*
	Csv parser for C++ for reading only
	By Ben a.k.a DreamVB
*/

#include <iostream>
#include <string>
#include "devcsv.h"

using namespace std;

int main(int argc, char *argv[]){
	CDevCsv csv;

	if (argc != 2){
		std::cout << "Use " << argv[0] << " filename.csv" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	//Open csv file.
	csv.open(argv[1]);

	if (!csv.is_good){
		std::cerr << "Error loading source csv filename" << std::endl;
		exit(EXIT_FAILURE);
	}

	//Print the name of the second header
	std::cout << "Make" << std::endl << "..........." << std::endl;
	//Print the car make types.
	for (int x = 0; x < csv.record_count(); x++){
		std::cout << csv.get_field_value<string>(x, "Make") << std::endl;
	}
	std::cout << endl << "Year" << std::endl << "..........." << std::endl;
	//Print the car make year.
	for (int x = 0; x < csv.record_count(); x++){
		std::cout << csv.get_field_value<int>(x, 0) << std::endl;
	}
	return EXIT_SUCCESS;
}