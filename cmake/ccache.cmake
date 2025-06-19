# see if we can use ccache
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  message("Using ccache")
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
else()
  message("ccache not found")
endif()
