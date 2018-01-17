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
DataFrame NULLdf(0);
const DataFrame empty_df = NULLdf;
std::vector<std::string> autovector;
#define auto_vector autovector

namespace _chunkR {

//' reader__reader
//' @keywords internal

// matrix constructor
reader::reader(const std::string path, char sep, bool has_colnames, bool has_rownames, size_t chunksize) :
path(path), sep(sep), has_colnames(has_colnames),
has_rownames(has_rownames), chunksize(chunksize), n_row(0), n_col(0), output_format("matrix"),
rnames([&chunksize] {std::vector<std::string> out; out.reserve(chunksize); return out;}()), 
cnames([] {std::vector<std::string> out; return out;}()), 
pointer_position(0), line(new std::string), element(new std::string), 
lines_completed(0), temp(auto_vector) {
  
  data_chunk.m = empty_stringm;
  data_chunk.df = empty_df; 
  // configure for data frames
 
  Rcout << "New reader object\n";
  Rcout << "Path: " << path << std::endl;
  set_colnames();
}

// dataframe constructor
reader::reader(const std::string path, char sep, bool has_colnames, bool has_rownames, size_t chunksize,
                StringVector column_types) :
                path(path), sep(sep), has_colnames(has_colnames),
                has_rownames(has_rownames), chunksize(chunksize), n_row(0), n_col(0),
                output_format("data.frame"),
                rnames([&chunksize] {std::vector<std::string> out; out.reserve(chunksize); return out;}()), 
                cnames([] {std::vector<std::string> out; return out;}()), 
                pointer_position(0), line(new std::string), element(new std::string), 
                lines_completed(0), temp(auto_vector) {
  
  // count lines in file
  Rcout << "Counting lines in file... ";
  std::ifstream this_file(path); 
  n_lines = std::count(std::istreambuf_iterator<char>(this_file), 
                       std::istreambuf_iterator<char>(), '\n');
  if(has_colnames) {
    n_lines = n_lines -1;
  }
  Rcout << "done.\n";
  
  data_chunk.m = empty_stringm;
  data_chunk.df = empty_df; 
  // configure for data frames
  size_t df_size = column_types.size();
  if(df_size > 0) {
    std::vector<int>temp;
    temp.reserve(df_size);
    for(size_t i = 0; i < df_size; ++i) {
      if(column_types[i] == "character") {
        temp.push_back(0);
      } else  if(column_types[i] == "double" || column_types[i] == "numeric" ) {
          temp.push_back(1);
        } else if(column_types[i] == "integer") {
          temp.push_back(2);
        } else if(column_types[i] == "logical") {
          temp.push_back(3);
        }
      }
      col_types = temp; 
    }
    
    
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
    // si no hay has_colnames, crear uno default.
  }
  
  //' reader__next_chunk
  //' @keywords internal
  //' 
  bool reader::next_chunk() {
    if(output_format == "matrix") {
      return next_chunk_matrix();
    } else {
      return next_chunk_df();
    }
  }
  
  bool reader::next_chunk_matrix() {
    
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
      data_chunk.m = output;
      return false;
    }
    
    data_chunk.m = output;
    return true;
    
  } else {
    StringMatrix output(0);
    data_chunk.m = output;
    return false;
  }
  
}


bool reader::next_chunk_df() {
 
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
    List output;
    
    if(n_lines  - initial_lines >= chunksize) {
    output = mixed_list(col_types, chunksize);
    } else {
    output = mixed_list(col_types, n_lines - initial_lines); 
    }

    while (std::getline(ifs, *line)) {
      bool is_name = true;

      std::stringstream ss(*line);
      int count_elements = 0;
      while (std::getline(ss, *element, sep)) {
        if (is_name && has_rownames) {
          rnames.push_back(*element);
          is_name = false;
          continue;
        }
        
        if(col_types[count_elements] == 0) {
          std::string temp_out = * element;
          (Rcpp::as<StringVector>(output[count_elements]))[lines_read_chunk] = temp_out;
        } else if(col_types[count_elements] == 1) {
          double temp_out = std::atof((* element).c_str());
          (Rcpp::as<NumericVector>(output[count_elements]))[lines_read_chunk] = temp_out;
        } else if(col_types[count_elements] == 2 || output_format[count_elements++] == 3) {
          int temp_out = std::atoi((* element).c_str());
          (Rcpp::as<IntegerVector>(output[count_elements])[lines_read_chunk]) = temp_out;
        }
        count_elements++;
      }

      n_row++;
      lines_completed++;
      if (++lines_read_chunk >= chunksize) {
        break;
      }
    }

    pointer_position = ifs.tellg();

  
    StringVector cnames_rcpp;
    cnames_rcpp = cnames;
    output.attr("names") = cnames_rcpp;
    
    // set rownames
    StringVector rnames_rcpp;
    if (has_rownames) {
      rnames_rcpp = rnames;
      rnames.clear();
    } else {
      rnames_rcpp = set_generic_rownames("R", initial_lines + 1, lines_read_chunk);
    }
    
    //  if(lines_read_chunk < chunksize) {
    
   // // last chunk
   //  if(lines_read_chunk < chunksize) {
   // 
   //    int count_elements = 0;
   //    for(int i = 0; i < n_col; ++i)  {
   //      for(int j = chunksize - 1; j > lines_read_chunk; j--) {
   //      if(col_types[count_elements] == 0) {
   //        Rcpp::as<StringVector>(output[count_elements]).erase(j);
   //      } else if(col_types[count_elements] == 1) {
   //        Rcpp::as<NumericVector>(output[count_elements]).erase(j);
   //        Rcout << "hola!";
   //      } else if(col_types[count_elements] == 2 || output_format[count_elements++] == 3) {
   //        Rcpp::as<IntegerVector>(output[count_elements]).erase(j);
   //      }
   //      }
   //      count_elements++;
   //    }
   //  }
  
    output.attr("row.names") = rnames_rcpp;
    output.attr("class") = "data.frame";
    
 
    // if(lines_read_chunk < chunksize) {
    //   IntegerVector y({0,5});
    //   Function subset("[.data.frame");
    //   output =  subset(output, y, R_MissingArg);
    //   }
      
    //cnames.clear();
    temp.clear();
    ifs.close();
    
    if (lines_read_chunk == 0) {
      DataFrame output;
      data_chunk.df = output;
      return false;
    }
    
    data_chunk.df = output;
    return true;
    
  } else {
    DataFrame output;
    data_chunk.df = output;
    return false;
  }
  
}

//' reader__get_dataframe
//' @keywords internal

DataFrame reader::get_matrix2dataframe() {
  List output(n_col);
  for (size_t i = 0; i < n_col; ++i) {
    output[i] = data_chunk.m(_, i);
  }
  output.attr("row.names") = rownames(data_chunk.m);
  output.attr("names") = colnames(data_chunk.m);
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


StringMatrix reader::get_matrix() {
    return data_chunk.m;
  } 

DataFrame reader::get_dataframe() {
  return data_chunk.df;
} 

//' reader__get_completed
//' @keywords internal

size_t reader::get_completed() {
  return lines_completed;
}

//' reader__get_completed
//' @keywords internal

const std::string reader::get_type() {
  return output_format;
}


inline List reader::mixed_list(std::vector<int> x,  int howmuch) {
  List output;
  
  for(int i = 0; i < x.size(); ++i) {
    switch (x[i]) {
    case 0:
  {
    StringVector temp(howmuch);
    output.push_back(temp);
    break;
  }
    case 1:
  {
    NumericVector temp1(howmuch);
    output.push_back(temp1);
    break;
  }
    case 2:
  {
    IntegerVector temp2(howmuch);
    output.push_back(temp2);
    break;
  }
    case 3:
  {
    LogicalVector temp3(howmuch);
    output.push_back(temp3);
    break;
  }
    default:
      stop("Unknown type");
    }
  }
  return output;
}


//' validate_string
//' @description Validate the input of a chunkR object. Useful for dispatching
//' @param SEXP* args objects passed to the function
//' @param nargs Number of arguments

bool validate_C1(SEXP* args, int nargs) {
  if( nargs != 6 ) return false;
  return true;
}

bool validate_C2(SEXP* args, int nargs) {
  if( nargs != 5 ) return false;
  return true;
}


// //' chunkR finalizer
// //' @description finalizer of a chunkR object
// //' @param formatG* ptr pointer to chunkR object
// 
// static void reader_finalizer(reader* ptr) {
//   ptr->clear(); // requires a clear() function
//   Rcout << "reader finalizer called...\n";
// }

// reader_module

RCPP_MODULE(reader_module) {
  class_<reader>("reader")
  .constructor<std::string, char, bool, bool, size_t, StringVector >("data.frame constructor", &validate_C1)
  .constructor<std::string, char, bool, bool, size_t>("matrix constructor", &validate_C2)
  .method("set_colnames", &reader::set_colnames)
  .method("get_colnames", &reader::get_colnames)
  .method("next_chunk", &reader::next_chunk)
  .method("get_matrix", &reader::get_matrix)
  .method("get_dataframe", &reader::get_dataframe)
  .method("get_matrix2dataframe", &reader::get_matrix2dataframe)
  .method("get_completed", &reader::get_completed)
  .method("get_type", &reader::get_completed)
  ;
}

} /* namespace _chunkR */
