# Dev Csv
CSV parser for C++

This is a simple easy to use parser class for reading and writing CSV (comma-separated values) in C++.

# Features

- Read header names
- Read field values
- Write field values
- Add new headers
- Delete headers
- Get header count
- Set header names
- Delete rows
- Add new rows
- Read the number of fields
- Read the total number of records
- Save and load CSV files
- Set custom comma separator

# Using the class Reading a csv file

```cpp
CDevCsv csv;

	//Open csv file.
	csv.open("Filename.csv");
  
  //Check the csv file was opened.
	if (!csv.is_good){
		std::cerr << "Error loading source csv filename" << std::endl;
		exit(EXIT_FAILURE);
	}
  
  //Read a field by header name
  std::cout << csv.get_field_value<string>(0, "test") << std::endl;
    //Read a field by index
  std::cout << csv.get_field_value<string>(0, 0) << std::endl;
  
```

# Using the class Adding new headers, data and saving.

```cpp
CDevCsv csv;
	vector<string>Cols;
	vector<string>headers;

	csv.set_seperator(';');

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

	//Save file name.
	if (!csv.save("persons.csv")){
		std::cout << "Error creating csv file." << std::endl;
		exit(EXIT_FAILURE);
	}

	csv.clear();
  
```
