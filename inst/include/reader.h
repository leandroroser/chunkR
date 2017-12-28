#ifndef READER_H_
#define READER_H_

#include <Rcpp.h>
#include <iostream>
#include <fstream>
using namespace Rcpp;

namespace _reader {

class reader {

public:
	reader(std::string path, char sep, bool has_colnames, bool has_rownames,
			unsigned int chunksize);
	virtual ~reader();
	void set_colnames();
	bool next_chunk();
	StringVector get_colnames();
	StringMatrix get_data();
	unsigned int get_completed();

private:
	const std::string path;
	const char sep;
	bool has_colnames;
	const bool has_rownames;
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
