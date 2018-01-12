
#' reader initializer
#' @aliases initialize,reader-method
#' @keywords internal

setMethod( "initialize", "reader", function(.Object, path_, sep_, has_colnames_,
                                            has_rownames_, chunksize_) {
  path_ <- normalizePath(path_)
  .Object@pointer  <- reader__new(path_, sep_, has_colnames_, has_rownames_, chunksize_)
  .Object
} )


#' Manipulation methods for reader objects
#' @name reader methods
#' @description Reader objects can be manipulated with the following methods:
#' \enumerate{
#' \item{\bold{next_chunk}}{: allows to read the next chunk of a reader object}
#' \item{\bold{get_matrix}}{: retrieve the current data chunk contained in the object, as matrix}
#' \item{\bold{get_dataframe}}{: retrieves the current data chunk, as a data frame} 
#' }
#' 
#' In addition, this information can be retrieved from reader objects:
#' \enumerate{
#' \item{\bold{get_completed}}{: get the number of rows already read}
#' \item{\bold{get_colnames}}{: get column names of the reader object}
#' }
#' @param obj object of class reader
#' @rdname reader-methods
NULL


#' next_chunk
#' @name next_chunk
#' @rdname reader-methods
#' @export
setGeneric("next_chunk", function(obj) standardGeneric("next_chunk"))


#' @rdname reader-methods
#' @aliases next_chunk,reader-methods

setMethod("next_chunk", "reader", function(obj) {
  reader__next_chunk(obj@pointer)
})


#' get_matrix
#' @name get_matrix
#' @rdname reader-methods
#' @export

setGeneric("get_matrix", function(obj) standardGeneric("get_matrix"))


#' @rdname reader-methods
#' @aliases get_matrix,reader-methods

setMethod("get_matrix", "reader", function(obj) {
  reader__get_matrix(obj@pointer)
})

#' get_dataframe
#' @name get_dataframe
#' @rdname reader-methods
#' @export

setGeneric("get_dataframe", function(obj) standardGeneric("get_dataframe"))


#' @aliases get_dataframe, reader-methods
#' @rdname reader-methods

setMethod("get_dataframe", "reader", function(obj) {
  reader__get_dataframe(obj@pointer)
})

#' get_completed
#' @name get_completed
#' @rdname reader-methods
#' @export

setGeneric("get_completed", function(obj) standardGeneric("get_completed"))


#' @aliases get_completed,reader-methods
#' @rdname reader-methods

setMethod("get_completed", "reader", function(obj) {
  reader__get_completed(obj@pointer)
})

#' get_colnames
#' @name get_colnames
#' @rdname reader-methods
#' @export

setGeneric("get_colnames", function(obj) standardGeneric("get_colnames"))


#' @aliases get_colnames,reader-method
#' @rdname reader-methods

setMethod("get_colnames", "reader", function(obj) {
  reader__get_colnames(obj@pointer)
})

#-------------
# Add documentation to matrix2df

#' matrix2df
#' @description conversion from matrix to DataFrame
#' @param x matrix
#' @export
NULL


# #' reader_method
# #' @description Method constructor
# #' @param name Name of the method
# #' @keywords internal
# 
# reader_method <- function (name) { 
#   paste ("_chunkR_reader", name, sep = "__")
# }
# 
# #' $
# #' @description Method accessor
# #' @param x reader object
# #' @param name Name of the method
# #' @aliases $,reader-method
# #' @exportMethod 
# 
# # syntactic sugar to allow object$method( ... )
# setMethod("$", "reader", function (x, name) {
#   function (...) .Call (reader_method(name) ,x@pointer, ...)
# } )
