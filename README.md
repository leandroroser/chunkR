
# chunkR 1.1.0  <img src  = "https://github.com/leandroroser/chunkR/blob/master/inst/extdata/icon.png" width="250px" align="middle" padding-left="50px" alt = "chunkR icon"> 


| [Linux][lin-link] | [Windows][win-link] | [R][R-link]          |
| :---------------: | :-----------------: | :-------------------:|
| ![lin-badge]      | ![win-badge]        | ![R-badge]           |

[lin-badge]: https://travis-ci.org/leandroroser/EcoGenetics-devel.svg?branch=master "Travis build status"
[lin-link]:  https://travis-ci.org/leandroroser/chunkR.svg?branch=master "Travis build status"
[win-badge]: https://ci.appveyor.com/api/projects/status/github/leandroroser/chunkR?branch=master&svg=true "AppVeyor build status"
[win-link]: https://ci.appveyor.com/project/leandroroser/chunkR "AppVeyor build status"
[R-badge]: https://img.shields.io/badge/R%3E%3D-3.0-red.svg "R site"
[R-link]:  https://cran.r-project.org/web/packages/chunkR/index.html "R site"


<br/>
This package allows to read long data tables by chunks in R, using a fast C++ backend. The program can read dataframe (with column type detection) or matrix data. The program reads each chunk with the next_chunk() function (that return TRUE), and makes it accessible via the get_table() function. After reading all the file, the next_chunk() function returns FALSE and the get_data() function an empty dataframe/matrix.

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
 
 #-------------------------------------------------------#
 #--- Reading a dataframe using column types argument ---#
 #-------------------------------------------------------#
 
 ## Four types can be passed : "character", "numeric" (aka "double"), "integer", "logical"
 
 # create a 'reader' object passing the path of the input file.
 my_reader_object3 <- reader("iris.txt", chunksize = 120, columns_classes = c("numeric", "numeric", "numeric","numeric", "character"))
 
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
 
 
 # The package provides a fast generic C++ function for conversion from
 # matrix (any R type) to dataframe
 this_data_as_df2 <- matrix2df(this_data)
 
 # remove temporal file
 file.remove(tmp_path)
 

 #----------------------------------#
 #--- Example with a big table -----#
 #----------------------------------#
 
 ### Example 2 with a dataframe
 
 # create a large dataframe, and write it in a temporal directory
 
 tmp_path <- file.path(tempdir(),"big_table.txt")
 
 out <- data.frame(numeric_data = runif(1000000),
                   character_data = sample(c("a", "t", "c", "g"), 1000000, replace = TRUE),
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
 write.table(matrix(sample(c("a", "t", "c", "g"), 1000000, replace = TRUE), 100000, 1000), my_table)
 
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
 file.remove(my_table)

```
