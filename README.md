## reader

This package allows to read long data tables by chunks in R, using a fast C++ backend. The program reads each chunk and stores it in a structure that is accessible via R functions. In each cycle, a file pointer that indicates the position of the next chunk is updated.

### Example

```diff
data(iris)
write.table(iris, "iris.txt", quote = FALSE)

# create object passing the file path of the input file
my_reader_object <- reader("iris.txt", chunksize = 30)

# read a chunk
next_chunk(my_reader_object)

# get the chunk
get_data(my_reader_object)

# read another chunk
next_chunk(my_reader_object)

# store the chunk as a character matrix in R
this_data <- get_data(my_reader_object)

# select only numeric columns
this_data <- this_data[, 1:4]

# convert into numeric
mode(this_data)<-"numeric"

```
