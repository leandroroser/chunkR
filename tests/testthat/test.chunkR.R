context("test_chunkR.R")

test_that("chunkR works", {
 tmp <- tempfile()
 data(iris)
 write.table(iris, tmp, quote = FALSE)
 
 # test object creation
 my_reader_object <- reader(tmp, chunksize = 30)
 expect_that(my_reader_object, is_a("reader"))
 # test read_chunk
 expect_true(next_chunk(my_reader_object))
 expect_true(next_chunk(my_reader_object))
 
 this_data <- get_matrix(my_reader_object)
 # test get_matrix
 expect_that(this_data, is_a("matrix"))
 
 # test get_completed
 expect_that(get_completed(my_reader_object), is_a("numeric"))

 # thest as_datafrmae
 expect_that(get_dataframe(my_reader_object), is_a("data.frame"))

 # test matrix2df
 expect_that(matrix2df(this_data), is_a("data.frame"))
 
 file.remove(tmp)
 
})

test_that("chunkR is able to read csv files", {
  tmp <- tempfile()
  data(iris)
  write.table(iris, tmp, quote = FALSE, sep = ",")
  
  # test object creation
  my_reader_object <- reader(tmp, chunksize = 30, sep = ",")
  expect_that(my_reader_object, is_a("reader"))
  # test read_chunk
  file.remove(tmp)
})