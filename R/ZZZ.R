
#Rcpp::loadModule("reader", TRUE, loadNow = TRUE)

.onUnload <- function (libpath) {
  library.dynam.unload("chunkR", libpath)
}
