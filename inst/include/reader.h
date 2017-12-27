#ifndef READER_H_
#define READER_H_

#include <Rcpp.h>
#include <iostream>
#include <fstream>
using namespace Rcpp;

namespace _reader {

class reader {

public:
	reader(std::string path, char sep, bool header, bool has_row_names,
			unsigned int chunksize);
	virtual ~reader();
	void set_header();
	bool next_chunk();
	StringVector get_header();
	StringMatrix get_data();
	unsigned int get_completed();

private:
	const std::string path;
	const char sep;
	bool header;
	const bool has_row_names;
	unsigned const int chunksize;

	unsigned int n_row;
	unsigned int n_col;
	std::vector<std::string> rnames;
	std::vector<std::string> cnames;

	std::ifstream ifs;
	unsigned int pointer_position;

	std::string* line;
	std::string* element;
	unsigned int lines_completed;
	std::vector<std::string> temp;
	StringMatrix data_chunk;

};

} /* namespace _reader */

#endif /* READER_H_ */
