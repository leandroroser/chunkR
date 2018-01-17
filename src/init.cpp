#include <Rcpp.h>
#include <chunkR.h>
using namespace _chunkR;
using namespace Rcpp;

  
// [[Rcpp::export]]
RcppExport SEXP reader__new_data_frame(SEXP path_, SEXP sep_, SEXP has_colnames_,
		SEXP has_rownames_, SEXP chunksize_,  SEXP column_types_) {

	std::string path = Rcpp::as < std::string > (path_);
	char sep = Rcpp::as<char>(sep_);
	bool has_colnames = Rcpp::as<bool>(has_colnames_);
	bool has_rownames = Rcpp::as<bool>(has_rownames_);
	unsigned int chunksize = Rcpp::as<unsigned int>(chunksize_);
	StringVector column_types = Rcpp::as<StringVector>(column_types_);
	Rcpp::XPtr < reader > ptr(new reader(path, sep, has_colnames, 
                                      has_rownames, chunksize, 
                                      column_types), 
                                      true);
	return ptr;
}

// [[Rcpp::export]]
RcppExport SEXP reader__new_matrix(SEXP path_, SEXP sep_, SEXP has_colnames_,
                                       SEXP has_rownames_, SEXP chunksize_) {
  
  std::string path = Rcpp::as < std::string > (path_);
  char sep = Rcpp::as<char>(sep_);
  bool has_colnames = Rcpp::as<bool>(has_colnames_);
  bool has_rownames = Rcpp::as<bool>(has_rownames_);
  unsigned int chunksize = Rcpp::as<unsigned int>(chunksize_);
  Rcpp::XPtr < reader > ptr(new reader(path, sep, has_colnames, 
                                       has_rownames, chunksize), 
                                       true);
  return ptr;
}

// [[Rcpp::export]]
RcppExport bool reader__set_colnames(SEXP ptr) {
	Rcpp::XPtr < reader > data(ptr);
	data->set_colnames();
	return true;
}

// [[Rcpp::export]]
RcppExport bool reader__next_chunk(SEXP ptr) {
	Rcpp::XPtr < reader > data(ptr);
	return data->next_chunk();
}

// [[Rcpp::export]]
RcppExport StringVector reader__get_colnames(SEXP ptr) {
	Rcpp::XPtr < reader > data(ptr);
	return data->get_colnames();
}

// [[Rcpp::export]]
RcppExport StringMatrix reader__get_matrix(SEXP ptr) {
	Rcpp::XPtr < reader > data(ptr);
	return data->get_matrix();
}

// [[Rcpp::export]]
RcppExport DataFrame reader__get_dataframe(SEXP ptr) {
  Rcpp::XPtr < reader > data(ptr);
  return data->get_dataframe();
}

// [[Rcpp::export]]
RcppExport DataFrame reader__get_matrix2dataframe(SEXP ptr) {
  Rcpp::XPtr < reader > data(ptr);
  return data->get_matrix2dataframe();
}

// [[Rcpp::export]]
RcppExport unsigned int reader__get_completed(SEXP ptr) {
	Rcpp::XPtr < reader > data(ptr);
	return data->get_completed();
}

// [[Rcpp::export]]
RcppExport std::string reader__get_type(SEXP ptr) {
  Rcpp::XPtr < reader > data(ptr);
  return data->get_type();
}

