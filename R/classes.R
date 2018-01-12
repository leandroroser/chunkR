
#' reader class
#' @name reader-class
#' @slot pointer externalptr object
#' @keywords internal

setClass( "reader", representation( pointer = "externalptr" ) )