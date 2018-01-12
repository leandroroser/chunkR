#ifndef READER_H_
#define READER_H_

#include <Rcpp.h>
#include <iostream>
#include <fstream>
using namespace Rcpp;

namespace _chunkR {

class reader {

public:
	reader(std::string path, char sep, bool has_colnames, bool has_rownames,
			unsigned int chunksize);
	virtual ~reader();
	void set_colnames();
	bool next_chunk();
	StringVector get_colnames();
	StringMatrix get_matrix();
	unsigned int get_completed();
	DataFrame get_dataframe();
	std::vector<std::string> set_generic_rownames(std::string what, int start_from, int n_row);
	std::vector<std::string> set_generic_colnames(std::string what, int start_from, int n_col);

private:
	const std::string path;
	const char sep;
	bool has_colnames;
	const bool has_rownames;
	unsigned const int chunksize;

	size_t n_row;
	size_t n_col;
	std::vector<std::string> rnames;
	std::vector<std::string> cnames;

	std::ifstream ifs;
	unsigned int pointer_position;

	std::string* line;
	std::string* element;
	size_t lines_completed;
	std::vector<std::string> temp;
	StringMatrix data_chunk;

};

} /* namespace _reader */

#endif /* READER_H_ */
