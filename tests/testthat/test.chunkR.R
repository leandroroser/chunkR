context("test_chunkR.R")

data(iris)
tmp_csv <- tempfile(fileext = ".csv")
write.table(iris, tmp_csv, quote = FALSE, sep = ",")
tmp_txt <- tempfile(fileext = ".txt")
write.table(iris, tmp_txt, quote = FALSE)


test_that("chunkR data.frame autodetection works", {
 
 # test object creation
 my_chunker_object <- chunker(tmp_txt, chunksize = 30)
 expect_that(my_chunker_object, is_a("chunker"))
 
 # test read_chunk
 expect_true(next_chunk(my_chunker_object))
 expect_true(next_chunk(my_chunker_object))
 
 # test get_table
 this_data <- get_table(my_chunker_object)
 expect_that(this_data, is_a("data.frame"))
 
 # test get_completed
 expect_that(get_completed(my_chunker_object), is_a("numeric"))

})


test_that("chunkR data.frame custom detection works", {
 
  # test object creation
  my_chunker_object <- chunker(tmp_txt, chunksize = 120, 
                             columns_classes = c("numeric", "numeric", "numeric","numeric", "character"))

  # test object creation
  expect_that(my_chunker_object, is_a("chunker"))
  
  # test next_chunk
  expect_true(next_chunk(my_chunker_object))
  expect_true(next_chunk(my_chunker_object))
  
  # test get_table
  this_data <- get_table(my_chunker_object)
  expect_that(this_data, is_a("data.frame"))
  
  # test get_completed
  expect_that(get_completed(my_chunker_object), is_a("numeric"))

})

test_that("chunkR matrix works", {
  
  # test object creation
  my_chunker_object <- chunker(tmp_txt, chunksize = 120, data_format = "matrix")
  expect_that(my_chunker_object, is_a("chunker"))
  
  # test read_chunk
  expect_true(next_chunk(my_chunker_object))
  expect_true(next_chunk(my_chunker_object))
  
  # test get_table
  this_data <- get_table(my_chunker_object)
  expect_that(this_data, is_a("matrix"))
  
  # Matrix data can be converted to data frame with the C++ method get_table. 
  # , which is much faster than the native function "as.data.frame"
  this_data_as_df <- get_table(my_chunker_object)
  
  # The package also provides a fast generic C++ function for conversion from
  # matrix (any R type) to dataframe
  this_data_as_df2 <- matrix2df(this_data)

})


test_that("chunkR is able to read csv files", {
 
  # test object creation
  my_chunker_object <- chunker(tmp_csv, chunksize = 30, sep = ",")
  
  #test next_chunk
  expect_that(my_chunker_object, is_a("chunker"))
  expect_true(next_chunk(my_chunker_object))
  
  this_data <- get_table(my_chunker_object)
  # test get_table
  expect_that(this_data, is_a("data.frame"))
  
})


# remove temporal files
file.remove(tmp_txt)
file.remove(tmp_csv)
