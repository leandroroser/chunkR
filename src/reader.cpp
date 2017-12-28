// [[Rcpp::interfaces(cpp)]]

#include <Rcpp.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <reader.h>
using namespace Rcpp;

//StringVector NULLstringv(0);
//const StringVector empty_stringv(0);

StringMatrix NULLstringm(0);
const StringMatrix empty_stringm = NULLstringm;
std::vector<std::string> autovector;
#define auto_vector autovector

namespace _reader {

//' reader
//' @description reader object constructor
//' @param path Input file path 
//' @param sep Character separating cells 
//' @param has_colnames has_colnames present (Logical)
//' @param has_rown_names Row names present (Logical)
//' @param chunksize Size of chunk (Logical)


reader::reader(std::string path, char sep, bool has_colnames, bool has_rownames,
		unsigned int chunksize) :
		path(path), sep(sep), has_colnames(has_colnames), has_rownames(has_rownames), chunksize(
		chunksize), n_row(0), n_col(0), rnames(
		[&chunksize] {std::vector<std::string> out; out.reserve(chunksize); return out;}()),
		cnames([] {std::vector<std::string> out; return out;}()), 
		ifs([&path] {std::ifstream out; return out;}()),
		pointer_position(0), line(new std::string), element(new std::string), 
		lines_completed(0), temp(auto_vector), data_chunk(empty_stringm) {
  
	Rcout << "New reader object\n";
	Rcout << "Path: " << path << std::endl;
	set_colnames();
}

//' destructor 
//' @description has_colnames destructor


reader::~reader() {
	delete line;
	delete element;
}

//' set_colnames 
//' @description set file has_colnames


void reader::set_colnames() {
  
	ifs.open(path, std::ios::binary);
	try {
		if (ifs.fail()) {
			std::ostringstream msg;
			msg << "Input file opening failed.\n";
			throw(msg.str());
		}
	} catch (std::string& stopmsg) {
		Rcout << stopmsg;
	}

	if(has_colnames) 
	{
		std::getline(ifs, *line);
		std::stringstream headss(*line);
		while (std::getline(headss, *element, sep)) {
			cnames.push_back(*element);
		}
	}
	pointer_position = ifs.tellg();

	// Read first line only to set number of columns
	bool is_name = true;
	std::getline(ifs, *line);
	std::stringstream ss(*line);
	while (std::getline(ss, *element, sep)) {
		if (is_name && has_rownames) {
			is_name = false;
			continue;
		}
		n_col++;
	}

	// check consistency between column number determined and # elements in has_colnames
	if(has_colnames)
	{
	try {
		int number_in_has_colnames = cnames.size();

		if (number_in_has_colnames != n_col) {
			std::ostringstream msg;
			msg << "Error: Number of strings in has_colnames (" << number_in_has_colnames
					<< ") " << "has not " << n_col << " elements";
			throw(msg.str());
		}
	} catch (std::string& m) {
		Rcout << m;
	}
	}
	
	ifs.close();
	// si no hay has_colnames, creaar uno default.
}

//' next_chunk
//' @description read next chunk


bool reader::next_chunk() {

	if (!ifs.eof()) {

		ifs.open(path, std::ios::binary);
		if (ifs.fail()) {
			std::ostringstream msg;
			msg << "Input file opening failed.\n";
			throw(msg.str());
		}

		ifs.seekg(pointer_position);

		// for first line read before
		int lines_read_chunk = 0;

		while (std::getline(ifs, *line)) {
			bool is_name = true;

			std::stringstream ss(*line);
			while (std::getline(ss, *element, sep)) {
				if (is_name && has_rownames) {
					rnames.push_back(*element);
					is_name = false;
					continue;
				}
				temp.push_back(*element);
			}

			n_row++;
			lines_completed++;
			if (++lines_read_chunk >= chunksize) {
				break;
			}
		}

		pointer_position = ifs.tellg();

		// create output
		StringMatrix output(lines_read_chunk, n_col);
		int k = 0;
		for (int i = 0; i < lines_read_chunk; ++i) {
			for (int j = 0; j < n_col; ++j) {
				output(i, j) = temp[k++];
			}
		}

		StringVector cnames_rcpp;
		cnames_rcpp = cnames;
		colnames(output) = cnames_rcpp;

		if (has_rownames) {
			StringVector rnames_rcpp;
			rnames_rcpp = rnames;
			rownames(output) = rnames_rcpp;
		} else {
			//ROWNAMES DEFAULT
		}

		//cnames.clear();
		rnames.clear();
		temp.clear();
		ifs.close();

		if(output.nrow() == 0)
		{
		  StringMatrix temp(0);
		  output = temp;
		  return false;
		}
		data_chunk = output;
		return true;

	} else {
		StringMatrix output(0);
		data_chunk = output;
		return false;
	}

}

//' get_colnames
//' @description get table has_colnames


StringVector reader::get_colnames() {
	StringVector out(cnames.size());
	out = cnames;
	return out;
}

//' get_data
//' @description get data chunk stored in object


StringMatrix reader::get_data() {
	return data_chunk;
}

//' get_completed
//' @description get the number of lines read


unsigned int reader::get_completed() {
	return lines_completed;
}

//' reader_module
//' @description reader module

RCPP_MODULE(reader_module)
{
	class_<reader>("reader")
	.constructor<std::string, char, bool, bool, unsigned int>()
	.method("set_colnames", &reader::set_colnames)
  .method("get_colnames", &reader::get_colnames)
	.method("next_chunk", &reader::next_chunk)
  .method("get_data", &reader::get_data)
  .method("get_completed", &reader::get_completed)
	;
}

} /* namespace _reader */
