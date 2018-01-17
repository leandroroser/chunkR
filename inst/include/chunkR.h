#ifndef READER_H_
#define READER_H_

#include <Rcpp.h>
#include <iostream>
#include <fstream>
using namespace Rcpp;

namespace _chunkR {

class reader {

public:
  // matrix constructor
  reader(const std::string path, char sep, bool has_colnames, bool has_rownames,
         size_t chunksize, StringVector column_types);
  // data.frame constructor
	reader(const std::string path, char sep, bool has_colnames, bool has_rownames,
			size_t chunksize);
	virtual ~reader();
	inline List mixed_list(std::vector<int> x,  int howmuch);
	void set_colnames();
	bool next_chunk();
	bool next_chunk_matrix();
	bool next_chunk_df();
	StringVector get_colnames();
	size_t get_completed();
	StringMatrix get_matrix();
	DataFrame get_dataframe();
	DataFrame get_matrix2dataframe();
	const std::string get_type();
	std::vector<std::string> set_generic_rownames(std::string what, size_t start_from, size_t n_row);
	std::vector<std::string> set_generic_colnames(std::string what, size_t start_from, size_t n_col);

private:
	const std::string path;
	const char sep;
	bool has_colnames;
	const bool has_rownames;
	const size_t chunksize;
	const std::string output_format;
	std::vector<int> col_types;

	size_t n_row;
	size_t n_col;
	std::vector<std::string> rnames;
	std::vector<std::string> cnames;
	size_t n_lines;
	
	std::ifstream ifs;
	size_t pointer_position;

	std::string* line;
	std::string* element;
	size_t lines_completed;
	std::vector<std::string> temp;
	bool shorter_table;
  
	struct chunkInfo {
	  StringMatrix m;
	  DataFrame df;
	} data_chunk;

};

} /* namespace _reader */

#endif /* READER_H_ */
