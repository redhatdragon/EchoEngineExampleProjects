#include(copyAlteredFiles_impl.cmake)

function(copyAlteredFiles src, dest)
    #Grab list of files...
    file(GLOB ___listOfFiles CONFIGURE_DEPENDS
         "*"
    )

	message("Copying contents: ${src} ${dest}")
    foreach (my_entry IN LISTS ___listOfFiles)
        add_custom_command(
            OUTPUT ${src}/${my_entry}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${src}/${my_entry} ${dest}/${my_entry}
            MAIN_DEPENDENCY ${src}/${my_entry}/${my_entry}
	    )
        message("Copying: ${src} ${dest}")
    endforeach()
endfunction()

copyAlteredFiles(${src} ${dest})