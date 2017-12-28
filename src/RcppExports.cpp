// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include "../inst/include/reader.h"
#include <Rcpp.h>
#include <string>
#include <set>

using namespace Rcpp;

// reader__new
RcppExport SEXP reader__new(SEXP path_, SEXP sep_, SEXP has_colnames_, SEXP has_rownames_, SEXP chunksize_);
RcppExport SEXP _reader_reader__new(SEXP path_SEXP, SEXP sep_SEXP, SEXP has_colnames_SEXP, SEXP has_rownames_SEXP, SEXP chunksize_SEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type path_(path_SEXP);
    Rcpp::traits::input_parameter< SEXP >::type sep_(sep_SEXP);
    Rcpp::traits::input_parameter< SEXP >::type has_colnames_(has_colnames_SEXP);
    Rcpp::traits::input_parameter< SEXP >::type has_rownames_(has_rownames_SEXP);
    Rcpp::traits::input_parameter< SEXP >::type chunksize_(chunksize_SEXP);
    rcpp_result_gen = Rcpp::wrap(reader__new(path_, sep_, has_colnames_, has_rownames_, chunksize_));
    return rcpp_result_gen;
END_RCPP
}
// reader__set_colnames
RcppExport bool reader__set_colnames(SEXP ptr);
RcppExport SEXP _reader_reader__set_colnames(SEXP ptrSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type ptr(ptrSEXP);
    rcpp_result_gen = Rcpp::wrap(reader__set_colnames(ptr));
    return rcpp_result_gen;
END_RCPP
}
// reader__next_chunk
RcppExport bool reader__next_chunk(SEXP ptr);
RcppExport SEXP _reader_reader__next_chunk(SEXP ptrSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type ptr(ptrSEXP);
    rcpp_result_gen = Rcpp::wrap(reader__next_chunk(ptr));
    return rcpp_result_gen;
END_RCPP
}
// reader__get_colnames
RcppExport StringVector reader__get_colnames(SEXP ptr);
RcppExport SEXP _reader_reader__get_colnames(SEXP ptrSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type ptr(ptrSEXP);
    rcpp_result_gen = Rcpp::wrap(reader__get_colnames(ptr));
    return rcpp_result_gen;
END_RCPP
}
// reader__get_data
RcppExport StringMatrix reader__get_data(SEXP ptr);
RcppExport SEXP _reader_reader__get_data(SEXP ptrSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type ptr(ptrSEXP);
    rcpp_result_gen = Rcpp::wrap(reader__get_data(ptr));
    return rcpp_result_gen;
END_RCPP
}
// reader__get_completed
RcppExport unsigned int reader__get_completed(SEXP ptr);
RcppExport SEXP _reader_reader__get_completed(SEXP ptrSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type ptr(ptrSEXP);
    rcpp_result_gen = Rcpp::wrap(reader__get_completed(ptr));
    return rcpp_result_gen;
END_RCPP
}

// validate (ensure exported C++ functions exist before calling them)
static int _reader_RcppExport_validate(const char* sig) { 
    static std::set<std::string> signatures;
    if (signatures.empty()) {
    }
    return signatures.find(sig) != signatures.end();
}

// registerCCallable (register entry points for exported C++ functions)
RcppExport SEXP _reader_RcppExport_registerCCallable() { 
    R_RegisterCCallable("reader", "_reader_RcppExport_validate", (DL_FUNC)_reader_RcppExport_validate);
    return R_NilValue;
}

RcppExport SEXP _rcpp_module_boot_reader_module();

static const R_CallMethodDef CallEntries[] = {
    {"_reader_reader__new", (DL_FUNC) &_reader_reader__new, 5},
    {"_reader_reader__set_colnames", (DL_FUNC) &_reader_reader__set_colnames, 1},
    {"_reader_reader__next_chunk", (DL_FUNC) &_reader_reader__next_chunk, 1},
    {"_reader_reader__get_colnames", (DL_FUNC) &_reader_reader__get_colnames, 1},
    {"_reader_reader__get_data", (DL_FUNC) &_reader_reader__get_data, 1},
    {"_reader_reader__get_completed", (DL_FUNC) &_reader_reader__get_completed, 1},
    {"_rcpp_module_boot_reader_module", (DL_FUNC) &_rcpp_module_boot_reader_module, 0},
    {"_reader_RcppExport_registerCCallable", (DL_FUNC) &_reader_RcppExport_registerCCallable, 0},
    {NULL, NULL, 0}
};

RcppExport void R_init_reader(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
