# Dev Csv
CSV parser for C++

This is a simple easy to use parser class for reading CSV (comma-separated values) in C++.

# Features

- Read header names
- Read field values uses a template so can read in any value
- Fields can be read using an index number, or you can use the header name
- Read the number of fields
- Read the total number of records

# Using the class

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

# Next version
- Add writing to CSV files.
- Fix any bugs
