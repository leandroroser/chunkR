
#' reader class
#' @name reader-class
#' @keywords internal
#' 
setClass( "reader", representation( pointer = "externalptr" ) )

#' reader initializer
#' @keywords internal

setMethod( "initialize", "reader", function(.Object,path_, sep_, header_,
                                            has_row_names_, chunksize_) {
  path_ <- normalizePath(path_)
  .Object@pointer  <- .Call(reader_method("new"), path_, sep_, header_,
                            has_row_names_, chunksize_)
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

setGeneric("reader", function(path_, sep_ = " ", header_ = TRUE,
                              has_row_names_ = TRUE, chunksize_ = 10L) {
  new("reader", path_, sep_, header_, has_row_names_, chunksize_)
})


## set_header
## @description 
## @param
## @export

#setGeneric("set_header", function(obj) standardGeneric("set_header"))
#setMethod("set_header", "reader", function(obj) {
#  .Call(reader_method("set_header"), obj@pointer)
#})


#' next_chunk
#' @description Read next data chunk
#' @param obj reader object
#' @export

setGeneric("next_chunk", function(obj) standardGeneric("next_chunk"))
setMethod("next_chunk", "reader", function(obj) {
  .Call(reader_method("next_chunk"), obj@pointer)
})

#' get_header
#' @description get file header
#' @param obj reader object
#' @export

setGeneric("get_header", function(obj) standardGeneric("get_header"))
setMethod("get_header", "reader", function(obj) {
  .Call(reader_method("get_header"), obj@pointer)
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
