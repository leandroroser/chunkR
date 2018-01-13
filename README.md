## chunkR 1.0.0  [![Build Status](https://travis-ci.org/leandroroser/chunkR.svg?branch=master)](https://travis-ci.org/leandroroser/chunkR.svg?branch=master) [![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/leandroroser/chunkR?branch=master&svg=true)](https://ci.appveyor.com/project/leandroroser/chunkR) [![R](https://img.shields.io/badge/R%3E%3D-3.0-red.svg)](https://img.shields.io/badge/R%3E%3D-3.0-red.svg) 

This package allows to read long data tables by chunks in R, using a fast C++ backend. The data is interpreted in R as a character matrix. The program reads each chunk using a file pointer with the next_chunk() function (that return TRUE), and stores it in a structure that is accessible via the get_data() function. In each cycle, a file pointer that indicates the position of the next chunk is updated. After reading all the file, the next_chunk() function returns FALSE and the get_data() function an empty matrix.

### Installation:

```diff
library(devtools)
install_github("leandroroser/chunkR")
```

### Example

```diff

 data(iris)
 
 # write iris as tab delimited file. Note that quote is set to FALSE
 
 tmp_path <- file.path(tempdir(),"iris.txt")
 write.table(iris, tmp_path, quote = FALSE)
 
 # create a 'reader' object passing the path of the input file
 my_reader_object <- reader(tmp_path, chunksize = 30)
 
 # read a chunk
 next_chunk(my_reader_object)
 
 # get the chunk
 get_matrix(my_reader_object)
 
 # read another chunk
 next_chunk(my_reader_object)
 
 # get the number of lines already read
 get_completed(my_reader_object)
 
 
 # store the chunk as a character matrix in R
 this_data <- get_matrix(my_reader_object)
 
 # select only numeric columns
 this_data_numeric <- this_data[, 1:4]
 
 # convert into numeric
 mode(this_data_numeric)<-"numeric"
 
 # Matrix data can be converted to data frame with the C++ method get_dataframe. 
 # , which is much faster than the native function "as.data.frame"
 this_data_as_df <- get_dataframe(my_reader_object)
 
 # The package also provides a fast generic C++ function for conversion from
 # matrix (any R type) to dataframe
 this_data_as_df2 <- matrix2df(this_data)
 
 # remove temporal file
 file.remove(tmp_path)
 
 #--- read a csv file ---#
 tmp_path_csv <- file.path(tempdir(),"iris.txt")
 
 write.table(iris, tmp_path_csv, quote = FALSE, sep = ",")
 
 # read the csv indicating the value of the sep parameter
 my_reader_object <- reader(tmp_path_csv, chunksize = 30, sep = ",")
 # the file can  then be processed as with tab delimiters
 
 # remove temporal file
 file.remove(tmp_path_csv)
 
 
 #--- Example with a large table ---#
 
 # create a large table, and write it in a temporal directory
 
 my_table <- tempfile()
 write.table(matrix(sample(c("a", "t", "c", "g"), 1000000, replace = TRUE), 100000, 1000), my_table)
 
 # create a reader object, reading in chunks of 10000 lines
 my_reader_object_2 <- reader(my_table, chunksize = 10000)
 
 # create a loop to read all the file and make something with it
 
 lines <- 0
 while(next_chunk(my_reader_object_2))
 {
   data <- get_matrix(my_reader_object_2) 
   
   
   # do something with data, e.g., convert to data frame first
   data <- matrix2df(data)
   
   lines <- lines + nrow(data)
   cat("Processed ", lines, "lines\n")
 }
 
 # remove the temporal file
 file.remove(my_table)


```
