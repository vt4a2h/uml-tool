function(setCommonTargetProperties target)
    set_property(TARGET ${target} PROPERTY CXX_STANDARD 17)
    set_property(TARGET ${target} PROPERTY CXX_EXTENSIONS OFF)
endfunction(setCommonTargetProperties)