
setGeneric("get_matrix2dataframe", function(obj) standardGeneric("get_matrix2dataframe"))

setMethod("get_matrix2dataframe", "reader", function(obj) {
  warning("This function was deprecated in chunkR 1.1.0. 
           Use matrix2df or read the data directly as 
           a dataframe.")
  invisible(NULL)
})


setGeneric("get_dataframe", function(obj) standardGeneric("get_dataframe"))

setMethod("get_dataframe", "reader", function(obj) {
  warning("This function was deprecated in chunkR 1.1.0. 
           Use get_table.")
  invisible(NULL)
})


setGeneric("get_matrix", function(obj) standardGeneric("get_matrix"))

setMethod("get_matrix", "reader", function(obj) {
  warning("This function was deprecated in chunkR 1.1.0. 
           Use get_table.")
  invisible(NULL)
})
