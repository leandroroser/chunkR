
#' reader class
#' @name reader-class
#' @keywords internal
#' 
setClass( "reader", representation( pointer = "externalptr" ) )

#' reader initializer
#' @keywords internal

setMethod( "initialize", "reader", function(.Object,path_, sep_, has_colnames_,
                                            has_rownames_, chunksize_) {
  path_ <- normalizePath(path_)
  .Object@pointer  <- .Call(reader_method("new"), path_, sep_, has_colnames_,
                            has_rownames_, chunksize_)
  .Object
} )


#' reader_method
#' @description Method constructor
#' @param name Name of the method
#' @keywords internal

reader_method <- function (name) { 
  paste ("_reader_reader", name, sep = "__")
}

#' $
#' @description Method accessor
#' @param x reader object
#' @param name Name of the method
#' @keywords internal
#' @export

# syntactic sugar to allow object$method( ... )
setMethod("$", "reader", function (x, name) {
  function (...) .Call (reader_method(name) ,x@pointer, ...)
} )


#' reader
#' @description reader constructor
#' @export

setGeneric("reader", function(path, sep = " ", has_colnames = TRUE,
                              has_rownames = TRUE, chunksize = 1000L) {
  new("reader", path, sep, has_colnames, has_rownames, chunksize)
})


## set_colnames
## @description 
## @param
## @export

#setGeneric("set_colnames", function(obj) standardGeneric("set_colnames"))
#setMethod("set_colnames", "reader", function(obj) {
#  .Call(reader_method("set_colnames"), obj@pointer)
#})


#' next_chunk
#' @description Read next data chunk
#' @param obj reader object
#' @export

setGeneric("next_chunk", function(obj) standardGeneric("next_chunk"))
setMethod("next_chunk", "reader", function(obj) {
  .Call(reader_method("next_chunk"), obj@pointer)
})

#' get_colnames
#' @description get file has_colnames
#' @param obj reader object
#' @export

setGeneric("get_colnames", function(obj) standardGeneric("get_colnames"))
setMethod("get_colnames", "reader", function(obj) {
  .Call(reader_method("get_colnames"), obj@pointer)
})


#' get_data
#' @description get data chunk
#' @param obj reader object
#' @export

setGeneric("get_data", function(obj) standardGeneric("get_data"))
setMethod("get_data", "reader", function(obj) {
  .Call(reader_method("get_data"), obj@pointer)
})

#' get_completed
#' @description get_number_lines_completed
#' @param obj reader object
#' @export

setGeneric("get_completed", function(obj) standardGeneric("get_completed"))
setMethod("get_completed", "reader", function(obj) {
  .Call(reader_method("get_completed"), obj@pointer)
})


#' as_dataframe
#' @description as_dataframe
#' @param obj reader object
#' @export

setGeneric("as_dataframe", function(obj) standardGeneric("as_dataframe"))
setMethod("as_dataframe", "reader", function(obj) {
  .Call(reader_method("as_dataframe"), obj@pointer)
})

