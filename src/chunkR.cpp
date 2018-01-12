// [[Rcpp::interfaces(cpp)]]
// [[Rcpp::plugins("cpp11")]]

#include <Rcpp.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <chunkR.h>
using namespace Rcpp;

//StringVector NULLstringv(0);
//const StringVector empty_stringv(0);

StringMatrix NULLstringm(0);
const StringMatrix empty_stringm = NULLstringm;
std::vector<std::string> autovector;
#define auto_vector autovector

namespace _chunkR {

//' reader__reader
//' @keywords internal

reader::reader(std::string path, char sep, bool has_colnames, bool has_rownames,
		size_t chunksize) :
		path(path), sep(sep), has_colnames(has_colnames), has_rownames(
				has_rownames), chunksize(chunksize), n_row(0), n_col(0), rnames(
				[&chunksize] {std::vector<std::string> out; out.reserve(chunksize); return out;}()), cnames(
				[] {std::vector<std::string> out; return out;}()), ifs(
				[&path] {std::ifstream out; return out;}()), pointer_position(
				0), line(new std::string), element(new std::string), lines_completed(
				0), temp(auto_vector), data_chunk(empty_stringm) {

	Rcout << "New reader object\n";
	Rcout << "Path: " << path << std::endl;
	set_colnames();
}

//' reader__destructor 
//' @keywords internal

reader::~reader() {
	delete line;
	delete element;
}

//' reader__set_colnames 
//' @keywords internal

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

	if (has_colnames) {
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
	if (has_colnames) {
		try {
			size_t number_in_has_colnames = cnames.size();

			if (number_in_has_colnames != n_col) {
				std::ostringstream msg;
				msg << "Error: Number of strings in has_colnames ("
						<< number_in_has_colnames << ") " << "has not " << n_col
						<< " elements";
				throw(msg.str());
			}
		} catch (std::string& m) {
			Rcout << m;
		}
	}

	if (!has_colnames) {
	  cnames = set_generic_colnames("C", 1, n_col);
	}
	
	ifs.close();
	// si no hay has_colnames, creaar uno default.
}

//' reader__next_chunk
//' @keywords internal

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
		size_t lines_read_chunk = 0;
    size_t initial_lines  = lines_completed;
	
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
		for (size_t i = 0; i < lines_read_chunk; ++i) {
			for (size_t j = 0; j < n_col; ++j) {
				output(i, j) = temp[k++];
			}
		}

		StringVector cnames_rcpp;
		cnames_rcpp = cnames;
		colnames(output) = cnames_rcpp;

		// set rownames
		StringVector rnames_rcpp;
		if (has_rownames) {
		  rnames_rcpp = rnames;
		  rnames.clear();
		} else {
		  rnames_rcpp = set_generic_rownames("R", initial_lines + 1, lines_read_chunk);
		}
		rownames(output) = rnames_rcpp;

		//cnames.clear();
		temp.clear();
		ifs.close();

		if (output.nrow() == 0) {
		  StringMatrix output(0);
		  data_chunk = output;
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

//' reader__get_dataframe
//' @keywords internal

DataFrame reader::get_dataframe() {
	List output(n_col);
	for (size_t i = 0; i < n_col; ++i) {
		output[i] = data_chunk(_, i);
	}
	output.attr("row.names") = rownames(data_chunk);
	output.attr("names") = colnames(data_chunk);
	output.attr("class") = "data.frame";
	return output;
}

// reader__set_generic_rownames

std::vector<std::string> reader::set_generic_rownames(std::string what, size_t start_from, size_t n_row) {
  std::ostringstream os;
  std::vector<std::string> output;
  output.reserve(n_row);
  for (size_t i = start_from; i < start_from + n_row; ++i) {
    os << what << "_" << i;
    output.push_back(os.str());
    os.str("");
    os.clear();
  }
  return output;
}

// set_generic_colnames

std::vector<std::string> reader::set_generic_colnames(std::string what,  size_t start_from, size_t n_col) {
  std::ostringstream os;
  std::vector<std::string> output;
  output.reserve(n_col);
  for (size_t i = start_from; i < start_from + n_col; ++i) {
    os << what << "_" << i;
    output.push_back(os.str());
    os.str("");
    os.clear();
  }
  return output;
}

//' reader__get_colnames
//' @keywords internal

StringVector reader::get_colnames() {
	StringVector out(cnames.size());
	out = cnames;
	return out;
}

//' reader__get_matrix
//' @keywords internal

StringMatrix reader::get_matrix() {
	return data_chunk;
}

//' reader__get_completed
//' @keywords internal

size_t reader::get_completed() {
	return lines_completed;
}

// reader_module

RCPP_MODULE(reader_module)
{
	class_<reader>("reader")
	.constructor<std::string, char, bool, bool, unsigned int>()
	.method("set_colnames", &reader::set_colnames)
	.method("get_colnames", &reader::get_colnames)
	.method("next_chunk", &reader::next_chunk)
	.method("get_matrix", &reader::get_matrix)
	.method("get_completed", &reader::get_completed)
	.method("get_dataframe", &reader::get_dataframe)
	;
}

} /* namespace _chunkR */
