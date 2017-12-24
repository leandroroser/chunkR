.onUnload <- function (libpath) {
  library.dynam.unload("reader", libpath)
}
