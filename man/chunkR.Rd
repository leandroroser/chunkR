\name{chunkR-package}
\alias{chunkR-package}
\alias{chunkR}
\docType{package}
\title{
 Read Tables in Chunks
}
\description{
  Read external data tables in chunks using a C++ backend.
}

\details{
 This package allows to read long data tables by chunks in R, using a fast C++ backend. 
 
The objects of class "reader" are the central elements of the chunkR package. These objects can store a data chunk and other information required for the process of reading a file in pieces via C++. A reader object is created with the \code{\link{reader}} function, that requires the path to a file, and other arguments, as the size of the chunk. Two basic methods are defined to manipulate the object:

- \code{\link{next_chunk}} function to read the next chunk

- \code{\link{get_table}} to retrieve the data


The functions \code{\link{get_completed}} and \code{\link{get_colnames}} allow to get the number of rows already read, and the column names of the table.

This package allows to read long data tables by chunks in R, using a fast C++ backend. The program can read dataframe (with column type detection) or matrix data. The program reads each chunk with the \code{\link{next_chunk}} function (that return TRUE), and makes it accessible via the \code{\link{get_table}} function. After reading all the file, \code{\link{next_chunk}} function returns FALSE and the \code{\link{get_table}} function an empty dataframe/matrix.
 
}
\author{
Leandro Roser
 
 Maintainer: Leandro Roser <learoser@gmail.com>
}

\keyword{ package }
\examples{
data(iris)
 
 # write iris as tab delimited file. Note that quote is set to FALSE
 
 tmp_path <- file.path(tempdir(),"iris.txt")
 write.table(iris, tmp_path, quote = FALSE)
 
 #----------------------------------------------------------------#
 #--- Reading a dataframe with automatic column-type detection ---#
 #----------------------------------------------------------------#
 
 # create a 'reader' object passing the path of the input file.
 my_reader_object <- reader(tmp_path, chunksize = 30)
 
 # read a chunk
 next_chunk(my_reader_object)
 
 # get the chunk
# get_table(my_reader_object)
 
 # read another chunk
 next_chunk(my_reader_object)
 
 # get the number of lines already read
 get_completed(my_reader_object)
 
 
 #--- read a csv file ---#
 
 tmp_path_csv <- file.path(tempdir(),"iris.csv")
 
 write.table(iris, tmp_path_csv, quote = FALSE, sep = ",")
 
 # read the csv indicating the value of the sep parameter
 my_reader_object2 <- reader(tmp_path_csv, chunksize = 30, sep = ",")
 # the file can  then be processed as with tab delimiters
 
 # remove temporal file
 file.remove(tmp_path_csv)
 
 #----------------------------------------------------#
 #--- Reading a dataframe passing the column types ---#
 #----------------------------------------------------#
 
 ## Four types can be passed : "character", "numeric" (aka "double"), "integer", "logical"
 
 # create a 'reader' object passing the path of the input file.
 my_reader_object3 <- reader(tmp_path, chunksize = 120, 
                              columns_classes = c("numeric", "numeric", 
                              "numeric","numeric", "character"))
 
 # read a chunk
 next_chunk(my_reader_object3)
 
 # get the chunk
 get_table(my_reader_object3)
 
 # read another chunk
 next_chunk(my_reader_object3)
 
 # get the number of lines already read
 get_completed(my_reader_object3)
 
 
 #-------------------------#
 #--- Reading a matrix  ---#
 #-------------------------#
 
 my_reader_object4 <- reader(tmp_path, chunksize = 30, data_format= "matrix")
 
 # store the chunk as a character matrix in R
 this_data <- get_table(my_reader_object4)
 
 # Matrix data can be converted to data frame with the C++ method get_dataframe. 
 # , which is much faster than the native function "as.data.frame"
 this_data_as_df <- get_dataframe(my_reader_object4)
 
 # The package also provides a fast generic C++ function for conversion from
 # matrix (any R type) to dataframe
 this_data_as_df2 <- matrix2df(this_data)
 
 # remove temporal file
 file.remove(tmp_path)
 

 \dontrun{ 
 #----------------------------------#
 #--- Example with a big table -----#
 #----------------------------------#
 
 ### Example 2 with a dataframe
 
 # create a large dataframe, and write it in a temporal directory
 
 tmp_path <- file.path(tempdir(),"big_table.txt")
 
 out <- data.frame(numeric_data = runif(1000000),
                   character_data = sample(c("a", "t", "c", "g"), 1000000, 
                   replace = TRUE),
                   integer_data = sample(1000000),
                   bool_data = sample(c(TRUE, FALSE), 1000000, replace = TRUE))
 
 
 write.table(out, tmp_path)
 
 # create a reader object, reading in chunks of 10000 lines
 my_reader_object5 <- reader(tmp_path, chunksize = 10000)
 
 next_chunk(my_reader_object)
 data <- get_table(my_reader_object5) 
 
 # check classes
 lapply(data,typeof)
 file.remove(tmp_path)
 
 
 ### Example 2 with a matrix
 
 # create a large matrix, and write it in a temporal directory
 
 my_table <- tempfile()
 write.table(matrix(sample(c("a", "t", "c", "g"), 1000000, replace = TRUE),
 100000, 1000), my_table)
 
 # create a reader object, reading in chunks of 10000 lines
 my_reader_object6 <- reader(my_table, chunksize = 10000, data_format= "matrix")

 # create a loop to read all the file and make something with it
 
 lines <- 0
 while(next_chunk(my_reader_object6))
 {
   data <- get_table(my_reader_object6) 
   
   # do something with data, e.g., convert to data frame first
   data <- matrix2df(data)
   
   lines <- lines + nrow(data)
   cat("Processed ", lines, "lines\n")
 }
 
 # remove the temporal file
}
}