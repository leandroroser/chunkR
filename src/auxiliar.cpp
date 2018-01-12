#include <Rcpp.h>
using namespace Rcpp;

// Matrix to DataFrame

template<int RTYPE>
DataFrame matrix2df_(Matrix<RTYPE> x) {
	size_t n_col = x.ncol();
	List output(n_col);
	for (int i = 0; i < n_col; ++i) {
		output[i] = x(_, i);
	}
	output.attr("row.names") = rownames(x);
	output.attr("names") = colnames(x);
	output.attr("class") = "data.frame";
	return output;
}

//' matrix2df
//' @name matrix2df
//' @description conversion from matrix to DataFrame
//' @param x matrix

// [[Rcpp::export]]
DataFrame matrix2df(SEXP x) {
	DataFrame output;

	switch (TYPEOF(x)) {
	case STRSXP:
		output = matrix2df_<STRSXP>(x);
		break;
	case REALSXP:
		output = matrix2df_<REALSXP>(x);
		break;
	case INTSXP:
		output = matrix2df_<INTSXP>(x);
		break;
	case LGLSXP:
		output = matrix2df_<LGLSXP>(x);
		break;
	default:
		stop("Unknown SEXP type");
	}
	return output;
}

