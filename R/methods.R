
#' reader initializer
#' @aliases initialize,reader-method
#' @keywords internal

setMethod( "initialize", "reader", 
           function(.Object, path_, sep_, has_colnames_, has_rownames_, 
                    chunksize_, data_format_,  columns_classes_) {
             
             path_ <- normalizePath(path_)
             
             if(data_format_ == "matrix") {
               .Object@pointer  <- reader__new_matrix(path_, 
                                                      sep_, 
                                                      has_colnames_, 
                                                      has_rownames_, 
                                                      chunksize_)
             } else {
               .Object@pointer  <- reader__new_data_frame(path_, sep_,
                                                          has_colnames_, 
                                                          has_rownames_,
                                                          chunksize_, 
                                                          columns_classes_)
             }
             .Object
           })

#' Manipulation methods for reader objects
#' @name reader methods
#' @description Reader objects can be manipulated with the following methods:
#' \enumerate{
#' \item{\bold{next_chunk}}{: allows to read the next chunk of a reader object}
#' \item{\bold{get_table}}{: retrieve the current data chunk contained in the object}
#' }
#' 
#' In addition, this information can be retrieved from reader objects:
#' \enumerate{
#' \item{\bold{get_completed}}{: get the number of rows already read}
#' \item{\bold{get_colnames}}{: get column names of the reader object}
#' }
#' 
#' @details See \code{\link{reader}} for examples.
#' 
#' @param obj object of class reader
#' @rdname reader-methods
NULL


#' next_chunk
#' @name next_chunk
#' @description NULL
#' @rdname reader-methods
#' @export
setGeneric("next_chunk", function(obj) standardGeneric("next_chunk"))


#' @rdname reader-methods
#' @aliases next_chunk,reader-methods

setMethod("next_chunk", "reader", function(obj) {
  reader__next_chunk(obj@pointer)
})


#' get_table
#' @name get_table
#' @description NULL
#' @rdname reader-methods
#' @export
setGeneric("get_table", function(obj) standardGeneric("get_table"))


#' @aliases get_dataframe, reader-methods
#' @rdname reader-methods

setMethod("get_table", "reader", function(obj) {
  what_is <- get_type(obj) 
  if(what_is == "data.frame") {
    reader__get_dataframe(obj@pointer)
  } else {
    reader__get_matrix(obj@pointer)
  }
})


#' get_colnames
#' @name get_colnames
#' @rdname reader-methods
#' @description NULL
#' @export

setGeneric("get_colnames", function(obj) standardGeneric("get_colnames"))


#' @aliases get_colnames,reader-method
#' @rdname reader-methods

setMethod("get_colnames", "reader", function(obj) {
  reader__get_colnames(obj@pointer)
})


#' get_completed
#' @name get_completed
#' @description NULL
#' @rdname reader-methods
#' @export

setGeneric("get_completed", function(obj) standardGeneric("get_completed"))


#' @aliases get_completed,reader-methods
#' @rdname reader-methods

setMethod("get_completed", "reader", function(obj) {
  reader__get_completed(obj@pointer)
})


#' get_type
#' @name get_type
#' @description NULL
#' @rdname reader-methods
#' @export

setGeneric("get_type", function(obj) standardGeneric("get_type"))


#' @aliases get_type,reader-methods
#' @rdname reader-methods

setMethod("get_type", "reader", function(obj) {
  reader__get_type(obj@pointer)
})


#-------------
# Add documentation to matrix2df

#' matrix2df
#' @description conversion from matrix to DataFrame
#' @param x matrix
#' @export
NULL
