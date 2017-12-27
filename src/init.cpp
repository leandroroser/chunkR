#include <Rcpp.h>
#include <reader.h>
using namespace _reader;
using namespace Rcpp;

//' @param x A single integer.
//' @export
// [[Rcpp::export]]
RcppExport SEXP reader__new(SEXP path_, SEXP sep_, SEXP header_,
		SEXP has_row_names_, SEXP chunksize_) {

	std::string path = Rcpp::as < std::string > (path_);
	char sep = Rcpp::as<char>(sep_);
	bool header = Rcpp::as<bool>(header_);
	bool has_row_names = Rcpp::as<bool>(has_row_names_);
	unsigned int chunksize = Rcpp::as<unsigned int>(chunksize_);
	Rcpp::XPtr<reader> ptr(
			new reader(path, sep, header, has_row_names, chunksize), true);
	return ptr;
}

//' @param x A single integer.
//' @export
// [[Rcpp::export]]
RcppExport bool reader__set_header(SEXP ptr) {
	Rcpp::XPtr<reader> data(ptr);
	data->set_header();
	return true;
}

//' @param x A single integer.
//' @export
// [[Rcpp::export]]
RcppExport bool reader__next_chunk(SEXP ptr) {
	Rcpp::XPtr<reader> data(ptr);
	return data->next_chunk();
}

//' @param x A single integer.
//' @export
// [[Rcpp::export]]
RcppExport StringVector reader__get_header(SEXP ptr) {
	Rcpp::XPtr<reader> data(ptr);
	return data->get_header();
}

//' @param x A single integer.
//' @export
// [[Rcpp::export]]
RcppExport StringMatrix reader__get_data(SEXP ptr) {
	Rcpp::XPtr<reader> data(ptr);
	return data->get_data();
}

//' @param x A single integer.
//' @export
// [[Rcpp::export]]
RcppExport unsigned int reader__get_completed(SEXP ptr) {
	Rcpp::XPtr<reader> data(ptr);
	return data->get_completed();
}
