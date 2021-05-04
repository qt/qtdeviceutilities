add_library(DeviceUtilities INTERFACE)
target_link_libraries(DeviceUtilities INTERFACE Qt::Core)
qt_internal_add_docs(DeviceUtilities
    qtdeviceutilities.qdocconf
)
