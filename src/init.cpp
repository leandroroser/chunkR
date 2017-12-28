#include <Rcpp.h>
#include <reader.h>
using namespace _reader;
using namespace Rcpp;

//' @param x A single integer.
//' @export
// [[Rcpp::export]]
RcppExport SEXP reader__new(SEXP path_, SEXP sep_, SEXP has_colnames_,
		SEXP has_rownames_, SEXP chunksize_) {

	std::string path = Rcpp::as < std::string > (path_);
	char sep = Rcpp::as<char>(sep_);
	bool has_colnames = Rcpp::as<bool>(has_colnames_);
	bool has_rownames = Rcpp::as<bool>(has_rownames_);
	unsigned int chunksize = Rcpp::as<unsigned int>(chunksize_);
	Rcpp::XPtr<reader> ptr(
			new reader(path, sep, has_colnames, has_rownames, chunksize), true);
	return ptr;
}

//' @param x A single integer.
//' @export
// [[Rcpp::export]]
RcppExport bool reader__set_colnames(SEXP ptr) {
	Rcpp::XPtr<reader> data(ptr);
	data->set_colnames();
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
RcppExport StringVector reader__get_colnames(SEXP ptr) {
	Rcpp::XPtr<reader> data(ptr);
	return data->get_colnames();
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
