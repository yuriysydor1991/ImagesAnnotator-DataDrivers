cmake_minimum_required(VERSION 3.13)

include(imagesannotator-datadrivers-sanitizers-regular)

set(IADD_EXTRA_COMPILE_OPTIONS 
  ${IADD_EXTRA_COMPILE_OPTIONS}
  -fsanitize=pointer-compare
  -fsanitize=leak 
  -fsanitize=address
)

set(
  IADD_EXTRA_LINK_OPTIONS 
  ${IADD_EXTRA_LINK_OPTIONS} 
  -fsanitize=address
)